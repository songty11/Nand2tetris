@1
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@R15
M=D
@1
D=A
@R3
D=A+D
@R14
M=D
@R15
D=M
@R14
A=M
M=D
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@R15
M=D
@0
D=A
@THAT
D=M+D
@R14
M=D
@R15
D=M
@R14
A=M
M=D
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@R15
M=D
@1
D=A
@THAT
D=M+D
@R14
M=D
@R15
D=M
@R14
A=M
M=D
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M-D
@SP
M=M+1
@SP
AM=M-1
D=M
@R15
M=D
@0
D=A
@ARG
D=M+D
@R14
M=D
@R15
D=M
@R14
A=M
M=D
(Sys.init$MAIN_LOOP_START)
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@Sys.init$COMPUTE_ELEMENT
D;JNE
@Sys.init$END_PROGRAM
0;JMP
(Sys.init$COMPUTE_ELEMENT)
@0
D=A
@THAT
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@THAT
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M+D
@SP
M=M+1
@SP
AM=M-1
D=M
@R15
M=D
@2
D=A
@THAT
D=M+D
@R14
M=D
@R15
D=M
@R14
A=M
M=D
@1
D=A
@R3
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M+D
@SP
M=M+1
@SP
AM=M-1
D=M
@R15
M=D
@1
D=A
@R3
D=A+D
@R14
M=D
@R15
D=M
@R14
A=M
M=D
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M-D
@SP
M=M+1
@SP
AM=M-1
D=M
@R15
M=D
@0
D=A
@ARG
D=M+D
@R14
M=D
@R15
D=M
@R14
A=M
M=D
@Sys.init$MAIN_LOOP_START
0;JMP
(Sys.init$END_PROGRAM)