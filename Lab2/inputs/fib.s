.text

main:
li $4, 5

Fib:
li $2, 0            # load zero into the return register
slti $18, $4, 1     # if (n < 1) tmp = 1
bgtz $18, Exit      # if (tmp == 1) goto Exit
li $16, 0           # a = 0
li $17, 1           # b = 0
add $2, $16, $17    # c = a + b 

Loop:
li $18, 0           # tmp = 0
beq $4, $18, Exit   # if (n == 0) goto Exit
add $2 $16, $17     # c = a + b
move $16, $17       # a = b
move $17, $2        # b = c
addi $4, $4, -1     # n--
j Loop            # go to Loop

Exit:
jr $31             # exit program