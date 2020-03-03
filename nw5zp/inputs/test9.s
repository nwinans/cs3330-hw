.text
main:
    lui $4, 0x1001
    ori $5, 0xf00d
    sw	$5, 0($4)
    lbu $6, 0($4)
    lbu $7, 1($4)
    lbu $8, 2($4)
    lbu $9, 3($4)
    addiu $2, $zero, 0xa
    syscall