/*
 * emc.cpp
 * emulecollection support for aMule using wxwidgets and ec
 * written by Johannes Krampf <<wuischke@amule.org>>
 */

#include <emc.h>

//Icon
#include "amule.xpm"


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(ID_Quit, MyFrame::OnQuit)
    EVT_BUTTON(ID_Open, MyFrame::OnOpen)
    EVT_BUTTON(ID_Download, MyFrame::OnDownload)
    EVT_BUTTON(ID_Delete, MyFrame::OnDelete)
    EVT_BUTTON(ID_AddLink, MyFrame::OnAddLink)
    EVT_BUTTON(ID_AddFile, MyFrame::OnAddFile)
    EVT_BUTTON(ID_About, MyFrame::OnAbout)
    EVT_BUTTON(ID_Options, MyFrame::OnOptions)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(OptionsDialog, wxDialog)
    EVT_BUTTON(ID_QuitDlg, OptionsDialog::OnCancel)
    EVT_BUTTON(ID_OK, OptionsDialog::OnOK)
END_EVENT_TABLE()

IMPLEMENT_APP(emc)

OptionsDialog::OptionsDialog(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxDialog(parent, -1, title, pos, size)
{

  wxIcon icon = wxIcon(amule__icon);
  SetIcon (icon);

 wxConfig *config = new wxConfig(wxT("amule-emc"));
  wxString host, login, pass;
  config->Read(wxT("host"), &host, wxT("localhost"));
  int port = config->Read(wxT("port"), 4712);
  config->Read(wxT("login") , &login , wxT("amule"));
  config->Read(wxT("password") , &pass , wxT("password"));

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

     wxStaticBoxSizer *ecsettings = new wxStaticBoxSizer(wxVERTICAL, this, wxT("EC settings"));

      wxFlexGridSizer *ec_set = new wxFlexGridSizer(4, 2, 5, 5);

      ec_set->Add(new wxStaticText(this, -1, wxT("Host:")), 0, wxALIGN_CENTER_VERTICAL);
      ec_set->Add(new wxTextCtrl(this, ID_TextHost, host, wxDefaultPosition, wxSize(150,-1)));

      ec_set->Add(new wxStaticText(this, -1, wxT("Port:")), 0, wxALIGN_CENTER_VERTICAL);
      ec_set->Add(new wxTextCtrl(this, ID_TextPort, wxString::Format(wxT("%d"), port), wxDefaultPosition, wxSize(150,-1) ));

      ec_set->Add(new wxStaticText(this, -1, wxT("Login:")), 0, wxALIGN_CENTER_VERTICAL);
      wxTextCtrl *txtctrl_login = new wxTextCtrl(this, ID_TextLogin, login, wxDefaultPosition, wxSize(150,-1));
      txtctrl_login->Disable();
      ec_set->Add(txtctrl_login);

      ec_set->Add(new wxStaticText(this, -1, wxT("Password:")), 0, wxALIGN_CENTER_VERTICAL);
      ec_set->Add(new wxTextCtrl(this, ID_TextPass, pass, wxDefaultPosition, wxSize(150,-1), wxTE_PASSWORD));

     ecsettings->Add(ec_set, 0, wxALIGN_CENTER);

    sizer->Add(ecsettings, 0, wxEXPAND);

     wxBoxSizer *buttons = new wxBoxSizer(wxHORIZONTAL);

      buttons->Add(new wxButton(this, ID_OK, wxT("&OK")));
      buttons->Add(new wxButton(this, ID_QuitDlg, wxT("&Cancel")));

    sizer->Add(buttons);

    SetSizer(sizer);
    sizer->SetSizeHints(this);
}


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
  wxIcon icon = wxIcon(amule__icon);
  SetIcon (icon);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

     wxBoxSizer *menu = new wxBoxSizer(wxHORIZONTAL);

      menu->Add(new wxButton(this, ID_Open, wxT("&Open")));
      btn_download = new wxButton(this, ID_Download, wxT("&Download"));
      menu->Add(btn_download, 0, 0, 0);
      menu->Add(new wxButton(this, ID_Options, wxT("O&ptions")));
      menu->Add(new wxButton(this, ID_About, wxT("&About")));
      menu->Add(new wxButton(this, ID_Quit, wxT("&Close")));
      menu->Add(new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxALL, 5);

      menu->Add(new wxButton(this, ID_AddLink, wxT("Add &Link")));
      menu->Add(new wxButton(this, ID_AddFile, wxT("Add &File")));
      menu->Add(new wxButton(this, ID_Delete, wxT("&Delete")));
//      menu->Add(new wxButton(this, -1, wxT("&Save")));
     sizer->Add(menu,0,wxALIGN_LEFT);


    flst = new wxListCtrl(this, ID_FLST, wxDefaultPosition, wxSize(4+250+100+280,150), wxLC_REPORT|wxSUNKEN_BORDER|wxLC_EDIT_LABELS );
    sizer->Add( flst, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

     flst->InsertColumn(0, _("File Name"), wxLIST_FORMAT_LEFT, 250);
     flst->InsertColumn(1, _("File Size"), wxLIST_FORMAT_LEFT, 100);
     flst->InsertColumn(2, _("File Hash"), wxLIST_FORMAT_LEFT, 280);

    CreateStatusBar();
    SetSizer(sizer);
    sizer->SetSizeHints(this);
}

bool emc::OnInit()
{
  wxString s;
  s.Printf(wxT("amule-emc %s"), emcVERSION.c_str());
  MyFrame *frame = new MyFrame( s, wxPoint(50,50), wxSize(510,340) );
  frame->Show(TRUE);
  SetTopWindow(frame);
  if (!ConnectOnStart()) {
   frame->btn_download->Disable();
  }
     return TRUE;
} 

//###*********************###
//###****button events****###
//###*********************###

void MyFrame::OnOptions(wxCommandEvent& WXUNUSED(event)) {

 OptionsDialog *OptionsDlg = new OptionsDialog(this, wxT("amule-emc options"), wxDefaultPosition, wxDefaultSize);
 OptionsDlg->Show(TRUE);

}

void OptionsDialog::OnOK(wxCommandEvent& WXUNUSED(event)) {

 wxConfig *config = new wxConfig(wxT("amule-emc"));

  config->Write(wxT("host"), CastChild(ID_TextHost, wxTextCtrl)->GetValue());
  config->Write(wxT("port"), CastChild(ID_TextPort, wxTextCtrl)->GetValue());
  config->Write(wxT("login") , CastChild(ID_TextLogin, wxTextCtrl)->GetValue());
  config->Write(wxT("password") , CastChild(ID_TextPass, wxTextCtrl)->GetValue());
 config->Flush();
 wxLogStatus(wxT("EC settings changed. Please restart amule-emc to connect with the new settings."));
 Close(TRUE);

}

void OptionsDialog::OnCancel(wxCommandEvent& WXUNUSED(event)) {

 Close(TRUE);

}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
  wxMessageBox (wxT("amule-emc, aMule emulecollection support\n\n"
                    "(c) 2006 Johannes Krampf <wuischke@amule.org>\n\n"
                    "Distributed under GPL"),
                wxT("About amule-emc"), wxOK | wxCENTRE | wxICON_INFORMATION, this);
}

