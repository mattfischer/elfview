#include "App.h"

App::App()
{
}

bool App::OnInit()
{
	mFrameMain = new FrameMain(NULL, -1);
	mFrameMain->Show();

	return true;
}

IMPLEMENT_APP(App)