#include <string.h>
static const char* header_files[] = {
	"<sstream>",
	"\"app_platform/intf_serializable.h\"",
	"\"c2cplatform/library/the3/jsoncpp64/reader.h\"",
	"\"c2cplatform/library/the3/jsoncpp64/value.h\"",
	"\"c2cplatform/library/the3/jsoncpp64/writer.h\"",
};
static const char* basic_types[] = {
	"bool",
	"char",
	"short",
	"int",
	"long",
	"long long",
	"unsigned char",
	"unsigned short",
	"unsigned int",
	"unsigned long",
	"unsigned long long",
	"int8_t",
	"int16_t",
	"int32_t",
	"int64_t",
	"uint8_t",
	"uint16_t",
	"uint32_t",
	"uint64_t",
	"float",
	"double",
	"std::string",
};
static const char* list_types[] = {
	"std::vector",
	"std::list",	
	"std::set",
};
static const char* map_types[] = {
	"std::map",
};
static int isBasicType( const char* a ){
	int i;
	for( i = 0 ; i != sizeof( basic_types )/ sizeof( basic_types[0]) ; ++i )
		if( strcmp( a ,basic_types[i] ) == 0 )
			return 1;
	return 0;
}
static int isListType( const char* a ){
	int i;
	for( i = 0 ; i != sizeof( list_types )/ sizeof( list_types[0]) ; ++i )
		if( strcmp( a ,list_types[i] ) == 0 )
			return 1;
	return 0;
}
static int isMapType( const char* a ){
	int i;
	for( i = 0 ; i != sizeof( map_types )/ sizeof( map_types[0]) ; ++i )
		if( strcmp( a ,map_types[i] ) == 0 )
			return 1;
	return 0;
}
