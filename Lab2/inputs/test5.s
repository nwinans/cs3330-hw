.text
main:
    ori     $4, 0xdead
    lui     $5, 0x1000
    sw		$4, 0($5) #0xdead
    xor     $4, $4, 0xd00d #0xea0
    sw      $4, 0($5) #0xea0
    lw      $6, 0($5) #0xea0
    lw      $7, 8($5) #0x000
    addiu   $2, $zero, 0xa #0xa
    syscall