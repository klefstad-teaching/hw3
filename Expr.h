typedef struct ExprBlock * Expr;
typedef ListPair<Expr> ExprPair;
typedef ExprPair * ExprList;

// The root of the Expression tree class hierarchy

struct ExprBlock
{
    Type type;

    ExprBlock(Type ty)
        : type(ty)
    {
    }

    virtual void put(ostream & out)
    {
        compiler_error("Undefined member function: ExprBlock :: put");
    }


// The methods below will be written and used later....
//     let derived classes inherit these defaults for now

    virtual bool isConst()
    {
        return false;
    }

    virtual bool isLocation()
    {
        return false;
    }

    virtual void check()
    {
        compiler_error("Undefined member function: ExprBlock :: check");
    }

    virtual Expr gen()
    {
        compiler_error("Undefined member function: ExprBlock :: gen");
        return 0;
    }

};

inline ostream & operator << (ostream & out, Expr e)
{
    if (e)
        e->put(out);
    else
        out << "NULL";
    return out;
}

inline ostream & operator << (ostream & out, ExprList el)
{
    for (ExprList p = el; p; p=p->next)
    {
        out << p->info;
        if (p->next) out << ' ';
    }
    return out;
}

void put_args(ostream & out, ExprList args)
{
    for (ExprList p = args; p; p=p->next)
    {
        out << p->info;
        if (p->next) out << ", ";
    }
}


struct UnaryExpr
    : ExprBlock
{
    Expr first;
    UnaryExpr(Expr f, Type ty = 0)
        : ExprBlock(ty), first(f)
    {
    }

    static Expr make(Expr f)
    {
        return new UnaryExpr(f);
    }
};


struct BinaryExpr
    : ExprBlock
{
    Expr first;
    Expr second;
    BinaryExpr(Expr f, Expr s, Type ty = 0)
        : ExprBlock(ty), first(f), second(s)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new BinaryExpr(f, s);
    }

};



// ARRAY INDEX Expr


struct IndexedExpr
    : ExprBlock
{
    Expr list;
    Expr index;

    IndexedExpr(Expr lst, Expr ind, Type ty = 0)
        : ExprBlock(ty), list(lst), index(ind)
    {
    }

    static Expr make(Expr lst, Expr ind)
    {
        return new IndexedExpr(lst, ind);
    }

    virtual void put(ostream & out)
    {
        out << list << '[' << index << ']';
    }

};


// MEMBER SELECTION Expr, e.g., obj.member or r.f


struct SelectedExpr
    : ExprBlock
{
    Expr obj;
    string mem;
    SelectedExpr(Expr ob, string m, Type ty = 0)
        : ExprBlock(ty), obj(ob), mem(m)
    {
    }

    static Expr make(Expr o, string m)
    {
        return new SelectedExpr(o, m);
    }

    virtual void put(ostream & out)
    {
        out << obj << '.' << mem;
    }

};


// Identifier (variable or parameter) reference Exprs


struct IdentExpr
    : ExprBlock
{
    string name;

    IdentExpr(string nm, Type ty = 0)
        : ExprBlock(ty), name(nm)
    {
    }

    static Expr make(string name)
    {
        return new IdentExpr(name);
    }

    virtual void put(ostream & out)
    {
        out << name;
    }

};



struct CallExpr
    : ExprBlock
{
    Expr fn;
    ExprList args;

    CallExpr(Expr fun, ExprList ars, Type ty = 0)
        : ExprBlock(ty), fn(fun), args(ars)
    {
    }

    static Expr make(Expr e, ExprList l)
    {
        return new CallExpr(e, l);
    }

    virtual void put(ostream & out)
    {
        out << fn << '(';
        put_args(out, args);
        out << ')';
    }

};


// CONSTANT VALUE Exprs


struct ConstExpr
    : ExprBlock
{
    ConstExpr(Type ty = 0)
        : ExprBlock(ty)
    {
    }

    virtual bool isConst()
    {
        return true;
    }

};


