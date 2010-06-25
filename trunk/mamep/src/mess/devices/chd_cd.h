/*********************************************************************

    chd_cd.h

    MESS interface to the MAME CHD CDROM code

*********************************************************************/

#ifndef MESS_CD_H
#define MESS_CD_H

#include "cdrom.h"

DECLARE_LEGACY_IMAGE_DEVICE(CDROM, cdrom);


#define MDRV_CDROM_ADD(_tag) \
	MDRV_DEVICE_ADD(_tag, CDROM, 0) \



cdrom_file *mess_cd_get_cdrom_file(running_device *device);

#endif /* MESS_CD_H */
