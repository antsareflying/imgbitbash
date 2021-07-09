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
#ifndef IBB_H
#define IBB_H

uint32_t swapbytes32(uint32_t a);
uint16_t swapbytes16(uint16_t a);
int checkendian(void);
void print_help(void);

typedef struct BITMAPFILEHEADER
{
	uint16_t bmsigbytes;
	uint32_t filesize;
	uint32_t reserved;
	uint32_t dataoffset;
}__attribute__((packed)) BITMAPFILEHEADER_T;

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
}__attribute__((packed)) BITMAPINFOHEADER_T;

typedef struct BITMAPHEADER
{
	BITMAPFILEHEADER_T file;
	BITMAPINFOHEADER_T info;
}__attribute__((packed)) BITMAPHEADER_T;

#endif
