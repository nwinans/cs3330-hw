.text
main:
	addiu $2, $zero, 0xa
	addiu $4, $zero, -1
	bgtz $4, l_0
	beq $4, $zero, l_1
	addiu $5, $zero, 1
l_0:
	addiu $5, $5, 2
l_1:
	addiu $5, $5, 3
	syscall