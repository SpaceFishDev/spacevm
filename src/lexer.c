#include "parser.h"
lexer_state *create_lexer(char *src)
{
    if (!src)
    {
        panic("SRC is null.");
    }
    lexer_state *res = malloc(sizeof(lexer_state));
    res->src = src;
    res->len = strlen(src);
    res->line = 1;
    res->column = 0;
    res->pos = 0;
    return res;
}

void check_lexer(lexer_state *lexer)
{
    if (!lexer)
    {
        panic("Lexer not initialized.");
    }
}

char get_cur(lexer_state *lexer)
{
    check_lexer(lexer);
    if (lexer->len <= lexer->pos)
    {
        return 0;
    }
    return lexer->src[lexer->pos];
}

void next(lexer_state *lexer)
{
    check_lexer(lexer);
    ++lexer->pos;
    ++lexer->column;
}

token_t lex_comment(lexer_state *lexer);
token_t lex_char(lexer_state *lexer);
token_t lex_str(lexer_state *lexer);
token_t lex_number(lexer_state *lexer);
token_t lex_keyword(lexer_state *lexer);

#define TOK(type, src, lexer) \
    ((token_t){type, lexer->line, lexer->column, src})

token_t lex(lexer_state *lexer)
{
    check_lexer(lexer);
    char c = get_cur(lexer);
    switch (c)
    {
    case '/':
    {
        return lex_comment(lexer);
    }
    case '\'':
    {
        return lex_char(lexer);
    }
    case '"':
    {
        return lex_str(lexer);
    }
    case '\0':
    {
        return TOK(ENDOFFILE, "\0", lexer);
    }
    case ' ':
    case '\t':
    {
        next(lexer);
        return lex(lexer);
    }
    case '\n':
    {
        next(lexer);
        lexer->column = 0;
        lexer->line++;
        return lex(lexer);
    }
    default:
    {
        if (c >= '0' && c <= '9')
        {
            return lex_number(lexer);
        }
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            return lex_keyword(lexer);
        }
        return TOK(ENDOFFILE, "\0", lexer);
        // keyword or number
    }
    }
    return TOK(ENDOFFILE, "\0", lexer);
}

token_t lex_comment(lexer_state *lexer)
{
    check_lexer(lexer);
    next(lexer);
    if (get_cur(lexer) == '/')
    {
        while (get_cur(lexer) && get_cur(lexer) != '\n')
        {
            next(lexer);
        }
    }
    return lex(lexer);
}

token_t lex_char(lexer_state *lexer)
{
    check_lexer(lexer);
    next(lexer);
    char c = get_cur(lexer);
    if (c == '\\')
    {
        next(lexer);
        switch (get_cur(lexer))
        {
        case 'n':
        {
            c = '\n';
        }
        break;
        case 't':
        {
            c = '\t';
        }
        break;
        }
    }
    next(lexer);
    if (c == 0 || get_cur(lexer) != '\'')
    {
        char buffer[500];
        sprintf(buffer, "Unclosed char on [ln: %d col: %d]", lexer->line, lexer->column);
        panic(buffer);
    }
    char *str = malloc(2);
    str[0] = c;
    str[1] = 0;
    token_t t = TOK(CHARACTER, str, lexer);
    next(lexer);
    return t;
}

token_t lex_str(lexer_state *lexer)
{
    check_lexer(lexer);
    next(lexer);
    int len = 0;
    int col = lexer->column;
    while (get_cur(lexer) && get_cur(lexer) != '"')
    {
        ++len;
        next(lexer);
    }
    lexer->pos -= len;
    lexer->column = col;
    char *buffer = malloc(len + 1);
    int i = 0;
    while (get_cur(lexer) && get_cur(lexer) != '"')
    {
        if (get_cur(lexer) == '\\')
        {
            next(lexer);
            if (get_cur(lexer) == 'n')
            {
                buffer[i] = '\n';
                next(lexer);
                continue;
            }
            if (get_cur(lexer) == 't')
            {
                buffer[i] = '\t';
                next(lexer);
                continue;
            }
            --lexer->pos;
            --lexer->column;
        }
        buffer[i] = get_cur(lexer);
        ++i;
        next(lexer);
    }
    buffer[len] = 0;
    next(lexer);
    return TOK(STRING, buffer, lexer);
}

bool is_num(char c)
{
    return c >= '0' && c <= '9';
}

bool is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

token_t lex_number(lexer_state *lexer)
{
    check_lexer(lexer);
    int len = 0;
    int col = lexer->column;
    while (is_num(get_cur(lexer)) && (get_cur(lexer)))
    {
        ++len;
        next(lexer);
    }
    char *src = malloc(len + 1);
    src[len] = 0;
    lexer->pos -= len;
    lexer->column = col;
    int i = 0;
    while (is_num(get_cur(lexer)))
    {
        src[i] = get_cur(lexer);
        ++i;
        next(lexer);
    }
    return TOK(NUMBER, src, lexer);
}

bool is_lexer_char(char c)
{
    return (c && (is_num(c) || is_letter(c) || (c == '_')));
}

token_t lex_keyword(lexer_state *lexer)
{
    check_lexer(lexer);
    int len = 0;
    int col = lexer->column;
    while (is_lexer_char(get_cur(lexer)))
    {
        ++len;
        next(lexer);
    }
    char *buffer = malloc(len + 1);
    buffer[len] = 0;
    lexer->column = col;
    lexer->pos -= len;
    int i = 0;
    while (is_lexer_char(get_cur(lexer)))
    {
        buffer[i] = get_cur(lexer);
        ++i;
        next(lexer);
    }
    if (get_cur(lexer) == ':')
    {
        next(lexer);
        return TOK(LABEL, buffer, lexer);
    }
    return TOK(KEYWORD, buffer, lexer);
}

token_t *lex_all(lexer_state *lexer)
{
    token_t *tokens = malloc(sizeof(token_t));
    int sz = 0;
    int i = 0;
    while (true)
    {
        token_t t = lex(lexer);
        tokens = realloc(tokens, sz + sizeof(token_t));
        tokens[i] = t;
        sz += sizeof(token_t);
        ++i;
        if (t.type == ENDOFFILE)
        {
            return tokens;
        }
    }
    printf("lexed\n");
    return tokens;
}

void display_token(token_t t)
{
    printf("[type= %d]@[%d,%d] = %s\n", t.type, t.line, t.column, t.src);
}