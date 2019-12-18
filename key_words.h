
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
    RETURN,
    WRITE,
    IF
};


const int K_WORDS_NUMBER = 22;
static char* key_words_str[] =
{
    "null",
    "нападает_на",
    "Бой_до_последней_капли_крови",
    "AVE_MARIA!",
    "DEUS_VULT!",
    "Славная_победа:",
    "Победа:",
    "Горькое_поражение:",
    "Поражение:",
    "Равные_силы:",
    "Неравные_силы:",
    "имеет_силу",
    "выступает_в_сенате",
    "(",
    ")",
    "познает_волю_богов",
    "Вдруг",
    "Если_нет:",
    ",",
    "В_Рим_возвращается",
    "Объявить_плебеям",
    "IF"
};


enum node_types {
                        NUMBER,
                        OPERATOR,
                        VAR,
                        K_WORD,
                        LINK,
                        END_LINE,
                        CALL,
                        STRING
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
                        ARCTAN,
                        SQRT,
                        DER
                    };

const int OPERATOR_NUMBER = 22;
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
    "корень",
    "производная"
};

static const char* NEMO = "NIHIL";

static const char* MAIN = "Рим";
