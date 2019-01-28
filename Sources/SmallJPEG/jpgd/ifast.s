#
# jpeg_idct_ifast, assembly implementation.
#
# Originally written by Rich Geldreich<richgel@voicenet.com>
# Ported from MSVC++ inline assembly to nasm by Magnus Norddahl <mbn@clanlib.org>
# GNU Assembler (gas) version can be created by running this file through intel2gas.
#


.globl jpeg_idct_ifast_asm
.globl jpeg_idct_ifast_deinit

.text
.align 8

fix_141:         .long      0x5a825a82, 0x5a825a82
fix_184n261:     .long      0xcf04cf04, 0xcf04cf04
fix_184:         .long      0x76417641, 0x76417641
fix_n184:        .long      0x896f896f, 0x896f896f
fix_108n184:     .long      0xcf04cf04, 0xcf04cf04
const_0x0080:    .long      0x00800080, 0x00800080


## jpeg_idct_ifast_asm(
##   BLOCK_TYPE *inptr,
##   short *quantptr,
##   uchar * *outptr,
##   int output_col,
##   short *wsptr)
## -------------------------
##   4: BLOCK_TYPE *inptr
##   8: short *quantptr
##  12: uchar **outptr
##  16: int output_col
##  20: short *wsptr
jpeg_idct_ifast_asm: 

        pushl   %esi
        pushl   %edi
        pushl   %eax
        pushl   %ecx
        pushl   %ebx
        # save mmx registers here!?

#DEFINE inptr      [esp+20+4]
#DEFINE quantptr   [esp+20+8]
#DEFINE outptr     [esp+20+12]
#DEFINE output_col [esp+20+16]
#DEFINE wsptr      [esp+20+20]

        movl            20+8(%esp),%edi
        movl            20+4(%esp),%ebx
        movl            20+20(%esp),%esi
        addl            $0x07,%esi              #align wsptr to qword
        andl            $0xfffffff8,%esi #align wsptr to qword

        movl            %esi,%eax

    # Odd part


        movq            8*10(%ebx),%mm1                 #load inptr[DCTSIZE*5]

        pmullw          8*10(%edi),%mm1                 #tmp6 = DEQUANTIZE(inptr[DCTSIZE*5], quantptr[DCTSIZE*5]);

        movq            8*6(%ebx),%mm0                  #load inptr[DCTSIZE*3]

        pmullw          8*6(%edi),%mm0                  #tmp5 = DEQUANTIZE(inptr[DCTSIZE*3], quantptr[DCTSIZE*3]);

        movq            8*2(%ebx),%mm3                  #load inptr[DCTSIZE*1]
        movq    %mm1,%mm2                                       #copy tmp6      /* phase 6 */

        pmullw          8*2(%edi),%mm3                  #tmp4 = DEQUANTIZE(inptr[DCTSIZE*1], quantptr[DCTSIZE*1]);

        movq            8*14(%ebx),%mm4                 #load inptr[DCTSIZE*1]
        paddw   %mm0,%mm1                                       #z13 = tmp6 + tmp5;

        pmullw          8*14(%edi),%mm4             #tmp7 = DEQUANTIZE(inptr[DCTSIZE*7], quantptr[DCTSIZE*7]);
        psubw   %mm0,%mm2                                       #z10 = tmp6 - tmp5

        psllw           $2,%mm2                         #shift z10
        movq            %mm2,%mm0                       #copy z10

        pmulhw          fix_184n261,%mm2               #MULTIPLY( z12, FIX_1_847759065); /* 2*c2 */
        movq            %mm3,%mm5                               #copy tmp4

        pmulhw          fix_n184,%mm0                  #MULTIPLY(z10, -FIX_1_847759065); /* 2*c2 */
        paddw           %mm4,%mm3                               #z11 = tmp4 + tmp7;

        movq            %mm3,%mm6                               #copy z11                       /* phase 5 */
        psubw           %mm4,%mm5                               #z12 = tmp4 - tmp7;

        psubw           %mm1,%mm6                               #z11-z13
        psllw           $2,%mm5                         #shift z12

        movq            8*12(%ebx),%mm4                 #load inptr[DCTSIZE*6], even part
        movq            %mm5,%mm7                       #copy z12

        pmulhw          fix_108n184,%mm5               #MULT(z12, (FIX_1_08-FIX_1_84)) ;- z5; /* 2*(c2-c6) */ even part
        paddw           %mm1,%mm3                               #tmp7 = z11 + z13;


    # Even part
        pmulhw          fix_184,%mm7                   #MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) ;+ z5; /* -2*(c2+c6) */
        psllw           $2,%mm6

        movq            8*4(%ebx),%mm1                  #load inptr[DCTSIZE*2]

        pmullw          8*4(%edi),%mm1                  #tmp1 = DEQUANTIZE(inptr[DCTSIZE*2], quantptr[DCTSIZE*2]);
        paddw           %mm5,%mm0                       #tmp10

        pmullw          8*12(%edi),%mm4                 #tmp3 = DEQUANTIZE(inptr[DCTSIZE*6], quantptr[DCTSIZE*6]);
        paddw           %mm7,%mm2                       #tmp12

        pmulhw          fix_141,%mm6                           #tmp11 = MULTIPLY(z11 - z13, FIX_1_414213562); /* 2*c4 */
        psubw           %mm3,%mm2               #tmp6 = tmp12 - tmp7

        movq            %mm1,%mm5                               #copy tmp1
        paddw           %mm4,%mm1                               #tmp13= tmp1 + tmp3;    /* phases 5-3 */

        psubw           %mm4,%mm5                               #tmp1-tmp3
        psubw           %mm2,%mm6               #tmp5 = tmp11 - tmp6;

        movq            %mm1,8*0(%esi)                  #save tmp13 in workspace
        psllw           $2,%mm5                                 #shift tmp1-tmp3

        movq            8*0(%ebx),%mm7                  #load inptr[DCTSIZE*0]

        pmulhw          fix_141,%mm5                           #MULTIPLY(tmp1 - tmp3, FIX_1_414213562)
        paddw           %mm6,%mm0               #tmp4 = tmp10 + tmp5;

        pmullw          8*0(%edi),%mm7                  #tmp0 = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);

        movq            8*8(%ebx),%mm4                  #load inptr[DCTSIZE*4]

        pmullw          8*8(%edi),%mm4                  #tmp2 = DEQUANTIZE(inptr[DCTSIZE*4], quantptr[DCTSIZE*4]);
        psubw           %mm1,%mm5                               #tmp12 = MULTIPLY(tmp1 - tmp3, FIX_1_414213562) - tmp13; /* 2*c4 */

        movq            %mm0,8*4(%esi)          #save tmp4 in workspace
        movq            %mm7,%mm1                       #copy tmp0      /* phase 3 */

        movq            %mm5,8*2(%esi)          #save tmp12 in workspace
        psubw           %mm4,%mm1                       #tmp11 = tmp0 - tmp2;

        paddw           %mm4,%mm7                       #tmp10 = tmp0 + tmp2;
    movq                %mm1,%mm5               #copy tmp11

        paddw           8*2(%esi),%mm1  #tmp1 = tmp11 + tmp12;
        movq            %mm7,%mm4               #copy tmp10             /* phase 2 */

        paddw           8*0(%esi),%mm7  #tmp0 = tmp10 + tmp13;

        psubw           8*0(%esi),%mm4  #tmp3 = tmp10 - tmp13;
        movq            %mm7,%mm0               #copy tmp0

        psubw           8*2(%esi),%mm5  #tmp2 = tmp11 - tmp12;
        paddw           %mm3,%mm7               #wsptr[DCTSIZE*0] = (int) (tmp0 + tmp7);

        psubw           %mm3,%mm0                       #wsptr[DCTSIZE*7] = (int) (tmp0 - tmp7);

        movq            %mm7,8*0(%esi)          #wsptr[DCTSIZE*0]
        movq            %mm1,%mm3                       #copy tmp1

        movq            %mm0,8*14(%esi)         #wsptr[DCTSIZE*7]
        paddw           %mm2,%mm1                       #wsptr[DCTSIZE*1] = (int) (tmp1 + tmp6);

        psubw           %mm2,%mm3                       #wsptr[DCTSIZE*6] = (int) (tmp1 - tmp6);

        movq            %mm1,8*2(%esi)          #wsptr[DCTSIZE*1]
        movq            %mm4,%mm1                       #copy tmp3

        movq            %mm3,8*12(%esi)         #wsptr[DCTSIZE*6]

        paddw           8*4(%esi),%mm4          #wsptr[DCTSIZE*4] = (int) (tmp3 + tmp4);

        psubw           8*4(%esi),%mm1          #wsptr[DCTSIZE*3] = (int) (tmp3 - tmp4);

        movq            %mm4,8*8(%esi)
        movq            %mm5,%mm7                       #copy tmp2

        paddw           %mm6,%mm5                       #wsptr[DCTSIZE*2] = (int) (tmp2 + tmp5)

        movq            %mm1,8*6(%esi)          #
        psubw           %mm6,%mm7                       #wsptr[DCTSIZE*5] = (int) (tmp2 - tmp5);

        movq            %mm5,8*4(%esi)

        movq            %mm7,8*10(%esi)



