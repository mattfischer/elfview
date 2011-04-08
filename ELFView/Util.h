#ifndef UTIL_H
#define UTIL_H

#include <wx/string.h>

#include "ElfFile.h"

class Util
{
public:
	static int GetSectionNumber(wxString location);
	static wxString GetSectionTitle(ElfFile *file, int section);
};

#endif