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

unsigned short buffer[500][500];

void fill_buffer()
{
	unsigned short temp_data = 0;
	for (int i =0;i < 500; i++)
	{
		for (int j =0 ;j < 500;j++)
		{
			buffer[i][j] = temp_data;
		}
		temp_data += 65535/ 500;
	}
}




int main()

{
	//fill_buffer();
	unsigned char * img = NULL;
	img = (unsigned char *)malloc(3 * 500 * 500);
	memset(img, 0, 3 * 500 * 500);



	std::vector<std::string> string_buffer;
	unsigned char bmppad[3] = { 0,0,0 };


	//unsigned short buffer[1250][1250];

	unsigned short temp_data = 0;

	for (int i = 0; i < 500; i++)

	{

		for (int j = 0; j < 500; j++)

		{

			buffer[i][j] = temp_data;

		}

		temp_data += 65535 / 500;

	}

	for (unsigned short *ptr = (unsigned short*)buffer + 500 * 500; ptr>(unsigned short*)buffer; )
	{
		const unsigned short val = *(--ptr);
		*ptr = (unsigned short)((val >> 8) | ((val << 8)));
	}


	//auto img = buffer;// hold_dude[1250];


	//Invert endianness before getting the "RGB" buffer[0][i]s



	//Get RGB Color of every pixle ??? // Since we are building a grayScale image the RGB values need to be the same? no need to avg?
	int count = 0;
	for (int i = 0; i < 500; i++)

	{

		for (int j = 0; j < 500; j++)

		{
			img[count] = buffer[i][j]; count++;
			img[count] = buffer[i][j]; count++;
			img[count] = buffer[i][j]; count++;

		}

	}

	for (int i = 0; i<500; i++)
	{
		string_buffer.push_back(reinterpret_cast<char *>(img + (500*(500 - i - 1) * 3)));
		string_buffer.push_back(reinterpret_cast<char *>(bmppad));
	}

	std::string image_data;
	//Return the buffer in a string
	for (auto const& elements : string_buffer)
	{
		image_data += elements;
	}

	auto pass_data = reinterpret_cast<unsigned char*>(const_cast< char*>(image_data.c_str()));

	//cimg_library::CImg<uint16_t> img(&buffer[0][0],1250,1250);
	//img.invert_endianness();
	//img.save_bmp("test.bmp");
	HBITMAP hold_image = (HBITMAP)LoadImage(NULL, "test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HAVI avi = CreateAvi("test.avi", 500, NULL);
	AddAviFrame(avi,hold_image, pass_data);
	//AddAviFrame(avi, hold_image); 
	//AddAviFrame(avi, hold_image); AddAviFrame(avi, hold_image); AddAviFrame(avi, hold_image); AddAviFrame(avi, hold_image); AddAviFrame(avi, hold_image); AddAviFrame(avi, hold_image);
	DeleteObject(hold_image);
	CloseAvi(avi);
	////BITMAPINFO *bmi=NULL;
	//struct
	//{
	//	BITMAPINFOHEADER bmiHeader;
	//	RGBQUAD bmiColors[256];
	//}bmi;
	//memset(&bmi,0,sizeof(bmi));
	//bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//dib = GetDIBits(hdc,hold_image,0,0,NULL,(BITMAPINFO*)&bmi,0);





	return 0;
}