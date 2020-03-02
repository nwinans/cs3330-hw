.text
bgtz $zero, nottaken
bgtz $8, nottaken
bgtz $9, taken1
addiu $10, $zero, 1
addiu $2, $zero, 10
syscall

nottaken:
addiu $12, $zero, 1
addiu $2, $zero, 10
syscall

taken1:
addiu $11, $zero, 1
addiu $2, $zero, 10
syscall