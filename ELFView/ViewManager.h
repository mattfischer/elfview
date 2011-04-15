#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <wx/string.h>
#include <wx/event.h>

#include "View.h"
#include "ElfFile.h"
#include "FileManager.h"

#include <vector>

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(EVT_VM_VIEW_ADDED, -1)
	DECLARE_EVENT_TYPE(EVT_VM_VIEW_REMOVED, -1)
	DECLARE_EVENT_TYPE(EVT_VM_CURRENT_VIEW_CHANGED, -1)
END_DECLARE_EVENT_TYPES()

class ViewManager : public wxEvtHandler
{
public:
	ViewManager(FileManager *fileManager);

	void GoToLocation(wxString location);
	void CloseView(int view);
	void CloseAllViews(ElfFile *file);

protected:
	std::vector<View*> mViewList;
	FileManager *mFileManager;
	int mCurrentView;

	View *FindView(wxString location, int &idx);
	View *CreateView(wxString location);
	int AddView(View *view);
	wxString TranslateLocation(wxString location);
};
#endif