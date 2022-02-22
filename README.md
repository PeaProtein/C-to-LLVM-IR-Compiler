# C-to-LLVM-IR-Compiler
A compiler that converts a simplified form of the C programming language into LLVM IR which can be compiled and run with Clang. Written entirely in C.

This compiler is capable of handling variable declaration, assignment, logical and arithmetic expression evaluation, print statements, integer input, error messaging, if-else branches, and while loops.

# SimpleC Syntax (Informal)
This simplified form of C is mostly similar to conventional C with only a few exceptions.

1.	All variable declarations must occur at the start of the program.
	### Correct
	```
	int x;
	int y;
	x = 2;
	y = 4;
	
	print x + y;
	```
	
	### Incorrect
	```
	inx x;
	x = 2;
	int y;
	y = 4;
	
	print x + y;
	```
2.
