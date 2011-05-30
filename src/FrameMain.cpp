#include "FrameMain.h"

#include <wx/splitter.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/artprov.h>

#include "Elf32.h"

#include "Location.h"
#include "FrameFlags.h"

enum {
	ID_File_Open,
	ID_File_Exit,
	ID_Go_Back,
	ID_Go_Forward
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

	mToolBar = CreateToolBar();

	mToolBar->AddTool(ID_File_Open, "", wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
	mToolBar->AddSeparator();
	mToolBar->AddTool(ID_Go_Back, "", wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_TOOLBAR));
	mToolBar->AddTool(ID_Go_Forward, "", wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR));
	mToolBar->Realize();

	mToolBar->EnableTool(ID_Go_Back, false);
	mToolBar->EnableTool(ID_Go_Forward, false);

	mFileManager = new FileManager;
	mViewManager = new ViewManager(mFileManager);

	mViewManager->Bind(EVT_VM_HISTORY_INDEX_CHANGED, &FrameMain::OnHistoryIndexChanged, this);

	wxSplitterWindow *splitter = new wxSplitterWindow(this, -1);

	mWindowNavigator = new WindowNavigator(splitter, -1, mViewManager);
	mWindowMain = new WindowMain(splitter, -1, mViewManager);

	splitter->SplitVertically(mWindowNavigator, mWindowMain);

	mFile = NULL;
}

void FrameMain::OnFileOpen(wxCommandEvent &e)
{
	wxFileDialog *dialog = new wxFileDialog(this, "Open File", "", "", "Object Files (*.o)|*.o|Shared Object Files (*.so)|*.so|All Files (*.*)|*", wxFD_OPEN);

	int result = dialog->ShowModal();

	if(result == wxID_OK) {
		if(mFile != NULL) {
			mViewManager->CloseAllViews(mFile);
			mFileManager->CloseFile(mFile);
		}

		mFile = new ElfFile(dialog->GetPath());
		mFileManager->AddFile(mFile);

		mWindowNavigator->SetFile(mFile);		
	}
}

void FrameMain::OnFileExit(wxCommandEvent &e)
{
	Close();
}

void FrameMain::OnGoBack(wxCommandEvent &e)
{
	mViewManager->GoBackInHistory();
}

void FrameMain::OnGoForward(wxCommandEvent &e)
{
	mViewManager->GoForwardInHistory();
}

void FrameMain::OnHistoryIndexChanged(wxCommandEvent &e)
{
	if(e.GetInt() == 0) {
		mToolBar->EnableTool(ID_Go_Back, false);
	} else {
		mToolBar->EnableTool(ID_Go_Back, true);
	}

	if(e.GetInt() == mViewManager->GetHistoryCount() - 1) {
		mToolBar->EnableTool(ID_Go_Forward, false);
	} else {
		mToolBar->EnableTool(ID_Go_Forward, true);
	}
}


void FrameMain::OnLink(wxHyperlinkEvent &e)
{
	HandleLink(e.GetURL());
}

void FrameMain::OnHtmlLink(wxHtmlLinkEvent &e)
{
	HandleLink(e.GetLinkInfo().GetHref());
}

void FrameMain::HandleLink(wxString location)
{
	if(Location::GetPrefix(location) == "elf") {
		mWindowMain->GoToLocation(location);
	} else if(Location::GetPrefix(location) == "flags") {
		FrameFlags *frameFlags = FrameFlags::GetInstance(this, wxID_ANY);

		frameFlags->SetLocation(location);
		frameFlags->Show();
	}
}

BEGIN_EVENT_TABLE(FrameMain, wxFrame)
	EVT_MENU(ID_File_Open, FrameMain::OnFileOpen)
	EVT_MENU(ID_File_Exit, FrameMain::OnFileExit)
	EVT_MENU(ID_Go_Back, FrameMain::OnGoBack)
	EVT_MENU(ID_Go_Forward, FrameMain::OnGoForward)
	EVT_HYPERLINK(wxID_ANY, FrameMain::OnLink)
	EVT_HTML_LINK_CLICKED(wxID_ANY, FrameMain::OnHtmlLink)
END_EVENT_TABLE()