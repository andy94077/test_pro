# test_pro
A program to test your program with some input and output file.

## Installation
Use the [makefile]( https://github.com/andy94077/test_pro/blob/master/test_pro/makefile).
```
make
```

## How to use

You need to write some settings in the ```default.dat```. The program will read the data when initializing.
### Set the default.dat
The first line should be "test_pro" and its version.
```
test_pro 2.0
```
And the next 5 lines should be the path to your test program, the directory of your test files, the file extension of test files, and the test files range. After version 2.0, you can use the diff command option.
```
~/path/to/test/program
“/you/can/enclose it/in/quotes”
.testFileExtension
0 10
-y -W30
```
You can also use pipeline | to redirect the output to another command. For instance,
```
-y -W30 | head –n 10
```
It will use diff to compare answers, and if they are incorrect, it will output the first 10 lines.

### Note
For Windows users, since you don't have diff, you can't use the diff feature. To solve this problem, you must launch test_pro only in git bash, or you can copy ```diff.exe``` and some dll files from git bash and then add diff.exe to the environment variables.


## Examples
To learn more about the examples, check out [example]( https://github.com/andy94077/test_pro/tree/master/example)

* We have a program to be tested: ```~/swap_number```
* We have some test files, which save in ```~/test files```
* The filename extension of test files is ```.in```
* We need to test them from 0.in to 5.in.
* The diff options are ```-Z -y -W80 --suppress-common-lines```

Therefore, the default.dat will be,
```
test_pro 2.0
~/swap_number
~/test files
.in
0 5
-Z -y -W80 --suppress-common-lines
```
And then we run test_pro. Assume that we want to test 5.in and 3.in twice. So we type ```-s 5 3```, and type ```r``` to start testing.
test_pro will show you which ones are incorrect, and which ones are accepted.

## History

I major in computer science at National Taiwan University, and I did some programming exercises on our programming website. I don't want to submit my code and get a wrong answer label, so I develop a program to test it.

## Special Thanks

* Stackoverflow
* cplusplus.com
* Google
