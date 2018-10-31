#include<stdio.h>
#include<malloc.h>
#include<memory.h>
#include<conio.h>
#include<string.h>
#pragma warning(disable:4996)

struct File
{
	unsigned int* FileNameLength;
	char* FileName;
	unsigned long long* FileSize;
	unsigned char* FileData;
};

struct DigitalContainer
{
	unsigned int* ContainerSize;
	unsigned int* ContainerIndex;
	File** FilesArray;
};

File* CreateFile();
File* DeepCopyFile(File* FileStruct);
File* ReadFile(const char* FileName);
void WriteFile(const char* FileName, File* FileStruct);
void DeleteStruct(File* &FileStruct);
DigitalContainer* CreateDigitalContainer(unsigned int ContainerSize);
void CopyContainerInContainer(DigitalContainer* &DigitalContainerStructDestination, DigitalContainer* DigitalContainerDtructSource);
void InsertFileIntoDigitalContainer(DigitalContainer* &DigitalContainerStruct, File* FileStruct);
void EncodeDigitalContainer(const char* DigitalContainerFileName, DigitalContainer* DigitalContainerStruct);
DigitalContainer* DecodeDigitalContainer(const char* DigitalContainerName);
void ExtractFilesFromDigitalContainer(DigitalContainer* DigitalContainerStruct);
void DeleteDigitalContainer(DigitalContainer* &DigitalContainerStruct);

File* CreateFile()
{
	File* FileStruct = (File*)malloc(sizeof(File) * 1);
	FileStruct->FileNameLength = NULL;
	FileStruct->FileName = NULL;
	FileStruct->FileSize = NULL;
	FileStruct->FileData = NULL;
	return FileStruct;
}

DigitalContainer* CreateDigitalContainer(unsigned int ContainerSize)
{
	DigitalContainer* DigitalContainerStruct = (DigitalContainer*)malloc(sizeof(DigitalContainer) * 1);
	DigitalContainerStruct->ContainerIndex = (unsigned int*)malloc(sizeof(unsigned int) * 1);
	*(DigitalContainerStruct->ContainerIndex) = 0;
	DigitalContainerStruct->ContainerSize = (unsigned int*)malloc(sizeof(unsigned int) * 1);
	*(DigitalContainerStruct->ContainerSize) = ContainerSize;
	DigitalContainerStruct->FilesArray = (File**)malloc(sizeof(File*) * *(DigitalContainerStruct->ContainerSize));
	for (unsigned int i = 0; i < *(DigitalContainerStruct->ContainerSize); i++)
	{
		DigitalContainerStruct->FilesArray[i] = CreateFile();
	}
	return DigitalContainerStruct;
}

void DeleteStruct(File* &FileStruct)
{
	if (FileStruct != NULL)
	{
		if (FileStruct->FileNameLength != NULL)
		{
			free(FileStruct->FileNameLength);
			FileStruct->FileNameLength = NULL;
		}
		if (FileStruct->FileName != NULL)
		{
			free(FileStruct->FileName);
			FileStruct->FileName = NULL;
		}
		if (FileStruct->FileSize != NULL)
		{
			free(FileStruct->FileSize);
			FileStruct->FileSize = NULL;
		}
		if (FileStruct->FileData != NULL)
		{
			free(FileStruct->FileData);
			FileStruct->FileData = NULL;
		}
		free(FileStruct);
		FileStruct = NULL;
	}
}

void DeleteDigitalContainer(DigitalContainer* &DigitalContainerStruct)
{
	if (DigitalContainerStruct != NULL)
	{
		if (DigitalContainerStruct->FilesArray != NULL)
		{
			for (unsigned int i = 0; i < *(DigitalContainerStruct->ContainerSize); i++)
			{
				if (DigitalContainerStruct->FilesArray[i] != NULL)
				{
					DeleteStruct(DigitalContainerStruct->FilesArray[i]);
				}
			}
			free(DigitalContainerStruct->FilesArray);
			DigitalContainerStruct->FilesArray = NULL;
		}
		if (DigitalContainerStruct->ContainerSize != NULL)
		{
			free(DigitalContainerStruct->ContainerSize);
			DigitalContainerStruct->ContainerSize = NULL;
		}
		if (DigitalContainerStruct->ContainerIndex != NULL)
		{
			free(DigitalContainerStruct->ContainerIndex);
			DigitalContainerStruct->ContainerIndex = NULL;
		}
		free(DigitalContainerStruct);
		DigitalContainerStruct = NULL;
	}
}