#/*****************************************************************/
        addl            $8,%edi
        addl            $8,%ebx
        addl            $8,%esi

#/*****************************************************************/




        movq            8*10(%ebx),%mm1                 #load inptr[DCTSIZE*5]

        pmullw          8*10(%edi),%mm1                 #tmp6 = DEQUANTIZE(inptr[DCTSIZE*5], quantptr[DCTSIZE*5]);

        movq            8*6(%ebx),%mm0                  #load inptr[DCTSIZE*3]

        pmullw          8*6(%edi),%mm0                  #tmp5 = DEQUANTIZE(inptr[DCTSIZE*3], quantptr[DCTSIZE*3]);

        movq            8*2(%ebx),%mm3                  #load inptr[DCTSIZE*1]
        movq    %mm1,%mm2                                       #copy tmp6      /* phase 6 */

        pmullw          8*2(%edi),%mm3                  #tmp4 = DEQUANTIZE(inptr[DCTSIZE*1], quantptr[DCTSIZE*1]);

        movq            8*14(%ebx),%mm4                 #load inptr[DCTSIZE*1]
        paddw   %mm0,%mm1                                       #z13 = tmp6 + tmp5;

        pmullw          8*14(%edi),%mm4             #tmp7 = DEQUANTIZE(inptr[DCTSIZE*7], quantptr[DCTSIZE*7]);
        psubw   %mm0,%mm2                                       #z10 = tmp6 - tmp5

        psllw           $2,%mm2                         #shift z10
        movq            %mm2,%mm0                       #copy z10

        pmulhw          fix_184n261,%mm2               #MULTIPLY( z12, FIX_1_847759065); /* 2*c2 */
        movq            %mm3,%mm5                               #copy tmp4

        pmulhw          fix_n184,%mm0                  #MULTIPLY(z10, -FIX_1_847759065); /* 2*c2 */
        paddw           %mm4,%mm3                               #z11 = tmp4 + tmp7;

        movq            %mm3,%mm6                               #copy z11                       /* phase 5 */
        psubw           %mm4,%mm5                               #z12 = tmp4 - tmp7;

        psubw           %mm1,%mm6                               #z11-z13
        psllw           $2,%mm5                         #shift z12

        movq            8*12(%ebx),%mm4                 #load inptr[DCTSIZE*6], even part
        movq            %mm5,%mm7                       #copy z12

        pmulhw          fix_108n184,%mm5               #MULT(z12, (FIX_1_08-FIX_1_84)) ;- z5; /* 2*(c2-c6) */ even part
        paddw           %mm1,%mm3                               #tmp7 = z11 + z13;


    # Even part
        pmulhw          fix_184,%mm7                   #MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) ;+ z5; /* -2*(c2+c6) */
        psllw           $2,%mm6

        movq            8*4(%ebx),%mm1                  #load inptr[DCTSIZE*2]

        pmullw          8*4(%edi),%mm1                  #tmp1 = DEQUANTIZE(inptr[DCTSIZE*2], quantptr[DCTSIZE*2]);
        paddw           %mm5,%mm0                       #tmp10

        pmullw          8*12(%edi),%mm4                 #tmp3 = DEQUANTIZE(inptr[DCTSIZE*6], quantptr[DCTSIZE*6]);
        paddw           %mm7,%mm2                       #tmp12

        pmulhw          fix_141,%mm6                           #tmp11 = MULTIPLY(z11 - z13, FIX_1_414213562); /* 2*c4 */
        psubw           %mm3,%mm2               #tmp6 = tmp12 - tmp7

        movq            %mm1,%mm5                               #copy tmp1
        paddw           %mm4,%mm1                               #tmp13= tmp1 + tmp3;    /* phases 5-3 */

        psubw           %mm4,%mm5                               #tmp1-tmp3
        psubw           %mm2,%mm6               #tmp5 = tmp11 - tmp6;

        movq            %mm1,8*0(%esi)                  #save tmp13 in workspace
        psllw           $2,%mm5                                 #shift tmp1-tmp3

        movq            8*0(%ebx),%mm7                  #load inptr[DCTSIZE*0]
        paddw           %mm6,%mm0               #tmp4 = tmp10 + tmp5;

        pmulhw          fix_141,%mm5                           #MULTIPLY(tmp1 - tmp3, FIX_1_414213562)

        pmullw          8*0(%edi),%mm7                  #tmp0 = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);

        movq            8*8(%ebx),%mm4                  #load inptr[DCTSIZE*4]

        pmullw          8*8(%edi),%mm4                  #tmp2 = DEQUANTIZE(inptr[DCTSIZE*4], quantptr[DCTSIZE*4]);
        psubw           %mm1,%mm5                               #tmp12 = MULTIPLY(tmp1 - tmp3, FIX_1_414213562) - tmp13; /* 2*c4 */

        movq            %mm0,8*4(%esi)          #save tmp4 in workspace
        movq            %mm7,%mm1                       #copy tmp0      /* phase 3 */

        movq            %mm5,8*2(%esi)          #save tmp12 in workspace
        psubw           %mm4,%mm1                       #tmp11 = tmp0 - tmp2;

        paddw           %mm4,%mm7                       #tmp10 = tmp0 + tmp2;
    movq                %mm1,%mm5               #copy tmp11

        paddw           8*2(%esi),%mm1  #tmp1 = tmp11 + tmp12;
        movq            %mm7,%mm4               #copy tmp10             /* phase 2 */

        paddw           8*0(%esi),%mm7  #tmp0 = tmp10 + tmp13;

        psubw           8*0(%esi),%mm4  #tmp3 = tmp10 - tmp13;
        movq            %mm7,%mm0               #copy tmp0

        psubw           8*2(%esi),%mm5  #tmp2 = tmp11 - tmp12;
        paddw           %mm3,%mm7               #wsptr[DCTSIZE*0] = (int) (tmp0 + tmp7);

        psubw           %mm3,%mm0                       #wsptr[DCTSIZE*7] = (int) (tmp0 - tmp7);

        movq            %mm7,8*0(%esi)          #wsptr[DCTSIZE*0]
        movq            %mm1,%mm3                       #copy tmp1

        movq            %mm0,8*14(%esi)         #wsptr[DCTSIZE*7]
        paddw           %mm2,%mm1                       #wsptr[DCTSIZE*1] = (int) (tmp1 + tmp6);

        psubw           %mm2,%mm3                       #wsptr[DCTSIZE*6] = (int) (tmp1 - tmp6);

        movq            %mm1,8*2(%esi)          #wsptr[DCTSIZE*1]
        movq            %mm4,%mm1                       #copy tmp3

        movq            %mm3,8*12(%esi)         #wsptr[DCTSIZE*6]

        paddw           8*4(%esi),%mm4          #wsptr[DCTSIZE*4] = (int) (tmp3 + tmp4);

        psubw           8*4(%esi),%mm1          #wsptr[DCTSIZE*3] = (int) (tmp3 - tmp4);

        movq            %mm4,8*8(%esi)
        movq            %mm5,%mm7                       #copy tmp2

        paddw           %mm6,%mm5                       #wsptr[DCTSIZE*2] = (int) (tmp2 + tmp5)

        movq            %mm1,8*6(%esi)          #
        psubw           %mm6,%mm7                       #wsptr[DCTSIZE*5] = (int) (tmp2 - tmp5);

        movq            %mm5,8*4(%esi)

        movq            %mm7,8*10(%esi)




