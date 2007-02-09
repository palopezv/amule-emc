import std.stdio;
import collection;

void main() {
	collection my_collection = new collection;
	my_collection.open("eMule 0_47c.emulecollection");
	writefln("File count: ",my_collection.GetCount());
	for (int i = 0; i < my_collection.GetCount(); i++)
	{
		writefln(my_collection.GetLink(i));
	}
	
}
