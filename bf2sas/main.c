#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

enum types
{
    ADD,
    SUB,
    RIGHT,
    LEFT,
    LOOP,
    LOOPEND,
    OUTPUT,
    INPUT,
    END,
};

char *type_str(int t)
{
    char *values[] = {"ADD", "SUB", "RIGHT", "LEFT", "LOOP", "LOOPEND", "OUTPUT", "INPUT", "END"};
    if (t > END)
    {
        return "Unknown";
    }
    return values[t];
}

typedef struct
{
    int num;
    int type;
} tok;

int lex_pos = 0;
tok lex(char *src)
{
    if (lex_pos > strlen(src))
    {
        return (tok){1, END};
    }
    switch (src[lex_pos])
    {
    case '\0':
    {
        return (tok){
            .num = 1,
            .type = END,
        };
    }
    case '+':
    {
        int num = 0;
        while (src[lex_pos] == '+')
        {
            ++num;
            ++lex_pos;
        }
        return (tok){num, ADD};
    }
    case '-':
    {
        int num = 0;
        while (src[lex_pos] == '-')
        {
            ++num;
            ++lex_pos;
        }
        return (tok){num, SUB};
    }
    case '>':
    {
        int num = 0;
        while (src[lex_pos] == '>')
        {
            ++num;
            ++lex_pos;
        }
        return (tok){num, RIGHT};
    }
    case '<':
    {
        int num = 0;
        while (src[lex_pos] == '<')
        {
            ++num;
            ++lex_pos;
        }
        return (tok){num, LEFT};
    }
    case '[':
    {
        ++lex_pos;
        return (tok){1, LOOP};
    }
    case ']':
    {
        ++lex_pos;
        return (tok){1, LOOPEND};
    }
    case '.':
    {
        ++lex_pos;
        return (tok){1, OUTPUT};
    }
    case ',':
    {
        ++lex_pos;
        return (tok){1, INPUT};
    }
    default:
    {
        ++lex_pos;
        return lex(src);
    }
    }
}

tok *lex_all(char *src, int *len_o)
{
    tok *tokens = malloc(sizeof(tok));
    int len = strlen(src);
    int i = 1;
    while (lex_pos < len)
    {
        tokens[i - 1] = lex(src);
        if (tokens[i - 1].type == END)
        {
            return tokens;
        }
        ++i;
        ++*len_o;
        tokens = realloc(tokens, i * sizeof(tok));
    }
    return tokens;
}
char *compile(tok t)
{
    switch (t.type)
    {
    case ADD:
    {
        char *out = malloc(100);
        sprintf(out, "pushb32\nload32\npush32 %d\nadd32\npushb32\nstore32\n", t.num);
        return out;
    }
    case SUB:
    {
        char *out = malloc(100);
        sprintf(out, "push32 %d\npushb32\nload32\nsub32\npushb32\nstore32\n", t.num);
        return out;
    }
    case OUTPUT:
    {
        char *out = malloc(100);
        sprintf(out, "pushb32\nload32\nmk1\nputc\n");
        return out;
    }
    case INPUT:
    {
        char *out = malloc(100);
        sprintf(out, "readc\nmk4\npushb32\nstore32\n");
        return out;
    }
    case RIGHT:
    {
        char *out = malloc(100);
        sprintf(out, "pushb32\npush32 %d\nadd32\npopb32", t.num);
        return out;
    }
    case LEFT:
    {
        char *out = malloc(100);
        sprintf(out, "push32 %d\npushb32\nsub32\npopb32", t.num);
        return out;
    }
    default:
    {
        char *out = malloc(1);
        out[0] = 0;
        return out;
    }
    }
}

char *compile_all(tok *tokens, int len)
{
    char *stack[1000];
    int pos = 0;
    char *out = malloc(1);
    int sz = 10;
    for (int i = 0; i < len; ++i)
    {
        switch (tokens[i].type)
        {
        case LOOP:
        {
            char *label = malloc(100);
            sprintf(label, "loop%d%d", pos, i);
            stack[pos] = label;
            ++pos;
            out = realloc(out, sz + strlen(label) + 2);
            sprintf(out, "%s\n%s:\n", out, label);
            sz += strlen(label) + 2;
        }
        break;
        case LOOPEND:
        {
            char *o = malloc(100);
            --pos;
            char *l = stack[pos];
            sprintf(o, "pushb32\nload32\npush32 0\ncmp32\nmk4\njne %s", l);

            out = realloc(out, sz + strlen(o) + 2);
            sprintf(out, "%s\n%s\n", out, o);
            sz += strlen(o) + 2;
        }
        break;
        default:
        {
            char *o = compile(tokens[i]);
            out = realloc(out, sz + strlen(o) + 2);
            sprintf(out, "%s\n%s\n", out, o);
            sz += strlen(o) + 2;
        }
        }
    }
    return out;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Provide file.\n");
        return 0;
    }

    char *fp = argv[1];
    FILE *f = fopen(fp, "rb");
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *file = malloc(len + 1);
    fread(file, len, 1, f);
    len = 0;
    tok *tokens = lex_all(file, &len);
    int i = 0;

    while (i < len)
    {
        if (tokens[i].type == END)
        {
            break;
        }
        ++i;
    }
    char *init = "push32 500\npopb32\n";
    char *comp = compile_all(tokens, len);
    int length = strlen(comp) + strlen(init);
    char *out = malloc(length + 1);
    sprintf(out, "%s%s", init, comp);
    printf("%s\n", out);
}
