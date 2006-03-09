/********************************************************************
 Eolith 32 bits hardware: Gradation 2D system

 driver by Tomasz Slanina    analog [at] op.pl
 and       Pierpaolo Prazzoli

 Main CPU:
  Hyperstone E1-32N @ 50MHz

  Sound CPU:
   80c301/AT89c52

  Sound:
   General MIDI Chipset QDSP 1000 MIDI Player (80c32 CPU)
   MIDI 16th Channel(32 Poly) using as Effect EPROM : 512Kbytes
   MIDI Background Music EPROM : 512Kbyte
     TDA1519A(Philips)Stereo Power AMP

  Video:
   16 bit True Color/Dot
   Resolution - 320x340 or 512x384

  Memory:
    256KByte x2 VRAM
    512Kbyte/1Mega main RAM

 Games dumped
 - Hidden Catch (pcb ver 3.03)
 - Land Breaker (pcb ver 3.03) (MCU internal flash dump is missing)
 - Land Breaker (pcb ver 3.02)
 - Raccoon World
 - Fortress 2 Blue Arcade (v. 1.01 / pcb ver 3.05)
 - Fortress 2 Blue Arcade (v. 1.00 / pcb ver 3.05)

 Known games not dumped
 - Hidden Catch (pcb ver 3.02)
 - Fortress 2 Blue Arcade (v. 1.02)
 - Ribbon (Step1. Mild Mind) (c) 1999

 TODO:
 - sound & sound cpu

 *********************************************************************/

#include "driver.h"
#include "machine/eeprom.h"

READ32_HANDLER(eolith_vram_r);
WRITE32_HANDLER(eolith_vram_w);
VIDEO_START(eolith);
VIDEO_UPDATE(eolith);

extern int eolith_buffer;

static int coin_counter_bit = 0;

// It's configured for 512 bytes
static struct EEPROM_interface eeprom_interface_93C66 =
{
	9,				// address bits 9
	8,				// data bits    8
	"*110",			// read         110 aaaaaaaaa
	"*101",			// write        101 aaaaaaaaa dddddddd
	"*111",			// erase        111 aaaaaaaaa
	"*10000xxxxxx",	// lock         100 00xxxxxxx
	"*10011xxxxxx"	// unlock       100 11xxxxxxx
};



static NVRAM_HANDLER( eolith )
{
	if (read_or_write)
		EEPROM_save(file);
	else
	{
		EEPROM_init(&eeprom_interface_93C66);
		if (file)	EEPROM_load(file);
	}
}


static READ32_HANDLER( eeprom_r )
{
	/*
        bit 3 = eeprom bit
        bit 6 = vblank flag

        Are these used only in landbrka ?
        bit 8 = ???
        bit 9 = ???
    */

	return (readinputport(0) & ~0x308) | (EEPROM_read_bit() << 3) | (mame_rand() & 0x300);
}

static WRITE32_HANDLER( systemcontrol_w )
{
	eolith_buffer = (data & 0x80) >> 7;
	coin_counter_w(0, data & coin_counter_bit);
	set_led_status(0, data & 1);

	EEPROM_write_bit(data & 0x08);
	EEPROM_set_cs_line((data & 0x02) ? CLEAR_LINE : ASSERT_LINE);
	EEPROM_set_clock_line((data & 0x04) ? ASSERT_LINE : CLEAR_LINE);

	// bit 0x100 and 0x040 ?
}

static ADDRESS_MAP_START( eolith_map, ADDRESS_SPACE_PROGRAM, 32 )
	AM_RANGE(0x00000000, 0x001fffff) AM_RAM // fort2b wants ram here
	AM_RANGE(0x40000000, 0x401fffff) AM_RAM
	AM_RANGE(0x90000000, 0x9003ffff) AM_READWRITE(eolith_vram_r, eolith_vram_w)
	AM_RANGE(0xfc000000, 0xfc000003) AM_READ(eeprom_r)
	AM_RANGE(0xfc400000, 0xfc400003) AM_WRITE(systemcontrol_w)
	AM_RANGE(0xfc800000, 0xfc800003) AM_WRITENOP // sound latch
	AM_RANGE(0xfcc00000, 0xfcc0005b) AM_WRITENOP // crt registers ?
	AM_RANGE(0xfca00000, 0xfca00003) AM_READ(input_port_1_dword_r)
	AM_RANGE(0xfd000000, 0xfeffffff) AM_ROM AM_REGION(REGION_USER1, 0)
	AM_RANGE(0xfff80000, 0xffffffff) AM_ROM AM_REGION(REGION_CPU1, 0)
ADDRESS_MAP_END

