#ifndef FRAME_FLAGS_H
#define FRAME_FLAGS_H

#include <wx/frame.h>
#include <wx/grid.h>

#include "FlagManager.h"

#include <vector>

class FrameFlags : public wxFrame
{
public:
	static FrameFlags *GetInstance(wxWindow *parent, wxWindowID id);

	FrameFlags(wxWindow *parent, wxWindowID id);
	~FrameFlags();

	void SetLocation(wxString location);

protected:
	wxGrid *mGrid;
	wxString mLocation;

	std::vector<FlagManager::Value> mValues;

	void SetupValues(wxString section);
	void SetRowBold(int row, bool bold);

	void OnClose(wxCommandEvent &e);

	DECLARE_EVENT_TABLE();
};
#endif