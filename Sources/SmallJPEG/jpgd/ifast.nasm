;
; jpeg_idct_ifast, assembly implementation.
;
; Originally written by Rich Geldreich<richgel@voicenet.com>
; Ported from MSVC++ inline assembly to nasm by Magnus Norddahl <mbn@clanlib.org>
; GNU Assembler (gas) version can be created by running this file through intel2gas.
;

BITS 32

GLOBAL jpeg_idct_ifast_asm
GLOBAL jpeg_idct_ifast_deinit

SECTION .text
ALIGN 8

fix_141		dd	5a825a82h, 5a825a82h
fix_184n261	dd	cf04cf04h, cf04cf04h
fix_184		dd	76417641h, 76417641h
fix_n184	dd	896f896fh, 896f896fh
fix_108n184	dd	cf04cf04h, cf04cf04h
const_0x0080	dd	00800080h, 00800080h


;; jpeg_idct_ifast_asm(
;;   BLOCK_TYPE *inptr,
;;   short *quantptr,
;;   uchar * *outptr,
;;   int output_col,
;;   short *wsptr)
;; -------------------------
;;   4: BLOCK_TYPE *inptr
;;   8: short *quantptr
;;  12: uchar **outptr
;;  16: int output_col
;;  20: short *wsptr
jpeg_idct_ifast_asm:

	push	esi
	push	edi
	push	eax
	push	ecx
	push	ebx
	; save mmx registers here!?

;DEFINE inptr      [esp+20+4]
;DEFINE quantptr   [esp+20+8]
;DEFINE outptr     [esp+20+12]
;DEFINE output_col [esp+20+16]
;DEFINE wsptr      [esp+20+20]

	mov		edi, [esp+20+8]
	mov		ebx, [esp+20+4]
	mov		esi, [esp+20+20]
	add		esi, 0x07		;align wsptr to qword
	and		esi, 0xfffffff8	;align wsptr to qword

	mov		eax, esi

    ; Odd part


	movq		mm1, [ebx + 8*10]		;load inptr[DCTSIZE*5]

	pmullw		mm1, [edi + 8*10]		;tmp6 = DEQUANTIZE(inptr[DCTSIZE*5], quantptr[DCTSIZE*5]);

	movq		mm0, [ebx + 8*6]		;load inptr[DCTSIZE*3]

	pmullw		mm0, [edi + 8*6]		;tmp5 = DEQUANTIZE(inptr[DCTSIZE*3], quantptr[DCTSIZE*3]);

	movq		mm3, [ebx + 8*2]		;load inptr[DCTSIZE*1]
	movq	mm2, mm1					;copy tmp6	/* phase 6 */

	pmullw		mm3, [edi + 8*2]		;tmp4 = DEQUANTIZE(inptr[DCTSIZE*1], quantptr[DCTSIZE*1]);

	movq		mm4, [ebx + 8*14]		;load inptr[DCTSIZE*1]
	paddw	mm1, mm0					;z13 = tmp6 + tmp5;

	pmullw		mm4, [edi + 8*14]	    ;tmp7 = DEQUANTIZE(inptr[DCTSIZE*7], quantptr[DCTSIZE*7]);
	psubw	mm2, mm0					;z10 = tmp6 - tmp5

	psllw		mm2, 2				;shift z10
	movq		mm0, mm2			;copy z10

	pmulhw		mm2, qword [fix_184n261]	;MULTIPLY( z12, FIX_1_847759065); /* 2*c2 */
	movq		mm5, mm3				;copy tmp4

	pmulhw		mm0, qword [fix_n184]		;MULTIPLY(z10, -FIX_1_847759065); /* 2*c2 */
	paddw		mm3, mm4				;z11 = tmp4 + tmp7;

	movq		mm6, mm3				;copy z11			/* phase 5 */
	psubw		mm5, mm4				;z12 = tmp4 - tmp7;

	psubw		mm6, mm1				;z11-z13
	psllw		mm5, 2				;shift z12

	movq		mm4, [ebx + 8*12]		;load inptr[DCTSIZE*6], even part
 	movq		mm7, mm5			;copy z12

	pmulhw		mm5, qword [fix_108n184]	;MULT(z12, (FIX_1_08-FIX_1_84)) ;- z5; /* 2*(c2-c6) */ even part
	paddw		mm3, mm1				;tmp7 = z11 + z13;


    ; Even part
	pmulhw		mm7, qword [fix_184]		;MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) ;+ z5; /* -2*(c2+c6) */
	psllw		mm6, 2

	movq		mm1, [ebx + 8*4]		;load inptr[DCTSIZE*2]

	pmullw		mm1, [edi + 8*4]		;tmp1 = DEQUANTIZE(inptr[DCTSIZE*2], quantptr[DCTSIZE*2]);
	paddw		mm0, mm5			;tmp10

	pmullw		mm4, [edi + 8*12]		;tmp3 = DEQUANTIZE(inptr[DCTSIZE*6], quantptr[DCTSIZE*6]);
	paddw		mm2, mm7			;tmp12

	pmulhw		mm6, qword [fix_141]			;tmp11 = MULTIPLY(z11 - z13, FIX_1_414213562); /* 2*c4 */
	psubw		mm2, mm3		;tmp6 = tmp12 - tmp7

	movq		mm5, mm1				;copy tmp1
	paddw		mm1, mm4				;tmp13= tmp1 + tmp3;	/* phases 5-3 */

	psubw		mm5, mm4				;tmp1-tmp3
	psubw		mm6, mm2		;tmp5 = tmp11 - tmp6;

	movq		[esi+8*0], mm1			;save tmp13 in workspace
	psllw		mm5, 2					;shift tmp1-tmp3

	movq		mm7, [ebx + 8*0]		;load inptr[DCTSIZE*0]

	pmulhw		mm5, qword [fix_141]			;MULTIPLY(tmp1 - tmp3, FIX_1_414213562)
	paddw		mm0, mm6		;tmp4 = tmp10 + tmp5;

	pmullw		mm7, [edi + 8*0]		;tmp0 = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);

	movq		mm4, [ebx + 8*8]		;load inptr[DCTSIZE*4]

	pmullw		mm4, [edi + 8*8]		;tmp2 = DEQUANTIZE(inptr[DCTSIZE*4], quantptr[DCTSIZE*4]);
	psubw		mm5, mm1				;tmp12 = MULTIPLY(tmp1 - tmp3, FIX_1_414213562) - tmp13; /* 2*c4 */

	movq		[esi+8*4], mm0		;save tmp4 in workspace
	movq		mm1, mm7			;copy tmp0	/* phase 3 */

	movq		[esi+8*2], mm5		;save tmp12 in workspace
	psubw		mm1, mm4			;tmp11 = tmp0 - tmp2;

	paddw		mm7, mm4			;tmp10 = tmp0 + tmp2;
    movq		mm5, mm1		;copy tmp11

	paddw		mm1, [esi+8*2]	;tmp1 = tmp11 + tmp12;
	movq		mm4, mm7		;copy tmp10		/* phase 2 */

	paddw		mm7, [esi+8*0]	;tmp0 = tmp10 + tmp13;

	psubw		mm4, [esi+8*0]	;tmp3 = tmp10 - tmp13;
	movq		mm0, mm7		;copy tmp0

	psubw		mm5, [esi+8*2]	;tmp2 = tmp11 - tmp12;
	paddw		mm7, mm3		;wsptr[DCTSIZE*0] = (int) (tmp0 + tmp7);

	psubw		mm0, mm3			;wsptr[DCTSIZE*7] = (int) (tmp0 - tmp7);

	movq		[esi + 8*0], mm7	;wsptr[DCTSIZE*0]
	movq		mm3, mm1			;copy tmp1

	movq		[esi + 8*14], mm0	;wsptr[DCTSIZE*7]
	paddw		mm1, mm2			;wsptr[DCTSIZE*1] = (int) (tmp1 + tmp6);

	psubw		mm3, mm2			;wsptr[DCTSIZE*6] = (int) (tmp1 - tmp6);

	movq		[esi + 8*2], mm1	;wsptr[DCTSIZE*1]
	movq		mm1, mm4			;copy tmp3

	movq		[esi + 8*12], mm3	;wsptr[DCTSIZE*6]

	paddw		mm4, [esi+8*4]		;wsptr[DCTSIZE*4] = (int) (tmp3 + tmp4);

	psubw		mm1, [esi+8*4]		;wsptr[DCTSIZE*3] = (int) (tmp3 - tmp4);

	movq		[esi + 8*8], mm4
	movq		mm7, mm5			;copy tmp2

	paddw		mm5, mm6			;wsptr[DCTSIZE*2] = (int) (tmp2 + tmp5)

	movq		[esi+8*6], mm1		;
	psubw		mm7, mm6			;wsptr[DCTSIZE*5] = (int) (tmp2 - tmp5);

	movq		[esi + 8*4], mm5

	movq		[esi + 8*10], mm7



