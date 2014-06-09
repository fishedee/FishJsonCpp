.PHONY:a.out lex yacc compile
a.out:
	$(MAKE) lex;
	$(MAKE) yacc;
	$(MAKE) compile;
lex:
	lex main.l
yacc:
	yacc -d main.y
compile:
	gcc main.c lex.yy.c y.tab.c struct.c -DYYDEBUG
clean:
	-rm lex.yy.c y.tab.c y.tab.h a.out
test:
	cd ../test && $(MAKE) 
