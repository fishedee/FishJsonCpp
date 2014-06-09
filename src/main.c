#include <stdio.h>
extern int yylineno;
extern int yyparse();
extern int yyerror( char* msg );
extern void generate();
int main(){
	yyparse();
	generate();
	return 0;
}
int yyerror( char* msg ){
	printf("Error Encounter:%s LineNumber:%d\n",msg ,yylineno);
}
