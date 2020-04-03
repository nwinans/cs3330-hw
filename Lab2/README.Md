# Nick Winans - nw5zp
## Lab 2 - Part 2

For the bonus instructions, I added 10 syscalls. 
I added the syscall to...
- output an int from a register
- output a char from a register
- output the contents of a register in hex
- print a null terminated string stored in memory.
- open a filestream
- close a filestream
- write n characters to a file from a null terminated string in memory
- read n characters from a file to a null terminated string in memory
- malloc n bytes of memory and return the pointer
- terminate running the program with a code

They are all tested in "bonustest.s". 
The program will create a file called test.txt in the directory you run the simulation in with the contents "hello, world!", then read from that file and output to the console.
It will also output the integer 5 in the beginning and then the hex value of the memory address of where "hello, world!" was stored in memory after reading from the file. 
Finally it will exit with code 0. 

Overall the code should be fairly self documented with variable names describing the function they perform. The file management syscalls are a bit more complex to deal with byte by byte reading and writing from memory and the file.