void MyFrame::OnAddLink(wxCommandEvent& WXUNUSED(event))
{

AddLink(wxGetTextFromUser(wxT("Enter ed2k link"), wxT("Add Link"), wxT("")));

}

void MyFrame::OnAddFile(wxCommandEvent& WXUNUSED(event))
{
wxString filename, filesize, filehash;

    wxFileDialog dialog(this, wxT("Add File"), wxEmptyString, wxEmptyString, wxT("*.*"));

    dialog.SetDirectory(wxGetHomeDir());
    dialog.CentreOnParent();

    if (dialog.ShowModal() == wxID_OK)
    {
    AddFile(dialog.GetPath());

    }
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog dialog(this, wxT("Open collection file"), wxEmptyString, wxEmptyString, wxT("emulecollection|*.emulecollection"));

    dialog.SetDirectory(wxGetHomeDir());
    dialog.CentreOnParent();

    if (dialog.ShowModal() == wxID_OK)
    {

    OpenFile(dialog.GetPath().c_str());

    }
}

void MyFrame::OnDelete(wxCommandEvent& WXUNUSED(event))
{
 wxString s;
 long item = -1;
 int cnt = 0;
  do {
      item = flst->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if (item != -1) {
       flst->DeleteItem(item);
       cnt++;
      }
  } while( item != -1 );
 s.Printf(wxT("Deleted %d items."), cnt);
 wxLogStatus(s);
}

