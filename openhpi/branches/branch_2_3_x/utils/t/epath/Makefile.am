#
# Copyright (c) 2003, 2004 Intel Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither the name of Intel Corporation nor the names
# of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

MAINTAINERCLEANFILES = Makefile.in

REMOTE_SOURCES = \
		epath_utils.c \
		sahpi_enum_utils.c \
		sahpi_event_encode.c \
		sahpi_event_utils.c \
		sahpi_struct_utils.c \
		sahpi_time_utils.c

MOSTLYCLEANFILES = $(REMOTE_SOURCES) @TEST_CLEAN@

INCLUDES = @OPENHPI_INCLUDES@

AM_CFLAGS = @NOCFLAGS@ @COV_CFLAGS@

$(REMOTE_SOURCES):
	if test ! -f $@ -a ! -L $@; then \
		if test -f ../../$@; then \
			$(LN_S) ../../$@ $@; \
		else \
			$(LN_S) $(top_srcdir)/utils/$@ $@; \
		fi; \
	fi

# FIXME:: Remove epath_conv_0xx files when we can auto-generate file for SMP systems
#         Create rule using gen_tests.pl
TESTS = \
	epath_utils_test \
	epath_numeric_000 \
	epath_numeric_001 \
	epath_numeric_002 \
	epath_numeric_003 \
	epath_numeric_004 \
	epath_numeric_005 \
	epath_conv_000 \
	epath_conv_001 \
	epath_conv_002 \
	epath_conv_003 \
	epath_conv_004 \
	epath_conv_005 \
	epath_conv_006 \
	epath_conv_007 \
	epath_conv_008 \
	epath_conv_009 \
	epath_conv_010 \
	epath_conv_011 \
	epath_conv_012 \
	epath_conv_013 \
	epath_conv_014 \
	epath_conv_015 \
	epath_conv_016 \
	epath_conv_017 \
	epath_conv_018 \
	epath_conv_019 \
	epath_conv_020 \
	epath_conv_021 \
	epath_conv_022 \
	epath_conv_023 \
	epath_conv_024 \
	epath_conv_025 \
	epath_conv_026 \
	epath_conv_027 \
	epath_conv_028 \
	epath_conv_029 \
	epath_conv_030 \
	epath_conv_031 \
	epath_conv_032 \
	epath_conv_033 \
	epath_conv_034 \
	epath_conv_035 \
	epath_conv_036 \
	epath_conv_037 \
	epath_conv_038 \
	epath_conv_039 \
	epath_conv_040 \
	epath_conv_041 \
	epath_conv_042 \
	epath_conv_043 \
	epath_conv_044 \
	epath_conv_045 \
	epath_conv_046 \
	epath_conv_047 \
	epath_conv_048 \
	epath_conv_049 \
	epath_conv_050 \
	epath_conv_051 \
	epath_conv_052 \
	epath_conv_053 \
	epath_conv_054 \
	epath_conv_055 \
	epath_conv_056 \
	epath_conv_057 \
	epath_conv_058 \
	epath_conv_059 \
	epath_conv_060 \
	epath_conv_061 \
	epath_conv_062 \
	epath_conv_063 \
	epath_conv_064 \
	epath_conv_065 \
	epath_conv_066 \
	epath_conv_067 \
	epath_conv_068 \
	epath_conv_069 \
	epath_conv_070 \
	epath_conv_071 \
	epath_conv_072 \
	epath_conv_073 \
	epath_conv_074 \
	epath_conv_075 \
	epath_conv_076 \
	epath_conv_077 \
	epath_conv_078 \
	epath_conv_079 \
	epath_conv_080 \
	epath_conv_081 \
	epath_conv_082 \
	epath_conv_083 \
	epath_conv_084 \
	epath_conv_085 \
	epath_conv_086 \
	epath_conv_087 \
	epath_conv_088 \
	epath_conv_089 \
	epath_conv_090 \
	epath_conv_091 \
	epath_conv_092 \
	epath_conv_093 \
	epath_conv_094 \
	epath_conv_095 \
	epath_conv_096 \
	epath_conv_097 \
	epath_conv_098 \
	epath_conv_099 \
	epath_conv_100 \
	epath_conv_101 \
	ep_cmp_000 \
	ep_cmp_001 \
	ep_cmp_002 \
	ep_cmp_003 \
	ep_cmp_004 \
	ep_cmp_005 \
	ep_cmp_006 \
	ep_cmp_007 \
	ep_cmp_008 \
	ep_cmp_009 \
	ep_cmp_010 \
	ep_cmp_011 \
	ep_concat_000 \
        ep_concat_001 \
        ep_concat_002 \
        ep_concat_003 \
        ep_concat_004 \
        ep_concat_005 \
        ep_concat_006 \
        ep_concat_007 \
        ep_concat_008 \
        ep_concat_009 \
        ep_concat_010 \
        ep_concat_011 \
        ep_concat_012 \
        ep_concat_013 \
        ep_concat_014 \
        ep_concat_015 \
	ep_derive_001 \
        set_ep_loc_000 \
        set_ep_loc_001 \
        set_ep_loc_002 \
        set_ep_loc_003 \
        set_ep_loc_004 \
        set_ep_loc_005 \
        set_ep_loc_006 \
        set_ep_loc_007 \
        set_ep_loc_008 \
        set_ep_loc_009 \
        set_ep_loc_010 \
        set_ep_loc_011 \
        set_ep_loc_012 \
        set_ep_loc_013 \
        print_ep_000 \
        print_ep_001 \
        print_ep_002 \
        print_ep_003 \
        print_ep_004 \
        print_ep_005

