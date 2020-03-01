        # Basic arithmetic instructions
        # This is a hodgepodge of arithmetic instructions to test
        # your basic functionality.
        # No overflow exceptions should occur
	.text
main:   
        addiu   $2, $zero, 1024         # $2 = 0x00000400
        addu    $3, $2, $2              # $3 = 0x00000800
        or      $4, $3, $2              # $4 = 0x00000C00
        add     $5, $zero, 1234         # $5 = 0x000004D2
        sll     $6, $5, 16              # $6 = 0x04D20000
        addiu   $7, $6, 9999            # $7 = 0x04D2270F
        subu    $8, $7, $2              # $8 = 0x04D2230F
        xor     $9, $4, $3              # $9 = 0x00000400
        xori    $10, $2, 255            # $10 = 0x000004FF
        srl     $11, $6, 5              # $11 = 0x00269000
        sra     $12, $6, 4              # $12 = 0x004D2000
        and     $13, $11, $5            # $13 = 0x00000000
        andi    $14, $4, 100            # $14 = 0x00000000
        sub     $15, $zero, $10         # $15 = 0xFFFFFB01
        mult    $7, $3                  
        mfhi    $16                     # $16 = 0x00000026
        mflo    $17                     # $17 = 0x91387800
        addiu   $v0, $zero, 0xa         # $2 = 0x0000000A
        syscall
        
        
                        
