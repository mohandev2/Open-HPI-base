/*
 *
 * Copyright (c) 2003,3004 by FORCE Computers
 *
 * Note that this file is based on parts of OpenIPMI
 * written by Corey Minyard <minyard@mvista.com>
 * of MontaVista Software. Corey's code was helpful
 * and many thanks go to him. He gave the permission
 * to use this code in OpenHPI under BSD license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     Thomas Kanngieser <thomas.kanngieser@fci.com>
 */

#include <errno.h>
#include <assert.h>

#include "ipmi.h"
#include "ipmi_utils.h"


cIpmiDomain::cIpmiDomain()
  : m_con( 0 ), m_is_atca( false ), m_main_sdrs( 0 ),
    m_sensors_in_main_sdr( 0 ), m_sensors_in_main_sdr_count( 0 ), 
    m_major_version( 0 ), m_minor_version( 0 ), m_sdr_repository_support( false ),
    m_si_mc( 0 ), m_mcs( 0 ),
    m_initial_discover( 0 ),
    m_mc_poll_interval( dIpmiMcPollInterval ),
    m_sel_rescan_interval( dIpmiSelQueryInterval ),
    m_entities( 0 )
{
  cIpmiMcVendorFactory::InitFactory();

  for( int i = 0; i < 256; i++ )
     {
       m_mc_to_check[i] = 0;
       m_mc_type[i]     = 0;
       m_mc_thread[i]   = 0;
       m_atca_site_property[i].m_property = 0;
       m_atca_site_property[i].m_max_side_id = 0;
       m_atca_site_property[i].m_mc_type = 0;
     }

  // scan at least at 0x20
  AddMcToScan( 0x20, dIpmiMcThreadInitialDiscover|dIpmiMcThreadPollAliveMc, 0 );

  // default site type properties
  unsigned int prop =   dIpmiMcThreadInitialDiscover
                      | dIpmiMcThreadPollAliveMc
                      | dIpmiMcThreadCreateM0;

  // atca board
  SetAtcaSiteProperty( eIpmiAtcaSiteTypeAtcaBoard, prop, 32, dIpmiMcTypeBitBoard );

  // power
  SetAtcaSiteProperty( eIpmiAtcaSiteTypePowerEntryModule, prop, 8, dIpmiMcTypeBitPower );

  // shelf fru information
  SetAtcaSiteProperty( eIpmiAtcaSiteTypeShelfFruInformation, prop, 4, 0 );

  // dedicated ShMc
  SetAtcaSiteProperty( eIpmiAtcaSiteTypeDedicatedShMc, prop, 2, 0 );

  // fan tray
  SetAtcaSiteProperty( eIpmiAtcaSiteTypeFanTray, prop, 8, dIpmiMcTypeBitFan );

  // fan filter tray
  SetAtcaSiteProperty( eIpmiAtcaSiteTypeFanFilterTray, prop, 8, 0 );

  // alarm
  SetAtcaSiteProperty( eIpmiAtcaSiteTypeAtcaModule, prop, 32, 0 );

  // atca module
  SetAtcaSiteProperty( eIpmiAtcaSiteTypePMC, prop, 32, 0 );

  // rear transition module
  SetAtcaSiteProperty( eIpmiAtcaSiteTypeRearTransitionModule, prop, 4, 0 );
}


cIpmiDomain::~cIpmiDomain()
{
  cIpmiMcVendorFactory::CleanupFactory();  
}


