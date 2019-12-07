
enum k_words
{
    NIL,
    INVADE,
    CYCLE,
    ENDLN,
    BEGIN,
    END,
    MORE,
    EMORE,
    LESS,
    ELESS,
    EQUAL,
    UNEQUAL,
    ASSIGN,
    PRINT,
    OPEN_BR,
    CLOSE_BR,
    READ
};


const int K_WORDS_NUMBER = 17;
static char* key_words_str[] =
{
    "null",
    "invades",
    "till_the_end",
    "#",
    "begin",
    "end",
    "glorious victory:",
    "slight victory:",
    "grievous defeat:",
    "slight defeat:",
    "equal forces:",
    "unequal forces:",
    "assign",
    "performs_in_senate",
    "(",
    ")",
    "read"
};


enum node_types {
                        NUMBER,
                        OPERATOR,
                        VAR,
                        K_WORD,
                        LINK
                };

enum operator_num   {
                        ZERO,
                        ADD,
                        MIN,
                        MUL,
                        DIV,
                        POW,
                        LOG,
                        LG,
                        LN,
                        SIN,
                        COS,
                        TAN,
                        COTAN,
                        SH,
                        CH,
                        TH,
                        CTH,
                        ARCSIN,
                        ARCCOS,
                        ARCTAN
                    };

const int OPERATOR_NUMBER = 20;
static char* oper_str[] =
{
    "null",
    "+",
    "-",
    "*",
    "/",
    "^",
    "log",
    "lg",
    "ln",
    "sin",
    "cos",
    "tan",
    "ctan",
    "sh",
    "ch",
    "th",
    "cth",
    "arcsin",
    "arccos",
    "arctan",
};
