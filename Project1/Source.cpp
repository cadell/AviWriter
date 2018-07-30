//#include "stdint.h"
#pragma comment(linker, "/STACK:6000000")
#pragma comment(linker, "/HEAP:6000000")

#include "windows.h"
#pragma comment (lib,"Gdiplus.lib")
#include "gdiplus.h"
#include "iostream"
#include "vector"
#include "fstream"
#include "string"
#include "vfw.h"
#include "avi_utils.h"
#include "CImg.h"
#include <assert.h>
#include <algorithm>
#include <iterator>

using namespace Gdiplus;
using namespace std;

unsigned short buffer[1000][1000]; //M by N ..rows and cols

int main()

{
	//fill_buffer();
	int h = 1000;
	int w = 1000;
	auto padding = (4 - (3 * w) % 4) % 4;
	unsigned char * img = NULL;
	img = (unsigned char *)malloc((3 * w+padding) * h);
	memset(img, 0, (3 * w + padding) * h);



	std::vector<std::string> string_buffer;
	//unsigned char bmppad[4] = { 0,0,0 ,0};


	//unsigned short buffer[1250][1250];

	unsigned short temp_data = 0;

	for (int i = 0; i < h; i++)

	{
		for (int j = 0; j < w; j++)
		{
			buffer[i][j] = temp_data;
		}
		temp_data += 65535 / w;
	}

	for (unsigned short *ptr = (unsigned short*)buffer + h * w; ptr>(unsigned short*)buffer; )
	{
		const unsigned short val = *(--ptr);
		*ptr = (unsigned short)((val >> 8) | ((val << 8)));
	}

	//for (int i =0;i <h;i++)
	//{
	//	for (int j = i+1;j < w;j++)
	//	{
	//		std::swap(buffer[i][j],buffer[j][i]);
	//	}
	//}

	//Uncompressed images need to be rotated 180 degress because windows api is broken??? negative biHeight not working.
	//This will probably break if the codec is different.
	for (int i = 0; i < h/2; i++)
	{
		for (int j = 0; j < w; j++)
		{
			std::swap(buffer[i][j],buffer[h-i-1][w-j-1]);
		}
	}
	if (h & 1)
	{
		for (int i =0; i < w/2; i++)
		{
			std::swap(buffer[h/2][i],buffer[h/2][w-i-1]);
		}
	}

	//auto img = buffer;// hold_dude[1250];


	//Invert endianness before getting the "RGB" buffer[0][i]s



	//Get RGB Color of every pixle ??? // Since we are building a grayScale image the RGB values need to be the same? no need to avg?
	int count = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			img[count] = buffer[i][j]; count++; // R
			img[count] = buffer[i][j]; count++; // G
			img[count] = buffer[i][j]; count++; // B
		}
		for (size_t i = 0; i < padding; i++)
		{
			img[count] = 0x00; count++;
		}
	}


	//std::reverse(std::begin(img),std::end(img));
	std::string image_data;
	//Return the buffer in a string
	for (auto const& elements : string_buffer)
	{
		image_data += elements;
	}

	
	HAVI avi = CreateAvi("test.avi", 500, NULL);
	AddAviFrame(avi, img);
	CloseAvi(avi);
	return 0;
}