static INPUT_PORTS_START( common )
	PORT_START_TAG("IN0")
	PORT_BIT( 0x00000001, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x00000002, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x00000004, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x00000008, IP_ACTIVE_LOW, IPT_SPECIAL ) // eeprom bit
	PORT_BIT( 0x00000010, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x00000020, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x00000040, IP_ACTIVE_LOW, IPT_VBLANK )
	PORT_BIT( 0x00003f80, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x00004000, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_SERVICE_NO_TOGGLE( 0x00008000, IP_ACTIVE_LOW )
	PORT_BIT( 0x00010000, IP_ACTIVE_LOW, IPT_JOYSTICK_UP	) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x00020000, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN	) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x00040000, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT	) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x00080000, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT	) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x00100000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x00200000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)
	PORT_BIT( 0x00400000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x00800000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x01000000, IP_ACTIVE_LOW, IPT_JOYSTICK_UP	) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x02000000, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN	) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x04000000, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT	) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x08000000, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT	) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x10000000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x20000000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x40000000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80000000, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START_TAG("DSW1")
	PORT_BIT( 0xffffffff, IP_ACTIVE_LOW, IPT_UNUSED	)
INPUT_PORTS_END

INPUT_PORTS_START( hidnctch )
	PORT_INCLUDE(common)
	PORT_MODIFY("IN0")
	PORT_BIT( 0x00000020, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x00000001, 0x00000001, "Show Settings" )
	PORT_DIPSETTING(          0x00000001, DEF_STR( Off ) )
	PORT_DIPSETTING(          0x00000000, DEF_STR( On ) )
	PORT_DIPNAME( 0x00000002, 0x00000002, "Show Counters" )
	PORT_DIPSETTING(          0x00000002, DEF_STR( Off ) )
	PORT_DIPSETTING(          0x00000000, DEF_STR( On ) )
	PORT_BIT( 0xfffffffc, IP_ACTIVE_LOW, IPT_UNUSED	)
INPUT_PORTS_END

INPUT_PORTS_START( raccoon )
	PORT_INCLUDE(common)
	PORT_MODIFY("IN0")
	PORT_BIT( 0x00008000, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x0000000f, 0x0000000f, DEF_STR( Coinage ) )
	PORT_DIPSETTING(          0x0000000d, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(          0x0000000e, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(          0x0000000c, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(          0x0000000f, DEF_STR( 1C_1C ) )
	// other values are just mirrors
	PORT_BIT( 0xfffffff0, IP_ACTIVE_LOW, IPT_UNUSED	)
INPUT_PORTS_END

INPUT_PORTS_START( landbrk )
	PORT_INCLUDE(common)
	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x00000001, 0x00000001, "Show Settings" )
	PORT_DIPSETTING(          0x00000001, DEF_STR( Off ) )
	PORT_DIPSETTING(          0x00000000, DEF_STR( On ) )
	PORT_DIPNAME( 0x00000002, 0x00000002, "Show Counters" )
	PORT_DIPSETTING(          0x00000002, DEF_STR( Off ) )
	PORT_DIPSETTING(          0x00000000, DEF_STR( On ) )
	PORT_DIPNAME( 0x00000004, 0x00000004, DEF_STR( Unknown ) )
	PORT_DIPSETTING(          0x00000004, DEF_STR( Off ) )
	PORT_DIPSETTING(          0x00000000, DEF_STR( On ) )
	PORT_DIPNAME( 0x00000008, 0x00000008, DEF_STR( Unknown ) )
	PORT_DIPSETTING(          0x00000008, DEF_STR( Off ) )
	PORT_DIPSETTING(          0x00000000, DEF_STR( On ) )
	PORT_BIT( 0xfffffff0, IP_ACTIVE_LOW, IPT_UNUSED	)
INPUT_PORTS_END

static MACHINE_DRIVER_START( eolith )
	MDRV_CPU_ADD(E132N, 50000000)		 /* 50 MHz */
	MDRV_CPU_PROGRAM_MAP(eolith_map,0)

	/* sound cpu */

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_REAL_60HZ_VBLANK_DURATION)

	MDRV_NVRAM_HANDLER(eolith)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER | VIDEO_RGB_DIRECT)
	MDRV_SCREEN_SIZE(512, 512)
	MDRV_VISIBLE_AREA(0, 319, 0, 239)
	MDRV_PALETTE_LENGTH(32768)

	MDRV_VIDEO_START(eolith)
	MDRV_VIDEO_UPDATE(eolith)

	/* sound hardware */
MACHINE_DRIVER_END

