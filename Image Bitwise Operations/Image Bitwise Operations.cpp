#include<stdio.h>
#include<malloc.h>
#include<memory.h>
#include<conio.h>
#pragma warning(disable:4996)

struct Bitmap
{
	int* BitmapFileHeaderSize;
	unsigned char* BitmapFileHeader;
	unsigned char* BitmapFileData;
	unsigned short int* IsValidBitmapFile;
	int* BitmapSize;
};

Bitmap* ReadBitmapFile(const char* BitmapFileName);
Bitmap* XorBitmaps(Bitmap* FirstBitmapStruct, Bitmap* SecondBitmapStruct);
Bitmap* AndBitmaps(Bitmap* FirstBitmapStruct, Bitmap* SecondBitmapStruct);
Bitmap* OrBitmaps(Bitmap* FirstBitmapStruct, Bitmap* SecondBitmapStruct);
void WriteBitmapFile(const char* BitmapFileName, Bitmap* BitmapStruct);
void DeleteBitmapStruct(Bitmap* &BitmapStruct);

Bitmap* ReadBitmapFile(const char* BitmapFileName)
{
	Bitmap* BitmapStruct = NULL;
	FILE* f = fopen(BitmapFileName, "rb");
	if (f)
	{
		BitmapStruct = (Bitmap*)malloc(sizeof(Bitmap));
		BitmapStruct->BitmapFileHeaderSize = NULL;
		BitmapStruct->BitmapFileHeader = NULL;
		BitmapStruct->BitmapFileData = NULL;
		BitmapStruct->IsValidBitmapFile = NULL;
		BitmapStruct->BitmapSize = NULL;
		BitmapStruct->BitmapFileHeaderSize = (int*)malloc(sizeof(int*) * 1);
		*(BitmapStruct->BitmapFileHeaderSize) = 54;
		BitmapStruct->BitmapFileHeader = (unsigned char*)malloc(sizeof(unsigned char) * *(BitmapStruct->BitmapFileHeaderSize));
		fread(BitmapStruct->BitmapFileHeader, sizeof(unsigned char), *(BitmapStruct->BitmapFileHeaderSize), f);
		int PictureWidth = *(int*)&BitmapStruct->BitmapFileHeader[18];
		int PictureHeigth = *(int*)&BitmapStruct->BitmapFileHeader[22];
		BitmapStruct->BitmapSize = (int*)malloc(sizeof(int) * 1);
		*(BitmapStruct->BitmapSize) = 3 * PictureWidth * PictureHeigth;
		int BitsPerPixel = *(int*)&BitmapStruct->BitmapFileHeader[28];
		unsigned char BitmapSignature[2];
		BitmapSignature[0] = *(unsigned char*)&BitmapStruct->BitmapFileHeader[0];
		BitmapSignature[1] = *(unsigned char*)&BitmapStruct->BitmapFileHeader[1];
		if ((memicmp(BitmapSignature, "BM", 2) == 0) && (BitsPerPixel == 24))
		{
			BitmapStruct->IsValidBitmapFile = (unsigned short int*)malloc(sizeof(unsigned short int) * 1);
			*(BitmapStruct->IsValidBitmapFile) = 1;
			BitmapStruct->BitmapFileData = (unsigned char*)malloc(sizeof(unsigned char) * *(BitmapStruct->BitmapSize));
			fread(BitmapStruct->BitmapFileData, sizeof(unsigned char), *(BitmapStruct->BitmapSize), f);
		}
		else
		{
			printf("File: %s Is Not A Valid 24 Bits Per Pixel Bitmap!\n", BitmapFileName);
			DeleteBitmapStruct(BitmapStruct);
		}
		fclose(f);
		return BitmapStruct;
	}
	else
	{
		printf("Unable To Read Bitmap File: %s!\n", BitmapFileName);
		return BitmapStruct;
	}
}

