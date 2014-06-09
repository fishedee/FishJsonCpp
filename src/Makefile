.PHONY:fishjson lex yacc compile
fishjson:
	$(MAKE) lex;
	$(MAKE) yacc;
	$(MAKE) compile;
lex:
	lex main.l
yacc:
	yacc -d main.y
compile:
	gcc main.c lex.yy.c y.tab.c struct.c -DYYDEBUG -o fishjson
clean:
	-rm lex.yy.c y.tab.c y.tab.h fishjson
test:
	cd ../test && $(MAKE) 
