#ifndef VIEW_H
#define VIEW_H

#include <wx/string.h>
#include <wx/panel.h>

#include "ElfFile.h"

class View
{
public:
	View(ElfFile *file, wxString location);

	wxString GetName();
	wxString GetLocation();
	ElfFile *GetFile();

	int GetOffset();
	void SetOffset(int offset);

	wxWindow *CreateWindow(wxWindow *parent, wxWindowID id);

protected:
	void SetName(wxString name);
	
	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id) = 0;
	virtual void doSetOffset(int offset) = 0;

private:
	ElfFile *mFile;
	wxString mName;
	wxString mLocation;
	wxWindow *mWindow;
	int mOffset;
};
#endif