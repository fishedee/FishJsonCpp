%{
#define YYSTYPE void*
#include <stdio.h>
#include <string.h>
#include "struct.h"
%}
%token WORD CLASS NAMESPACE HEADER
%%
program: code{
		g_program = (struct SProgram*)malloc(sizeof(struct SProgram));
		g_program->listHeaders = NULL;
		g_program->sCode = (struct SCode* )$1;
		$$ = g_program;
	}
	| HEADER program {
		struct SHeaderList* headerList = ( struct SHeaderList*)malloc( sizeof( struct SHeaderList ) );	
		headerList->strHeader = ( const char*)$1;
		headerList->next = g_program->listHeaders;
		g_program->listHeaders = headerList;
		$$ = g_program;
	}
code: classes {
		struct SCode* code = (struct SCode*)malloc(sizeof(struct SCode));
		code->listClasses = ( struct SClassList*)$1;
		code->listNamespaces = NULL;
		$$ = code;
	}
	| NAMESPACE WORD '{' code '}' {
		struct SNameSpaceList* namespaceList = (struct SNameSpaceList*)malloc(sizeof(struct SNameSpaceList));
		namespaceList->strNamespace = (const char*)$2;
		namespaceList->next = ((struct SCode*)$4)->listNamespaces;
		((struct SCode*)($4))->listNamespaces = namespaceList;
		$$ = $4;
	}
;
classes: class{
		struct SClassList* classList = (struct SClassList*)malloc(sizeof(struct SClassList));
		classList->strClass = (struct SClass*)$1;
		classList->next = NULL;
		$$ = classList;
	}
	| class classes{
		struct SClassList* classList = (struct SClassList*)malloc(sizeof(struct SClassList));
		classList->strClass = (struct SClass*)$1;
		classList->next = (struct SClassList*)$2;
		$$ = classList;
	}
;
class:CLASS WORD '{' classFields '}' ';'{
		struct SClass* class = (struct SClass*)malloc(sizeof(struct SClass));
		class->strClassName =(const char*)$2;
		class->listClassField = (struct SFiledList*)$4;
		$$ = class;
	}
;
classFields:classField{
		struct SFiledList* fieldList = (struct SFiledList*)malloc(sizeof(struct SFiledList));
		fieldList->sField = (struct SFiled*)$1;
		fieldList->next = NULL;
		$$ = fieldList;
	}
	| classField classFields{
		struct SFiledList* fieldList = (struct SFiledList*)malloc(sizeof(struct SFiledList));
		fieldList->sField = (struct SFiled*)$1;
		fieldList->next = (struct SFiledList*)$2;
		$$ = fieldList;
	}
;
classField:classFieldType WORD ';'{
		struct SFiled* sField = (struct SFiled*)malloc(sizeof(struct SFiled));
		sField->sFieldType = (struct SFieldType*)$1;
		sField->strFieldName = (const char*)$2;
		$$ = sField;
	}
;
classFieldType:classFieldTypeName{
		struct SFieldType* sFieldType = (struct SFieldType*)malloc(sizeof(struct SFieldType));
		sFieldType->strFieldTypeName = (const char*)$1;
		sFieldType->sFirstSubFieldType = (struct SFieldType*)NULL;
		sFieldType->sSecondSubFieldType = (struct SFieldType*)NULL;
		$$ = sFieldType;
	}
	| classFieldTypeName '<' classFieldType '>' {
		struct SFieldType* sFieldType = (struct SFieldType*)malloc(sizeof(struct SFieldType));
		sFieldType->strFieldTypeName = (const char*)$1;
		sFieldType->sFirstSubFieldType = (struct SFieldType*)$3;
		sFieldType->sSecondSubFieldType = (struct SFieldType*)NULL;
		$$ = sFieldType;
	}
	| classFieldTypeName '<' classFieldType ',' classFieldType '>' {
		struct SFieldType* sFieldType = (struct SFieldType*)malloc(sizeof(struct SFieldType));
		sFieldType->strFieldTypeName = (const char*)$1;
		sFieldType->sFirstSubFieldType = (struct SFieldType*)$3;
		sFieldType->sSecondSubFieldType = (struct SFieldType*)$5;
		$$ = sFieldType;
	}
;
classFieldTypeName:WORD{
		$$ = $1;
	}
	| WORD ':' ':' classFieldTypeName{
		char* result = (char*)malloc( strlen( (const char*)$1 ) + strlen( (const char*)$4 ) + 3 );
		strcpy( result , (const char*)$1 );
		strcat( result , "::" );
		strcat( result , (const char*)$4 );
		$$ = result;
	}
;