bool
cIpmiDomain::Init( cIpmiCon *con )
{
  assert( m_con == 0 );
  m_con = con;

  m_entities = new cIpmiEntityInfo( this );

  if ( m_entities == 0 )
     {
       IpmiLog( "cannot create entity into !\n" );
       return false;
     }

  cIpmiAddr si( eIpmiAddrTypeSystemInterface);

  m_si_mc = new cIpmiMc( this, si );

  if ( m_si_mc == 0 )
     {
       IpmiLog( "cannot create system interface !\n" );
       return false;
     }

  m_main_sdrs = new cIpmiSdrs( m_si_mc, 0, 0 );
  assert( m_main_sdrs );

  // send get device id
  cIpmiMsg msg( eIpmiNetfnApp, eIpmiCmdGetDeviceId );
  cIpmiMsg rsp;

  int rv = m_si_mc->SendCommand( msg, rsp );

  if ( rv )
     {
       IpmiLog( "cannot send IPMI get device id %d, %s !\n", rv, strerror( rv ) );
       return false;
     }

  if (    rsp.m_data[0] != 0
       || rsp.m_data_len < 12 )
     {
       // At least the get device id has to work.
       IpmiLog( "get device id fails 0x%02x !\n", rsp.m_data[0] );

       return false;
     }

  m_major_version          = rsp.m_data[5] & 0xf;
  m_minor_version          = (rsp.m_data[5] >> 4) & 0xf;
  m_sdr_repository_support = (rsp.m_data[6] & 0x02) == 0x02;

  m_si_mc->SdrRepositorySupport() = m_sdr_repository_support;

  if ( m_major_version < 1 )
     {
       // We only support 1.0 and greater.
       IpmiLog( "ipmi version %d.%d not supported !\n",
                m_major_version, m_minor_version );

       return false;
     }

  // set vendor
  unsigned int mid =    rsp.m_data[7]
                     | (rsp.m_data[8] << 8)
                     | (rsp.m_data[9] << 16);

  unsigned int pid = IpmiGetUint16( rsp.m_data + 10 );

  cIpmiMcVendor *mv = cIpmiMcVendorFactory::GetFactory()->Get( mid, pid );
  m_si_mc->SetVendor( mv );

  if ( mv->Init( m_si_mc, rsp ) == false )
     {
       IpmiLog( "cannot initialize system interface !\n" );

       return false;
     }

  // Check the number of outstanding requests.
  // This is fine tuning if the BMC/ShMc has
  // a block transfer interface.
  // If not supported use the defaults
  // given in openhpi.conf.
  msg.m_netfn = eIpmiNetfnApp;
  msg.m_cmd   = eIpmiCmdGetBtInterfaceCapabilities;
  msg.m_data_len = 0;

  rv = m_si_mc->SendCommand( msg, rsp, 0, 1 );

  // ignore on error
  if ( !rv && rsp.m_data[0] == 0 && rsp.m_data_len >= 6 )
     {
       int num = rsp.m_data[1];

       IpmiLog( "reading bt capabilities: max outstanding %d, input %d, output %d, retries %d.\n",
                num, rsp.m_data[2], rsp.m_data[3], rsp.m_data[5] );

       // check 
       if ( num < 1 )
            num = 1;

       if ( num > 32 )
            num = 32;

       IpmiLog( "max number of outstanding = %d.\n", num );
       m_con->SetMaxOutstanding( num );
     }

  // check for ATCA an modify m_mc_to_check
  CheckAtca();

  if ( m_sdr_repository_support )
     {
       IpmiLog( "reading repository SDR.\n" );

       rv = m_main_sdrs->Fetch();

       if ( rv )
            // Just report an error, it shouldn't be a big deal if this
            // fails.
            IpmiLog( "Could not get main SDRs, error 0x%02x.\n", rv );
       else if ( !m_is_atca )
          {
            // for each mc device locator record in main repository
            // create an entry in m_mc_to_check.
            for( unsigned int i = 0; i < m_main_sdrs->NumSdrs(); i++ )
               {
                 cIpmiSdr *sdr = m_main_sdrs->Sdr( i );

                 if ( sdr->m_type != eSdrTypeMcDeviceLocatorRecord )
                      continue;

                 unsigned char addr = sdr->m_data[5];

                 // already set by openhpi.conf
                 if ( m_mc_to_check[addr] )
                      continue;

                 AddMcToScan( addr,   dIpmiMcThreadInitialDiscover
                                    | dIpmiMcThreadPollDeadMc
                                    | dIpmiMcThreadPollAliveMc, m_mc_type[addr] );
               }
          }
     }

  rv = GetChannels();

  if ( rv )
       return false;

  // Start all MC threads with the
  // properties found in m_mc_to_check.
  m_initial_discover = 0;
  m_num_mc_threads   = 0;

  for( int i = 0; i < 256; i++ )
     {
       assert( m_mc_thread[i] == 0 );

       if ( m_mc_to_check[i] == 0 )
            continue;

       m_mc_thread[i] = new cIpmiMcThread( this, i, m_mc_to_check[i], m_mc_type[i] );

       // Count MC thread with initial discover.
       // This counter is used in cIpmi::IfDiscoverResources
       // to wait until discover is done
       if ( m_mc_to_check[i] & dIpmiMcThreadInitialDiscover )
          {
            m_initial_discover_lock.Lock();
            m_initial_discover++;
            m_initial_discover_lock.Unlock();
          }

       m_mc_thread[i]->Start();
     }

  return true;
}


