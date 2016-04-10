1.Instruction on how to compile:
(Lets use command line to compile)

$cd src
$g++ -o VMTranslator VMTranslator.cpp

After that, there will be a executable-file called "VMTranslator".


2.Instruction on how to run.
(Lets use command line to run. Assuming that there is a inputfile called test.vm)

$./VMTranslator test.vm

Then there will be a new file called test.asm which contains the results.

Also, if there are more than 1 file, simply input:

$./VMTranslator file1.vm file2.vm file3.vm file4.vm

And then it will translate all of them

3. Any xxx.vm file would work in my code.