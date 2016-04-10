There are two folder in src/, part1_tokenizer is for tokenizer testing, part2_Parser is for Parser Tesing, which also is the final version.

1.Instruction on how to compile:
(Lets use command line to compile)

$cd src
$g++ -std=c++11 -o Parser Parser.cpp

After that, there will be a executable-file called "Parser".


2.Instruction on how to run.
(Lets use command line to run. Assuming that there is a folder f1/)

$./VMTranslator f1/

Let's go to folder f1, and you will see a folder called "Myresult" which contains the output xml files.

3. The program is tested in a UNIX platform.