#/*****************************************************************/
#
#  /* Pass 2: process rows from work array, store into output array. */
#  /* Note that we must descale the results by a factor of 8 == 2**3, */
#  /* and also undo the PASS1_BITS scaling. */
#
#/*****************************************************************/
    # Even part

        movl                    %eax,%esi
        movl                    20+12(%esp),%eax

#    tmp10 = ((DCTELEM) wsptr[0] + (DCTELEM) wsptr[4]);
#    tmp13 = ((DCTELEM) wsptr[2] + (DCTELEM) wsptr[6]);
#    tmp11 = ((DCTELEM) wsptr[0] - (DCTELEM) wsptr[4]);
#    tmp14 = ((DCTELEM) wsptr[2] - (DCTELEM) wsptr[6]);
        movq            8*0(%esi),%mm0          #wsptr[0,0],[0,1],[0,2],[0,3]

        movq            8*1(%esi),%mm1          #wsptr[0,4],[0,5],[0,6],[0,7]
        movq            %mm0,%mm2

        movq            8*2(%esi),%mm3          #wsptr[1,0],[1,1],[1,2],[1,3]
        paddw           %mm1,%mm0                       #wsptr[0,tmp10],[xxx],[0,tmp13],[xxx]

        movq            8*3(%esi),%mm4          #wsptr[1,4],[1,5],[1,6],[1,7]
        psubw           %mm1,%mm2                       #wsptr[0,tmp11],[xxx],[0,tmp14],[xxx]

        movq            %mm0,%mm6
        movq            %mm3,%mm5

        paddw           %mm4,%mm3                       #wsptr[1,tmp10],[xxx],[1,tmp13],[xxx]
        movq            %mm2,%mm1

        psubw           %mm4,%mm5                       #wsptr[1,tmp11],[xxx],[1,tmp14],[xxx]
        punpcklwd       %mm3,%mm0                       #wsptr[0,tmp10],[1,tmp10],[xxx],[xxx]

        movq            8*7(%esi),%mm7          #wsptr[3,4],[3,5],[3,6],[3,7]
        punpckhwd       %mm3,%mm6                       #wsptr[0,tmp13],[1,tmp13],[xxx],[xxx]

        movq            8*4(%esi),%mm3          #wsptr[2,0],[2,1],[2,2],[2,3]
        punpckldq       %mm6,%mm0       #wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]

        punpcklwd       %mm5,%mm1                       #wsptr[0,tmp11],[1,tmp11],[xxx],[xxx]
        movq            %mm3,%mm4

        movq            8*6(%esi),%mm6          #wsptr[3,0],[3,1],[3,2],[3,3]
        punpckhwd       %mm5,%mm2                       #wsptr[0,tmp14],[1,tmp14],[xxx],[xxx]

        movq            8*5(%esi),%mm5          #wsptr[2,4],[2,5],[2,6],[2,7]
        punpckldq       %mm2,%mm1       #wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


        paddw           %mm5,%mm3                       #wsptr[2,tmp10],[xxx],[2,tmp13],[xxx]
        movq            %mm6,%mm2

        psubw           %mm5,%mm4                       #wsptr[2,tmp11],[xxx],[2,tmp14],[xxx]
        paddw           %mm7,%mm6                       #wsptr[3,tmp10],[xxx],[3,tmp13],[xxx]

        movq            %mm3,%mm5
        punpcklwd       %mm6,%mm3                       #wsptr[2,tmp10],[3,tmp10],[xxx],[xxx]

        psubw           %mm7,%mm2                       #wsptr[3,tmp11],[xxx],[3,tmp14],[xxx]
        punpckhwd       %mm6,%mm5                       #wsptr[2,tmp13],[3,tmp13],[xxx],[xxx]

        movq            %mm4,%mm7
        punpckldq       %mm5,%mm3       #wsptr[2,tmp10],[3,tmp10],[2,tmp13],[3,tmp13]

        punpcklwd       %mm2,%mm4                       #wsptr[2,tmp11],[3,tmp11],[xxx],[xxx]

        punpckhwd       %mm2,%mm7                       #wsptr[2,tmp14],[3,tmp14],[xxx],[xxx]

        punpckldq       %mm7,%mm4       #wsptr[2,tmp11],[3,tmp11],[2,tmp14],[3,tmp14]
        movq            %mm1,%mm6

