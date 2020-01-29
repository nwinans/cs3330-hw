.data
arr:       .word     4, 50, 200, 1, 5
space:     .asciiz   " "
new_line:  .asciiz   "\n"
 
.text
.globl main
main:
    la      $t0, arr             # set the starting address of the array in $s0
    li      $t1, 5               # set the number of elements in $s1
    jal     gnome_sort           # Perform gnome sort
    li      $t2, 0
print:
    beq     $t2, $t1, exit       # exit if reached the max count
    lw      $a0, 0($t0)          # get the element from the array
    ori     $v0, $0, 1           # $v0 = 1 to print an int value
    syscall
    la      $a0, space
    ori     $v0, $0, 4           # $v0 = 4 to print ascii
    syscall
    addiu   $t0, $t0, 4          # increment the index by 4
    addiu   $t2, $t2, 1          # increment the count by 1
    j       print
exit:
    la      $a0, new_line
    ori     $v0, $0, 4
    syscall
    ori     $v0, $0, 10          # set command to stop program,
    syscall
 
# $t0 => base address of arr (&arr[0])
# $t1 => number of elements in arr (n)
# $v0 => i
gnome_sort:
    addiu   $sp, $sp, -4
    sw      $ra, 0($sp)          # store $ra into the stack
    sll     $t1, $t1, 2          # count = count * 2
    li      $v0, 0               # int i = 0
  loop:
    slt     $t3, $v0, $t1        # if (i < n) => $t3 = 1
    beq     $t3, $zero, end      # while (i < n) {
    bne     $v0, $zero, compare  # if (i == 0)
    addiu   $v0, $v0, 4          # i = i + 1
  compare:
    addu    $t2, $t0, $v0        # $s2 = &arr[i]
    lw      $t4, -4($t2)         # $t4 = arr[i-1]
    lw      $t5, 0($t2)          # $t5 = arr[i]
    blt     $t5, $t4, swap       # swap if (arr[i] < arr[i-1])
    addiu   $v0, $v0, 4          # i = i+ 1
    j       loop
  swap:
    sw      $t4, 0($t2)          # swap (arr[i], arr[i-1])
    sw      $t5, -4($t2)
    addiu   $v0, $v0, -4         # i = i - 1
    j       loop
  end:
   srl     $t1, $t1, 2
   lw      $ra, ($sp)            # copy from stack to $ra
   addi    $sp, $sp, 4           # increment stack pointer by 4
   jr      $ra                   # return to main