check_PROGRAMS = $(TESTS)

epath_utils_test_SOURCES = epath_utils_test.c $(REMOTE_SOURCES)

epath_numeric_000_SOURCES = epath_numeric_000.c $(REMOTE_SOURCES)
epath_numeric_001_SOURCES = epath_numeric_001.c $(REMOTE_SOURCES)
epath_numeric_002_SOURCES = epath_numeric_002.c $(REMOTE_SOURCES)
epath_numeric_003_SOURCES = epath_numeric_003.c $(REMOTE_SOURCES)
epath_numeric_004_SOURCES = epath_numeric_004.c $(REMOTE_SOURCES)
epath_numeric_005_SOURCES = epath_numeric_005.c $(REMOTE_SOURCES)

epath_conv_000_SOURCES = epath_conv_000.c $(REMOTE_SOURCES)
epath_conv_001_SOURCES = epath_conv_001.c $(REMOTE_SOURCES)
epath_conv_002_SOURCES = epath_conv_002.c $(REMOTE_SOURCES)
epath_conv_003_SOURCES = epath_conv_003.c $(REMOTE_SOURCES)
epath_conv_004_SOURCES = epath_conv_004.c $(REMOTE_SOURCES)
epath_conv_005_SOURCES = epath_conv_005.c $(REMOTE_SOURCES)
epath_conv_006_SOURCES = epath_conv_006.c $(REMOTE_SOURCES)
epath_conv_007_SOURCES = epath_conv_007.c $(REMOTE_SOURCES)
epath_conv_008_SOURCES = epath_conv_008.c $(REMOTE_SOURCES)
epath_conv_009_SOURCES = epath_conv_009.c $(REMOTE_SOURCES)
epath_conv_010_SOURCES = epath_conv_010.c $(REMOTE_SOURCES)
epath_conv_011_SOURCES = epath_conv_011.c $(REMOTE_SOURCES)
epath_conv_012_SOURCES = epath_conv_012.c $(REMOTE_SOURCES)
epath_conv_013_SOURCES = epath_conv_013.c $(REMOTE_SOURCES)
epath_conv_014_SOURCES = epath_conv_014.c $(REMOTE_SOURCES)
epath_conv_015_SOURCES = epath_conv_015.c $(REMOTE_SOURCES)
epath_conv_016_SOURCES = epath_conv_016.c $(REMOTE_SOURCES)
epath_conv_017_SOURCES = epath_conv_017.c $(REMOTE_SOURCES)
epath_conv_018_SOURCES = epath_conv_018.c $(REMOTE_SOURCES)
epath_conv_019_SOURCES = epath_conv_019.c $(REMOTE_SOURCES)
epath_conv_020_SOURCES = epath_conv_020.c $(REMOTE_SOURCES)
epath_conv_021_SOURCES = epath_conv_021.c $(REMOTE_SOURCES)
epath_conv_022_SOURCES = epath_conv_022.c $(REMOTE_SOURCES)
epath_conv_023_SOURCES = epath_conv_023.c $(REMOTE_SOURCES)
epath_conv_024_SOURCES = epath_conv_024.c $(REMOTE_SOURCES)
epath_conv_025_SOURCES = epath_conv_025.c $(REMOTE_SOURCES)
epath_conv_026_SOURCES = epath_conv_026.c $(REMOTE_SOURCES)
epath_conv_027_SOURCES = epath_conv_027.c $(REMOTE_SOURCES)
epath_conv_028_SOURCES = epath_conv_028.c $(REMOTE_SOURCES)
epath_conv_029_SOURCES = epath_conv_029.c $(REMOTE_SOURCES)
epath_conv_030_SOURCES = epath_conv_030.c $(REMOTE_SOURCES)
epath_conv_031_SOURCES = epath_conv_031.c $(REMOTE_SOURCES)
epath_conv_032_SOURCES = epath_conv_032.c $(REMOTE_SOURCES)
epath_conv_033_SOURCES = epath_conv_033.c $(REMOTE_SOURCES)
epath_conv_034_SOURCES = epath_conv_034.c $(REMOTE_SOURCES)
epath_conv_035_SOURCES = epath_conv_035.c $(REMOTE_SOURCES)
epath_conv_036_SOURCES = epath_conv_036.c $(REMOTE_SOURCES)
epath_conv_037_SOURCES = epath_conv_037.c $(REMOTE_SOURCES)
epath_conv_038_SOURCES = epath_conv_038.c $(REMOTE_SOURCES)
epath_conv_039_SOURCES = epath_conv_039.c $(REMOTE_SOURCES)
epath_conv_040_SOURCES = epath_conv_040.c $(REMOTE_SOURCES)
epath_conv_041_SOURCES = epath_conv_041.c $(REMOTE_SOURCES)
epath_conv_042_SOURCES = epath_conv_042.c $(REMOTE_SOURCES)
epath_conv_043_SOURCES = epath_conv_043.c $(REMOTE_SOURCES)
epath_conv_044_SOURCES = epath_conv_044.c $(REMOTE_SOURCES)
epath_conv_045_SOURCES = epath_conv_045.c $(REMOTE_SOURCES)
epath_conv_046_SOURCES = epath_conv_046.c $(REMOTE_SOURCES)
epath_conv_047_SOURCES = epath_conv_047.c $(REMOTE_SOURCES)
epath_conv_048_SOURCES = epath_conv_048.c $(REMOTE_SOURCES)
epath_conv_049_SOURCES = epath_conv_049.c $(REMOTE_SOURCES)
epath_conv_050_SOURCES = epath_conv_050.c $(REMOTE_SOURCES)
epath_conv_051_SOURCES = epath_conv_051.c $(REMOTE_SOURCES)
epath_conv_052_SOURCES = epath_conv_052.c $(REMOTE_SOURCES)
epath_conv_053_SOURCES = epath_conv_053.c $(REMOTE_SOURCES)
epath_conv_054_SOURCES = epath_conv_054.c $(REMOTE_SOURCES)
epath_conv_055_SOURCES = epath_conv_055.c $(REMOTE_SOURCES)
epath_conv_056_SOURCES = epath_conv_056.c $(REMOTE_SOURCES)
epath_conv_057_SOURCES = epath_conv_057.c $(REMOTE_SOURCES)
epath_conv_058_SOURCES = epath_conv_058.c $(REMOTE_SOURCES)
epath_conv_059_SOURCES = epath_conv_059.c $(REMOTE_SOURCES)
epath_conv_060_SOURCES = epath_conv_060.c $(REMOTE_SOURCES)
epath_conv_061_SOURCES = epath_conv_061.c $(REMOTE_SOURCES)
epath_conv_062_SOURCES = epath_conv_062.c $(REMOTE_SOURCES)
epath_conv_063_SOURCES = epath_conv_063.c $(REMOTE_SOURCES)
epath_conv_064_SOURCES = epath_conv_064.c $(REMOTE_SOURCES)
epath_conv_065_SOURCES = epath_conv_065.c $(REMOTE_SOURCES)
epath_conv_066_SOURCES = epath_conv_066.c $(REMOTE_SOURCES)
epath_conv_067_SOURCES = epath_conv_067.c $(REMOTE_SOURCES)
epath_conv_068_SOURCES = epath_conv_068.c $(REMOTE_SOURCES)
epath_conv_069_SOURCES = epath_conv_069.c $(REMOTE_SOURCES)
epath_conv_070_SOURCES = epath_conv_070.c $(REMOTE_SOURCES)
epath_conv_071_SOURCES = epath_conv_071.c $(REMOTE_SOURCES)
epath_conv_072_SOURCES = epath_conv_072.c $(REMOTE_SOURCES)
epath_conv_073_SOURCES = epath_conv_073.c $(REMOTE_SOURCES)
epath_conv_074_SOURCES = epath_conv_074.c $(REMOTE_SOURCES)
epath_conv_075_SOURCES = epath_conv_075.c $(REMOTE_SOURCES)
epath_conv_076_SOURCES = epath_conv_076.c $(REMOTE_SOURCES)
epath_conv_077_SOURCES = epath_conv_077.c $(REMOTE_SOURCES)
epath_conv_078_SOURCES = epath_conv_078.c $(REMOTE_SOURCES)
epath_conv_079_SOURCES = epath_conv_079.c $(REMOTE_SOURCES)
epath_conv_080_SOURCES = epath_conv_080.c $(REMOTE_SOURCES)
epath_conv_081_SOURCES = epath_conv_081.c $(REMOTE_SOURCES)
epath_conv_082_SOURCES = epath_conv_082.c $(REMOTE_SOURCES)
epath_conv_083_SOURCES = epath_conv_083.c $(REMOTE_SOURCES)
epath_conv_084_SOURCES = epath_conv_084.c $(REMOTE_SOURCES)
epath_conv_085_SOURCES = epath_conv_085.c $(REMOTE_SOURCES)
epath_conv_086_SOURCES = epath_conv_086.c $(REMOTE_SOURCES)
epath_conv_087_SOURCES = epath_conv_087.c $(REMOTE_SOURCES)
epath_conv_088_SOURCES = epath_conv_088.c $(REMOTE_SOURCES)
epath_conv_089_SOURCES = epath_conv_089.c $(REMOTE_SOURCES)
epath_conv_090_SOURCES = epath_conv_090.c $(REMOTE_SOURCES)
epath_conv_091_SOURCES = epath_conv_091.c $(REMOTE_SOURCES)
epath_conv_092_SOURCES = epath_conv_092.c $(REMOTE_SOURCES)
epath_conv_093_SOURCES = epath_conv_093.c $(REMOTE_SOURCES)
epath_conv_094_SOURCES = epath_conv_094.c $(REMOTE_SOURCES)
epath_conv_095_SOURCES = epath_conv_095.c $(REMOTE_SOURCES)
epath_conv_096_SOURCES = epath_conv_096.c $(REMOTE_SOURCES)
epath_conv_097_SOURCES = epath_conv_097.c $(REMOTE_SOURCES)
epath_conv_098_SOURCES = epath_conv_098.c $(REMOTE_SOURCES)
epath_conv_099_SOURCES = epath_conv_099.c $(REMOTE_SOURCES)
epath_conv_100_SOURCES = epath_conv_100.c $(REMOTE_SOURCES)
epath_conv_101_SOURCES = epath_conv_101.c $(REMOTE_SOURCES)

