#include "debug.c"

FILE *fp;
size_t size;
char *text;
int txt_pos;
uintptr_t ptr;
int asm_fd;

Type types[255] = {
    ['+'] = add_,
    ['-'] = sub_,
    ['*'] = mul_,
    ['/'] = div_,
    ['('] = lparent_,
    [')'] = rparent_,
    ['='] = assign_,
};

struct
{
    char *string;
    Type type;
} DataTypes[] = {
    {"num ", num_var_},
    {"char ", char_var_},
};

void free_token(Token *token)
{
    printf("free token has type %s\n", type_to_string(token->type));
    switch (token->type)
    {
    case void_var_:
    case num_var_:
        free(token->name);
        break;
    case char_val_:
        free(token->character);
        break;
    case char_var_:
        free(token->name);
        free(token->character);
        break;
    default:
        break;
    }
    free(token);
}

Token *new_token(int s, int e, Type type)
{
    if (tk_pos + 10 > tk_len)
    {
        Token **tmp = calloc(tk_len * 2, sizeof(Token *));
        memcpy(tmp, tokens, tk_len * sizeof(Token *));
        free(tokens);
        tokens = tmp;
        tk_len *= 2;
    }
    char *value = NULL;
    tokens[tk_pos] = calloc(1, sizeof(Token));
    tokens[tk_pos]->type = type;
    switch (type)
    {
    case eof_:
    case add_:
    case sub_:
    case mul_:
    case div_:
    case lparent_:
    case rparent_:
    case assign_:
        break;
    case void_var_:
        tokens[tk_pos]->name = calloc(e - s + 1, sizeof(char));
        strncpy(tokens[tk_pos]->name, text + s, e - s);
        break;
    case char_val_:
        tokens[tk_pos]->character = calloc(e - s + 1, sizeof(char));
        strncpy(tokens[tk_pos]->character, text + s, e - s);
        break;
    case num_val_:
        while (s < e)
            tokens[tk_pos]->number = 10 * tokens[tk_pos]->number + text[s++] - '0';
        break;
    default:
        break;
    }
    debug("new token %k\n", tokens[tk_pos]);
    return tokens[tk_pos++];
}

Token *new_variable(Token *token)
{
    if (var_pos + 10 > var_len)
    {
        Token **tmp = calloc(var_len * 2, sizeof(Token *));
        memcpy(tmp, variables, var_len * sizeof(Token *));
        free(variables);
        variables = tmp;
        var_len *= 2;
    }
    return (variables[var_pos++] = token);
}

// build tokens
void build_tokens()
{
    Token *token;
    int start = 0;
    while (isspace(text[txt_pos]))
        txt_pos++;
    if (strncmp(&text[txt_pos], "main:\n", strlen("main:\n")))
        error("main label with new line is required\n");

    txt_pos += strlen("main:\n");
    while (text[txt_pos])
    {
        token = NULL;
        start = txt_pos;
        if (isspace(text[txt_pos]))
        {
            txt_pos++;
            continue;
        }
        for (int i = 0; i < sizeof(DataTypes) / sizeof(*DataTypes); i++)
        {
            if (strncmp(DataTypes[i].string, text + txt_pos, strlen(DataTypes[i].string)) == 0)
            {
                token = new_token(0, 0, DataTypes[i].type);
                txt_pos += strlen(DataTypes[i].string);
            }
        }
        if (token && token->type)
        {
            token = NULL;
            continue;
        }
        if (strchr("=+/*-", text[txt_pos]))
        {
            new_token(0, 0, types[text[txt_pos++]]);
            continue;
        }
        while (isalpha(text[txt_pos]))
            txt_pos++;
        if (txt_pos > start)
        {
            new_token(start, txt_pos, void_var_);
            continue;
        }
        while (isdigit(text[txt_pos]))
            txt_pos++;
        if (txt_pos > start)
        {
            new_token(start, txt_pos, num_val_);
            continue;
        }
    }
    new_token(0, 0, eof_);
}

// build tree
void free_node(Node *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        free_token(node->token);
        free(node);
    }
}
Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    return new;
}

Node *expr();
Node *assign();
Node *parents();
Node *prime();

void skip(Type type)
{
    if (tokens[tk_pos]->type != type)
        error("in skip");
    tk_pos++;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = prime();
    if (tokens[tk_pos] && tokens[tk_pos]->type == assign_)
    {
        Node *node = new_node(tokens[tk_pos++]);
        node->left = left;
        node->right = prime();
        // printf("found %s\n", type_to_string(node->token->type));
        // printf("    left : %s, %s\n", type_to_string(node->left->token->type), node->left->token->name);
        // printf("    right: %s, %ld\n", type_to_string(node->right->token->type), node->right->token->number);
        left = node;
    }
    return left;
}

Node *parents()
{
    Node *left = prime();
    if (tokens[tk_pos] && tokens[tk_pos]->type == lparent_)
    {
        left->token->type = func_call_;
        skip(lparent_);
        left->left = prime();
        skip(rparent_);
    }
    return left;
}

