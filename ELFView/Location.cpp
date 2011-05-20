#include "Location.h"

#include <wx/arrstr.h>

wxString Location::BuildLocation(wxString prefix, wxString body, wxString offset)
{
	wxString location = prefix + "://";

	location += body;

	if(offset != "") {
		location += "#" + offset;
	}

	return location;
}

wxString Location::BuildLocation(wxString prefix, wxString body, int offset)
{
	return BuildLocation(prefix, body, wxString::Format("%i", offset));
}


wxString Location::BuildElfLocation(int token, wxString body, wxString offset)
{
	return BuildLocation("elf", wxString::Format("%i/%s", token, body.c_str()), offset);
}

wxString Location::BuildElfLocation(int token, wxString body, int offset)
{
	return BuildElfLocation(token, body, wxString::Format("%i", offset));
}

wxString Location::BuildElfLocation(ElfFile *file, wxString body, wxString offset)
{
	return BuildElfLocation(file->GetToken(), body, offset);
}

wxString Location::BuildElfLocation(ElfFile *file, wxString body, int offset)
{
	return BuildElfLocation(file->GetToken(), body, offset);
}

static void Split(wxString location, wxString &prefix, wxArrayString &body, wxString &offset)
{
	int idx;

	idx = location.Find("://");

	if(idx != wxNOT_FOUND) {
		prefix = location.SubString(0, idx - 1);
		location = location.SubString(idx + 3, location.Length());
	}

	idx = location.Find("#");

	if(idx == wxNOT_FOUND) {
		offset = "";
	} else {
		offset = location.Mid(idx + 1);
		location = location.Mid(0, idx);
	} 

	body.Clear();

	while(location != "") {
		idx = location.Find("/");
		if(idx == wxNOT_FOUND) {
			body.Add(location);
			location = "";
		} else {
			body.Add(location.Mid(0, idx));
			location = location.Mid(idx + 1);
		}
	}
}

wxString Location::GetPrefix(wxString location)
{
	wxString prefix;
	wxArrayString body;
	wxString offset;

	Split(location, prefix, body, offset);

	return prefix;
}

int Location::GetElfToken(wxString location)
{
	return GetSectionInt(location, 0);
}

wxString Location::GetSectionString(wxString location, int section)
{
	wxString prefix;
	wxArrayString body;
	wxString offset;

	Split(location, prefix, body, offset);

	if(section < body.Count()) {
		return body.Item(section);
	} else {
		return "";
	}
}

int Location::GetSectionInt(wxString location, int section)
{
	wxString str = GetSectionString(location, section);
	long n;
	bool success = str.ToLong(&n);

	if(success) {
		return n;
	} else {
		return -1;
	}
}

wxString Location::GetOffsetString(wxString location)
{
	wxString prefix;
	wxArrayString body;
	wxString offset;

	Split(location, prefix, body, offset);

	return offset;
}

int Location::GetOffsetInt(wxString location)
{
	wxString str = GetOffsetString(location);
	long n;
	bool success = str.ToLong(&n);

	if(success) {
		return n;
	} else {
		return -1;
	}
}

wxString Location::GetBase(wxString location)
{
	int idx = location.Find("#");

	if(idx == wxNOT_FOUND) {
		return location;
	} else {
		return location.Mid(0, idx);
	}
}