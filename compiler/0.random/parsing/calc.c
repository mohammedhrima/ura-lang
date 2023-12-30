#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Token Token;
typedef struct Node Node;
typedef enum Type Type;

enum Type
{
    add_ = '+',
    sub_ = '-',
    mul_ = '*',
    div_ = '/',
    num_ = 'n',
    eof_ = 'e',
    lparent_ = '(',
    rparent_ = ')'
};

struct Token
{
    Type type;
    int val;
};

struct Node
{
    Node *left, *right;
    Token *token;
};

Token *tokens[100];
int tk_pos;
char *text;

void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
};

Token *check(Type type)
{
    if (tokens[tk_pos]->type != type)
        return NULL;
    tk_pos++;
    return tokens[tk_pos - 1];
}

Token *expect(Type type)
{
    if (tokens[tk_pos]->type != type)
        error("expected '%c'", type);
    tk_pos++;
    return tokens[tk_pos - 1];
}

Token *new_token(Type type, int s, int e)
{
    Token *token = calloc(1, sizeof(Token));
    token->type = type;
    printf("new token: %c\n", token->type);
    if (type == num_)
    {
        while (s < e)
            token->val = token->val * 10 + (text[s++] - '0');
    }
    tokens[tk_pos++] = token;
    return token;
}

void build_tokens()
{
    int s = 0, e = 0;
    while (text[e])
    {
        if (isspace(text[e]))
        {
            e++;
            continue;
        }
        if (strchr("/*-+()", text[e]))
        {
            new_token(text[e++], 0, 0);
            continue;
        }
        if (isdigit(text[e]))
        {
            s = e;
            while (isdigit(text[e]))
                e++;
            new_token(num_, s, e);
            continue;
        }
        error("invalid token");
    }
    new_token(eof_, 0, 0);
}

Node *new_node(Token *token, Node *left, Node *right)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    new->left = left;
    new->right = right;
    return new;
}

Node *expr();
Node *mul();
Node *prime();

Node *expr()
{
    Node *node = mul();
    Token *token = NULL;
    while ((token = check('+')) || (token = check('-')))
        node = new_node(token, node, mul());
    return node;
}

Node *mul()
{
    Node *node = prime();
    Token *token = NULL;
    while ((token = check('*')) || (token = check('/')))
        node = new_node(token, node, prime());
    return node;
}

Node *prime()
{
    if (check('('))
    {
        Node *node = expr();
        expect(')');
        return node;
    }
    return new_node(expect(num_), NULL, NULL);
}

void Generate(Node *node)
{
    if (node->token->type == num_)
        printf("  push %d\n", node->token->val);
    else
    {
        Generate(node->left);
        Generate(node->right);

        printf("  pop rdi\n");
        printf("  pop rax\n");

        switch (node->token->type)
        {
        case add_:
            printf("  add rax, rdi\n");
            break;
        case sub_:
            printf("  sub rax, rdi\n");
            break;
        case mul_:
            printf("  imul rax, rdi\n");
            break;
        case div_:
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        }
        printf("  push rax\n");
    }
    free(node->token);
    free(node);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        error("%s: invalid number of arguments", argv[0]);
        return 1;
    }
    text = argv[1];
    build_tokens();
    tk_pos = 0;
    printf("\n");

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    Node *node = expr();
    Generate(node);
    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}