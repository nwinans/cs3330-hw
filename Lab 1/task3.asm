.data
	string1: .asciiz "\nEnter the first string: "
	string2: .asciiz "\nEnter the second string: "
	output: .asciiz "\nThe output is: "
	newLine: .asciiz "\n"
	buffer1: .space 100
	buffer2: .space 100

.text
	main:
	li $v0,4 #v0 coded to 4 printing string
	la $a0, string1 #loading the address of the string1 text
	syscall #printing the string1 text prompt

	li $v0,8 #take in input string
	la $a0,buffer1 #load byte space into address
	li $a1, 100 #allot the byte space for the string input
	move $s1,$a0 # save string input1 to s1
	syscall #execute the command


	li $v0,4 #v0 coded to 4 printing string
	la $a0, string2
	syscall 

	li $v0,8 #take in input string
	la $a0,buffer2
	li $a1, 100
	move $s2,$a0
	syscall

	li $t0,0 #i=0
	li $t1,0 # count=0

	loop:

		lb   $s3, 0($s1) #s3 = input1[i]
		lb   $s4, 0($s2) #s4 = input2[i]
    		beq  $s3, $zero exit


		bne $s3,$s4,count

		addi $s1,$s1,1
		addi $s2,$s2,1
		j loop 

	count:
		addi $t1,$t1,1 # 
		addi $s1,$s1,1
		addi $s2,$s2,1

		j loop 

	exit:

		li $v0,4
		la $a0,output
		syscall

		li $v0,1
		move $a0,$t1
		syscall

		li $v0,10 #end program
		syscall
