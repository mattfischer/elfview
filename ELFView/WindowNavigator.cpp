#include "WindowNavigator.h"

WindowNavigator::WindowNavigator(wxWindow *parent, wxWindowID id)
: wxNotebook(parent, id)
{
	mSectionList = new wxListBox(this, -1);
	mSegmentList = new wxListBox(this, -1);

	AddPage(mSectionList, "Sections");
	AddPage(mSegmentList, "Segments");
}