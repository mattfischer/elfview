#include "Location.h"

#include <wx/arrstr.h>

wxString Location::BuildLocation(int token, wxString body, wxString offset)
{
	wxString location = "elf://";

	location += wxString::Format("%i", token);
	location += "/" + body;

	if(offset != "") {
		location += "#" + offset;
	}

	return location;
}

wxString Location::BuildLocation(int token, wxString body, int offset)
{
	return BuildLocation(token, body, wxString::Format("%i", offset));
}

wxString Location::BuildLocation(ElfFile *file, wxString body, wxString offset)
{
	return BuildLocation(file->GetToken(), body, offset);
}

wxString Location::BuildLocation(ElfFile *file, wxString body, int offset)
{
	return BuildLocation(file->GetToken(), body, offset);
}

static void Split(wxString location, wxString &protocol, long &token, wxArrayString &body, wxString &offset)
{
	int idx;

	idx = location.Find("://");

	if(idx != wxNOT_FOUND) {
		protocol = location.SubString(0, idx);
		location = location.SubString(idx + 3, location.Length());
	}

	idx = location.Find("/");
	wxString t;

	if(idx == wxNOT_FOUND) {
		t = location;
	} else {
		t = location.Mid(0, idx);
		location = location.Mid(idx + 1);
	}

	t.ToLong(&token);

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

int Location::GetToken(wxString location)
{
	wxString protocol;
	long token;
	wxArrayString body;
	wxString offset;

	Split(location, protocol, token, body, offset);

	return token;
}

wxString Location::GetSectionString(wxString location, int section)
{
	wxString protocol;
	long token;
	wxArrayString body;
	wxString offset;

	Split(location, protocol, token, body, offset);

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
	wxString protocol;
	long token;
	wxArrayString body;
	wxString offset;

	Split(location, protocol, token, body, offset);

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