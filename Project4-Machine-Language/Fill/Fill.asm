// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// Initialize screen memory and loop forever
(CHECK_KEY)
    @KBD  // address RAM[24576]     
    D=M         // Read keyboard input
    @FILL_BLACK
    D;JGT       // If key pressed > 0), go to fill screen black
    @FILL_WHITE
    0;JMP       // Otherwise, fill screen white

(FILL_BLACK)
    @0          // i value (i=0)
    D=A 
    @i
    M=D

(LOOP_BLACK)
    @i
    D=M        // Load i
    @8192 // SCREEN[8192]
    D=D-A // 
    @CHECK_KEY
    D;JGE      // if i exceeds 8192

    @i
    D=M        // get i
    @SCREEN
    A=D+A      // get SCREEN[i]
    M=-1       // black

    @i
    M=M+1      // i++
    @LOOP_BLACK
    0;JMP      // 

(FILL_WHITE)
    @0          // j = 0
    D=A
    @j
    M=D

(LOOP_WHITE)
    @j
    D=M        // load j
    @8192
    D=D-A
    @CHECK_KEY
    D;JGE      // If j exceeds, restart loop

    @j
    D=M        // Get j
    @SCREEN
    A=D+A      // Get SCREEN[j]
    M=0        // Set pixel to white

    @j
    M=M+1      // j++
    @LOOP_WHITE
    0;JMP      // Repeat