void
cIpmiDomain::Cleanup()
{
  unsigned int i;

  // stop MC threads
  for( i = 0; i < 256; i++ )
       if ( m_mc_thread[i] )
            m_mc_thread[i]->m_exit = true;

  // wait until all threads are finish
  bool loop = true;

  while( loop )
     {
       m_mc_thread_lock.Lock();
       loop = (bool)m_num_mc_threads;
       m_mc_thread_lock.Unlock();

       usleep( 100000 );
     }

  // wait for threads exit
  for( i = 0; i < 256; i++ )
       if ( m_mc_thread[i] )
          {
            void *rv;

            m_mc_thread[i]->Wait( rv );

            delete m_mc_thread[i];
            m_mc_thread[i] = 0;
          }

  // stop reader thread
  if ( m_con && m_con->IsOpen() )
       m_con->Close();

  // Delete the sensors from the main SDR repository.
  if ( m_sensors_in_main_sdr )
     {
       for( i = 0; i < m_sensors_in_main_sdr_count; i++ )
	    if ( m_sensors_in_main_sdr[i] )
                 m_sensors_in_main_sdr[i]->Destroy();

       delete [] m_sensors_in_main_sdr;
    }

  // cleanup all MCs
  GList *l = g_list_first( m_mcs );

  while( l )
     {
       GList *n = g_list_next( l );

       cIpmiMc *mc = (cIpmiMc *)l->data;
       CleanupMc( mc );

       l = n;
     }

  // now all mc's are ready to destroy
  l = g_list_first( m_mcs );

  while( l )
     {
       GList *n = g_list_next( l );

       cIpmiMc *mc = (cIpmiMc *)l->data;
       if ( CleanupMc( mc ) == false )
            assert( 0 );

       l = n;
     }

  // destroy si
  if ( m_si_mc )
     {
       bool rr = m_si_mc->Cleanup();
       assert( rr );
       delete m_si_mc;
       m_si_mc = 0;
     }

  /* Destroy the main SDR repository, if it exists. */
  if ( m_main_sdrs )
     {
       delete m_main_sdrs;
       m_main_sdrs = 0;
     }
  
  if ( m_entities )
     {
       delete m_entities;
       m_entities = 0;
     }
}


cIpmiMc *
cIpmiDomain::NewMc( const cIpmiAddr &addr )
{
  cIpmiMc *mc = new cIpmiMc( this, addr );
  m_mcs = g_list_append( m_mcs, mc );

  return mc;
}


bool
cIpmiDomain::CleanupMc( cIpmiMc *mc )
{
  if ( !mc->Cleanup() )
       return false;

  m_mcs = g_list_remove( m_mcs, mc );
  delete mc;

  return true;
}


int 
cIpmiDomain::GetChannels()
{
  int rv = 0;

  return rv;
}


