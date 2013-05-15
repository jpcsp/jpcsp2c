/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcsp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Common.h"
#include "wxDebugger.h"
#include "wx/mstream.h"
#include "Allegrex/Allegrex.h"
#include "Memory.h"
#include "Allegrex/DisAllegrex.h"
//#include "Allegrex/AllegrexInterpreter.h"
#include "../Resources/PlayIcon_png.cpp"
#include "../Resources/PauseIcon_png.cpp"
#include "../Resources/DeleteAllBreakpointsIcon_png.cpp"
#include "../Resources/DeleteBreakpointIcon_png.cpp"
#include "../Resources/DumpIcon_png.cpp"
#include "../Resources/JumpToIcon_png.cpp"
#include "../Resources/NewBreakpointIcon_png.cpp"
#include "../Resources/ResetToPcIcon_png.cpp"
#include "../Resources/StepIntoIcon_png.cpp"
#include "HLE/Kernel/Types/ThreadWaitInfo.h"
#include "HLE/Kernel/Types/SceKernelCallbackInfo.h"
#include "HLE/Kernel/Types/SceKernelThreadInfo.h"
#include "HLE/ThreadMan.h"
///////////////////////////////////////////////////////////////////////////
#define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name, sizeof(name))
inline wxBitmap _wxGetBitmapFromMemory(const unsigned char* data, int length)
{
	wxMemoryInputStream is(data, length);
	return(wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1));
}

