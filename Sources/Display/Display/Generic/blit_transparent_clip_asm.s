#       $Id: blit_transparent_clip_asm.s,v 1.1 2001/03/06 15:09:17 mbn Exp $
#
#       ------------------------------------------------------------------------
#       ClanLib, the platform independent game SDK.
#
#       This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
#       version 2. See COPYING for details.
#
#       For a total list of contributers see CREDITS.
#
#       This file by Magnus Norddahl 03/99
#       ------------------------------------------------------------------------
#
#       changed things a bit    Daniel Vogel (03/14/99)
#       changed things a bit    Mikko Tiihonen (04/09/99)


#BITS 32

.globl blit_transparent_clip_asm_16bpp
.globl blit_transparent_clip_asm_32bpp

.text

## blit_transparent_clip_asm_16bpp(uchar *src, ushort *dest, uint width)
## --------------------------------------
##  4: uchar *src
##  8: ushort *dest
## 12: uint width
## --------------------------------------
.align 8
blit_transparent_clip_asm_16bpp: 
        pushl %esi                              # save used registers (5)
        pushl %edi
        pushl %eax
        pushl %ecx
        pushl %edx

        movl 20+4(%esp),%esi                    # load registers
        movl 20+8(%esp),%edi
        movl 20+12(%esp),%edx

        andl %edx,%edx
        jle blit_transparent_clip_asm_16bpp.rle_done # just in case

        xorl %ecx,%ecx                          # should produce smaller opcode than mov ecx, 0
.align 4
blit_transparent_clip_asm_16bpp.cmd_copy: 
        movw (%esi),%cx
        subl %ecx,%edx                          # width -= rep
        jns blit_transparent_clip_asm_16bpp.do_copy # if (rep >= width)
        addl %edx,%ecx                          # rep = width

blit_transparent_clip_asm_16bpp.do_copy: 
        addl $2,%esi

        shrl %ecx
        rep
 movsl                              # copy all dwords

        jnc blit_transparent_clip_asm_16bpp.end_copy # carry == ecx & 1 before the shift

        movw (%esi),%ax
        movw %ax,(%edi)
        addl $2,%esi
        addl $2,%edi

blit_transparent_clip_asm_16bpp.end_copy: 
        andl %edx,%edx                          # if (width <= 0) jmp done
        jle blit_transparent_clip_asm_16bpp.rle_done

blit_transparent_clip_asm_16bpp.cmd_skip: 
        movw (%esi),%cx                         # read rep

        addl %ecx,%edi                          # do the skip
        addl $2,%esi
        addl %ecx,%edi                          # (x2 because dest is word)

        subl %ecx,%edx                          # width -= rep
        jns blit_transparent_clip_asm_16bpp.cmd_copy

blit_transparent_clip_asm_16bpp.rle_done: 
        popl %edx                               # restore used registers
        popl %ecx
        popl %eax
        popl %edi
        popl %esi

        ret


## blit_transparent_clip_asm_32bpp(uchar *src, uint *dest, uint width)
## --------------------------------------
##  4: uchar *src
##  8: uint *dest
## 12: uint width
## --------------------------------------
.align 8
blit_transparent_clip_asm_32bpp: 

        pushl %esi                              # save used registers (4)
        pushl %edi
        pushl %ecx
        pushl %edx

        movl 16+4(%esp),%esi            # load registers
        movl 16+8(%esp),%edi
        movl 16+12(%esp),%edx

        andl %edx,%edx
        jle blit_transparent_clip_asm_32bpp.rle_done # just in case

        xorl %ecx,%ecx                          # should produce smaller opcode than mov ecx, 0
.align 4
blit_transparent_clip_asm_32bpp.cmd_copy: 
        movw (%esi),%cx
        subl %ecx,%edx                          # width -= rep
        jns blit_transparent_clip_asm_32bpp.do_copy # if (rep >= width)
        addl %edx,%ecx                          # rep = width

blit_transparent_clip_asm_32bpp.do_copy: 
        addl $2,%esi
        rep
 movsl                              # copy all dwords

        andl %edx,%edx                          # if (width <= 0) jmp done
        jle blit_transparent_clip_asm_32bpp.rle_done

blit_transparent_clip_asm_32bpp.cmd_skip: 
        movw (%esi),%cx                         # read rep
        leal (%edi,%ecx,4),%edi                 # do the skip (x4 because dest is word)
        addl $2,%esi

        subl %ecx,%edx                          # width -= rep
        jns blit_transparent_clip_asm_32bpp.cmd_copy

blit_transparent_clip_asm_32bpp.rle_done: 

        popl %edx                               # restore used registers
        popl %ecx
        popl %edi
        popl %esi

        ret
