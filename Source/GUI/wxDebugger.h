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
#ifndef __wxDebugger__
#define __wxDebugger__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/frame.h>
#include <wx/textdlg.h>

#define wxID_RUNTOOL 1000
#define wxID_PAUSETOOL 1001
#define wxID_STEPTOOL 1002
#define wxID_RESETTOPCTOOL 1003
#define wxID_JUMPTOTOOL 1004
#define wxID_ADDBREAKPOINTTOOL 1005
#define wxID_REMOVEBREAKPOINTTOOL 1006
#define wxID_REMOVEALLBREAKPOINTSTOOL 1007
#define wxID_DUMPCODETOOL 1008

class wxDebugger : public wxFrame 
{
	private:
	
	protected:
		wxToolBar* DebugToolbar;
		wxListCtrl* m_listCtrl1;
		wxStaticLine* m_staticline1;
		wxNotebook* m_notebook1;
		wxPanel* m_panel1;
		wxListCtrl* m_listCtrl2;
		wxPanel* m_panel2;
		wxListCtrl* m_listCtrl3;
		wxPanel* m_panel3;
		wxListCtrl* m_listCtrl4;
	    void OnCloseWindow( wxCloseEvent& event );
		void OnChangeSize( wxSizeEvent& event );
		void RunToolClicked( wxCommandEvent& event );
		void PauseToolClicked( wxCommandEvent& event );
		void StepToolClicked( wxCommandEvent& event );
		void ResetToPcToolClicked( wxCommandEvent& event );
		void JumpToAddClicked( wxCommandEvent& event );
		void AddBreakPointClicked( wxCommandEvent& event );
		void RemoveBreakPointClicked( wxCommandEvent& event );
		void RemoveAllBreakPointsClicked( wxCommandEvent& event );
		void DumpCodeClicked( wxCommandEvent& event );
		void OnDisasmKeyDown( wxListEvent& event );
		void Refresh();
		int DebuggerPC;
	public:
		wxDebugger( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Pcsp Debugger"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 612,398 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);
		~wxDebugger();
		void setPCregister(int pcReg);
	
};

#endif //__wxDebugger__
