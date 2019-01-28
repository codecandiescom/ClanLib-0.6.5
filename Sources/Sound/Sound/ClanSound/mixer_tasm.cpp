/*
	$Id: mixer_tasm.cpp,v 1.6 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------



*/
#ifdef USE_TASM
extern "C" {
void mixer_clip_asm(int *mix_buffer2,short *mix_buffer,int size)
{
asm {
mixer_clip_asm:
	push		esi		
	push		edi
	push		eax
	push		ebx
	push		ecx
	push		edx

	mov		esi, mix_buffer2
	mov		edi, mix_buffer
	mov		ecx, size

	mov		ebx, 32200
	mov		edx, -32200

.loop:
	mov		eax, [esi]
	sar		eax, 1
	add		esi,  4

	cmp		eax, ebx
	jg		.above		
	cmp		eax, edx
	jl		.below
	jmp		.clipped

.above: 
	mov		eax, ebx
	jmp		.clipped

.below:	
	mov		eax, edx

.clipped:

	mov		[edi], ax
	add		edi,  2
	dec		ecx
	jz		.done
	jmp		.loop

.done:
	pop		edx
	pop		ecx
	pop		ebx
	pop		eax
	pop		edi
	pop		esi

; 	ret
};

}
}
#endif

           /*
;; mixer_asm(int *src, short *dest, int size)
;; --------------------------------------
;;  4: int *src
;;  8: short *dest
;; 12: int size
;; --------------------------------------


*/