int
cIpmiDomain::CheckAtca()
{
  cIpmiMsg msg( eIpmiNetfnPicmg, eIpmiCmdGetPicMgProperties );
  msg.m_data_len = 1;
  msg.m_data[0]  = dIpmiPigMgId;

  cIpmiMsg rsp;
  int      rv;
  int i;

  m_is_atca = false;

  assert( m_si_mc );

  IpmiLog( "checking for ATCA system.\n" );

  rv = m_si_mc->SendCommand( msg, rsp );

  if ( rv || rsp.m_data[0] || rsp.m_data[1] != dIpmiPigMgId )
     {
       IpmiLog( "not an ATCA system.\n" );

       return rv;
     }

  unsigned char minor = (rsp.m_data[2] >> 4) & 0x0f;
  unsigned char major = rsp.m_data[2] & 0x0f;

  IpmiLog( "found a PigMg system version %d.%d.\n",
           major, minor );

  if ( major != 2 || minor != 0 )
       return 0;

  IpmiLog( "found an ATCA system.\n" );

  // use atca timeout
  m_con->m_timeout = m_con->m_default_atca_timeout;

  m_is_atca = true;

  // read all fru addr
  msg.m_netfn   = eIpmiNetfnPicmg;
  msg.m_cmd     = eIpmiCmdGetAddressInfo;
  msg.m_data[0] = dIpmiPigMgId;
  msg.m_data[1] = 0; // fru id 0
  msg.m_data[2] = 0x03; // physical addr
  msg.m_data_len = 5;

  static const char *map[] =
  {
    "ATCA Board",
    "Power Entry Module",
    "Shelf FRU Information",
    "Dedicated ShMc",
    "Fan Tray",
    "Fan Filter Tray",
    "Alarm",
    "ATCA Module",
    "PMC",
    "Rear Transition Module"
  };

  static int map_num = sizeof( map ) / sizeof( char * );

  for( i = 0; i < 256; i++ )
     {
       if ( !m_atca_site_property[i].m_property )
            continue;

       if ( i < map_num )
            IpmiLog( "checking for %s.\n", map[i] );
       else
            IpmiLog( "checking for %x.\n", i );

       for( int j = 0; j < m_atca_site_property[i].m_max_side_id; j++ )
          {
            msg.m_data[3] = j + 1;
            msg.m_data[4] = i;

            rv = m_si_mc->SendCommand( msg, rsp );

            if ( rv )
               {
                 IpmiLog( "cannot send get address info: %d\n", rv );
                 break;
               }

            if ( rsp.m_data[0] )
                 break;

            if ( i < map_num )
                 IpmiLog( "\tfound %s at 0x%02x.\n", map[i], rsp.m_data[3] );
            else
                 IpmiLog( "\tfound %x at 0x%02x.\n", i, rsp.m_data[3] );

            // add slot for initial scan
            AddMcToScan( rsp.m_data[3], m_atca_site_property[i].m_property, m_atca_site_property[i].m_mc_type );
          }
     }

  return 0;
}


cIpmiMc *
cIpmiDomain::FindMcByAddr( const cIpmiAddr &addr )
{
  GList *l;

  if (    ( addr.m_type == eIpmiAddrTypeSystemInterface )
       && ( addr.m_channel == dIpmiBmcChannel ) )
       return m_si_mc;

  l = 0;

  for( l = g_list_first( m_mcs ); l; l = g_list_next( l ) )
     {
       cIpmiMc *mc = (cIpmiMc *)l->data;

       if ( addr == mc->Addr() )
            return mc;
     }

  return 0;
}


int
cIpmiDomain::SendCommand( const cIpmiAddr &addr, const cIpmiMsg &msg,
                          cIpmiMsg &rsp_msg, int retries )
{
  if ( m_con == 0 )
       return ENOSYS;

  return m_con->ExecuteCmd( addr, msg, rsp_msg, retries );
}