Bitmap* XorBitmaps(Bitmap* FirstBitmapStruct, Bitmap* SecondBitmapStruct)
{
	Bitmap* XorBitmapStruct = (Bitmap*)malloc(sizeof(Bitmap));
	XorBitmapStruct->BitmapFileHeaderSize = (int*)malloc(sizeof(int) * 1);
	*(XorBitmapStruct->BitmapFileHeaderSize) = *(int*)FirstBitmapStruct->BitmapFileHeaderSize;
	XorBitmapStruct->BitmapFileHeader = (unsigned char*)malloc(sizeof(unsigned char) * *(FirstBitmapStruct->BitmapFileHeaderSize));
	memcpy(XorBitmapStruct->BitmapFileHeader, FirstBitmapStruct->BitmapFileHeader, *(FirstBitmapStruct->BitmapFileHeaderSize));
	XorBitmapStruct->BitmapFileData = (unsigned char*)malloc(sizeof(unsigned char) * *(FirstBitmapStruct->BitmapSize));
	XorBitmapStruct->IsValidBitmapFile = (unsigned short int*)malloc(sizeof(unsigned short int) * 1);
	*(XorBitmapStruct->IsValidBitmapFile) = *(unsigned short int*)FirstBitmapStruct->IsValidBitmapFile;
	XorBitmapStruct->BitmapSize = (int*)malloc(sizeof(int) * 1);
	*(XorBitmapStruct->BitmapSize) = *(int*)FirstBitmapStruct->BitmapSize;
	for (int i = 0; i < *(FirstBitmapStruct->BitmapSize); i += 3)
	{
		unsigned char Gray = (FirstBitmapStruct->BitmapFileData[i] + FirstBitmapStruct->BitmapFileData[i + 1] + FirstBitmapStruct->BitmapFileData[i + 2]) / 3;
		XorBitmapStruct->BitmapFileData[i] = Gray;
		XorBitmapStruct->BitmapFileData[i + 1] = Gray;
		XorBitmapStruct->BitmapFileData[i + 2] = Gray;
	}
	for (int i = 0; i < *(XorBitmapStruct->BitmapSize); i += 3)
	{
		XorBitmapStruct->BitmapFileData[i] = ((XorBitmapStruct->BitmapFileData[i]) ^ (SecondBitmapStruct->BitmapFileData[i]));
		if ((XorBitmapStruct->BitmapFileData[i] > 0) & (XorBitmapStruct->BitmapFileData[i] < 255))
		{
			XorBitmapStruct->BitmapFileData[i] = 0;
		}
		XorBitmapStruct->BitmapFileData[i + 1] = ((XorBitmapStruct->BitmapFileData[i + 1]) ^ (SecondBitmapStruct->BitmapFileData[i + 1]));
		if ((XorBitmapStruct->BitmapFileData[i + 1] > 0) & (XorBitmapStruct->BitmapFileData[i + 1] < 255))
		{
			XorBitmapStruct->BitmapFileData[i + 1] = 0;
		}
		XorBitmapStruct->BitmapFileData[i + 2] = ((XorBitmapStruct->BitmapFileData[i + 2]) ^ (SecondBitmapStruct->BitmapFileData[i + 2]));
		if ((XorBitmapStruct->BitmapFileData[i + 2] > 0) & (XorBitmapStruct->BitmapFileData[i + 2] < 255))
		{
			XorBitmapStruct->BitmapFileData[i + 2] = 0;
		}
	}
	return XorBitmapStruct;
}

Bitmap* AndBitmaps(Bitmap* FirstBitmapStruct, Bitmap* SecondBitmapStruct)
{
	Bitmap* AndBitmapStruct = (Bitmap*)malloc(sizeof(Bitmap));
	AndBitmapStruct->BitmapFileHeaderSize = (int*)malloc(sizeof(int) * 1);
	*(AndBitmapStruct->BitmapFileHeaderSize) = *(int*)FirstBitmapStruct->BitmapFileHeaderSize;
	AndBitmapStruct->BitmapFileHeader = (unsigned char*)malloc(sizeof(unsigned char) * *(FirstBitmapStruct->BitmapFileHeaderSize));
	memcpy(AndBitmapStruct->BitmapFileHeader, FirstBitmapStruct->BitmapFileHeader, *(FirstBitmapStruct->BitmapFileHeaderSize));
	AndBitmapStruct->BitmapFileData = (unsigned char*)malloc(sizeof(unsigned char) * *(FirstBitmapStruct->BitmapSize));
	AndBitmapStruct->IsValidBitmapFile = (unsigned short int*)malloc(sizeof(unsigned short int) * 1);
	*(AndBitmapStruct->IsValidBitmapFile) = *(unsigned short int*)FirstBitmapStruct->IsValidBitmapFile;
	AndBitmapStruct->BitmapSize = (int*)malloc(sizeof(int) * 1);
	*(AndBitmapStruct->BitmapSize) = *(int*)FirstBitmapStruct->BitmapSize;
	for (int i = 0; i < *(FirstBitmapStruct->BitmapSize); i += 3)
	{
		AndBitmapStruct->BitmapFileData[i] = ((FirstBitmapStruct->BitmapFileData[i] & SecondBitmapStruct->BitmapFileData[i]));
		AndBitmapStruct->BitmapFileData[i + 1] = ((FirstBitmapStruct->BitmapFileData[i + 1] & SecondBitmapStruct->BitmapFileData[i + 1]));
		AndBitmapStruct->BitmapFileData[i + 2] = ((FirstBitmapStruct->BitmapFileData[i + 2] & SecondBitmapStruct->BitmapFileData[i + 2]));
	}
	return AndBitmapStruct;
}