/*

Name         Size     CRC32       Chip Type
-------------------------------------------
hc0_u39.bin  4194304  0xeefb6add  C32000 dumped as SGS 27C322
hc1_u34.bin  4194304  0x482f3e52  C32000 dumped as SGS 27C322
hc2_u40.bin  4194304  0x914a1544  C32000 dumped as SGS 27C322
hc3_u35.bin  4194304  0x80c59133  C32000 dumped as SGS 27C322
hc4_u41.bin  4194304  0x9a9e2203  C32000 dumped as SGS 27C322
hc5_u36.bin  4194304  0x74b1719d  C32000 dumped as SGS 27C322
hc_u108.bin   524288  0x2bae46cb  27C040
hc_u43.bin    524288  0x635b4478  27C040
hc_u97.bin    524288  0xebf9f77b  27C040
hc_u107.bin    32768  0xafd5263d  AMIC 275308 dumped as 27256
hc_u111.bin    32768  0x79012474  AMIC 275308 dumped as 27256

*/

ROM_START( hidnctch )
	ROM_REGION( 0x80000, REGION_CPU1, 0 ) /* Hyperstone CPU Code */
	ROM_LOAD( "hc_u43.bin", 0x00000, 0x80000, CRC(635b4478) SHA1(31ea4a9725e0c329447c7d221c22494c905f6940) )

	ROM_REGION32_BE( 0x2000000, REGION_USER1, ROMREGION_ERASE00 ) /* Game Data - banked ROM, swapping necessary */
	ROM_LOAD32_WORD_SWAP( "hc0_u39.bin", 0x0000000, 0x400000, CRC(eefb6add) SHA1(a0f6f2cf86699a666be0647274d8c9381782640d) )
	ROM_LOAD32_WORD_SWAP( "hc1_u34.bin", 0x0000002, 0x400000, CRC(482f3e52) SHA1(7a527c6af4c80e10cc25219a04ccf7c7ea1b23af) )
	ROM_LOAD32_WORD_SWAP( "hc2_u40.bin", 0x0800000, 0x400000, CRC(914a1544) SHA1(683cb007ace50d1ba88253da6ad71dc3a395299d) )
	ROM_LOAD32_WORD_SWAP( "hc3_u35.bin", 0x0800002, 0x400000, CRC(80c59133) SHA1(66ca4c2c014c4a1c87c46a3971732f0a2be95408) )
	ROM_LOAD32_WORD_SWAP( "hc4_u41.bin", 0x1000000, 0x400000, CRC(9a9e2203) SHA1(a90f5842b63696753e6c16114b1893bbeb91e45c) )
	ROM_LOAD32_WORD_SWAP( "hc5_u36.bin", 0x1000002, 0x400000, CRC(74b1719d) SHA1(fe2325259117598ad7c23217426ac9c28440e3a0) )
	// 0x1800000 - 0x1ffffff empty

	ROM_REGION( 0x008000, REGION_CPU2, 0 ) /* QDSP ('51) Code */
	ROM_LOAD( "hc_u107.bin", 0x0000, 0x8000, CRC(afd5263d) SHA1(71ace1b749d8a6b84d08b97185e7e512d04e4b8d) )

	ROM_REGION( 0x008000, REGION_CPU3, 0 )  /* Sound (80c301) CPU Code */
	ROM_LOAD( "hc_u111.bin", 0x0000, 0x8000, CRC(79012474) SHA1(09a2d5705d7bc52cc2d1644c87c1e31ee44813ef) )

	ROM_REGION( 0x080000, REGION_SOUND1, 0 ) /* Music data */
	ROM_LOAD( "hc_u108.bin", 0x00000, 0x80000, CRC(2bae46cb) SHA1(7c43f1002dfc20b9c1bb1647f7261dfa7ed2b4f9) )

	ROM_REGION( 0x080000, REGION_SOUND2, 0 ) /* QDSP samples (SFX) */
	ROM_LOAD( "hc_u97.bin", 0x00000, 0x80000, CRC(ebf9f77b) SHA1(5d472aeb84fc011e19b9e61d34aeddfe7d6ac216) )

	ROM_REGION( 0x080000, REGION_SOUND3, 0 ) /* QDSP wavetable rom */
	ROM_LOAD( "qs1001a.u97",  0x00000, 0x80000, NO_DUMP )
ROM_END


/*

Racoon World by Eolith

U43, u97, u108   are 27c040 devices

u111, u107   are 27c256 devices

On the ROM sub board:
u1, u2, u5, u10, u11, u14  are all 27c160 devices
--------------------------------------------------------------------------
Stereo sound?
24MHz crystal near the sound section

there is a 4 position DIP switch.

Hyperstone E1-32N    45.00000 MHz  near this chip
QDSP     QS1001A
QDSP     QS1000
EOLITH  EV0514-001  custom??   14.31818MHz  xtl near this chip
12MHz crystal is near the U111

U107 and U97 are mostlikely sound roms but not sure

*/