#       mm0 =   ;wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]
#       mm1 =   ;wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


        movq            %mm0,%mm2
        punpckhdq       %mm4,%mm6       #wsptr[0,tmp14],[1,tmp14],[2,tmp14],[3,tmp14]

        punpckldq       %mm4,%mm1       #wsptr[0,tmp11],[1,tmp11],[2,tmp11],[3,tmp11]
        psllw           $2,%mm6

        pmulhw          fix_141,%mm6
        punpckldq       %mm3,%mm0       #wsptr[0,tmp10],[1,tmp10],[2,tmp10],[3,tmp10]

        punpckhdq       %mm3,%mm2       #wsptr[0,tmp13],[1,tmp13],[2,tmp13],[3,tmp13]
        movq            %mm0,%mm7

#    tmp0 = tmp10 + tmp13;
#    tmp3 = tmp10 - tmp13;
        paddw           %mm2,%mm0       #[0,tmp0],[1,tmp0],[2,tmp0],[3,tmp0]
        psubw           %mm2,%mm7       #[0,tmp3],[1,tmp3],[2,tmp3],[3,tmp3]

#    tmp12 = MULTIPLY(tmp14, FIX_1_414213562) - tmp13;
        psubw           %mm2,%mm6       #wsptr[0,tmp12],[1,tmp12],[2,tmp12],[3,tmp12]
#    tmp1 = tmp11 + tmp12;
#    tmp2 = tmp11 - tmp12;
        movq            %mm1,%mm5



    # Odd part

#    z13 = (DCTELEM) wsptr[5] + (DCTELEM) wsptr[3];
#    z10 = (DCTELEM) wsptr[5] - (DCTELEM) wsptr[3];
#    z11 = (DCTELEM) wsptr[1] + (DCTELEM) wsptr[7];
#    z12 = (DCTELEM) wsptr[1] - (DCTELEM) wsptr[7];
        movq            8*0(%esi),%mm3          #wsptr[0,0],[0,1],[0,2],[0,3]
        paddw           %mm6,%mm1       #[0,tmp1],[1,tmp1],[2,tmp1],[3,tmp1]

        movq            8*1(%esi),%mm4          #wsptr[0,4],[0,5],[0,6],[0,7]
        psubw           %mm6,%mm5       #[0,tmp2],[1,tmp2],[2,tmp2],[3,tmp2]

        movq            %mm3,%mm6
        punpckldq       %mm4,%mm3                       #wsptr[0,0],[0,1],[0,4],[0,5]

        punpckhdq       %mm6,%mm4                       #wsptr[0,6],[0,7],[0,2],[0,3]
        movq            %mm3,%mm2

#Save tmp0 and tmp1 in wsptr
        movq            %mm0,8*0(%esi)          #save tmp0
        paddw           %mm4,%mm2                       #wsptr[xxx],[0,z11],[xxx],[0,z13]


#Continue with z10 --- z13
        movq            8*2(%esi),%mm6          #wsptr[1,0],[1,1],[1,2],[1,3]
        psubw           %mm4,%mm3                       #wsptr[xxx],[0,z12],[xxx],[0,z10]

        movq            8*3(%esi),%mm0          #wsptr[1,4],[1,5],[1,6],[1,7]
        movq            %mm6,%mm4

        movq            %mm1,8*1(%esi)          #save tmp1
        punpckldq       %mm0,%mm6                       #wsptr[1,0],[1,1],[1,4],[1,5]

        punpckhdq       %mm4,%mm0                       #wsptr[1,6],[1,7],[1,2],[1,3]
        movq            %mm6,%mm1

#Save tmp2 and tmp3 in wsptr
        paddw           %mm0,%mm6               #wsptr[xxx],[1,z11],[xxx],[1,z13]
        movq            %mm2,%mm4

#Continue with z10 --- z13
        movq            %mm5,8*2(%esi)          #save tmp2
        punpcklwd       %mm6,%mm2               #wsptr[xxx],[xxx],[0,z11],[1,z11]

        psubw           %mm0,%mm1               #wsptr[xxx],[1,z12],[xxx],[1,z10]
        punpckhwd       %mm6,%mm4               #wsptr[xxx],[xxx],[0,z13],[1,z13]

        movq            %mm3,%mm0
        punpcklwd       %mm1,%mm3               #wsptr[xxx],[xxx],[0,z12],[1,z12]

        movq            %mm7,8*3(%esi)          #save tmp3
        punpckhwd       %mm1,%mm0               #wsptr[xxx],[xxx],[0,z10],[1,z10]

        movq            8*4(%esi),%mm6          #wsptr[2,0],[2,1],[2,2],[2,3]
        punpckhdq       %mm2,%mm0               #wsptr[0,z10],[1,z10],[0,z11],[1,z11]

        movq            8*5(%esi),%mm7          #wsptr[2,4],[2,5],[2,6],[2,7]
        punpckhdq       %mm4,%mm3               #wsptr[0,z12],[1,z12],[0,z13],[1,z13]

        movq            8*6(%esi),%mm1          #wsptr[3,0],[3,1],[3,2],[3,3]
        movq            %mm6,%mm4

        punpckldq       %mm7,%mm6                       #wsptr[2,0],[2,1],[2,4],[2,5]
        movq            %mm1,%mm5

        punpckhdq       %mm4,%mm7                       #wsptr[2,6],[2,7],[2,2],[2,3]
        movq            %mm6,%mm2

        movq            8*7(%esi),%mm4          #wsptr[3,4],[3,5],[3,6],[3,7]
        paddw           %mm7,%mm6               #wsptr[xxx],[2,z11],[xxx],[2,z13]

        psubw           %mm7,%mm2               #wsptr[xxx],[2,z12],[xxx],[2,z10]
        punpckldq       %mm4,%mm1                       #wsptr[3,0],[3,1],[3,4],[3,5]

        punpckhdq       %mm5,%mm4                       #wsptr[3,6],[3,7],[3,2],[3,3]
        movq            %mm1,%mm7

        paddw           %mm4,%mm1               #wsptr[xxx],[3,z11],[xxx],[3,z13]
        psubw           %mm4,%mm7               #wsptr[xxx],[3,z12],[xxx],[3,z10]

        movq            %mm6,%mm5
        punpcklwd       %mm1,%mm6               #wsptr[xxx],[xxx],[2,z11],[3,z11]

        punpckhwd       %mm1,%mm5               #wsptr[xxx],[xxx],[2,z13],[3,z13]
        movq            %mm2,%mm4

        punpcklwd       %mm7,%mm2               #wsptr[xxx],[xxx],[2,z12],[3,z12]

        punpckhwd       %mm7,%mm4               #wsptr[xxx],[xxx],[2,z10],[3,z10]

        punpckhdq       %mm6,%mm4               #wsptr[2,z10],[3,z10],[2,z11],[3,z11]

        punpckhdq       %mm5,%mm2               #wsptr[2,z12],[3,z12],[2,z13],[3,z13]
        movq            %mm0,%mm5

        punpckldq       %mm4,%mm0               #wsptr[0,z10],[1,z10],[2,z10],[3,z10]

        punpckhdq       %mm4,%mm5               #wsptr[0,z11],[1,z11],[2,z11],[3,z11]
        movq            %mm3,%mm4

        punpckhdq       %mm2,%mm4               #wsptr[0,z13],[1,z13],[2,z13],[3,z13]
        movq            %mm5,%mm1

        punpckldq       %mm2,%mm3               #wsptr[0,z12],[1,z12],[2,z12],[3,z12]
