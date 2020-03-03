.text
main: 
addiu $2, $zero, 10
j l2

l1:
ori $3, 15 # shouldn't reach
syscall

l2:
bgtz $2, l3
syscall # shouldn't reach

l3:
addu $3, $3, $2
xori $3, 3
syscall #$3 = 0x00000009