wxDebugger::wxDebugger( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame( parent, id, title, pos, size, style )
{
	u32 x, y, w, h;
	if(wxConfigEmu::get("Debugger", "X", &x) &&
	   wxConfigEmu::get("Debugger", "Y", &y) &&
	   wxConfigEmu::get("Debugger", "W", &w) &&
	   wxConfigEmu::get("Debugger", "H", &h) ) //we find an record
		this->SetSize(x,y,w,h);

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	DebugToolbar = this->CreateToolBar( wxTB_HORZ_TEXT, wxID_ANY ); 
	DebugToolbar->AddTool( wxID_RUNTOOL, wxT("Run"),  wxGetBitmapFromMemory(PlayIcon_png), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	DebugToolbar->AddTool( wxID_PAUSETOOL, wxT("Pause"), wxGetBitmapFromMemory(PauseIcon_png), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	DebugToolbar->AddSeparator();
	DebugToolbar->AddTool( wxID_STEPTOOL, wxT("Step"), wxGetBitmapFromMemory(StepIntoIcon_png), wxNullBitmap, wxITEM_NORMAL, wxT("Step Code "), wxEmptyString );
	DebugToolbar->AddTool( wxID_RESETTOPCTOOL, wxT("To Pc"), wxGetBitmapFromMemory(ResetToPc_png), wxNullBitmap, wxITEM_NORMAL, wxT("Reset To PC counter"), wxEmptyString );
	DebugToolbar->AddTool( wxID_JUMPTOTOOL, wxT("Jump"), wxGetBitmapFromMemory(JumpTo_png), wxNullBitmap, wxITEM_NORMAL, wxT("Jump to a specific Address"), wxEmptyString );
	DebugToolbar->AddSeparator();
	DebugToolbar->AddTool( wxID_ADDBREAKPOINTTOOL, wxT("Add Br"), wxGetBitmapFromMemory(NewBreakpointIcon_png), wxNullBitmap, wxITEM_NORMAL, wxT("Add BreakPoint"), wxEmptyString );
	DebugToolbar->AddTool( wxID_REMOVEBREAKPOINTTOOL, wxT("Rem Br"), wxGetBitmapFromMemory(DeleteBreakpointIcon_png), wxNullBitmap, wxITEM_NORMAL, wxT("Remove BreakPoint"), wxEmptyString );
	DebugToolbar->AddTool( wxID_REMOVEALLBREAKPOINTSTOOL, wxT("Rem All"), wxGetBitmapFromMemory(DeleteAllBreakpointsIcon_png), wxNullBitmap, wxITEM_NORMAL, wxT("Remove All BreakPoints"), wxEmptyString );
	DebugToolbar->AddSeparator();
	DebugToolbar->AddTool( wxID_DUMPCODETOOL, wxT("Dump"), wxGetBitmapFromMemory(Dump_png), wxNullBitmap, wxITEM_NORMAL, wxT("Dump Code to File"), wxEmptyString );
	DebugToolbar->Realize();
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_listCtrl1->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Courier New") ) );
	bSizer2->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer2->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl2 = new wxListCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_listCtrl2->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Courier New") ) );
	bSizer5->Add( m_listCtrl2, 1, wxALL|wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer5 );
	m_panel1->Layout();
	bSizer5->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, wxT("GPR"), true );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl3 = new wxListCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_listCtrl3->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Courier New") ) );
	bSizer6->Add( m_listCtrl3, 1, wxALL|wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer6 );
	m_panel2->Layout();
	bSizer6->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, wxT("COP0"), false );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl4 = new wxListCtrl( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_listCtrl4->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Courier New") ) );
	bSizer7->Add( m_listCtrl4, 1, wxALL|wxEXPAND, 5 );
	
	m_panel3->SetSizer( bSizer7 );
	m_panel3->Layout();
	bSizer7->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("FPU"), false );
	
	bSizer2->Add( m_notebook1, 0, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxDebugger::OnCloseWindow ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( wxDebugger::OnChangeSize ) );
	this->Connect( wxID_RUNTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::RunToolClicked ) );
	this->Connect( wxID_PAUSETOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::PauseToolClicked ) );
	this->Connect( wxID_STEPTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::StepToolClicked ) );
	this->Connect( wxID_RESETTOPCTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::ResetToPcToolClicked ) );
	this->Connect( wxID_JUMPTOTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::JumpToAddClicked ) );
	this->Connect( wxID_ADDBREAKPOINTTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::AddBreakPointClicked ) );
	this->Connect( wxID_REMOVEBREAKPOINTTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::RemoveBreakPointClicked ) );
	this->Connect( wxID_REMOVEALLBREAKPOINTSTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::RemoveAllBreakPointsClicked ) );
	this->Connect( wxID_DUMPCODETOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::DumpCodeClicked ) );
	m_listCtrl1->Connect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( wxDebugger::OnDisasmKeyDown ), NULL, this );

    //add the disasm columns	
    wxListItem itemCol;
	itemCol.SetText(wxT("Br"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl1->InsertColumn(0, itemCol);
	m_listCtrl1->SetColumnWidth(0, 30 );
	itemCol.SetText(wxT("Address"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl1->InsertColumn(1, itemCol);
	m_listCtrl1->SetColumnWidth(1, 70  );
	itemCol.SetText(wxT("Code"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl1->InsertColumn(2, itemCol);
	m_listCtrl1->SetColumnWidth(2, 70  );
    itemCol.SetText(wxT("Instruction"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl1->InsertColumn(3, itemCol);
	m_listCtrl1->SetColumnWidth(3, 350); 
  
    //add GPR columns
    itemCol.SetText(wxT("Register"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl2->InsertColumn(0, itemCol);
	m_listCtrl2->SetColumnWidth(0, 80 );
	itemCol.SetText(wxT("Value"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl2->InsertColumn(1, itemCol);
	m_listCtrl2->SetColumnWidth(1, 70 );
    wxString buf;
	buf.Printf(wxT("pc"));
	m_listCtrl2->InsertItem(0,buf);
	buf.Printf(wxT("lo"));
	m_listCtrl2->InsertItem(1,buf);
	buf.Printf(wxT("hi"));
	m_listCtrl2->InsertItem(2,buf);
	for(int i=0; i<32; i++)
	{
      buf.Printf(wxT("%s"),GPR_REG[i]);
	  m_listCtrl2->InsertItem(i+3,buf);
	}
    //COP0 regs
    itemCol.SetText(wxT("Register"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl3->InsertColumn(0, itemCol);
	m_listCtrl3->SetColumnWidth(0, 80 );
	itemCol.SetText(wxT("Value"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl3->InsertColumn(1, itemCol);
	m_listCtrl3->SetColumnWidth(1, 70 );
	for(int i=0; i<32; i++)
	{
      buf.Printf(wxT("%s"),COP0_REG[i]);
	  m_listCtrl3->InsertItem(i,buf);
	}
	//FPU regs
	itemCol.SetText(wxT("Register"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl4->InsertColumn(0, itemCol);
	m_listCtrl4->SetColumnWidth(0, 80 );
	itemCol.SetText(wxT("Value"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	m_listCtrl4->InsertColumn(1, itemCol);
	m_listCtrl4->SetColumnWidth(1, 70 );
	for(int i=0; i<32; i++)
	{
      buf.Printf(wxT("%s"),COP1_REG_FP[i]);
	  m_listCtrl4->InsertItem(i,buf);
	}
	buf.Printf(wxT("fpuC0"));
	m_listCtrl4->InsertItem(32,buf);
    buf.Printf(wxT("fpuC31"));
	m_listCtrl4->InsertItem(33,buf);
    Refresh();
}

wxDebugger::~wxDebugger()
{
		// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxDebugger::OnCloseWindow ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( wxDebugger::OnChangeSize ) );
	this->Disconnect( wxID_RUNTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::RunToolClicked ) );
	this->Disconnect( wxID_PAUSETOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::PauseToolClicked ) );
	this->Disconnect( wxID_STEPTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::StepToolClicked ) );
	this->Disconnect( wxID_RESETTOPCTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::ResetToPcToolClicked ) );
	this->Disconnect( wxID_JUMPTOTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::JumpToAddClicked ) );
	this->Disconnect( wxID_ADDBREAKPOINTTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::AddBreakPointClicked ) );
	this->Disconnect( wxID_REMOVEBREAKPOINTTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::RemoveBreakPointClicked ) );
	this->Disconnect( wxID_REMOVEALLBREAKPOINTSTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::RemoveAllBreakPointsClicked ) );
	this->Disconnect( wxID_DUMPCODETOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxDebugger::DumpCodeClicked ) );
	m_listCtrl1->Disconnect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( wxDebugger::OnDisasmKeyDown ), NULL, this );
}
void wxDebugger::setPCregister(int pcReg)
{
   DebuggerPC=pcReg;
   Refresh();
}
void wxDebugger::OnCloseWindow( wxCloseEvent& event )
{
	wxRect rc;
	rc = wxDebugger::GetRect();
	wxConfigEmu::set("Debugger", "X", rc.GetLeft());
	wxConfigEmu::set("Debugger", "Y", rc.GetTop());
	wxConfigEmu::set("Debugger", "W", rc.GetWidth()); 
	wxConfigEmu::set("Debugger", "H", rc.GetHeight()); 
	event.Skip();
	
}

void wxDebugger::OnChangeSize( wxSizeEvent& event )
{
	Refresh();
	event.Skip();
}

void wxDebugger::RunToolClicked( wxCommandEvent& event )
{
	// TODO: Implement RunToolClicked
}

void wxDebugger::PauseToolClicked( wxCommandEvent& event )
{
	// TODO: Implement PauseToolClicked
}

void wxDebugger::StepToolClicked( wxCommandEvent& event )
{
	//TODO better way to handle it...
	pspCpu->Execute();
	ThreadMan::step();
	DebuggerPC=pspRegs.pc;
	Refresh();
}

void wxDebugger::ResetToPcToolClicked( wxCommandEvent& event )
{
	DebuggerPC=pspRegs.pc;
	Refresh();
}

void wxDebugger::JumpToAddClicked( wxCommandEvent& event )
{
	wxString pcadd = wxString::Format(wxT("%x"),pspRegs.pc);
	wxTextEntryDialog dialog(this,
                             wxT("Enter an address (hex)"),
                             wxT("Jump To Address"),
                             wxT(pcadd),
                             wxOK | wxCANCEL);
    if (dialog.ShowModal() == wxID_OK)
	{
		wxString resultstring = dialog.GetValue(); 
		long num;
		resultstring.ToLong(&num,16);
		DebuggerPC=num;
		Refresh();
		
	}
}

void wxDebugger::AddBreakPointClicked( wxCommandEvent& event )
{
	// TODO: Implement AddBreakPointClicked
}

void wxDebugger::RemoveBreakPointClicked( wxCommandEvent& event )
{
	// TODO: Implement RemoveBreakPointClicked
}

void wxDebugger::RemoveAllBreakPointsClicked( wxCommandEvent& event )
{
	// TODO: Implement RemoveAllBreakPointsClicked
}

void wxDebugger::DumpCodeClicked( wxCommandEvent& event )
{
	// TODO: Implement DumpCodeClicked
}

void wxDebugger::OnDisasmKeyDown( wxListEvent& event )
{
    if(event.GetKeyCode() == WXK_DOWN)
	{
       DebuggerPC+=4;
	   Refresh();
	}
	else if(event.GetKeyCode() == WXK_UP)
	{
      DebuggerPC-=4;
	  Refresh();
	}
}
void wxDebugger::Refresh()
{
	m_listCtrl1->DeleteAllItems();
	int pc;
	pc = DebuggerPC;
	for ( int i = 0; i < m_listCtrl1->GetCountPerPage(); i++)
    {
		if(true)//TODO: do that properly when we add breakpoint support
		{
            wxString buf;
			buf.Printf(wxT("")); 
			m_listCtrl1->InsertItem(i,buf);
		}
		else
		{
            wxString buf;
			buf.Printf(wxT("*")); 
			m_listCtrl1->InsertItem(i,buf);
		}
		 
		 if(Memory::isValid(pc))
		 {
			 wxString buf;
			 buf.Printf(wxT("%08X"), pc); 
			 m_listCtrl1->SetItem(i,1,buf);
			 int code =Memory::read32(pc);
			 buf.Printf(wxT("%08X"), code);
			 m_listCtrl1->SetItem(i,2,buf);
			 //TODO:disasm opcodes here!
			 wxString buf2 = DisAllegrex::disAllegrex(code,pc);
			 buf.Printf(wxT("%s"), buf2.c_str());
			 m_listCtrl1->SetItem(i,3,buf);
			 
		 }
		 else
		 {
            wxString buf;
			 buf.Printf(wxT("%08X"), pc); 
			 m_listCtrl1->SetItem(i,1,buf);
			 buf.Printf(wxT("00000000"));
			 m_listCtrl1->SetItem(i,2,buf);
             buf.Printf(wxT("invalid address"));
			 m_listCtrl1->SetItem(i,3,buf);
		 }
		 pc+=4;
	}
	//refresh registers
	wxString buf;
	buf.Printf(wxT("%08X"), pspRegs.pc); 
	m_listCtrl2->SetItem(0,1,buf);
	buf.Printf(wxT("%08X"), pspRegs.GPR[32]); 
	m_listCtrl2->SetItem(1,1,buf);
	buf.Printf(wxT("%08X"), pspRegs.GPR[33]); 
	m_listCtrl2->SetItem(2,1,buf);
    for(int i=0; i<32; i++)
	{
	  buf.Printf(wxT("%08X"),pspRegs.GPR[i]);
	  m_listCtrl2->SetItem(i+3,1,buf);
	}
    for(int i=0; i<32; i++)
	{
	  buf.Printf(wxT("%08X"),pspRegs.COP0[i]);
	  m_listCtrl3->SetItem(i,1,buf);
	}
	for(int i=0; i<32; i++)
	{
      buf.Printf(wxT("%08X"),pspRegs.FPR[i]);
	  m_listCtrl4->SetItem(i,1,buf);
	}
    buf.Printf(wxT("%08X"),pspRegs.FPRc0);
	m_listCtrl4->SetItem(32,1,buf);
    buf.Printf(wxT("%08X"),pspRegs.FPRc31);
	m_listCtrl4->SetItem(33,1,buf);

}
