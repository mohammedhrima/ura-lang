#include "header.h"

sType *dataTypes = (sType[]){
    {"int", int_},
    {0, 0},
};

sType *symbTypes = (sType[]){
    {"==", equal_},
    {"=", assign_},
    {"+", add_},
    {"-", sub_},
    {"*", mul_},
    {"/", div_},
    {0, 0},
};

sType *randTypes = (sType[]){
    {"name", name_},
    {"END", end_},
    {0, 0},
};

bool check_list(Token *new, sType *list)
{
    for (int i = 0; list[i].value; i++)
    {
        if (strncmp(new->name, list[i].value, strlen(list[i].value)) == 0)
        {
            free(new->name);
            memset(new, 0, sizeof(Token));
            new->type = list[i].type;
            return true;
        }
    }
    return false;
}

void new_token(char *input, int s, int e, Type type)
{
    if (tk_pos + 1 > tk_len)
    {
        Token **tmp = calloc(2 * tk_len, sizeof(Token *));
        memcpy(tmp, tokens, tk_pos * sizeof(Token *));
        free(tokens);
        tokens = tmp;
        tk_len *= 2;
    }
    Token *new = calloc(1, sizeof(Token));
    new->type = type;

    switch (type)
    {
    case name_:
        new->name = calloc(e - s + 1, sizeof(char));
        strncpy(new->name, input + s, e - s);
        if (check_list(new, dataTypes))
        {
            new->declare = true;
            break;
        }
        break;
    case add_:
    case sub_:
    case mul_:
    case div_:
        break;
    case assign_:
        break;
    case int_:
        while (s < e)
            new->_int.value = 10 * new->_int.value + (input[s++] - '0');
        break;
    default:
        break;
    }

    GLOG("new", " ");
    print_token(new, true);
    tokens[tk_pos++] = new;
}

int tokenize(char *input)
{
    printf("\n%sTOKENIZE%s\n", SPLIT, SPLIT);
    size_t e = 0;
    int line = 1;
    int col = 0;
    while (input[e])
    {
        bool found = false;
        size_t s = e;
        if (input[e] == '\n')
        {
            e++;
            col = 0;
            line++;
            continue;
        }
        if (input[e] == ' ')
        {
            e++;
            col++;
            continue;
        }
        if (input[e] == '#')
        {
            e++;
            while (input[e] && input[e] != '#')
                e++;
            if (input[e] != '#')
            {
                RLOG("error", "tokenize: expected '#'\n");
                error = true;
                return -1;
            }
            e++;
            continue;
        }
        if (isdigit(input[e]))
        {
            while (isdigit(input[e]))
                e++;
            new_token(input, s, e, int_);
            continue;
        }
        for (int i = 0; symbTypes[i].value; i++)
        {
            if (strncmp(input + e, symbTypes[i].value, strlen(symbTypes[i].value)) == 0)
            {
                found = true;
                new_token(input, 0, 0, symbTypes[i].type);
                e += strlen(symbTypes[i].value);
                break;
            }
        }
        if (found)
            continue;
        if (isalpha(input[e]))
        {
            while (isalnum(input[e]))
                e++;
            new_token(input, s, e, name_);
            continue;
        }
        RLOG("error", "tokenize: unexpedted %s\n", input + e);
        error = true;
        break;
    }
    new_token(input, 0, 0, end_);
    return 0;
}

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    if (token)
    {
        GLOG("new node", "has ");
        print_token(token, true);
    }
    return new;
}

Token *check(Type *arr)
{
    for (int i = 0; arr[i] && !error; i++)
        if (arr[i] == tokens[exe_pos]->type)
            return tokens[exe_pos++];
    return NULL;
}

Token *expect(Type type)
{
    if (tokens[exe_pos]->type == type)
        return tokens[exe_pos++];
    return NULL;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = add_sub();
    Token *token;

    while ((token = check((Type[]){assign_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
        left = node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    Token *token;
#if 0
    if ((token = check((Type[]){add_, sub_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
        left = node;
    }
#else
    while ((token = check((Type[]){add_, sub_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
#endif
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    Token *token;

    while ((token = check((Type[]){mul_, div_, 0})))
    {
        Node *node = new_node(token);
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
    if ((token = check((Type[]){int_, 0})))
    {
        if (token->declare)
        {
            Token *next;
            if ((next = expect(name_)) == NULL)
            {
                error = true;
                RLOG("error", "expected identifier after %s word\n", to_string(token->type));
                return NULL;
            }
            token->name = strdup(next->name);
        }
        return new_node(token);
    }
    else if ((token = check((Type[]){name_, 0})))
        return new_node(token);
    else if (check((Type[]){'(', 0}))
    {
        node = expr();
        if (tokens[exe_pos]->type != ')')
        {
            error = true;
            printf("Expected )\n");
        }
        exe_pos++;
    }
    else
    {
        error = true;
        printf("Unexpected: ");
        print_token(tokens[exe_pos], true);
    }
    return node;
}