#ifndef FRAME_MAIN_H
#define FRAME_MAIN_H

#include <wx/frame.h>

#include "WindowNavigator.h"
#include "WindowMain.h"
#include "ElfFile.h"
#include "ViewManager.h"

class FrameMain : public wxFrame
{
public:
	FrameMain(wxWindow *parent, wxWindowID id);

protected:
	WindowNavigator *mWindowNavigator;
	WindowMain *mWindowMain;

	ElfFile *mFile;
	ViewManager *mViewManager;

	void OnFileOpen(wxCommandEvent &e);
	void OnFileExit(wxCommandEvent &e);

	DECLARE_EVENT_TABLE();
};

#endif