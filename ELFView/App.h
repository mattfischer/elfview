#ifndef APP_H
#define APP_H

#include <wx/app.h>

#include "FrameMain.h"

class App : public wxApp
{
public:
	App();

	virtual bool OnInit();

protected:
	FrameMain *mFrameMain;
};
#endif