#include "Common.h"
#include "wx/wx.h"
#include "MainEmuFrame.h"

#include "GUI/wxMemView.h"
#include "GUI/wxDebugger.h"
#include "Memory.h"
#include "Emulator.h"
#include "Allegrex/Allegrex.h"
#include "Utilities/wxConfigEmu.h"

#include "Memory.h"
#include "Video.h"
#include "HLE/Kernel/Types/ThreadWaitInfo.h"
#include "HLE/Kernel/Types/SceKernelCallbackInfo.h"
#include "HLE/Kernel/Types/SceKernelThreadInfo.h"
#include "HLE/ThreadMan.h"
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT,
	MEMVIEW = 2,
	DEBUGGER = 3,
	OPENFILE=4,
	RUN=5
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MainEmuFrame, wxFrame)
    EVT_CLOSE(MainEmuFrame::OnClose)
    EVT_MENU(Minimal_Quit,  MainEmuFrame::OnQuit)
    EVT_MENU(Minimal_About, MainEmuFrame::OnAbout)
	EVT_MENU(MEMVIEW , MainEmuFrame::OnMemView)
	EVT_MENU(DEBUGGER, MainEmuFrame::OnDebugger)
	EVT_MENU(OPENFILE,MainEmuFrame::OnOpenFile)
	EVT_MENU(RUN,MainEmuFrame::OnRun)
END_EVENT_TABLE()

MainEmuFrame::MainEmuFrame(wxFrame* parent,
		                   wxWindowID id,
		                   const wxString& title,
		                   const wxPoint& pos,
		                   const wxSize& size,
		                   long style): wxFrame(parent, id, title, pos, size, style)
{
    // set the frame icon
    SetIcon(wxICON(sample));


    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
	wxMenu *DebugMenu = new wxMenu;
	wxMenu *Emulation = new wxMenu;


    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));
    fileMenu->Append(OPENFILE,_T("Open File"));
    fileMenu->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
    DebugMenu->Append(DEBUGGER,_T("Debugger"));
	DebugMenu->Append(MEMVIEW,_T("Memory Viewer"), _T("Enter's memory viewer"));
	Emulation->Append(RUN,_T("Run"));
    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(Emulation,_T("Emulation"));
	menuBar->Append(DebugMenu, _T("Debugger"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);



    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWidgets!"));

	m_Panel = new wxPanel(this);
	sizerPanel = new wxBoxSizer(wxHORIZONTAL);
	m_Panel->SetSizer(sizerPanel);

}
void MainEmuFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
	video::Finalize();
    Close(true);
    PostQuitMessage(0);

}
void MainEmuFrame::OnClose(wxCloseEvent& event)
{
	// Don't forget the skip of the window won't be destroyed
	event.Skip();
	video::Finalize();
	PostQuitMessage(0);
    
}

void MainEmuFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format(
                    _T("Welcome to %s!\n")
                    _T("\n")
                    _T("This is the minimal wxWidgets sample\n")
                    _T("running under %s."),
                    wxVERSION_STRING,
                    wxGetOsDescription().c_str()
                 ),
                 _T("About wxWidgets minimal sample"),
                 wxOK | wxICON_INFORMATION,
                 this);
}
void MainEmuFrame::OnMemView(wxCommandEvent& WXUNUSED(event))
{
	MEMORY_VIEWER *frame = new MEMORY_VIEWER(NULL,wxID_ANY,"Memory Viewer",wxDefaultPosition,wxSize(578,384),(2048|64|1024|512|4096|536870912|4194304|524288));
	frame->Show(true);
	//pspRegs.pc=0x08900000;//fake!!!
	frame->setPCregister(pspRegs.pc);
}
void MainEmuFrame::OnDebugger(wxCommandEvent& WXUNUSED(event))
{

	wxDebugger *frame = new wxDebugger(NULL);
	frame->Show(true);
	//pspRegs.pc=0x08900000;//fake!!!
	frame->setPCregister(pspRegs.pc);
}
void MainEmuFrame::OnOpenFile(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog *OpenDialog = new wxFileDialog(
		this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
		_T("PBP Files (*.pbp)|*.pbp|Elf Files(*.elf)|*.elf")
		,
		wxFD_OPEN, wxDefaultPosition);
  	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		wxString filename = OpenDialog->GetPath();
		Emulator::load(OpenDialog->GetPath(),ifstream(OpenDialog->GetPath(),ios::in | ios::binary));
	}

}
//TODO timer is probably not good!....
u64 emuReferenceTime;
void MainEmuFrame::OnRun(wxCommandEvent& WXUNUSED(event))
{
	u64 freq;

	u32 lastCycles=0;

	u64 cntLast;
	u64 cntEnd;

	QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
	QueryPerformanceCounter((LARGE_INTEGER *)&cntLast);

	video::Initialize();
	for (;;) {
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//for(;;) 
			//{
			pspCpu->Execute();

			QueryPerformanceCounter((LARGE_INTEGER *)&cntEnd);

			u64 elapsed = cntEnd - cntLast;
			cntLast = cntEnd;

	//		u64 refTime = MulDiv64(elapsed,1000000000,freq);
            emuReferenceTime+=cntEnd;	
			//lastCycles = pspRegs.cycle;
			ThreadMan::step();
			video::Step();//temp hack...
			//}

		}
	}

}




