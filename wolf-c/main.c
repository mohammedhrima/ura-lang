#include "header.h"

char *input;
Token **tokens;
size_t pos0;
size_t pos1;
size_t len;
bool error;
FILE *asm_fd;

void new_token(int s, int e, Type type, size_t col)
{
    if (pos0 + 1 > len)
    {
        tokens = realloc(tokens, 2 * len * sizeof(Token *));
        len *= 2;
    }
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    switch (type)
    {
    case add_:
    case sub_:
    case mul_:
    case div_:
    case lpar_:
    case rpar_:
        break;
    case int_:
        while (s < e)
            new->_int.value = 10 * new->_int.value + (input[s++] - '0');
        break;

    case float_:
        break;

    default:
        break;
    }
    tokens[pos0++] = new;
}

int tokenize()
{
    size_t i = 0;
    size_t col = 0;
    size_t line = 1;
    while (input[i])
    {
        size_t s = i;
        if (input[i] == '\n')
        {
            i++;
            col = 0;
            line++;
            continue;
        }
        if (input[i] == ' ')
        {
            i++;
            col++;
            continue;
        }
        if (isdigit(input[i]))
        {
            while (isdigit(input[i]))
                i++;
            new_token(s, i, int_, col);
            continue;
        }
        if (input[i] && strchr("/*-+/()", input[i]))
        {
            new_token(s, i, input[i], col);
            i++;
            continue;
        }
        if (input[i] == '"')
        {
            s++;
            while (input[i] && input[i] != '"')
                i++;
            if (input[i] != '"')
            {
                printf("line: %zu, column: %zu, Expected '\"'\n", line, col);
                return -1;
            }
            i++;
            continue;
        }
    }
    new_token(0, 0, end_, 0);
    return 0;
}

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    if (token)
    {
        printf("New node has ");
        print_token(token);
    }
    return new;
}

Token *check(Type *arr)
{
    for (int i = 0; arr[i] && !error; i++)
        if (arr[i] == tokens[pos1]->type)
            return tokens[pos1++];
    return NULL;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = add_sub();
    Node *token;
}

Node *add_sub()
{
    Node *left = mul_div();
    Node *node;
    Token *token;
    while ((token = check((Type[]){add_, sub_, 0})))
    {
        node = new_node(token);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    Node *node;
    Token *token;
    while ((token = check((Type[]){mul_, div_, 0})))
    {
        node = new_node(token);
        node->left = left;
        node->right = prime();
        left = node;
    }
    return left;
}

Node *prime()
{
    Node *node = NULL;
    Token *token;
    if ((token = check((Type[]){int_, float_, char_, string_, bool_, name_, 0})))
        return new_node(token);
    else if (check((Type[]){'(', 0}))
    {
        node = expr();
        if (tokens[pos1]->type != ')')
        {
            error = true;
            printf("Expected )\n");
        }
        pos1++;
    }
    else
    {
        error = true;
        printf("Unexpected: ");
        print_token(tokens[pos1]);
    }
    return node;
}

void init_asm()
{
    pasm(".intel_syntax noprefix\n");
    // pasm(".include \"/wolf-c/import/header.s\"\n\n");
    pasm(".text\n");
    pasm(".globl	main\n");
}

long long evaluate(Node *node)
{
    switch (node->token->type)
    {
    case int_:
        return node->token->_int.value;
    case add_:
    {
        long long left = evaluate(node->left);
        long long right = evaluate(node->right);
        return left + right;
    }
    case sub_:
    {
        long long left = evaluate(node->left);
        long long right = evaluate(node->right);
        return left - right;
    }
    case div_:
    {
        long long left = evaluate(node->left);
        long long right = evaluate(node->right);
        return left / right;
    }
    case mul_:
    {
        long long left = evaluate(node->left);
        long long right = evaluate(node->right);
        return left * right;
    }
    default:
        break;
    }
    return 0;
}

void compile()
{
    pos0 = 0;
    pos1 = 0;
    len = 10;
    Node *head = new_node(NULL);
    Node *curr = head;
    tokens = calloc(len, sizeof(Token *));
    if (tokenize() == 0)
    {
        long long res = 0;
        for (size_t i = 0; i < pos0; i++)
            print_token(tokens[i]);
        while (tokens[pos1]->type != end_ && !error)
        {
            curr->left = expr();
            curr->right = new_node(NULL);
            curr = curr->right;
        }
        init_asm();
        curr = head;
        while (curr)
        {
            print_node(curr->left, NULL, 0);
            curr = curr->right;
        }
        free_node(head);
        printf("result: %ld\n", res);
    }
    free_tokens();
}

int main(int argc, char **argv)
{
    asm_fd = fopen("file.s", "w");
    FILE *file = fopen("file.w", "r");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    input = calloc(size + 1, sizeof(char));
    fread(input, size, sizeof(char), file);
    fclose(file);
    compile();
    fclose(asm_fd);
}