cIpmiMc *
cIpmiDomain::FindOrCreateMcBySlaveAddr( unsigned int slave_addr )
{
  cIpmiMc   *mc;
  cIpmiAddr addr( eIpmiAddrTypeIpmb, 0, 0, slave_addr );

  mc = FindMcByAddr( addr );

  if ( mc )
       return mc;

  mc = NewMc( addr );
  mc->SetActive( false );

  return mc;
}


cIpmiSensor **
cIpmiDomain::GetSdrSensors( cIpmiMc  *mc,
                            unsigned int &count )
{
  if ( mc )
       return mc->GetSdrSensors( count );

  count = m_sensors_in_main_sdr_count;
  return m_sensors_in_main_sdr;
}


void
cIpmiDomain::SetSdrSensors( cIpmiMc      *mc,
                            cIpmiSensor **sensors,
                            unsigned int  count )
{
  if ( mc )
       mc->SetSdrSensors( sensors, count );
  else
     {
       m_sensors_in_main_sdr       = sensors;
       m_sensors_in_main_sdr_count = count;
     }
}


cIpmiMc *
cIpmiDomain::GetEventRcvr()
{
  GList *l = m_mcs;

  while( l )
     {
       cIpmiMc *mc = (cIpmiMc *)l->data;

       if ( mc->SelDeviceSupport() )
            return mc /*->GetAddress()*/;

       l = g_list_next( l );
     }

  return 0;
}


void
cIpmiDomain::HandleEvents( GList *list )
{
  while( list )
     {
       cIpmiEvent *event = (cIpmiEvent *)list->data;
       list = g_list_remove( list, event );

       HandleEvent( event );
     }
}


void
cIpmiDomain::HandleAsyncEvent( const cIpmiAddr &addr, const cIpmiMsg &msg )
{
  // It came from an MC, so find the MC.
  cIpmiMc *mc = FindMcByAddr( addr );

  if ( !mc )
     {
       IpmiLog( "cannot find mc for event !\n" );
       return;
     }

  cIpmiEvent *event = new cIpmiEvent;

  event->m_mc        = mc;
  event->m_record_id = IpmiGetUint16( msg.m_data );
  event->m_type      = msg.m_data[2];
  memcpy( event->m_data, msg.m_data + 3, dIpmiMaxSelData );

  // Add it to the MCs event log.
  mc->Sel()->AddAsyncEvent( event );

  HandleEvent( event );
}


void
cIpmiDomain::HandleEvent( cIpmiEvent *event )
{
  // find MC thread
  unsigned char a = event->m_data[4];

  bool hotswap = ( event->m_data[7] == eIpmiSensorTypeAtcaHotSwap ) ? true : false;

  // if there is no MC thread => create MC thread
  if ( m_mc_thread[a] == 0 )
     {
       // start MC thread
       m_mc_thread[a] = new cIpmiMcThread( this, a,
                                             dIpmiMcThreadInitialDiscover
                                           | hotswap ? (   dIpmiMcThreadPollAliveMc
                                                         | dIpmiMcThreadCreateM0) : 0,
                                           dIpmiMcTypeBitAll );

       m_mc_thread[a]->Start();
     }

  m_mc_thread[a]->AddEvent( event );
}


cIpmiEntity *
cIpmiDomain::VerifyEntity( cIpmiEntity *ent )
{
  return m_entities->VerifyEntify( ent );
}


cIpmiMc *
cIpmiDomain::VerifyMc( cIpmiMc *mc )
{
  if ( m_si_mc == mc )
       return mc;

  if ( g_list_find( m_mcs, mc ) )
       return mc;

  return 0;
}


cIpmiSensor *
cIpmiDomain::VerifySensor( cIpmiSensor *s )
{
  return m_entities->VerifySensor( s );
}


cIpmiControl *
cIpmiDomain::VerifyControl( cIpmiControl *c )
{
  return m_entities->VerifyControl( c );
}


cIpmiFru *
cIpmiDomain::VerifyFru( cIpmiFru *f )
{
  return m_entities->VerifyFru( f );
}
