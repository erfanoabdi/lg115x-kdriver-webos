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
#	DESCRIPTION	:	Release Kernel&Kdriver code for open source    #
#******************************************************************************#

include ./cmn_def.inc
#------------------------------------------------------------------------------
#		Make command option 
#------------------------------------------------------------------------------
MAKE	:=	$(Q)make 
#------------------------------------------------------------------------------
#		Build rules 
#------------------------------------------------------------------------------
.PHONY: kernel kdriver toolchain 

all: toolchain kernel kdriver

toolchain:
	$(MAKE) -C ./toolchain
kernel:
	$(MAKE) -C ./kernel
kdriver:
	$(MAKE) -C ./drivers

clean:
	$(MAKE) -C ./toolchain clean
	$(MAKE) -C ./drivers clean 
	$(MAKE) -C ./kernel clean 

#------------------------------------------------------------------------------
