CCC = g++
CCFLAGS = -c -std=c++11 -ggdb -Wall
LDFLAGS = -ggdb

parser: parse.tab.cpp scan.cpp
	$(CCC) $(CCFLAGS) $(LDFLAGS) parse.tab.cpp -o parser

all.h: List.h error.h Expr.h Stmt.h
	touch all.h

parse.tab.cpp: parse.y all.h
	bison -v parse.y; /bin/mv parse.tab.c parse.tab.cpp

scan.cpp: scan.l
	flex -t scan.l > scan.cpp

clean:
	/bin/rm -f scan.cpp parser parse.tab.cpp parse.output test.scan1 test.scan2 test.parse

test: parser
	./parser -0 < test.py > test.scan1
	./parser -1 < test.py > test.scan2
	./parser -2 < test.py > test.parse
