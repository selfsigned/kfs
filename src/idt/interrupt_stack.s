

/* SAVING STACK STATE
Uses the stack to store the information needed for returning to the interrupted code
An interrupt pushes the EFLAGS register before saving the address of the interrupted instruction. 
Certain types of exceptions also cause an error code to be pushed on the stack to help debug the exception.
*/

/*

    CPU checks the current privilege level

    if need to change privilege level

            change stack with the one associated with new privilege
            save old stack information on the new stack

    save EFLAGS, CS, EIP on stack

    save error code on stack in case of an abort

    execute the kernel interrupt handler

*/

/* EIP address to save the instruction that cause the fault or ther instruction after*/

/* RESTABLISH  STACK STATE

    pop the error code (in case of an abort)
    call IRET
        pops values from the stack and restore the following register: CS, EIP, EFLAGS
        if privilege level changed returns to the old stack and old privilege level
*/
