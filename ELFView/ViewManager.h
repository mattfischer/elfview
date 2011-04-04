#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <wx/string.h>

#include "View.h"
#include "ElfFile.h"

class WindowMain;

class ViewManager
{
public:
	ViewManager(WindowMain *windowMain);

	void SetFile(ElfFile *file);

	void GoToLocation(wxString location);
	int AddLocation(wxString location);

protected:
	WindowMain *mWindowMain;
	ViewList mViewList;
	ElfFile *mFile;

	View *FindView(wxString location, int &idx);
	int AddView(View *view);
	View *CreateView(wxString location);
};
#endif