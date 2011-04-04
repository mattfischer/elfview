#include "View.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(ViewList);

View::View()
{
	mFile = NULL;
	mWindow = NULL;
}

wxString View::GetName()
{
	return mName;
}

void View::SetName(wxString name)
{
	mName = name;
}

void View::SetFile(ElfFile *file)
{
	mFile = file;

	if(mWindow) {
		doUpdateWindow();
	}
}

ElfFile *View::GetFile()
{
	return mFile;
}

wxWindow *View::CreateWindow(wxWindow *parent, wxWindowID id)
{
	mWindow = doCreateWindow(parent, id);

	if(mFile) {
		doUpdateWindow();
	}

	return mWindow;
}

void View::SetLocation(wxString location)
{
	mLocation = location;
}

wxString View::GetLocation()
{
	return mLocation;
}