#    tmp7 = z11 + z13;          /* phase 5 */
#    tmp8 = z11 - z13;          /* phase 5 */
        psubw           %mm4,%mm1               #tmp8

        paddw           %mm4,%mm5               #tmp7
#    tmp21 = MULTIPLY(tmp8, FIX_1_414213562); /* 2*c4 */
        psllw           $2,%mm1

        psllw           $2,%mm0

        pmulhw          fix_141,%mm1           #tmp21
#    tmp20 = MULTIPLY(z12, (FIX_1_082392200- FIX_1_847759065))  /* 2*(c2-c6) */
#                       + MULTIPLY(z10, - FIX_1_847759065); /* 2*c2 */
        psllw           $2,%mm3
        movq            %mm0,%mm7

        pmulhw          fix_n184,%mm7
        movq            %mm3,%mm6

        movq            8*0(%esi),%mm2  #tmp0,final1

        pmulhw          fix_108n184,%mm6
#        tmp22 = MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) /* -2*(c2+c6) */
#                       + MULTIPLY(z12, FIX_1_847759065); /* 2*c2 */
        movq            %mm2,%mm4               #final1

        pmulhw          fix_184n261,%mm0
        paddw           %mm5,%mm2               #tmp0+tmp7,final1

        pmulhw          fix_184,%mm3
        psubw           %mm5,%mm4               #tmp0-tmp7,final1

#    tmp6 = tmp22 - tmp7;       /* phase 2 */
        psraw           $5,%mm2                 #outptr[0,0],[1,0],[2,0],[3,0],final1

        paddsw          const_0x0080,%mm2              #final1
        paddw           %mm6,%mm7                       #tmp20
        psraw           $5,%mm4                 #outptr[0,7],[1,7],[2,7],[3,7],final1

        paddsw          const_0x0080,%mm4              #final1
        paddw           %mm0,%mm3                       #tmp22

#    tmp5 = tmp21 - tmp6;
        psubw           %mm5,%mm3               #tmp6

#    tmp4 = tmp20 + tmp5;
        movq            8*1(%esi),%mm0          #tmp1,final2
        psubw           %mm3,%mm1               #tmp5

        movq            %mm0,%mm6                       #final2
        paddw           %mm3,%mm0               #tmp1+tmp6,final2

    # Final output stage: scale down by a factor of 8 and range-limit


#    outptr[0] = range_limit[IDESCALE(tmp0 + tmp7, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[7] = range_limit[IDESCALE(tmp0 - tmp7, PASS1_BITS+3)
#                           & RANGE_MASK];      final1


#    outptr[1] = range_limit[IDESCALE(tmp1 + tmp6, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[6] = range_limit[IDESCALE(tmp1 - tmp6, PASS1_BITS+3)
#                           & RANGE_MASK];      final2
        psubw           %mm3,%mm6               #tmp1-tmp6,final2
        psraw           $5,%mm0                 #outptr[0,1],[1,1],[2,1],[3,1]

        paddsw          const_0x0080,%mm0
        psraw           $5,%mm6                 #outptr[0,6],[1,6],[2,6],[3,6]

        paddsw          const_0x0080,%mm6                      #need to check this value
        packuswb        %mm4,%mm0       #out[0,1],[1,1],[2,1],[3,1],[0,7],[1,7],[2,7],[3,7]

        movq            8*2(%esi),%mm5          #tmp2,final3
        packuswb        %mm6,%mm2       #out[0,0],[1,0],[2,0],[3,0],[0,6],[1,6],[2,6],[3,6]

#    outptr[2] = range_limit[IDESCALE(tmp2 + tmp5, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[5] = range_limit[IDESCALE(tmp2 - tmp5, PASS1_BITS+3)
#                           & RANGE_MASK];      final3
        paddw           %mm1,%mm7               #tmp4
        movq            %mm5,%mm3

        paddw           %mm1,%mm5               #tmp2+tmp5
        psubw           %mm1,%mm3               #tmp2-tmp5

        psraw           $5,%mm5                 #outptr[0,2],[1,2],[2,2],[3,2]

        paddsw          const_0x0080,%mm5
        movq            8*3(%esi),%mm4          #tmp3,final4
        psraw           $5,%mm3                 #outptr[0,5],[1,5],[2,5],[3,5]

        paddsw          const_0x0080,%mm3


#    outptr[4] = range_limit[IDESCALE(tmp3 + tmp4, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[3] = range_limit[IDESCALE(tmp3 - tmp4, PASS1_BITS+3)
#                           & RANGE_MASK];      final4
        movq            %mm4,%mm6
        paddw           %mm7,%mm4               #tmp3+tmp4

        psubw           %mm7,%mm6               #tmp3-tmp4
        psraw           $5,%mm4                 #outptr[0,4],[1,4],[2,4],[3,4]
        movl                    (%eax),%ecx

        paddsw          const_0x0080,%mm4
        psraw           $5,%mm6                 #outptr[0,3],[1,3],[2,3],[3,3]

        paddsw          const_0x0080,%mm6
        packuswb        %mm4,%mm5       #out[0,2],[1,2],[2,2],[3,2],[0,4],[1,4],[2,4],[3,4]

        packuswb        %mm3,%mm6       #out[0,3],[1,3],[2,3],[3,3],[0,5],[1,5],[2,5],[3,5]
        movq            %mm2,%mm4

        movq            %mm5,%mm7
        punpcklbw       %mm0,%mm2       #out[0,0],[0,1],[1,0],[1,1],[2,0],[2,1],[3,0],[3,1]

        punpckhbw       %mm0,%mm4       #out[0,6],[0,7],[1,6],[1,7],[2,6],[2,7],[3,6],[3,7]
        movq            %mm2,%mm1

        punpcklbw       %mm6,%mm5       #out[0,2],[0,3],[1,2],[1,3],[2,2],[2,3],[3,2],[3,3]
        addl                    $4,%eax

        punpckhbw       %mm6,%mm7       #out[0,4],[0,5],[1,4],[1,5],[2,4],[2,5],[3,4],[3,5]

        punpcklwd       %mm5,%mm2       #out[0,0],[0,1],[0,2],[0,3],[1,0],[1,1],[1,2],[1,3]
        addl                    20+16(%esp),%ecx

        movq            %mm7,%mm6
        punpckhwd       %mm5,%mm1       #out[2,0],[2,1],[2,2],[2,3],[3,0],[3,1],[3,2],[3,3]

        movq            %mm2,%mm0
        punpcklwd       %mm4,%mm6       #out[0,4],[0,5],[0,6],[0,7],[1,4],[1,5],[1,6],[1,7]

        movl                    (%eax),%ebx
        punpckldq       %mm6,%mm2       #out[0,0],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7]

        addl                    $4,%eax
        movq            %mm1,%mm3

        addl                    20+16(%esp),%ebx
        punpckhwd       %mm4,%mm7       #out[2,4],[2,5],[2,6],[2,7],[3,4],[3,5],[3,6],[3,7]

        movq            %mm2,(%ecx)
        punpckhdq       %mm6,%mm0       #out[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],[1,6],[1,7]

        movl                    (%eax),%ecx
        addl                    $4,%eax
        addl                    20+16(%esp),%ecx

        movq            %mm0,(%ebx)
        punpckldq       %mm7,%mm1       #out[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6],[2,7]

        movl                    (%eax),%ebx

        addl                    20+16(%esp),%ebx
        punpckhdq       %mm7,%mm3       #out[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7]
        movq            %mm1,(%ecx)


        movq            %mm3,(%ebx)



