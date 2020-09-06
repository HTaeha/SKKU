all:
	flex taeha.l
	bison -d taeha.y
	gcc lex.yy.c taeha.tab.c print.c -o taeha -lfl -g

clean:
	rm -rf lex.yy.c taeha.tab.c taeha.tab.h taeha
