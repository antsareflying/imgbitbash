# IBB

bashing image bits together

# Description

ImageBitBashing is a command line utility that takes 2 .bmp images and a bitwise operator as arguments. It uses the images bits and the operands to make a new .bmp image.

## Input files

This program takes 2 .bmp images that uses the BITMAPINFOHEADER header(other headers like the V4 and V5 are planned), 24 bit colour, and no compression.

## Output file

This program outputs 1 .bmp image that uses the BITMAPINFOHEADER, 24 bit colour, and no compression.

## Operator

This program takes a bitwise operator as its third argument. It can either be:

+ XOR
	+ XOR
	+ xor
	+ ^
+ OR
	+ OR
	+ or
	+ |(this is not recommended as it affects the shell)
+ AND
	+ AND
	+ and
	+ &

## Example

This:

![sample image 1](https://raw.githubusercontent.com/antsareflying/imgbitbash/main/images/sample1.png)

and this:

![sample image 2](https://raw.githubusercontent.com/antsareflying/imgbitbash/main/images/sample2.png)

using the and operator prodces this:

![and operator output](https://raw.githubusercontent.com/antsareflying/imgbitbash/main/images/outputand.png)

using that xor operator produces this:

![xor operator output](https://raw.githubusercontent.com/antsareflying/imgbitbash/main/images/outputxor.png)

and using the or operator produces this:

![or operator output](https://raw.githubusercontent.com/antsareflying/imgbitbash/main/images/outputor.png)


## Building 

Tested on Linux only but should compile and work with any c11 (c99 not tested) standard compiler. (Only tested with GCC)

To build on linux with GCC type `gcc -o ibb ibb.c ibb.h` in the terminal after cd-ing into the directory

to run type `./ibb` 
