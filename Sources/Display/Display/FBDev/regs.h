/*
        $Id: regs.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

        ------------------------------------------------------------------------
        ClanLib, the platform independent game SDK.

        This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
        version 2. See COPYING for details.

        For a total list of contributers see CREDITS.

        ------------------------------------------------------------------------

*/

#define FNORM_TO_F0915(x)	(((uint32) ((x*256) * (1 << 15))) & 0x00FFFFFF)
#define F1220(x)			((uint32)((x) * (1 << 20)))
#define F1616(x)			((uint32)((x) * (1 << 16)))

#define DWGSYNC		0x2C4C
#define SYNC_DMA_BUSY   0x8325340              /* just a random number */

#define OPMODE		0x1E54

/* Alpha registers */

#define ALPHACTRL	0x2C7C
#define ALPHASTART	0x2C70
#define ALPHAXINC	0x2C74
#define ALPHAYINC	0x2C78

/* Texture registers */

#define TEXCTL		0x2C30
#define TEXCTL2		0x2C3C
#define TEXFILTER	0x2C58
#define TEXWIDTH	0x2C28
#define TEXHEIGHT	0x2C2C
#define TEXORG		0x2C24
#define TEXTRANS	0x2C34
#define TEXTRANSHIGH	0x2C38

#define TMR0		0x2C00
#define TMR1		0x2C04
#define TMR2		0x2C08
#define TMR3		0x2C0C
#define TMR4		0x2C10
#define TMR5		0x2C14
#define TMR6		0x2C18
#define TMR7		0x2C1C
#define TMR8		0x2C20

/* Alpha flags */

#define SRC_ALPHA		0x004
#define DST_ONE_MINUS_SRC_ALPHA	0x050
#define ALPHACHANNEL		0x100

/* Texture flags */

#define LINEXT		0x00100
#define TW16		0x00003
#define MIN_BILINEAR	0x00002
#define MAG_BILINEAR	0x00020
#define DECALDIS	0x00004
#define CKSTRANSDIS	0x00010


/*
 * The following part is from ggi's mga code:
 *    Copyright (C) 1999 Marcus Sundberg   [marcus@ggi-project.org]
 *         www.ggi-project.org
 */

#define EXECUTE		0x100	/* or with register to execute a programmed
				   accel command */

#define DWGCTL		0x1C00	/* Drawing control */
	/* opcod - Operation code */
#	define OP_LINE_OPEN		0x00
#	define OP_AUTOLINE_OPEN		0x01
#	define OP_LINE_CLOSE		0x02
#	define OP_AUTOLINE_CLOSE	0x03
#	define OP_TRAP			0x04
#	define OP_TRAP_ILOAD		0x05
#	define OP_TEXTURE_TRAP		0x06
#	define OP_BITBLT		0x08
#	define OP_FBITBLT		0x0C
#	define OP_ILOAD			0x09
#	define OP_ILOAD_SCALE		0x0D
#	define OP_ILOAD_FILTER		0x0F
#	define OP_IDUMP			0x0A
#	define OP_ILOAD_HIQH		0x07
#	define OP_ILOAD_HIQHV		0x0E

	/* atype - Access type */
#	define ATYPE_MASK	0x70
#	define ATYPE_RPL	0x00
#	define ATYPE_RSTR	0x10
#	define ATYPE_ZI		0x30
#	define ATYPE_BLK	0x40
#	define ATYPE_I		0x70

	/* Flag */
#	define LINEAR		0x80
#	define NOCLIP		(1<<31)
#	define TRANS		(1<<30)

	/* zmode - Z drawing mode */
#	define ZMODE_NOZCMP	0x000
#	define ZMODE_ZE		0x200
#	define ZMODE_ZNE	0x300
#	define ZMODE_ZLT	0x400
#	define ZMODE_ZLTE	0x500
#	define ZMODE_ZGT	0x600
#	define ZMODE_ZGTE	0x700

	/* Flags */
#	define SOLID		0x0800
#	define ARZERO		0x1000
#	define SGNZERO		0x2000
#	define SHFTZERO		0x4000

	/* bop - Boolean operation */
#	define BOP_CLEAR	0x00000
#	define BOP_NOR		0x10000
#	define BOP_COPYINV	0x30000
#	define BOP_INVERT	0x50000
#	define BOP_XOR		0x60000
#	define BOP_NAND		0x70000
#	define BOP_AND		0x80000
#	define BOP_EQUIV	0x90000
#	define BOP_NOOP		0xA0000
#	define BOP_IMP		0xB0000
#	define BOP_COPY		0xC0000
#	define BOP_OR		0xE0000
#	define BOP_SET		0xF0000

	/* bltmod - Blit mode selection */
#	define BLTMOD_BMONOLEF	0x00000000
#	define BLTMOD_BMONOWF	0x08000000
#	define BLTMOD_BPLAN	0x02000000
#	define BLTMOD_BFCOL	0x04000000
#	define BLTMOD_BUYUV	0x1C000000
#	define BLTMOD_BU32BGR	0x06000000
#	define BLTMOD_BU32RGB	0x0E000000
#	define BLTMOD_BU24BGR	0x16000000
#	define BLTMOD_BU24RGB	0x1E000000

#define MACCESS		0x1C04
#define ZORG		0x1C0C
#define PAT0		0x1C10
#define PAT1		0x1C14
#define PLNWT		0x1C1C
#define BCOL		0x1C20
#define FCOL		0x1C24
#define SRC0		0x1C30
#define SRC1		0x1C34
#define SRC2		0x1C38
#define SRC3		0x1C3C
#define XYSTRT		0x1C40
#define XYEND		0x1C44
#define SHIFT		0x1C50
#define DMAPAD		0x1C54
#define SGN		0x1C58
#define LEN		0x1C5C
#define AR0		0x1C60
#define AR1		0x1C64
#define AR2		0x1C68
#define AR3		0x1C6C
#define AR4		0x1C70
#define AR5		0x1C74
#define AR6		0x1C78
#define CXBNDRY		0x1C80
#define FXBNDRY		0x1C84
#define YDSTLEN		0x1C88
#define PITCH		0x1C8C
#define YDST		0x1C90
#define YDSTORG		0x1C94
#define YTOP		0x1C98
#define YBOT		0x1C9C
#define CXLEFT		0x1CA0
#define CXRIGHT		0x1CA4
#define FXLEFT		0x1CA8
#define FXRIGHT		0x1CAC
#define XDST		0x1CB0
#define DR0		0x1CC0
#define DR2		0x1CC8
#define DR3		0x1CCC
#define DR4		0x1CD0
#define DR6		0x1CD8
#define DR7		0x1CDC
#define DR8		0x1CE0
#define WO		0x1CE4
#define DR10		0x1CE8
#define DR11		0x1CEC
#define DR12		0x1CF0
#define DR14		0x1CF8
#define DR15		0x1CFC

#define FIFOSTATUS	0x1E10
#define STATUS		0x1E14
