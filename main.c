#include "utils.c"

FILE *fp;
size_t size;
char *text;
Token *tokens[100];
int tk_pos;
int txt_pos;
uintptr_t ptr;
int fd;

Token *VARIABLES[100];
int var_pos;

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
    case char_val_:
    case num_var_:
        free(token->character);
        break;
    default:
        break;
    }
    free(token);
}

Token *new_token(int s, int e, Type type)
{
    char *value = NULL;
    tokens[tk_pos] = calloc(1, sizeof(Token));
    tokens[tk_pos]->type = type;
    printf("new token has type: %s", type_to_string(type));
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
    case char_val_:
        tokens[tk_pos]->character = calloc(e - s + 1, sizeof(char));
        strncpy(tokens[tk_pos]->character, text + s, e - s);
        printf(", value: %s", tokens[tk_pos]->character);
        break;
    case num_val_:
        while (s < e)
        {
            tokens[tk_pos]->number = 10 * tokens[tk_pos]->number + text[s] - '0';
            s++;
        }
        printf(", value: %ld", tokens[tk_pos]->number);
        break;
    default:
        break;
    }
    printf("\n");
    return tokens[tk_pos++];
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

    txt_pos += strlen("main\n");
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
        printf("found %s\n", type_to_string(node->token->type));
        printf("    left : %s, %s\n", type_to_string(node->left->token->type), node->left->token->character);
        printf("    right: %s, %ld\n", type_to_string(node->right->token->type), node->right->token->number);
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
    printf("call prime\n");
    switch (tokens[tk_pos]->type)
    {
    case char_var_:
    case num_var_:
    {
        printf("found %s\n", type_to_string(tokens[tk_pos]->type));
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
        printf("found %s\n", type_to_string(tokens[tk_pos]->type));
        return new_node(tokens[tk_pos++]);
    }
    default:
        error("in prime\n");
        break;
    }
    return NULL;
}

Node *evaluate(Node *node)
{
    switch (node->token->type)
    {
    case void_var_:
        return node;
    case assign_:
        /*
            left variable name
            right is the value
        */
        printf("assign %s with %ld\n", node->left->token->character, node->right->token->number);
        if (twine(node->left->token->type) != node->right->token->type)
            error("Error in assignement\n");
        switch (node->left->token->type)
        {
        case num_var_:
            printf("%ld", node->right->token->number);
            ptr += 4;
            node->left->token->addr = ptr;
            // TODO : check initilizing and modifying value
            VARIABLES[var_pos++] = node->token;
            dprintf(fd, "    movl $%ld, -%zu(%%rbp)\n", node->right->token->number, ptr);
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
        printf("found function call has name '%s'\n", node->left->token->character);
        if (strncmp("output", node->token->character, strlen("output")) == 0)
        {
            printf("found output\n");
            dprintf(fd, "   ");
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
    fd = open("file.s", O_CREAT | O_TRUNC | O_RDWR);
    fp = fopen("file.hr", "r");
    if (fp == NULL || fd < 0)
        error("Opening file");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (!(text = calloc(size + 1, sizeof(char))))
        error("Allocation");
    fseek(fp, 0, SEEK_SET);
    fread(text, size, sizeof(char), fp);
    fclose(fp);
    printf("%s\n\n", text);
    build_tokens();
    free(text);
    tk_pos = 0;

    // write the assembly
    dprintf(fd, ".text\n.globl main\nmain:\n");
    dprintf(fd, "    push  %%rbp\n");
    dprintf(fd, "    movq   %%rsp, %%rbp\n");

    Node *curr = expr();
    while (curr->token->type != eof_)
    {
        // printf("===> loop\n");
        evaluate(curr);
        free_node(curr);
        curr = expr();
    }
    dprintf(fd, "    pop  %%rbp\n");
    dprintf(fd, "    ret\n");
    free_node(curr);
    close(fd);
}