#include "struct.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
struct SProgram* g_program = NULL;
void generateClassFieldType( struct SFieldType* sFieldType ){
	printf("%s",sFieldType->strFieldTypeName);
	if( sFieldType->sFirstSubFieldType != NULL
		&& sFieldType->sSecondSubFieldType == NULL ){
		printf("< ");
		generateClassFieldType(sFieldType->sFirstSubFieldType);
		printf(" >");
	}else if( sFieldType->sFirstSubFieldType != NULL
		&& sFieldType->sSecondSubFieldType != NULL ){
		printf("< ");
		generateClassFieldType(sFieldType->sFirstSubFieldType);
		printf(" , ");
		generateClassFieldType(sFieldType->sSecondSubFieldType);
		printf(" >");
	}
}
const char* firstToUpper( const char* strFieldMethodName ){
	char* temp;
	temp = strdup(strFieldMethodName);
	temp[0] = toupper(temp[0]);
	return temp;
}
const char* allToUpper( const char* strFieldMethodName ){
	char* temp;
	char* temp2;
	temp = strdup(strFieldMethodName);
	for( temp2 = temp ; *temp2 != '\0' ; temp2 ++ )
		*temp2 = toupper( *temp2 );
	return temp;
}
void generateClassField( struct SFiled* sField ){
	printf("\t");
	generateClassFieldType( sField->sFieldType );
	printf(" m_%s;\n",sField->strFieldName);
	printf("\tbool m_%s_u;\n",sField->strFieldName);
}
void generateClassConstructAndDestructMethod( struct SClass* sClass  ){
	struct SFiledList* listClassField;
	//生成构造函数
	printf("\t%s():\n",sClass->strClassName);
	for( listClassField = sClass->listClassField ; 
		listClassField != NULL ; listClassField = listClassField->next ){
		printf("\t\tm_%s(),\n",listClassField->sField->strFieldName);
		printf("\t\tm_%s_u(false)",listClassField->sField->strFieldName);
		if( listClassField->next != NULL )
			printf(",\n");
		else
			printf("{\n");
	}
	printf("\t}\n");
	//生成析构函数
	printf("\t~%s(){\n",sClass->strClassName);
	printf("\t}\n");
}
void generateClassFieldMethod( struct SFiled* sField){
	//生成常量获取器
	printf("\tconst ");
	generateClassFieldType( sField->sFieldType );
	printf("& Get%s()const{\n",firstToUpper( sField->strFieldName  ));
	printf("\t\treturn m_%s;\n",sField->strFieldName );
	printf("\t}\n");
	//生成非常量获取器
	/*
	printf("\t");
	generateClassFieldType( sField->sFieldType );
	printf("& Get%s(){\n",firstToUpper( sField->strFieldName  ));
	printf("\t\treturn m_%s;\n",sField->strFieldName );
	printf("\t}\n");
	*/
	//生成常量是否设置获取器
	printf("\tbool Is%sSet()const{\n",firstToUpper( sField->strFieldName  ));
	printf("\t\treturn m_%s_u;\n",sField->strFieldName );
	printf("\t}\n");
	//生成设置器
	printf("\tvoid Set%s( const ",firstToUpper( sField->strFieldName  ));
	generateClassFieldType( sField->sFieldType );
	printf("& %s){\n",sField->strFieldName );
	printf("\t\tm_%s = %s;\n",sField->strFieldName ,sField->strFieldName );
	printf("\t\tm_%s_u = true;\n",sField->strFieldName );
	printf("\t}\n");
}
/*
void generateClassFieldDumpMethod( struct SFiled* sField , int index ){
	if( isBasicType( sField->strFieldName ) == 1 ){
		//基础类型	
		printf( "\t\tos<<m_%s;\n", sField->strFieldName );
	}else if( isListType( sField->strFieldName ) == 1 ){
		//数组类型	
		printf("\t\tos<<\"[\"\n");
		printf("\t\tfor( ");
		generateClassFieldType( sField->sFieldType );
		printf("::const_iterator i%d = m_%s.begin() ; i%d != m_%s.end() ; ++i%d){\n",
				index,
				sField->strFieldName , 
				index,
				sField->strFieldName ,
				index );
		generateClassFieldDumpMethod( sField->sFieldType->sFirstSubFieldType , index + 1 );
		printf("\t\tos<<\",\";\n");	
		printf("\t\t}\n");
	}else if( isMapType( sField->strFieldName ) == 1 ){
		//映射类型	
		printf("\t\tos<<\"[\"\n");
		printf("\t\tfor( ");
		generateClassFieldType( sField->sFieldType );
		printf("::const_iterator i%d = m_%s.begin() ; i%d != m_%s.end() ; ++i%d){\n",
				index,
				sField->strFieldName , 
				index,
				sField->strFieldName ,
				index );
		generateClassFieldDumpMethod( sField->sFieldType->sFirstSubFieldType , index + 1 );
		printf("\t\tos<<\":\";\n");	
		generateClassFieldDumpMethod( sField->sFieldType->sSecondSubFieldType , index + 1 );
		printf("\t\t}\n");
	}else{
		//自定义类型	
		printf("\t\tm_%s.DumpDataTree( os );\n",sField->strFieldName );
	}
}
*/
void generateClassDumpMethod( struct SClass* sClass ){
	struct SFiledList* listClassField;
	//生成DumpData函数
	printf("\tvoid DumpData( std::ostream& os , int32_t indent = 0 )const{\n");	
	printf("\t\tDumpDataTree( os ,indent );\n");
	printf("\t}\n");
	//生成DumpDataTree函数
	printf("\tvoid DumpDataTree( std::ostream& os ,int32_t indent = 0 )const{\n");
	for( listClassField = sClass->listClassField;
		listClassField != NULL ; listClassField = listClassField->next ){
		printf("\t\tg_pprint(os,\"%s\",m_%s,indent);\n", 
			listClassField->sField->strFieldName , 
			listClassField->sField->strFieldName );
	}
	printf("\t}\n");
}
void generateClassToJsonFieldMethod( struct SFieldType* sFieldType, const char* jsonRoot , const char* key , const char* value, int* index ,int mayBeNull ){
	//构造常量
	char strIndex[64];
	char strRoot[64];
	char strVectorPointer[64];
	char strMapFirstPointer[64];
	char strMapSecondPointer[64];
	snprintf(strIndex,sizeof(strIndex),"i%d",*index);
	snprintf(strRoot,sizeof(strRoot),"root_%d",*index);
	snprintf(strVectorPointer,sizeof(strVectorPointer),"(*i%d)",*index);
	snprintf(strMapFirstPointer,sizeof(strMapFirstPointer),"i%d->first",*index);
	snprintf(strMapSecondPointer,sizeof(strMapSecondPointer),"i%d->second",*index);
	(*index)++;
	
	printf("\t\tJson::Value %s;\n",strRoot);
	
	if( isBasicType( sFieldType->strFieldTypeName ) == 1 ){
		//基础类型	
		if( mayBeNull == 1 ){
			printf("\t\tif( %s_u == true ){\n",value);
			printf("\t\t\t%s = Json::Value(%s);\n",strRoot , value);
			printf("\t\t}\n");
		}else{
			printf("\t\t%s = Json::Value(%s);\n",strRoot , value);
		}
	}else if( isListType( sFieldType->strFieldTypeName ) == 1 ){
		//数组类型	
		printf("\t\tfor( ");
		generateClassFieldType( sFieldType );
		printf("::const_iterator %s = %s.begin() ; %s != %s.end() ; ++%s ){\n",
				strIndex,
				value, 
				strIndex,
				value,
				strIndex );
		generateClassToJsonFieldMethod( 
			sFieldType->sFirstSubFieldType ,
			strRoot,	
			"",
			strVectorPointer,
			index ,
			0 );
		printf("\t\t}\n");
	}else if( isMapType( sFieldType->strFieldTypeName ) == 1 ){
		//映射类型	
		printf("\t\tfor( ");
		generateClassFieldType( sFieldType );
		printf("::const_iterator %s = %s.begin() ; %s != %s.end() ; ++%s ){\n",
				strIndex,
				value, 
				strIndex,
				value,
				strIndex );
		generateClassToJsonFieldMethod( 
			sFieldType->sSecondSubFieldType ,
			strRoot,	
			strMapFirstPointer,
			strMapSecondPointer,
			index ,
			0 );
		printf("\t\t}\n");
	}else{
		//自定义类型	
		if( mayBeNull == 1 ){
			printf("\t\tif( %s_u == true ){\n",value);
			printf("\t\t\t%s = %s.ToJson();\n",strRoot , value);
			printf("\t\t}\n");
		}else{
			printf("\t\t%s = %s.ToJson();\n",strRoot , value);
		}
	}
	if( strlen(key) == 0 ){
		printf( "\t\t%s.append(%s);\n", jsonRoot , strRoot );
	}else{
		printf("\t\tss.str(\"\");ss<<%s;\n",key);
		printf( "\t\t%s[ss.str()] = %s;\n", jsonRoot,strRoot );
	}
	printf("\n");
}
void generateClassFromJsonFieldMethod( 
	struct SFieldType* sFieldType, 
	const char* jsonRoot , 
	const char* value , 
	const char* rootKey ,
	const char* rootValue , 
	const char* rootType , 
	int* index ,
	int mayBeNull  ){
	//构造常量
	char strIndex[64];
	char strSingleTemp1[64];
	char strSingleTemp2[64];
	char strSingleTemp3[64];
	char strVectorRoot[64];
	char strMapRoot[64];
	char strType[64];
	
	snprintf(strIndex,sizeof(strIndex),"i%d",*index);
	snprintf(strSingleTemp1,sizeof(strSingleTemp1),"temp1_%d",*index);
	snprintf(strSingleTemp2,sizeof(strSingleTemp2),"temp2_%d",*index);
	snprintf(strSingleTemp3,sizeof(strSingleTemp3),"members_%d",*index);
	snprintf(strVectorRoot,sizeof(strVectorRoot),"%s[%s]",jsonRoot,strIndex);
	snprintf(strMapRoot,sizeof(strMapRoot),"%s[%s[%s]]",jsonRoot,strSingleTemp3,strIndex);
	(*index)++;

	if( isBasicType( sFieldType->strFieldTypeName ) == 1 ){
		//基础类型	
		if( strcmp("bool", sFieldType->strFieldTypeName ) == 0 ){
			//布尔变量
			strcpy(strType,"Bool");
		}else if( strcmp("std::string",sFieldType->strFieldTypeName) == 0){
			strcpy(strType,"String");
		}else if( strcmp("float",sFieldType->strFieldTypeName) == 0
				|| strcmp("double",sFieldType->strFieldTypeName) == 0 ){
			strcpy(strType,"Double");
		}else if( strcmp("int64_t",sFieldType->strFieldTypeName) == 0
				|| strcmp("uint64_t",sFieldType->strFieldTypeName) == 0 ){
			strcpy(strType,"Int");
		}else{
			strcpy(strType,"Int");
		}
		printf("\t\tif( %s.is%s() == true ){\n",jsonRoot,strType);
		printf("\t\t\t%s = %s.as%s();\n",value,jsonRoot,strType);
		if( strcmp("std::vector", rootType ) == 0 
			|| strcmp("std::list", rootType ) == 0 ){
			printf("\t\t\t%s.push_back(%s);\n",rootValue,value);
		}else if( strcmp("std::set", rootType ) == 0 ){
			printf("\t\t\t%s.insert(%s);\n",rootValue,value);
		}else if( strcmp("std::map", rootType ) == 0 ){
			printf("\t\t\t%s[%s] = %s;\n",rootValue,rootKey,value);
		}
		if( mayBeNull == 1  )
			printf("\t\t\t%s_u = true;\n",value);
		printf("\t\t}\n");
	}else if( isListType( sFieldType->strFieldTypeName ) == 1 ){
		//数组类型
		printf("\t\tif( %s.isArray() == true ){\n",jsonRoot);
		printf("\t\tfor( uint32_t %s = 0 ; %s != %s.size() ; ++%s ){\n",strIndex,strIndex,jsonRoot,strIndex);
		printf("\t\t");
		generateClassFieldType( sFieldType->sFirstSubFieldType );
		printf(" %s;\n",strSingleTemp1);
		generateClassFromJsonFieldMethod( 
			sFieldType->sFirstSubFieldType ,
			strVectorRoot,
			strSingleTemp1,
			"",
			value,
			sFieldType->strFieldTypeName,
			index ,
			0 );
		printf("\t\t}\n");
		if( strcmp("std::vector", rootType ) == 0 
			|| strcmp("std::list", rootType ) == 0 ){
			printf("\t\t\t%s.push_back(%s);\n",rootValue,value);
		}else if( strcmp("std::set", rootType ) == 0 ){
			printf("\t\t\t%s.insert(%s);\n",rootValue,value);
		}else if( strcmp("std::map", rootType ) == 0 ){
			printf("\t\t\t%s[%s] = %s;\n",rootValue,rootKey,value);
		}
		if( mayBeNull == 1  )
			printf("\t\t%s_u = true;\n",value);
		printf("\t\t}\n");
	}else if( isMapType( sFieldType->strFieldTypeName ) == 1 ){
		//映射类型
		printf("\t\tif( %s.isObject() == true ){\n",jsonRoot);
		printf("\t\tJson::Value::Members %s = %s.getMemberNames();\n",strSingleTemp3,jsonRoot);
		printf("\t\tfor( uint32_t %s = 0 ; %s != %s.size() ; ++%s ){\n",strIndex,strIndex,strSingleTemp3,strIndex);
		printf("\t\t");
		generateClassFieldType( sFieldType->sFirstSubFieldType );
		printf(" %s;\n",strSingleTemp1);
		printf("\t\t");
		generateClassFieldType( sFieldType->sSecondSubFieldType );
		printf(" %s;\n",strSingleTemp2);
		if( strcmp("std::string",sFieldType->sFirstSubFieldType->strFieldTypeName) == 0 )
			printf("\t\t%s = %s[%s];\n",strSingleTemp1,strSingleTemp3,strIndex);
		else
			printf("\t\t%s = atoll(%s[%s].c_str());\n",strSingleTemp1,strSingleTemp3,strIndex);
		generateClassFromJsonFieldMethod( 
			sFieldType->sSecondSubFieldType ,
			strMapRoot,
			strSingleTemp2,
			strSingleTemp1,
			value,
			sFieldType->strFieldTypeName,
			index ,
			0 );
		printf("\t\t}\n");
		if( strcmp("std::vector", rootType ) == 0 
			|| strcmp("std::list", rootType ) == 0 ){
			printf("\t\t\t%s.push_back(%s);\n",rootValue,value);
		}else if( strcmp("std::set", rootType ) == 0 ){
			printf("\t\t\t%s.insert(%s);\n",rootValue,value);
		}else if( strcmp("std::map", rootType ) == 0 ){
			printf("\t\t\t%s[%s] = %s;\n",rootValue,rootKey,value);
		}
		if( mayBeNull == 1  )
			printf("\t\t%s_u = true;\n",value);
		printf("\t\t}\n");
	}else{
		//自定义类型	
		printf("\t\t%s.FromJson(%s);\n",value,jsonRoot);
	}
	printf("\n");
}
void generateClassJsonMethod( struct SClass* sClass ){
	struct SFiledList* listClassField;
	int index = 1;
	char strRoot[128];
	char strKey[128];
	char strValue[128];

	//生成ToJson函数
	printf("\tJson::Value ToJson()const{\n");
	printf("\t\tJson::Value root;\n");
	printf("\t\tstd::stringstream ss;\n");
	printf("\t\t\n");
	for( listClassField = sClass->listClassField ; 
		listClassField != NULL ; listClassField = listClassField->next ){
		snprintf( strKey , sizeof(strKey) , "\"%s\"" , listClassField->sField->strFieldName );	
		snprintf( strValue , sizeof(strValue) , "m_%s" , listClassField->sField->strFieldName );	
		generateClassToJsonFieldMethod( 
			listClassField->sField->sFieldType ,
			"root",
			strKey,
			strValue,
			&index ,
			1 );
	}
	printf("\t\treturn root;\n");
	printf("\t}\n");
	//生成FromJson函数
	printf("\tvoid FromJson( const Json::Value& root ){\n");
	printf("\t\tif( root.isObject() == false ){\n");
	printf("\t\t\treturn;\n");
	printf("\t\t}\n");
	printf("\t\t\n");
	for( listClassField = sClass->listClassField ; 
		listClassField != NULL ; listClassField = listClassField->next ){
		snprintf( strRoot , sizeof(strRoot) , "root[\"%s\"]" , listClassField->sField->strFieldName );	
		snprintf( strValue , sizeof(strValue) , "m_%s" , listClassField->sField->strFieldName );	
		generateClassFromJsonFieldMethod( 
			listClassField->sField->sFieldType ,
			strRoot,
			strValue,
			"",
			"",
			"",
			&index ,
			1 );
	}
	printf("\t}\n");
}
void generateClass( struct SClass* sClass ){
	struct SFiledList* listClassField;
	//生成类开始
	printf("class %s{\n",sClass->strClassName);
	//生成类变量
	printf("private:\n");
	for( listClassField = sClass->listClassField ; 
		listClassField != NULL ; listClassField = listClassField->next ){
		generateClassField( listClassField->sField );
	}
	printf("public:\n");
	//生成构造函数，析构函数
	generateClassConstructAndDestructMethod( sClass );
	printf("public:\n");
	//生成属性获取器与属性设置器
	for( listClassField = sClass->listClassField ; 
		listClassField != NULL ; listClassField = listClassField->next ){
		generateClassFieldMethod( listClassField->sField );
	}
	//生成Dump函数
	printf("public:\n");
	generateClassDumpMethod( sClass );
	//生成Json函数
	printf("public:\n");
	generateClassJsonMethod( sClass );
	//生成类结束
	printf("};\n");
}
void generate(){
	struct SNameSpaceList* namespace;
	struct SHeaderList* headerList;
	struct SClassList* classList;
	int i;
	//检查g_program
	if( g_program  == NULL ){
		yyerror("parse error!");
		exit(0);
	}
	//生成重复编译头
	printf("#ifndef __");
	for( namespace = g_program->sCode->listNamespaces; 
		namespace != NULL ; namespace = namespace->next ){
		printf("%s_",allToUpper(namespace->strNamespace));
	}
	printf("H__\n");
	printf("#define __");
	for( namespace = g_program->sCode->listNamespaces; 
		namespace != NULL ; namespace = namespace->next ){
		printf("%s_",allToUpper(namespace->strNamespace));
	}
	printf("H__\n\n");
	//生成预定义头文件
	for( i = 0 ;
		 i != sizeof(header_files)/sizeof(header_files[0]) ; ++i ){
		printf("#include %s\n",header_files[i]);
	}
	printf("\n");
	//生成自定义头文件
	for( headerList = g_program->listHeaders ;
		headerList != NULL ; headerList = headerList->next ){
		printf("%s",headerList->strHeader );
	}
	printf("\n");
	//生成命名空间
	for( namespace = g_program->sCode->listNamespaces; 
		namespace != NULL ; namespace = namespace->next ){
		printf("namespace %s{\n",namespace->strNamespace);
	}
	printf("\n");
	//生成类
	for( classList = g_program->sCode->listClasses ;
		 classList != NULL;	classList = classList->next){
		generateClass( classList->strClass );
	}
	printf("\n");
	//生成命名空间
	for( namespace = g_program->sCode->listNamespaces; 
		namespace != NULL ; namespace = namespace->next ){
		printf("}\n");
	}
	printf("\n");
	//生成重复编译头
	printf("\n#endif\n");
}
