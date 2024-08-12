#include "utils.c"

#if TOKENIZE
Token **tokens;
size_t tk_pos;
size_t tk_size;
size_t exe_pos;

void add_token(Token *token)
{
    if (tokens == NULL)
    {
        tk_size = 100;
        tokens = calloc(tk_size, sizeof(Token *));
    }
    else if (tk_pos + 1 == tk_size)
    {
        Token **tmp = calloc(tk_size * 2, sizeof(Token *));
        memcpy(tmp, tokens, tk_pos * sizeof(Token *));
        tk_size *= 2;
        free(tokens);
        tokens = tmp;
    }
    tokens[tk_pos++] = token;
}

Token *new_token(char *input, int s, int e, int space, Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->space = space;
    switch (type)
    {
    case int_:
    {
        while (s < e)
            new->Int.value = new->Int.value * 10 + input[s++] - '0';
        break;
    }
    case chars_:
    {
        new->Chars.value = calloc(e - s + 1, sizeof(char));
        strncpy(new->Chars.value, input + s, e - s);
        break;
    }
    case char_:
    {
        new->Char.value = input[s];
        break;
    }
    case float_:
    {
        while(s < e && input[s] != '.')
            new->Float.value = new->Float.value * 10 + (float)(input[s++] - '0');
        if(input[s] == '.') s++; // TODO: it shoul be always .
        float tmp = 0.0;
        float pres = 0.1;
        while(s < e)
        {
            tmp += (float)(input[s++] - '0') * pres;
            pres *= 0.1;
        }
        new->Float.value += tmp;
        break;
    }
    // case fcall_: case id_: case jmp_: case bloc_:
    default:
    {
        if(e > s)
        {
            new->name = calloc(e - s + 1, sizeof(char));
            strncpy(new->name, input + s, e - s);
            if 
            (
                (strcmp(new->name, "True") == 0 && (new->Bool.value = true))
                ||
                (strcmp(new->name, "False") == 0 && (new->Bool.value = false))
            )
            {
                free(new->name);
                new->name = NULL;
                new->type = bool_;
            }
            else if (strcmp(new->name, "sys") == 0)
            {
                new->type = module_;
            }
        }
        break;
    }
    }
    add_token(new);
    return (new);
}

Token *copy_token(Token *token)
{
    RLOG(FUNC, "call it [%s]\n", to_string(token->type));
    if(token == NULL) return NULL;
    Token *new = calloc(1, sizeof(Token));
    memcpy(new, token, sizeof(Token));
    if(token->name) // TODO: check all values that can be copied exmaple: name ...
        new->name = strdup(token->name);
    if(token->Chars.value)
        new->Chars.value = strdup(token->Chars.value);
    add_token(new);
    return new;
}

void tokenize(char *input)
{
    int i = 0;
    int space = 0;
    bool new_line = true;
    while (input[i])
    {
        int s = i;
        if (isspace(input[i]))
        {
            if(input[i] == '\n')
            {
                new_line = true;
                space = 0;
            }
            if(new_line && input[i] == ' ') space++;
            else if(!new_line && input[i] == ' ') space = 0;
            i++;
            continue;
        }
        else if(input[i] == '#') // TODO: handle new lines inside comment
        {
            i++;
            while(input[i] && input[i] != '#') i++;
            if(input[i] != '#'){error("expected '#'\n"); exit(1);}
            i++;
            continue;
        }
        else if(strncmp(input + i, "//", 2) == 0)
        {
            while(input[i] && input[i] != '\n') i++;
            continue;
        }
        new_line = false;
        bool found = false;
        for (int j = 0; specials[j].value; j++)
        {
            if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
            {
                new_token(NULL, 0, 0, space, specials[j].type);
                found = true;
                i += strlen(specials[j].value);
                break;
            }
        }
        if (found) continue;
        if(input[i] == '\"')
        {
            i++;
            while(input[i] && input[i] != '\"') i++;
            if(input[i] != '\"'){error("expected '\"'\n"); exit(1);}
            i++;
            new_token(input, s, i, space, chars_);
            continue;
        }
        if(input[i] == '\'')
        {
            i++;
            if(input[i] && input[i] != '\'') i++;
            if(input[i] != '\''){error("expected '\''\n"); exit(1);}
            i++;
            new_token(input, s + 1, i, space, char_);
            continue;
        }
        if (isalpha(input[i]))
        {
            while (isalnum(input[i])) i++;
            new_token(input, s, i, space, id_);
            continue;
        }
        if (isdigit(input[i]))
        {
            Type type = int_;
            while (isdigit(input[i])) i++;
            if(input[i] == '.') {type = float_; i++;}
            while (isdigit(input[i])) i++;
            new_token(input, s, i, space, type);
            continue;
        }
        if(input[i]){error("Syntax <%c>\n", input[i]); exit(1);}
    }
    new_token(NULL, 0, 0, space, end_);
}
#endif

