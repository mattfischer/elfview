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

	mViewManager->Bind(EVT_VM_VIEW_ADDED, &WindowMain::OnViewAdded, this);
	mViewManager->Bind(EVT_VM_VIEW_REMOVED, &WindowMain::OnViewRemoved, this);
	mViewManager->Bind(EVT_VM_CURRENT_VIEW_CHANGED, &WindowMain::OnCurrentViewChanged, this);

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
	mViewManager->GoToLocation(e.GetURL());
}

void WindowMain::OnHtmlLink(wxHtmlLinkEvent &e)
{
	mViewManager->GoToLocation(e.GetLinkInfo().GetHref());
}

BEGIN_EVENT_TABLE(WindowMain, wxNotebook)
	EVT_RIGHT_UP(WindowMain::OnRightUp)
	EVT_MENU(wxID_ANY, WindowMain::OnMenu)
	EVT_HYPERLINK(wxID_ANY, WindowMain::OnLink)
	EVT_HTML_LINK_CLICKED(wxID_ANY, WindowMain::OnHtmlLink)
END_EVENT_TABLE()