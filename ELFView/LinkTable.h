#ifndef LINK_TABLE_H
#define LINK_TABLE_H

#include <wx/grid.h>
#include <wx/hyperlink.h>

class LinkTable : public wxGrid
{
public:
	LinkTable(wxWindow *parent, wxWindowID id);

	void Setup(int rows, int cols);
	void SetColumnLabel(int col, wxString label);
	void SetCell(int row, int col, wxString text, wxString target = "");

protected:
	wxArrayString mTargets;
	int mNumCols;
	wxGridCellCoords mDownCoords;

	void OnMotion(wxMouseEvent &e);
	void OnLeft(wxMouseEvent &e);

	wxGridCellCoords GetCoords(wxPoint point);
	bool IsOverLink(wxPoint point);
};

#endif