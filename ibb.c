/*
Copyright (c) 2021 Moon Seongmu

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

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
int checkendian(void);
void print_help(void);

typedef struct BITMAPFILEHEADER
{
	uint16_t bmsigbytes;
	uint32_t filesize;
	uint32_t reserved;
	uint32_t dataoffset;
} __attribute__((packed)) BITMAPFILEHEADER_T ;

typedef struct BITMAPINFOHEADER
{
	uint32_t headersize;
	int32_t width;
	int32_t height;
	uint16_t colorplanes;
	uint16_t bitsperpixel;
	uint32_t compression;
	uint32_t imagesize;
	int32_t horizres;
	int32_t vertres;
	uint32_t colorpalatte;
	uint32_t impcolors;
} __attribute__((packed)) BITMAPINFOHEADER_T ;

typedef struct BITMAPHEADER
{
	BITMAPFILEHEADER_T file;
	BITMAPINFOHEADER_T info;
} __attribute__((packed)) BITMAPHEADER_T;


int main(int argc, char *argv[])
{
	/*looks for 3 command line arguments.*/
	
	if (argc != 4) 
	{
		printf("Only input 3 command line arguments!\n\n");
		print_help();
		return 1;
	}

	/*Read operator from cmd line arguments*/
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
		print_help();
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

	/*read header into structs*/
	BITMAPHEADER_T imgheader1, imgheader2;


	fread(&imgheader1, 1, sizeof(imgheader1), fp1);
	fread(&imgheader2, sizeof(imgheader2), 1, fp2);

	/*check if bitmap signature present*/

	if (imgheader1.file.bmsigbytes != 0x4D42)
	{
		printf("The first file is not a .BMP file\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}


	if (imgheader2.file.bmsigbytes != 0x4D42)
	{
		printf("The second file is not a .BMP file\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}


	/*check that both img is using BITMAPINFOHEADER*/
	if(checkendian() == 2)
	{
		swapbytes32(imgheader1.info.headersize);
		swapbytes32(imgheader2.info.headersize);
	}
	
	if (imgheader1.info.headersize != 40)
	{
		printf("The first file does not use BITMAPINFOHEADER\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}


	
	if (imgheader2.info.headersize != 40)
	{
		printf("The second file does not use BITMAPINFOHEADER\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}
	/*TODO check for other headers*/
	
	if(checkendian() == 2)
	{
	/*find height of images*/
		swapbytes32(imgheader1.info.height);
		swapbytes32(imgheader2.info.height);
	/*find width of images*/
		swapbytes32(imgheader1.info.width);
		swapbytes32(imgheader2.info.width);
	}

	/*check that both images use 24 bit colour*/
	if (checkendian() == 2)
	{
		swapbytes16(imgheader1.info.bitsperpixel);
	}
	if(imgheader1.info.bitsperpixel != 24)
	{
		printf("First image is not using a 24 bit color!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	if(checkendian() == 2)
	{
		swapbytes16(imgheader2.info.bitsperpixel);
	}

	if(imgheader2.info.bitsperpixel != 24)
	{
		printf("Second image is not using a 24 bit color!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	/*check that both images dont use compression*/
	if(checkendian() == 2)
	{
		swapbytes16(imgheader1.info.compression);
	}
		
	if(imgheader1.info.compression != 0)
	{
		printf("First image is using compression!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	if(checkendian() == 2)
	{
		swapbytes16(imgheader2.info.compression);
	}
		
	if(imgheader2.info.compression != 0)
	{
		printf("Second image is using compression!\n");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	/*get data offset for both files*/
	if(checkendian() == 2)
	{
		swapbytes32(imgheader1.file.dataoffset);

		swapbytes32(imgheader2.file.dataoffset);
	}


	/*find no of padding bytes*/
	int32_t imgwd1padding, imgwd2padding;
	if(((imgheader1.info.width*3)%4) != 0)
	{
		imgwd1padding = (4 - ((imgheader1.info.width * 3) % 4));	
	}
	else
	{
		imgwd1padding = 0;
	}

	if((imgheader1.info.width%4) != 0)
	{
		imgwd2padding = (4 - ((imgheader1.info.width * 3) % 4));	
	}
	else
	{
		imgwd2padding = 0;
	}


	BITMAPHEADER_T imgheader3;
	/*set imght3 & imgwd3 to be smaller of the two*/

	int imgwd3padding;

	if(imgheader1.info.height <= imgheader2.info.height)
	{
		imgheader3.info.height = imgheader1.info.height;
	}
	else
	{
		imgheader3.info.height = imgheader2.info.height;
	}

	if(imgheader1.info.width <= imgheader2.info.width)
	{
		imgheader3.info.width = imgheader1.info.width;
		imgwd3padding = imgwd1padding;
	}
	else
	{
		imgheader3.info.width = imgheader2.info.width;
		imgwd3padding = imgwd2padding;
	}
	
	
	/*read images into memory array*/
	uint8_t *img1arr = (uint8_t *)malloc(imgheader1.info.width * 3 * imgheader1.info.height);
	if(img1arr == NULL)
	{
		printf("Memory allocation failed");
		fclose(fp1);
		fclose(fp2);
		return 1;
	}
	
	fseek(fp1, imgheader1.file.dataoffset, SEEK_SET);

	for(int i = 0; i < imgheader3.info.height; i++)
	{
		fread(img1arr, 1, imgheader3.info.width * 3, fp1);
		fseek(fp1,(imgheader1.info.width - imgheader3.info.width + imgwd1padding) * 3, SEEK_CUR);
	}

	
	uint8_t *img2arr = (uint8_t *)malloc(imgheader3.info.width * 3 * imgheader3.info.height);
	if(img2arr == NULL)
	{
		printf("Memory allocation failed");
		free(img1arr);
		fclose(fp1);
		fclose(fp2);
		return 1;
	}
	
	fseek(fp2, imgheader2.file.dataoffset, SEEK_SET);

	for(int i = 0; i < imgheader3.info.height; i++)
	{
		fread(img2arr, 1, imgheader3.info.width * 3, fp2);
		fseek(fp2, (imgheader2.info.width - imgheader3.info.width + imgwd2padding) * 3, SEEK_CUR);
	}



	/*size of padded imgwd3padded*/
	int32_t imgwd3padded = 0;

	imgwd3padded = imgheader3.info.width + imgwd3padding;


	/*allocate memory for img3*/
	uint8_t *img3arr = (uint8_t *)malloc(imgwd3padded * 3 * imgheader3.info.height);
	if (img3arr == NULL)
	{
		printf("Memory allocation failed");
		free(img1arr);
		free(img2arr);
		fclose(fp1);
		fclose(fp2);
		return 1;
	}

	memset(img3arr, 1, imgwd3padded * 3 * imgheader3.info.height);

	/* bashing the bits together*/
	
	for (int32_t i = 0; i < imgheader3.info.height; i++)
	{
		
		for (int32_t j = 0; j < imgheader3.info.width * 3; j++)
		{
			if(!strcmp(operator, "and"))
			{
				img3arr[(i * imgheader3.info.width*3) + j] = 
				img1arr[(i * imgheader3.info.width*3) + j] & 
				img2arr[(i * imgheader3.info.width*3) + j];
			}

			if(!strcmp(operator, "or"))
			{
				img3arr[(i * imgheader3.info.width*3) + j] = 
				img1arr[(i * imgheader3.info.width*3) + j] | 
				img2arr[(i * imgheader3.info.width*3) + j];
			}

			if(!strcmp(operator, "xor"))
			{
				img3arr[(i * imgheader3.info.width*3) + j] = 
				img1arr[(i * imgheader3.info.width*3) + j] ^ 
				img2arr[(i * imgheader3.info.width*3) + j];
			}
		}
	}
	
	free(img1arr);
	free(img2arr);


	/*writing new image to file*/

	FILE *fp3 = fopen("output.bmp", "wb");
	imgheader3.file.bmsigbytes = 0x4D42;
	imgheader3.file.filesize =(54) + (imgwd3padded * 3 * imgheader3.info.height);
	imgheader3.file.reserved = 0;
	imgheader3.info.width = imgwd3padded;
	imgheader3.file.dataoffset = 54;
	imgheader3.info.headersize = 40;
	imgheader3.info.colorplanes = 1;	
	imgheader3.info.bitsperpixel = 24;
	imgheader3.info.compression = 0;
	imgheader3.info.imagesize = (imgheader3.info.height * (imgwd3padded * 3));
	imgheader3.info.horizres = 0;
	imgheader3.info.vertres = 0;
	imgheader3.info.colorpalatte = 0;
	imgheader3.info.impcolors = 0;


	if (checkendian() == 2)
	{
		swapbytes32(imgheader3.file.filesize);
		swapbytes32(imgheader3.file.dataoffset);
		swapbytes32(imgheader3.info.headersize);
		swapbytes32(imgheader3.info.width);
		swapbytes32(imgheader3.info.height);
		swapbytes16(imgheader3.info.colorplanes);
		swapbytes16(imgheader3.info.bitsperpixel);
		swapbytes32(imgheader3.info.imagesize);
	}

	fwrite(&imgheader3, sizeof(imgheader3), 1, fp3);
	
	fwrite(img3arr, 1, ((imgwd3padded * 3) * imgheader3.info.height), fp3);


	/*cleanup*/
	//fclose(fp3);
	free(img3arr);
	fclose(fp1);
	fclose(fp2);

	
	return 0;
}

uint32_t swapbytes32(uint32_t a)
{
	a = ((a & 0x000000FF) << 24) |
		((a & 0x0000FF00) << 8) |
		((a & 0x00FF0000) >> 8) |
		((a & 0xFF000000) >> 24);
	return a;
} 

uint16_t swapbytes16(uint16_t a)
{
	a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);
	return a;
} 

int checkendian(void)
{
	int num = 1;
	if(*(char *)&num == 1)
	{
		return 1;  //little
	}
	else
	{
		return 2;  //big
	}
}

void print_help(void)
{
	printf("IBB: Image Bit Bashing\n"
	"Usage: ibb [image1.bmp] [image2.bmp] [operator]\n"
	"Opertators:\nXOR, xor, ^\nOR, or, |\nAND, and, &\n");
	
}