ep_cmp_000_SOURCES = ep_cmp_000.c $(REMOTE_SOURCES)
ep_cmp_001_SOURCES = ep_cmp_001.c $(REMOTE_SOURCES)
ep_cmp_002_SOURCES = ep_cmp_002.c $(REMOTE_SOURCES)
ep_cmp_003_SOURCES = ep_cmp_003.c $(REMOTE_SOURCES)
ep_cmp_004_SOURCES = ep_cmp_004.c $(REMOTE_SOURCES)
ep_cmp_005_SOURCES = ep_cmp_005.c $(REMOTE_SOURCES)
ep_cmp_006_SOURCES = ep_cmp_006.c $(REMOTE_SOURCES)
ep_cmp_007_SOURCES = ep_cmp_007.c $(REMOTE_SOURCES)
ep_cmp_008_SOURCES = ep_cmp_008.c $(REMOTE_SOURCES)
ep_cmp_009_SOURCES = ep_cmp_009.c $(REMOTE_SOURCES)
ep_cmp_010_SOURCES = ep_cmp_010.c $(REMOTE_SOURCES)
ep_cmp_011_SOURCES = ep_cmp_011.c $(REMOTE_SOURCES)

ep_concat_000_SOURCES = ep_concat_000.c $(REMOTE_SOURCES)
ep_concat_001_SOURCES = ep_concat_001.c $(REMOTE_SOURCES)
ep_concat_002_SOURCES = ep_concat_002.c $(REMOTE_SOURCES)
ep_concat_003_SOURCES = ep_concat_003.c $(REMOTE_SOURCES)
ep_concat_004_SOURCES = ep_concat_004.c $(REMOTE_SOURCES)
ep_concat_005_SOURCES = ep_concat_005.c $(REMOTE_SOURCES)
ep_concat_006_SOURCES = ep_concat_006.c $(REMOTE_SOURCES)
ep_concat_007_SOURCES = ep_concat_007.c $(REMOTE_SOURCES)
ep_concat_008_SOURCES = ep_concat_008.c $(REMOTE_SOURCES)
ep_concat_009_SOURCES = ep_concat_009.c $(REMOTE_SOURCES)
ep_concat_010_SOURCES = ep_concat_010.c $(REMOTE_SOURCES)
ep_concat_011_SOURCES = ep_concat_011.c $(REMOTE_SOURCES)
ep_concat_012_SOURCES = ep_concat_012.c $(REMOTE_SOURCES)
ep_concat_013_SOURCES = ep_concat_013.c $(REMOTE_SOURCES)
ep_concat_014_SOURCES = ep_concat_014.c $(REMOTE_SOURCES)
ep_concat_015_SOURCES = ep_concat_015.c $(REMOTE_SOURCES)

