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

/* This is a test pattern mode setting. 0x05c0[2] off*/
const PE_REG_PARAM_T dnr_tp_l_default_h13b0[] = 
{
	/* dnr 0x0588~0x05A0, 0x05AC~0x05E0*/
	{0x05D8, 0x8000001F},
	{0x05C0, 0xFF103373},	//filter type[20]:sd:0x0,hd:0x1, ac_protection_en[2]:0x1->0x0
	{0x05AC, 0xFF00FF47},	//blur sel[6:5]:0x3->0x2
	{0x0584, 0x00202101},
	{0x0588, 0x0000FF35},
	{0x05DC, 0x00000000},
	{0x05CC, 0x00700700},
	{0x05B0, 0x04080E14},
	{0x05B4, 0x10182030},
	{0x05B8, 0xA0801000},
	{0x05C4, 0x04FF4018},	//strength_h_max[23:16]:0x80->0xFF
	{0x05C8, 0x00807030},	//strength_v_max[23:16]:0x80->0xFF
	{0x05D0, 0x0000140F},
	{0x05D4, 0xF0A05000},	//ac_gain:0xFF000000->0xFF803000
	{0x05BC, 0x2038FFFD},	//ac_bnr_protect_motion_min,max[31:16]:0x0810->0x1820
	{0x058C, 0x44B00028},
	{0x0590, 0x80808884},
	{0x0594, 0x00FF3040},
	{0x0598, 0x30102D21},
	{0x059C, 0x00FF2030}
};