void EncodeDigitalContainer(const char* DigitalContainerFileName, DigitalContainer* DigitalContainerStruct)
{
	FILE* FileStream = fopen(DigitalContainerFileName, "wb");
	if (FileStream)
	{
		fwrite(DigitalContainerStruct->ContainerIndex, sizeof(unsigned int), 1, FileStream);
		for (unsigned int i = 0; i < *(DigitalContainerStruct->ContainerIndex); i++)
		{
			fwrite(DigitalContainerStruct->FilesArray[i]->FileNameLength, sizeof(unsigned int), 1, FileStream);
			fwrite(DigitalContainerStruct->FilesArray[i]->FileName, sizeof(char), *(DigitalContainerStruct->FilesArray[i]->FileNameLength), FileStream);
			fwrite(DigitalContainerStruct->FilesArray[i]->FileSize, sizeof(unsigned long long), 1, FileStream);
			fwrite(DigitalContainerStruct->FilesArray[i]->FileData, sizeof(unsigned char), *(DigitalContainerStruct->FilesArray[i]->FileSize), FileStream);
		}
		fclose(FileStream);
	}
	else
	{
		printf("Unable To Save File: %s!\n", DigitalContainerFileName);
	}
}

DigitalContainer* DecodeDigitalContainer(const char* DigitalContainerFileName)
{
	FILE* FileStream = fopen(DigitalContainerFileName, "rb");
	DigitalContainer* DigitalContainerStruct = NULL;
	if (FileStream)
	{
		unsigned int NumberOfFiles = 0;
		fread(&NumberOfFiles, sizeof(unsigned int), 1, FileStream);
		DigitalContainerStruct = CreateDigitalContainer(NumberOfFiles);
		for (unsigned int i = 0; i < NumberOfFiles; i++)
		{
			DigitalContainerStruct->FilesArray[i]->FileNameLength = (unsigned int*)malloc(sizeof(unsigned int) * 1); //read the name length
			fread(DigitalContainerStruct->FilesArray[i]->FileNameLength, sizeof(unsigned int), 1, FileStream);
			DigitalContainerStruct->FilesArray[i]->FileName = (char*)malloc(sizeof(char) * *(DigitalContainerStruct->FilesArray[i]->FileNameLength));
			fread(DigitalContainerStruct->FilesArray[i]->FileName, sizeof(char), *(DigitalContainerStruct->FilesArray[i]->FileNameLength), FileStream); //read the file name 
			DigitalContainerStruct->FilesArray[i]->FileSize = (unsigned long long*)malloc(sizeof(unsigned long long) * 1);
			fread(DigitalContainerStruct->FilesArray[i]->FileSize, sizeof(unsigned long long), 1, FileStream);
			DigitalContainerStruct->FilesArray[i]->FileData = (unsigned char*)malloc(sizeof(unsigned char) * *(DigitalContainerStruct->FilesArray[i]->FileSize)); // read file data
			fread(DigitalContainerStruct->FilesArray[i]->FileData, sizeof(unsigned char), *(DigitalContainerStruct->FilesArray[i]->FileSize), FileStream);
			*(DigitalContainerStruct->ContainerIndex) = (*(DigitalContainerStruct->ContainerIndex) + 1);
		}
		fclose(FileStream);
		return DigitalContainerStruct;
	}
	else
	{
		printf("Unable To Read File: %s\n", DigitalContainerFileName);
		return DigitalContainerStruct;
	}
}

