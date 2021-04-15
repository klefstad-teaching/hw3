using namespace std;
#include <iostream>
#include "List.h"
#include "error.h"

// #include "Symbol.h"
// #include "SymTab.h"

typedef struct SymbolBlock * Symbol;
typedef struct TypeBlock * Type;
typedef ListPair<Symbol> SymbolPair;
typedef SymbolPair * SymbolList;

#include "Expr.h"
#include "Stmt.h"
// #include "SymUtils.h"
// #include "TypeUtils.h"