Bitmap* OrBitmaps(Bitmap* FirstBitmapStruct, Bitmap* SecondBitmapStruct)
{
	Bitmap* OrBitmapStruct = (Bitmap*)malloc(sizeof(Bitmap));
	OrBitmapStruct->BitmapFileHeaderSize = (int*)malloc(sizeof(int) * 1);
	*(OrBitmapStruct->BitmapFileHeaderSize) = *(int*)FirstBitmapStruct->BitmapFileHeaderSize;
	OrBitmapStruct->BitmapFileHeader = (unsigned char*)malloc(sizeof(unsigned char) * *(FirstBitmapStruct->BitmapFileHeaderSize));
	memcpy(OrBitmapStruct->BitmapFileHeader, FirstBitmapStruct->BitmapFileHeader, *(FirstBitmapStruct->BitmapFileHeaderSize));
	OrBitmapStruct->BitmapFileData = (unsigned char*)malloc(sizeof(unsigned char) * *(FirstBitmapStruct->BitmapSize));
	OrBitmapStruct->IsValidBitmapFile = (unsigned short int*)malloc(sizeof(unsigned short int) * 1);
	*(OrBitmapStruct->IsValidBitmapFile) = *(unsigned short int*)FirstBitmapStruct->IsValidBitmapFile;
	OrBitmapStruct->BitmapSize = (int*)malloc(sizeof(int) * 1);
	*(OrBitmapStruct->BitmapSize) = *(int*)FirstBitmapStruct->BitmapSize;
	for (int i = 0; i < *(FirstBitmapStruct->BitmapSize); i += 3)
	{
		OrBitmapStruct->BitmapFileData[i] = ((FirstBitmapStruct->BitmapFileData[i] | SecondBitmapStruct->BitmapFileData[i]));
		OrBitmapStruct->BitmapFileData[i + 1] = ((FirstBitmapStruct->BitmapFileData[i + 1] | SecondBitmapStruct->BitmapFileData[i + 1]));
		OrBitmapStruct->BitmapFileData[i + 2] = ((FirstBitmapStruct->BitmapFileData[i + 2] | SecondBitmapStruct->BitmapFileData[i + 2]));
	}
	return OrBitmapStruct;
}

void WriteBitmapFile(const char* BitmapFileName, Bitmap* BitmapStruct)
{
	FILE* f = fopen(BitmapFileName, "wb");
	if (f)
	{
		fwrite(BitmapStruct->BitmapFileHeader, sizeof(unsigned char), *(BitmapStruct->BitmapFileHeaderSize), f);
		fwrite(BitmapStruct->BitmapFileData, sizeof(unsigned char), *(BitmapStruct->BitmapSize), f);
		fclose(f);
	}
	else
	{
		printf("Unable To Save Bitmap File: %s!\n", BitmapFileName);
	}
}

void DeleteBitmapStruct(Bitmap* &BitmapStruct)
{
	if (BitmapStruct != NULL)
	{
		if (BitmapStruct->BitmapFileHeaderSize != NULL)
		{
			free(BitmapStruct->BitmapFileHeaderSize);
			BitmapStruct->BitmapFileHeaderSize = NULL;
		}
		if (BitmapStruct->BitmapFileHeader != NULL)
		{
			free(BitmapStruct->BitmapFileHeader);
			BitmapStruct->BitmapFileHeader = NULL;
		}
		if (BitmapStruct->BitmapFileData != NULL)
		{
			free(BitmapStruct->BitmapFileData);
			BitmapStruct->BitmapFileData = NULL;
		}
		if (BitmapStruct->IsValidBitmapFile != NULL)
		{
			free(BitmapStruct->IsValidBitmapFile);
			BitmapStruct->IsValidBitmapFile = NULL;
		}
		if (BitmapStruct->BitmapSize != NULL)
		{
			free(BitmapStruct->BitmapSize);
			BitmapStruct->BitmapSize = NULL;
		}
		free(BitmapStruct);
		BitmapStruct = NULL;
	}
}

