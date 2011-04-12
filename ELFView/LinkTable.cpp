#include "LinkTable.h"

#include <wx/dcclient.h>

LinkTable::LinkTable(wxWindow *parent, wxWindowID id)
: wxGrid(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN)
{
	mClientData = NULL;
}

void LinkTable::Setup(int rows, int cols, void *clientData)
{
	CreateGrid(rows, cols, wxGrid::wxGridSelectRows);
	HideRowLabels();
	UseNativeColHeader();
	EnableGridLines(false);
	SetCellHighlightPenWidth(0);
	EnableEditing(false);
	DisableDragRowSize();

	mNumCols = cols;
	mTargets.SetCount(rows * cols);
	mDownCoords.Set(-1, -1);
	mClientData = clientData;

	GetGridWindow()->Bind(wxEVT_MOTION, &LinkTable::OnMotion, this);
	GetGridWindow()->Bind(wxEVT_LEFT_UP, &LinkTable::OnLeft, this);
	GetGridWindow()->Bind(wxEVT_LEFT_DOWN, &LinkTable::OnLeft, this);
}

void LinkTable::SetColumnLabel(int col, wxString label)
{
	SetColLabelValue(col, label);
}

void LinkTable::SetCell(int row, int col, wxString text, wxString target)
{
	mTargets.Item(row * mNumCols + col) = target;
	SetCellValue(row, col, text);

	if(target != "") {
		wxGridCellAttr *attr = GetOrCreateCellAttr(row, col);
		wxFont font = attr->GetFont();
		font.SetUnderlined(true);
		attr->SetFont(font);
		attr->SetTextColour(wxColor(0,0,255));
	}
}

void LinkTable::OnMotion(wxMouseEvent &e)
{
	if(IsOverLink(e.GetPosition())) {
		GetGridWindow()->SetCursor(wxCursor(wxCURSOR_HAND));
	} else {
		GetGridWindow()->SetCursor(wxNullCursor);
	}
}

void LinkTable::OnLeft(wxMouseEvent &e)
{
	if(e.GetEventType() == wxEVT_LEFT_DOWN) {
		mDownCoords = GetCoords(e.GetPosition());
		if(IsOverLink(e.GetPosition())) {
			return;
		}
	} else if(e.GetEventType() == wxEVT_LEFT_UP) {
		wxGridCellCoords c = mDownCoords;
		mDownCoords.Set(-1, -1);

		if(c == GetCoords(e.GetPosition()) && IsOverLink(e.GetPosition())) {
			wxHyperlinkEvent e(this, wxEVT_COMMAND_HYPERLINK, mTargets[c.GetRow() * mNumCols + c.GetCol()]);
			e.SetClientData(mClientData);
			ProcessEvent(e);
			return;
		}
	}

	e.Skip();
}

wxGridCellCoords LinkTable::GetCoords(wxPoint point)
{
	wxGridCellCoords coord;
	int x, y;
	CalcUnscrolledPosition(point.x, point.y, &x, &y);
	wxPoint unscrolledPos(x, y);
	XYToCell(unscrolledPos.x, unscrolledPos.y, coord);

	return coord;
}

bool LinkTable::IsOverLink(wxPoint point)
{
	wxGridCellCoords coord = GetCoords(point);
	int x, y;
	CalcUnscrolledPosition(point.x, point.y, &x, &y);
	wxPoint unscrolledPos(x, y);

	if(coord.GetCol() == -1 || coord.GetRow() == -1) {
		return false;
	}

	if(mTargets.Item(coord.GetRow() * mNumCols + coord.GetCol()) == "") {
		return false;
	}

	wxGridCellAttr *attr = GetOrCreateCellAttr(coord.GetRow(), coord.GetCol());
	wxGridCellRenderer *renderer = GetCellRenderer(coord.GetRow(), coord.GetCol());
	wxSize textSize = renderer->GetBestSize(*this, *attr, wxWindowDC(this), coord.GetRow(), coord.GetCol());
	attr->DecRef();
	wxRect rect = CellToRect(coord);
	
	if(wxRect(textSize).Contains(unscrolledPos - rect.GetTopLeft())) {
		return true;
	}

	return false;
}
	
