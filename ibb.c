#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define BMSIG_OFFSET 0x00
#define FILESIZE_OFFSET 0x02
#define DATA_OFFSET_OFFSET 0x0A

#define HEADER_SIZE_OFFSET 0x0E
#define WIDTH_OFFSET 0x12
#define HEIGHT_OFFSET 0x16
#define BPP_OFFSET 0x1C
#define COMPRESSION_OFFSET 0x1E


uint32_t swapbytes32 (uint32_t a);
uint16_t swapbytes16 (uint16_t a);

int main(int argc, char *argv[])
{
	/*looks for 3 command line arguments.*/
	
	if (argc != 4) 
	{
		printf("Only input 3 command line arguments!\n");
		/*TODO print_help();*/
		return 1;
	}

	errno = 0;
	/*opens the img files given in cmd line args*/
	FILE *fp1 = fopen(argv[1], "rb");
	FILE *fp2 = fopen(argv[2], "rb");
	if (fp1 == NULL)
	{
		printf("Error accessing first file errno %d: %s\n", errno, strerror(errno));
		fclose(fp1);
		return 1;
	}
	
	if (fp2 == NULL)
	{
		printf("Error accessing second file errno %d: %s\n", errno, strerror(errno));		printf("Invalid second file\n");
		fclose(fp2);
		return 1;
	}


	uint8_t tmpbytes2[2];

	/*check if bitmap signature present*/


	fseek(fp1, BMSIG_OFFSET, SEEK_SET);
	fread(tmpbytes2, 1, 2, fp1);

	if (tmpbytes2[0] != 0x42 || tmpbytes2[1] != 0x4D)
	{
		printf("The first file is not a .BMP file\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	fseek(fp2, BMSIG_OFFSET, SEEK_SET);
	fread(tmpbytes2, 1, 2, fp2);

	if (tmpbytes2[0] != 0x42 || tmpbytes2[1] != 0x4D)
	{
		printf("The second file is not a .BMP file\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}


	/*check that both img is using BITMAPINFOHEADER*/
	uint32_t headersize1 = 0;
	uint32_t headersize2 = 0;
	
	fseek(fp1, HEADER_SIZE_OFFSET, SEEK_SET);
	fread(&headersize1, 4, 1, fp1);
	swapbytes32(headersize1);
	if (headersize1 != 40)
	{
		printf("The first file does not use BITMAPINFOHEADER\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	fseek(fp2, HEADER_SIZE_OFFSET, SEEK_SET);
	fread(&headersize2, 4, 1, fp2);
	swapbytes32(headersize2);
	if (headersize1 != 40)
	{
		printf("The second file does not use BITMAPINFOHEADER\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}
/*TODO chek for other headers*/
	

	/*find size of both the files*/

	uint32_t filesize1 = 0;
	uint32_t filesize2 = 0;
	
			
	fseek(fp1, FILESIZE_OFFSET, SEEK_SET);
	fread(&filesize1, 4, 1, fp1);
	swapbytes32(filesize1);
	

	fseek(fp2, FILESIZE_OFFSET, SEEK_SET);
	fread(&filesize2, 4, 1, fp2);
	swapbytes32(filesize2);

	/*find height of images*/

	uint32_t imght1 = 0;
	uint32_t imght2 = 0;

	fseek(fp1, HEIGHT_OFFSET, SEEK_SET);
	fread(&imght1, 4, 1, fp1);
	swapbytes32(imght1);

	fseek(fp2, HEIGHT_OFFSET, SEEK_SET);
	fread(&imght2, 4, 1, fp2);
	swapbytes32(imght2);



	/*find width of images*/
	uint32_t imgwd1 = 0;
	uint32_t imgwd2 = 0;

	fseek(fp1, WIDTH_OFFSET, SEEK_SET);
	fread(&imgwd1, 4, 1, fp1);
	swapbytes32(imgwd1);

	fseek(fp2, WIDTH_OFFSET, SEEK_SET);
	fread(&imgwd2, 4, 1, fp2);
	swapbytes32(imgwd2);

	/*check that both images use 24 bit colour*/
	uint16_t bppvalue = 0;
	
	fseek(fp1, BPP_OFFSET, SEEK_SET);
	fread(&bppvalue, 2, 1, fp1);
	if(bppvalue != 24)
	{
		printf("First image is not using a 24 bit color!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	bppvalue = 0;
	
	fseek(fp2, BPP_OFFSET, SEEK_SET);
	fread(&bppvalue, 2, 1, fp2);
	if(bppvalue != 24)
	{
		printf("Second image is not using a 24 bit color!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	/*check that both images dont use compression*/
	uint32_t compressionvalue = 0;
	
	fseek(fp1, COMPRESSION_OFFSET, SEEK_SET);
	fread(&compressionvalue, 4, 1, fp1);
	if(compressionvalue != 0)
	{
		printf("First image is using compression!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	fseek(fp2, COMPRESSION_OFFSET, SEEK_SET);
	fread(&compressionvalue, 4, 1, fp2);
	if(compressionvalue != 0)
	{
		printf("Second image is using compression!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	/*get data offset for both files*/
	uint32_t dataoffset1 = 0;
	uint32_t dataoffset2 = 0;
	
	fseek(fp1, DATA_OFFSET_OFFSET, SEEK_SET);
	fread(&dataoffset1, 4, 1, fp1);
	swapbytes32(dataoffset1);


	fseek(fp2, DATA_OFFSET_OFFSET, SEEK_SET);
	fread(&dataoffset2, 4, 1, fp2);
	swapbytes32(dataoffset2);

	/*read images into memory 2d array*/
	uint8_t imgarray1[imght1][imgwd1];
	uint8_t imgarray2[imght2][imgwd2];
	
	fseek(fp1, dataoffset1, SEEK_SET);
	fread(imgarray1, 1, imght1*imgwd1, fp1);

	
	fseek(fp2, dataoffset2, SEEK_SET);
	fread(imgarray2, 1, imght2*imgwd2, fp2);
/*TODO crop larger photo to be same width and height as smaller*/
/*TODO setup the operands*/

	char operator[4];
	
	if(strcmp(argv[3], "&") == 0 || strcmp(argv[3], "and") == 0 || strcmp(argv[3], "AND") == 0)
	{
		strcpy(operator, "and");
	}

	else if(strcmp(argv[3], "|") == 0 || strcmp(argv[3], "or") == 0 || strcmp(argv[3], "OR") == 0)
	{
		strcpy(operator, "or");
	}

	else if(strcmp(argv[3], "^") == 0 || strcmp(argv[3], "xor") == 0 ||strcmp(argv[3], "XOR") == 0)
	{
		strcpy(operator, "xor");
	}

	else 
	{
		printf("No valid operator given!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	
	/*close files*/
	fclose(fp1);
	fclose(fp2);

/*TODO write to a file*/
	
	return 0;
}

uint32_t swapbytes32 (uint32_t a)
{
	a = ((a & 0x000000FF) << 24) |
		((a & 0x0000FF00) << 8) |
		((a & 0x00FF0000) >> 8) |
		((a & 0xFF000000) >> 24);
	return a;
} 

uint16_t swapbytes16 (uint16_t a)
{
	a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);
	return a;
} 