ep_derive_001_SOURCES = ep_derive_001.c $(REMOTE_SOURCES)

set_ep_loc_000_SOURCES = set_ep_loc_000.c $(REMOTE_SOURCES)
set_ep_loc_001_SOURCES = set_ep_loc_001.c $(REMOTE_SOURCES)
set_ep_loc_002_SOURCES = set_ep_loc_002.c $(REMOTE_SOURCES)
set_ep_loc_003_SOURCES = set_ep_loc_003.c $(REMOTE_SOURCES)
set_ep_loc_004_SOURCES = set_ep_loc_004.c $(REMOTE_SOURCES)
set_ep_loc_005_SOURCES = set_ep_loc_005.c $(REMOTE_SOURCES)
set_ep_loc_006_SOURCES = set_ep_loc_006.c $(REMOTE_SOURCES)
set_ep_loc_007_SOURCES = set_ep_loc_007.c $(REMOTE_SOURCES)
set_ep_loc_008_SOURCES = set_ep_loc_008.c $(REMOTE_SOURCES)
set_ep_loc_009_SOURCES = set_ep_loc_009.c $(REMOTE_SOURCES)
set_ep_loc_010_SOURCES = set_ep_loc_010.c $(REMOTE_SOURCES)
set_ep_loc_011_SOURCES = set_ep_loc_011.c $(REMOTE_SOURCES)
set_ep_loc_012_SOURCES = set_ep_loc_012.c $(REMOTE_SOURCES)
set_ep_loc_013_SOURCES = set_ep_loc_013.c $(REMOTE_SOURCES)

print_ep_000_SOURCES = print_ep_000.c $(REMOTE_SOURCES)
print_ep_001_SOURCES = print_ep_001.c $(REMOTE_SOURCES)
print_ep_002_SOURCES = print_ep_002.c $(REMOTE_SOURCES)
print_ep_003_SOURCES = print_ep_003.c $(REMOTE_SOURCES)
print_ep_004_SOURCES = print_ep_004.c $(REMOTE_SOURCES)
print_ep_005_SOURCES = print_ep_005.c $(REMOTE_SOURCES)

