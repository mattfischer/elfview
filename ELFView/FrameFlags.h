#ifndef FRAME_FLAGS_H
#define FRAME_FLAGS_H

#include <wx/frame.h>
#include <wx/grid.h>

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

	struct FlagValue {
		wxString name;
		int value;

		FlagValue(wxString n, int v) : name(n), value(v) {}
	};
	std::vector<FlagValue> mValues;

	void SetupValues(wxString section);
	void AddValue(wxString name, int value);
	void SetRowBold(int row, bool bold);

	void OnClose(wxCommandEvent &e);

	DECLARE_EVENT_TABLE();
};
#endif