Node *prime()
{
    // printf("call prime\n");
    switch (tokens[tk_pos]->type)
    {
    case char_var_:
    case num_var_:
    {
        // TODO: check redefined variable
        // printf("found %s\n", type_to_string(tokens[tk_pos]->type));
        tk_pos++;
        if (tokens[tk_pos]->type != void_var_)
            error("Expected variable name");
        tokens[tk_pos]->type = tokens[tk_pos - 1]->type;
        free_token(tokens[tk_pos - 1]);
        return new_node(tokens[tk_pos++]);
    }
    case eof_:
    case char_val_:
    case num_val_:
    case void_var_:
    {
        // printf("found %s\n", type_to_string(tokens[tk_pos]->type));
        return new_node(tokens[tk_pos++]);
    }
    default:
        error("in prime\n");
        break;
    }
    return NULL;
}

Token *get_var(char *name)
{
    // printf("there is %d variables: ", var_pos);
    // for(int i = 0; i < var_pos; i++)
    //     printf("%s ", VARIABLES[i]->name);
    // printf("\n");
    // printf("%s\n", VARIABLES[0]->name);
    if (name == NULL)
        error("in get_var");
    for (int i = 0; i < var_pos; i++)
    {
        // printf("get_var %d\n", i);
        if (
            variables[i]->name &&
            strcmp(variables[i]->name, name) == 0)
        {
            // printf("find variable %s\n", name);
            return variables[i];
        }
    }
    return NULL;
}

Token *evaluate(Node *node)
{
    Token *to_find = NULL;
    Token *left = NULL;
    Token *right = NULL;
    switch (node->token->type)
    {
    case char_val_:
    case num_val_:
    case void_var_:
    case char_var_:
    case num_var_:
        return node->token;
    case assign_:
        /*
            - always left should be a variable
            - check if variable is does exists
        */
        // TODO: split assignement and initializing
        printf("assign %s with %ld\n", node->left->token->name, node->right->token->number);
        // check the compatibility
        left = evaluate(node->left);
        right = evaluate(node->right);

        if (left->type != void_var_ && twine(left->type) != right->type)
            error("in assignement (incompatible type)\n");
        to_find = get_var(left->name);
        if (!to_find && left->type != void_var_)
        {
            ptr += 4;
            node->left->token->addr = ptr;
            printf("new variable has name %s and value %ld\n", node->left->token->name, node->right->token->number);
            new_variable(node->left->token);
            to_find = node->left->token;
        }
        else if (!to_find)
            error("Undeclared variable");
#if 0
        for (int i = 0; i < var_pos; i++)
            printf("%s\n", VARIABLES[i]->name);
            // exit(0);
#endif
        switch (to_find->type)
        {
        case num_var_:
            // printf("%ld", node->right->token->number);
            dprintf(asm_fd, "    movl   $%ld, -%zu(%%rbp)\n", node->right->token->number, to_find->addr);
            break;
        case char_var_:
            printf("%s", node->right->token->character);
            break;
        default:
            break;
        }
        break;
    case func_call_:
    {
        printf("found function call has name '%s'\n", node->left->token->name);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            printf("found output\n");
            dprintf(asm_fd, "   ");
        }
        break;
    }
    default:
        break;
    }
    printf("\n");
    return NULL;
}

int main(void)
{
    // opening file
#if 1
    asm_fd = open("file.s", O_CREAT | O_TRUNC | O_RDWR, 0777);
#else
    asm_fd = 1;
#endif
    fp = fopen("file.hr", "r");
    if (fp == NULL || asm_fd < 0)
        error("Opening file");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (!(text = calloc(size + 1, sizeof(char))))
        error("Allocation");
    fseek(fp, 0, SEEK_SET);
    fread(text, size, sizeof(char), fp);
    fclose(fp);
    printf("%s\n\n", text);

    tk_len = var_len = 100;
    tokens = calloc(tk_len, sizeof(Token *));
    variables = calloc(tk_len, sizeof(Token *));
    build_tokens();
    free(text);
    tk_pos = 0;

    // write the assembly
    dprintf(asm_fd, ".globl main\n");
    dprintf(asm_fd, "main:\n");
    dprintf(asm_fd, "    push   %%rbp\n");
    dprintf(asm_fd, "    mov    %%rsp, %%rbp\n");

    Node *main = new_node(NULL);
    Node *curr = main;
    while (tokens[tk_pos]->type != eof_)
    {
        // printf("===> loop\n");
        curr->left = expr();
        evaluate(curr->left);
        curr->right = new_node(NULL);
        curr = curr->right;
        // free_node(curr);
    }
    while (main)
    {
        Node *tmp = main->right;
        free_node(main->left);
        free(main);
        main = tmp;
    }
    dprintf(asm_fd, "    mov    $0, %%rax\n");
    dprintf(asm_fd, "    pop    %%rbp\n");
    dprintf(asm_fd, "    ret\n");
    // free_node(curr);
    close(asm_fd);
}