;/*****************************************************************/
	add		edi, 8
	add		ebx, 8
	add		esi, 8

;/*****************************************************************/




	movq		mm1, [ebx + 8*10]		;load inptr[DCTSIZE*5]

	pmullw		mm1, [edi + 8*10]		;tmp6 = DEQUANTIZE(inptr[DCTSIZE*5], quantptr[DCTSIZE*5]);

	movq		mm0, [ebx + 8*6]		;load inptr[DCTSIZE*3]

	pmullw		mm0, [edi + 8*6]		;tmp5 = DEQUANTIZE(inptr[DCTSIZE*3], quantptr[DCTSIZE*3]);

	movq		mm3, [ebx + 8*2]		;load inptr[DCTSIZE*1]
	movq	mm2, mm1					;copy tmp6	/* phase 6 */

	pmullw		mm3, [edi + 8*2]		;tmp4 = DEQUANTIZE(inptr[DCTSIZE*1], quantptr[DCTSIZE*1]);

	movq		mm4, [ebx + 8*14]		;load inptr[DCTSIZE*1]
	paddw	mm1, mm0					;z13 = tmp6 + tmp5;

	pmullw		mm4, [edi + 8*14]	    ;tmp7 = DEQUANTIZE(inptr[DCTSIZE*7], quantptr[DCTSIZE*7]);
	psubw	mm2, mm0					;z10 = tmp6 - tmp5

	psllw		mm2, 2				;shift z10
	movq		mm0, mm2			;copy z10

	pmulhw		mm2, qword [fix_184n261]	;MULTIPLY( z12, FIX_1_847759065); /* 2*c2 */
	movq		mm5, mm3				;copy tmp4

	pmulhw		mm0, qword [fix_n184]		;MULTIPLY(z10, -FIX_1_847759065); /* 2*c2 */
	paddw		mm3, mm4				;z11 = tmp4 + tmp7;

	movq		mm6, mm3				;copy z11			/* phase 5 */
	psubw		mm5, mm4				;z12 = tmp4 - tmp7;

	psubw		mm6, mm1				;z11-z13
	psllw		mm5, 2				;shift z12

	movq		mm4, [ebx + 8*12]		;load inptr[DCTSIZE*6], even part
 	movq		mm7, mm5			;copy z12

	pmulhw		mm5, qword [fix_108n184]	;MULT(z12, (FIX_1_08-FIX_1_84)) ;- z5; /* 2*(c2-c6) */ even part
	paddw		mm3, mm1				;tmp7 = z11 + z13;


    ; Even part
	pmulhw		mm7, qword [fix_184]		;MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) ;+ z5; /* -2*(c2+c6) */
	psllw		mm6, 2

	movq		mm1, [ebx + 8*4]		;load inptr[DCTSIZE*2]

	pmullw		mm1, [edi + 8*4]		;tmp1 = DEQUANTIZE(inptr[DCTSIZE*2], quantptr[DCTSIZE*2]);
	paddw		mm0, mm5			;tmp10

	pmullw		mm4, [edi + 8*12]		;tmp3 = DEQUANTIZE(inptr[DCTSIZE*6], quantptr[DCTSIZE*6]);
	paddw		mm2, mm7			;tmp12

	pmulhw		mm6, qword [fix_141]			;tmp11 = MULTIPLY(z11 - z13, FIX_1_414213562); /* 2*c4 */
	psubw		mm2, mm3		;tmp6 = tmp12 - tmp7

	movq		mm5, mm1				;copy tmp1
	paddw		mm1, mm4				;tmp13= tmp1 + tmp3;	/* phases 5-3 */

	psubw		mm5, mm4				;tmp1-tmp3
	psubw		mm6, mm2		;tmp5 = tmp11 - tmp6;

	movq		[esi+8*0], mm1			;save tmp13 in workspace
	psllw		mm5, 2					;shift tmp1-tmp3

	movq		mm7, [ebx + 8*0]		;load inptr[DCTSIZE*0]
	paddw		mm0, mm6		;tmp4 = tmp10 + tmp5;

	pmulhw		mm5, qword [fix_141]			;MULTIPLY(tmp1 - tmp3, FIX_1_414213562)

	pmullw		mm7, [edi + 8*0]		;tmp0 = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);

	movq		mm4, [ebx + 8*8]		;load inptr[DCTSIZE*4]

	pmullw		mm4, [edi + 8*8]		;tmp2 = DEQUANTIZE(inptr[DCTSIZE*4], quantptr[DCTSIZE*4]);
	psubw		mm5, mm1				;tmp12 = MULTIPLY(tmp1 - tmp3, FIX_1_414213562) - tmp13; /* 2*c4 */

	movq		[esi+8*4], mm0		;save tmp4 in workspace
	movq		mm1, mm7			;copy tmp0	/* phase 3 */

	movq		[esi+8*2], mm5		;save tmp12 in workspace
	psubw		mm1, mm4			;tmp11 = tmp0 - tmp2;

	paddw		mm7, mm4			;tmp10 = tmp0 + tmp2;
    movq		mm5, mm1		;copy tmp11

	paddw		mm1, [esi+8*2]	;tmp1 = tmp11 + tmp12;
	movq		mm4, mm7		;copy tmp10		/* phase 2 */

	paddw		mm7, [esi+8*0]	;tmp0 = tmp10 + tmp13;

	psubw		mm4, [esi+8*0]	;tmp3 = tmp10 - tmp13;
	movq		mm0, mm7		;copy tmp0

	psubw		mm5, [esi+8*2]	;tmp2 = tmp11 - tmp12;
	paddw		mm7, mm3		;wsptr[DCTSIZE*0] = (int) (tmp0 + tmp7);

	psubw		mm0, mm3			;wsptr[DCTSIZE*7] = (int) (tmp0 - tmp7);

	movq		[esi + 8*0], mm7	;wsptr[DCTSIZE*0]
	movq		mm3, mm1			;copy tmp1

	movq		[esi + 8*14], mm0	;wsptr[DCTSIZE*7]
	paddw		mm1, mm2			;wsptr[DCTSIZE*1] = (int) (tmp1 + tmp6);

	psubw		mm3, mm2			;wsptr[DCTSIZE*6] = (int) (tmp1 - tmp6);

	movq		[esi + 8*2], mm1	;wsptr[DCTSIZE*1]
	movq		mm1, mm4			;copy tmp3

	movq		[esi + 8*12], mm3	;wsptr[DCTSIZE*6]

	paddw		mm4, [esi+8*4]		;wsptr[DCTSIZE*4] = (int) (tmp3 + tmp4);

	psubw		mm1, [esi+8*4]		;wsptr[DCTSIZE*3] = (int) (tmp3 - tmp4);

	movq		[esi + 8*8], mm4
	movq		mm7, mm5			;copy tmp2

	paddw		mm5, mm6			;wsptr[DCTSIZE*2] = (int) (tmp2 + tmp5)

	movq		[esi+8*6], mm1		;
	psubw		mm7, mm6			;wsptr[DCTSIZE*5] = (int) (tmp2 - tmp5);

	movq		[esi + 8*4], mm5

	movq		[esi + 8*10], mm7




