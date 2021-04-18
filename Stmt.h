typedef struct StmtBlock * Stmt;

typedef ListPair<Stmt> StmtPair;
typedef StmtPair * StmtList;

// The root of the Statement tree class hierarchy

struct StmtBlock
{
    StmtBlock()
    {
    }

    virtual void put(ostream & out)
    {
        compiler_error("Undefined member function: StmtBlock :: put");
    }


    static Stmt make()
    {
        compiler_error("Attempt to create instance of abstract base class StmtBlock");
        return new StmtBlock();
    }

    virtual void check()
    {
        compiler_error("Undefined member function: StmtBlock :: check");
    }

    virtual void gen()
    {
        compiler_error("Undefined member function: StmtBlock :: gen");
    }

};

inline ostream & operator << (ostream & out, Stmt s)
{
    if (s)
        s->put(out);
    else
        out << "NULL";
    return out;
}


void put(ostream &out, StmtList stmts)
{
    for (StmtList p = stmts; p; p=p->next)
    {
        out << p->info;
    }
}


inline ostream & operator << (ostream & out, StmtList l)
{
    if (l)
        put(out, l);
    else
        out << "NULL";
    return out;
}



struct IfStmt
    : StmtBlock
{
    Expr cond;
    Stmt trueStmt;
    Stmt falseStmt; // could be null or another IfStmt, Else has cond = True

    IfStmt(Expr c, Stmt t, Stmt f = 0)
        : StmtBlock(), cond(c), trueStmt(t), falseStmt(f)
    {
    }

    static Stmt make(Expr c, Stmt t, Stmt f)
    {
        return new IfStmt(c, t, f);
    }

    virtual void put(ostream & out)
    {
        out << "if ( " << cond << " )\n" << trueStmt;
        if (falseStmt)
            out << falseStmt;
    }

};


struct ForStmt
    : StmtBlock
{
    string ident;
    Expr ex;
    Stmt stmt;
    ForStmt(string i, Expr e, Stmt s)
        : StmtBlock(), ident(i), ex(e), stmt(s)
    {
    }

    static Stmt make(string i, Expr e, Stmt s)
    {
        return new ForStmt(i, e, s);
    }

    virtual void put(ostream & out)
    {
        out << "for (" << ident << " in " << ex << ")\n"
            << stmt << endl;
    }

};


struct WhileStmt
    : StmtBlock
{
    Expr cond;
    Stmt stmt;
    WhileStmt(Expr c, Stmt s)
        : StmtBlock(), cond(c), stmt(s)
    {
    }

    static Stmt make(Expr c, Stmt s)
    {
        return new WhileStmt(c, s);
    }

    virtual void put(ostream & out)
    {
        out << "while (" << cond << ")\n" << stmt << endl;
    }

};


struct ReturnStmt
    : StmtBlock
{
    Expr expr;
    ReturnStmt(Expr e)
        : StmtBlock(), expr(e)
    {
    }

    static Stmt make(Expr e)
    {
        return new ReturnStmt(e);
    }

    virtual void put(ostream & out)
    {
        out << "return";
        if (expr)
            out << " " << expr;
        out << ';' << endl;
    }

};

struct BlockStmt
    : StmtBlock
{
    StmtList stmts;
    BlockStmt(StmtList sl)
        : StmtBlock(), stmts(sl)
    {
    }

    static Stmt make(StmtList sl)
    {
        return new BlockStmt(sl);
    }

    virtual void put(ostream & out)
    {
        out << "{\n";
        out << stmts;
        out << "}\n";
    }

};

struct CallStmt
    : StmtBlock
{
    Expr object;
    CallStmt(Expr o)
        : StmtBlock(), object(o)
    {
    }

    static Stmt make(Expr o)
    {
        return new CallStmt(o);
    }

    virtual void put(ostream & out)
    {
        out << object << ";\n";
    }

};

struct AssignStmt
    : StmtBlock
{
    Expr object;
    AssignStmt(Expr o)
        : StmtBlock(), object(o)
    {
    }

    static Stmt make(Expr o)
    {
        return new AssignStmt(o);
    }

    virtual void put(ostream & out)
    {
        out << object << ";\n";
    }

};

struct PassStmt
    : StmtBlock
{
    PassStmt()
        : StmtBlock()
    {
    }

    static Stmt make()
    {
        return new PassStmt();
    }

    virtual void put(ostream & out)
    {
        out << "Pass\n";
    }

};

struct BreakStmt
    : StmtBlock
{
    BreakStmt()
        : StmtBlock()
    {
    }

    static Stmt make()
    {
        return new BreakStmt();
    }

    virtual void put(ostream & out)
    {
        out << "break\n";
    }

};

struct ContinueStmt
    : StmtBlock
{
    ContinueStmt()
        : StmtBlock()
    {
    }

    static Stmt make()
    {
        return new ContinueStmt();
    }

    virtual void put(ostream & out)
    {
        out << "continue\n";
    }

};



/* These are declarations */

struct VarStmt
    : StmtBlock
{
    string name;
    Type type;
    Expr init;

    VarStmt(string nm, Type ty, Expr i)
        : StmtBlock(), name(nm), type(ty), init(i)
    {
    }

    static Stmt make(string nm, Type ty, Expr i)
    {
        return new VarStmt(nm, ty, i);
    }

    virtual void put(ostream & out)
    {
        out << name << ": " << type;
        if (init)
        {
            out << " = ";
            out << init;
        }
        out << ";\n";
    }

};


struct ParamStmt
    : StmtBlock
{
    string name;
    Type type;

    ParamStmt(string nm, Type ty)
        : StmtBlock(), name(nm), type(ty)
    {
    }

    static Stmt make(string nm, Type ty)
    {
        return new ParamStmt(nm, ty);
    }

    virtual void put(ostream & out)
    {
        out << name << ":" << type;
    }

};


struct DefStmt
    : StmtBlock
{
    string name;
    StmtList params;
    Type ret_type;
    Stmt body;

    DefStmt(string nm, StmtList prms, Type rt, Stmt bdy)
        : StmtBlock(), name(nm), params(prms), ret_type(rt), body(bdy)
    {
    }

    static Stmt make(string nm, StmtList prms, Type rt, Stmt bdy)
    {
        return new DefStmt(nm, prms, rt, bdy);
    }

    virtual void put(ostream & out)
    {
        out << "Def " << name << "(" << params << ")";
        if (ret_type)
        {
            out << " -> " << ret_type;
        }
        out << ":";
        out << body;
    }

};


struct ClassStmt
    : StmtBlock
{
    string name;
    TypeList bases;
    Stmt body;

    ClassStmt(string nm, TypeList bc, Stmt bdy)
        : StmtBlock(), name(nm), bases(bc), body(bdy)
    {
    }

    static Stmt make(string nm, TypeList bc, Stmt bdy)
    {
        return new ClassStmt(nm, bc, bdy);
    }

    virtual void put(ostream & out)
    {
        out << "Class " << name << "(" << bases << "):" ;
        out << body;
    }

};


