.text
addi    $t0, $zero, -2
addi    $t1, $zero, 32767
mult	$t0, $t1
addiu   $v0, $zero, 0xa
syscall