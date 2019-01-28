;	Daniel Vogel 99/08/30


BITS 32

GLOBAL _calculate_plasma

SECTION .text

;; calculate_plasma(int *dest, int *s1, int *s2, int *s3, int w, int h)
;; --------------------------------------
;;  4: int *dest
;;  8: int *s1
;; 12: int *s2
;; 16: int *s3
;; 20: int w
;; 24: int h
;; --------------------------------------
ALIGN 8
_calculate_plasma:
	push 		ebp			; save used registers (7)
	push		esi
	push		edi
	push		eax
	push		ebx
	push		ecx
	push		edx

	mov		edi, [esp+28+4]		; dest
	mov		esi, [esp+28+8]		; s1
	mov		ebp, [esp+28+12]	; s2
	mov		edx, [esp+28+16]	; s3

	mov		ebx, [esp+28+24]	; h
	
.loop_y:
	mov		ecx, [esp+28+20]	; w

ALIGN 4

.loop_x:
	mov		eax, [esi]
	add		eax, [ebp]
	add		eax, [edx]
	
	add		esi, 4
	not		al			; alpha == 255 == opaque
	add		ebp, 4
	add		edx, 4

	mov		[edi], eax

	add		edi, 4

	dec		ecx
	jz		.loop_x_end
	jmp		.loop_x

.loop_x_end:
	mov		eax, [esp+28+20]	; w
	shl		eax, 2
	
	add		esi, eax
	add		ebp, eax
	add		edx, eax
	
	dec		ebx
	jz		.end
	jmp		.loop_y

.end:	
	pop		edx			; restore used registers (7)
	pop		ecx
	pop		ebx
	pop		eax
	pop		edi
	pop		esi
	pop		ebp
	ret
