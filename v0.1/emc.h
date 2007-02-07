/*
 * emc.h
 * emulecollection support for aMule using wxwidgets and ec
 * written by Johannes Krampf <<wuischke@amule.org>>
 */

#include <wx/wx.h> 
#include <wx/textfile.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/statline.h>
#include <wx/config.h>
#include <wx/textdlg.h>
#include <wx/progdlg.h>
#include <wx/sizer.h>

#include "CFile.h"
#include "ec/cpp/RemoteConnect.h"
#include "common/MD5Sum.h"

#include "utils/aLinkCreator/src/md4.h"
#include "utils/aLinkCreator/src/ed2khash.h"

class emc: public wxApp
{
  public:
    virtual bool OnInit();
};

class OptionsDialog: public wxDialog
{
public:

    OptionsDialog(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnCancel(wxCommandEvent& event);
    void OnOK(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};

class MyFrame: public wxFrame
{
public:

    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    wxButton *btn_download;

    void OnQuit(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnDownload(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnAddLink(wxCommandEvent& WXUNUSED(event));
    void OnAddFile(wxCommandEvent& WXUNUSED(event));
    void OnAbout(wxCommandEvent& WXUNUSED(event));
    void OnOptions(wxCommandEvent& WXUNUSED(event));
    DECLARE_EVENT_TABLE()
};

const int ID_TEXT = 10000;
const int ID_FLST  = 10001;
wxString emcVERSION = wxT("0.1");

wxListCtrl *flst;
CRemoteConnect *ECconn;

    wxProgressDialog *m_progressBar;
    bool m_goAhead;

void OpenFile(wxString filename);
bool OpenTextFile(wxString filename);
void AddtoList(wxString filename, wxString filesize, wxString filehash);
void AddtoList(char* filename, int filesize, CMD4Hash filehash);
bool AddLink(wxString link);
wxString GetColItemText(long index, int col);
bool ConnectOnStart();
void AddLinkToAMule(wxString filename, wxString filesize, wxString filehash);
void AddFile(wxString filename);

enum
{
    ID_Quit      = 1,
    ID_Open      = 2,
    ID_Download  = 3,
    ID_Delete    = 4,
    ID_AddLink   = 5,
    ID_AddFile   = 6,
    ID_About     = 7,
    ID_OK        = 8,
    ID_QuitDlg   = 9,
    ID_Options   = 10,
    ID_TextHost  = 50,
    ID_TextPort  = 51,
    ID_TextLogin = 52,
    ID_TextPass  = 53
};