struct BoolConstExpr
    : ConstExpr
{
    int value;

    BoolConstExpr(int v, Type ty = 0)
        : ConstExpr(ty), value(v)
    {
    }

    static Expr make(int v)
    {
        return new BoolConstExpr(v);
    }

    virtual void put(ostream & out)
    {
        out << (value == 0 ? "False" : "True");
    }

};


struct IntConstExpr
    : ConstExpr
{
    int value;

    IntConstExpr(int v, Type ty = 0)
        : ConstExpr(ty), value(v)
    {
    }

    static Expr make(int i)
    {
        return new IntConstExpr(i);
    }

    virtual void put(ostream & out)
    {
        out << value;
    }

};


struct StrConstExpr
    : ConstExpr
{
    string value;

    StrConstExpr(string  v, Type ty = 0)
        : ConstExpr(ty), value(v)
    {
    }

    static Expr make(string v)
    {
        return new StrConstExpr(v);
    }

    virtual void put(ostream & out)
    {
        out << value;
    }

};


struct NoneConstExpr
    : ConstExpr
{
    NoneConstExpr(Type ty = 0)
        : ConstExpr(ty)
    {
    }

    static Expr make()
    {
        return new NoneConstExpr();
    }

    virtual void put(ostream & out)
    {
        out << "None";
    }

};


// Various UNARY Exprs


struct NotExpr
    : UnaryExpr
{
    NotExpr(Expr f, Type ty = 0)
        : UnaryExpr(f, ty)
    {
    }

    static Expr make(Expr f)
    {
        return new NotExpr(f);
    }

    virtual void put(ostream & out)
    {
        out << "not " << first;
    }

};


struct UnaryMinusExpr
    : UnaryExpr
{
    UnaryMinusExpr(Expr f, Type ty = 0)
        : UnaryExpr(f, ty)
    {
    }

    static Expr make(Expr f)
    {
        return new UnaryMinusExpr(f);
    }

    virtual void put(ostream & out) {
        out << '-' << '(' << first << ')';
    }

};


struct UnaryPlusExpr
    : UnaryExpr
{
    UnaryPlusExpr(Expr f, Type ty = 0)
        : UnaryExpr(f, ty)
    {
    }

    static Expr make(Expr f)
    {
        return new UnaryPlusExpr(f);
    }

    virtual void put(ostream & out)
    {
        out << '+' << '(' << first << ')';
    }

};


// Various BINARY Exprs


struct AssignExpr
    : BinaryExpr
{
    AssignExpr(Expr f, Expr s, Type ty = 0)
        : BinaryExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new AssignExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") = (" << second << ')';
    }

};


// Various BINARY ARITHMETIC Exprs


struct ArithmeticExpr
    : BinaryExpr
{
    ArithmeticExpr(Expr f, Expr s, Type ty = 0)
        : BinaryExpr(f, s, ty)
    {
    }
};


struct PlusExpr
    : ArithmeticExpr
{
    PlusExpr(Expr f, Expr s, Type ty = 0)
        : ArithmeticExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new PlusExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") + (" << second << ')';
    }

};


struct MinusExpr
    : ArithmeticExpr
{
    MinusExpr(Expr f, Expr s, Type ty = 0)
        : ArithmeticExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new MinusExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") - (" << second << ')';
    }

};


struct TimesExpr
    : ArithmeticExpr
{
    TimesExpr(Expr f, Expr s, Type ty = 0)
        : ArithmeticExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new TimesExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") * (" << second << ')';
    }

};


struct DivideExpr
    : ArithmeticExpr
{
    DivideExpr(Expr f, Expr s, Type ty = 0)
        : ArithmeticExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new DivideExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") / (" << second << ')';
    }

};

struct ModuloExpr
    : ArithmeticExpr
{
    ModuloExpr(Expr f, Expr s, Type ty = 0)
        : ArithmeticExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new ModuloExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") % (" << second << ')';
    }

};



// Various BINARY LOGICAL Exprs


