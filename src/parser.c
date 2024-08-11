#include "parser.h"

char *readfile(char *fp)
{
    FILE *file = fopen(fp, "rb");
    if (!file)
    {
        char buffer[100];
        sprintf(buffer, "Could not read file: '%s'", fp);
        panic(buffer);
        return 0;
    }
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(len + 1);
    int x = fread(buffer, len, 1, file);
    if (!x)
    {
        printf("Reading failed\n");
    }
    fclose(file);

    return buffer;
}

parser_state *create_parser(char *src)
{
    lexer_state *lexer = create_lexer(src);
    token_t *tokens = lex_all(lexer);
    free(lexer);
    parser_state *parser = malloc(sizeof(parser_state));
    parser->pos = 0;
    parser->tokens = tokens;
    return parser;
}

void check_parser(parser_state *parser)
{
    if (!parser)
    {
        panic("Parser not initialized.");
    }
}

token_t current(parser_state *parser)
{
    check_parser(parser);
    return parser->tokens[parser->pos];
}

#include <ctype.h>

char *toLower(char *str)
{
    char *s = calloc(1, strlen(str) + 1);
    strcpy(s, str);
    for (char *p = s; *p; p++)
        *p = tolower(*p);
    return s;
}

#define CMD(type, token) \
    ((command_t){type, token})

bool expect(parser_state *parser, token_type type)
{
    check_parser(parser);
    return parser->tokens[parser->pos + 1].type == type;
}

#define PARSE_ERROR(token, err_str)                                          \
    char *err = malloc(1000);                                                \
    sprintf(err, "%s [ln: %d, col: %d]", err_str, token.line, token.column); \
    panic(err)

