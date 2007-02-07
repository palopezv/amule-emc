#include "amule-emc.h"
#include "collection.h"

#include <wx/wx.h>
#include <wx/file.h> 
#include <wx/cmdline.h>

IMPLEMENT_APP(amuleEMC)

bool amuleEMC::OnInit()
{

 wxCmdLineParser cmdline;
 cmdline.SetCmdLine(amuleEMC::argc, amuleEMC::argv);
 //following function needs EC
 //cmdline.AddSwitch(wxT("c"), wxT("category"), wxT("Add collection files to a category"));
 cmdline.AddSwitch(wxT("h"), wxT("help"), wxT("Display help message"), wxCMD_LINE_OPTION_HELP);
 cmdline.AddParam( wxT("emulecollection file"));
 cmdline.Parse(false);
 if (cmdline.Found(wxT("h"))) {
  cmdline.Usage();
  return 0;
 }
 if (cmdline.GetParamCount() == 0) {
  cmdline.Usage();
  return 0;
 }
 
 /*printf("Create category: ");
 if (cmdline.Found(wxT("c"))) {
  printf("yes \n");
  UseCategory=true;
 }else {
  printf("no \n");
  UseCategory=false;
 }*/

 wxString file = cmdline.GetParam(0);
 printf("collection file: %S\n", file.c_str());
 wxFile test;
 if (!test.Exists(file)) {
  printf("Input file is not a valid file. Exiting.\n");
  return 0;
 }
 collection collection;
 collection.OpenCollection(file);
 int n = collection.GetFileCount();

 wxString ed2klink;
 int retvalue;
 wxArrayString output, errors;
 for (int i = 0; i < n; i++)
 {
  ed2klink.Printf(wxT("ed2k ed2k://|file|%s|%i|%s|/"), collection.GetFileName(i).c_str(), collection.GetFileSize(i), collection.GetFileHash(i).c_str() );
  wxExecute(ed2klink, output, errors);
 }
 for (int i = 0; i < errors.GetCount(); i++)
  printf("%S\n", errors[i].c_str());

 return 0;
}