struct LogicalExpr
    : BinaryExpr
{
    LogicalExpr(Expr f, Expr s, Type ty = 0)
        : BinaryExpr(f, s, ty)
    {
    }
};


struct AndExpr
    : LogicalExpr
{
    AndExpr(Expr f, Expr s, Type ty = 0)
        : LogicalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new AndExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") && (" << second << ')';
    }

};


struct OrExpr
    : LogicalExpr
{
    OrExpr(Expr f, Expr s, Type ty = 0)
        : LogicalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new OrExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") || (" << second << ')';
    }

};



// Various BINARY RELATIONAL Exprs


struct RelationalExpr
    : BinaryExpr
{
    RelationalExpr(Expr f, Expr s, Type ty = 0)
        : BinaryExpr(f, s, ty)
    {
    }
};


struct EQExpr
    : RelationalExpr
{
    EQExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new EQExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") == (" << second << ')';
    }

};


struct NEExpr
    : RelationalExpr
{
    NEExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new NEExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") != (" << second << ')';
    }

};


struct LTExpr
    : RelationalExpr
{
    LTExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new LTExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") < (" << second << ')';
    }

};


struct LEExpr
    : RelationalExpr
{
    LEExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new LEExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") <= (" << second << ')';
    }

};


struct GTExpr
    : RelationalExpr
{
    GTExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new GTExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") > (" << second << ')';
    }

};


struct GEExpr
    : RelationalExpr
{
    GEExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new GEExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") >= (" << second << ')';
    }

};

struct InExpr
    : RelationalExpr
{
    InExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new InExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") in (" << second << ')';
    }

};


struct NotInExpr
    : RelationalExpr
{
    NotInExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new NotInExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") not in (" << second << ')';
    }

};

struct IsExpr
    : RelationalExpr
{
    IsExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new IsExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") is (" << second << ')';
    }

};

struct IsNotExpr
    : RelationalExpr
{
    IsNotExpr(Expr f, Expr s, Type ty = 0)
        : RelationalExpr(f, s, ty)
    {
    }

    static Expr make(Expr f, Expr s)
    {
        return new IsNotExpr(f, s);
    }

    virtual void put(ostream & out)
    {
        out << '(' << first << ") is not (" << second << ')';
    }

};




struct InputExpr
    : ExprBlock
{
    InputExpr(Type ty = 0)
        : ExprBlock(ty)
    {
    }

    static Expr make()
    {
        return new InputExpr();
    }

    virtual void put(ostream & out)
    {
        out << "input()";
    }

};


struct PrintExpr
    : ExprBlock
{
    ExprList args;

    PrintExpr(ExprList ars, Type ty = 0)
        : ExprBlock(ty), args(ars)
    {
    }

    static Expr make(ExprList args)
    {
        return new PrintExpr(args);
    }

    virtual void put(ostream & out)
    {
        out << "print(";
        put_args(out, args);
        out << ")";
    }

};



// This is for class object constructor calls, e.g., s: Student = Student("Peter", "Anteater", 123456789)

struct ObjConstrExpr
    : ExprBlock
{
    string name;
    ExprList args;
    ObjConstrExpr(string nm, ExprList ar, Type ty = 0)
        : ExprBlock(ty), name(nm), args(ar)
    {
    }

    static Expr make(string nm, ExprList ar)
    {
        return new ObjConstrExpr(nm, ar);
    }

    virtual void put(ostream & out)
    {
        out << name << '(' << args << ')' << endl;
    }

};



// this is for literal lists, e.g., [1,3,5,7,9] or ["a","b"]

struct ListExpr
    : ExprBlock
{
    ExprList elements;

    ListExpr(ExprList el, Type ty = 0)
        : ExprBlock(ty), elements(el)
    {
    }

    static Expr make(ExprList el)
    {
        return new ListExpr(el);
    }

    virtual void put(ostream & out)
    {
        out << '[';
        out << elements;
        out << ']';
    }

};


