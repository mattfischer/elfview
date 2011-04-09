#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <wx/string.h>
#include <wx/event.h>

#include "View.h"
#include "ElfFile.h"

#include <vector>

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(EVT_VM_VIEW_ADDED, -1)
	DECLARE_EVENT_TYPE(EVT_VM_VIEW_REMOVED, -1)
	DECLARE_EVENT_TYPE(EVT_VM_CURRENT_VIEW_CHANGED, -1)
END_DECLARE_EVENT_TYPES()

class ViewManager : public wxEvtHandler
{
public:
	ViewManager();

	void GoToLocation(ElfFile *file, wxString location);
	int AddLocation(ElfFile *file, wxString location);
	void CloseView(int view);
	void CloseAllViews(ElfFile *file);

protected:
	std::vector<View*> mViewList;

	View *FindView(ElfFile *file, wxString location, int &idx);
	View *CreateView(ElfFile *file, wxString location);
	int AddView(View *view);
};
#endif