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
#include "wxMemView.h"
#include "Memory.h"


MEMORY_VIEWER::MEMORY_VIEWER( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	u32 x, y, w, h;
	if(wxConfigEmu::get("MemoryView", "X", &x) &&
	   wxConfigEmu::get("MemoryView", "Y", &y) &&
	   wxConfigEmu::get("MemoryView", "W", &w) &&
	   wxConfigEmu::get("MemoryView", "H", &h) ) //we find an record
		this->SetSize(x,y,w,h);

	
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Memory Viewer") ), wxVERTICAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Address:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer4->Add( m_staticText2, 0, wxALL, 5 );
	
	ADDRESS_TEXTFIELD = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer4->Add( ADDRESS_TEXTFIELD, 0, wxALL, 5 );
	
	GOTOADDRESS_BUTTON = new wxButton( this, wxID_ANY, wxT("Go To Address"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( GOTOADDRESS_BUTTON, 0, wxALL, 5 );
	
	wxString MEMORYSELECTION_COMBOChoices[] = { wxT("08800000 - 0BFFFFFF user main ram"), wxT("08000000 - 087FFFFF kernel main ram"), wxT("04000000 - 047FFFFF video ram"), wxT("00010000 - 00013FFF scratch pad") };
	int MEMORYSELECTION_COMBONChoices = sizeof( MEMORYSELECTION_COMBOChoices ) / sizeof( wxString );
	MEMORYSELECTION_COMBO = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, MEMORYSELECTION_COMBONChoices, MEMORYSELECTION_COMBOChoices, 0 );
	MEMORYSELECTION_COMBO->SetSelection( 0 );
	bSizer4->Add( MEMORYSELECTION_COMBO, 0, wxALL, 5 );
	
	sbSizer2->Add( bSizer4, 0, 0, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	MEMORYVIEW_LIST = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	MEMORYVIEW_LIST->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Courier New") ) );
	bSizer5->Add( MEMORYVIEW_LIST, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer2->Add( bSizer5, 1, wxEXPAND, 5 );
	
	this->SetSizer( sbSizer2 );
	this->Layout();
	
		// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW,wxCloseEventHandler(MEMORY_VIEWER::OnDestroy));
	this->Connect(wxEVT_SIZE,wxSizeEventHandler(MEMORY_VIEWER::ChangeSize));
	ADDRESS_TEXTFIELD->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MEMORY_VIEWER::UpdateAddress ), NULL, this );
	GOTOADDRESS_BUTTON->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MEMORY_VIEWER::UpdateAddress ), NULL, this );
	MEMORYSELECTION_COMBO->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MEMORY_VIEWER::UpdateAddressOnSelection ), NULL, this );
	MEMORYVIEW_LIST->Connect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( MEMORY_VIEWER::ListKeyPressed ), NULL, this );
	// Insert three columns
    wxListItem itemCol;
	itemCol.SetText(wxT("Address"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MEMORYVIEW_LIST->InsertColumn(0, itemCol);
	MEMORYVIEW_LIST->SetColumnWidth(0, 70 );
	itemCol.SetText(wxT("Value"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MEMORYVIEW_LIST->InsertColumn(1, itemCol);
	MEMORYVIEW_LIST->SetColumnWidth(1, 190 );
	itemCol.SetText(wxT("ASCII"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MEMORYVIEW_LIST->InsertColumn(2, itemCol);
	MEMORYVIEW_LIST->SetColumnWidth(2, 90 );
    //null address;
	address=0x08800000;
    sprintf(addressz, "%08X", address);
	ADDRESS_TEXTFIELD->SetValue(addressz);
	Refresh();
}

MEMORY_VIEWER::~MEMORY_VIEWER()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW,wxCloseEventHandler(MEMORY_VIEWER::OnDestroy));
	this->Disconnect(wxEVT_SIZE,wxSizeEventHandler(MEMORY_VIEWER::ChangeSize));
	ADDRESS_TEXTFIELD->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MEMORY_VIEWER::UpdateAddress ), NULL, this );
	GOTOADDRESS_BUTTON->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MEMORY_VIEWER::UpdateAddress ), NULL, this );
	MEMORYSELECTION_COMBO->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MEMORY_VIEWER::UpdateAddressOnSelection ), NULL, this );
    MEMORYVIEW_LIST->Disconnect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( MEMORY_VIEWER::ListKeyPressed ), NULL, this );
}
void MEMORY_VIEWER::setPCregister(int pcReg)
{
   address=pcReg;
   Refresh();
}
void MEMORY_VIEWER::Refresh()//TODO this needs to be finished
{
	MEMORYVIEW_LIST->DeleteAllItems();

	unsigned long addr = address;
	for ( int i = 0; i < MEMORYVIEW_LIST->GetCountPerPage(); i++)
    {   

	  wxString buf;
	  buf.Printf(wxT("%08X"), addr);  
	  MEMORYVIEW_LIST->InsertItem(i,buf);
	  if(Memory::isValid(addr))
	    buf.Printf(wxT("%02X %02X %02X %02X %02X %02X %02X %02X"),Memory::read8(addr),Memory::read8(addr+1),Memory::read8(addr+2),Memory::read8(addr+3),Memory::read8(addr+4),Memory::read8(addr+5),Memory::read8(addr+6),Memory::read8(addr+7));
	  else
		  buf.Printf(wxT("?? ?? ?? ?? ?? ?? ?? ??"));
	  MEMORYVIEW_LIST->SetItem(i,1,buf);
      if(Memory::isValid(addr))
	   buf.Printf(wxT("%c%c%c%c%c%c%c%c"),isgraph(Memory::read8(addr))   ? Memory::read8(addr)   : '.',
										 isgraph(Memory::read8(addr+1)) ? Memory::read8(addr+1) : '.',
									     isgraph(Memory::read8(addr+2)) ? Memory::read8(addr+2) : '.',
										 isgraph(Memory::read8(addr+3)) ? Memory::read8(addr+3) : '.',
										 isgraph(Memory::read8(addr+4)) ? Memory::read8(addr+4) : '.',
										 isgraph(Memory::read8(addr+5)) ? Memory::read8(addr+5) : '.',
										 isgraph(Memory::read8(addr+6)) ? Memory::read8(addr+6) : '.',
										 isgraph(Memory::read8(addr+7)) ? Memory::read8(addr+7) : '.');
	  else
		  buf.Printf(wxT("........"));
      MEMORYVIEW_LIST->SetItem(i,2,buf);
	  addr+=8;

	}
}
void MEMORY_VIEWER::UpdateAddress( wxCommandEvent& event )
{
	wxString buf  = ADDRESS_TEXTFIELD->GetValue();
    sscanf(buf, "%08X", &address);
	Refresh();
}

