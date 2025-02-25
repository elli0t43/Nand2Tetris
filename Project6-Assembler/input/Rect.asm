// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/6/rect/Rect.asm

// Draws a rectangle at the top-left corner of the screen.
// The rectangle is 16 pixels wide and R0 pixels high.
// Usage: Before executing, put a value in R0.

   // If (R0 <= 0) goto END else n = R0
   @R0 // reading the column value from RAM[0]
   D=M // fetching the value into D
   @END
   D;JLE // jump if less equal
   @n // (16)
   M=D // inserting the value of RAM[0] into n
   // addr = base address of first screen row
   @SCREEN // very first address of SCREEN register
   D=A // setting D as address of the SCREEN register
   @addr // (17)
   M=D // insertng the address of @SCREEN into @addr
   
   
(LOOP)

   // RAM[addr] = -1 // since 2's complement of -1 is 1111111111111 (i forgot how many 1s)
   @addr 
   A=M // putting the @addr/@SCREEN into the A register
   M=-1 // setting 1111111111 to make sure the it puts black pixels on that specific address
   
   // addr = base address of next screen row
   @addr 
   D=M // current address of the screen put into D
   @32 // using 32 as a variable, since row wise, its +32 per block
   D=D+A // adding that 32 with current address of the screen which gives the next address of screen
   @addr // fetching the current address of the screen
   M=D // putting the next address which we got from D = D+A into M for next instruction
   
   // decrements n and loops
   @n
   MD=M-1 // basicall putting M = M-1 and D = M-1 (author was lazy as fuck)
   // now here's the thing, why there is 2 substraction ? mainly because we need to decrease the column number as well
   // so if its 255, we need to make it 255-1, since we already put all 16 black pixel on the 0th index
   @LOOP // basically loop
   D;JGT // loop until D becomes 0 nd then proceeds to @END
   
(END)
   @END
   0;JMP