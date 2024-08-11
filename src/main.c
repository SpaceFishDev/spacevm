#include <memdebug.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "term_colors.h"
#include "parser.h"

instruction_t loop_instructions[] = {
    INS(PUSH32, 0xFFFE), // [50]
    INS(POPA32, 0),      // []
    INS(PUSH32, 0),      // [0]
    // loop (3)
    INS(PUSHA32, 0), // [0,50]
    INS(SWAP32, 0),  // [50,0]
    INS(DUP32, 0),   // [50,0,0]
    INS(POPB32, 0),  // [50,0]
    INS(CMP32, 0),   // [uint8 3]
    INS(MK4, 0),     // [3]
    INS(PUSHB32, 0), // [3,0]
    INS(PUSH32, 1),  // [3,0,1]
    INS(ADD32, 0),   // [3,1]
    INS(SWAP32, 0),  // [1,3]
    INS(PUSH32, 5000),
    INS(JE, 0), // [1,3]
    INS(PUSH32, 3),
    INS(JMP, 0),
};

instruction_t memory[] = {
    INS(PUSH32, 40),
    INS(PUSH32, 10),
    INS(STORE32, 0),
    INS(PUSH32, 10),
    INS(LOAD32, 0),
};
instruction_t fizzbuzz[] = {
    INS(PUSH32, 1000),
    INS(POPA32, 0),
    INS(PUSH32, 0),
    // start
    INS(PUSH, 0),
    INS(POPB8, 0),
    INS(DUP32, 0),
    INS(PUSH32, 3),
    INS(MOD32, 0),
    INS(PUSH32, 0),
    INS(CMP32, 0),
    INS(MK4, 0),
    INS(PUSH32, 17),
    INS(JNE, 0),
    INS(PUSH, 'f'),
    INS(DUP8, 0),
    INS(POPB8, 0),
    INS(PUTC, 0),
    // end0
    INS(DUP32, 0),
    INS(PUSH32, 5),
    INS(MOD32, 0),
    INS(PUSH32, 0),
    INS(CMP32, 0),
    INS(MK4, 0),
    INS(PUSH32, 29),
    INS(JNE, 0),
    INS(PUSH, 'b'),
    INS(DUP8, 0),
    INS(POPB8, 0),
    INS(PUTC, 0),
    // end1
    INS(DUP32, 0),
    INS(PUSHB8, 0),
    INS(DUP8, 0),
    INS(PUSH, 0),
    INS(CMP, 0),
    INS(MK4, 0),
    INS(PUSH32, 39),
    INS(JNE, 0),
    INS(DUP32, 0),
    INS(PRINT_I32, 0),
    // end 2
    INS(PUSH, '\n'),
    INS(PUTC, 0),
    INS(PUSH32, 1),
    INS(ADD32, 0),
    INS(SWAP32, 0),
    INS(PUSHA32, 0),
    INS(CMP32, 0),
    INS(MK4, 0),
    INS(PUSH32, 3),
    INS(JL, 0),
    // end loop
    INS(POPA32, 0), // cleanup stack
};

instruction_t ins[] = {};

int main(void)
{
    // vm_state *vm = create_vm(50000, 50000);
    // printf("OUTPUT: \n");
    // exec_all(vm, fizzbuzz, (sizeof(fizzbuzz) / sizeof(instruction_t)));
    // printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    // vm_view(vm);
    // cleanup_vm(vm);
    init_memdebug();

    char *file = readfile("./main.sas");
    parser_state *parser = create_parser(file);
    command_t *commands = parse_all(parser);
    free(parser->tokens);
    int len = 0;
    instruction_t *compiled = compile(commands[0], &len);
    vm_state *vm = create_vm(100, 100);
    exec_all(vm, compiled, len);
    vm_view(vm);
    free(file);

    memdebug_view();
    cleanup_memdebug();
    return 0;
}