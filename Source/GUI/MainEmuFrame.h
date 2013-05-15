#pragma once

class MainEmuFrame : public wxFrame
{
public:
   	MainEmuFrame(wxFrame* parent,wxWindowID id = wxID_ANY,const wxString& title = _T("JPCSP2C"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN | wxNO_FULL_REPAINT_ON_RESIZE);


	void OnQuit(wxCommandEvent& event);
	void OnOpenFile(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnMemView(wxCommandEvent& event);
	void OnDebugger(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);
    void OnClose(wxCloseEvent &event);
	void* GetRenderHandle() {return(m_Panel->GetHandle());}

private:
	wxPanel* m_Panel;
	wxBoxSizer* sizerPanel;
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};
