.text
main:
    addiu $2, $zero, 0xa
    jal next
next:
    addiu $ra, $ra, 0x10
    jr $ra
    addiu $5, $zero, 5
    syscall
    addiu $5, $zero, 0xd00d
    syscall