void MEMORY_VIEWER::UpdateAddressOnSelection( wxCommandEvent& event )
{
	if(MEMORYSELECTION_COMBO->GetSelection()==0)//user main ram
	{
            address=0x08800000;
			sprintf(addressz, "%08X", address);
			ADDRESS_TEXTFIELD->SetValue(addressz);
	}
	else if(MEMORYSELECTION_COMBO->GetSelection()==1)//kernel main ram
	{
           address=0x08000000;
		   sprintf(addressz, "%08X", address);
		   ADDRESS_TEXTFIELD->SetValue(addressz);
	}
	else if(MEMORYSELECTION_COMBO->GetSelection()==2)//video ram
	{
          address=0x04000000;
		  sprintf(addressz, "%08X", address);
		  ADDRESS_TEXTFIELD->SetValue(addressz);
	}
	else if(MEMORYSELECTION_COMBO->GetSelection()==3)//scratch pad
	{
          address=0x00010000;
		  sprintf(addressz, "%08X", address);
		  ADDRESS_TEXTFIELD->SetValue(addressz);
	}
    Refresh();
}

void MEMORY_VIEWER::ListKeyPressed(wxListEvent& event)
{
	if(event.GetKeyCode() == WXK_DOWN)
	{
       address+=8;
	   Refresh();
	}
	else if(event.GetKeyCode() == WXK_UP)
	{
      address-=8;
	  Refresh();
	}
}
void MEMORY_VIEWER::OnDestroy(wxCloseEvent& event)
{
	wxRect rc;
	rc = MEMORY_VIEWER::GetRect();
	wxConfigEmu::set("MemoryView", "X", rc.GetLeft());
	wxConfigEmu::set("MemoryView", "Y", rc.GetTop());
	wxConfigEmu::set("MemoryView", "W", rc.GetWidth()); 
	wxConfigEmu::set("MemoryView", "H", rc.GetHeight());
	event.Skip();
	
}
void MEMORY_VIEWER::ChangeSize(wxSizeEvent& event)
{

  Refresh();//refresh window when size change
  event.Skip();
}
