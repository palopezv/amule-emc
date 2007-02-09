import std.stdio;
import std.stream;
import std.string;
import std.conv;

class collection {
	
	//open emulecollection file
		bool open(char[] file) {
			if (!openBinaryFormat(file))
				return openTextFormat(file);
			return true;
		}

	//get file count
		int GetCount() {
			return collectionFiles.length;
		}
	
	//get ed2k link
		char[] GetLink(int i) {
		//ed2k://|file|NAME|SIZE|HASH|/			
			char[] retval = format("ed2k://|file|", GetName(i), "|", GetSize(i), "|", GetHash(i), "|/");
			return retval;
		}

	//get file name		
		char[] GetName(int i) {
			return collectionFiles[i].name[];
		}

	//get file size		
		ulong GetSize(int i) {
			return collectionFiles[i].size;
		}

	//get file hash
		char[] GetHash(int i) {
			return collectionFiles[i].hash[];
		}
			
	private:

	//declare internal data structure	
		struct collectionFile
		{
			char[] name;
			ulong size;
			char[32] hash;
		}		
	
		collectionFile[] collectionFiles;
	
	//open emulecollection file in binary format
		bool openBinaryFormat(char[] file) {
			return false;
		}

	//open emulecollection file in text format
		bool openTextFormat(char[] file) {
			File f = new File(file);
    		int filecount;
    		while (!f.eof())
    		{
    		    if (addLink(f.readLine()))
    		    	filecount++;
    		}
    		f.close();  
			if (filecount == 0)
				return false;
			return true;
		}


	//add new link to collection
		bool addLink(char[] string)	{
			char[][] a = split(string, "|");
			if ( (a.length < 6) ||
				 (a[0] != "ed2k://") || 
				 (a[1] != "file") || 
				 (a[2].length == 0) ||
				 (a[3].length == 0) || 
				 (!isNumeric(a[3])) ||
				 (a[4].length != 32)
			)
				return false;
			
			char[] name = a[2];

			ulong size = toUlong(a[3]);
			if (size == 0)
				return false;
			
			char[32] hash = a[4];
			
			for(int i=0;i<32;i++) {
			if (!inPattern(hash[i],"a-fA-F0-9"))
				return false;			
			}
									
			return add(name,size,hash);
		}
		
		bool add(char[] name, ulong size, char[32] hash) {
			int i = collectionFiles.length;
			collectionFiles.length = i+1;

			collectionFiles[i].name.length = name.length;
			collectionFiles[i].name[] = name[];
			collectionFiles[i].size = size;
			collectionFiles[i].hash[] = hash[];
			
			if( !(collectionFiles[i].name == name) || 
				!(collectionFiles[i].size == size) || 
				!(collectionFiles[i].hash == hash)
			) {
				collectionFiles.length = i-1;
				return false;
			}
				
			return true;
		}
	
}
