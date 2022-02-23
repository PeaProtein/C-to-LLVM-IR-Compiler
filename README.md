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
	int x;
	x = 2;
	int y;
	y = 4;
	print x + y;
	```
2.	Variables must be declared and assigned on separate lines.
	### Correct
	```
	int x;
	x = 2;
	print x;
	```
	
	### Incorrect
	```
	int x = 2;
	print x;
	```
3.	Rather than using scanf(), user input can be read using "read <variable name>;".
	### Correct
	```
	int x;
	read x;
	print x;
	```
	
	### Incorrect
	```
	int x;
	scanf("%d", &x);
	print x;
	```
4.	Rather than using printf(), values can be output to the console using "print <variable name>;" or "print <numerical expression>;".
	### Correct
	```
	int x;
	x = 5 + 5 * 20;
	print x;
	print 2;
	print -1--1;
	print (4 + 2) / 17 + 8;
	print 10 % 3;
	```
	As can be seen above, operator precendence, as well as the modulo (%) and negation (-) operators, are supported in expression evaluation.
	
	### Incorrect
	```
	int x;
	x = 5 + 5 * 20;
	printf("%d\n", x);
	printf("%d\n, 2);
	printf("%d\n, 10 % 3);
	```
5.	Conditional statements must be enclosed between curly brackets.
	### Correct
	```
	int x;
	int y;
	read x;
	read y;
	if (x > y) {
		print x;
	}
	else {
		while (x <= y) {
			x = x + 1;
		}
		print x;
	}
	```
	     
	### Incorrect
