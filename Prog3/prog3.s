.global check_cache
# Riley Oest 899859039
check_cache:
        pushl   %ebp
        pushl   %ebx
        movl    %esp,   %ebp

        movb 16(%ebp), %ah # %ah : tag 
        movb %ah, %ch # %ch : set index
        movb %ah, %cl # %cl : offset

        shrb $4, %ah # shift right by 4 ie: ah * 2^4
        andb $12, %ch #and ch by 00001100
        shrb $2, %ch # shift right by 2 ie: ch * 2^2
        andb $3, %cl # and cl by 00000011

        movzbl %ch, %ebx # move set index to 4 byte reg
        imull $6, %ebx # multiply by C, size of struct
        movl 12(%ebp), %edx # %edx = cache
        addl %ebx, %edx # edx = &cache

        movb 4(%edx), %al # %al = valid bit
        cmpb $0, %al # compare 0 to see valid bit
        je MISS 

        movb 5(%edx), %al # %al = tag bit
        cmpb %ah, %al # which one is the tag bit
        jne MISS

        movzbl %cl, %ebx # move offset to 4 byte reg
        movb (%edx, %ebx, 1), %al # return edx + ebx

        popl    %ebx
        popl    %ebp
        ret

        MISS:
                movb $0xFF, %al # return 0xFF
                popl %ebx
                popl %ebp
                ret