void CopyContainerInContainer(DigitalContainer* &DigitalContainerStructDestination, DigitalContainer* DigitalContainerStructSource)
{
	if (DigitalContainerStructSource != NULL)
	{
		if (DigitalContainerStructSource->FilesArray != NULL)
		{
			for (unsigned int i = 0; i < *(DigitalContainerStructSource->ContainerIndex); i++)
			{
				DigitalContainerStructDestination->FilesArray[i] = DeepCopyFile(DigitalContainerStructSource->FilesArray[i]);
				*(DigitalContainerStructDestination->ContainerIndex) = (*(DigitalContainerStructDestination->ContainerIndex) + 1);
			}
		}
	}
}

File* DeepCopyFile(File* FileStruct)
{
	File* Copy = CreateFile();
	Copy->FileNameLength = (unsigned int*)malloc(sizeof(unsigned int) * 1);
	*(Copy->FileNameLength) = *(unsigned int*)FileStruct->FileNameLength;
	Copy->FileName = (char*)malloc(sizeof(char) * (strlen(FileStruct->FileName) + 1));
	strcpy(Copy->FileName, FileStruct->FileName);
	Copy->FileSize = (unsigned long long*)malloc(sizeof(unsigned long long) * 1);
	*(Copy->FileSize) = *(unsigned long long*)FileStruct->FileSize;
	Copy->FileData = (unsigned char*)malloc(*(FileStruct->FileSize));
	memcpy(Copy->FileData, FileStruct->FileData, *(FileStruct->FileSize));
	return Copy;
}

void InsertFileIntoDigitalContainer(DigitalContainer* &DigitalContainerStruct, File* FileStruct)
{
	if (DigitalContainerStruct == NULL)
	{
		DigitalContainerStruct = CreateDigitalContainer(1);
	}
	if (*(DigitalContainerStruct->ContainerIndex) == *(DigitalContainerStruct->ContainerSize))
	{
		printf("Container Reallocation\n");
		DigitalContainer* Destination = CreateDigitalContainer(*(DigitalContainerStruct->ContainerSize) * 2);
		CopyContainerInContainer(Destination, DigitalContainerStruct);
		DeleteDigitalContainer(DigitalContainerStruct);
		DigitalContainerStruct = Destination;
		printf("Added File %s\n", FileStruct->FileName);
		printf("New Container Size: %d\n", *(DigitalContainerStruct->ContainerSize));
		printf("Container Size: %d\n", *(DigitalContainerStruct->ContainerSize));
		printf("Container Index: %d\n", *(DigitalContainerStruct->ContainerIndex));
	}
	DigitalContainerStruct->FilesArray[*(DigitalContainerStruct->ContainerIndex)] = DeepCopyFile(FileStruct);
	*(DigitalContainerStruct->ContainerIndex) = (*(DigitalContainerStruct->ContainerIndex) + 1);
	printf("Added File %s\n", FileStruct->FileName);
	printf("Container Size: %d\n", *(DigitalContainerStruct->ContainerSize));
	printf("Container Index: %d\n", *(DigitalContainerStruct->ContainerIndex));
}

void ExtractFilesFromDigitalContainer(DigitalContainer* DigitalContainerStruct)
{
	if (DigitalContainerStruct != NULL)
	{
		if (DigitalContainerStruct->FilesArray != NULL)
		{
			for (unsigned int i = 0; i < *(DigitalContainerStruct->ContainerIndex); i++)
			{
				WriteFile(DigitalContainerStruct->FilesArray[i]->FileName, DigitalContainerStruct->FilesArray[i]);
			}
		}
	}
}

