##############################################################
#  ____            _        _   ____       _     _       _
# |  _ \(_) __ _(_) |_ __ _| | |  _ \ __ _| |__ | |__ (_) |_
# | | | | |/ _` | | __/ _` | | | |_) / _` | '_ \| '_ \| | __|
# | |_| | | (_| | | |_ (_| | | |  _ < (_| | |_) | |_) | | |_
# |____/|_|\__, |_|\__\__,_|_| |_| \_\__,_|_.__/|_.__/|_|\__|
#          |___/
##############################################################
# Copyright(C) 2012 Digital Rabbit All Rights Reserved.
#
# \file     create_out_dir.mk
#
# \author   Digital Rabbit(digital.rabbit.jp@gmail.com)
# \date     2012/02/21
#
create-out-dir := \
	$(shell for f in $(sort                                            \
				$(dir $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(sources))) \
				$(BINARY_DIR)                                          \
				$(dir $(binary_res)));                                 \
		do                                                             \
			$(TEST) -d $$f || $(MKDIR) $$f;                            \
		done                                                           \
	)