#if AST
Node *expr_node();
Node *assign_node();
Node *equality_node();
Node *comparison_node();
Node *add_sub_node();
Node *mul_div_node();
Node *dot_node();
Node *prime_node();

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    debug("new node has ");
    if(token) ptoken(token);
    else debug("NULL\n");
    return new;
}

Node *copy_node(Node *node)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = copy_token(node->token);
    if(node->left) new->left = copy_node(node->left);
    if(node->right) new->right = copy_node(node->right);
    return new;
}

Token *check(Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == tokens[exe_pos]->type) return tokens[exe_pos++];
        type = va_arg(ap, Type);
    }
    return NULL;
};

Node *expr_node()
{
    return assign_node();
}

Node *assign_node()
{
    Node *left = equality_node();
    Token *token;
    while ((token = check(assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, 0)))
    {
        Node *node = new_node(token);
        node->token->space = left->token->space;
        Node *right = equality_node();
        switch(token->type)
        {
            case add_assign_: case sub_assign_: case mul_assign_: case div_assign_:
            {
                Node *tmp = new_node(new_token(NULL, 0, 0, node->token->space, 
                node->token->type == add_assign_ ? add_ :
                node->token->type == sub_assign_ ? sub_ :
                node->token->type == mul_assign_ ? mul_ :
                node->token->type == div_assign_ ? div_ : 0
                ));
                node->token->type = assign_;
                tmp->left = copy_node(left);
                tmp->left->token->declare = false;
                tmp->right = right;
                right = tmp;
                break;
            }
            default:
                break;
        }
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

Node *equality_node()
{
    Node *left = comparison_node();
    Token *token;
    while ((token = check(equal_, not_equal_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = comparison_node();
        left = node;
    }
    return left;
}

Node *comparison_node()
{
    Node *left = add_sub_node();
    Token *token;
    while ((token = check(less_, more_, less_equal_, more_equal_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub_node();
        left = node;
    }
    return left;
}

Node *add_sub_node()
{
    Node *left = mul_div_node();
    Token *token;
    while ((token = check(add_, sub_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div_node();
        left = node;
    }
    return left;
}

Node *mul_div_node()
{
    Node *left = dot_node();
    Token *token = NULL;
    while ((token = check(mul_, div_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = dot_node();
        left = node;
    }
    return left;
}

Node *dot_node()
{
    Node *left = prime_node();
    Token *token = NULL;
    if((token = check(module_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = prime_node();
        // GLOG("", "found module\n");
        // pnode(node, NULL, 0);
        // exit(1);
        return node;
    }
    else if((token = check(dot_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = dot_node();
        return node;
    }
    return left;
}

Specials DataTypes[] = {
    {"int", int_}, {"bool", bool_}, {"chars", chars_}, {"void", void_},
    {"float", float_}, {"char", char_},
    {0, 0}};

Node *prime_node()
{
    Node *node = NULL;
    Token *token;
    if ((token  = check(int_, bool_, char_, float_, chars_, void_, id_, lpar_, rpar_, 0)))
    {
        for (int i = 0; token->type == id_ && DataTypes[i].value; i++)
        {
            if (strcmp(token->name, DataTypes[i].value) == 0)
            {
                Type type = DataTypes[i].type;
                int space = token->space;
                token = check(id_, 0);
                if (token)
                {
                    node = new_node(token);
                    node->token->type = type;
                    node->token->space = space;
                    node->token->declare = true;
                    return node;
                }
                else
                {
                    error("%s:%s\n", FUNC, LINE);
                    exit(1);
                }
            }
        }
        if (token->type == lpar_)
        {
            node = expr_node();
            if (!check(rpar_, 0))
            {
                error("Expected ) but found '%s'\n", to_string(tokens[exe_pos]->type));
                exit(1);
            }
        }
        else
        {
            node = new_node(token);
            if (check(lpar_, 0)) // TODO: only if is identifier
            {
                if(strcmp(node->token->name, "main") == 0)
                {
                    if(!check(rpar_, 0))
                    {
                        // TODO: error
                    }
                    if(!check(dots_, 0))
                    {
                        // TODO: error
                    }
                    node->token->type = fdec_;
                    Node *curr = node;
                    while
                    (
                        tokens[exe_pos]->space > node->token->space &&
                        tokens[exe_pos]->type != end_
                    )
                    {
                        curr->right = new_node(NULL);
                        curr = curr->right;
                        curr->left = expr_node();
                    }
                }
                else
                {
                    node->token->type = fcall_;
                    // Node *func = get_function(node->token->name);
                    Node *curr = node;
                    while (!check(rpar_, end_, 0)) // TODO: protect it, if no ) exists
                    {
                        curr->left = expr_node();
                        if (!check(coma_, 0))
                        {
                            // TODO: syntax error
                        }
                        curr->right = new_node(NULL);
                        curr = curr->right;
                    }
                }
            }
        }
    }
#if 1
    else if((token = check(fdec_, 0)))
    {
        node = new_node(token);

        node->left = new_node(NULL);
        node->left->left = prime_node();
        // TODO: hard code it
        if(!node->left->left->token || !node->left->left->token->declare)
        {
            error("expected datatype after func declaration\n");
            exit(1);
        }
        node->token->name = node->left->left->token->name;
        node->left->left->token->name = NULL;
        // TODO: those errors should be checked
        if(!check(lpar_, 0))
        {
            error("expected ( after function declaration\n");
            exit(1);
        }
        debug("type: [%s]\n", to_string(tokens[exe_pos]->type));
        if(!check(rpar_, 0))
        {
            // inside_function = true;
            // arg_ptr = 8;
            Node *curr = node->left;
            while(!check_type((Type[]){rpar_, end_, 0}, tokens[exe_pos]->type))
            {
                curr->right = new_node(NULL);
                curr = curr->right;
                curr->left = expr_node();
                if(tokens[exe_pos]->type != rpar_ && !check(coma_, 0))
                {
                    error("expected ',' between arguments\n");
                    exit(1);
                }
            }
            if(!check(rpar_, 0))
            {
                error("expected ) after func dec, but found <%s>\n",
                to_string(tokens[exe_pos]->type));
                exit(1);
            }
        }
        if(!check(dots_, 0))
        {
            error("expected : after func dec, but found <%s>\n",
            to_string(tokens[exe_pos]->type));
            exit(1);
        }

        node->token->type = fdec_;
        Node *curr = node;
        while
        (
            tokens[exe_pos]->space > node->token->space &&
            tokens[exe_pos]->type != end_
        )
        {
            curr->right = new_node(NULL);
            curr = curr->right;
            curr->left = expr_node();
        }
        // TODO: add endblock for funstions also
        // curr->right = new_node(NULL);
        // Token *ret_token = new_token(NULL, 0, 0, node->token->space + 1, ret_);
        // curr->right->left = new_node(ret_token);
        return node;
    }
#endif
    // TODO: handle error parsing
    else if((token  = check(if_, 0)))
    {
        node = new_node(token);
        
        Node *tmp = node;
        tmp->left = new_node(NULL);
        tmp = tmp->left;

        tmp->left = expr_node(); // if condition
        if(!check(dots_, 0))
            ; // TODO: expected ':' after condition
        
        tmp->right = new_node(NULL);
        tmp = tmp->right;
        while(tokens[exe_pos]->space > node->token->space) // if bloc code
        {
            tmp->left = expr_node();
            tmp->right = new_node(NULL);
            tmp = tmp->right;
        }

        Node *curr = node;
        while
        (
            check_type((Type[]){elif_, else_, 0}, tokens[exe_pos]->type) &&
            tokens[exe_pos]->space == node->token->space
        )
        {
            token = tokens[exe_pos++];
            curr->right = new_node(NULL);
            curr = curr->right;
            curr->left = new_node(token);
            if(token->type == elif_)
            {
                Node *tmp0 = curr->left;
                tmp0->left = expr_node();
                if(!check(dots_, 0))
                {
                    error("expected dots");
                    exit(1);
                }
                tmp0->right = new_node(NULL);
                tmp0 = tmp0->right;
                while(tokens[exe_pos]->space > token->space)
                {
                    tmp0->left = expr_node();
                    tmp0->right = new_node(NULL);
                    tmp0 = tmp0->right;
                }
            }
            else if(token->type == else_)
            {
                if(!check(dots_, 0))
                {
                    error("expected dots");
                    exit(1);
                }
                Node *tmp0 = curr->left;

                tmp0->right = new_node(NULL);
                tmp0 = tmp0->right;
                while(tokens[exe_pos]->space > token->space)
                {
                    tmp0->left = expr_node();
                    tmp0->right = new_node(NULL);
                    tmp0 = tmp0->right;
                }
                break;
            }
        }
/*
sys.output()
sys.puts()
*/
    }
    else if((token = check(while_, 0)))
    {
        node = new_node(token);
        node->left = expr_node();
        if(!check(dots_, 0))
        {
            error("expected :\n");
            exit(1);
        }
        Node *tmp = node;
        while
        (
            tokens[exe_pos]->type != end_  &&
            tokens[exe_pos]->space > token->space
        )
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr_node();
        }
    }
    else if((token = check(ret_, 0)))
    {
        // TODO: check return type if is compatible with function
        node = new_node(token);
        node->left = expr_node();
    }
    else if((token = check(module_, 0)))
    {
        return new_node(token);
    }
    else if(tokens[exe_pos]->type == end_);
    else
    {
        debug("Prime: Unexpected token <%s>\n", to_string(tokens[exe_pos]->type));
        exit(1);
    }
    return node;
}
#endif

