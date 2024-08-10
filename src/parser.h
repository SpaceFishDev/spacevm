
#include "vm.h"
#include <string.h>
char *readfile(char *fp);
typedef struct
{
    int pos;
    int line, column;
    char *src;
    int len;
} lexer_state;

typedef enum
{
    KEYWORD,
    NUMBER,
    CHARACTER,
    STRING,
    LABEL,
    ENDOFFILE,
} token_type;

typedef struct
{
    token_type type;
    int line, column;
    char *src;
} token_t;
lexer_state *create_lexer(char *src);
token_t lex(lexer_state *lexer);
void display_token(token_t t);
token_t *lex_all(lexer_state *lexer);

typedef struct
{
    int pos;
    token_t *tokens;
} parser_state;

typedef enum
{
    CPUSH8,
    CPUSH32,
    CPUSHSTR,
    CSTORESTR,
    CPRINTSTR,
    CLABEL,
    CPOPA32,
    CPOPA8,
    CPUSHA32,
    CJL,
    CPUSHA8,
    CPOPB32,
    CPOPB8,
    CMOD32,
    CMOD8,
    CCMP32,
    CCMP8,
    CDUP8,
    CDUP32,
    CMK4,
    CJNE,
    CPUTC,
    CPUSHB8,
    CPUSHB32,
    CPRINTI32,
    CADD32,
    CADD8,
    CSWAP8,
    CSWAP32,
    CNONE,

} command_type;

typedef struct
{
    command_type type;
    token_t token;
} command_t;

parser_state *create_parser(char *src);
void check_parser(parser_state *parser);
command_t parse(parser_state *parser);
void display_command(command_t cmd);
command_t *parse_all(parser_state *parser);