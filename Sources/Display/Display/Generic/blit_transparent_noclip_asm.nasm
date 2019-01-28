;	$Id: blit_transparent_noclip_asm.nasm,v 1.1 2001/03/06 15:09:17 mbn Exp $
;
;	------------------------------------------------------------------------
;	ClanLib, the platform independent game SDK.
;
;	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
;	version 2. See COPYING for details.
;
;	For a total list of contributers see CREDITS.
;
;	This file by Magnus Norddahl 03/99
;	------------------------------------------------------------------------
;
;	changed things a bit	Daniel Vogel (03/14/99)
;	changed things a bit	Mikko Tiihonen (04/09/99)


BITS 32

GLOBAL blit_transparent_noclip_asm_16bpp
GLOBAL blit_transparent_noclip_asm_32bpp

SECTION .text

;; blit_transparent_noclip_asmbpp(uchar *src, ushort *dest, uint width)
;; --------------------------------------
;;  4: uchar *src
;;  8: ushort *dest
;; 12: uint width
;; --------------------------------------
ALIGN 8
blit_transparent_noclip_asm_16bpp:
	push		esi			; save used registers (5)
	push		edi
	push		eax
	push		ecx
	push		edx

	mov		esi, [esp+20+4]		; load registers
	mov		edi, [esp+20+8]
	mov		edx, [esp+20+12]

	xor		ecx, ecx		; make sure upper part of register is zero
ALIGN 4
.cmd_copy:
	mov		cx, [esi]		; read rep
	sub		edx, ecx
	add		esi, BYTE 2

	shr		ecx, 1
	rep		movsd			; copy all dwords

	jnc		.end_copy
	mov		ax, [esi]
	mov		[edi], ax
	add		esi, BYTE 2
	add		edi, BYTE 2

.end_copy:
	and		edx, edx
	jz		.rle_done

.cmd_skip:
	mov		cx, [esi]		; read rep
	add		edi, ecx		; do the skip
	add		esi, BYTE 2
	add		edi, ecx		; (x2 because dest is word)

	sub		edx, ecx		; width -= rep
	jnz		.cmd_copy

.rle_done:

	pop		edx			; restore used registers
	pop		ecx
	pop		eax
	pop		edi
	pop		esi

	ret

;; blit_transparent_noclip_asmbpp(uchar *src, uint *dest, uint width)
;; --------------------------------------
;;  4: uchar *src
;;  8: uint *dest
;; 12: uint width
;; --------------------------------------
ALIGN 8
blit_transparent_noclip_asm_32bpp:
	push		esi			; save used registers
	push		edi
	push		ecx
	push		edx

	mov		esi, [esp+16+4]		; load registers
	mov		edi, [esp+16+8]
	mov		edx, [esp+16+12]

	xor		ecx, ecx		; make sure upper part of register is zero

.cmd_copy:
	mov		cx, [esi]		; read rep
	add		esi, BYTE 2
	sub		edx, ecx

	rep		movsd			; copy all dwords
	jz		.rle_done

.cmd_skip:
	mov		cx, [esi]		; read rep
	lea		edi, [edi + 4*ecx]	; do the skip (x4 because dest is word)
	add		esi, BYTE 2

	sub		edx, ecx		; width -= rep
	jnz		.cmd_copy

.rle_done:

	pop		edx			; restore used registers
	pop		ecx
	pop		edi
	pop		esi

	ret
