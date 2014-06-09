%{
#define YYSTYPE void*
#include "y.tab.h"
extern int yywrap();
extern int yyerror();
%}
word [A-Za-z_][0-9A-Za-z_]*
symbol [{}:;<>,]
namespace namespace
class class
whitespace [ \r\n\t]+
note [/][/][^\n]*[\n]
header [#]include[ ]*["<][A-Za-z0-9._/ ]*[>"][ ]*[\n]
%%
{header} {
	//printf("%s\n",yytext);
	yylval = strdup( yytext );
	return HEADER;
}
{note} {
	//printf("%s\n",yytext);
}
{class} {
	//printf("%s\n",yytext);
	return CLASS;
}
{namespace} {
	//printf("%s\n",yytext);
	return NAMESPACE;
}
{word} {
	//printf("%s\n",yytext);
	yylval = strdup( yytext );
	return WORD;
}
{symbol} {
	//printf("%s\n",yytext);
	return *yytext;
}
{whitespace} {
	//printf("%s\n",yytext);
}
. {
	//printf("!! %s\n",yytext);
	yyerror("invalid character");
}
%%
int yywrap(){
	return 1;
}
