#ifndef LOCATION_H
#define LOCATION_H

#include <wx/string.h>

#include "ElfFile.h"

class Location
{
public:
	static wxString BuildLocation(int token, wxString body, wxString offset = "");
	static wxString BuildLocation(int token, wxString body, int offset);

	static wxString BuildLocation(ElfFile *file, wxString body, wxString offset = "");
	static wxString BuildLocation(ElfFile *file, wxString body, int offset);

	static int GetToken(wxString location);

	static wxString GetSectionString(wxString location, int section);
	static int GetSectionInt(wxString location, int section);

	static wxString GetOffsetString(wxString location);
	static int GetOffsetInt(wxString location);

	static wxString GetBase(wxString location);
};
#endif