;/*****************************************************************/
;
;  /* Pass 2: process rows from work array, store into output array. */
;  /* Note that we must descale the results by a factor of 8 == 2**3, */
;  /* and also undo the PASS1_BITS scaling. */
;
;/*****************************************************************/
    ; Even part

	mov			esi, eax
	mov			eax, [esp+20+12]

;    tmp10 = ((DCTELEM) wsptr[0] + (DCTELEM) wsptr[4]);
;    tmp13 = ((DCTELEM) wsptr[2] + (DCTELEM) wsptr[6]);
;    tmp11 = ((DCTELEM) wsptr[0] - (DCTELEM) wsptr[4]);
;    tmp14 = ((DCTELEM) wsptr[2] - (DCTELEM) wsptr[6]);
	movq		mm0, [esi+8*0]		;wsptr[0,0],[0,1],[0,2],[0,3]

	movq		mm1, [esi+8*1]		;wsptr[0,4],[0,5],[0,6],[0,7]
	movq		mm2, mm0

	movq		mm3, [esi+8*2]		;wsptr[1,0],[1,1],[1,2],[1,3]
	paddw		mm0, mm1			;wsptr[0,tmp10],[xxx],[0,tmp13],[xxx]

	movq		mm4, [esi+8*3]		;wsptr[1,4],[1,5],[1,6],[1,7]
	psubw		mm2, mm1			;wsptr[0,tmp11],[xxx],[0,tmp14],[xxx]

	movq		mm6, mm0
	movq		mm5, mm3

	paddw		mm3, mm4			;wsptr[1,tmp10],[xxx],[1,tmp13],[xxx]
	movq		mm1, mm2

	psubw		mm5, mm4			;wsptr[1,tmp11],[xxx],[1,tmp14],[xxx]
	punpcklwd	mm0, mm3			;wsptr[0,tmp10],[1,tmp10],[xxx],[xxx]

	movq		mm7, [esi+8*7]		;wsptr[3,4],[3,5],[3,6],[3,7]
	punpckhwd	mm6, mm3			;wsptr[0,tmp13],[1,tmp13],[xxx],[xxx]

	movq		mm3, [esi+8*4]		;wsptr[2,0],[2,1],[2,2],[2,3]
	punpckldq	mm0, mm6	;wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]

	punpcklwd	mm1, mm5			;wsptr[0,tmp11],[1,tmp11],[xxx],[xxx]
	movq		mm4, mm3

	movq		mm6, [esi+8*6]		;wsptr[3,0],[3,1],[3,2],[3,3]
	punpckhwd	mm2, mm5			;wsptr[0,tmp14],[1,tmp14],[xxx],[xxx]

	movq		mm5, [esi+8*5]		;wsptr[2,4],[2,5],[2,6],[2,7]
	punpckldq	mm1, mm2	;wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


	paddw		mm3, mm5			;wsptr[2,tmp10],[xxx],[2,tmp13],[xxx]
	movq		mm2, mm6

	psubw		mm4, mm5			;wsptr[2,tmp11],[xxx],[2,tmp14],[xxx]
	paddw		mm6, mm7			;wsptr[3,tmp10],[xxx],[3,tmp13],[xxx]

	movq		mm5, mm3
	punpcklwd	mm3, mm6			;wsptr[2,tmp10],[3,tmp10],[xxx],[xxx]

	psubw		mm2, mm7			;wsptr[3,tmp11],[xxx],[3,tmp14],[xxx]
	punpckhwd	mm5, mm6			;wsptr[2,tmp13],[3,tmp13],[xxx],[xxx]

	movq		mm7, mm4
	punpckldq	mm3, mm5	;wsptr[2,tmp10],[3,tmp10],[2,tmp13],[3,tmp13]

	punpcklwd	mm4, mm2			;wsptr[2,tmp11],[3,tmp11],[xxx],[xxx]

	punpckhwd	mm7, mm2			;wsptr[2,tmp14],[3,tmp14],[xxx],[xxx]

	punpckldq	mm4, mm7	;wsptr[2,tmp11],[3,tmp11],[2,tmp14],[3,tmp14]
	movq		mm6, mm1

