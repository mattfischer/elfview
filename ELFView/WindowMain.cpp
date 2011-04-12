#include "WindowMain.h"

#include <wx/msgdlg.h>
#include <wx/menu.h>

enum {
	ID_Menu_Close
};

WindowMain::WindowMain(wxWindow *parent, wxWindowID id, ViewManager *viewManager)
: wxNotebook(parent, id)
{
	mViewManager = viewManager;

	mViewManager->Connect(EVT_VM_VIEW_ADDED, (wxObjectEventFunction)&WindowMain::OnViewAdded, NULL, this);
	mViewManager->Connect(EVT_VM_VIEW_REMOVED, (wxObjectEventFunction)&WindowMain::OnViewRemoved, NULL, this);
	mViewManager->Connect(EVT_VM_CURRENT_VIEW_CHANGED, (wxObjectEventFunction)&WindowMain::OnCurrentViewChanged, NULL, this);

	mCloseTab = -1;
}

void WindowMain::OnViewAdded(wxCommandEvent &e)
{
	View *view = (View*)e.GetClientData();

	Hide();
	wxWindow *window = view->CreateWindow(this, wxID_ANY);
	AddPage(window, view->GetName());
	Show();
}

void WindowMain::OnViewRemoved(wxCommandEvent &e)
{
	DeletePage(e.GetInt());
}

void WindowMain::OnCurrentViewChanged(wxCommandEvent &e)
{
	ChangeSelection(e.GetInt());
}

void WindowMain::OnRightUp(wxMouseEvent &e)
{
	int tab = HitTest(e.GetPosition(), NULL);

	if(tab != wxNOT_FOUND) {
		wxMenu *menu = new wxMenu;
		menu->Append(ID_Menu_Close, "Close");

		mCloseTab = tab;
		PopupMenu(menu);
		mCloseTab = -1;
	}
	e.Skip();
}

void WindowMain::OnMenu(wxCommandEvent &e)
{
	mViewManager->CloseView(mCloseTab);
}

void WindowMain::OnLink(wxHyperlinkEvent &e)
{
	mViewManager->GoToLocation((ElfFile*)e.GetClientData(), e.GetURL());
}

BEGIN_EVENT_TABLE(WindowMain, wxNotebook)
	EVT_RIGHT_UP(WindowMain::OnRightUp)
	EVT_MENU(wxID_ANY, WindowMain::OnMenu)
	EVT_HYPERLINK(wxID_ANY, WindowMain::OnLink)
END_EVENT_TABLE()