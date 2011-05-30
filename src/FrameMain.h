#ifndef FRAME_MAIN_H
#define FRAME_MAIN_H

#include <wx/frame.h>
#include <wx/toolbar.h>

#include "WindowNavigator.h"
#include "WindowMain.h"
#include "ElfFile.h"
#include "ViewManager.h"
#include "FileManager.h"

class FrameMain : public wxFrame
{
public:
	FrameMain(wxWindow *parent, wxWindowID id);

protected:
	WindowNavigator *mWindowNavigator;
	WindowMain *mWindowMain;

	ElfFile *mFile;
	FileManager *mFileManager;
	ViewManager *mViewManager;
	wxToolBar *mToolBar;

	void OnFileOpen(wxCommandEvent &e);
	void OnFileExit(wxCommandEvent &e);
	void OnGoBack(wxCommandEvent &e);
	void OnGoForward(wxCommandEvent &e);
	void OnHistoryIndexChanged(wxCommandEvent &e);
	void OnLink(wxHyperlinkEvent &e);
	void OnHtmlLink(wxHtmlLinkEvent &e);

	void HandleLink(wxString location);

	DECLARE_EVENT_TABLE();
};

#endif