#ifndef LOCATION_H
#define LOCATION_H

#include <wx/string.h>

#include "ElfFile.h"

class Location
{
public:
	static wxString BuildLocation(wxString prefix, wxString body, wxString offset = "");
	static wxString BuildLocation(wxString prefix, wxString body, int offset);

	static wxString BuildElfLocation(int token, wxString body, wxString offset = "");
	static wxString BuildElfLocation(int token, wxString body, int offset);

	static wxString BuildElfLocation(ElfFile *file, wxString body, wxString offset = "");
	static wxString BuildElfLocation(ElfFile *file, wxString body, int offset);

	static wxString GetPrefix(wxString location);

	static int GetElfToken(wxString location);

	static wxString GetSectionString(wxString location, int section);
	static int GetSectionInt(wxString location, int section);

	static wxString GetOffsetString(wxString location);
	static int GetOffsetInt(wxString location);

	static wxString GetBase(wxString location);
};
#endif