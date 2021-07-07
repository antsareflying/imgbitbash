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

+XOR
	+XOR
	+xor
	+^
+OR
	+OR
	+or
	+|(this is not recommended as it affects the shell)
+AND
	+AND
	+and
	+&


