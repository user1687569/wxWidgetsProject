//Copyright (c) 2022 user1687569

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include "wx/animate.h"
#include "wx/aboutdlg.h"
#include "wx/artprov.h"
#include "wx/colordlg.h"
#include "wx/wfstream.h"


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};


class MyFrame : public wxFrame
{
public:
    MyFrame(wxWindow *parent, const wxWindowID id, const wxString& title,
            const wxPoint& pos, const wxSize& size, const long style);
    ~MyFrame();

    void OnAbout(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);

    void OnPlay(wxCommandEvent& event);
    void OnSetNullAnimation(wxCommandEvent& event);
    void OnSetInactiveBitmap(wxCommandEvent& event);
    void OnSetNoAutoResize(wxCommandEvent& event);
    void OnSetBgColor(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

    void OnUpdateUI(wxUpdateUIEvent& event);

#if wxUSE_FILEDLG
    void OnOpen(wxCommandEvent& event);
#endif

private:
    void RecreateAnimation(long style);

    wxAnimationCtrlBase* m_animationCtrl;

    wxDECLARE_EVENT_TABLE();    
};


enum
{
    ID_Play = 1,
    ID_SET_NULL_ANIMATION,
    ID_SET_INACTIVE_BITMAP,
    ID_SET_NO_AUTO_RESIZE,
    ID_SET_BGCOLOR,
    ID_SET_GENERIC
};


wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_PLAY, MyFrame::OnPlay)
    EVT_MENU(ID_SET_NULL_ANIMATION, MyFrame::OnSetNullAnimation)
    EVT_MENU(ID_SET_INACTIVE_BITMAP, MyFrame::OnSetInactiveBitmap)
    EVT_MENU(ID_SET_NO_AUTO_RESIZE, MyFrame::OnSetNoAutoResize)
    EVT_MENU(ID_SET_BGCOLOR, MyFrame::OnSetBgColor)

    EVT_MENU(wxID_STOP, MyFrame::OnStop)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_EXIT, MyFrame::OnQuit)

#if wxUSE_FILEDLG
    EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
#endif

    EVT_SIZE(MyFrame::OnSize)
    EVT_UPDATE_UI(wxID_ANY, MyFrame::OnUpdateUI)
wxEND_EVENT_TABLE()


wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
    if(!wxApp::OnInit())
        return false;

    MyFrame *frame = new MyFrame((wxFrame *)NULL, wxID_ANY, "Animation Demo",
                                wxDefaultPosition, wxSize(500, 400),
                                wxDEFAULT_FRAME_STYLE);
    frame->Show(true);

    return true;
}


MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    SetIcon(wxICON(sample));

    wxMenu *menuFile = new wxMenu;
    

    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}


void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}


void MyFrame::OnHello(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("Hello world from wxWidgets!");
}


