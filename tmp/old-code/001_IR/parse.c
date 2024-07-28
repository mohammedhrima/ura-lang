#include "header.h"

size_t col;
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
            return true;
        }
    }
    return false;
}

void new_token(char *input, int s, int e, Type type, bool declaration)
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
    {
        switch (type)
        {
        case name_:
            new->name = calloc(e - s + 1, sizeof(char));
            strncpy(new->name, input + s, e - s);
            if (check_list(new, dataTypes))
            {
                new->index = var_index++;
                break;
            }
            if (check_list(new, blocTypes))
            {
                new->index = bloc_index++;
                RLOG("error:", "is bloc\n");
                break;
            }
            break;
        case add_:
        case sub_:
        case mul_:
        case div_:
            new->index = var_index++;
            break;
        case assign_:
            break;
        case int_:
            new->index = var_index++;
            while (s < e)
                new->_int.value = 10 * new->_int.value + (input[s++] - '0');
            break;
        case float_:
            break;

        default:
            break;
        }
    }
    else
    {
        switch (type)
        {
        case int_:
            // new->index = var_index++;
            break;
        default:
            break;
        }
    }

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
            new_token(input, s, e, int_, false);
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
            new_token(input, s, e, string_, false);
            e++;
            continue;
        }
        for (int i = 0; symbTypes[i].value; i++)
        {
            if (strncmp(input + e, symbTypes[i].value, strlen(symbTypes[i].value)) == 0)
            {
                found = true;
                new_token(input, 0, 0, symbTypes[i].type, false);
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
            new_token(input, s, e, name_, false);
            continue;
        }
        RLOG("error", "tokenize: unexpedted %s\n", input + e);
        error = true;
        break;
    }
    new_token(input, 0, 0, end_, false);
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
            // token->index = next->token->index;
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