#/*******************************************************************/


        addl                    $64,%esi
        addl                    $4,%eax

#/*******************************************************************/

#    tmp10 = ((DCTELEM) wsptr[0] + (DCTELEM) wsptr[4]);
#    tmp13 = ((DCTELEM) wsptr[2] + (DCTELEM) wsptr[6]);
#    tmp11 = ((DCTELEM) wsptr[0] - (DCTELEM) wsptr[4]);
#    tmp14 = ((DCTELEM) wsptr[2] - (DCTELEM) wsptr[6]);
        movq            8*0(%esi),%mm0          #wsptr[0,0],[0,1],[0,2],[0,3]

        movq            8*1(%esi),%mm1          #wsptr[0,4],[0,5],[0,6],[0,7]
        movq            %mm0,%mm2

        movq            8*2(%esi),%mm3          #wsptr[1,0],[1,1],[1,2],[1,3]
        paddw           %mm1,%mm0                       #wsptr[0,tmp10],[xxx],[0,tmp13],[xxx]

        movq            8*3(%esi),%mm4          #wsptr[1,4],[1,5],[1,6],[1,7]
        psubw           %mm1,%mm2                       #wsptr[0,tmp11],[xxx],[0,tmp14],[xxx]

        movq            %mm0,%mm6
        movq            %mm3,%mm5

        paddw           %mm4,%mm3                       #wsptr[1,tmp10],[xxx],[1,tmp13],[xxx]
        movq            %mm2,%mm1

        psubw           %mm4,%mm5                       #wsptr[1,tmp11],[xxx],[1,tmp14],[xxx]
        punpcklwd       %mm3,%mm0                       #wsptr[0,tmp10],[1,tmp10],[xxx],[xxx]

        movq            8*7(%esi),%mm7          #wsptr[3,4],[3,5],[3,6],[3,7]
        punpckhwd       %mm3,%mm6                       #wsptr[0,tmp13],[1,tmp13],[xxx],[xxx]

        movq            8*4(%esi),%mm3          #wsptr[2,0],[2,1],[2,2],[2,3]
        punpckldq       %mm6,%mm0       #wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]

        punpcklwd       %mm5,%mm1                       #wsptr[0,tmp11],[1,tmp11],[xxx],[xxx]
        movq            %mm3,%mm4

        movq            8*6(%esi),%mm6          #wsptr[3,0],[3,1],[3,2],[3,3]
        punpckhwd       %mm5,%mm2                       #wsptr[0,tmp14],[1,tmp14],[xxx],[xxx]

        movq            8*5(%esi),%mm5          #wsptr[2,4],[2,5],[2,6],[2,7]
        punpckldq       %mm2,%mm1       #wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


        paddw           %mm5,%mm3                       #wsptr[2,tmp10],[xxx],[2,tmp13],[xxx]
        movq            %mm6,%mm2

        psubw           %mm5,%mm4                       #wsptr[2,tmp11],[xxx],[2,tmp14],[xxx]
        paddw           %mm7,%mm6                       #wsptr[3,tmp10],[xxx],[3,tmp13],[xxx]

        movq            %mm3,%mm5
        punpcklwd       %mm6,%mm3                       #wsptr[2,tmp10],[3,tmp10],[xxx],[xxx]

        psubw           %mm7,%mm2                       #wsptr[3,tmp11],[xxx],[3,tmp14],[xxx]
        punpckhwd       %mm6,%mm5                       #wsptr[2,tmp13],[3,tmp13],[xxx],[xxx]

        movq            %mm4,%mm7
        punpckldq       %mm5,%mm3       #wsptr[2,tmp10],[3,tmp10],[2,tmp13],[3,tmp13]

        punpcklwd       %mm2,%mm4                       #wsptr[2,tmp11],[3,tmp11],[xxx],[xxx]

        punpckhwd       %mm2,%mm7                       #wsptr[2,tmp14],[3,tmp14],[xxx],[xxx]

        punpckldq       %mm7,%mm4       #wsptr[2,tmp11],[3,tmp11],[2,tmp14],[3,tmp14]
        movq            %mm1,%mm6

#       mm0 =   ;wsptr[0,tmp10],[1,tmp10],[0,tmp13],[1,tmp13]
#       mm1 =   ;wsptr[0,tmp11],[1,tmp11],[0,tmp14],[1,tmp14]


        movq            %mm0,%mm2
        punpckhdq       %mm4,%mm6       #wsptr[0,tmp14],[1,tmp14],[2,tmp14],[3,tmp14]

        punpckldq       %mm4,%mm1       #wsptr[0,tmp11],[1,tmp11],[2,tmp11],[3,tmp11]
        psllw           $2,%mm6

        pmulhw          fix_141,%mm6
        punpckldq       %mm3,%mm0       #wsptr[0,tmp10],[1,tmp10],[2,tmp10],[3,tmp10]

        punpckhdq       %mm3,%mm2       #wsptr[0,tmp13],[1,tmp13],[2,tmp13],[3,tmp13]
        movq            %mm0,%mm7

#    tmp0 = tmp10 + tmp13;
#    tmp3 = tmp10 - tmp13;
        paddw           %mm2,%mm0       #[0,tmp0],[1,tmp0],[2,tmp0],[3,tmp0]
        psubw           %mm2,%mm7       #[0,tmp3],[1,tmp3],[2,tmp3],[3,tmp3]

#    tmp12 = MULTIPLY(tmp14, FIX_1_414213562) - tmp13;
        psubw           %mm2,%mm6       #wsptr[0,tmp12],[1,tmp12],[2,tmp12],[3,tmp12]
#    tmp1 = tmp11 + tmp12;
#    tmp2 = tmp11 - tmp12;
        movq            %mm1,%mm5



    # Odd part

