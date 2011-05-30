#include "LinkTable.h"

#include <wx/dcclient.h>

LinkTable::LinkTable(wxWindow *parent, wxWindowID id)
: wxGrid(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN)
{
}

void LinkTable::Setup(int rows, int cols)
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
	mHighlightCoords.Set(-1, -1);

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
		attr->SetFont(font);
		attr->SetTextColour(wxColor(0,0,255));
	}
}

void LinkTable::OnMotion(wxMouseEvent &e)
{
	if(mHighlightCoords.GetRow() != -1 || mHighlightCoords.GetCol() != -1) {
		SetCellUnderline(mHighlightCoords, false);
	}

	if(IsOverLink(e.GetPosition())) {
		GetGridWindow()->SetCursor(wxCursor(wxCURSOR_HAND));

		wxGridCellCoords coords = GetCoords(e.GetPosition());
		SetCellUnderline(coords, true);
		mHighlightCoords = coords;
	} else {
		GetGridWindow()->SetCursor(wxNullCursor);
		mHighlightCoords.Set(-1, -1);
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
			SetCellUnderline(c, false);
			wxHyperlinkEvent e(this, wxEVT_COMMAND_HYPERLINK, mTargets[c.GetRow() * mNumCols + c.GetCol()]);
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
	XYToCell(x, y, coord);

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
	wxWindowDC dc(this);
	wxSize textSize = renderer->GetBestSize(*this, *attr, dc, coord.GetRow(), coord.GetCol());
	attr->DecRef();
	wxRect rect = CellToRect(coord);
	
	if(wxRect(textSize).Contains(unscrolledPos - rect.GetTopLeft())) {
		return true;
	}

	return false;
}
	
wxRect LinkTable::CalcScrolledRect(wxRect in)
{
	int x, y;

	CalcScrolledPosition(in.GetPosition().x, in.GetPosition().y, &x, &y);
	return wxRect(x, y, in.GetWidth(), in.GetHeight());
}

void LinkTable::SetCellUnderline(wxGridCellCoords coords, bool underline)
{
	wxFont font = GetCellFont(coords.GetRow(), coords.GetCol());
	font.SetUnderlined(underline);
	SetCellFont(coords.GetRow(), coords.GetCol(), font);
	GetGridWindow()->RefreshRect(CalcScrolledRect(CellToRect(coords)));
}