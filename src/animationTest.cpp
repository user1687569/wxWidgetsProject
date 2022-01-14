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
    ID_PLAY = 1,
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


MyFrame::MyFrame(wxWindow *parent,
                const wxWindowID id,
                const wxString& title,
                const wxPoint& pos,
                const wxSize& size,
                const long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    SetIcon(wxICON(wxWidgetsIcon));

    wxMenu *fileMenu = new wxMenu;
#if wxUSE_FILEDLG
    fileMenu->Append(wxID_OPEN, "&Open Animation\tCtrl+O", "Loads an animation");
#endif
    fileMenu->Append(wxID_EXIT);

    wxMenu *playMenu = new wxMenu;
    playMenu->Append(ID_PLAY, "Play\tCtrl+P", "Play the animation");
    playMenu->Append(wxID_STOP, "Stop\tCtrl+S", "Stop the animation");
    playMenu->AppendSeparator();
    playMenu->Append(ID_SET_NULL_ANIMATION, "Set null animation",
                    "Sets the empty animation in the control");
    playMenu->AppendCheckItem(ID_SET_INACTIVE_BITMAP, "Set inactive bitmap",
                            "Sets an inactive bitmap for the control");
    playMenu->AppendCheckItem(ID_SET_NO_AUTO_RESIZE, "Set no autoresize",
                            "Tells the control not to resize automatically");
    playMenu->Append(ID_SET_BGCOLOR, "Set background colour",
                    "Sets the background colour of the control");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(playMenu, "&Animation");
    menuBar->Append(helpMenu, "&Help");
    
    SetMenuBar( menuBar );

#if wxUSE_STATUSBAR
    CreateStatusBar();
#endif

    wxSizer *sz = new wxBoxSizer(wxVERTICAL);
    sz->Add(new wxStaticText(this, wxID_ANY, "wxAnimationCtrl:"),
            wxSizerFlags().Centre().Border());
    
    m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
    if(m_animationCtrl->LoadFile("./../res/throbber.gif"))
        m_animationCtrl->Play();

    sz->Add(m_animationCtrl, wxSizerFlags().Centre().Border());
    SetSizer(sz);
}


MyFrame::~MyFrame()
{
}


void MyFrame::OnPlay(wxCommandEvent& WXUNUSED(event))
{
    if(!m_animationCtrl->Play())
    {
        wxLogError("Invalid animation");
    }
}


void MyFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
    m_animationCtrl->Stop();
}


void MyFrame::OnSetNullAnimation(wxCommandEvent& WXUNUSED(event))
{
    m_animationCtrl->SetAnimation(wxNullAnimation);
}


void MyFrame::OnSetInactiveBitmap(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        wxBitmap bitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE);
        m_animationCtrl->SetInactiveBitmap(bitmap);
    }
    else
        m_animationCtrl->SetInactiveBitmap(wxNullBitmap);
}


void MyFrame::OnSetNoAutoResize(wxCommandEvent& event)
{
    long style = wxAC_DEFAULT_STYLE | (event.IsChecked() ? wxAC_NO_AUTORESIZE : 0);

    if(style != m_animationCtrl->GetWindowStyle())
    {
        RecreateAnimation(style);
    }
}


void MyFrame::OnSetBgColor(wxCommandEvent& WXUNUSED(event))
{
    wxColour colour = wxGetColourFromUser(this, m_animationCtrl->GetBackgroundColour(),
                                            "Choose the background colour");
    if(colour.IsOk())
        m_animationCtrl->SetBackgroundColour(colour);
}


void MyFrame::RecreateAnimation(long style)
{
    wxAnimation curr;
    wxBitmap inactive = m_animationCtrl->GetInactiveBitmap();
    wxColour bg = m_animationCtrl->GetBackgroundColour();

    wxAnimationCtrlBase *old = m_animationCtrl;

    m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY, curr,
                                        wxDefaultPosition, wxDefaultSize, style);

    GetSizer()->Replace(old, m_animationCtrl);
    delete old;

    m_animationCtrl->SetInactiveBitmap(inactive);
    m_animationCtrl->SetBackgroundColour(bg);

    GetSizer()->Layout();
}


void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
    info.SetName(_("wxAnimationCtrl and wxAnimation sample"));
    info.SetDescription(_("This sample program demonstrates the usage of wxAnimationCtrl"));
    info.SetCopyright("(C) 2022 Blockchain Lee");

    info.AddDeveloper("Lee");

    wxAboutBox(info, this);
}


#if wxUSE_FILEDLG
void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog dialog(this, "Please choose an animation",
                        wxEmptyString, wxEmptyString, ".gif;*.ani", wxFD_OPEN);

    if(dialog.ShowModal() == wxID_OK)
    {
        wxString fileName(dialog.GetPath());
        
        wxAnimation temp(m_animationCtrl->CreateAnimation());
        if(!temp.LoadFile(fileName))
        {
            wxLogError("Sorry, this animation is not a valid format for wxAnimation.");
            return;
        }

        m_animationCtrl->SetAnimation(temp);
        m_animationCtrl->Play();

        GetSizer()->Layout();
    }
}
#endif


void MyFrame::OnUpdateUI(wxUpdateUIEvent& WXUNUSED(event))
{
    GetMenuBar()->FindItem(wxID_STOP)->Enable(m_animationCtrl->IsPlaying());
    GetMenuBar()->FindItem(ID_PLAY)->Enable(!m_animationCtrl->IsPlaying());
    GetMenuBar()->FindItem(ID_SET_NO_AUTO_RESIZE)->Enable(!m_animationCtrl->IsPlaying());
}

