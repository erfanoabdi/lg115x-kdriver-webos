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

const PE_REG_PARAM_T tnr_l_pc_default_m14b0[] = 
{
	/* pe0m tnr, 0x2140~0x21BC , 0x21DC~0x21FC , 0x23D0~0x23FC*/
	{0x2140, 0xC0BDE407},		/* TNR Off 0x4440[0] = 0;*/
	{0x2144, 0x000CF418},
	{0x2148, 0x30404C4F},
	{0x214C, 0x80081C08},
	{0x2150, 0x8F4E130E},
	{0x2154, 0x03FF0203},
	{0x2158, 0xF4A34610},
	{0x215C, 0x2864000C},
	{0x2160, 0x08232440},
	{0x2164, 0x04C92032},
	{0x2168, 0xFFD8D020},
	{0x216C, 0xDE9C0091},
	{0x2170, 0xFF404081},
	{0x2174, 0x60808230},
	{0x2178, 0x00D8D069},
	{0x217C, 0x01E00002},
	{0x2180, 0x46106F19},
	{0x2184, 0x418F542C},
	{0x2188, 0x14222821},
	{0x218C, 0x60603018},
	{0x2190, 0x1030B003},
	{0x2194, 0x40FC0010},
	{0x2198, 0x2004C00D},
	{0x219C, 0x0088E702},
	{0x21A0, 0x50EA00C3},
	{0x21A4, 0x00802302},
	{0x21A8, 0x1161F001},
	{0x21AC, 0xC0A08083},
	{0x21B0, 0xF0FFD8A8},
	{0x21B4, 0xA0582003},
	{0x21BC, 0xFFC40415},   // 131231 : fix for small scene change 
	{0x21DC, 0x00001700},
	{0x21E0, 0x30FF00FF},
	{0x21E4, 0x05406080},
	{0x21E8, 0xFFFF1000},
	{0x21EC, 0xE0CA1821},
	{0x21F0, 0x7F7E1C33},
	{0x21F4, 0xB5A98187},
	{0x21F8, 0x84444403},
	{0x21FC, 0x6B892B0A},
	{0x23D0, 0x445112C3},
	{0x23D4, 0xCBA0BC01},
	{0x23D8, 0x1040F010},
	{0x23DC, 0x38A008E0},
	{0x23E0, 0x00004045},
	{0x23E4, 0x00000102},
	{0x23E8, 0x00060000},
	{0x23EC, 0x380B10B0},
	{0x23F0, 0xFFFF0000},
	{0x23F8, 0x4010FF30},
	{0x23FC, 0xFF000000}
};