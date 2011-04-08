#include "ViewStringTable.h"

#include "Util.h"
ViewStringTable::ViewStringTable(ElfFile *file, wxString location)
: View(file, location)
{
	mSection = Util::GetSectionNumber(location);
	SetName(GetFile()->GetSectionName(mSection));
}

wxWindow *ViewStringTable::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mListCtrl = new wxListCtrl(parent, id);

	mListCtrl->SetSingleStyle(wxLC_REPORT);
	const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);

	char *buffer = new char[header->sh_size];

	GetFile()->Read(buffer, header->sh_offset, header->sh_size);

	mListCtrl->InsertColumn(0, "Offset");
	mListCtrl->InsertColumn(1, "String");

	int offset = 0;
	int i=0;
	while(offset < header->sh_size) {
		mListCtrl->InsertItem(i, "");
		mListCtrl->SetItem(i, 0, wxString::Format("0x%x", offset));
		mListCtrl->SetItem(i, 1, buffer + offset);
		i++;
		while(buffer[offset] != '\0') offset++;
		offset++;
	}

	delete[] buffer;

	return mListCtrl;
}