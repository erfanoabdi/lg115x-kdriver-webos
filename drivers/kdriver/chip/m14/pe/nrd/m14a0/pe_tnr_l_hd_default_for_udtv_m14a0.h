/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 

const PE_REG_PARAM_T tnr_l_hd_default_for_udtv_m14a0[] = 
{
	/* tnr 0x0440~0x04FC, 0x06D0~0x06F8*/
	{0x0468, 0x4080A000},
	{0x0440, 0x49FD4003},
	{0x0474, 0x80018000},
	{0x0488, 0x20204131},
	{0x0448, 0x15904010},
	{0x0444, 0x020DF410},	//tnr_filter_lsb_ctrl[21:20]:0x3->0x0
	{0x04F8, 0x850443FA},	//mv_pfilter_en[25]:0x0
	{0x06D8, 0x20101043},
	{0x06F8, 0x10200880},
	{0x06DC, 0xFE905030},
	{0x06EC, 0x00040101},
	{0x06E0, 0x28801800},	//sf_mb_y1[23:16]:0xff->0x80
	{0x06E8, 0xF000D0FF},
	{0x06D0, 0x84184409},
	{0x06F4, 0xF8384804},
	{0x0464, 0x0860108B},
	{0x04CC, 0x0F031171},
	{0x04C8, 0x5A8201D5},
	{0x04DC, 0x075D032F},
	{0x0494, 0x40FF20C0},
	{0x0498, 0x20001000},
	{0x04E8, 0x73E820E8},
	{0x06F0, 0x10E008A0},
	{0x04FC, 0x28612861},
	{0x0484, 0x40C02018},
	{0x04EC, 0x80D060D0},
	{0x049C, 0x40403020},
	{0x04A0, 0x80806060},
	{0x04A4, 0x40403020},
	{0x04A8, 0x80806060},
	{0x06D4, 0x00900000},
	{0x048C, 0xC0804020},
	{0x0490, 0x20202020},
	{0x04AC, 0x20204038},	//[7:0]crgn0_gain:0x40->0x55->0x38, changed by sy.sohn, 20130609,0615
	{0x0454, 0x03FF0201},
	{0x0460, 0x3F3F281E},
	{0x044C, 0x10082020},
	{0x04B4, 0x70502020},
	{0x04E0, 0x00101010},
	{0x04E4, 0x00E87AE8},
	{0x046C, 0x28F02080},
	{0x0458, 0xF4181030},
	{0x0450, 0x148008C0},
	{0x0470, 0x90403860},
	{0x04B0, 0x608040C0},
	{0x04B8, 0x90406860},
	{0x045C, 0x140E0800},
	//{0x0478, 0x00000168},		//tnr measure win, set in drv
	//{0x047C, 0x01E002D0},		//tnr measure win, set in drv
	{0x04C4, 0x00487E8F},
	{0x04C0, 0x1802082D},
	{0x0480, 0x32100000}
};