ROM_START( raccoon )
	ROM_REGION( 0x80000, REGION_CPU1, 0 ) /* Hyperstone CPU Code */
	ROM_LOAD( "racoon-u.43", 0x00000, 0x80000, CRC(711ee026) SHA1(c55dfaa24cbaa7a613657cfb25e7f0085f1e4cbf) )

	ROM_REGION32_BE( 0x2000000, REGION_USER1, ROMREGION_ERASE00 ) /* Game Data - banked ROM, swapping necessary */
 	ROM_LOAD32_WORD_SWAP( "racoon.u10", 0x0000000, 0x200000, CRC(f702390e) SHA1(47520ba0e6d3f044136a517ebbec7426a66ce33d) )
	ROM_LOAD32_WORD_SWAP( "racoon.u1",  0x0000002, 0x200000, CRC(49775125) SHA1(2b8ee9dd767465999c828d65bb02b8aaad94177c) )
 	ROM_LOAD32_WORD_SWAP( "racoon.u11", 0x0400000, 0x200000, CRC(3f23f368) SHA1(eb1ea51def2cde5e7e4f334888294b794aa03dfc) )
 	ROM_LOAD32_WORD_SWAP( "racoon.u2",  0x0400002, 0x200000, CRC(1eb00529) SHA1(d9af75e116f5237a3c6812538b77155b9c08dd5c) )
 	ROM_LOAD32_WORD_SWAP( "racoon.u14", 0x0800000, 0x200000, CRC(870fe45e) SHA1(f8d800b92eb1ee9ef4663319fd3cb1f5e52d0e72) )
 	ROM_LOAD32_WORD_SWAP( "racoon.u5",  0x0800002, 0x200000, CRC(5fbac174) SHA1(1d3e3f40a737d61ff688627891dec183af7fa19a) )
	// 0x0c00000 - 0x1ffffff empty

	ROM_REGION( 0x08000, REGION_CPU2, 0 ) /* QDSP ('51) Code */
	ROM_LOAD( "racoon-u.107", 0x0000, 0x8000, CRC(89450a2f) SHA1(d58efa805f497bec179fdbfb8c5860ac5438b4ec) )

	ROM_REGION( 0x08000, REGION_CPU3, 0 ) /* Sound (80c301) CPU Code */
	ROM_LOAD( "racoon-u.111", 0x0000, 0x8000, CRC(52f419ea) SHA1(79c9f135b0cf8b1928411faed9b447cd98a83287) )

	ROM_REGION( 0x080000, REGION_SOUND1, 0 ) /* Music data */
	ROM_LOAD( "racoon-u.108", 0x00000, 0x80000, CRC(fc4f30ee) SHA1(74b9e60cceb03ad572e0e080fbe1de5cffa1b2c3) )

	ROM_REGION( 0x080000, REGION_SOUND2, 0 ) /* QDSP samples (SFX) */
	ROM_LOAD( "racoon-u.97", 0x00000, 0x80000, CRC(fef828b1) SHA1(38352b67d18300db40113df9426c2aceec12a29b) )

	ROM_REGION( 0x080000, REGION_SOUND3, 0 ) /* QDSP wavetable rom */
	ROM_LOAD( "qs1001a.u97",  0x00000, 0x80000, NO_DUMP )
ROM_END

