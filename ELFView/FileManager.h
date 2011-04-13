#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "ElfFile.h"

#include <map>

class FileManager
{
public:
	FileManager();

	void AddFile(ElfFile *file);

	ElfFile *FindFile(int token);
	void CloseFile(ElfFile *file);

protected:
	std::map<int, ElfFile*> mMap;
};
#endif