/*
	$Id: blit_transparent_noclip_tasm.cpp,v 1.4 2001/09/08 19:12:48 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_TASM

void blit_transparent_noclip_asmbpp(unsigned char *src, unsigned short *dest, unsigned int width)
{
asm
{
ALIGN 8
blit_transparent_noclip_asm_16bpp:
	push		esi			;; save used registers (5)
	push		edi
	push		eax
	push		ecx
	push		edx

	mov		esi, [esp+20+4]		;; load registers
	mov		edi, [esp+20+8]
	mov		edx, [esp+20+12]

	xor		ecx, ecx		;; make sure upper part of register is zero
ALIGN 4
.cmd_copy:
	mov		cx, [esi]		;; read rep
	sub		edx, ecx
	add		esi,  2

	shr		ecx, 1
	rep		movsd			;; copy all dwords

	jnc		.end_copy
	mov		ax, [esi]
	mov		[edi], ax
	add		esi,  2
	add		edi,  2

.end_copy:
	and		edx, edx
	jz		.rle_done

.cmd_skip:
	mov		cx, [esi]		;; read rep
	add		edi, ecx		;; do the skip
	add		esi,  2
	add		edi, ecx		;; (x2 because dest is word)

	sub		edx, ecx		;; width -= rep
	jnz		.cmd_copy

.rle_done:

	pop		edx			;; restore used registers
	pop		ecx
	pop		eax
	pop		edi
	pop		esi

	ret
}
}


void blit_transparent_noclip_asm_32bpp(unsigned char *src, unsigned int *dest, unsigned int width)
{
asm
{
ALIGN 8
blit_transparent_noclip_asm_32bpp:
	push		esi			; ; save used registers
	push		edi
	push		ecx
	push		edx

	mov		esi, [esp+16+4]		; ; load registers
	mov		edi, [esp+16+8]
	mov		edx, [esp+16+12]

	xor		ecx, ecx		; ; make sure upper part of register is zero

.cmd_copy32:
	mov		cx, [esi]		;; read rep
	add		esi,  2
	sub		edx, ecx

	rep		movsd			;; copy all dwords
	jz		.rle_done32

.cmd_skip32:
	mov		cx, [esi]		;; read rep
	lea		edi, [edi + 4*ecx]	;; do the skip (x4 because dest is word)
	add		esi,  2

	sub		edx, ecx		;; width -= rep
	jnz		.cmd_copy32

.rle_done32:

	pop		edx			;; restore used registers
	pop		ecx
	pop		edi
	pop		esi

	ret
}
}

#endif
 