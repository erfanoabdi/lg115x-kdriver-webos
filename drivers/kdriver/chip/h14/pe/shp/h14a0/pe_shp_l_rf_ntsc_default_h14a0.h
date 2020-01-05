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

const PE_REG_PARAM_T shp_l_rf_ntsc_default_h14a0[] =
{
	/* scaler : min-max clipping(V)*/
	{0x1180, 0x00000281},
	/* cva cti */
	{0x0184, 0x30000014},
	/* p0l cti */
	{0x0584, 0x00141810},
	{0x05E4, 0x00286015},
	/* pe1 shp, 0x53D0~0x542C,0x5470~0x54CC,0x54F0~0x5550,0x5560~0x559C */
	{0x53D0, 0x09030109},
	{0x53D4, 0x00004100},
	{0x53D8, 0x0801f808},
	{0x53DC, 0xc80c0000},
	{0x53E0, 0x0000ff01},
	{0x53E4, 0x1e100001},
	{0x53E8, 0x85040400},
	{0x53EC, 0x3f000000},
	{0x53F0, 0x30ff1010},
	{0x53F4, 0x00004020},
	{0x5400, 0x0006120f},
	{0x5404, 0x00000018},
	{0x5408, 0x44280001},
	{0x540C, 0x00002020},
	{0x5410, 0x00000000},
	{0x5414, 0x50504848},
	{0x5470, 0x06101000},
	{0x5474, 0x17114428},
	{0x5478, 0x00003f20},
	{0x547C, 0x00000000},
	{0x5490, 0x00000001},
	{0x5494, 0x10302828},
	{0x5498, 0x20012024},
	{0x549C, 0x00000000},
	{0x54A0, 0x01401000},
	{0x54A8, 0x00000000},
	{0x54AC, 0x00001010},
	{0x54B0, 0x000c0c01},
	{0x54B4, 0x00c81919},
	{0x54B8, 0xd8d06040},
	{0x54BC, 0x3800ffff},
	{0x54C0, 0xffd8d048},
	{0x54C4, 0x000000ff},
	{0x54D0, 0x00000000},
	{0x54D4, 0x00000000},
	{0x54D8, 0x00000000},
	{0x54F0, 0x00000001},
	{0x54F4, 0x10302828},
	{0x54F8, 0x20012024},
	{0x54FC, 0x00000000},
	{0x5500, 0x01401000},
	{0x5508, 0x00000000},
	{0x550C, 0x00001044},
	{0x5510, 0x000c0c01},
	{0x5514, 0x00b01919},
	{0x5518, 0xd8d06040},
	{0x551C, 0x3800ffff},
	{0x5520, 0xffd8d049},
	{0x5524, 0x000000ff},
	{0x5530, 0x80208D31},
	{0x5534, 0x00713808},
	{0x5538, 0x00880852},
	{0x5540, 0x00000000},
	{0x5544, 0x00000000},
	{0x5548, 0x00000000},
	{0x5560, 0x0c100430},
	{0x5564, 0x78380a05},
	{0x5568, 0x00040c01},
	{0x556C, 0x00040c01},
	{0x5570, 0x00210703},
	{0x5574, 0x01481820},
	{0x5578, 0x10083820},
	{0x557C, 0x40403b2a},
	{0x5580, 0x0000003f},
	{0x5584, 0x48485050},
	{0x5590, 0xb0000165},
	{0x5594, 0x00001000},	//n_avg_gain : 0x80 -> 0x00, because of bit issue
	{0x5598, 0x0000100b},
	{0x559C, 0x100b100b}
};