void MyFrame::OnDownload(wxCommandEvent& WXUNUSED(event))
{
 wxString s;
 long item = -1;
 int cnt = 0;
 do {
      item = flst->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
      if (item != -1) {
       AddLinkToAMule(GetColItemText(item, 0), GetColItemText(item, 1), GetColItemText(item, 2));
       cnt++;
      }
  } while( item != -1 );
 s.Printf(wxT("Submitted %d items."), cnt);
 wxLogStatus(s);

}

//###*************************###
//###****general functions****###
//###*************************###

wxString GetColItemText(long index, int col) {
 wxListItem info;
  info.m_mask = wxLIST_MASK_TEXT;
  info.m_itemId = index;
  info.m_col = col;
  flst->GetItem( info );
 return info.m_text;
}

bool AddLink(wxString link) {
	wxStringTokenizer tokens(link.Mid(13), wxT("|/"), wxTOKEN_RET_EMPTY_ALL);
	if (tokens.CountTokens() < 5) {
          wxString s;
	  s.Printf(wxT("Not a valid file link: %s\n"), link.c_str());
	  printf(s.mb_str());
          return false;
	}else {
	 wxString name = tokens.GetNextToken().Strip(wxString::both);
	 wxString size = tokens.GetNextToken().Strip(wxString::both);
	 wxString hash = tokens.GetNextToken().Strip(wxString::both);
         AddtoList(name,size,hash);
         return true;
        }
}

void AddtoList(wxString filename, wxString filesize, wxString filehash) {
    long tmp = flst->InsertItem(0, filename);
    flst->SetItem(tmp, 1, filesize);
    flst->SetItem(tmp, 2, filehash);

}

void AddtoList(wxString name, int filesize, CMD4Hash filehash) {
    wxString size, hash;
    size.Printf(wxT("%d"), filesize);
    hash = filehash.Encode();
    long tmp = flst->InsertItem(0, name);
    flst->SetItem(tmp, 1, size);
    flst->SetItem(tmp, 2, hash);
}

bool Hook(int percent)
{
  wxLogStatus(wxString::Format(wxT("Hashing file (%d%%) ..."), percent));
  wxTheApp->Yield(true);
  return (true);
}

//###**********************###
//###****file functions****###
//###**********************###

void AddFile(wxString filename) {
      Ed2kHash hash;
      wxStopWatch chrono;
      wxFileName fileToHash(filename);
      hash.SetED2KHashFromFile(filename, Hook);
      AddLink(hash.GetED2KLink());

      wxLogStatus(wxString::Format(wxT("Hashing of %s done in %.2f s"), fileToHash.GetFullName().c_str(), chrono.Time()*.001));
}