ROM_START( landbrk )
	ROM_REGION( 0x80000, REGION_CPU1, 0 ) /* Hyperstone CPU Code */
	ROM_LOAD( "rom3.u43", 0x00000, 0x80000, CRC(8429189a) SHA1(f38e4792426ca2c138c44053a6c7525906281dff) )

	ROM_REGION32_BE( 0x2000000, REGION_USER1, ROMREGION_ERASE00 ) /* Game Data - banked ROM, swapping necessary */
	ROM_LOAD32_WORD_SWAP( "00.bin", 0x0000000, 0x200000, CRC(a803aace) SHA1(52a9e27b4f400767a953aa01321a9fa7cdbf3976) )
	ROM_LOAD32_WORD_SWAP( "01.bin", 0x0000002, 0x200000, CRC(439c95cc) SHA1(18830024b2e43f0a89a7bd32841fbcb574e50fc6) )
	ROM_LOAD32_WORD_SWAP( "02.bin", 0x0400000, 0x200000, CRC(a0c2828c) SHA1(967dc467d25f093749aa0146ebf54959c9803b92) )
	ROM_LOAD32_WORD_SWAP( "03.bin", 0x0400002, 0x200000, CRC(5106b61a) SHA1(5e584ce5247d598e59d071579edb1da87fb2c9d6) )
	ROM_LOAD32_WORD_SWAP( "04.bin", 0x0800000, 0x200000, CRC(fbe4a215) SHA1(2dec06a8641ef3db2a5448c95677ee8d7b0dc0ad) )
	ROM_LOAD32_WORD_SWAP( "05.bin", 0x0800002, 0x200000, CRC(61d422b2) SHA1(c9774747c94cac44ffd3c1781374c009950b2add) )
	ROM_LOAD32_WORD_SWAP( "06.bin", 0x0c00000, 0x200000, CRC(bc4f7f30) SHA1(31e7a6077a823f16a645547c9c11640535e02eb3) )
	ROM_LOAD32_WORD_SWAP( "07.bin", 0x0c00002, 0x200000, CRC(d4d108ce) SHA1(d462690b782b363db5a062e6e04fe0c00e21bd46) )
	ROM_LOAD32_WORD_SWAP( "08.bin", 0x1000000, 0x200000, CRC(36237654) SHA1(38a4d6d195683128c05d8869ff4184d79d26fead) )
	ROM_LOAD32_WORD_SWAP( "09.bin", 0x1000002, 0x200000, CRC(9171828b) SHA1(648a398654bfba2e7681a3eee61e3803538929b5) )
	ROM_LOAD32_WORD_SWAP( "10.bin", 0x1400000, 0x200000, CRC(fe51744d) SHA1(2f6fb5cc7a52d2fa3459b9b2cf3450a227c19643) )
	ROM_LOAD32_WORD_SWAP( "11.bin", 0x1400002, 0x200000, CRC(fe6fd5e0) SHA1(66442b0d308517ebf0e13653cf4289ee9e9e0741) )
	ROM_LOAD32_WORD_SWAP( "12.bin", 0x1800000, 0x200000, CRC(9f0f17f3) SHA1(c280dd5127ea6d1585f8b7ad753de45e1beaca8f) )
	ROM_LOAD32_WORD_SWAP( "13.bin", 0x1800002, 0x200000, CRC(c7d51bbe) SHA1(a399087496f1c3f4c4b9e9b97e97585302ff29e5) )
	ROM_LOAD32_WORD_SWAP( "14.bin", 0x1c00000, 0x200000, CRC(f30f7bc5) SHA1(8b3e002b773a88229d013e1d95336736a259c698) )
	ROM_LOAD32_WORD_SWAP( "15.bin", 0x1c00002, 0x200000, CRC(bc1664e3) SHA1(a13a4e1df8d9825a72ecca1552ee52958c0d33d8) )

	ROM_REGION( 0x008000, REGION_CPU2, 0 ) /* QDSP ('51) Code */
	ROM_LOAD( "rom5.u107", 0x0000, 0x8000, CRC(afd5263d) SHA1(71ace1b749d8a6b84d08b97185e7e512d04e4b8d) )

	ROM_REGION( 0x008000, REGION_CPU3, 0 ) /* Sound (80c301) CPU Code */
	ROM_LOAD( "rom1.u111", 0x0000, 0x8000, CRC(79012474) SHA1(09a2d5705d7bc52cc2d1644c87c1e31ee44813ef) )

	ROM_REGION( 0x080000, REGION_SOUND1, 0 ) /* Music data */
	ROM_LOAD( "rom2.u108", 0x00000, 0x80000, CRC(f3b327ef) SHA1(4b359171afd6ca10275961f795f3fe64f9df9897) )

	ROM_REGION( 0x080000, REGION_SOUND2, 0 ) /* QDSP samples (SFX) */
	ROM_LOAD( "rom4.u97", 0x00000, 0x80000, CRC(5b34dff0) SHA1(1668763e977e272781ddcc74beba97b53477cc9d) )

	ROM_REGION( 0x080000, REGION_SOUND3, 0 ) /* QDSP wavetable rom */
	ROM_LOAD( "qs1001a.u97",  0x00000, 0x80000, NO_DUMP )
ROM_END

/*

Documentation
-------------------------------------------
lb_pcb.jpg    614606  0xf041e24c

Name         Size     CRC32       Chip Type
-------------------------------------------
lb.107         32768  0xafd5263d  AMIC 275308 dumped as 27256
lb2-000.u39  4194304  0xb37faf7a  C32000 dumped as SGS 27C322
lb2-001.u34  4194304  0x07e620c9  C32000 dumped as SGS 27C322
lb2-002.u40  4194304  0x3bb4bca6  C32000 dumped as SGS 27C322
lb2-003.u35  4194304  0x28ce863a  C32000 dumped as SGS 27C322
lb2-004.u41  4194304  0xcbe84b06  C32000 dumped as SGS 27C322
lb2-005.u36  4194304  0x350c77a3  C32000 dumped as SGS 27C322
lb2-006.u42  4194304  0x22c57cd8  C32000 dumped as SGS 27C322
lb2-007.u37  4194304  0x31f957b3  C32000 dumped as SGS 27C322
lb_1.u43      524288  0xf8bbcf44  27C040
lb_2.108      524288  0xa99182d7  27C040
lb_3.u97      524288  0x5b34dff0  27C040

*/

