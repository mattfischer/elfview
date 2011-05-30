#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <wx/string.h>
#include <wx/event.h>

#include "View.h"
#include "ElfFile.h"
#include "FileManager.h"

#include <vector>

wxDECLARE_EVENT(EVT_VM_VIEW_ADDED, wxCommandEvent);
wxDECLARE_EVENT(EVT_VM_VIEW_REMOVED, wxCommandEvent);
wxDECLARE_EVENT(EVT_VM_CURRENT_VIEW_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_VM_HISTORY_INDEX_CHANGED, wxCommandEvent);

class ViewManager : public wxEvtHandler
{
public:
	ViewManager(FileManager *fileManager);

	void GoToLocation(wxString location, bool addToHistory = true);
	void CloseView(int view);
	void CloseAllViews(ElfFile *file);

	void GoBackInHistory();
	void GoForwardInHistory();

	int GetHistoryIndex();
	int GetHistoryCount();

protected:
	std::vector<View*> mViewList;
	FileManager *mFileManager;
	int mCurrentView;

	std::vector<wxString> mHistory;
	int mHistoryIndex;

	View *FindView(wxString location, int &idx);
	View *CreateView(wxString location);
	int AddView(View *view);
	wxString TranslateLocation(wxString location);
};
#endif