;	mm0 = 	;wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]
;	mm1 =	;wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


	movq		mm2, mm0
	punpckhdq	mm6, mm4	;wsptr[0,tmp14],[1,tmp14],[2,tmp14],[3,tmp14]

	punpckldq	mm1, mm4	;wsptr[0,tmp11],[1,tmp11],[2,tmp11],[3,tmp11]
	psllw		mm6, 2

	pmulhw		mm6, qword [fix_141]
	punpckldq	mm0, mm3	;wsptr[0,tmp10],[1,tmp10],[2,tmp10],[3,tmp10]

	punpckhdq	mm2, mm3	;wsptr[0,tmp13],[1,tmp13],[2,tmp13],[3,tmp13]
	movq		mm7, mm0

;    tmp0 = tmp10 + tmp13;
;    tmp3 = tmp10 - tmp13;
	paddw		mm0, mm2	;[0,tmp0],[1,tmp0],[2,tmp0],[3,tmp0]
	psubw		mm7, mm2	;[0,tmp3],[1,tmp3],[2,tmp3],[3,tmp3]

;    tmp12 = MULTIPLY(tmp14, FIX_1_414213562) - tmp13;
	psubw		mm6, mm2	;wsptr[0,tmp12],[1,tmp12],[2,tmp12],[3,tmp12]
;    tmp1 = tmp11 + tmp12;
;    tmp2 = tmp11 - tmp12;
	movq		mm5, mm1



    ; Odd part

;    z13 = (DCTELEM) wsptr[5] + (DCTELEM) wsptr[3];
;    z10 = (DCTELEM) wsptr[5] - (DCTELEM) wsptr[3];
;    z11 = (DCTELEM) wsptr[1] + (DCTELEM) wsptr[7];
;    z12 = (DCTELEM) wsptr[1] - (DCTELEM) wsptr[7];
	movq		mm3, [esi+8*0]		;wsptr[0,0],[0,1],[0,2],[0,3]
	paddw		mm1, mm6	;[0,tmp1],[1,tmp1],[2,tmp1],[3,tmp1]

	movq		mm4, [esi+8*1]		;wsptr[0,4],[0,5],[0,6],[0,7]
	psubw		mm5, mm6	;[0,tmp2],[1,tmp2],[2,tmp2],[3,tmp2]

	movq		mm6, mm3
	punpckldq	mm3, mm4			;wsptr[0,0],[0,1],[0,4],[0,5]

	punpckhdq	mm4, mm6			;wsptr[0,6],[0,7],[0,2],[0,3]
	movq		mm2, mm3

;Save tmp0 and tmp1 in wsptr
	movq		[esi+8*0], mm0		;save tmp0
	paddw		mm2, mm4			;wsptr[xxx],[0,z11],[xxx],[0,z13]


;Continue with z10 --- z13
	movq		mm6, [esi+8*2]		;wsptr[1,0],[1,1],[1,2],[1,3]
	psubw		mm3, mm4			;wsptr[xxx],[0,z12],[xxx],[0,z10]

	movq		mm0, [esi+8*3]		;wsptr[1,4],[1,5],[1,6],[1,7]
	movq		mm4, mm6

	movq		[esi+8*1], mm1		;save tmp1
	punpckldq	mm6, mm0			;wsptr[1,0],[1,1],[1,4],[1,5]

	punpckhdq	mm0, mm4			;wsptr[1,6],[1,7],[1,2],[1,3]
	movq		mm1, mm6

;Save tmp2 and tmp3 in wsptr
	paddw		mm6, mm0		;wsptr[xxx],[1,z11],[xxx],[1,z13]
	movq		mm4, mm2

;Continue with z10 --- z13
	movq		[esi+8*2], mm5		;save tmp2
	punpcklwd	mm2, mm6		;wsptr[xxx],[xxx],[0,z11],[1,z11]

	psubw		mm1, mm0		;wsptr[xxx],[1,z12],[xxx],[1,z10]
	punpckhwd	mm4, mm6		;wsptr[xxx],[xxx],[0,z13],[1,z13]

	movq		mm0, mm3
	punpcklwd	mm3, mm1		;wsptr[xxx],[xxx],[0,z12],[1,z12]

	movq		[esi+8*3], mm7		;save tmp3
	punpckhwd	mm0, mm1		;wsptr[xxx],[xxx],[0,z10],[1,z10]

	movq		mm6, [esi+8*4]		;wsptr[2,0],[2,1],[2,2],[2,3]
	punpckhdq	mm0, mm2		;wsptr[0,z10],[1,z10],[0,z11],[1,z11]

	movq		mm7, [esi+8*5]		;wsptr[2,4],[2,5],[2,6],[2,7]
	punpckhdq	mm3, mm4		;wsptr[0,z12],[1,z12],[0,z13],[1,z13]

	movq		mm1, [esi+8*6]		;wsptr[3,0],[3,1],[3,2],[3,3]
	movq		mm4, mm6

	punpckldq	mm6, mm7			;wsptr[2,0],[2,1],[2,4],[2,5]
	movq		mm5, mm1

	punpckhdq	mm7, mm4			;wsptr[2,6],[2,7],[2,2],[2,3]
	movq		mm2, mm6

	movq		mm4, [esi+8*7]		;wsptr[3,4],[3,5],[3,6],[3,7]
	paddw		mm6, mm7		;wsptr[xxx],[2,z11],[xxx],[2,z13]

	psubw		mm2, mm7		;wsptr[xxx],[2,z12],[xxx],[2,z10]
	punpckldq	mm1, mm4			;wsptr[3,0],[3,1],[3,4],[3,5]

	punpckhdq	mm4, mm5			;wsptr[3,6],[3,7],[3,2],[3,3]
	movq		mm7, mm1

	paddw		mm1, mm4		;wsptr[xxx],[3,z11],[xxx],[3,z13]
	psubw		mm7, mm4		;wsptr[xxx],[3,z12],[xxx],[3,z10]

	movq		mm5, mm6
	punpcklwd	mm6, mm1		;wsptr[xxx],[xxx],[2,z11],[3,z11]

	punpckhwd	mm5, mm1		;wsptr[xxx],[xxx],[2,z13],[3,z13]
	movq		mm4, mm2

	punpcklwd	mm2, mm7		;wsptr[xxx],[xxx],[2,z12],[3,z12]

	punpckhwd	mm4, mm7		;wsptr[xxx],[xxx],[2,z10],[3,z10]

	punpckhdq	mm4, mm6		;wsptr[2,z10],[3,z10],[2,z11],[3,z11]

	punpckhdq	mm2, mm5		;wsptr[2,z12],[3,z12],[2,z13],[3,z13]
	movq		mm5, mm0

	punpckldq	mm0, mm4		;wsptr[0,z10],[1,z10],[2,z10],[3,z10]

	punpckhdq	mm5, mm4		;wsptr[0,z11],[1,z11],[2,z11],[3,z11]
	movq		mm4, mm3

	punpckhdq	mm4, mm2		;wsptr[0,z13],[1,z13],[2,z13],[3,z13]
	movq		mm1, mm5

	punpckldq	mm3, mm2		;wsptr[0,z12],[1,z12],[2,z12],[3,z12]
