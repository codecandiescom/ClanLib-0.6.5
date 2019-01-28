
/*
        $Id: mmio.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

        ------------------------------------------------------------------------
        ClanLib, the platform independent game SDK.

        This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
        version 2. See COPYING for details.

        For a total list of contributers see CREDITS.

        ------------------------------------------------------------------------

This file is from ggi's mga code:
   Copyright (C) 1999 Marcus Sundberg   [marcus@ggi-project.org]
        www.ggi-project.org

*/

#include <asm/types.h>

typedef __u8 uint8;
typedef __u32 uint32;


static inline void
mga_out8(volatile uint8 *mmioaddr, uint8 value, uint32 reg)
{
	*((uint8*)(mmioaddr+reg)) = value;
}

static inline void
mga_out32(volatile uint8 *mmioaddr, uint32 value, uint32 reg)
{
	*((uint32*)(mmioaddr+reg)) = value;
}

static inline volatile uint8
mga_in8(volatile uint8 *mmioaddr, uint32 reg)
{
	return *((uint8*)(mmioaddr+reg));
}

static inline volatile uint32
mga_in32(volatile uint8 *mmioaddr, uint32 reg)
{
	return *((uint32*)(mmioaddr+reg));
}

/* We seem to need this for gcc to work... */
static int grodis;

extern int fifos;

/* Wait for fifo space */
static inline void
mga_waitfifo(volatile uint8 *mmioaddr, int space)
{
//	fifos+=space;
	while (mga_in8(mmioaddr, FIFOSTATUS) < space) {
		grodis = 0;
	}
}


/* Wait for idle accelerator */
static inline void
mga_waitidle(volatile uint8 *mmioaddr)
{
	while (mga_in32(mmioaddr, STATUS) & 0x10000) {
		grodis = 0;
	}
}

