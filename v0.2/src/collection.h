/* ###############

collection.h

emulecollection processing class

by Johannes Krampf <wuischke@amule.org>

This code is licensed under the GPL

See the included license.txt

for further information

   ############### */

#pragma once
#include <wx/wx.h>
#include "Types.h"

class collection
{
public:
	collection(void);
	~collection(void);

	bool	 OpenCollection(wxString file);
	bool	 SaveCollection(wxString file);
	
	int	 GetCollectionFileFormat();
	int	 GetFileCount();
	wxString GetCollectionName();
	wxString GetCollectionAuthorName();
	int	 CollectionIsSigned();
	wxString GetCollectionAuthorKey();

	bool	 AddFileToCollection(wxString name, int size, wxString hash);
	bool	 AddFilePerLinkToCollection(wxString link);
	bool	 SetFileName(wxString name, int idx);
	bool	 RemoveFileFromCollection(int idx);
	
	wxString GetFileName(int idx);
	int	 GetFileSize(int idx);
	wxString GetFileHash(int idx);

private:

	bool OpenCollectionInTextFormat(wxString file);
	bool OpenCollectionInBinaryFormat(wxString file);

	bool SaveCollectionInTextFormat(wxString file);
	bool SaveCollectionInBinaryFormat(wxString file);
	bool SaveCollectionInSignedBinaryFormat(wxString file);

	bool SetAuthorNameFromTextFile(wxString comment);

	wxArrayString ArrayFileName;
	wxArrayInt    ArrayFileSize;
	wxArrayString ArrayFileHash;

	int IsSigned;
	int FileFormat;

	wxString CollectionName;
	wxString CollectionAuthorName; 
	wxString CollectionAuthorKey;
	uint32	CollectionAuthorKeySize;

};
