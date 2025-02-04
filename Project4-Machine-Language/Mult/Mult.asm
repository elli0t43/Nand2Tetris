// Multiplication is nothing but adding 1 value times the 2nd value, meaning if I have 6 * 4, I can sum 6, 4 times to get the value (6+6+6+6), can be done the other way around (4+4+4+4+4+4)


@mult // setting the mult=0
M=0
@i 
M=0 // setting i = 0, for the if check
(LOOP)
    @i
    D=M  // fetching the i value

    @R0
    D=D-M // if statement

    @STOP 
    D;JGE // jump to stop if greather than / equal to if (i >= R0)


    @R1 // fetching R1 value
    D=M

    @mult
    M=D+M // the sum operation

    @i
    M=M+1 // i++

    @LOOP // loop
    0;JMP

(STOP)
    @mult // fetching the final mult value
    D=M

    @R2 // setting RAM[2] to the mult value
    M=D

(END)
    @END
    0;JMP

