extern int row;

inline void lexical_error(char c)
{
    cout << "Lexical Error " << row << ": " << c << endl;
}

inline void compiler_error(string s)
{
    cerr << "Fatal Error " << row << ":" << s << endl;
}

inline void syntax_error(string s)
{
    cerr << "Syntax Error " << row << ":" << s << endl;
}

inline void semantic_error(string s)
{
    cerr << "Semantic Error " << row << ":" << s << endl;
}

inline void yyerror(string s)
{
    syntax_error(s);
}


inline void require(bool cond, string msg)
{
    if (!cond)
        cerr << "Semantic Error " << row << ":" << msg << " required" << endl;
}

