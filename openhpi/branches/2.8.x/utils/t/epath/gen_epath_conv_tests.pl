#!/usr/bin/perl

use strict;

my $numtests = 99;
my $el = shift;

if($el eq "--list") {
    for my $n (0..$numtests) {
        my $file = sprintf("epath_conv_%3.3d.t",$n);
        print "$file ";
    }
} elsif($el eq "--make") {
    my $final;
    my $list;
    for my $n (0..$numtests) {
        $list .= sprintf("\\\n\tepath_conv_%3.3d ",$n);
        $final .= sprintf("epath_conv_%3.3d_SOURCES = epath_conv_%3.3d.c \$(REMOTE_SOURCES)\n",$n,$n);
    }
    print "TESTS = $list\n\n";
    print "check_PROGRAMS = $list\n\n";
    print $final;
} else {

    my @ents = ent_list();
    
    for my $n (0..$numtests) {
        my $file = sprintf("epath_conv_%3.3d.c",$n);
        open(OUT,">$file");
        my $str = gen_epath_test($ents[int(rand(scalar(@ents)))],int(rand(100)),
                                 $ents[int(rand(scalar(@ents)))],int(rand(100)));
        print OUT $str;
        close(OUT);
    }
}

sub ent_list {
    my $file = "../../../include/SaHpi.h";
    my @ents = ();
    open(IN,"<$file");
    while(<IN>) {
        if(/^\s+SAHPI_ENT_([\_\w]+)/) {
            if($1 ne "ROOT") {
                    push @ents, $1;
	   }	     
        }
    }
    return @ents;
}

sub gen_epath_test {
    my ($ent1, $inst1, $ent2, $inst2) = @_;
    return <<END;
/* -*- linux-c -*-
 * 
 * (C) Copyright IBM Corp. 2004
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 */

/******************************************************************* 
 * WARNING! This file is auto-magically generated by:
 *          $0.
 *          Do not change this file manually. Update script instead
 *******************************************************************/

#include <string.h>

#include <SaHpi.h>
#include <oh_utils.h>

/**
 * main: 
 * epathstr -> epath test
 * 
 * Test if an entity path string is converted properly into an entity path.
 **/
int main(int argc, char **argv) 
{
        char *test_string = "{$ent1,$inst1}{$ent2,$inst2}";
        oh_big_textbuffer bigbuf;
	SaErrorT err;
        SaHpiEntityPathT ep;
        
	err = oh_encode_entitypath(test_string, &ep);
	if (err) {
		printf("  Error! Testcase failed. Line=%d\\n", __LINE__);
		printf("  Received error=%s\\n", oh_lookup_error(err));
		return -1;
	}
         
        if (ep.Entry[0].EntityType != SAHPI_ENT_$ent2) {
	    printf("  Error! Testcase failed. Line=%d\\n", __LINE__);
	    printf("  Received=%d; Expected=%d\\n", ep.Entry[0].EntityType, SAHPI_ENT_$ent2);
	    return -1;
	}
                
        if (ep.Entry[0].EntityLocation != $inst2) {
	    printf("  Error! Testcase failed. Line=%d\\n", __LINE__);
	    printf("  Received=%d; Expected=%d\\n", ep.Entry[0].EntityLocation, $inst2);
	    return -1;
	}
        
        if (ep.Entry[1].EntityType != SAHPI_ENT_$ent1) {
	    printf("  Error! Testcase failed. Line=%d\\n", __LINE__);
	    printf("  Received=%d; Expected=%d\\n", ep.Entry[1].EntityType, SAHPI_ENT_$ent1);
	    return -1;
	}
        
        if (ep.Entry[1].EntityLocation != $inst1) {
	    printf("  Error! Testcase failed. Line=%d\\n", __LINE__);
	    printf("  Received=%d; Expected=%d\\n", ep.Entry[1].EntityLocation, $inst1);
	    return -1;
	}

	oh_init_bigtext(&bigbuf);
	err = oh_decode_entitypath(&ep, &bigbuf);
	if (err) {
		printf("  Error! Testcase failed. Line=%d\\n", __LINE__);
		printf("  Received error=%s\\n", oh_lookup_error(err));
		return -1;
	}

	if (strcmp((char *)bigbuf.Data, test_string)) {
		printf("  Error! Testcase failed. Line=%d\\n", __LINE__);
		printf("  Received Entity Path=%s.\\n", bigbuf.Data);
		return -1;
	}

        return 0;
}
END
}
