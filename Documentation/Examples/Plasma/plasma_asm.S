#       Daniel Vogel 99/08/30



.globl calculate_plasma

.text

## calculate_plasma(int *dest, int *s1, int *s2, int *s3, int w, int h)
## --------------------------------------
##  4: int *dest
##  8: int *s1
## 12: int *s2
## 16: int *s3
## 20: int w
## 24: int h
## --------------------------------------
.align 8
calculate_plasma: 
        pushl           %ebp                    # save used registers (7)
        pushl           %esi
        pushl           %edi
        pushl           %eax
        pushl           %ebx
        pushl           %ecx
        pushl           %edx

        movl            28+4(%esp),%edi         # dest
        movl            28+8(%esp),%esi         # s1
        movl            28+12(%esp),%ebp        # s2
        movl            28+16(%esp),%edx        # s3

        movl            28+24(%esp),%ebx        # h

calculate_plasma.loop_y: 
        movl            28+20(%esp),%ecx        # w

.align 4

calculate_plasma.loop_x: 
        movl            (%esi),%eax
        addl            (%ebp),%eax
        addl            (%edx),%eax

        addl            $4,%esi
        notb            %al                     # alpha == 255 == opaque
        addl            $4,%ebp
        addl            $4,%edx

        movl            %eax,(%edi)

        addl            $4,%edi

        decl            %ecx
        jz              calculate_plasma.loop_x_end
        jmp             calculate_plasma.loop_x

calculate_plasma.loop_x_end: 
        movl            28+20(%esp),%eax        # w
        shll            $2,%eax

        addl            %eax,%esi
        addl            %eax,%ebp
        addl            %eax,%edx

        decl            %ebx
        jz              calculate_plasma.end
        jmp             calculate_plasma.loop_y

calculate_plasma.end: 
        popl            %edx                    # restore used registers (7)
        popl            %ecx
        popl            %ebx
        popl            %eax
        popl            %edi
        popl            %esi
        popl            %ebp
        ret

