#ifndef VIEW_H
#define VIEW_H

#include <wx/string.h>
#include <wx/window.h>

#include "ElfFile.h"

class View
{
public:
	View();

	wxString GetName();
	void SetFile(ElfFile *file);
	ElfFile *GetFile();

	wxWindow *CreateWindow(wxWindow *parent, wxWindowID id);

protected:
	void SetWindow(wxWindow *window);
	void SetName(wxString name);

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id) = 0;
	virtual void doUpdateWindow() = 0;

private:
	wxString mName;
	ElfFile *mFile;
	wxWindow *mWindow;
};

WX_DECLARE_LIST(View, ViewList);
#endif