
enum k_words
{
    NIL,
    INVADE,
    CYCLE,
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
    READ,
    SUDDENLY,
    ELSE,
    COMMA,
    RETURN
};


const int K_WORDS_NUMBER = 20;
static char* key_words_str[] =
{
    "null",
    "invades",
    "till_the_end",
    "AVE_MARIA!",
    "DEUS_VULT!",
    "glorious_victory:",
    "slight_victory:",
    "grievous_defeat:",
    "slight_defeat:",
    "equal_forces:",
    "unequal_forces:",
    "assign",
    "performs_in_senate",
    "(",
    ")",
    "read",
    "suddenly",
    "else:",
    ",",
    "return"
};


enum node_types {
                        NUMBER,
                        OPERATOR,
                        VAR,
                        K_WORD,
                        LINK,
                        END_LINE
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

static const char* MAIN = "main";
