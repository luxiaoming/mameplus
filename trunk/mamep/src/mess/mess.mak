###########################################################################
#
#   mess.mak
#
#   MESS target makefile
#
###########################################################################



#-------------------------------------------------
# this is the list of driver libraries that
# comprise MESS plus messdriv.o which contains
# the list of drivers
#-------------------------------------------------

DRVLIBS += \
	$(MESSOBJ)/messdriv.o \
	$(MESSOBJ)/ascii.a \
	$(MESSOBJ)/atari.a \
	$(MESSOBJ)/bandai.a \
	$(MESSOBJ)/capcom.a \
	$(MESSOBJ)/nec.a \
	$(MESSOBJ)/nintendo.a \
	$(MESSOBJ)/sega.a \
	$(MESSOBJ)/snk.a \
	$(MESSOBJ)/shared.a \




#-------------------------------------------------
# the following files are general components and
# shared across a number of drivers
#-------------------------------------------------

$(MESSOBJ)/shared.a: \
	$(MESS_DEVICES)/basicdsk.o	\
	$(MESS_DEVICES)/cartslot.o	\
	$(MESS_DEVICES)/cassette.o	\
	$(MESS_DEVICES)/chd_cd.o	\
	$(MESS_DEVICES)/flopdrv.o	\
	$(MESS_DEVICES)/mflopimg.o	\
	$(MESS_DEVICES)/multcart.o	\
	$(MESS_DEVICES)/printer.o	\
	$(MESS_FORMATS)/basicdsk.o	\
	$(MESS_FORMATS)/cassimg.o	\
	$(MESS_FORMATS)/flopimg.o	\
	$(MESS_FORMATS)/ioprocs.o	\
	$(MESS_FORMATS)/wavfile.o	\
	$(MESS_MACHINE)/ctronics.o	\
	$(MESS_MACHINE)/tc8521.o	\
	$(MESS_MACHINE)/wd17xx.o	\



#-------------------------------------------------
# manufacturer-specific groupings for drivers
#-------------------------------------------------

$(MESSOBJ)/ascii.a :			\
	$(MESS_FORMATS)/fmsx_cas.o	\
	$(MESS_DRIVERS)/msx.o		\
	$(MESS_MACHINE)/msx_slot.o	\
	$(MESS_MACHINE)/msx.o		\

$(MESSOBJ)/atari.a:			\
	$(MESS_MACHINE)/ataricrt.o	\
	$(MESS_MACHINE)/atarifdc.o	\
	$(MESS_DRIVERS)/atari.o		\
	$(MESS_MACHINE)/a7800.o		\
	$(MESS_DRIVERS)/a7800.o		\
	$(MESS_VIDEO)/a7800.o		\
	$(MESS_DRIVERS)/a2600.o		\
	$(MESS_FORMATS)/a26_cas.o	\

$(MESSOBJ)/bandai.a:			\
	$(MESS_DRIVERS)/wswan.o		\
	$(MESS_MACHINE)/wswan.o		\
	$(MESS_VIDEO)/wswan.o		\
	$(MESS_AUDIO)/wswan.o		\

$(MESSOBJ)/capcom.a:			\
	$(MESS_DRIVERS)/cpschngr.o	\
	$(MESS_VIDEO)/cpschngr.o	\

$(MESSOBJ)/nec.a:			\
	$(MESS_MACHINE)/pce.o		\
	$(MESS_DRIVERS)/pce.o		\

$(MESSOBJ)/nintendo.a:			\
	$(MESS_AUDIO)/gb.o		\
	$(MESS_VIDEO)/gb.o		\
	$(MESS_MACHINE)/gb.o		\
	$(MESS_DRIVERS)/gb.o		\
	$(MESS_DRIVERS)/gba.o	 	\
	$(MESS_MACHINE)/nes_mmc.o	\
	$(MESS_VIDEO)/nes.o		\
	$(MESS_MACHINE)/nes.o		\
	$(MESS_DRIVERS)/nes.o		\
	$(MESS_MACHINE)/snescart.o	\
	$(MESS_DRIVERS)/snes.o	 	\

$(MESSOBJ)/sega.a:			\
	$(MESS_DRIVERS)/genesis.o	\
	$(MESS_MACHINE)/genesis.o	\
	$(MESS_VIDEO)/smsvdp.o		\
	$(MESS_MACHINE)/sms.o		\
	$(MESS_DRIVERS)/sms.o		\

$(MESSOBJ)/snk.a:			\
	$(MESS_DRIVERS)/ngp.o		\
	$(MESS_VIDEO)/k1ge.o		\

#-------------------------------------------------
# layout dependencies
#-------------------------------------------------

$(MESSOBJ)/mess.o:	$(MESS_LAYOUT)/lcd.lh
