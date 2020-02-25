.text
addi    $t0, $zero, 24593
addi    $t1, $zero, 32767
mult	$t0, $t1
addi    $t0, $zero, 20000
mflo    $t1
mult    $t0, $t1
addiu   $v0, $zero, 0xa
syscall