#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "term_colors.h"
#include "vm.h"

#define INS(type, value) \
    (instruction_t) { (uint32_t) type, (uint32_t)value }

int main(void)
{
    printf("Hello, World!\n");
    vm_state *vm = create_vm(500, 500);
    execute(vm, INS(PUSH, 69));
    printf("\n");
    vm_view(vm);
    cleanup_vm(vm);
    return 0;
}