ROM_START( landbrka )
	ROM_REGION( 0x80000, REGION_CPU1, 0 ) /* Hyperstone CPU Code */
	ROM_LOAD( "lb_1.u43", 0x00000, 0x80000, CRC(f8bbcf44) SHA1(ad85a890ae2f921cd08c1897b4d9a230ccf9e072) )

	ROM_REGION32_BE( 0x2000000, REGION_USER1, ROMREGION_ERASE00 ) /* Game Data - banked ROM, swapping necessary */
	ROM_LOAD32_WORD_SWAP( "lb2-000.u39", 0x0000000, 0x400000, CRC(b37faf7a) SHA1(30e9af3957ada7c72d85f55add221c2e9b3ea823) )
	ROM_LOAD32_WORD_SWAP( "lb2-001.u34", 0x0000002, 0x400000, CRC(07e620c9) SHA1(19f95316208fb4e52cef78f18c5d93460a644566) )
	ROM_LOAD32_WORD_SWAP( "lb2-002.u40", 0x0800000, 0x400000, CRC(3bb4bca6) SHA1(115029be4a4e322549a35f3ae5093ec161e9a421) )
	ROM_LOAD32_WORD_SWAP( "lb2-003.u35", 0x0800002, 0x400000, CRC(28ce863a) SHA1(1ba7d8be0ed4459dbdf99df18a2ad817904b9f04) )
	ROM_LOAD32_WORD_SWAP( "lb2-004.u41", 0x1000000, 0x400000, CRC(cbe84b06) SHA1(52505939fb88cd24f409c795fe5ceed5b41a52c2) )
	ROM_LOAD32_WORD_SWAP( "lb2-005.u36", 0x1000002, 0x400000, CRC(350c77a3) SHA1(231e65ea7db19019615a8aa4444922bcd5cf9e5c) )
	ROM_LOAD32_WORD_SWAP( "lb2-006.u42", 0x1800000, 0x400000, CRC(22c57cd8) SHA1(c9eb745523005876395ff7f0b3e996994b3f1220) )
	ROM_LOAD32_WORD_SWAP( "lb2-007.u37", 0x1800002, 0x400000, CRC(31f957b3) SHA1(ab1c4c50c2d5361ba8db047feb714423d84e6df4) )

	ROM_REGION( 0x008000, REGION_CPU2, 0 ) /* QDSP ('51) Code */
	ROM_LOAD( "lb.107", 0x0000, 0x8000, CRC(afd5263d) SHA1(71ace1b749d8a6b84d08b97185e7e512d04e4b8d) )

	ROM_REGION( 0x000800, REGION_CPU3, 0 ) /* AT89c52 */
	ROM_LOAD( "lb.103", 0x0000, 0x0800, NO_DUMP ) /* MCU internal 8K flash */

	ROM_REGION( 0x080000, REGION_SOUND1, 0 ) /* Music data */
	ROM_LOAD( "lb_2.108", 0x00000, 0x80000, CRC(a99182d7) SHA1(628c8d09efb3917a4e97d9e02b6b0ca1f339825d) )

	ROM_REGION( 0x080000, REGION_SOUND2, 0 ) /* QDSP samples (SFX) */
	ROM_LOAD( "lb_3.u97", 0x00000, 0x80000, CRC(5b34dff0) SHA1(1668763e977e272781ddcc74beba97b53477cc9d) )

	ROM_REGION( 0x080000, REGION_SOUND3, 0 ) /* QDSP wavetable rom */
	ROM_LOAD( "qs1001a.u97",  0x00000, 0x80000, NO_DUMP )
ROM_END


/* Fortress 2 Blue */

