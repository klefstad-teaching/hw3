template
    <typename T>
struct ListPair
{
    T info;
    ListPair *next;
    ListPair(T in, ListPair *ne) :
            info(in), next(ne)
    {
    }

    static void put(ostream & out, ListPair<T>* l)
    {   
        for (; l; l=l->next)
        {   
            l->info->put(out);
            if (l->next)
                out << ", ";
        }   
    }   
};


template
    <typename T>
ostream & operator << (ostream & out, ListPair<T> *l)
{
    ListPair<T> :: put(out, l);
    return out;
}
