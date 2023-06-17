.globl prog2

#your full name: Riley Oest
#your LSU ID number: 899859039

prog2:
        # set up code
        pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        # body code

        movl 12(%ebp), %eax # %eax : j
        subl 8(%ebp), %eax # %eax : j - i
        addl $2, %eax # %eax : j - i + 2

        movl 16(%ebp), %edx # %edx : *k
        movl (%edx), %ecx # %ecx : k
        sall $2, (%edx) # *k = k*4
        sall $1, %ecx # temp = k*2
        addl %ecx, (%edx) # *k + temp = 6k

        movl 20(%ebp), %ebx # ebx = a
        movl $0, %esi # esi : i = 0
        movl $0, %edi # edi : count = 0
        loop:
                addl (%ebx, %esi, 4), %edi # count += 4i + a
                addl $1, %esi # i++
                cmpl $5, %esi # i < 5 ?
                jl loop 
        movl 24(%ebp), %esi # i = *l
        movl %edi, (%esi) # answer (l) = count

        # Finish code
        popl %ebx
        popl %ebp
        ret
        