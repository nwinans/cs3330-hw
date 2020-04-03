.text
main:
    addi    $4, $zero, -1
    blt		$4, $zero, taken
    ori     $5, 15
    
taken:
    addiu   $5, 10
    addiu   $2, $zero, 0xa
    syscall