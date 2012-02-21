##############################################################
#  ____            _        _   ____       _     _       _
# |  _ \(_) __ _(_) |_ __ _| | |  _ \ __ _| |__ | |__ (_) |_
# | | | | |/ _` | | __/ _` | | | |_) / _` | '_ \| '_ \| | __|
# | |_| | | (_| | | |_ (_| | | |  _ < (_| | |_) | |_) | | |_
# |____/|_|\__, |_|\__\__,_|_| |_| \_\__,_|_.__/|_.__/|_|\__|
#          |___/
##############################################################
# Copyright(C) 2012 Digital Rabbit Rights Reserved.
#
# \file     makefile
#
# \author   Digital Rabbit(digital.rabbit.jp@gmail.com)
# \date     2012/02/21
#

# 環境変数読み込み
include .make/env_value.mk

# Project ( application ) 名
program := projectmaker
# Source file list
sources := $(shell $(FIND) $(SOURCE_DIR) -name "*.cc" -type f)
# Resource file list
resources := $(shell $(FIND) $(RESOURCE_DIR) -type f)
#
extra_clean :=

# Include directory
include_dirs := $(shell $(FIND) . -regex ".*inc\(lude\)?\(/.*\|\)" -type d)
# Object list
objects = $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(subst .cc,.o,$(sources)))
# Binary resource list
binary_res = $(addprefix $(BINARY_DIR)/,$(resources))
# dependencies file list
dependencies = $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(subst .cc,.d,$(sources)))

CXXFLAGS += -std=c++0x -g -Wall $(addprefix -I , $(include_dirs)) $(shell pkg-config gtkmm-3.0 libxml++-2.6 --cflags)
LDFLAGS += -Wall $(shell pkg-config gtkmm-3.0 libxml++-2.6 --libs)
vpath %.h $(include_dirs)

# Directory 生成( ※. <create-out-dir> 変数が生成されてしまう。 )
include .make/create_out_dir.mk

all:

.PHONY: all
all: $(program) $(binary_res)

.PHONY: libraries
libraries: $(libraries)

.PHONY: clean
clean:
	@$(RM) $(OBJECT_DIR) $(BINARY_DIR) $(libraries) $(extra_clean)

ifneq ($(MAKECMDGOALS), clean)
    -include $(dependencies)
endif

$(program): $(objects)
	@echo
	@echo "---------- Link object files ----------"
	$(CXX) $(objects) $(LDFLAGS) $(TARGET_ARCH) -o $(BINARY_DIR)/$@

$(binary_res): $(resources)
	@echo
	@echo "---------- Copy resource files ----------"
	$(CP) $< $@

$(OBJECT_DIR)/%.d: $(SOURCE_DIR)/%.cc
	@echo
	@echo "---------- Make the dependencies list from $<. ----------"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< | \
		$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp;
	$(MV) $@.tmp $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cc
	@echo
	@echo "---------- Make the object file from $<. ----------"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

