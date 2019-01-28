#       ------------------------------------------------------------------------
#       ClanLib, the platform independent game SDK.
#
#       This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
#       version 2. See COPYING for details.
#
#       For a total list of contributers see CREDITS.
#
#       This file by Daniel Vogel 04/99
#       ------------------------------------------------------------------------

#BITS 32

.globl mixer_clip_asm

.text

## mixer_asm(int *src, short *dest, int size)
## --------------------------------------
##  4: int *src
##  8: short *dest
## 12: int size
## --------------------------------------

mixer_clip_asm: 
        pushl %esi                              # save used registers (6)
        pushl %edi
        pushl %eax
        pushl %ebx
        pushl %ecx
        pushl %edx

        movl 24+4(%esp),%esi                    # load registers
        movl 24+8(%esp),%edi
        movl 24+12(%esp),%ecx

        movl $32200,%ebx
        movl $-32200,%edx

mixer_clip_asm.loop: 
        movl (%esi),%eax
        sarl %eax
        addl $4,%esi

        cmpl %ebx,%eax
        jg mixer_clip_asm.above
        cmpl %edx,%eax
        jl mixer_clip_asm.below
        jmp mixer_clip_asm.clipped

mixer_clip_asm.above: 
        movl %ebx,%eax                          # 32200
        jmp mixer_clip_asm.clipped

mixer_clip_asm.below: 
        movl %edx,%eax                          # -32200

mixer_clip_asm.clipped: 

        movw %ax,(%edi)
        addl $2,%edi
        decl %ecx
        jz mixer_clip_asm.done
        jmp mixer_clip_asm.loop

mixer_clip_asm.done: 
        popl %edx                               # restore used registers
        popl %ecx
        popl %ebx
        popl %eax
        popl %edi
        popl %esi

        ret

