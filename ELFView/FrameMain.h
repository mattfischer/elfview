#ifndef FRAME_MAIN_H
#define FRAME_MAIN_H

#include <wx/frame.h>

#include "WindowNavigator.h"
#include "WindowMain.h"

class FrameMain : public wxFrame
{
public:
	FrameMain(wxWindow *parent, wxWindowID id);

protected:
	WindowNavigator *mWindowNavigator;
	WindowMain *mWindowMain;

	void OnFileOpen(wxCommandEvent &e);
	void OnFileExit(wxCommandEvent &e);

	DECLARE_EVENT_TABLE();
};

#endif