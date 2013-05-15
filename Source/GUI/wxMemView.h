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
#ifndef __wxMemView__
#define __wxMemView__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/statbox.h>
#include <wx/frame.h>


class MEMORY_VIEWER : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText2;
		wxTextCtrl* ADDRESS_TEXTFIELD;
		wxButton* GOTOADDRESS_BUTTON;
		wxChoice* MEMORYSELECTION_COMBO;
		wxListCtrl* MEMORYVIEW_LIST;
		
        void UpdateAddress( wxCommandEvent& event );
	    void UpdateAddressOnSelection( wxCommandEvent& event );
		void ListKeyPressed( wxListEvent& event );
		void OnDestroy(wxCloseEvent& event);
		void ChangeSize(wxSizeEvent& event);
		void Refresh();
	    unsigned int address;
	    char addressz[9];
	
	public:
		MEMORY_VIEWER( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Memory Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 578,384 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MEMORY_VIEWER();
		void setPCregister(int pcReg);
	
};

#endif //__wxMemView__
