#include "vm.h"

void panic(char *str)
{
    if (!str)
    {
        exit(-1);
    }
    printf(RED "Panic: ");
    printf("%s", str);
    printf(RESET "\n");
    exit(-1);
}

void check_vm(vm_state *vm)
{
    if (!vm)
    {
        panic("VM is uninitialized.");
    }
}

void stack_underflow()
{
    panic("Stack Underflow.");
}

void check_stack(vm_state *vm, uint32_t num)
{
    if (vm->sp < num)
    {
        stack_underflow();
    }
}
void push(vm_state *vm, uint8_t value)
{
    check_vm(vm);
    vm->stack[vm->sp++] = value;
}

void push32(vm_state *vm, uint32_t value)
{
    uint32_t *stack = (uint32_t *)(vm->stack + vm->sp);
    stack[0] = value;
    vm->sp += sizeof(value);
}

uint32_t get32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, sizeof(uint32_t));
    uint32_t *stack = (uint32_t *)(vm->stack + (vm->sp - 4));
    return stack[0];
}

void add(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 2);
    vm->sp -= 1;
    uint8_t a = vm->stack[vm->sp];
    vm->sp -= 1;
    uint8_t b = vm->stack[vm->sp];
    push(vm, a + b);
}

void add32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, sizeof(int) * 2);
    uint32_t a = get32(vm);
    vm->sp -= sizeof(a);
    uint32_t b = get32(vm);
    vm->sp -= sizeof(a);
    push32(vm, a + b);
}
void sub(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 2);
    vm->sp -= 1;
    uint8_t a = vm->stack[vm->sp];
    vm->sp -= 1;
    uint8_t b = vm->stack[vm->sp];
    push(vm, a - b);
}

void sub32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, sizeof(int) * 2);
    uint32_t a = get32(vm);
    vm->sp -= sizeof(a);
    uint32_t b = get32(vm);
    vm->sp -= sizeof(a);
    push32(vm, a - b);
}

void putch(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 1);
    vm->sp -= 1;
    printf("%c", vm->stack[vm->sp]);
}

void stack_view(vm_state *vm)
{
    printf("STACK:\n");
    for (uint32_t i = 0; i < vm->sp; ++i)
    {
        printf("\t%d\n", vm->stack[i]);
    }
}

void popA8(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 1);
    vm->sp--;
    vm->A = (int32_t)vm->stack[vm->sp];
}
void popA32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 4);
    vm->A = get32(vm);
    vm->sp -= 4;
}
void pushA8(vm_state *vm)
{
    check_vm(vm);
    push(vm, (uint8_t)vm->A);
}
void dup(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 1);
    uint8_t d = vm->stack[vm->sp - 1];
    push(vm, d);
}
void swap(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 2);
    int x = vm->stack[vm->sp - 1];
    int y = vm->stack[vm->sp - 2];
    vm->stack[vm->sp - 1] = y;
    vm->stack[vm->sp - 2] = x;
}
void dup32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 4);
    uint32_t d = get32(vm);
    push32(vm, d);
}
void swap32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 8);
    uint32_t a = get32(vm);
    vm->sp -= 4;
    uint32_t b = get32(vm);
    vm->sp -= 4;
    push32(vm, a);
    push32(vm, b);
}

void mul(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 2);
    vm->sp -= 1;
    uint8_t a = vm->stack[vm->sp];
    vm->sp -= 1;
    uint8_t b = vm->stack[vm->sp];
    push(vm, a * b);
}

void mul32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, sizeof(uint32_t) * 2);
    uint32_t a = get32(vm);
    vm->sp -= 4;
    uint32_t b = get32(vm);
    vm->sp -= 4;
    push32(vm, a * b);
}

void mod(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 2);
    vm->sp -= 1;
    uint8_t a = vm->stack[vm->sp];
    vm->sp -= 1;
    uint8_t b = vm->stack[vm->sp];
    push(vm, a % b);
}

void mod32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, sizeof(uint32_t) * 2);
    uint32_t a = get32(vm);
    vm->sp -= 4;
    uint32_t b = get32(vm);
    vm->sp -= 4;
    push32(vm, a % b);
}

