extern int lineNumber;

inline void lexical_error(char c)
{
    cout << "Lexical Error " << lineNumber << ": " << c << endl;
}

inline void compiler_error(string s)
{
    cerr << "Fatal Error " << lineNumber << ":" << s << endl;
}

inline void syntax_error(string s)
{
    cerr << "Syntax Error " << lineNumber << ":" << s << endl;
}

inline void semantic_error(string s)
{
    cerr << "Semantic Error " << lineNumber << ":" << s << endl;
}

inline void yyerror(string s)
{
    syntax_error(s);
}


inline void require(bool cond, string msg)
{
    if (!cond)
        cerr << "Semantic Error " << lineNumber << ":" << msg << " required" << endl;
}

