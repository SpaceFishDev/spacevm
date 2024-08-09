#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "term_colors.h"
#include "vm.h"

#define INS(type, value) \
    (instruction_t) { (uint32_t) type, (uint32_t)value }

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
int main(void)
{
    printf("Hello, World!\n");
    vm_state *vm = create_vm(50000, 50000);
    exec_all(vm, loop_instructions, (sizeof(loop_instructions) / sizeof(instruction_t)));
    printf("\n");
    vm_view(vm);
    cleanup_vm(vm);
    return 0;
}