#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define SPLIT "==============================================\n"

// TOKENIZER
typedef struct Token
{
    char type;
    int value;
} Token;

Token *tokens[100];
int tk_pos = 0;
int exe_pos = 0;

void ptoken(Token *token)
{
    printf("token has type [%c] ", token->type);
    if(token->type == 'd')
        printf("value: %d", token->value);
    printf("\n");
}

void new_token(char *input, int s, int e, char type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    if(new->type == 'd')
    {
        while(s < e)
            new->value = 10 * new->value + (input[s++] - '0');
    }
    printf("new ");
    ptoken(new);
    tokens[tk_pos++] = new;
}

void tokenize(char *input)
{
    int i = 0;
    while(input[i])
    {
        int s = i;
        // check if integer
        if(isspace(input[i]))
        {
            while(isspace(input[i]))
                i++;
            continue;
        }
        if(isdigit(input[i]))
        {
            while(isdigit(input[i]))
                i++;
            new_token(input, s, i, 'd');
            continue;
        }
        // check if it's + -
        if(strchr("+-*/()", input[i]))
        {
            new_token(input, 0, 0, input[i]);
            i++;
            continue;
        }
        i++;
    }
    new_token(NULL, 0, 0, 'e');
}

void free_tokens()
{
    for(int i = 0; i < tk_pos; i++)
        free(tokens[i]);
}

// ABSTRACT SYNTAX TREE
typedef struct Node 
{
    Token *token;
    struct Node *left;
    struct Node *right;
} Node;

Node *new_node(Token *token)
{
    Node *node = calloc(1, sizeof(Node));
    printf("new node: has ");
    ptoken(token);
    node->token = token;
    return node;
}

void free_node(Node *node)
{
    if(node)
    {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

void pnode(Node *node, char *side, int space)
{
    if(node)
    {
        int i = 0;
        while(i < space)
            i += printf(" ");
        if(side)
            printf("%s", side);
        printf("node: ");
        ptoken(node->token);
        pnode(node->left , "LEFT : ", space + 4);
        pnode(node->right, "RIGHT: ", space + 4);
    }
}

Node *expr();
Node *add_sub();
Node *mul_div();
Node *prime();

Node *expr()
{
    return add_sub();
}

Node *add_sub()
{
    Node *left = mul_div();
    while(strchr("+-", tokens[exe_pos]->type))
    {
        Node *node = new_node(tokens[exe_pos++]);
        node->left = left;
        node->right = mul_div();
        // return node;
        left = node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    while(strchr("*/", tokens[exe_pos]->type))
    {
        Node *node = new_node(tokens[exe_pos++]);
        node->left = left;
        node->right = prime();
        // return node;
        left = node;
    }
    return left;
}

Node *prime()
{
    if(tokens[exe_pos]->type == 'd')
        return new_node(tokens[exe_pos++]);
    else if(tokens[exe_pos]->type == '(')
    {
        exe_pos++;
        Node *node = expr();
        if(tokens[exe_pos]->type != ')')
        {
            printf("Errooora: khasse lright qaws\n");
            exit(1);
        }
        exe_pos++;
        return node;
    }
    return NULL;
}

// Evaluation
int eval(Node *node)
{
    char type = node->token->type;
    switch(type)
    {
        case 'd':
            return node->token->value;
        case '+':
        {
            int left = eval(node->left);
            int right = eval(node->right);
            return (left + right);
        }
        case '-':
        {
            int left = eval(node->left);
            int right = eval(node->right);
            return (left - right);
        }
        case '*':
        {
            int left = eval(node->left);
            int right = eval(node->right);
            return (left * right);
        }
        case '/':
        {
            int left = eval(node->left);
            int right = eval(node->right);
            return (left / right);
        }
    }
    return 0;
}

int main()
{
    char *text="3 * (2 + 7)";
    tokenize(text); // build tokens
    printf(SPLIT);
#if 1
    Node *curr = expr();
    printf(SPLIT);
    pnode(curr, NULL, 0);
    printf(SPLIT);
    int res = eval(curr);
    printf("result: %d\n", res);
    printf(SPLIT);
    // clearing shit
    free_node(curr);
#endif
    free_tokens();
}