/* ###############

collection.cpp

emulecollection processing class

by Johannes Krampf <wuischke@amule.org>

This code is licensed under the GPL

See the included license.txt

for further information

   ############### */

#include "collection.h"
#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include "CFile.h"
#include "Types.h"
#include "Tag.h"
#include "MD4Hash.h"

collection::collection(void)
{
 IsSigned   = -1; //-1 = error, 0 = not signed, 1 = signed
 FileFormat = -1; //-1 = error, 0 = text format, 1 = emc v1, 2 = emc v2

 CollectionAuthorName = wxT(""); 
 CollectionAuthorKey = wxT("");
}

collection::~collection(void)
{
}

bool collection::OpenCollection(wxString file)
{
 if (!OpenCollectionInBinaryFormat(file))
 {
  if (!OpenCollectionInTextFormat(file))
  {
   return false;
  }
 }

 return true;
}

bool collection::SaveCollection(wxString file)
{
//##### implementation missing ##### //
}


	
int collection::GetCollectionFileFormat()
{
 return FileFormat;
}

int collection::GetFileCount()
{
 return ArrayFileName.GetCount();
}

wxString collection::GetCollectionName()
{
 return CollectionName;
}

wxString collection::GetCollectionAuthorName()
{
 return CollectionAuthorName;
}

int collection::CollectionIsSigned()
{
 return IsSigned;
}

wxString collection::GetCollectionAuthorKey()
{
 return CollectionAuthorKey;
}

bool collection::AddFilePerLinkToCollection(wxString link)
{
 wxStringTokenizer tokens(link.Mid(13), wxT("|/"), wxTOKEN_RET_EMPTY_ALL);
 if ( (link.Mid(0,13) != wxT("ed2k://|file|")) || (tokens.CountTokens() < 5)) {
  return false;
 }else
 {
  wxString name = tokens.GetNextToken().Strip(wxString::both);
  double size;
  tokens.GetNextToken().Strip(wxString::both).ToDouble(&size);
  wxString hash = tokens.GetNextToken().Strip(wxString::both);
  return AddFileToCollection(name ,(int)size ,hash);
 }
}

bool collection::AddFileToCollection(wxString name, int size, wxString hash)
{
 ArrayFileName.Add(name);
 ArrayFileSize.Add(size);
 ArrayFileHash.Add(hash);

 return true;
}

bool collection::SetFileName(wxString name, int idx)
{
 if ( ArrayFileName.GetCount() > idx && idx >= 0 ) {
  ArrayFileName[idx] = name;
  return true;
 }else {
  return false;
 }
}

bool collection::RemoveFileFromCollection(int idx)
{
 if ( ArrayFileName.GetCount() > idx && idx >= 0 ) {
  ArrayFileName.RemoveAt(idx);
  if ( ArrayFileSize.GetCount() > idx && idx >= 0 ) {
   ArrayFileSize.RemoveAt(idx);
   if ( ArrayFileHash.GetCount() > idx && idx >= 0 ) {
    ArrayFileHash.RemoveAt(idx);
    return true;
   }else {
    return false;
   }
  }else {
   return false;
  }
 }else {
  return false;
 }
}

	
wxString collection::GetFileName(int idx)
{
 if ( ArrayFileName.GetCount() > idx && idx >= 0 ) {
  return ArrayFileName[idx];
 }else {
  return wxT("Trying to access non-existant item!");
 }
}

int collection::GetFileSize(int idx)
{
 if ( ArrayFileSize.GetCount() > idx && idx >= 0 ) {
  return ArrayFileSize[idx];
 }else {
  return -1;
 }
}

wxString collection::GetFileHash(int idx)
{
 if ( ArrayFileHash.GetCount() > idx && idx >= 0 ) {
  return ArrayFileHash[idx];
 }else {
  return wxT("Trying to access non-existant item!");
 }
}

bool collection::OpenCollectionInTextFormat(wxString file)
{
 bool return_value = false;
 wxTextFile File;
  File.Open(file);
   int count = File.GetLineCount();
    for ( int n = 0; n < count; n++ )
    {
     if (AddFilePerLinkToCollection(File.GetLine(n)))
     {
      return_value=true;
     }
    }
   if ( count > 0 )
   {
    SetAuthorNameFromTextFile(File.GetLine(0));
   }
  File.Close();
 if ( return_value == true)
 {
  FileFormat = 0;
 } 
 return return_value;
}

bool collection::SetAuthorNameFromTextFile(wxString comment)
{
 if (comment.Mid(0,1) != wxT("#"))
 {
  return false;
 }
 int pos = comment.Find(wxT("created by "));
 if ( pos != -1 )
 {
  CollectionAuthorName = comment.Mid(pos+11);
  return true;
 }
}

