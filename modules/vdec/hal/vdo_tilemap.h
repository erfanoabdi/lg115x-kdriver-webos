typedef enum {
	TILED_MAP_LINEAR  = 0,
	TILED_MAP_FRAME_V = 1,
	TILED_MAP_MIXED_V = 2,
#if defined(CHIP_NAME_d13)
	TILED_MAP_FRAME_V_HEVC = 3,
#endif
	TILED_MAP_MAX
} TILED_MAP_T;

typedef struct {
	UINT32		u32Data[36];
	UINT32		u32Config;
	UINT32		u32Data2[32];
} TILED_MAP_CONFIG_T;

const static TILED_MAP_CONFIG_T	_gastTiledMapConfig[TILED_MAP_MAX][2] =
{
	{	// TILED_MAP_LINEAR
		{	// < 1024
			{
				0x00004040, 0x00004040, 0x00004040, 0x00004040,		/*XY2CA*/
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,

				0x00004040, 0x00004040, 0x00004040, 0x00004040,		/*XY2BA*/

				0x00004040, 0x00004040, 0x00004040, 0x00004040,		/*XY2RA*/
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040
			},
			0x00000000,		/*XY2RBC*/
			{
				0x00000104, 0x8114C900, 0x00000012, 0x00000000,		/*RBC2AXI*/
				0xCA807600, 0x8114C900, 0xCA92B400, 0x8114CD18,
				0x8114C900, 0xCA92B4F8, 0x7A358200, 0x0000000A,
				0x3B9ACA00, 0x00000000, 0x8079E8D0, 0x00000000,
				0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xCA87DDC0,
				0x00000001, 0x00000000, 0xC9D7C540, 0x00000005,
				0x8114C900, 0xC78320F8, 0x00000001, 0x00000000,
				0x00000000, 0xFFFFFFFF, 0x00000144, 0x00000000
			}
		},
		{	// > 1024
			{	0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,

				0x00004040, 0x00004040, 0x00004040, 0x00004040,

				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040
			},
			0x00000000,
			{
				0x00000104, 0x8114C900, 0x00000012, 0x00000000,
				0xCA807600, 0x8114C900, 0xCA92B400, 0x8114CD18,
				0x8114C900, 0xCA92B4F8, 0x7A358200, 0x0000000A,
				0x3B9ACA00, 0x00000000, 0x8079E8D0, 0x00000000,
				0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xCA87DDC0,
				0x00000001, 0x00000000, 0xC9D7C540, 0x00000005,
				0x8114C900, 0xC78320F8, 0x00000001, 0x00000000,
				0x00000000, 0xFFFFFFFF, 0x00000144, 0x00000000
			}
		}
	},
	{	// TILED_MAP_FRAME_V
		{	// < 1024
			{
				0x00003030, 0x00001010, 0x00001111, 0x00001212,
				0x00001313, 0x00000303, 0x00000404, 0x00000505,
				0x00000606, 0x00001414, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,

				0x00000787, 0x00000888, 0x00001595, 0x00004040,

				0x00000909, 0x00001616, 0x00001717, 0x00001818,
				0x00001919, 0x00001A1A, 0x00001B1B, 0x00001C1C,
				0x00001D1D, 0x00001E1E, 0x00001F1F, 0x00003030,
				0x00004040, 0x00004040, 0x00004040, 0x00004040
			},
			0x0003F0F0,
			{
				0x00000C30, 0x00000C30, 0x00000000, 0x00000041,
				0x00000082, 0x000000C3, 0x00000104, 0x00000145,
				0x00000186, 0x000001C7, 0x00000208, 0x00000249,
				0x00000410, 0x00000451, 0x00000492, 0x00000820,
				0x00000861, 0x000008A2, 0x000008E3, 0x00000924,
				0x00000965, 0x000009A6, 0x000009E7, 0x00000A28,
				0x00000A69, 0x00000AAA, 0x00000AEB, 0x00000B2C,
				0x00000C30, 0x00000C30, 0x00000C30, 0x00000C30
			}
		},
		{	// > 1024
			{
				0x00003030, 0x00001010, 0x00001111, 0x00001212,
				0x00001313, 0x00000303, 0x00000404, 0x00000505,
				0x00000606, 0x00001414, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,

				0x00000787, 0x00000888, 0x00001595, 0x00004040,

				0x00000909, 0x00000A0A, 0x00001616, 0x00001717,
				0x00001818, 0x00001919, 0x00001A1A, 0x00001B1B,
				0x00001C1C, 0x00001D1D, 0x00001E1E, 0x00001F1F,
				0x00004040, 0x00004040, 0x00004040, 0x00004040
			},
			0x0003F0F0,
			{
				0x00000C30, 0x00000C30, 0x00000000, 0x00000041,
				0x00000082, 0x000000C3, 0x00000104, 0x00000145,
				0x00000186, 0x000001C7, 0x00000208, 0x00000249,
				0x00000410, 0x00000451, 0x00000492, 0x00000820,
				0x00000861, 0x000008A2, 0x000008E3, 0x00000924,
				0x00000965, 0x000009A6, 0x000009E7, 0x00000A28,
				0x00000A69, 0x00000AAA, 0x00000AEB, 0x00000B2C,
				0x00000C30, 0x00000C30, 0x00000C30, 0x00000C30
			}
		}
	},
	{	// TILED_MAP_MIXED_V
		{	// < 1024
			{
				0x00003030, 0x00001111, 0x00001212, 0x00001313,
				0x00001010, 0x00000303, 0x00000404, 0x00000505,
				0x00000606, 0x00001414, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,

				0x00000787, 0x00000888, 0x00001595, 0x00004040,

				0x00000909, 0x00001616, 0x00001717, 0x00001818,
				0x00001919, 0x00001A1A, 0x00001B1B, 0x00001C1C,
				0x00001D1D, 0x00001E1E, 0x00001F1F, 0x00003030,
				0x00004040, 0x00004040, 0x00004040, 0x00004040
			},
			0x00077777,
			{
				0x00000C30, 0x00000C30, 0x00000000, 0x00000041,
				0x00000082, 0x000000C3, 0x00000104, 0x00000145,
				0x00000186, 0x000001C7, 0x00000208, 0x00000249,
				0x00000410, 0x00000451, 0x00000492, 0x00000820,
				0x00000861, 0x000008A2, 0x000008E3, 0x00000924,
				0x00000965, 0x000009A6, 0x000009E7, 0x00000A28,
				0x00000A69, 0x00000AAA, 0x00000AEB, 0x00000B2C,
				0x00000C30, 0x00000C30, 0x00000C30, 0x00000C30
			}
		},
		{	// > 1024
			{	0x00003030, 0x00001111, 0x00001212, 0x00001313,
				0x00001010, 0x00000303, 0x00000404, 0x00000505,
				0x00000606, 0x00001414, 0x00004040, 0x00004040,
				0x00004040, 0x00004040, 0x00004040, 0x00004040,

				0x00000787, 0x00000888, 0x00001595, 0x00004040,

				0x00000909, 0x00000A0A, 0x00001616, 0x00001717,
				0x00001818, 0x00001919, 0x00001A1A, 0x00001B1B,
				0x00001C1C, 0x00001D1D, 0x00001E1E, 0x00001F1F,
				0x00004040, 0x00004040, 0x00004040, 0x00004040
			},
			0x00077777,
			{
				0x00000C30, 0x00000C30, 0x00000000, 0x00000041,
				0x00000082, 0x000000C3, 0x00000104, 0x00000145,
				0x00000186, 0x000001C7, 0x00000208, 0x00000249,
				0x00000410, 0x00000451, 0x00000492, 0x00000820,
				0x00000861, 0x000008A2, 0x000008E3, 0x00000924,
				0x00000965, 0x000009A6, 0x000009E7, 0x00000A28,
				0x00000A69, 0x00000AAA, 0x00000AEB, 0x00000B2C,
				0x00000C30, 0x00000C30, 0x00000C30, 0x00000C30
			}
		}
	}
};

