main:
push32 1000
popa32
push32 0
loop:
    push 0
    popb8
    dup32 
    push32 3
    mod32
    cmp32
    mk4
    jne end0
    push 'f'
    dup8
    popb8
    putc
end0:
    dup32
    push32 5
    mod32 
    push32 0
    cmp32 
    mk4
    jne end1
    push 'b'
    dup8
    popb8
    putc
end1:
    dup32
    pushb8
    dup8
    push 0
    cmp8
    mk4
    jne end2
    dup32 
    printi32
end2:
    push '\n'
    putc
    push32 1
    add32
    swap32
    pusha32
    cmp32
    jl start
popa32