void div8(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 2);
    vm->sp -= 1;
    uint8_t a = vm->stack[vm->sp];
    vm->sp -= 1;
    uint8_t b = vm->stack[vm->sp];
    push(vm, a / b);
}

void div32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, sizeof(uint32_t) * 2);
    uint32_t a = get32(vm);
    vm->sp -= 4;
    uint32_t b = get32(vm);
    vm->sp -= 4;
    push32(vm, a / b);
}
void pushA32(vm_state *vm)
{
    check_vm(vm);
    push32(vm, (uint32_t)vm->A);
}

void popB32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 4);
    uint32_t a = get32(vm);
    vm->sp -= 4;
    vm->B = a;
}

void popB8(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 1);
    vm->sp -= 1;
    uint8_t a = vm->stack[vm->sp];
    vm->B = (uint32_t)a;
}

void pushB8(vm_state *vm)
{
    check_vm(vm);
    push(vm, (uint8_t)vm->B);
}

void pushB32(vm_state *vm)
{
    check_vm(vm);
    push32(vm, (uint32_t)vm->B);
}
void cmp32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, sizeof(uint32_t) * 2);
    uint32_t a = get32(vm);
    vm->sp -= 4;
    uint32_t b = get32(vm);
    vm->sp -= 4;
    if (a == b)
    {
        push(vm, 1);
        return;
    }
    if (a > b)
    {
        push(vm, 2);

        return;
    }
    if (a < b)
    {
        push(vm, 3);
        return;
    }
    push(vm, 0);
}

void cmp(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 2);
    vm->sp -= 1;
    uint8_t a = vm->stack[vm->sp];
    vm->sp -= 1;
    uint8_t b = vm->stack[vm->sp];
    if (a == b)
    {
        push(vm, 1);
        return;
    }
    if (a > b)
    {
        push(vm, 2);
        return;
    }
    push(vm, 0);
}

void jmpneq(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 8);
    uint32_t pc = get32(vm);
    vm->sp -= 4;
    uint32_t cnd = get32(vm);
    vm->sp -= 4;
    vm->pc = (cnd != 1) ? pc - 1 : vm->pc;
}

void jmp(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 4);
    uint32_t pc = get32(vm);
    vm->sp -= 4;
    vm->pc = pc - 1;
}

void jmpeq(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 8);
    uint32_t pc = get32(vm);
    vm->sp -= 4;
    uint32_t cnd = get32(vm);
    vm->sp -= 4;
    vm->pc = (cnd == 1) ? pc - 1 : vm->pc;
}

void jmpg(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 8);
    uint32_t pc = get32(vm);
    vm->sp -= 4;
    uint32_t cnd = get32(vm);
    vm->sp -= 4;
    vm->pc = (cnd == 2) ? pc - 1 : vm->pc;
}

void jmpl(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 8);
    uint32_t pc = get32(vm);
    vm->sp -= 4;
    uint32_t cnd = get32(vm);
    vm->sp -= 4;
    vm->pc = (cnd == 3) ? pc - 1 : vm->pc;
}

void vm_view(vm_state *vm)
{
    printf("VM STATE:\n");
    printf("Registers:\n");
    printf("\tA: %d\n", vm->A);
    printf("\tB: %d\n", vm->B);
    printf("\tC: %d\n", vm->C);
    printf("\tD: %d\n", vm->D);
    stack_view(vm);
}

void expand_mem(vm_state *vm, uint32_t new_size)
{
    check_vm(vm);
    if (vm->mem_size > new_size)
        return;

    vm->mem = realloc(vm->mem, new_size);
}

vm_state *create_vm(int stack_size, int mem_size)
{
    vm_state *vm = calloc(1, sizeof(vm_state));
    vm->mem = calloc(1, mem_size);
    vm->mem_size = mem_size;
    vm->stack = calloc(1, stack_size);
    vm->sp = 0;
    vm->stack_top = stack_size;
    return vm;
}

void cleanup_vm(vm_state *vm)
{
    free(vm->stack);
    free(vm->mem);
    free(vm);
}

