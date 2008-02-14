/*********************************************************************

	mslegacy.h

	Defines since removed from MAME, but kept in MESS for legacy
	reasons

*********************************************************************/

#ifndef MSLEGACY_H
#define MSLEGACY_H

#include "driver.h"

enum
{
	UI_comp1 = UI_last_mame_entry,
	UI_comp2,
	UI_keyb1,
	UI_keyb2,
	UI_keyb3,
	UI_keyb4,
	UI_keyb5,
	UI_keyb6,
	UI_keyb7,

	UI_imageinfo,
	UI_filemanager,
	UI_tapecontrol,

	UI_notapeimageloaded,
	UI_recording,
	UI_playing,
	UI_recording_inhibited,
	UI_playing_inhibited,
	UI_stopped,
	UI_pauseorstop,
	UI_record,
	UI_play,
	UI_rewind,
	UI_fastforward,
	UI_mount,
	UI_create,
	UI_unmount,
	UI_emptyslot,
	UI_categories,
	UI_quitfileselector,
	UI_filespecification,	/* IMPORTANT: be careful to ensure that the following */
	UI_cartridge,			/* device list matches the order found in device.h    */
	UI_floppydisk,			/* and is ALWAYS placed after UI_filespecification    */
	UI_harddisk,
	UI_cylinder,
	UI_cassette,
	UI_punchcard,
	UI_punchtape,
	UI_printer,
	UI_serial,
	UI_parallel,
	UI_snapshot,
	UI_quickload,
	UI_memcard,
	UI_cdrom
};


/* palette */
void palette_set_colors_rgb(running_machine *machine, pen_t color_base, const UINT8 *colors, int color_count);


/* uitext */
extern const char * ui_getstring (int string_num);


/* timer */
void timer_adjust(emu_timer *which, attotime duration, INT32 param, attotime period);


/* mc6845 */
struct _mc6845_t;
struct _mc6845_interface;

extern struct _mc6845_t *mslegacy_mc6845;

WRITE8_HANDLER(crtc6845_0_address_w);
READ8_HANDLER(crtc6845_0_register_r);
WRITE8_HANDLER(crtc6845_0_register_w);
VIDEO_UPDATE(crtc6845);
void crtc6845_config(int index, const struct _mc6845_interface *intf);



#endif /* MSLEGACY_H */