#    z13 = (DCTELEM) wsptr[5] + (DCTELEM) wsptr[3];
#    z10 = (DCTELEM) wsptr[5] - (DCTELEM) wsptr[3];
#    z11 = (DCTELEM) wsptr[1] + (DCTELEM) wsptr[7];
#    z12 = (DCTELEM) wsptr[1] - (DCTELEM) wsptr[7];
        movq            8*0(%esi),%mm3          #wsptr[0,0],[0,1],[0,2],[0,3]
        paddw           %mm6,%mm1       #[0,tmp1],[1,tmp1],[2,tmp1],[3,tmp1]

        movq            8*1(%esi),%mm4          #wsptr[0,4],[0,5],[0,6],[0,7]
        psubw           %mm6,%mm5       #[0,tmp2],[1,tmp2],[2,tmp2],[3,tmp2]

        movq            %mm3,%mm6
        punpckldq       %mm4,%mm3                       #wsptr[0,0],[0,1],[0,4],[0,5]

        punpckhdq       %mm6,%mm4                       #wsptr[0,6],[0,7],[0,2],[0,3]
        movq            %mm3,%mm2

#Save tmp0 and tmp1 in wsptr
        movq            %mm0,8*0(%esi)          #save tmp0
        paddw           %mm4,%mm2                       #wsptr[xxx],[0,z11],[xxx],[0,z13]


#Continue with z10 --- z13
        movq            8*2(%esi),%mm6          #wsptr[1,0],[1,1],[1,2],[1,3]
        psubw           %mm4,%mm3                       #wsptr[xxx],[0,z12],[xxx],[0,z10]

        movq            8*3(%esi),%mm0          #wsptr[1,4],[1,5],[1,6],[1,7]
        movq            %mm6,%mm4

        movq            %mm1,8*1(%esi)          #save tmp1
        punpckldq       %mm0,%mm6                       #wsptr[1,0],[1,1],[1,4],[1,5]

        punpckhdq       %mm4,%mm0                       #wsptr[1,6],[1,7],[1,2],[1,3]
        movq            %mm6,%mm1

#Save tmp2 and tmp3 in wsptr
        paddw           %mm0,%mm6               #wsptr[xxx],[1,z11],[xxx],[1,z13]
        movq            %mm2,%mm4

#Continue with z10 --- z13
        movq            %mm5,8*2(%esi)          #save tmp2
        punpcklwd       %mm6,%mm2               #wsptr[xxx],[xxx],[0,z11],[1,z11]

        psubw           %mm0,%mm1               #wsptr[xxx],[1,z12],[xxx],[1,z10]
        punpckhwd       %mm6,%mm4               #wsptr[xxx],[xxx],[0,z13],[1,z13]

        movq            %mm3,%mm0
        punpcklwd       %mm1,%mm3               #wsptr[xxx],[xxx],[0,z12],[1,z12]

        movq            %mm7,8*3(%esi)          #save tmp3
        punpckhwd       %mm1,%mm0               #wsptr[xxx],[xxx],[0,z10],[1,z10]

        movq            8*4(%esi),%mm6          #wsptr[2,0],[2,1],[2,2],[2,3]
        punpckhdq       %mm2,%mm0               #wsptr[0,z10],[1,z10],[0,z11],[1,z11]

        movq            8*5(%esi),%mm7          #wsptr[2,4],[2,5],[2,6],[2,7]
        punpckhdq       %mm4,%mm3               #wsptr[0,z12],[1,z12],[0,z13],[1,z13]

        movq            8*6(%esi),%mm1          #wsptr[3,0],[3,1],[3,2],[3,3]
        movq            %mm6,%mm4

        punpckldq       %mm7,%mm6                       #wsptr[2,0],[2,1],[2,4],[2,5]
        movq            %mm1,%mm5

        punpckhdq       %mm4,%mm7                       #wsptr[2,6],[2,7],[2,2],[2,3]
        movq            %mm6,%mm2

        movq            8*7(%esi),%mm4          #wsptr[3,4],[3,5],[3,6],[3,7]
        paddw           %mm7,%mm6               #wsptr[xxx],[2,z11],[xxx],[2,z13]

        psubw           %mm7,%mm2               #wsptr[xxx],[2,z12],[xxx],[2,z10]
        punpckldq       %mm4,%mm1                       #wsptr[3,0],[3,1],[3,4],[3,5]

        punpckhdq       %mm5,%mm4                       #wsptr[3,6],[3,7],[3,2],[3,3]
        movq            %mm1,%mm7

        paddw           %mm4,%mm1               #wsptr[xxx],[3,z11],[xxx],[3,z13]
        psubw           %mm4,%mm7               #wsptr[xxx],[3,z12],[xxx],[3,z10]

        movq            %mm6,%mm5
        punpcklwd       %mm1,%mm6               #wsptr[xxx],[xxx],[2,z11],[3,z11]

        punpckhwd       %mm1,%mm5               #wsptr[xxx],[xxx],[2,z13],[3,z13]
        movq            %mm2,%mm4

        punpcklwd       %mm7,%mm2               #wsptr[xxx],[xxx],[2,z12],[3,z12]

        punpckhwd       %mm7,%mm4               #wsptr[xxx],[xxx],[2,z10],[3,z10]

        punpckhdq       %mm6,%mm4               #wsptr[2,z10],[3,z10],[2,z11],[3,z11]

        punpckhdq       %mm5,%mm2               #wsptr[2,z12],[3,z12],[2,z13],[3,z13]
        movq            %mm0,%mm5

        punpckldq       %mm4,%mm0               #wsptr[0,z10],[1,z10],[2,z10],[3,z10]

        punpckhdq       %mm4,%mm5               #wsptr[0,z11],[1,z11],[2,z11],[3,z11]
        movq            %mm3,%mm4

        punpckhdq       %mm2,%mm4               #wsptr[0,z13],[1,z13],[2,z13],[3,z13]
        movq            %mm5,%mm1

        punpckldq       %mm2,%mm3               #wsptr[0,z12],[1,z12],[2,z12],[3,z12]
#    tmp7 = z11 + z13;          /* phase 5 */
#    tmp8 = z11 - z13;          /* phase 5 */
        psubw           %mm4,%mm1               #tmp8

        paddw           %mm4,%mm5               #tmp7
#    tmp21 = MULTIPLY(tmp8, FIX_1_414213562); /* 2*c4 */
        psllw           $2,%mm1

        psllw           $2,%mm0

        pmulhw          fix_141,%mm1           #tmp21
#    tmp20 = MULTIPLY(z12, (FIX_1_082392200- FIX_1_847759065))  /* 2*(c2-c6) */
#                       + MULTIPLY(z10, - FIX_1_847759065); /* 2*c2 */
        psllw           $2,%mm3
        movq            %mm0,%mm7

        pmulhw          fix_n184,%mm7
        movq            %mm3,%mm6

        movq            8*0(%esi),%mm2  #tmp0,final1

        pmulhw          fix_108n184,%mm6
