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

#include "amule-emc.h"
#include "collection.h"

#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < argc; i++)
	{
		OutputEd2kLinks(argv[i]);
	}
	return 0;
}

size_t	OutputEd2kLinks( std::string File )
{
	collection my_collection;
	if (my_collection.Open( File ))
	{
		for(int i = 0;i < my_collection.GetFileCount();i++)
			std::cout	<< my_collection.GetEd2kLink(i)
					<< std::endl;
	}
	return my_collection.GetFileCount();
}