ROM_START( fort2b )
	ROM_REGION( 0x80000, REGION_CPU1, 0 ) /* Hyperstone CPU Code */
	ROM_LOAD( "1.u43",        0x00000, 0x80000, CRC(b2279485) SHA1(022591b260be28820f04a1c1fdd61cb9b68d6703) )

	ROM_REGION32_BE( 0x2000000, REGION_USER1, ROMREGION_ERASE00 ) /* Game Data - banked ROM, swapping necessary */
	ROM_LOAD32_WORD_SWAP( "00.u5", 0x0000000, 0x200000, CRC(4437b595) SHA1(b87518110955947264d93b1f377289f1741ce5dc) )
	ROM_LOAD32_WORD_SWAP( "01.u1", 0x0000002, 0x200000, CRC(2a410aed) SHA1(def822ead339180aa3e0ebb266b6a6eb1271a2ae) )
	ROM_LOAD32_WORD_SWAP( "02.u6", 0x0400000, 0x200000, CRC(12f0e4c0) SHA1(fa1e1c3510af61b4058507f1aca801377cafffb4) )
	ROM_LOAD32_WORD_SWAP( "03.u2", 0x0400002, 0x200000, CRC(aaa7c45a) SHA1(fa06932ab1d41eddd384785f1b2b4dc70046da0f) )
	ROM_LOAD32_WORD_SWAP( "04.u7", 0x0800000, 0x200000, CRC(428070d2) SHA1(3e25c644be28fd64e8ea60a5b6c675ce2a84ec91) )
	ROM_LOAD32_WORD_SWAP( "05.u3", 0x0800002, 0x200000, CRC(a66f9ba9) SHA1(3984c42358403b692b7e09b5849c2444578305f0) )
	ROM_LOAD32_WORD_SWAP( "06.u8", 0x0c00000, 0x200000, CRC(899d318e) SHA1(3bd552eae9985928fe211186f1913c155cbde1a5) )
	ROM_LOAD32_WORD_SWAP( "07.u4", 0x0c00002, 0x200000, CRC(c4644798) SHA1(3debeef3abc6ce4aea5eb0c6be9f99689a9f111c) )
	ROM_LOAD32_WORD_SWAP( "08.u15", 0x1000000, 0x200000, CRC(ce0cccfc) SHA1(42cb6698e9168712699616d46ece5a6482825e87) )
	ROM_LOAD32_WORD_SWAP( "09.u10", 0x1000002, 0x200000, CRC(5b7de0f1) SHA1(f0ee1c6d0766fec95d77d46a9c68b8c3b0d09dde) )
	ROM_LOAD32_WORD_SWAP( "10.u16", 0x1400000, 0x200000, CRC(b47fc014) SHA1(e1094bc8a3edff635c4abe311a7902a714b9102c) )
	ROM_LOAD32_WORD_SWAP( "11.u11", 0x1400002, 0x200000, CRC(7113d3f9) SHA1(5c34b2b21bb09c5dec075fabfd426c143a0a8dc2) )
	ROM_LOAD32_WORD_SWAP( "12.u17", 0x1800000, 0x200000, CRC(8c4b63a6) SHA1(2493c9e9547cb06fbdc6240fb67b15047b66a5fd) )
	ROM_LOAD32_WORD_SWAP( "13.u12", 0x1800002, 0x200000, CRC(1d9b9995) SHA1(e18b93c244d8e959053dff66e2e5e4341e2b8034) )
	ROM_LOAD32_WORD_SWAP( "14.u18", 0x1c00000, 0x200000, CRC(450fa784) SHA1(d1244b94be6571f1f8930154778362af5cd4c334) )
	ROM_LOAD32_WORD_SWAP( "15.u13", 0x1c00002, 0x200000, CRC(c1f02d5c) SHA1(73fe9e654c097cd57863b49545c6aa05996a7645) )

	ROM_REGION( 0x008000, REGION_CPU2, 0 ) /* QDSP ('51) Code */
	ROM_LOAD( "5.u107",       0x00000, 0x08000, CRC(afd5263d) SHA1(71ace1b749d8a6b84d08b97185e7e512d04e4b8d) )

	ROM_REGION( 0x08000, REGION_CPU3, 0 ) /* Sound (80c301) CPU Code */
	ROM_LOAD( "4.u111",       0x00000, 0x08000, CRC(79012474) SHA1(09a2d5705d7bc52cc2d1644c87c1e31ee44813ef) )

	ROM_REGION( 0x080000, REGION_SOUND1, 0 ) /* Music data */
	ROM_LOAD( "3.u108",       0x00000, 0x80000, CRC(9b996b60) SHA1(c4e34601f754ae2908dd6d59ea9da0c5c6f56f2d) )

	ROM_REGION( 0x080000, REGION_SOUND2, 0 ) /* QDSP samples (SFX) */
	ROM_LOAD( "2.u97",        0x00000, 0x80000, CRC(8a431b14) SHA1(5a9824280f30ef2e7b7f16652b2f9f9559cb764f) )

	ROM_REGION( 0x080000, REGION_SOUND3, 0 ) /* QDSP wavetable rom */
	ROM_LOAD( "qs1001a.u97",  0x00000, 0x80000, NO_DUMP )
ROM_END

