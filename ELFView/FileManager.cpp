#include "FileManager.h"

FileManager::FileManager()
{
}

void FileManager::AddFile(ElfFile *file)
{
	mMap[file->GetToken()] = file;
}

ElfFile *FileManager::FindFile(int token)
{
	return mMap[token];
}

void FileManager::CloseFile(ElfFile *file)
{
	int token = file->GetToken();

	mMap.erase(mMap.find(token));

	delete file;
}