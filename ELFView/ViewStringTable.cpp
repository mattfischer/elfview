#include "ViewStringTable.h"

#include "Location.h"

ViewStringTable::ViewStringTable(ElfFile *file, wxString location)
: View(file, location)
{
	mSection = Location::GetSectionInt(location, 1);
	SetName(GetFile()->GetSectionName(mSection));
}

wxWindow *ViewStringTable::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mTable = new LinkTable(parent, id);

	
	const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);

	char *buffer = new char[header->sh_size];

	GetFile()->Read(buffer, header->sh_offset, header->sh_size);

	int offset = 0;
	int i=0;
	while(offset < header->sh_size) {
		mOffsets.push_back(offset);
		i++;
		while(buffer[offset] != '\0') offset++;
		offset++;
	}

	mTable->Setup(mOffsets.size(), 2);

	mTable->SetColumnLabel(0, "Offset");
	mTable->SetColumnLabel(1, "String");

	for(int i=0; i<mOffsets.size(); i++) {
		mTable->SetCell(i, 0, wxString::Format("0x%x",  mOffsets[i]));
		mTable->SetCell(i, 1, buffer +  mOffsets[i]);
	}

	delete[] buffer;

	return mTable;
}

void ViewStringTable::doSetOffset(int offset)
{
	for(int i=0; i<mOffsets.size(); i++) {
		if(mOffsets[i] >= offset) {
			mTable->SelectRow(i);
			return;
		}
	}

	mTable->SelectRow(mOffsets.size() - 1);
}