command_t parse(parser_state *parser)
{
    check_parser(parser);
    token_t t = current(parser);
    switch (t.type)
    {
    case LABEL:
    {
        ++parser->pos;
        return CMD(CLABEL, parser->tokens[parser->pos - 1]);
    }
    break;
    case KEYWORD:
    {
        char *value = toLower(t.src);
        if (strcmp(value, "push") == 0)
        {
            if (expect(parser, NUMBER))
            {
                parser->pos += 2;
                token_t token = parser->tokens[parser->pos - 1];
                free(value);
                return CMD(CPUSH8, token);
            }
            if (expect(parser, CHARACTER))
            {
                parser->pos += 2;
                token_t token = parser->tokens[parser->pos - 1];
                free(value);
                return CMD(CPUSH8, token);
            }
            if (!expect(parser, STRING))
            {
                PARSE_ERROR(t, "Expected a string or uint8");
            }
            parser->pos += 2;
            token_t token = parser->tokens[parser->pos - 1];
            free(value);
            return CMD(CPUSHSTR, token);
        }
        if (strcmp(value, "push32") == 0)
        {
            if (!expect(parser, NUMBER))
            {
                PARSE_ERROR(t, "Expected a uint32");
            }
            parser->pos += 2;
            token_t token = parser->tokens[parser->pos - 1];
            free(value);
            return CMD(CPUSH32, token);
        }
        if (strcmp(value, "store_str") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CSTORESTR, t);
        }
        if (strcmp(value, "print_str") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPRINTSTR, t);
        }
        if (strcmp(value, "popa32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPOPA32, t);
        }
        if (strcmp(value, "popa8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPOPA8, t);
        }
        if (strcmp(value, "popb32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPOPB32, t);
        }
        if (strcmp(value, "popb8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPOPB8, t);
        }
        if (strcmp(value, "dup8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CDUP8, t);
        }
        if (strcmp(value, "dup32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CDUP32, t);
        }
        if (strcmp(value, "mod8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CMOD8, t);
        }
        if (strcmp(value, "mod32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CMOD32, t);
        }
        if (strcmp(value, "cmp32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CCMP32, t);
        }
        if (strcmp(value, "cmp8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CCMP8, t);
        }
        if (strcmp(value, "mk4") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CMK4, t);
        }
        if (strcmp(value, "jne") == 0)
        {
            if (!expect(parser, KEYWORD))
            {
                PARSE_ERROR(t, "Expected keyword");
            }
            ++parser->pos;
            token_t token = parser->tokens[parser->pos];
            free(value);
            ++parser->pos;
            return CMD(CJNE, token);
        }
        if (strcmp(value, "jl") == 0)
        {
            if (!expect(parser, KEYWORD))
            {
                PARSE_ERROR(t, "Expected keyword");
            }
            ++parser->pos;
            token_t token = parser->tokens[parser->pos];
            free(value);
            ++parser->pos;
            return CMD(CJL, token);
        }
        if (strcmp(value, "putc") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPUTC, t);
        }
        if (strcmp(value, "pushb32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPUSHB32, t);
        }
        if (strcmp(value, "pushb8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPUSHB8, t);
        }
        if (strcmp(value, "printi32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPRINTI32, t);
        }
        if (strcmp(value, "add32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CADD32, t);
        }
        if (strcmp(value, "add8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CADD8, t);
        }
        if (strcmp(value, "swap8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CSWAP8, t);
        }
        if (strcmp(value, "swap32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CSWAP32, t);
        }
        if (strcmp(value, "pusha8") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPUSHA8, t);
        }
        if (strcmp(value, "pusha32") == 0)
        {
            ++parser->pos;
            free(value);
            return CMD(CPUSHA32, t);
        }

        free(value);
    }
    break;
    default:
    {
    }
    }
    return CMD(CNONE, t);
}
void display_command(command_t cmd)
{
    printf("CMD: Type= %d\n", cmd.type);
    display_token(cmd.token);
}
command_t *parse_all(parser_state *parser)
{
    command_t *commands = malloc(sizeof(command_t));
    int i = 0;
    int sz = 1;
    while (true)
    {
        command_t cmd = parse(parser);
        commands[i] = cmd;
        ++i;
        ++sz;
        commands = realloc(commands, sizeof(command_t) * sz);
        if (cmd.type == CNONE)
        {
            return commands;
        }
    }
    // should never reach here but compiler warnings
    return commands;
}

instruction_t *compile(command_t command, int *len)
{
    switch (command.type)
    {
    case CPUSH8:
    {
        uint8_t value = 0;
        switch (command.token.type)
        {
        case CHARACTER:
        {
            value = command.token.src[0];
        }
        break;
        case NUMBER:
        {
            uint32_t val = atoi(command.token.src);
            value = (uint8_t)val;
        }
        break;
        default:
        {
            // shouldnt be possible
            value = 0;
        }
        }
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(PUSH, value);
        *len = 1;
        return ins;
    }
    case CPUSH32:
    {
        uint32_t value = 0;
        switch (command.token.type)
        {
        case NUMBER:
        {
            value = atoi(command.token.src);
        }
        break;
        default:
        {
            value = 0;
        }
        break;
        }
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(PUSH32, value);
        *len = 1;
        return ins;
    }
    case CPUSHSTR:
    {
        char *str = command.token.src;
        instruction_t *instrucions = malloc(sizeof(instruction_t) * (strlen(str) + 1));
        int length = strlen(str);
        int x = 0;
        for (int i = length; i > 0; --i)
        {
            instrucions[x] = INS(PUSH, str[i]);
            ++x;
        }
        *len = (length);
        return instrucions;
    }
    case CPUSHA32:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(PUSHA32, 0);
        *len = 1;
        return ins;
    }
    case CPUSHA8:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(PUSHA8, 0);
        *len = 1;
        return ins;
    }
    case LABEL:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(INS_LABEL, 0);
        *len = 1;
        return ins;
    }
    case CPOPB32:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(POPB32, 0);
        *len = 1;
        return ins;
    }
    case CPOPB8:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(POPB8, 0);
        *len = 1;
        return ins;
    }
    case CMOD32:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(MOD32, 0);
        *len += 1;
        return ins;
    }
    case CMOD8:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(MOD, 0);
        *len += 1;
        return ins;
    }
    case CCMP32:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(CMP32, 0);
        *len += 1;
        return ins;
    }
    case CCMP8:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(CMP, 0);
        *len += 1;
        return ins;
    }
    default:
    {
        instruction_t *ins = malloc(sizeof(instruction_t));
        *ins = INS(NOP, 0);
        *len = 1;
        return ins;
    }
    }
}