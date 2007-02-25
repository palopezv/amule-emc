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

collection::collection()
{
};

collection::~collection()
{
};
		
bool	collection::Open( std::string File )
{
	return 0;
};

int	collection::GetFileCount( int index )
{
	return 0;
};

std::string	collection::GetEd2kLink( int index )
{
	return 0;
};

std::string	collection::GetFileName( int index )
{
	return 0;
};

uint64_t	collection::GetFileSize( int index )
{
	return 0;
};

std::string	collection::GetFileHash( int index )
{
	return 0;
};

bool	collection::OpenBinary( std::string File )
{
	return 0;
};

bool	collection::OpenText( std::string File )
{
	return 0;
};

bool	collection::AddLink( std::string Link )
{
	return 0;
};

bool	collection::AddFile( std::string	FileName,
			     uint64_t		FileSize,
			     std::string	FileHash
				)
{
	return 0;
};
