#include "View.h"

View::View(ElfFile *file, wxString location)
: mFile(file),
  mLocation(location),
  mWindow(NULL)
{
}

wxString View::GetName()
{
	return mName;
}

ElfFile *View::GetFile()
{
	return mFile;
}

void View::SetName(wxString name)
{
	mName = name;
}

wxString View::GetLocation()
{
	return mLocation;
}

wxWindow *View::CreateWindow(wxWindow *parent, wxWindowID id)
{
	mWindow = doCreateWindow(parent, id);

	return mWindow;
}