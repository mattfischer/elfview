#include "WindowNavigator.h"

#include "Location.h"

struct ItemData : public wxTreeItemData {
	wxString path;

	ItemData(wxString p) : path(p) {}
};

WindowNavigator::WindowNavigator(wxWindow *parent, wxWindowID id, ViewManager *viewManager)
: wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_HIDE_ROOT)
{
	mViewManager = viewManager;
}

static wxString GetPhdrTypeDescription(int type)
{
	switch(type) {
		case PT_LOAD:
			return "LOAD";
		case PT_DYNAMIC:
			return "DYNAMIC";
		case PT_INTERP:
			return "INTERP";
		case PT_NOTE:
			return "NOTE";
		case PT_SHLIB:
			return "SHLIB";
		case PT_PHDR:
			return "PHDR";
		default:
			return "";
	}
}

void WindowNavigator::SetFile(ElfFile *file)
{
	mFile = file;

	Freeze();
	DeleteAllItems();
	wxTreeItemId root = AddRoot("");

	AppendItem(root, "ELF Header", -1, -1, new ItemData(Location::BuildLocation(file, "header")));

	wxTreeItemId sections = AppendItem(root, "Sections");
	AppendItem(sections, "Section Headers", -1, -1, new ItemData(Location::BuildLocation(file, "section/headers")));
	for(int i=1;i<mFile->GetHeader()->e_shnum;i++) {
		AppendItem(sections, mFile->GetSectionName(i), -1, -1, new ItemData(Location::BuildLocation(file, wxString::Format("section/%i", i))));
	}

	wxTreeItemId segments = AppendItem(root, "Segments");
	AppendItem(segments, "Program Headers", -1, -1, new ItemData(Location::BuildLocation(file, "segment/headers")));
	for(int i=1;i<mFile->GetHeader()->e_phnum;i++) {
		const Elf32_Phdr *header = mFile->GetProgramHeader(i);
		wxString title;
		wxString desc = GetPhdrTypeDescription(header->p_type);
		if(desc == "") {
			title = wxString::Format("%i", i);
		} else {
			title = wxString::Format("%i (%s)", i, desc.c_str());
		}
		AppendItem(segments, title, -1, -1, new ItemData(Location::BuildLocation(file, wxString::Format("segment/%i", i))));
	}

	Thaw();
}

void WindowNavigator::OnItemActivated(wxTreeEvent &e)
{
	wxTreeItemId id = e.GetItem();

	ItemData *data = (ItemData*)GetItemData(id);

	if(data != NULL && data->path != "") {
		mViewManager->GoToLocation(data->path);
	}
}

BEGIN_EVENT_TABLE(WindowNavigator, wxTreeCtrl)
	EVT_TREE_ITEM_ACTIVATED(wxID_ANY, WindowNavigator::OnItemActivated)
END_EVENT_TABLE()