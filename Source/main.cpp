#include "Common.h"
#include "Memory.h"
#include "Emulator.h"
#include "Allegrex/Allegrex.h"
#include "Utilities/wxConfigEmu.h"
#include "wx/wx.h"
#include "main.h"
#include "GUI/MainEmuFrame.h"



void* hMainWindow;
IMPLEMENT_APP(pcspApp);

bool pcspApp::OnInit()
{
  if ( !wxApp::OnInit() )
        return false;
   wxImage::AddHandler(new wxPNGHandler);
   wxConfigEmu::load();
   Allegrex::cpuIntialaze();
   Memory::initialize();	
   Emulator::initialize();	
   ShowConsole(true);
   
    MainEmuFrame *frame = new MainEmuFrame((wxFrame*) NULL, wxID_ANY, "PCSP",
				wxPoint(100, 100), wxSize(800, 600));

	hMainWindow=frame->GetRenderHandle();

	
    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    
	frame->Show();
	
	
   return true;
}
void pcspApp::OnEndSession()
{
   
}

