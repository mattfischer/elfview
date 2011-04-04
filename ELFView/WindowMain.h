#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <wx/notebook.h>
#include <wx/htmllbox.h>
#include <wx/list.h>

#include "ElfFile.h"
#include "View.h"

class WindowMain : public wxNotebook
{
public:
	WindowMain(wxWindow *parent, wxWindowID id);

	void SetFile(ElfFile *file);

protected:
	ElfFile *mFile;

	ViewList mViewList;

	void AddView(View *view);
};
#endif