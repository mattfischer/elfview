#include "Util.h"

int Util::GetSectionNumber(wxString location)
{
	long section;
	int idx = location.Find('/', true);
	wxString number = location.SubString(idx + 1, location.size());
	number.ToLong(&section);

	return section;
}

wxString Util::GetSectionTitle(ElfFile *file, int section)
{
	wxString name = file->GetSectionName(section);

	if(name == "") {
		return wxString::Format("%i", section);
	} else {
		return wxString::Format("%s", name.c_str());
	}
}