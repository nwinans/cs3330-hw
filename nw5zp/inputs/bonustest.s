.data
    file: .ascii "file.txt"

.text
main:
    addiu $2, $zero, 1 # Syscall 1: Print INT
    addiu $4, $zero, 5 # load 5 to print
    syscall # print 5

    addiu $2, $zero, 11 # Syscall 11: Print CHAR
    addiu $4, $zero, 0x0A # load new line char
    syscall # new line

    addiu $2, $zero, 4 # Syscall 4: Print String
    lui $6, 0x1000 # setup memory access
    addiu $7, $zero, 0x68656c6c #put hello world into memory
    sw $7, ($6)
    addiu $7, $zero, 0x6f2c2077
    sw $7, 4($6)
    addiu $7, $zero, 0x6f726c64
    sw $7, 8($6)
    addiu $7, $zero, 0x210A0000
    sw $7, 12($6)
    addu $4, $zero, $6
    syscall # print "hello, world\n"

    addiu $2, $zero, 9 # Syscall 9: SBRK (malloc)
    addiu $4, $zero, 24
    syscall # malloc(24)

    addiu $2, $zero, 0xa
    syscall # end program