;    tmp7 = z11 + z13;		/* phase 5 */
;    tmp8 = z11 - z13;		/* phase 5 */
	psubw		mm1, mm4		;tmp8

	paddw		mm5, mm4		;tmp7
;    tmp21 = MULTIPLY(tmp8, FIX_1_414213562); /* 2*c4 */
	psllw		mm1, 2

	psllw		mm0, 2

	pmulhw		mm1, qword [fix_141]	;tmp21
;    tmp20 = MULTIPLY(z12, (FIX_1_082392200- FIX_1_847759065))  /* 2*(c2-c6) */
;			+ MULTIPLY(z10, - FIX_1_847759065); /* 2*c2 */
	psllw		mm3, 2
	movq		mm7, mm0

	pmulhw		mm7, qword [fix_n184]
	movq		mm6, mm3

	movq		mm2, [esi+8*0]	;tmp0,final1

	pmulhw		mm6, qword [fix_108n184]
;	 tmp22 = MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) /* -2*(c2+c6) */
;			+ MULTIPLY(z12, FIX_1_847759065); /* 2*c2 */
	movq		mm4, mm2		;final1

	pmulhw		mm0, qword [fix_184n261]
	paddw		mm2, mm5		;tmp0+tmp7,final1

	pmulhw		mm3, qword [fix_184]
	psubw		mm4, mm5		;tmp0-tmp7,final1

;    tmp6 = tmp22 - tmp7;	/* phase 2 */
	psraw		mm2, 5			;outptr[0,0],[1,0],[2,0],[3,0],final1

	paddsw		mm2, qword [const_0x0080]	;final1
	paddw		mm7, mm6			;tmp20
	psraw		mm4, 5			;outptr[0,7],[1,7],[2,7],[3,7],final1

	paddsw		mm4, qword [const_0x0080]	;final1
	paddw		mm3, mm0			;tmp22

;    tmp5 = tmp21 - tmp6;
	psubw		mm3, mm5		;tmp6

;    tmp4 = tmp20 + tmp5;
	movq		mm0, [esi+8*1]		;tmp1,final2
	psubw		mm1, mm3		;tmp5

	movq		mm6, mm0			;final2
	paddw		mm0, mm3		;tmp1+tmp6,final2

    ; Final output stage: scale down by a factor of 8 and range-limit


