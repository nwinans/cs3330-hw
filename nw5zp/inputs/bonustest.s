.text
main:
    addiu $2, $zero, 1 # Syscall 1: Print INT
    addiu $4, $zero, 5 # load 5 to print
    syscall # print 5

    addiu $2, $zero, 11 # Syscall 11: Print CHAR
    addiu $4, $zero, 0x0A # load new line char
    syscall # new line

    lui $6, 0x1000 # setup memory access
    addiu $7, $zero, 0x68656c6c #put hello world into memory
    sw $7, ($6)
    addiu $7, $zero, 0x6f2c2077
    sw $7, 4($6)
    addiu $7, $zero, 0x6f726c64
    sw $7, 8($6)
    addiu $7, $zero, 0x210A0000
    sw $7, 12($6)

    addiu $2, $zero, 9 # Syscall 9: SBRK (malloc)
    addiu $4, $zero, 24
    syscall # malloc(24)

    addiu $2, $zero, 13 # Syscall 13, Open File
    addiu $6, $6, 16
    addiu $7, $zero, 0x74657374
    sw $7, ($6)
    addiu $7, $zero, 0x2e747874
    sw $7, 4($6)
    addu $4, $zero, $6
    addiu $5, $zero, 1
    syscall

    addu $4, $zero, $2 # move file pointer to r4
    addiu $2, $zero, 15 # Syscall 15, print to file
    addi $5, $6, -16 # move to hello world pointer
    addiu $6, $zero, 14 # print 4 chars to file
    syscall 

    addiu $2, $zero, 14 # Syscall 14, read from file
    addiu $5, $5, 40
    syscall

    addiu $2, $zero, 16 # Syscall 16, close file
    syscall

    addiu $2, $zero, 4 # Syscall 4: Print String
    addu $4, $5, $zero
    syscall # print "hello, world\n" (the one read from the file)

    addiu $2, $zero, 34 # Syscall 34: Output Hex
    syscall 

    addiu $2, $zero, 11 # Syscall 11: Print CHAR
    addiu $4, $zero, 0x0A # load new line char
    syscall # new line

    addiu $2, $zero, 17 # Syscall 17: Exit with code
    addiu $4, $zero, 0
    syscall # end program, with code 0