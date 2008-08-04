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
#include "MuleCollection.h"

#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	int i;

	std::string help("-h");
	std::string version("-v");
	if( (1 == argc) || (0 == help.compare(argv[1]))) {
		std::cout << "amule-emc - list links inside an emulecollection file" << std::endl;
		std::cout << std::endl << "Usage Information:" << std::endl;
		std::cout << "amule_emc [-h] [-v] [file.emulecollection]..." << std::endl;
		return 0;
	}

	if (0 == version.compare(argv[1])) {
		std::cout << "amule-emc version: " << VERSION_MAJ << "." << VERSION_MIN << std::endl;
		return 0;
	}

	for (i = 1; i < argc; i++)
	{
		OutputEd2kLinks(argv[i]);
	}
	return 0;
}

size_t	OutputEd2kLinks( std::string File )
{
	CMuleCollection my_collection;
	if (my_collection.Open( File ))
	{
		for(int i = 0;i < my_collection.GetFileCount();i++)
			std::cout	<< my_collection.GetEd2kLink(i) << std::endl;
	}
	return my_collection.GetFileCount();
}