File* ReadFile(const char* FileName)
{
	File* FileStruct = CreateFile();
	FILE* FileStream = fopen(FileName, "rb");
	if (FileStream)
	{
		fseek(FileStream, 0, SEEK_END);
		FileStruct->FileSize = (unsigned long long*)malloc(sizeof(unsigned long long) * 1);
		*(FileStruct->FileSize) = ftell(FileStream);
		fseek(FileStream, 0, SEEK_SET);
		FileStruct->FileName = (char*)malloc(sizeof(char) * (strlen(FileName) + 1));
		strcpy(FileStruct->FileName, FileName);
		FileStruct->FileNameLength = (unsigned int*)malloc(sizeof(unsigned int) * 1);
		*(FileStruct->FileNameLength) = (strlen(FileName) + 1);
		FileStruct->FileData = (unsigned char*)malloc(sizeof(unsigned char) * *(FileStruct->FileSize));
		fread(FileStruct->FileData, sizeof(unsigned char), *(FileStruct->FileSize), FileStream);
		fclose(FileStream);
		return FileStruct;
	}
	else
	{
		printf("Unable To Read File: %s!\n", FileName);
		DeleteStruct(FileStruct);
		return FileStruct;
	}
}

void WriteFile(const char* FileName, File* FileStruct)
{
	int ExtractedFileNameLength = strlen("Extracted_") + strlen(FileName) + 1;
	char* ExtractedFileName = (char*)malloc(sizeof(char) * ExtractedFileNameLength);
	strcpy(ExtractedFileName, "Extracted_");
	strcat(ExtractedFileName, FileName);
	FILE* FileStream = fopen(ExtractedFileName, "wb");
	if (FileStream)
	{
		fwrite(FileStruct->FileData, sizeof(unsigned char), *(FileStruct->FileSize), FileStream);
		fclose(FileStream);
		printf("Wrote File %s\n", ExtractedFileName);
		free(ExtractedFileName);
		ExtractedFileName = NULL;
	}
	else
	{
		printf("Unable To Save File: %s!\n", FileName);
	}
}

void main()
{
	const char* FirstFileName = (const char*)"First.jpg";
	const char* SecondFileName = (const char*)"Second.mp3";
	const char* ThirdFileName = (const char*)"Third.bmp";
	const char* FourthFileName = (const char*)"CurrentSource.txt";
	const char* ContainerFileName = (const char*)"DigitalContainer.dc";
	DigitalContainer* DigitalContainerStruct = NULL;
	File* FileStruct = NULL;

	FileStruct = ReadFile(FirstFileName);
	if (FileStruct != NULL)
	{
		InsertFileIntoDigitalContainer(DigitalContainerStruct, FileStruct);
		DeleteStruct(FileStruct);
	}
	else
	{
		printf("File: %s Not Inserted In Container!\n", FirstFileName);
	}

	FileStruct = ReadFile(SecondFileName);
	if (FileStruct != NULL)
	{
		InsertFileIntoDigitalContainer(DigitalContainerStruct, FileStruct);
		DeleteStruct(FileStruct);
	}
	else
	{
		printf("File: %s Not Inserted In Container!\n", SecondFileName);
	}

	FileStruct = ReadFile(ThirdFileName);
	if (FileStruct != NULL)
	{
		InsertFileIntoDigitalContainer(DigitalContainerStruct, FileStruct);
		DeleteStruct(FileStruct);
	}
	else
	{
		printf("File: %s Not Inserted In Container!\n", ThirdFileName);
	}

	FileStruct = ReadFile(FourthFileName);
	if (FileStruct != NULL)
	{
		InsertFileIntoDigitalContainer(DigitalContainerStruct, FileStruct);
		DeleteStruct(FileStruct);
	}
	else
	{
		printf("File: %s Not Inserted In Container!\n", FourthFileName);
	}

	if (DigitalContainerStruct != NULL)
	{
		EncodeDigitalContainer(ContainerFileName, DigitalContainerStruct);
		DeleteDigitalContainer(DigitalContainerStruct);
		printf("Container Encoded As %s\n", ContainerFileName);
	}
	else
	{
		printf("Container Empty Cannot Encode!\n");
	}

	DigitalContainerStruct = DecodeDigitalContainer(ContainerFileName);
	if (DigitalContainerStruct != NULL)
	{
		ExtractFilesFromDigitalContainer(DigitalContainerStruct);
		printf("Container Extracted!\n");
	}
	else
	{
		printf("Container Empty!\n");
	}
	DeleteDigitalContainer(DigitalContainerStruct);
	_getch();
}