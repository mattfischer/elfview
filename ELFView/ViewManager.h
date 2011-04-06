#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <wx/string.h>

#include "View.h"
#include "ElfFile.h"

#include <vector>

class WindowMain;

class ViewManager
{
public:
	ViewManager();

	void SetWindowMain(WindowMain *windowMain);

	void GoToLocation(ElfFile *file, wxString location);
	int AddLocation(ElfFile *file, wxString location);

	void CloseAllViews(ElfFile *file);

protected:
	WindowMain *mWindowMain;
	std::vector<View*> mViewList;

	View *FindView(ElfFile *file, wxString location, int &idx);
	View *CreateView(ElfFile *file, wxString location);
	int AddView(View *view);
};
#endif