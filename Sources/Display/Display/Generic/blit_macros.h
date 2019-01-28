/*
	The ClanLib Game SDK, but the source has been taken from:
	(just yet ANOTHER advantage of open source :-) )

    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    5635-34 Springhouse Dr.
    Pleasanton, CA 94588 (USA)
    slouken@devolution.com
*/

#ifndef header_blit_macros
#define header_blit_macros

struct SDL_PixelFormat
{
	unsigned char  BitsPerPixel;
	unsigned char  BytesPerPixel;
	unsigned char  Rloss;
	unsigned char  Gloss;
	unsigned char  Bloss;   
	unsigned char  Aloss;   
	unsigned char  Rshift;
	unsigned char  Gshift;
	unsigned char  Bshift;
	unsigned char  Ashift;
	unsigned int   Rmask;
	unsigned int   Gmask;
	unsigned int   Bmask;
	unsigned int   Amask;

	/* RGB color key information */
	unsigned int   colorkey;

	/* Alpha value information (per-surface alpha) */
	unsigned char  alpha;
};


/*
 * Useful macros for blitting routines
 */

#define FORMAT_EQUAL(A, B) \
	(((A)->BitsPerPixel == (B)->BitsPerPixel) && ((A)->Rmask == (B)->Rmask))

/* Load pixel of the specified format from a buffer and get its R-G-B values */
#define RGB_FROM_PIXEL(pixel, fmt, r, g, b)				\
{									\
	r = (((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss); 		\
	g = (((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss); 		\
	b = (((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss); 		\
}
#define DISEMBLE_RGB(buf, bpp, fmt, pixel, r, g, b)			\
{									\
	switch (bpp) {							\
		case 2:							\
			pixel = *((unsigned short *)(buf));			\
			RGB_FROM_PIXEL(pixel, fmt, r, g, b);		\
		break;							\
									\
		case 3:							\
			r = *(((unsigned char *)buf)+fmt->Rshift/8);		\
			g = *(((unsigned char *)buf)+fmt->Gshift/8);		\
			b = *(((unsigned char *)buf)+fmt->Bshift/8);		\
			pixel = (r<<fmt->Rshift)|			\
				(g<<fmt->Gshift)|			\
				(b<<fmt->Bshift);			\
		break;							\
									\
		case 4:							\
			pixel = *((unsigned int *)(buf));			\
			RGB_FROM_PIXEL(pixel, fmt, r, g, b);		\
		break;							\
	}								\
}
/* Assemble R-G-B values into a specified pixel format and store them */
#define PIXEL_FROM_RGB(pixel, fmt, r, g, b)				\
{									\
	pixel = ((r>>fmt->Rloss)<<fmt->Rshift)|				\
		((g>>fmt->Gloss)<<fmt->Gshift)|				\
		((b>>fmt->Bloss)<<fmt->Bshift);				\
}
#define ASSEMBLE_RGB(buf, bpp, fmt, r, g, b) 				\
{									\
	switch (bpp) {							\
		case 2: {						\
			unsigned short pixel;					\
									\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);		\
			*((unsigned short *)(buf)) = pixel;			\
		}							\
		break;							\
									\
		case 3: {						\
			*((buf)+fmt->Rshift/8) = r;			\
			*((buf)+fmt->Gshift/8) = g;			\
			*((buf)+fmt->Bshift/8) = b;			\
		}							\
		break;							\
									\
		case 4: {						\
			unsigned int pixel;					\
									\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);		\
			*((unsigned int *)(buf)) = pixel;			\
		}							\
		break;							\
	}								\
}

#define RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a)				\
{									\
	r = (((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss); 		\
	g = (((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss); 		\
	b = (((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss); 		\
	a =  ((pixel&fmt->Amask)>>fmt->Ashift);		 		\
}
#define DISEMBLE_RGBA(buf, bpp, fmt, pixel, r, g, b, a)			\
{									\
	switch (bpp) {							\
		case 2:							\
			pixel = *((unsigned short *)(buf));			\
			RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a);	\
			pixel &= ~fmt->Amask;				\
		break;							\
									\
		case 3:	/* FIXME: broken code (no alpha) */		\
			r = *(((unsigned char *)buf)+fmt->Rshift/8);		\
			g = *(((unsigned char *)buf)+fmt->Gshift/8);		\
			b = *(((unsigned char *)buf)+fmt->Bshift/8);		\
			a = 0;						\
			pixel = (r<<fmt->Rshift)|			\
				(g<<fmt->Gshift)|			\
				(b<<fmt->Bshift);			\
		break;							\
									\
		case 4:							\
			pixel = *((unsigned int *)(buf));			\
			RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a);	\
			pixel &= ~fmt->Amask;				\
		break;							\
	}								\
}

#define PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a)				\
{									\
	pixel = ((r>>fmt->Rloss)<<fmt->Rshift)|				\
		((g>>fmt->Gloss)<<fmt->Gshift)|				\
		((b>>fmt->Bloss)<<fmt->Bshift)|				\
		(a<<fmt->Ashift);					\
}
#define ASSEMBLE_RGBA(buf, bpp, fmt, r, g, b, a)			\
{									\
	switch (bpp) {							\
		case 2: {						\
			unsigned short pixel;					\
									\
			PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a);	\
			*((unsigned short *)(buf)) = pixel;			\
		}							\
		break;							\
									\
		case 3: { /* FIXME: broken code (no alpha) */		\
			*((buf)+fmt->Rshift/8) = r;			\
			*((buf)+fmt->Gshift/8) = g;			\
			*((buf)+fmt->Bshift/8) = b;			\
		}							\
		break;							\
									\
		case 4: {						\
			unsigned int pixel;					\
									\
			PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a);	\
			*((unsigned int *)(buf)) = pixel;			\
		}							\
		break;							\
	}								\
}

/* Blend the RGB values of two pixels based on a source alpha value */
#define ALPHA_BLEND(sR, sG, sB, A, Adiv, dR, dG, dB)			\
{									\
	dR = ((unsigned short)sR*(Adiv-A) + (unsigned short)dR*A) / Adiv;		\
	dG = ((unsigned short)sG*(Adiv-A) + (unsigned short)dG*A) / Adiv;		\
	dB = ((unsigned short)sB*(Adiv-A) + (unsigned short)dB*A) / Adiv;		\
}

#endif