;    outptr[0] = range_limit[IDESCALE(tmp0 + tmp7, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[7] = range_limit[IDESCALE(tmp0 - tmp7, PASS1_BITS+3)
;			    & RANGE_MASK];	final1


;    outptr[1] = range_limit[IDESCALE(tmp1 + tmp6, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[6] = range_limit[IDESCALE(tmp1 - tmp6, PASS1_BITS+3)
;			    & RANGE_MASK];	final2
	psubw		mm6, mm3		;tmp1-tmp6,final2
	psraw		mm0, 5			;outptr[0,1],[1,1],[2,1],[3,1]

	paddsw		mm0, qword [const_0x0080]
	psraw		mm6, 5			;outptr[0,6],[1,6],[2,6],[3,6]

	paddsw		mm6, qword [const_0x0080]		;need to check this value
	packuswb	mm0, mm4	;out[0,1],[1,1],[2,1],[3,1],[0,7],[1,7],[2,7],[3,7]

	movq		mm5, [esi+8*2]		;tmp2,final3
	packuswb	mm2, mm6	;out[0,0],[1,0],[2,0],[3,0],[0,6],[1,6],[2,6],[3,6]

;    outptr[2] = range_limit[IDESCALE(tmp2 + tmp5, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[5] = range_limit[IDESCALE(tmp2 - tmp5, PASS1_BITS+3)
;			    & RANGE_MASK];	final3
	paddw		mm7, mm1		;tmp4
	movq		mm3, mm5

	paddw		mm5, mm1		;tmp2+tmp5
	psubw		mm3, mm1		;tmp2-tmp5

	psraw		mm5, 5			;outptr[0,2],[1,2],[2,2],[3,2]

	paddsw		mm5, qword [const_0x0080]
	movq		mm4, [esi+8*3]		;tmp3,final4
	psraw		mm3, 5			;outptr[0,5],[1,5],[2,5],[3,5]

	paddsw		mm3, qword [const_0x0080]


;    outptr[4] = range_limit[IDESCALE(tmp3 + tmp4, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[3] = range_limit[IDESCALE(tmp3 - tmp4, PASS1_BITS+3)
;			    & RANGE_MASK];	final4
	movq		mm6, mm4
	paddw		mm4, mm7		;tmp3+tmp4

	psubw		mm6, mm7		;tmp3-tmp4
	psraw		mm4, 5			;outptr[0,4],[1,4],[2,4],[3,4]
	mov			ecx, [eax]

	paddsw		mm4, qword [const_0x0080]
	psraw		mm6, 5			;outptr[0,3],[1,3],[2,3],[3,3]

	paddsw		mm6, qword [const_0x0080]
	packuswb	mm5, mm4	;out[0,2],[1,2],[2,2],[3,2],[0,4],[1,4],[2,4],[3,4]

	packuswb	mm6, mm3	;out[0,3],[1,3],[2,3],[3,3],[0,5],[1,5],[2,5],[3,5]
	movq		mm4, mm2

	movq		mm7, mm5
	punpcklbw	mm2, mm0	;out[0,0],[0,1],[1,0],[1,1],[2,0],[2,1],[3,0],[3,1]

	punpckhbw	mm4, mm0	;out[0,6],[0,7],[1,6],[1,7],[2,6],[2,7],[3,6],[3,7]
	movq		mm1, mm2

	punpcklbw	mm5, mm6	;out[0,2],[0,3],[1,2],[1,3],[2,2],[2,3],[3,2],[3,3]
	add		 	eax, 4

	punpckhbw	mm7, mm6	;out[0,4],[0,5],[1,4],[1,5],[2,4],[2,5],[3,4],[3,5]

	punpcklwd	mm2, mm5	;out[0,0],[0,1],[0,2],[0,3],[1,0],[1,1],[1,2],[1,3]
	add			ecx, [esp+20+16]

	movq		mm6, mm7
	punpckhwd	mm1, mm5	;out[2,0],[2,1],[2,2],[2,3],[3,0],[3,1],[3,2],[3,3]

	movq		mm0, mm2
	punpcklwd	mm6, mm4	;out[0,4],[0,5],[0,6],[0,7],[1,4],[1,5],[1,6],[1,7]

	mov			ebx, [eax]
	punpckldq	mm2, mm6	;out[0,0],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7]

	add		 	eax, 4
	movq		mm3, mm1

	add			ebx, [esp+20+16]
	punpckhwd	mm7, mm4	;out[2,4],[2,5],[2,6],[2,7],[3,4],[3,5],[3,6],[3,7]

	movq		[ecx], mm2
	punpckhdq	mm0, mm6	;out[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],[1,6],[1,7]

	mov			ecx, [eax]
	add		 	eax, 4
	add			ecx, [esp+20+16]

	movq		[ebx], mm0
	punpckldq	mm1, mm7	;out[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6],[2,7]

	mov			ebx, [eax]

	add			ebx, [esp+20+16]
	punpckhdq	mm3, mm7	;out[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7]
	movq		[ecx], mm1


	movq		[ebx], mm3



;/*******************************************************************/


	add			esi, 64
	add			eax, 4

;/*******************************************************************/

;    tmp10 = ((DCTELEM) wsptr[0] + (DCTELEM) wsptr[4]);
;    tmp13 = ((DCTELEM) wsptr[2] + (DCTELEM) wsptr[6]);
;    tmp11 = ((DCTELEM) wsptr[0] - (DCTELEM) wsptr[4]);
;    tmp14 = ((DCTELEM) wsptr[2] - (DCTELEM) wsptr[6]);
	movq		mm0, [esi+8*0]		;wsptr[0,0],[0,1],[0,2],[0,3]

	movq		mm1, [esi+8*1]		;wsptr[0,4],[0,5],[0,6],[0,7]
	movq		mm2, mm0

	movq		mm3, [esi+8*2]		;wsptr[1,0],[1,1],[1,2],[1,3]
	paddw		mm0, mm1			;wsptr[0,tmp10],[xxx],[0,tmp13],[xxx]

	movq		mm4, [esi+8*3]		;wsptr[1,4],[1,5],[1,6],[1,7]
	psubw		mm2, mm1			;wsptr[0,tmp11],[xxx],[0,tmp14],[xxx]

	movq		mm6, mm0
	movq		mm5, mm3

	paddw		mm3, mm4			;wsptr[1,tmp10],[xxx],[1,tmp13],[xxx]
	movq		mm1, mm2

	psubw		mm5, mm4			;wsptr[1,tmp11],[xxx],[1,tmp14],[xxx]
	punpcklwd	mm0, mm3			;wsptr[0,tmp10],[1,tmp10],[xxx],[xxx]

	movq		mm7, [esi+8*7]		;wsptr[3,4],[3,5],[3,6],[3,7]
	punpckhwd	mm6, mm3			;wsptr[0,tmp13],[1,tmp13],[xxx],[xxx]

	movq		mm3, [esi+8*4]		;wsptr[2,0],[2,1],[2,2],[2,3]
	punpckldq	mm0, mm6	;wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]

	punpcklwd	mm1, mm5			;wsptr[0,tmp11],[1,tmp11],[xxx],[xxx]
	movq		mm4, mm3

	movq		mm6, [esi+8*6]		;wsptr[3,0],[3,1],[3,2],[3,3]
	punpckhwd	mm2, mm5			;wsptr[0,tmp14],[1,tmp14],[xxx],[xxx]

	movq		mm5, [esi+8*5]		;wsptr[2,4],[2,5],[2,6],[2,7]
	punpckldq	mm1, mm2	;wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


	paddw		mm3, mm5			;wsptr[2,tmp10],[xxx],[2,tmp13],[xxx]
	movq		mm2, mm6

	psubw		mm4, mm5			;wsptr[2,tmp11],[xxx],[2,tmp14],[xxx]
	paddw		mm6, mm7			;wsptr[3,tmp10],[xxx],[3,tmp13],[xxx]

	movq		mm5, mm3
	punpcklwd	mm3, mm6			;wsptr[2,tmp10],[3,tmp10],[xxx],[xxx]

	psubw		mm2, mm7			;wsptr[3,tmp11],[xxx],[3,tmp14],[xxx]
	punpckhwd	mm5, mm6			;wsptr[2,tmp13],[3,tmp13],[xxx],[xxx]

	movq		mm7, mm4
	punpckldq	mm3, mm5	;wsptr[2,tmp10],[3,tmp10],[2,tmp13],[3,tmp13]

	punpcklwd	mm4, mm2			;wsptr[2,tmp11],[3,tmp11],[xxx],[xxx]

	punpckhwd	mm7, mm2			;wsptr[2,tmp14],[3,tmp14],[xxx],[xxx]

	punpckldq	mm4, mm7	;wsptr[2,tmp11],[3,tmp11],[2,tmp14],[3,tmp14]
	movq		mm6, mm1

;	mm0 = 	;wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]
;	mm1 =	;wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


	movq		mm2, mm0
	punpckhdq	mm6, mm4	;wsptr[0,tmp14],[1,tmp14],[2,tmp14],[3,tmp14]

	punpckldq	mm1, mm4	;wsptr[0,tmp11],[1,tmp11],[2,tmp11],[3,tmp11]
	psllw		mm6, 2

	pmulhw		mm6, qword [fix_141]
	punpckldq	mm0, mm3	;wsptr[0,tmp10],[1,tmp10],[2,tmp10],[3,tmp10]

	punpckhdq	mm2, mm3	;wsptr[0,tmp13],[1,tmp13],[2,tmp13],[3,tmp13]
	movq		mm7, mm0

;    tmp0 = tmp10 + tmp13;
;    tmp3 = tmp10 - tmp13;
	paddw		mm0, mm2	;[0,tmp0],[1,tmp0],[2,tmp0],[3,tmp0]
	psubw		mm7, mm2	;[0,tmp3],[1,tmp3],[2,tmp3],[3,tmp3]

;    tmp12 = MULTIPLY(tmp14, FIX_1_414213562) - tmp13;
	psubw		mm6, mm2	;wsptr[0,tmp12],[1,tmp12],[2,tmp12],[3,tmp12]
;    tmp1 = tmp11 + tmp12;
;    tmp2 = tmp11 - tmp12;
	movq		mm5, mm1



    ; Odd part

;    z13 = (DCTELEM) wsptr[5] + (DCTELEM) wsptr[3];
;    z10 = (DCTELEM) wsptr[5] - (DCTELEM) wsptr[3];
;    z11 = (DCTELEM) wsptr[1] + (DCTELEM) wsptr[7];
;    z12 = (DCTELEM) wsptr[1] - (DCTELEM) wsptr[7];
	movq		mm3, [esi+8*0]		;wsptr[0,0],[0,1],[0,2],[0,3]
	paddw		mm1, mm6	;[0,tmp1],[1,tmp1],[2,tmp1],[3,tmp1]

	movq		mm4, [esi+8*1]		;wsptr[0,4],[0,5],[0,6],[0,7]
	psubw		mm5, mm6	;[0,tmp2],[1,tmp2],[2,tmp2],[3,tmp2]

	movq		mm6, mm3
	punpckldq	mm3, mm4			;wsptr[0,0],[0,1],[0,4],[0,5]

	punpckhdq	mm4, mm6			;wsptr[0,6],[0,7],[0,2],[0,3]
	movq		mm2, mm3

;Save tmp0 and tmp1 in wsptr
	movq		[esi+8*0], mm0		;save tmp0
	paddw		mm2, mm4			;wsptr[xxx],[0,z11],[xxx],[0,z13]


;Continue with z10 --- z13
	movq		mm6, [esi+8*2]		;wsptr[1,0],[1,1],[1,2],[1,3]
	psubw		mm3, mm4			;wsptr[xxx],[0,z12],[xxx],[0,z10]

	movq		mm0, [esi+8*3]		;wsptr[1,4],[1,5],[1,6],[1,7]
	movq		mm4, mm6

	movq		[esi+8*1], mm1		;save tmp1
	punpckldq	mm6, mm0			;wsptr[1,0],[1,1],[1,4],[1,5]

	punpckhdq	mm0, mm4			;wsptr[1,6],[1,7],[1,2],[1,3]
	movq		mm1, mm6

;Save tmp2 and tmp3 in wsptr
	paddw		mm6, mm0		;wsptr[xxx],[1,z11],[xxx],[1,z13]
	movq		mm4, mm2

;Continue with z10 --- z13
	movq		[esi+8*2], mm5		;save tmp2
	punpcklwd	mm2, mm6		;wsptr[xxx],[xxx],[0,z11],[1,z11]

	psubw		mm1, mm0		;wsptr[xxx],[1,z12],[xxx],[1,z10]
	punpckhwd	mm4, mm6		;wsptr[xxx],[xxx],[0,z13],[1,z13]

	movq		mm0, mm3
	punpcklwd	mm3, mm1		;wsptr[xxx],[xxx],[0,z12],[1,z12]

	movq		[esi+8*3], mm7		;save tmp3
	punpckhwd	mm0, mm1		;wsptr[xxx],[xxx],[0,z10],[1,z10]

	movq		mm6, [esi+8*4]		;wsptr[2,0],[2,1],[2,2],[2,3]
	punpckhdq	mm0, mm2		;wsptr[0,z10],[1,z10],[0,z11],[1,z11]

	movq		mm7, [esi+8*5]		;wsptr[2,4],[2,5],[2,6],[2,7]
	punpckhdq	mm3, mm4		;wsptr[0,z12],[1,z12],[0,z13],[1,z13]

	movq		mm1, [esi+8*6]		;wsptr[3,0],[3,1],[3,2],[3,3]
	movq		mm4, mm6

	punpckldq	mm6, mm7			;wsptr[2,0],[2,1],[2,4],[2,5]
	movq		mm5, mm1

	punpckhdq	mm7, mm4			;wsptr[2,6],[2,7],[2,2],[2,3]
	movq		mm2, mm6

	movq		mm4, [esi+8*7]		;wsptr[3,4],[3,5],[3,6],[3,7]
	paddw		mm6, mm7		;wsptr[xxx],[2,z11],[xxx],[2,z13]

	psubw		mm2, mm7		;wsptr[xxx],[2,z12],[xxx],[2,z10]
	punpckldq	mm1, mm4			;wsptr[3,0],[3,1],[3,4],[3,5]

	punpckhdq	mm4, mm5			;wsptr[3,6],[3,7],[3,2],[3,3]
	movq		mm7, mm1

	paddw		mm1, mm4		;wsptr[xxx],[3,z11],[xxx],[3,z13]
	psubw		mm7, mm4		;wsptr[xxx],[3,z12],[xxx],[3,z10]

	movq		mm5, mm6
	punpcklwd	mm6, mm1		;wsptr[xxx],[xxx],[2,z11],[3,z11]

	punpckhwd	mm5, mm1		;wsptr[xxx],[xxx],[2,z13],[3,z13]
	movq		mm4, mm2

	punpcklwd	mm2, mm7		;wsptr[xxx],[xxx],[2,z12],[3,z12]

	punpckhwd	mm4, mm7		;wsptr[xxx],[xxx],[2,z10],[3,z10]

	punpckhdq	mm4, mm6		;wsptr[2,z10],[3,z10],[2,z11],[3,z11]

	punpckhdq	mm2, mm5		;wsptr[2,z12],[3,z12],[2,z13],[3,z13]
	movq		mm5, mm0

	punpckldq	mm0, mm4		;wsptr[0,z10],[1,z10],[2,z10],[3,z10]

	punpckhdq	mm5, mm4		;wsptr[0,z11],[1,z11],[2,z11],[3,z11]
	movq		mm4, mm3

	punpckhdq	mm4, mm2		;wsptr[0,z13],[1,z13],[2,z13],[3,z13]
	movq		mm1, mm5

	punpckldq	mm3, mm2		;wsptr[0,z12],[1,z12],[2,z12],[3,z12]
;    tmp7 = z11 + z13;		/* phase 5 */
;    tmp8 = z11 - z13;		/* phase 5 */
	psubw		mm1, mm4		;tmp8

	paddw		mm5, mm4		;tmp7
;    tmp21 = MULTIPLY(tmp8, FIX_1_414213562); /* 2*c4 */
	psllw		mm1, 2

	psllw		mm0, 2

	pmulhw		mm1, qword [fix_141]	;tmp21
;    tmp20 = MULTIPLY(z12, (FIX_1_082392200- FIX_1_847759065))  /* 2*(c2-c6) */
;			+ MULTIPLY(z10, - FIX_1_847759065); /* 2*c2 */
	psllw		mm3, 2
	movq		mm7, mm0

	pmulhw		mm7, qword [fix_n184]
	movq		mm6, mm3

	movq		mm2, [esi+8*0]	;tmp0,final1

	pmulhw		mm6, qword [fix_108n184]
;	 tmp22 = MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) /* -2*(c2+c6) */
;			+ MULTIPLY(z12, FIX_1_847759065); /* 2*c2 */
	movq		mm4, mm2		;final1

	pmulhw		mm0, qword [fix_184n261]
	paddw		mm2, mm5		;tmp0+tmp7,final1

	pmulhw		mm3, qword [fix_184]
	psubw		mm4, mm5		;tmp0-tmp7,final1

;    tmp6 = tmp22 - tmp7;	/* phase 2 */
	psraw		mm2, 5			;outptr[0,0],[1,0],[2,0],[3,0],final1

	paddsw		mm2, qword [const_0x0080]	;final1
	paddw		mm7, mm6			;tmp20
	psraw		mm4, 5			;outptr[0,7],[1,7],[2,7],[3,7],final1

	paddsw		mm4, qword [const_0x0080]	;final1
	paddw		mm3, mm0			;tmp22

;    tmp5 = tmp21 - tmp6;
	psubw		mm3, mm5		;tmp6

;    tmp4 = tmp20 + tmp5;
	movq		mm0, [esi+8*1]		;tmp1,final2
	psubw		mm1, mm3		;tmp5

	movq		mm6, mm0			;final2
	paddw		mm0, mm3		;tmp1+tmp6,final2

    ; Final output stage: scale down by a factor of 8 and range-limit


;    outptr[0] = range_limit[IDESCALE(tmp0 + tmp7, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[7] = range_limit[IDESCALE(tmp0 - tmp7, PASS1_BITS+3)
;			    & RANGE_MASK];	final1


;    outptr[1] = range_limit[IDESCALE(tmp1 + tmp6, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[6] = range_limit[IDESCALE(tmp1 - tmp6, PASS1_BITS+3)
;			    & RANGE_MASK];	final2
	psubw		mm6, mm3		;tmp1-tmp6,final2
	psraw		mm0, 5			;outptr[0,1],[1,1],[2,1],[3,1]

	paddsw		mm0, qword [const_0x0080]
	psraw		mm6, 5			;outptr[0,6],[1,6],[2,6],[3,6]

	paddsw		mm6, qword [const_0x0080]		;need to check this value
	packuswb	mm0, mm4	;out[0,1],[1,1],[2,1],[3,1],[0,7],[1,7],[2,7],[3,7]

	movq		mm5, [esi+8*2]		;tmp2,final3
	packuswb	mm2, mm6	;out[0,0],[1,0],[2,0],[3,0],[0,6],[1,6],[2,6],[3,6]

;    outptr[2] = range_limit[IDESCALE(tmp2 + tmp5, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[5] = range_limit[IDESCALE(tmp2 - tmp5, PASS1_BITS+3)
;			    & RANGE_MASK];	final3
	paddw		mm7, mm1		;tmp4
	movq		mm3, mm5

	paddw		mm5, mm1		;tmp2+tmp5
	psubw		mm3, mm1		;tmp2-tmp5

	psraw		mm5, 5			;outptr[0,2],[1,2],[2,2],[3,2]

	paddsw		mm5, qword [const_0x0080]
	movq		mm4, [esi+8*3]		;tmp3,final4
	psraw		mm3, 5			;outptr[0,5],[1,5],[2,5],[3,5]

	paddsw		mm3, qword [const_0x0080]


;    outptr[4] = range_limit[IDESCALE(tmp3 + tmp4, PASS1_BITS+3)
;			    & RANGE_MASK];
;    outptr[3] = range_limit[IDESCALE(tmp3 - tmp4, PASS1_BITS+3)
;			    & RANGE_MASK];	final4
	movq		mm6, mm4
	paddw		mm4, mm7		;tmp3+tmp4

	psubw		mm6, mm7		;tmp3-tmp4
	psraw		mm4, 5			;outptr[0,4],[1,4],[2,4],[3,4]
	mov			ecx, [eax]

	paddsw		mm4, qword [const_0x0080]
	psraw		mm6, 5			;outptr[0,3],[1,3],[2,3],[3,3]

	paddsw		mm6, qword [const_0x0080]
	packuswb	mm5, mm4	;out[0,2],[1,2],[2,2],[3,2],[0,4],[1,4],[2,4],[3,4]

	packuswb	mm6, mm3	;out[0,3],[1,3],[2,3],[3,3],[0,5],[1,5],[2,5],[3,5]
	movq		mm4, mm2

	movq		mm7, mm5
	punpcklbw	mm2, mm0	;out[0,0],[0,1],[1,0],[1,1],[2,0],[2,1],[3,0],[3,1]

	punpckhbw	mm4, mm0	;out[0,6],[0,7],[1,6],[1,7],[2,6],[2,7],[3,6],[3,7]
	movq		mm1, mm2

	punpcklbw	mm5, mm6	;out[0,2],[0,3],[1,2],[1,3],[2,2],[2,3],[3,2],[3,3]
	add		 	eax, 4

	punpckhbw	mm7, mm6	;out[0,4],[0,5],[1,4],[1,5],[2,4],[2,5],[3,4],[3,5]

	punpcklwd	mm2, mm5	;out[0,0],[0,1],[0,2],[0,3],[1,0],[1,1],[1,2],[1,3]
	add			ecx, [esp+20+16]

	movq		mm6, mm7
	punpckhwd	mm1, mm5	;out[2,0],[2,1],[2,2],[2,3],[3,0],[3,1],[3,2],[3,3]

	movq		mm0, mm2
	punpcklwd	mm6, mm4	;out[0,4],[0,5],[0,6],[0,7],[1,4],[1,5],[1,6],[1,7]

	mov			ebx, [eax]
	punpckldq	mm2, mm6	;out[0,0],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7]

	add		 	eax, 4
	movq		mm3, mm1

	add			ebx, [esp+20+16]
	punpckhwd	mm7, mm4	;out[2,4],[2,5],[2,6],[2,7],[3,4],[3,5],[3,6],[3,7]

	movq		[ecx], mm2
	punpckhdq	mm0, mm6	;out[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],[1,6],[1,7]

	mov			ecx, [eax]
	add		 	eax, 4
	add			ecx, [esp+20+16]

	movq		[ebx], mm0
	punpckldq	mm1, mm7	;out[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6],[2,7]

	mov			ebx, [eax]

	add			ebx, [esp+20+16]
	punpckhdq	mm3, mm7	;out[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7]
	movq		[ecx], mm1

	movq		[ebx], mm3

	; pop mmx registers here!?
	pop		ebx
	pop		ecx
	pop		eax
	pop		edi
	pop		esi
	ret

ALIGN 8
jpeg_idct_ifast_deinit_asm:
	emms
	ret
