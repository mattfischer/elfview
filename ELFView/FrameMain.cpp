#include "FrameMain.h"

#include <wx/splitter.h>
#include <wx/menu.h>
#include <wx/filedlg.h>

enum {
	ID_File_Open,
	ID_File_Exit
};

FrameMain::FrameMain(wxWindow *parent, wxWindowID id)
: wxFrame(parent, id, "ELFView")
{
	wxMenuBar *menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu();
	fileMenu->Append(ID_File_Open, "Open");
	fileMenu->AppendSeparator();
	fileMenu->Append(ID_File_Exit, "Exit");

	menuBar->Append(fileMenu, "File");
	SetMenuBar(menuBar);

	wxSplitterWindow *splitter = new wxSplitterWindow(this, -1);

	mWindowNavigator = new WindowNavigator(splitter, -1);
	mWindowMain = new WindowMain(splitter, -1);

	splitter->SplitVertically(mWindowNavigator, mWindowMain);
}

void FrameMain::OnFileOpen(wxCommandEvent &e)
{
	wxFileDialog *dialog = new wxFileDialog(this, "Open File", "", "", "Object Files (*.o)|*.o|Shared Object Files (*.so)|*.so|All Files (*.*)|*", wxFD_OPEN);

	dialog->ShowModal();
}

void FrameMain::OnFileExit(wxCommandEvent &e)
{
	Close();
}

BEGIN_EVENT_TABLE(FrameMain, wxFrame)
	EVT_MENU(ID_File_Open, FrameMain::OnFileOpen)
	EVT_MENU(ID_File_Exit, FrameMain::OnFileExit)
END_EVENT_TABLE()