ROM_START( fort2ba )
	ROM_REGION( 0x80000, REGION_CPU1, 0 ) /* Hyperstone CPU Code */
	ROM_LOAD( "ftii012.u43", 0x00000, 0x80000, CRC(6424e05f) SHA1(2f02f103de180561e372ce897f8410a11c4cb58d) )

	ROM_REGION32_BE( 0x2000000, REGION_USER1, ROMREGION_ERASE00 ) /* Game Data - banked ROM, swapping necessary */
	ROM_LOAD32_WORD_SWAP( "ftii000.u39", 0x0000000, 0x400000, CRC(be74121d) SHA1(ee072044f84e11c48537d79bd9766bf8cc28f002) )
	ROM_LOAD32_WORD_SWAP( "ftii004.u34", 0x0000002, 0x400000, CRC(d4399f98) SHA1(88f5a1097f44d2070cfc96c9cd83342d1975dcfe) )
	ROM_LOAD32_WORD_SWAP( "ftii001.u40", 0x0800000, 0x400000, CRC(35c396ff) SHA1(d05dee021e1a93e224b05949c18a5107e0aceb4d) )
	ROM_LOAD32_WORD_SWAP( "ftii005.u35", 0x0800002, 0x400000, CRC(ff553679) SHA1(d9f1ebc28098a20a59b76ee859527e946c82a1df) )
	ROM_LOAD32_WORD_SWAP( "ftii002.u41", 0x1000000, 0x400000, CRC(1d79ed5a) SHA1(0319392dbfe4a20be462ca9cc5a66575ba0a32b4) )
	ROM_LOAD32_WORD_SWAP( "ftii006.u36", 0x1000002, 0x400000, CRC(c6049bbc) SHA1(bd9231b5fe1a7307668960c1f9f188f4a49f1c45) )
	ROM_LOAD32_WORD_SWAP( "ftii003.u42", 0x1800000, 0x400000, CRC(3cac1efe) SHA1(aca009a39b5d6049e3cf234f4412868a569ffb18) )
	ROM_LOAD32_WORD_SWAP( "ftii007.u37", 0x1800002, 0x400000, CRC(a583a672) SHA1(b013fbfaa1e3f573a305e6346e50b930766daa1d) )

	ROM_REGION( 0x008000, REGION_CPU2, 0 ) /* QDSP ('51) Code */
	ROM_LOAD( "ftii010.u107", 0x0000, 0x8000, CRC(afd5263d) SHA1(71ace1b749d8a6b84d08b97185e7e512d04e4b8d) )

	ROM_REGION( 0x08000, REGION_CPU3, 0 ) /* Sound (80c301) CPU Code */
	ROM_LOAD( "ftii008.u111", 0x0000, 0x8000, CRC(79012474) SHA1(09a2d5705d7bc52cc2d1644c87c1e31ee44813ef) )

	ROM_REGION( 0x080000, REGION_SOUND1, 0 ) /* Music data */
	ROM_LOAD( "ftii009.u108", 0x00000, 0x80000,  CRC(9b996b60) SHA1(c4e34601f754ae2908dd6d59ea9da0c5c6f56f2d) )

	ROM_REGION( 0x080000, REGION_SOUND2, 0 ) /* QDSP samples (SFX) */
	ROM_LOAD( "ftii011.u97", 0x00000, 0x80000, CRC(8a431b14) SHA1(5a9824280f30ef2e7b7f16652b2f9f9559cb764f) )

	ROM_REGION( 0x080000, REGION_SOUND3, 0 ) /* QDSP wavetable rom */
	ROM_LOAD( "qs1001a.u97",  0x00000, 0x80000, NO_DUMP )
ROM_END


static DRIVER_INIT( landbrk )
{
	coin_counter_bit = 0x1000;
}

static DRIVER_INIT( landbrka )
{
	//it fails compares with memories:
	//$4002d338 -> $4002d348 .... $4002d33f -> $4002d34f
	//related with bits 0x100 - 0x200 read at startup from input(0) ?
	UINT32 *rombase = (UINT32*)memory_region(REGION_CPU1);
	rombase[0x14f00/4] = (rombase[0x14f00/4] & 0xffff) | 0x03000000; /* Change BR to NOP */

	coin_counter_bit = 0x2000;
}

GAME( 1998, hidnctch, 0,       eolith, hidnctch, 0,        ROT0, "Eolith", "Hidden Catch (World) / Tul Lin Gu Lim Chat Ki '98 (Korea) (pcb ver 3.03)",  GAME_NO_SOUND ) // or Teurrin Geurim Chajgi '98
GAME( 1998, raccoon,  0,       eolith, raccoon,  0,        ROT0, "Eolith", "Raccoon World", GAME_NO_SOUND )
GAME( 1999, landbrk,  0,       eolith, landbrk,  landbrk,  ROT0, "Eolith", "Land Breaker (World) / Miss Tang Ja Ru Gi (Korea) (pcb ver 3.02)",  GAME_NO_SOUND ) // or Miss Ttang Jjareugi
GAME( 1999, landbrka, landbrk, eolith, landbrk,  landbrka, ROT0, "Eolith", "Land Breaker (World) / Miss Tang Ja Ru Gi (Korea) (pcb ver 3.03)",  GAME_NO_SOUND ) // or Miss Ttang Jjareugi
GAME( 2001, fort2b,   0,       eolith, common,   0,        ROT0, "Eolith", "Fortress 2 Blue Arcade (ver 1.01 / pcb ver 3.05)",  GAME_NO_SOUND )
GAME( 2001, fort2ba,  fort2b,  eolith, common,   0,        ROT0, "Eolith", "Fortress 2 Blue Arcade (ver 1.00 / pcb ver 3.05)",  GAME_NO_SOUND )
