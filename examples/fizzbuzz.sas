push32 100
popa32
push32 0
loop: 
    push32 0
    popb32
    dup32
    push32 3
    mod32 
    push32 0
    cmp32
    mk4
    jne end0
    push "fizz"
    push32 0
    store_str
    push32 0
    print_str 
    push32 1
    popb32
end0:
    dup32
    push32 5
    mod32
    push32 0
    cmp32
    mk4
    jne end1
    push "buzz"
    push32 0
    store_str
    push32 0
    print_str
    push32 1
    popb32
end1:
    push32 0
    pushb32
    cmp32
    mk4
    jne end2
    dup32
    printi32
end2:
    push '\n'
    putc
    dup32
    push32 1
    add32
    swap32
    pusha32
    cmp32
    mk4
    jne loop

