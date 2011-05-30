#ifndef LOCATION_H
#define LOCATION_H

#include <wx/string.h>

#include "ElfFile.h"
#include "FlagManager.h"

class Location
{
public:
	static wxString BuildLocation(wxString prefix, wxString body, int offset = -1);

	static wxString BuildElfLocation(int token, wxString body, int offset = -1);
	static wxString BuildElfLocation(ElfFile *file, wxString body, int offset = -1);

	static wxString BuildFlagLocation(FlagManager::Set set, int value = -1);

	static wxString GetPrefix(wxString location);

	static int GetElfToken(wxString location);

	static wxString GetSectionString(wxString location, int section);
	static int GetSectionInt(wxString location, int section);

	static int GetOffset(wxString location);

	static wxString GetBase(wxString location);
};
#endif