void OpenFile(wxString filename) {
#define  FT_FILENAME		 0x01
#define  FT_COLLECTIONAUTHOR	 0x31
#define  FT_COLLECTIONAUTHORKEY  0x32
wxString CollectionName;
wxString CollectionAuthorName;
wxString ViewCollectionAuthorKey;
const byte* 	 CollectionAuthorKey;
uint32   KeySize;

bool bCollectionLoaded = false;
bool IsTextFile = false;


	CFile data;
	if(data.Open(filename, CFile::read, 0))
	{
		try {		
			uint32 nVersion = data.ReadUInt32();
			if(nVersion == 0x01 || nVersion == 0x02)
			{
				uint32 headerTagCount = data.ReadUInt32();
				while(headerTagCount)
				{
					CTag tag(data, true);
					switch(tag.GetNameID())
					{
						case FT_FILENAME:
						{
							if(tag.IsStr())
								CollectionName = tag.GetStr();
							break;
						}
						case FT_COLLECTIONAUTHOR:
						{
							if(tag.IsStr())
								CollectionAuthorName = tag.GetStr();
							break;
						}
						case FT_COLLECTIONAUTHORKEY:
						{
							if(tag.IsBlob())
							{
							KeySize = tag.GetBlobSize();
							CollectionAuthorKey = new byte[KeySize];
							printf("KeySize:   %d\n", KeySize);
							printf("AuthorKey: ");
							CollectionAuthorKey = tag.GetBlob();
							ViewCollectionAuthorKey = EncodeBase16( CollectionAuthorKey, KeySize);
							//Does not read/display the correct key, still don't know what's the problem.
							}
							break;
						}
					}
					headerTagCount--;
				}
				uint32 fileCount = data.ReadUInt32();

			char* tmp[6];
			CMD4Hash md4hash;
			uint16 k;
			uint8 n;
			uint8 i8;
			uint32 i32;
			int size;
//			char* nme;
			wxString name;
				while(fileCount)
				{
					fileCount--;
					data.Read(tmp, 6); //internal file structur
					md4hash = data.ReadHash();
					k = data.ReadUInt16(); //internal file structur
					if (k == 649) {
					i8 = data.ReadUInt8();
					size = i8;
					}else{
					i32 = data.ReadUInt32();
					size = i32;
					}
					uint8 l;
					l = data.ReadUInt8();
					if (l < 144) {
					data.Read(tmp, 1);//internal file structur
					n = data.ReadUInt8();
					}else{
					n = l-144;
					}
					data.Read(tmp, 1);//internal file structur
					name = data.ReadOnlyString(true, n);
					AddtoList(name, size, md4hash);
				}
				bCollectionLoaded = true;
			}
			else
			{
				data.Close();
				bCollectionLoaded = OpenTextFile(filename);
				IsTextFile = true;				
			}
		}
		catch(...)
		{
			wxASSERT( false );
			data.Close();
		}
	}

         wxString s;
if (!IsTextFile) {
	if (!CollectionAuthorName) {
          s.Printf(wxT("%s") , CollectionName.c_str());
	 }else {
           if (CollectionName) {
            s.Printf(wxT("%s (%s ) [key not verified] ") , CollectionName.c_str(), CollectionAuthorName.c_str());
           }else {
	    s = wxT("Unsupported file type");
           }	  
	}
       wxLogStatus(s);
}
}

bool OpenTextFile(wxString filename) {
      wxTextFile file;
      file.Open(filename);
       int count = file.GetLineCount();
       wxString tmp;
       int numfailed = 0;
       int numsuc = 0;
        for ( int n = 0; n < count; n++ )
        {
            tmp = file.GetLine(n);
            if ( tmp.Mid(0,1) != wxT("#") ) {
              if ( ! AddLink(tmp) ) {
               numfailed++;
              }else{
	       numsuc++;
              }
            }
        }
      file.Close();
         wxString s;
          s.Printf(wxT("Processed %d lines. (%d successful, %d failed)") , numsuc+numfailed, numsuc, numfailed);
       wxLogStatus(s);
}

//###********************###
//###****EC functions****###
//###********************###

bool ConnectOnStart() {
ECconn = new CRemoteConnect(NULL);

bool ConnectionEstablished;
 wxConfig *config = new wxConfig(wxT("amule-emc"));
 wxString s;

wxString host, login, pass, client, version;
config->Read(wxT("host"), &host, wxT("localhost"));
int port = config->Read(wxT("port"), 4712);
config->Read(wxT("login") , &login , wxT("amule"));
config->Read(wxT("password") , &pass , wxT("password"));
pass = MD5Sum(pass).GetHash();
client  = wxT("amule-emc");
version = wxT("0.1");

s.Printf(wxT("Connecting to host %s:%d ..."), host.c_str(), port);
wxLogStatus(s);

ConnectionEstablished = ECconn->ConnectToCore(host, port, login, pass, client, version);

 if (!ConnectionEstablished) {
  s.Printf(wxT("Connection to host %s:%d failed. Please review the EC settings."), host.c_str(), port);
 }else {
  s.Printf(wxT("Connected to host %s:%d."), host.c_str(), port);
 }
  wxLogStatus(s);
return ConnectionEstablished;
}

void AddLinkToAMule(wxString filename, wxString filesize, wxString filehash) {
 wxString link;
 link.Printf(wxT("ed2k://|file|%s|%s|%s|/"), filename.c_str(), filesize.c_str(), filehash.c_str());

// ECconn->Ed2kLink(&link); //Missing implementation in RemoteConnect.cpp

 CECPacket req(EC_OP_ED2K_LINK);
 req.AddTag(CECTag(EC_TAG_STRING, link));

 ECconn->SendPacket(&req);

}
