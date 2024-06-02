#include "header.h"

char *input;
Token **tokens;
size_t pos0;
size_t pos1;
size_t len;
bool error;
FILE *asm_fd;
int col;
size_t line;
size_t ptr;
size_t bloc_index;
size_t var_index;

sType *dataTypes = (sType[]){
    {"int", int_},
    {"float", float_},
    {"char", char_},
    {"string", string_},
    {"array", array_},
    {"bool", bool_},
    {0, 0},
};

sType *blocTypes = (sType[]){
    {"for", for_},
    {"in", in_},
    {"if", if_},
    {"else", else_},
    {"while", while_},
    {0, 0},
};

sType *symbTypes = (sType[]){
    {"==", equal_},
    {"=", assign_},
    {"+", add_},
    {"-", sub_},
    {"*", mul_},
    {"/", div_},
    {"(", lpar_},
    {")", rpar_},
    {"<", less_},
    {">", more_},
    {",", semi_},
    {":", dots_},
    {0, 0},
};

sType *randTypes = (sType[]){
    {"name", name_},
    {"END", end_},
    {0, 0},
};

bool is_bloc(Type type)
{
    return type == if_ || type == else_ || type == for_ || while_;
}

bool check_list(Token *new, sType *list)
{
    for (int i = 0; list[i].value; i++)
    {
        if (strncmp(new->name, list[i].value, strlen(list[i].value)) == 0)
        {
            free(new->name);
            memset(new, 0, sizeof(Token));
            new->declaration = true;
            new->type = list[i].type;
            new->space = col;
            if (is_bloc(new->type))
                new->index = ++bloc_index;
            return true;
        }
    }
    return false;
}

Token **variables;
size_t vpos;
size_t vlen;
Token *new_variable(Token *token)
{
    for (size_t i = 0; i < vpos; i++)
    {
        if (strcmp(token->name, variables[i]->name) == 0)
        {
            RLOG("error", "redefinition of %s\n", token->name);
            error = true;
            return NULL;
        }
    }
    if (vpos + 1 > vlen)
    {
        variables = realloc(variables, 2 * vlen * sizeof(Token *));
        vlen *= 2;
    }
    token->declaration = false;
    token->index = ++var_index;
    variables[vpos++] = token;
    return token;
}

Token *get_var(Token *token)
{
    for (size_t i = 0; i < vpos; i++)
    {
        if (strcmp(token->name, variables[i]->name) == 0)
            return variables[i];
    }
    error = true;
    RLOG("error", "%s not found\n", token->name);
    return token;
}

void new_token(int s, int e, Type type, bool declaration)
{
    if (pos0 + 1 > len)
    {
        tokens = realloc(tokens, 2 * len * sizeof(Token *));
        len *= 2;
    }
    Token *new = calloc(1, sizeof(Token));
    new->space = col;
    new->type = type;
    new->declaration = declaration;
    if (!declaration)
        switch (type)
        {
        case name_:
            new->name = calloc(e - s + 1, sizeof(char));
            strncpy(new->name, input + s, e - s);
            if (check_list(new, dataTypes))
                break;
            if (check_list(new, blocTypes))
                break;
            break;
        case add_:
        case sub_:
        case mul_:
        case div_:
            new->index = ++var_index;
            break;
        case assign_:
            break;
        case int_:
            new->index = ++var_index;
            while (s < e)
                new->_int.value = 10 * new->_int.value + (input[s++] - '0');
            break;
        case float_:
            break;

        default:
            break;
        }
    else
        new->index = ++var_index;

    GLOG("new", " ");
    print_token(new, true);
    tokens[pos0++] = new;
}

