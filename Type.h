typedef struct TypeBlock * Type;

typedef ListPair<Type> TypePair;
typedef TypePair * TypeList;

struct TypeBlock
{
    TypeBlock()
    {
    }

    virtual void put(ostream & out)
    {
        compiler_error("Undefined member function: TypeBlock :: put");
    }


    static Type make()
    {
        compiler_error("Attempt to create instance of abstract base class TypeBlock");
        return new TypeBlock();
    }

};

inline ostream & operator << (ostream & out, Type t)
{
    if (t)
        t->put(out);
    else
        out << "NULL";
    return out;
}


void put(ostream &out, TypeList types)
{
    for (TypeList p = types; p; p=p->next)
    {
        out << p->info;
        if (p->next) out << ", ";
    }
}


inline ostream & operator << (ostream & out, TypeList l)
{
    if (l)
        put(out, l);
    else
        out << "NULL";
    return out;
}



struct IdentType
    : TypeBlock
{
    string name;

    IdentType(string nm)
        : name(nm)
    {
    }

    static Type make(string nm)
    {
        return new IdentType(nm);
    }

    virtual void put(ostream & out)
    {
        out << name;
    }

};


struct ListType
    : TypeBlock
{
    Type elementType;

    ListType(Type et)
        : TypeBlock(), elementType(et)
    {
    }

    static Type make(Type et)
    {
        return new ListType(et);
    }

    virtual void put(ostream & out)
    {
        out << '[' << elementType << ']';
    }

};