void check_mem(vm_state *vm, uint32_t ptr)
{
    if (ptr > vm->mem_size)
    {
        panic("Memory overflow");
    }
}

void store(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 5);
    uint32_t addr = get32(vm);
    vm->sp -= 4;
    uint8_t data = vm->stack[vm->sp - 1];
    vm->sp -= 1;
    check_mem(vm, addr);
    vm->mem[addr] = data;
}

void store32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 8);
    uint32_t addr = get32(vm);
    vm->sp -= 4;
    uint32_t value = get32(vm);
    vm->sp -= 4;
    check_mem(vm, addr);
    uint32_t *ptr = (uint32_t *)(vm->mem + addr);
    ptr[0] = value;
}

void load(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 4);
    uint32_t addr = get32(vm);
    vm->sp -= 4;
    check_mem(vm, addr);
    push(vm, vm->mem[addr]);
}

void load32(vm_state *vm)
{
    check_vm(vm);
    check_stack(vm, 4);
    uint32_t addr = get32(vm);
    vm->sp -= 4;
    check_mem(vm, addr);
    uint32_t *ptr = (uint32_t *)(vm->mem + addr);
    uint32_t value = *ptr;
    push32(vm, value);
}

void execute(vm_state *vm, instruction_t instruction)
{
    switch (instruction.type)
    {
    case PUSH:
    {
        push(vm, (uint8_t)instruction.value);
    }
    break;
    case PUSH32:
    {
        push32(vm, instruction.value);
    }
    break;
    case POPA8:
    {
        popA8(vm);
    }
    break;
    case POPA32:
    {
        popA32(vm);
    }
    break;
    case SUB8:
    {
        sub(vm);
    }
    break;
    case ADD32:
    {
        add32(vm);
    }
    break;
    case ADD8:
    {
        add(vm);
    }
    break;
    case SUB32:
    {
        sub32(vm);
    }
    break;
    case MUL8:
    {
        mul(vm);
    }
    break;
    case MUL32:
    {
        mul32(vm);
    }
    break;
    case DIV8:
    {
        div8(vm);
    }
    break;
    case DIV32:
    {
        div32(vm);
    }
    break;
    case CMP:
    {
        cmp(vm);
    }
    break;
    case CMP32:
    {
        cmp32(vm);
    }
    break;
    case SWAP8:
    {
        swap(vm);
    }
    break;
    case SWAP32:
    {
        swap32(vm);
    }
    break;
    case PUTC:
    {
        putch(vm);
    }
    break;
    case JE:
    {
        jmpeq(vm);
    }
    break;
    case JL:
    {
        jmpl(vm);
    }
    break;
    case JG:
    {
        jmpg(vm);
    }
    break;
    case JNE:
    {
        jmpneq(vm);
    }
    break;
    case JMP:
    {
        jmp(vm);
    }
    break;
    case PUSHA32:
    {
        pushA32(vm);
    }
    break;
    case PUSHA8:
    {
        pushA8(vm);
    }
    break;
    case DUP32:
    {
        dup32(vm);
    }
    break;
    case DUP8:
    {
        dup(vm);
    }
    break;
    case POPB8:
    {
        popB8(vm);
    }
    break;
    case POPB32:
    {
        popB32(vm);
    }
    break;
    case PUSHB32:
    {
        pushB32(vm);
    }
    break;
    case PUSHB8:
    {
        pushB8(vm);
    }
    break;
    case MK4:
    {
        push(vm, 0);
        push(vm, 0);
        push(vm, 0);
    }
    break;
    case MK1:
    {
        check_stack(vm, 4);
        vm->sp -= 4;
    }
    break;
    case STORE:
    {
        store(vm);
    }
    break;
    case LOAD:
    {
        load(vm);
    }
    break;
    case LOAD32:
    {
        load32(vm);
    }
    break;
    case STORE32:
    {
        store32(vm);
    }
    break;
    }
}

void exec_all(vm_state *vm, instruction_t *instructions, uint32_t len)
{
    while (vm->pc < len)
    {
        execute(vm, instructions[vm->pc]);
        // vm_view(vm);
        ++vm->pc;
    }
}