bool collection::OpenCollectionInBinaryFormat(wxString file)
{
 CFile File;
  if(File.Open(file, CFile::read, 0))
 {
  try
  {
   uint32 nVersion = File.ReadUInt32();
   if(nVersion == 0x01 || nVersion == 0x02)
   {
    uint32 headerTagCount = File.ReadUInt32();
    while(headerTagCount)
    {
    CTag tag(File, true);
    switch(tag.GetNameID())
    {
     case 0x01:
     {
      if(tag.IsStr())
       CollectionName = tag.GetStr();
      break;
     }
     case 0x31:
     {
      if(tag.IsStr())
       CollectionAuthorName = tag.GetStr();
      break;
     }
     case 0x32:
     {
      if(tag.IsBlob())
       {
		const byte* abyCollectionAuthorKey = tag.GetBlob();
		uint32 nSize = tag.GetBlobSize();
		byte*	m_pabyCollectionAuthorKey;
		uint32	m_nKeySize;

	delete[] m_pabyCollectionAuthorKey;
	m_pabyCollectionAuthorKey = NULL;
	m_nKeySize = 0;
	if (abyCollectionAuthorKey != NULL){
		m_pabyCollectionAuthorKey = new byte[nSize];
		memcpy(m_pabyCollectionAuthorKey, abyCollectionAuthorKey, nSize);
		m_nKeySize = nSize;
	}
       }
      break;
     }
    }
    headerTagCount--;
   }

	if (CollectionName==wxT("")) {
		return false;
	}
	
   uint32 fileCount = File.ReadUInt32();
   while(fileCount)
   {
//#################################################

	wxString fname, fhash, ftype;
	CMD4Hash md4hash;
	int fsize;

	uint tagcount = File.ReadUInt32();
	CTag* taglist[tagcount];

	for (uint i = 0; i < tagcount; i++)
	{
		CTag* toadd = new CTag(File, true);
		if (toadd)
			taglist[i] = toadd;
	}

	int i = 0;
	do 
	{
		if (taglist[i]->GetNameID()==0x28) {
		md4hash = taglist[i]->GetHash();
		fhash = md4hash.Encode();
		}
		i++;
	}
	while (i < tagcount);
	
	i = 0;
	do 
	{
		if (taglist[i]->GetNameID()==0x03 && taglist[i]->IsStr()) {
			ftype = taglist[i]->GetStr();
		}
		i++;
	}
	while (i < tagcount);
	
	i = 0;
	do 
	{
		if (taglist[i]->GetNameID()==0x01 && taglist[i]->IsStr()) {
			fname = taglist[i]->GetStr();
		}
		i++;
	}
	while (i < tagcount);

	i = 0;
	do 
	{
		if (taglist[i]->GetNameID()==0x02) {
			fsize = taglist[i]->GetInt();
		}
		i++;
	}
	while (i < tagcount);

	AddFileToCollection(fname,fsize,fhash);

//#################################################

     fileCount--;
   }
/*  if (m_pabyCollectionAuthorKey != NULL)
  {
   bool bResult = false;
   if (data.GetLength() > data.GetPosition())
   {
    using namespace CryptoPP;

    uint32 nPos = (uint32)data.GetPosition();
    data.SeekToBegin();
    BYTE* pMessage = new BYTE[nPos];
    VERIFY( data.Read(pMessage, nPos) == nPos);
    StringSource ss_Pubkey(m_pabyCollectionAuthorKey, m_nKeySize, true, 0);
    RSASSA_PKCS1v15_SHA_Verifier pubkey(ss_Pubkey);
    int nSignLen = (int)(data.GetLength() - data.GetPosition());
    BYTE* pSignature = new BYTE[nSignLen ];
    VERIFY( data.Read(pSignature, nSignLen) == (UINT)nSignLen);

    bResult = pubkey.VerifyMessage(pMessage, nPos, pSignature, nSignLen);

    delete[] pMessage;
    delete[] pSignature;
   }	
   if (!bResult)
   {
    DebugLogWarning(_T("Collection %s: Verifying of public key failed!"), m_sCollectionName);
    delete[] m_pabyCollectionAuthorKey;
    m_pabyCollectionAuthorKey = NULL;
    m_nKeySize = 0;
    m_sCollectionAuthorName = _T("");
   }else
    DebugLog(_T("Collection %s: Public key verified"), m_sCollectionName);
  }else
   m_sCollectionAuthorName = _T("");
  data.Close(); */
  	File.Close();
	FileFormat = nVersion;
	return true;
  }else
  	return false;
  }
//  catch(CFileException* error)
//  {
//   error->Delete();
//   return false;
//  }
  catch(...)
  {
  wxASSERT( false );
  File.Close();
  return false;
  }
 }else
  return false;
}

bool collection::SaveCollectionInTextFormat(wxString file)
{
//##### implementation missing ##### //
}

bool collection::SaveCollectionInBinaryFormat(wxString file)
{
//##### implementation missing ##### //
}

bool collection::SaveCollectionInSignedBinaryFormat(wxString file)
{
//##### implementation missing ##### //
}
