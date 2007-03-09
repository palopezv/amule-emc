/*This file is part of amule-emc, emulecollection support for aMule.
Copyright (C) 2007  Johannes Krampf <wuischke@amule.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "collection.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <iostream>

collection::collection()
{
};

collection::~collection()
{
};
		
bool	collection::Open( std::string File )
{
	if(!OpenBinary( File ))
		if(!OpenText( File ))
			return false;
	return true;
};

size_t	collection::GetFileCount()
{
	return vCollection.size();
};

std::string	collection::GetEd2kLink( int index )
{
	if (!(index < GetFileCount()))
		return "Invalid Index!";
	
	std::stringstream retvalue;
	// ed2k://|file|FileName|FileSize|FileHash|/
	retvalue	<< "ed2k://|file|" << GetFileName( index )
			<< "|" << GetFileSize( index )
			<< "|" << GetFileHash( index )
			<< "|/";
		
	return retvalue.str();
};

std::string	collection::GetFileName( int index )
{
	if (!(index < GetFileCount()))
		return "Invalid Index!";

	std::string retvalue = vCollection[index].FileName;
	if (retvalue == "")
		return "Empty String!";
			
	return retvalue;
};

uint64_t	collection::GetFileSize( int index )
{
	if (!(index < GetFileCount()))
		return 0;
	
	uint64_t retvalue = vCollection[index].FileSize;
	
	return retvalue;
};

std::string	collection::GetFileHash( int index )
{
	if (!(index < GetFileCount()))
		return "Invalid Index!";
	std::string retvalue = vCollection[index].FileHash;
	if (retvalue == "")
		return "Empty String!";
			
	return retvalue;
};

bool	collection::OpenBinary( std::string File )
{
#warning function OpenBinary( std::string File ) is not finished
	return false;
	
	
	std::ifstream infile;
	
	infile.open(File.c_str(), std::ifstream::in|std::ifstream::binary);
		if(!infile.is_open())
			return false;

/*	emulecollection file structure:

uint32_t emulecollection version
values:	COLLECTION_FILE_VERSION1_INITIAL		0x01
	COLLECTION_FILE_VERSION2_LARGEFILES		0x02
	
	version 2 files support files bigger than 4GB (uint64_t file sizes)

uint32_t header tag count

header tags:
	FT_FILENAME					0x01
		TAGTYPE_STRING					0x02
		uint16_t					string length
		std::string					string value
	FT_COLLECTIONAUTHOR	 			0x31
		TAGTYPE_STRING					0x02
		uint16_t					string length
		std::string					string value
	FT_COLLECTIONAUTHORKEY				0x32
		TAGTYPE_BLOB					0x07
		uint32_t					blob size
		(notype)					blob data

uint32_t collection file count

uint32_t file tag count

	at this point the TAGTYPE 0x01 will appear as 0x81 (uType | 0x80)
	-> see eMule packets.cpp
	
	TAGTYPE_HASH					0x01
		FT_FILEHASH					0x28
		(notype)[16]					hash data

	TAGTYPE_UINT32					0x03
		FT_FILESIZE					0x02
		uint32_t					file size
	TAGTYPE_UINT16					0x08
		FT_FILESIZE					0x02
		uint16_t					file size
	TAGTYPE_UINT8					0x09
		FT_FILESIZE					0x02
		uint8_t					file size
	TAGTYPE_UINT64					0x0B
		FT_FILESIZE					0x02
		uint64_t					file size

	TAGTYPE_STR3					0x13
		FT_FILENAME					0x01
		std::string					file name		

(notype) signature (only for signed files)
		
*/



	uint32_t cVersion;
	infile.read (reinterpret_cast<char *>(&cVersion),sizeof(uint32_t));
	
	
	
	std::cout << "cVersion: " << cVersion << std::endl;
	
	infile.close();

	return true;
};

bool	collection::OpenText( std::string File )
{
	int		numLinks = 0;
	std::string	line;
	std::ifstream infile;
	
	infile.open(File.c_str(), std::ifstream::in);
		if(!infile.is_open())
			return false;
  	
  	while (getline(infile, line))
		if(AddLink(line))
			numLinks++;
	
	infile.close();
	
	if(numLinks == 0)
		return false;
		
	return true;
};

bool	collection::AddLink( std::string Link )
{
	// 12345678901234       56       7 + 32 + 89 = 19+32=51
	// ed2k://|file|FileName|FileSize|FileHash|/
	if (	(Link.size() < 51)			||
		(Link.substr(0,13) != "ed2k://|file|")	||
		(Link.substr(Link.size()-2) != "|/")
	)
		return false;
	
	int iName = Link.find("|",13);
		if(iName == std::string::npos)
			return false;
	std::string FileName = Link.substr(13,iName-13);

	int iSize = Link.find("|",iName+1);
		if(iSize == std::string::npos)
			return false;
	std::stringstream sFileSize;
		sFileSize << Link.substr(iName+1,iSize-iName-1);
	uint64_t FileSize;
		if((sFileSize >> std::dec >> FileSize).fail())
			return false;
		
	int iHash = Link.find("|",iSize+1);
			if(iHash == std::string::npos)
			return false;
	std::string FileHash = Link.substr(iSize+1,32);

	return AddFile(FileName, FileSize, FileHash);;
};

bool	collection::AddFile( std::string	FileName,
			     uint64_t		FileSize,
			     std::string	FileHash
				)
{
	if (	(FileName == "")		||
		(FileSize == 0)			||
		(FileSize > 0xffffffffLL)	||
		(!IsValidHash( FileHash) )
	)
		return false;
		
	vCollection.push_back( CollectionFile() );
	int index = vCollection.size()-1;
	vCollection[index].FileName = FileName;
	vCollection[index].FileSize = FileSize;
	vCollection[index].FileHash = FileHash;
	return true;
};

bool	collection::IsValidHash( std::string FileHash )
{
	if (	(FileHash.size() != 32)	||
		(FileHash == "")
	)
		return false;
	
	//FileHash needs to be a valid MD4Hash
	std::string hex = "01234567890abcdefABCDEF";
	for(int i = 0; i < FileHash.size();i++)
	{
		if(hex.find(FileHash[i]) == std::string::npos)
			return false;
	}
	return true;	
};
