REPL:
slti $19, $18, 1    #if (count < 1) temp = 1
bgtz $19, EXIT      # if (temp = 1) jump EXIT
lw $19, $16         # tmp = memory[src]
sw $19, $17         # memory[dest] = temp
addi $16, $16, 1    # src++
addi $17, $17, 1    # dest++
addi $18, $18, 1    # count--
j REPL              # jump to REPL

EXIT:
jr $31              # return to program