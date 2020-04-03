.text
main:
    lui $4, 0x1000
    addiu $5, $zero, 0xd00d
    sw $5, 0($4)
    and $5, $5, $zero
    lw $5, 0($4)
    addiu $2, $zero, 0xa
    syscall