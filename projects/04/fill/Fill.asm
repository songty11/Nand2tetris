// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.

 	@i             //i is the loacation of some memory
    M=0             // i = 0

(LOOP)
    @KBD    // waiting for input, pressed-> KBD != 0,
    D = M     //D = KBD, check if there is any input(key press)

    @WHITE
    D;JEQ // KBD = 0 no press, goto white

    // else we set it to black

    @i          
    D = M     // D = i
    @8192   // Max number of screen word 16 * 512
    D = D - A   // Check if the counter has reach its maximum value 8192

    @LOOP   // If it is we do nothing, everythin is blacken'd
    D;JGE   

    @8192      
    D = D + A  //correct offset

    @SCREEN
    A = A + D    //D is offset, we set addr: A+offset D to -1(next step)
    M = -1    // -1 is black
    
    @i
    M = M + 1   // i = i + 1 next pixel
    
    @LOOP   // Go back and listen to the keyboard again.
    0;JMP

(WHITE)
    @i
    D = M     // check if D = i < 0
    
    @LOOP   // if i < 0 go back to loop (set to the last pixel)
    D;JLT

    @SCREEN
    A =A + D  //D is offset, we set addr: A+offset D to 0(next step)
    M = 0     // 0 is white

    @i
    M = M - 1   // i = i - 1

    @LOOP
    0;JMP   //go back to loop and wait