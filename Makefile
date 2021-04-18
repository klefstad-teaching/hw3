CCC = g++
CCFLAGS = -c -std=c++11 -ggdb -Wall
LDFLAGS = -ggdb

parser: parse.tab.o
	$(CCC) $(LDFLAGS) parse.tab.o -o parser

parse.tab.o: parse.tab.cpp scan.cpp all.h
	$(CCC) $(CCFLAGS) parse.tab.cpp

all.h: List.h error.h Expr.h Stmt.h
	touch all.h

parse.tab.cpp: parse.y all.h
	bison -v parse.y; /bin/mv parse.tab.c parse.tab.cpp

scan.cpp: scan.l
	flex -t scan.l > scan.cpp

clean:
	/bin/rm -f scan.cpp parser parse.tab.cpp parse.output test.scan1 test.scan2 test.parse core.* parse.tab.o

test: parser
	# ./parser -0 < samples/test.py > test.scan1
	# ./parser -1 < samples/test.py > test.scan2
	# ./parser -2 < samples/test.py > test.parse
	./parser -3 < samples/test.py > test.parse
