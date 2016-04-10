// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
//basic idea: for(i = 1; i-R1 != 0;i++) R2 = R2 + R0


@i    //i refers to some mem location
M=1   //i = 1

@R2   //R2 refers to some mem location
M=0	  //R2 = 0

(LOOP) //loop
@i   	
D=M    //D = i
@R1    //R1 refers to some mem location 
D=D-M  //D = D(i) - R1
@END
D;JGT	//if D == 0(which is i == R1), jump to end

@R0		
D=M     //D = R0
@R2     
M=D+M    //R2 = D(R0) + R2

@i       
M=M+1    //i = i + 1

@LOOP   
0;JMP     //go to loop
 
(END)
@END
0;JMP    //stay in end
