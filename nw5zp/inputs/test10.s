.text
main:
    lui $4, 0x1001
    ori $5, 0xde
    ori $6, 0xad
    ori $7, 0xbe
    ori $8, 0xef
    ori $9, 0xdead
    ori $10, 0xbeef
    sb $5, 3($4)
    sb $6, 2($4)
    sb $7, 1($4)
    sb $8, ($4)
    sh $9, 6($4)
    sh $10, 4($4)
    lw $11, 0($4)
    lw $12, 4($4)
    addiu $2, $zero, 0xa
    syscall