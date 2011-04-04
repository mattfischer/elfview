#include "ViewProgramHeaders.h"

ViewProgramHeaders::ViewProgramHeaders(wxWindow *parent, wxWindowID id)
: wxSimpleHtmlListBox(parent, id)
{
}

void ViewProgramHeaders::SetFile(ElfFile *file)
{
	mFile = file;

	Clear();

	if(mFile->GetProgramHeaders()) {
		for(int i=0; i<mFile->GetHeader()->e_phnum;i++) {
			const Elf32_Phdr *header = mFile->GetProgramHeaders() + i;

			Append(wxString::Format("<b>Segment %i</b>", i));
			Append(wxString::Format("Type: 0x%x", header->p_type));
			Append(wxString::Format("Offset: 0x%x", header->p_offset));
			Append(wxString::Format("VAddr: 0x%x", header->p_vaddr));
			Append(wxString::Format("PAddr: 0x%x", header->p_paddr));
			Append(wxString::Format("File Size: 0x%x", header->p_filesz));
			Append(wxString::Format("Memory Size: 0x%x", header->p_memsz));
			Append(wxString::Format("Flags: 0x%x", header->p_flags));
			Append(wxString::Format("Alignment: 0x%x", header->p_align));
			Append("");
		}
	} else {
		Append("<i>No Program Headers</i>");
	}
}