int tokenize()
{
    GLOG("tokenize", "\n");
    size_t e = 0;
    line = 1;
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
            new_token(s, e, int_, false);
            continue;
        }
        if (input[e] == '"')
        {
            s++;
            e++;
            while (input[e] && input[e] != '"')
                e++;
            if (input[e] != '"')
            {
                RLOG("error", "line: %zu, Expected '\"'\n", line);
                error = true;
                return -1;
            }
            new_token(s, e, string_, false);
            e++;
            continue;
        }
        for (int i = 0; symbTypes[i].value; i++)
        {
            if (strncmp(input + e, symbTypes[i].value, strlen(symbTypes[i].value)) == 0)
            {
                found = true;
                new_token(0, 0, symbTypes[i].type, false);
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
            new_token(s, e, name_, false);
            continue;
        }
        RLOG("error", "tokenize: unexpedted %s\n", input + e);
        error = true;
        break;
    }
    new_token(0, 0, end_, false);
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
        if (arr[i] == tokens[pos1]->type)
            return tokens[pos1++];
    return NULL;
}

Token *expect(Type type)
{
    if (tokens[pos1]->type == type)
        return tokens[pos1++];
    return NULL;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = relation();
    Token *token;

    while ((token = check((Type[]){assign_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = relation();
        left = node;
    }
    return left;
}

Node *relation()
{
    Node *left = add_sub();
    Token *token;

    while ((token = check((Type[]){less_, more_, equal_, 0})))
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

    while ((token = check((Type[]){add_, sub_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = bloc();
    Token *token;

    while ((token = check((Type[]){mul_, div_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = bloc();
        left = node;
    }
    return left;
}

Node *bloc()
{
    Token *token;
#if 0
    if ((token = check((Type[]){for_, 0})))
    {
        Node *node = new_node(token);
        node->left = new_node(NULL);

        Node *curr = node->left;
        if ((token = expect(name_)) == NULL)
        {
            error = true;
            RLOG("error", "expecting name after for");
            return NULL;
        }
        curr->left = new_node(token);

        if ((token = expect(in_)) == NULL)
        {
            error = true;
            RLOG("error", "expecting in keyword");
            return NULL;
        }
        curr->right = new_node(token);
        curr = curr->right;
        if ((token = expect(name_)) == NULL)
        {
            error = true;
            RLOG("error", "expecting identifier to for loop throw it");
            return NULL;
        }
        curr->left = new_node(token);

        expect(dots_);

        node->right = new_node(NULL);
        curr = node->right;
        int space = node->token->space;
        while (tokens[pos1]->space > space && tokens[pos1]->type != end_)
        {
            curr->left = expr();
            if (tokens[pos1]->space > space && tokens[pos1]->type != end_)
            {
                curr->right = new_node(NULL);
                curr = curr->right;
            }
        }
        return node;
    }
#endif
    if ((token = check((Type[]){if_, 0})))
    {
        /*
        if:
            left :
                left : conditon
                right: bloc

            right:
                left : condition
                right : bloc
        */
        Node *node = new_node(token);
        int space = node->token->space;

        node->left = new_node(NULL);
        Node *curr = node->left;

        curr->left = expr();
        expect(dots_);

        curr->right = new_node(NULL);
        curr = curr->right;
        while (tokens[pos1]->space > space && tokens[pos1]->type != end_)
        {
            curr->left = expr();
            if (tokens[pos1]->space > space && tokens[pos1]->type != end_)
            {
                curr->right = new_node(NULL);
                curr = curr->right;
            }
        }
        if ((token = check((Type[]){else_, 0})))
        {
            node->right = new_node(token);
            curr = node->right;
            while (tokens[pos1]->space > space && tokens[pos1]->type != end_)
            {
                curr->left = expr();
                if (tokens[pos1]->space > space && tokens[pos1]->type != end_)
                {
                    curr->right = new_node(NULL);
                    curr = curr->right;
                }
            }
        }
        return node;
    }
    if ((token = check((Type[]){while_, 0})))
    {
        /*
        while:
            left : condition
            right: bloc
        */
        Node *node = new_node(token);
        int space = node->token->space;
        node->left = expr();
        expect(dots_);

        node->right = new_node(NULL);
        Node *curr = node->right;
        while (tokens[pos1]->space > space && tokens[pos1]->type != end_)
        {
            curr->left = expr();
            if (tokens[pos1]->space > space && tokens[pos1]->type != end_)
            {
                curr->right = new_node(NULL);
                curr = curr->right;
            }
        }
        return node;
    }
    return prime();
}

Node *prime()
{
    Node *node = NULL;
    Token *token;
    if ((token = check((Type[]){int_, float_, char_, string_, bool_, 0})))
    {
        if (token->declaration)
        {
            if (!tokens[pos1] || tokens[pos1]->type != name_)
            {
                error = true;
                RLOG("error", "expected identifier after %s word\n", to_string(token->type));
                return NULL;
            }
            Node *next = prime();
            token->name = strdup(next->token->name);
            free_node(next);
        }
        return new_node(token);
    }
    else if ((token = check((Type[]){name_, 0})))
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
        print_token(tokens[pos1], true);
    }
    return node;
}

void print_space(int space)
{
    int i = 0;
    while (i < space)
        i += printf(" ");
}

Token *inter(Node *node)
{
    Token *token = node->token;
    switch (token->type)
    {
    case name_:
    {
        node->token = get_var(node->token);
        break;
    }
    case less_:
    case more_:
    case equal_:
    {
        printf("comp ");
        inter(node->left);
        // printf(",");
        inter(node->right);
        break;
    }
    case add_:
    case sub_:
    case div_:
    case mul_:
    {
        // printf("v%zu: ", token->index);
        Token *left = inter(node->left);
        Token *right = inter(node->right);
        node->left->token = left;
        node->right->token = right;
        printf("v%zu: v%zu %s v%zu\n", token->index, left->index, to_string(token->type), right->index);
        break;
    }
    case assign_:
    {
        Token *left = inter(node->left);
        Token *right = inter(node->right);
        printf("v%zu: v%zu %s v%zu\n", left->index, left->index, to_string(token->type), right->index);
        token->index = left->index;
        break;
    }
    case int_:
    {
        // printf("v%zu: ", token->index);
        if (token->declaration)
            node->token = new_variable(token);
        //     printf("v%zu: %s %s\n", token->index, token->name, to_string(token->type));
        // else
        //     printf("v%zu: %lld \n", token->index, token->_int.value);
        break;
    }
#if 0
    case for_:
    {
        printf("%s ", to_string(token->type));
        inter(node->left->left);
        inter(node->left->right->left);

        printf("\n");
        Node *curr = node->right;
        while (curr)
        {
            print_space(token->space + 5);
            inter(curr->left);
            printf("\n");
            curr = curr->right;
        }
        printf("end%s\n", to_string(token->type));
        break;
    }
#endif
    case while_:
    {
        print_space(token->space);
        printf("%s%zu\n", to_string(token->type), node->token->index);
        inter(node->left); // condition
        printf("\n");
        print_space(token->space);
        printf("is false\n");
        print_space(token->space);
        printf("jmp end%s%zu\n", to_string(token->type), node->token->index);
        Node *curr = node->right;
        while (curr)
        {
            print_space(token->space);
            inter(curr->left);
            printf("\n");
            curr = curr->right;
            printf("jmp %s%zu\n", to_string(token->type), node->token->index);
        }
        printf("end%s%zu\n", to_string(token->type), node->token->index);
        break;
    }
    case if_:
    {
        print_space(token->space);
        inter(node->left->left); // condition
        // printf("\n%s%zu ", to_string(token->type), node->token->index);
        printf("\n");
        print_space(token->space);
        printf("is false\n");
        print_space(token->space);
        if (node->right)
            printf("jmp %s%zu\n", to_string(node->right->token->type), node->right->token->index);
        else
            printf("jmp end%s%zu\n", to_string(token->type), node->token->index);
        Node *curr = node->left->right;
        while (curr)
        {
            print_space(token->space);
            inter(curr->left);
            printf("\n");
            curr = curr->right;
        }

        // printf("jmp end%s%zu\n", to_string(token->type), node->token->index);
        if (node->right)
            inter(node->right);
        printf("end%s%zu\n", to_string(token->type), node->token->index);
        break;
    }
    case else_:
    {
        print_space(token->space);
        printf("%s%zu\n", to_string(token->type), node->token->index);
        Node *curr = node;
        while (curr)
        {
            print_space(token->space);
            inter(curr->left);
            printf("\n");
            curr = curr->right;
        }
        // printf("end%s%zu\n", to_string(token->type), node->token->index);
        break;
    }
    default:
        break;
    }
    return token;
}

Token *generate(Node *node)
{
    switch (node->token->type)
    {
    case name_:
    {
        printf("%s", node->token->name);
        node->token = get_var(node->token);
        break;
    }
    case less_:
    case more_:
    case equal_:
    {
        printf("comp ");
        generate(node->left);
        // printf(",");
        generate(node->right);
        break;
    }
    case add_:
    case sub_:
    case div_:
    case mul_:
    {
        Token *left = generate(node->left);
        node->left->token = left;
        printf(" + ");
        Token *right = generate(node->right);
        node->right->token = right;
        // printf("v%zu: v%zu %s v%zu\n", node->token->index, left->index, to_string(node->token->type), right->index);
        break;
    }
    case assign_:
    {
        Token *left = generate(node->left);
        printf(" = ");
        Token *right = generate(node->right);
        // node->left->token = left;
        // node->right->token = right;
        printf("\n");
        node->token->index = left->index;
        // printf("v%zu: v%zu %s v%zu\n", left->index, left->index, to_string(token->type), right->index);
        break;
    }
    case int_:
    {
        // printf("v%zu: ", token->index);
        if (node->token->name)
            printf("%s", node->token->name);
        else
            printf("%lld", node->token->_int.value);
        return node->token;
        //     printf("v%zu: %s %s\n", token->index, token->name, to_string(token->type));
        // else
        //     printf("v%zu: %lld \n", token->index, token->_int.value);
        break;
    }
#if 0
    case for_:
    {
        printf("%s ", to_string(token->type));
        inter(node->left->left);
        inter(node->left->right->left);

        printf("\n");
        Node *curr = node->right;
        while (curr)
        {
            print_space(token->space + 5);
            inter(curr->left);
            printf("\n");
            curr = curr->right;
        }
        printf("end%s\n", to_string(token->type));
        break;
    }
#endif
    // case while_:
    // {
    //     print_space(token->space);
    //     printf("%s%zu\n", to_string(token->type), node->token->index);
    //     inter(node->left); // condition
    //     printf("\n");
    //     print_space(token->space);
    //     printf("is false\n");
    //     print_space(token->space);
    //     printf("jmp end%s%zu\n", to_string(token->type), node->token->index);
    //     Node *curr = node->right;
    //     while (curr)
    //     {
    //         print_space(token->space);
    //         inter(curr->left);
    //         printf("\n");
    //         curr = curr->right;
    //         printf("jmp %s%zu\n", to_string(token->type), node->token->index);
    //     }
    //     printf("end%s%zu\n", to_string(token->type), node->token->index);
    //     break;
    // }
    // case if_:
    // {
    //     print_space(token->space);
    //     inter(node->left->left); // condition
    //     // printf("\n%s%zu ", to_string(token->type), node->token->index);
    //     printf("\n");
    //     print_space(token->space);
    //     printf("is false\n");
    //     print_space(token->space);
    //     if (node->right)
    //         printf("jmp %s%zu\n", to_string(node->right->token->type), node->right->token->index);
    //     else
    //         printf("jmp end%s%zu\n", to_string(token->type), node->token->index);
    //     Node *curr = node->left->right;
    //     while (curr)
    //     {
    //         print_space(token->space);
    //         inter(curr->left);
    //         printf("\n");
    //         curr = curr->right;
    //     }

    //     // printf("jmp end%s%zu\n", to_string(token->type), node->token->index);
    //     if (node->right)
    //         inter(node->right);
    //     printf("end%s%zu\n", to_string(token->type), node->token->index);
    //     break;
    // }
    // case else_:
    // {
    //     print_space(token->space);
    //     printf("%s%zu\n", to_string(token->type), node->token->index);
    //     Node *curr = node;
    //     while (curr)
    //     {
    //         print_space(token->space);
    //         inter(curr->left);
    //         printf("\n");
    //         curr = curr->right;
    //     }
    //     // printf("end%s%zu\n", to_string(token->type), node->token->index);
    //     break;
    // }
    default:
        break;
    }
    return node->token;
}

Node **ir;
size_t ir_pos;
size_t ir_len = 10;
void add_to_ir(Node *node)
{
    if (ir_pos + 1 > ir_len)
    {
        Node **tmp = calloc(ir_len * 2, sizeof(Node *));
        memcpy(tmp, ir, ir_pos * sizeof(Node *));
        free(ir);
        ir = tmp;
        ir_len *= 2;
    }
    ir[ir_pos] = node;
    ir_pos++;
}

bool ir_found(Node *to_find, Node *curr)
{
    bool found = false;
    if (curr)
    {
        if (curr->token->index == to_find->token->index)
            return true;
        found = ir_found(to_find, curr->left) || ir_found(to_find, curr->right);
    }
    return found;
}

void check_ir()
{
    size_t i = 0;
    while (i < ir_pos)
    {
        size_t j = i + 1;
        while (j < ir_pos)
        {
            if (!ir_found(ir[i], ir[j]))
                ir[i]->token->remove = true;
            else
                CLOG("found", "index %zu\n", ir[j]->token->index);
            j++;
        }
        i++;
    }
}

void visualize()
{
    CLOG("visualize", "\n");
    size_t i = 0;
    while (i < ir_pos)
    {
        // print_node(ir[i], NULL, 0);
        if (!ir[i]->token->remove)
        {
            // GLOG("NOT remove", "\n");
            generate(ir[i]);
        }
        else
        {
            // RLOG("remove", "\n");
            free_node(ir[i]);
        }
        i++;
    }
}

void compile()
{
    pos0 = 0;
    pos1 = 0;
    len = 10;
    vpos = 0;
    vlen = 10;

    Node *head = new_node(NULL);
    Node *curr = head;
    tokens = calloc(len, sizeof(Token *));
    variables = calloc(vlen, sizeof(Token *));
    ir = calloc(ir_len, sizeof(Node));

    printf("%s", SPLIT);
    if (tokenize() == 0)
    {
        printf("%s", SPLIT);
        curr->left = expr();
        while (tokens[pos1]->type != end_ && !error)
        {
            curr->right = new_node(NULL);
            curr = curr->right;
            curr->left = expr();
        }
        printf("%s", SPLIT);
        if (!error)
        {
            curr = head;
            while (curr)
            {
                print_node(curr->left, NULL, 0);
                curr = curr->right;
            }
            printf("%s", SPLIT);
            curr = head;
            while (curr)
            {
                // evaluate(curr->left);
                inter(curr->left);
                add_to_ir(curr->left);
                // printf("\n");
                curr = curr->right;
            }
            printf("%s", SPLIT);
            check_ir();
            printf("%s", SPLIT);
            visualize();
        }
        printf("%s", SPLIT);
    }
    // free_node(head);
    free_tokens();
}

int main(int argc, char **argv)
{
    // asm_fd = fopen("file.s", "w");
    FILE *file = fopen("file.w", "r");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    input = calloc(size + 1, sizeof(char));
    fread(input, size, sizeof(char), file);
    fclose(file);
    compile();
    // fclose(asm_fd);
    if (error)
        exit(1);
}