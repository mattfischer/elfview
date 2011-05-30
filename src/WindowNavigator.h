#ifndef WINDOW_NAVIGATOR_H
#define WINDOW_NAVIGATOR_H

#include <wx/treectrl.h>

#include "ElfFile.h"
#include "ViewManager.h"

class WindowNavigator : public wxTreeCtrl
{
public:
	WindowNavigator(wxWindow *parent, wxWindowID id, ViewManager *viewManager);

	void SetFile(ElfFile *file);

protected:
	ElfFile *mFile;
	ViewManager *mViewManager;
	
	void OnItemActivated(wxTreeEvent &e);

	DECLARE_EVENT_TABLE();
};
#endif