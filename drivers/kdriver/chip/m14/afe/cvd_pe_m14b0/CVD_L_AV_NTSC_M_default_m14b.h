static const CVD_REG_PARAM_T cvd_av_ntsc_m_l_default_m14b[] = 
{
    {0x12B4, 0x00000200},	
    {0x12B8, 0x00020010},	
    {0x12EC, 0x00016FC1},
    {0x12C0, 0x00014920},
    {0x1650, 0x00000000},   // CTA    
    {0x16A0, 0x00000000},   // SECAM DCR
    {0x16A4, 0x00000000},
    {0x16A8, 0x00000000},
    {0x16AC, 0x00000000},
    {0x1768, 0x08000003},   // Cordic
    {0x176C, 0x08000003},   // Cordic
    {0x1770, 0x08000003},   // Cordic
    {0x1678, 0x00000000},
    {0x1654, 0x00000000},
    {0x15CC, 0x55570000}, //0x45470000},
    {0x1414, 0x00000011},
    {0x16D4, 0x400FC047}, // H13B 0x00004F40 E2D_ST_dly : reg_cdct_ifcomp_ctrl_0[30]  -> 0x47D8,reg_cdct_ifcomp_ctrl_1[30]
    {0x16C0, 0x000F4040}, // 2D Comb IFC
    {0x1620, 0x000F4040}, // 3D Comb IFC
    {0x1624, 0x00000206}, // CTI
    {0x15C8, 0x00033078}, // 0x00073078}, //0x00032008},
    {0x15B0, 0x0010000F},
    {0x169C, 0x00000000},
    {0x1570, 0x00004D61},
    {0x1588, 0x0000FF00},
    {0x1560, 0x00000003},
    {0x1544, 0xF2100300}, //0xF2100000},
    {0x15E8, 0x40000020}, //0x00000008},
    {0x156C, 0x00100000}, //0x00000000},
    {0x15B8, 0x1818040B}, //0x30300409}, //0x3030040B},
    {0x1598, 0x86003FFF},
    {0x1574, 0x140E0F00}, //0x100A0F00},	//0B0A0F00
    {0x1578, 0x00801C20},
    {0x157C, 0x30200000},
    {0x1580, 0x8002A300},
    {0x1584, 0x08200120},
    {0x158C, 0x08200820},
    {0x1630, 0x00001200},
    {0x1718, 0x00040081},
    {0x1734, 0x40800400},
    {0x171C, 0x00200020},
    {0x1720, 0x8040F040},
    {0x1724, 0x00302040},
    {0x1728, 0xC060FFFF}, //0xC040FF60},
    {0x17C8, 0x00000005},
    {0x17BC, 0x08400F30},
    {0x17CC, 0x0F300F30},
    {0x17C0, 0x00301040},
    {0x17C4, 0xC040F060},
    {0x15F4, 0x102407F7}, //H13B: 0x102007FF},
    {0x15F8, 0xA0FF0800}, //0x90FF0800},
    {0x16D0, 0x00FFD0C0}, //H13B0x00FF9080}, //0x00FFA040},
    {0x15FC, 0x01000000},
    {0x1600, 0x00000000},
    {0x1604, 0x00000000},
    {0x1608, 0x0018002F}, //0x00190030 0x0016002C //0x00280046
    {0x1634, 0x002F002E}, //0x0030002E 0x002C002B //0x00340020
    {0x1638, 0x002E002E}, //0x002E002B 0x00290027 //0x00100006
    {0x1250, 0x0000B300}, //reg_blue_mode[9:8]
    {0x155C, 0x00050000}, //0x00000000},
    {0x1548, 0x80080425}, //H13B : 0x00080324
    {0x16D8, 0x40100219}, //0x40102219}, //adaptive coef H13B : 0x03302211
    {0x154C, 0x00201300}, //0x00202300},
    {0x1554, 0x30202000}, //0x30202800},
    {0x16DC, 0x00050000}, //0x00058800}, //H13B : 0x00000000, AV NTSC HomeSphoping
    {0x16E0, 0x10070044}, //H13B : 0x00000000
    {0x16E4, 0x20000942}, //H13B : 0x00000000    
    {0x1550, 0x00080010},
    {0x1558, 0x00080004}, //H13B : 0x00000000    
    {0x1568, 0x00000001},
    {0x15B4, 0x40000000},
    {0x16EC, 0x00000014},
    {0x16E8, 0x020030C0}, //0x200040C0},
    {0x16F0, 0x80FFC0FF},
    {0x15D8, 0x30204010},
    {0x15DC, 0xB0808060},
    {0x15E0, 0x20004010},
    {0x15E4, 0xB0808040},
    {0x15C0, 0x0D000060},
    {0x15C4, 0x000008FF},
    {0x1640, 0x80FFFFFF},
    {0x1644, 0x00FF80FF},
    {0x16BC, 0xA0E0FFE0},
    {0x1740, 0x40116002}, //H13B 0x40112002
    {0x1744, 0x18000800}, //0x17E10800},
    {0x1748, 0x00201051},
    {0x174C, 0x1C4F1000}, //0x1F4F1000},
    {0x1764, 0x404C0200}, //0x404C1500},
    {0x1750, 0x1C1C1C1C}, //0x18171716},
    {0x1788, 0x25211914},
    {0x1784, 0x2D000000},
    {0x1758, 0x70003510},
    {0x175C, 0x20044120}, //0x00044120, //0x060440A0}, //0x360840A0},
    {0x1760, 0x04000310}, //0x04000010, 0x30003F0B},
    {0x15BC, 0x08002021},
    {0x16F8, 0x5001F181},
    {0x16FC, 0x00210200},
    {0x170C, 0x523D1901},
    {0x1704, 0x00003027},
    {0x1700, 0x1F140A04},
    {0x1708, 0x00008048},
    {0x15EC, 0x081020FF}, // 0x008008FF}, // 0x08FF20FF Ref4 peaking unblance
    {0x15F0, 0x80FFFFFF}, // 0x10FF20FF},
    {0x1590, 0x0C002438},
    {0x1594, 0x48FF99FF},
    {0x159C, 0x20100013}, // 0x0010001B}, // ST-Y
    {0x172C, 0x00002410}, // 0x0C002408}, //0x0C082408}, // 0x0C002440},
    {0x1730, 0x48109904}, // 0x48089900}, //0x48089908}, // 0x48009900},
    {0x15A0, 0x00000000},
    {0x15A4, 0x00000000},
    {0x15AC, 0x20200000},
    {0x1738, 0x0C082408},
    {0x173C, 0x48089908},
    {0x15A8, 0x86003FFF},
    {0x1418, 0x00001110},
    {0x1410, 0x00000000},
    {0x141C, 0x00010000}, //AA_9p0_F
    {0x1420, 0xFC1EF9CE},
    {0x1424, 0x00001098},
    {0x1428, 0x23862BEC},    
    {0x139C, 0x0011038A},
    {0x13A0, 0x01031F8F},
    {0x13A4, 0x1FD00010},
    {0x13A8, 0x00000003},
    {0x13AC, 0x00010380},
    {0x13B0, 0x00EE1FA9},
    {0x13B4, 0x1FE60003},
    {0x13B8, 0x00000000},
    {0x13BC, 0x00010100},
    {0x13C0, 0x0077E77C},
    {0x13C4, 0x0000077C},
    {0x13C8, 0x0077C000},
    {0x13CC, 0x0077C77E},
    {0x13D0, 0x00020020},
    {0x13D4, 0x00002000},
    {0x13D8, 0x00030000}, //H14 0x00020000}, //{0x13D8, 0x00030000},
    {0x160C, 0x007FF00C},
    {0x1610, 0x007C0135},
    {0x1614, 0x001357C0},
    {0x1618, 0x0000C7FF},
    {0x13DC, 0x0F0007FC}, //H13B 0x0F7C003F},   // NTSC-J OADJ
    {0x13E0, 0x00000F14}, //H13B 0x00000EEC},
    {0x13E4, 0x00600202},
    {0x13E8, 0x10001000},
    {0x13EC, 0x06002020}
    //{0x13DC, 0x0F0007FA}, //H13B 0x0F7C003C},   // NTSC-M OADJ
    //{0x13E0, 0x00000F1C}, //H13B 0x00000EF8},
    //{0x13E4, 0x00600202},
    //{0x13E8, 0x10001000},
    //{0x13EC, 0x06002020}
};