void main()
{
	const char* FirstBitmapFileName = (const char*)"FirstBitmap.bmp";
	const char* SecondBitmapFileName = (const char*)"SecondBitmap.bmp";
	const char* ThirdBitmapFileName = (const char*)"ThirdBitmap.bmp";
	const char* XorBitmapFileName = (const char*)"1.XorBitmaps.bmp";
	const char* AndBitmapFileName = (const char*)"2.AndBitmaps.bmp";
	const char* OrBitmapFileName = (const char*)"3.OrBitmaps.bmp";

	Bitmap* FirstBitmapStruct = ReadBitmapFile(FirstBitmapFileName);
	Bitmap* SecondbitmapStruct = ReadBitmapFile(SecondBitmapFileName);
	Bitmap* ThirdBitmapStruct = NULL;
	Bitmap* XorBitmapStruct = NULL;
	Bitmap* AndBitmapStruct = NULL;
	Bitmap* OrBitmapStruct = NULL;

	if (FirstBitmapStruct != NULL && SecondbitmapStruct != NULL)
	{
		if (*(FirstBitmapStruct->BitmapSize) == *(SecondbitmapStruct->BitmapSize))
		{
			// 1. Xor FirstBitmap.bmp with SecondBitmap.bmp => 1.XorBitmaps.bmp
			XorBitmapStruct = XorBitmaps(FirstBitmapStruct, SecondbitmapStruct);
			WriteBitmapFile(XorBitmapFileName, XorBitmapStruct);
			DeleteBitmapStruct(FirstBitmapStruct);
			DeleteBitmapStruct(SecondbitmapStruct);
			DeleteBitmapStruct(XorBitmapStruct);
			printf("XOR(^) %s With %s => %s\n", FirstBitmapFileName, SecondBitmapFileName, XorBitmapFileName);
		}
		else
		{
			printf("Bitmaps: %s And %s Must Have Same Size!\n", FirstBitmapFileName, SecondBitmapFileName);
		}
	}

	XorBitmapStruct = ReadBitmapFile(XorBitmapFileName);
	ThirdBitmapStruct = ReadBitmapFile(ThirdBitmapFileName);
	if (XorBitmapStruct != NULL && ThirdBitmapStruct != NULL)
	{
		if (*(XorBitmapStruct->BitmapSize) == *(ThirdBitmapStruct->BitmapSize))
		{
			// 2. And 1.XorBitmaps.bmp with ThirdBitmap.bmp => 2.AndBitmaps.bmp
			AndBitmapStruct = AndBitmaps(XorBitmapStruct, ThirdBitmapStruct);
			WriteBitmapFile(AndBitmapFileName, AndBitmapStruct);
			DeleteBitmapStruct(AndBitmapStruct);
			DeleteBitmapStruct(XorBitmapStruct);
			DeleteBitmapStruct(ThirdBitmapStruct);
			printf("AND(&) %s With %s => %s\n", XorBitmapFileName, ThirdBitmapFileName, AndBitmapFileName);
		}
		else
		{
			printf("Bitmaps: %s And %s Must Have Same Size\n", XorBitmapFileName, ThirdBitmapFileName);
		}
	}

	AndBitmapStruct = ReadBitmapFile(AndBitmapFileName);
	FirstBitmapStruct = ReadBitmapFile(FirstBitmapFileName);
	if (AndBitmapStruct != NULL && FirstBitmapStruct != NULL)
	{
		if (*(AndBitmapStruct->BitmapSize) == *(FirstBitmapStruct->BitmapSize))
		{
			// 3. Or 2.AndBitmaps.bmp With FirstBitmap.bmp => 3.OrBitmaps.bmp
			OrBitmapStruct = OrBitmaps(AndBitmapStruct, FirstBitmapStruct);
			WriteBitmapFile(OrBitmapFileName, OrBitmapStruct);
			DeleteBitmapStruct(OrBitmapStruct);
			DeleteBitmapStruct(AndBitmapStruct);
			DeleteBitmapStruct(FirstBitmapStruct);
			printf("OR(|) %s With %s => %s\n", AndBitmapFileName, FirstBitmapFileName, OrBitmapFileName);
			printf("Done! Press Any Key To Exit\n");
		}
		else
		{
			printf("Bitmaps: %s And %s Must Have Same Size\n", AndBitmapFileName, FirstBitmapFileName);
		}
	}

	DeleteBitmapStruct(FirstBitmapStruct);
	DeleteBitmapStruct(SecondbitmapStruct);
	DeleteBitmapStruct(ThirdBitmapStruct);
	DeleteBitmapStruct(XorBitmapStruct);
	DeleteBitmapStruct(AndBitmapStruct);
	DeleteBitmapStruct(OrBitmapStruct);
	_getch();
}