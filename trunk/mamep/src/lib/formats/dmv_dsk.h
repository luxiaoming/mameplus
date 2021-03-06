/*********************************************************************

    formats/dmv_dsk.h

    NCR Decision Mate V format

*********************************************************************/

#ifndef DMV_DSK_H_
#define DMV_DSK_H_

#include "upd765_dsk.h"

class dmv_format : public upd765_format {
public:
	dmv_format();

	virtual const char *name() const;
	virtual const char *description() const;
	virtual const char *extensions() const;

private:
	static const format formats[];
};

extern const floppy_format_type FLOPPY_DMV_FORMAT;

#endif /* DMV_DSK_H_ */
