## emulecollection file structure ##

### text files ###

Lines contain either an ed2k link or a comment starting with #.

### binary files ###

```
uint32_t emulecollection version
values:	COLLECTION_FILE_VERSION1_INITIAL		0x01
	COLLECTION_FILE_VERSION2_LARGEFILES		0x02
	
	version 2 files support files bigger than 4GB (uint64_t file sizes)

uint32_t header tag count
 
header tags:
	
	TAGTYPE_STRING					0x02
		uint16_t					0x0001	
		FT_FILENAME					0x01
		uint16_t					string length
		std::string					string value
	TAGTYPE_STRING					0x02
		uint16_t					0x0001
		FT_COLLECTIONAUTHOR	 			0x31
		uint16_t					string length
		std::string					string value
	TAGTYPE_BLOB					0x07
		uint16_t					0x0001
		FT_COLLECTIONAUTHORKEY				0x32
		uint32_t					blob size
		(notype)					blob data

uint32_t collection file count

uint32_t file tag count

	at this point the TAGTYPE 0x01 will appear as 0x81 (uType | 0x80)
	-> see eMule packets.cpp
	FT_FILERATING and FT_FILECOMMENT are optional tags
 	
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
		uint8_t						file size
	TAGTYPE_UINT64					0x0B
		FT_FILESIZE					0x02
		uint64_t					file size

	TAGTYPE_STR1 to 16					0x11 to 0x20
		FT_FILENAME					0x01
		std::string					file name
	
	string length is TAGTYPE_STRx - 0x11 + 0x01 [1 to 16]

	TAGTYPE_STRING					0x02
		FT_FILENAME					0x01
		uint16_t					string length
		std::string					string value  	  	

	TAGTYPE_STRING					0x02
		FT_FILECOMMENT					0xF6
		uint16_t					string length
		std::string					string value  	  	

	TAGTYPE_UINT8					0x09
		FT_FILERATING					0xF7
		uint8_t						file rating 	

(notype) signature (only for signed files)
```