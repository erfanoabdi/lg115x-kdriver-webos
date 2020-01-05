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

const PE_REG_PARAM_T shp_l_av_ntsc_default_m14a0[] =
{
	/* scaler : min-max clipping(V)*/
	{0x1180, 0x00000281},
	/* cva cti */
	{0x0184, 0x30000034},
	/* p0l cti */
	{0x0584, 0x00001810},
	{0x05E4, 0x00282001},
	/* pe1 shp, 0x53D0~0x54CC,0x54F0~0x5550,0x5560~0x559C */
	{0x53D0, 0x07030109},
	{0x53D4, 0x00004100},
	{0x53D8, 0x0801f808},
	{0x53DC, 0xc80c0000},
	{0x53E0, 0x0000ff01},
	{0x53E4, 0x1e100201},
	{0x53E8, 0x85040400},
	{0x53EC, 0x10000000},
	{0x53F0, 0x30ff1010},
	{0x53F4, 0x00004020},
	{0x53F8, 0x2b8ae2b7},
	{0x53FC, 0x0503330a},
	{0x5400, 0x0106300a},
	{0x5404, 0x00000018},
	{0x5408, 0x44280000},
	{0x540C, 0x00002020},
	{0x5410, 0x00000000},
	{0x5414, 0x40402325},
	{0x5418, 0x00000000},
	{0x541C, 0x00000000},
	{0x5420, 0x1c9431bf},
	{0x5424, 0x000047b9},
	{0x5428, 0x00000000},
	{0x542C, 0x00000000},
	{0x5430, 0x0503330a},
	{0x5434, 0x2b8ae2b7},
	{0x5438, 0x000001ee},
	{0x543C, 0x0503330a},
	{0x5440, 0x00000117},
	{0x5444, 0x000001b7},
	{0x5448, 0x2b8ae2b7},
	{0x544C, 0x000001ae},
	{0x5450, 0x000001ff},
	{0x5454, 0x0503330a},
	{0x5458, 0x00000104},
	{0x545C, 0x000001ca},
	{0x5460, 0x0503330a},
	{0x5464, 0x0503330a},
	{0x5468, 0x000001f7},
	{0x546C, 0x0503330a},
	{0x5470, 0x06101000},
	{0x5474, 0x0503330a},
	{0x5478, 0x00002020},
	{0x547C, 0x00000000},
	{0x5480, 0x00000203},
	{0x5484, 0x2b8ae2b7},
	{0x5488, 0x0503330a},
	{0x548C, 0x000001b6},
	{0x5490, 0x00000001},
	{0x5494, 0x0a182020},
	{0x5498, 0x20014024},
	{0x549C, 0x00000000},
	{0x54A0, 0x01401000},
	{0x54A4, 0x2b8adeb7},
	{0x54A8, 0x00000000},
	{0x54AC, 0x00001011},
	{0x54B0, 0x000c0c01},
	{0x54B4, 0x00c81919},
	{0x54B8, 0xd8d06040},
	{0x54BC, 0x3800ffff},
	{0x54C0, 0xffd8d048},
	{0x54C4, 0x000000ff},
	{0x54C8, 0x0503330a},
	{0x54CC, 0x2b8ae2b7},
	{0x54D0, 0x00000000},
	{0x54D4, 0x00000000},
	{0x54D8, 0x00000000},
	{0x54DC, 0x00000000},
	{0x54E0, 0x00000000},
	{0x54E4, 0x00000000},
	{0x54E8, 0x00000000},
	{0x54EC, 0x00000000},
	{0x54F0, 0x00000001},
	{0x54F4, 0x101a1820},
	{0x54F8, 0x20014024},
	{0x54FC, 0x00000000},
	{0x5500, 0x01401000},
	{0x5504, 0x0503330a},
	{0x5508, 0x00000000},
	{0x550C, 0x00001044},
	{0x5510, 0x000c0c01},
	{0x5514, 0x00b01919},
	{0x5518, 0xd8d06040},
	{0x551C, 0x3800ffff},
	{0x5520, 0xffd8d049},
	{0x5524, 0x000000ff},
	{0x5528, 0x00000132},
	{0x552C, 0x0503330a},
	{0x5530, 0x80208051},
	{0x5534, 0x00000007},
	{0x5538, 0x0fff0852},
	{0x553C, 0x00000207},
	{0x5540, 0x00000121},
	{0x5544, 0x0503330a},
	{0x5548, 0x000001f6},
	{0x554C, 0x000001aa},
	{0x5550, 0x00000000},
	{0x5560, 0x04281830},
	{0x5564, 0x78380a05},
	{0x5568, 0x00040c01},
	{0x556C, 0x00040c01},
	{0x5570, 0x00210503},
	{0x5574, 0x01200420},
	{0x5578, 0x10081808},
	{0x557C, 0x40403b2a},
	{0x5580, 0x0000003f},
	{0x5584, 0x40402325},
	{0x5588, 0x00000115},
	{0x558C, 0x2b8ae2b7},
	{0x5590, 0xb0000165},
	{0x5594, 0x00801000},
	{0x5598, 0x0000100b},
	{0x559C, 0x100b100b}
};
