#******************************************************************************#
# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# Copyright(c) 2013 by LG Electronics Inc.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	Makefile                                       #
#	AUTHOR		:	sanghyeun.kim@lge.com                          #
#	DATE       	:	2013/12/06                                     #
#	DESCRIPTION	:	Makefile only to compile kernel&kdriver.       #
#******************************************************************************#

include ../cmn_def.inc
#-----------------------------------------------------------------------------
#		Toolchain Definition
#-----------------------------------------------------------------------------
TOOLCHAIN_NAME		:=  arm-lg115x-linux-gnueabi-2013.08_nc4

#------------------------------------------------------------------------------
#		Build rules 
#------------------------------------------------------------------------------
.PHONY: all install clean

all: install 

install:
	$(Q)if [ ! -d $(TOOLCHAIN_NAME) ] ; then \
		echo -e "\n@@@ Installing $(TOOLCHAIN_NAME) ..."; \
		tar -xjf $(TOOLCHAIN_NAME).tar.bz2  > /dev/null; \
	fi

clean:
	$(ECHO) '\n@@@ Cleaning toolchain($(TOOLCHAIN_NAME)) ...'
	$(Q)if [ -d "$(TOOLCHAIN_NAME)" ] ; then \
		chmod 777 -R $(TOOLCHAIN_NAME); \
		rm -rf $(TOOLCHAIN_NAME) ; \
	fi
#------------------------------------------------------------------------------