#        tmp22 = MULTIPLY(z10,(FIX_1_847759065 - FIX_2_613125930)) /* -2*(c2+c6) */
#                       + MULTIPLY(z12, FIX_1_847759065); /* 2*c2 */
        movq            %mm2,%mm4               #final1

        pmulhw          fix_184n261,%mm0
        paddw           %mm5,%mm2               #tmp0+tmp7,final1

        pmulhw          fix_184,%mm3
        psubw           %mm5,%mm4               #tmp0-tmp7,final1

#    tmp6 = tmp22 - tmp7;       /* phase 2 */
        psraw           $5,%mm2                 #outptr[0,0],[1,0],[2,0],[3,0],final1

        paddsw          const_0x0080,%mm2              #final1
        paddw           %mm6,%mm7                       #tmp20
        psraw           $5,%mm4                 #outptr[0,7],[1,7],[2,7],[3,7],final1

        paddsw          const_0x0080,%mm4              #final1
        paddw           %mm0,%mm3                       #tmp22

#    tmp5 = tmp21 - tmp6;
        psubw           %mm5,%mm3               #tmp6

#    tmp4 = tmp20 + tmp5;
        movq            8*1(%esi),%mm0          #tmp1,final2
        psubw           %mm3,%mm1               #tmp5

        movq            %mm0,%mm6                       #final2
        paddw           %mm3,%mm0               #tmp1+tmp6,final2

    # Final output stage: scale down by a factor of 8 and range-limit


#    outptr[0] = range_limit[IDESCALE(tmp0 + tmp7, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[7] = range_limit[IDESCALE(tmp0 - tmp7, PASS1_BITS+3)
#                           & RANGE_MASK];      final1


#    outptr[1] = range_limit[IDESCALE(tmp1 + tmp6, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[6] = range_limit[IDESCALE(tmp1 - tmp6, PASS1_BITS+3)
#                           & RANGE_MASK];      final2
        psubw           %mm3,%mm6               #tmp1-tmp6,final2
        psraw           $5,%mm0                 #outptr[0,1],[1,1],[2,1],[3,1]

        paddsw          const_0x0080,%mm0
        psraw           $5,%mm6                 #outptr[0,6],[1,6],[2,6],[3,6]

        paddsw          const_0x0080,%mm6                      #need to check this value
        packuswb        %mm4,%mm0       #out[0,1],[1,1],[2,1],[3,1],[0,7],[1,7],[2,7],[3,7]

        movq            8*2(%esi),%mm5          #tmp2,final3
        packuswb        %mm6,%mm2       #out[0,0],[1,0],[2,0],[3,0],[0,6],[1,6],[2,6],[3,6]

#    outptr[2] = range_limit[IDESCALE(tmp2 + tmp5, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[5] = range_limit[IDESCALE(tmp2 - tmp5, PASS1_BITS+3)
#                           & RANGE_MASK];      final3
        paddw           %mm1,%mm7               #tmp4
        movq            %mm5,%mm3

        paddw           %mm1,%mm5               #tmp2+tmp5
        psubw           %mm1,%mm3               #tmp2-tmp5

        psraw           $5,%mm5                 #outptr[0,2],[1,2],[2,2],[3,2]

        paddsw          const_0x0080,%mm5
        movq            8*3(%esi),%mm4          #tmp3,final4
        psraw           $5,%mm3                 #outptr[0,5],[1,5],[2,5],[3,5]

        paddsw          const_0x0080,%mm3


#    outptr[4] = range_limit[IDESCALE(tmp3 + tmp4, PASS1_BITS+3)
#                           & RANGE_MASK];
#    outptr[3] = range_limit[IDESCALE(tmp3 - tmp4, PASS1_BITS+3)
#                           & RANGE_MASK];      final4
        movq            %mm4,%mm6
        paddw           %mm7,%mm4               #tmp3+tmp4

        psubw           %mm7,%mm6               #tmp3-tmp4
        psraw           $5,%mm4                 #outptr[0,4],[1,4],[2,4],[3,4]
        movl                    (%eax),%ecx

        paddsw          const_0x0080,%mm4
        psraw           $5,%mm6                 #outptr[0,3],[1,3],[2,3],[3,3]

        paddsw          const_0x0080,%mm6
        packuswb        %mm4,%mm5       #out[0,2],[1,2],[2,2],[3,2],[0,4],[1,4],[2,4],[3,4]

        packuswb        %mm3,%mm6       #out[0,3],[1,3],[2,3],[3,3],[0,5],[1,5],[2,5],[3,5]
        movq            %mm2,%mm4

        movq            %mm5,%mm7
        punpcklbw       %mm0,%mm2       #out[0,0],[0,1],[1,0],[1,1],[2,0],[2,1],[3,0],[3,1]

        punpckhbw       %mm0,%mm4       #out[0,6],[0,7],[1,6],[1,7],[2,6],[2,7],[3,6],[3,7]
        movq            %mm2,%mm1

        punpcklbw       %mm6,%mm5       #out[0,2],[0,3],[1,2],[1,3],[2,2],[2,3],[3,2],[3,3]
        addl                    $4,%eax

        punpckhbw       %mm6,%mm7       #out[0,4],[0,5],[1,4],[1,5],[2,4],[2,5],[3,4],[3,5]

        punpcklwd       %mm5,%mm2       #out[0,0],[0,1],[0,2],[0,3],[1,0],[1,1],[1,2],[1,3]
        addl                    20+16(%esp),%ecx

        movq            %mm7,%mm6
        punpckhwd       %mm5,%mm1       #out[2,0],[2,1],[2,2],[2,3],[3,0],[3,1],[3,2],[3,3]

        movq            %mm2,%mm0
        punpcklwd       %mm4,%mm6       #out[0,4],[0,5],[0,6],[0,7],[1,4],[1,5],[1,6],[1,7]

        movl                    (%eax),%ebx
        punpckldq       %mm6,%mm2       #out[0,0],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7]

        addl                    $4,%eax
        movq            %mm1,%mm3

        addl                    20+16(%esp),%ebx
        punpckhwd       %mm4,%mm7       #out[2,4],[2,5],[2,6],[2,7],[3,4],[3,5],[3,6],[3,7]

        movq            %mm2,(%ecx)
        punpckhdq       %mm6,%mm0       #out[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],[1,6],[1,7]

        movl                    (%eax),%ecx
        addl                    $4,%eax
        addl                    20+16(%esp),%ecx

        movq            %mm0,(%ebx)
        punpckldq       %mm7,%mm1       #out[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6],[2,7]

        movl                    (%eax),%ebx

        addl                    20+16(%esp),%ebx
        punpckhdq       %mm7,%mm3       #out[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7]
        movq            %mm1,(%ecx)

        movq            %mm3,(%ebx)

        # pop mmx registers here!?
        popl            %ebx
        popl            %ecx
        popl            %eax
        popl            %edi
        popl            %esi
        ret

.align 8
jpeg_idct_ifast_deinit_asm: 
        emms
        ret

