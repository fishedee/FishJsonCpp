#include <stdio.h>
struct SFieldType{
	const char* strFieldTypeName;
	struct SFieldType* sFirstSubFieldType;
	struct SFieldType* sSecondSubFieldType;
};
struct SFiled{
	struct SFieldType* sFieldType;
	const char* strFieldName;
};
struct SFiledList{
	struct SFiled* sField;
	struct SFiledList* next;
};
struct SClass{
	const char* strClassName;
	struct SFiledList* listClassField;
};
struct SNameSpaceList{
	const char* strNamespace;
	struct SNameSpaceList* next;
};
struct SClassList{
	struct SClass* strClass;
	struct SClassList* next;
};
struct SCode{
	struct SNameSpaceList* listNamespaces;
	struct SClassList* listClasses;
};
struct SHeaderList{
	const char* strHeader;
	struct SHeaderList* next;
};
struct SProgram{
	struct SHeaderList* listHeaders;
	struct SCode* sCode;
};
extern struct SProgram* g_program;
void generate();
int yyerror( char* msg );
