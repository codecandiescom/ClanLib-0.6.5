;	------------------------------------------------------------------------
;	ClanLib, the platform independent game SDK.
;
;	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
;	version 2. See COPYING for details.
;
;	For a total list of contributers see CREDITS.
;
;	This file by Daniel Vogel 04/99
;	------------------------------------------------------------------------

BITS 32

GLOBAL mixer_clip_asm

SECTION .text

;; mixer_asm(int *src, short *dest, int size)
;; --------------------------------------
;;  4: int *src
;;  8: short *dest
;; 12: int size
;; --------------------------------------

mixer_clip_asm:
	push		esi			; save used registers (6)
	push		edi
	push		eax
	push		ebx
	push		ecx
	push		edx

	mov		esi, [esp+24+4]		; load registers
	mov		edi, [esp+24+8]
	mov		ecx, [esp+24+12]

	mov		ebx, 32200
	mov		edx, -32200

.loop:
	mov		eax, [esi]
	sar		eax, 1
	add		esi, BYTE 4

	cmp		eax, ebx
	jg		.above		
	cmp		eax, edx
	jl		.below
	jmp		.clipped

.above: 
	mov		eax, ebx		; 32200
	jmp		.clipped

.below:	
	mov		eax, edx		; -32200

.clipped:

	mov		[edi], ax
	add		edi, BYTE 2
	dec		ecx
	jz		.done
	jmp		.loop

.done:
	pop		edx			; restore used registers
	pop		ecx
	pop		ebx
	pop		eax
	pop		edi
	pop		esi

	ret

