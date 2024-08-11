#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memdebug.h>
#include "term_colors.h"

typedef enum
{
    PUSH,
    PUSH32,
    POPA8,
    POPA32,
    PUSHA32,
    PUSHA8,
    SUB8,
    SUB32,
    ADD8,
    ADD32,
    DIV8,
    DIV32,
    MUL8,
    MUL32,
    SWAP8,
    SWAP32,
    DUP8,
    DUP32,
    PUTC,
    POPB32,
    POPB8,
    CMP,
    CMP32,
    JE,
    MK4,
    MK1,
    JL,
    JNE,
    STORE,
    STORE32,
    LOAD,
    LOAD32,
    PUSHB32,
    PUSHB8,
    JG,
    JMP,
    LOAD_STR,
    PRINT_STR,
    PRINT_I32,
    PRINT_I8,
    STORE_STR,
    MOD,
    MOD32,
    NOP,
    INS_LABEL, // not used, just used if its compiled from assembly
} opcodes;

typedef struct
{
    // registers
    uint32_t A, B, C, D, E, F;
    // memory
    uint8_t *mem;
    uint32_t mem_size;
    // stack
    uint8_t *stack;
    uint32_t sp;
    uint32_t stack_top;

    uint32_t pc;
} vm_state;
typedef struct
{
    int type;
    uint32_t value;
} instruction_t;

void panic(char *str);
void jmpneq(vm_state *vm);
void check_vm(vm_state *vm);
void stack_underflow();
void check_stack(vm_state *vm, uint32_t num);
void push(vm_state *vm, uint8_t value);
void jmp(vm_state *vm);
void push32(vm_state *vm, uint32_t value);
uint32_t get32(vm_state *vm);
void add(vm_state *vm);
void add32(vm_state *vm);
void putch(vm_state *vm);
void popA8(vm_state *vm);
void popA32(vm_state *vm);
void pushA8(vm_state *vm);
void pushB8(vm_state *vm);
void pushB32(vm_state *vm);
void popB32(vm_state *vm);
void popB8(vm_state *vm);
void pushA32(vm_state *vm);
void dup(vm_state *vm);
void dup32(vm_state *vm);
void mod(vm_state *vm);
void mod32(vm_state *vm);
void sub(vm_state *vm);
void sub32(vm_state *vm);
void mul(vm_state *vm);
void mul32(vm_state *vm);
void div8(vm_state *vm);
void div32(vm_state *vm);
void cmp(vm_state *vm);
void cmp32(vm_state *vm);
void stack_view(vm_state *vm);
void swap(vm_state *vm);
void vm_view(vm_state *vm);
vm_state *create_vm(int stack_size, int mem_size);
void stack_view(vm_state *vm);
void swap(vm_state *vm);
void vm_view(vm_state *vm);
void cleanup_vm(vm_state *vm);
void expand_mem(vm_state *vm, uint32_t size);
vm_state *create_vm(int stack_size, int mem_size);
void execute(vm_state *vm, instruction_t instruction);
void exec_all(vm_state *vm, instruction_t *instructions, uint32_t len);
void panic(char *str);