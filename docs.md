# DOCUMENTATION
This is the documentation for the VM
# Format
instruction {immediate value type} (stack argument type) -> (stack result type)
# Instructions
## PUSH
push {u8} () -> (u8) 
- pushes u8 value to stack. 
## PUSH32 
push {u32} () -> (u32) 
- pushes u32 value to stack
## PUSHA8
pushA8 () -> (u8) 
- pushes value in A register to stack

## POPA8
popa8 (u8) -> () 
- pops u8 value into A register

## PUSHA32 
pushA32 (u32) -> ()
- pops u32 value into A register

# POPA32 
popA32 () -> (u32)
- pushes value in A register to stack as u32
