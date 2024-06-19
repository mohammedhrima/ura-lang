#include "header.h"

size_t tk_pos = 0;
size_t tk_len = 10;
size_t exe_pos = 0;
Token **tokens;
bool error;

Inst **insts;
size_t inst_pos = 0;
size_t inst_len = 10;

void new_inst(Token *token, Token *left, Token *right)
{
    if (inst_pos + 1 > inst_len)
    {
        Inst **tmp = calloc(2 * inst_len, sizeof(Inst *));
        memcpy(tmp, insts, inst_pos * sizeof(Inst *));
        free(tokens);
        insts = tmp;
        inst_len *= 2;
    }
    Inst *new = calloc(1, sizeof(Inst));
    new->token = token;
    new->left = left;
    new->right = right;
    // new->cmd = strdup(cmd);
    // new->node = node;
    // new->index = index;
    insts[inst_pos++] = new;
}

void free_insts()
{
    for (size_t i = 0; i < inst_pos; i++)
    {
        // free(insts[i]->cmd);
        free(insts[i]);
    }
    free(insts);
}

Token **variables;
size_t vpos = 0;
size_t vlen = 10;
size_t var_index;
size_t ptr;

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
        Token **tmp = calloc(2 * vlen, sizeof(Token *));
        memcpy(tmp, variables, vpos * sizeof(Token *));
        free(variables);
        variables = tmp;
        vlen *= 2;
    }
    token->declare = false;
    // token->index = ++var_index;
    token->ptr = (ptr += 8);
    variables[vpos++] = token;
    return token;
}

Token *get_var(Token *token)
{
    // GLOG("get var", "<%s> has index <%zu>\n", token->name, token->index);
    for (size_t i = 0; i < vpos; i++)
    {
        if (strcmp(token->name, variables[i]->name) == 0)
        {
            // GLOG("get var", "found <%s> has index <%zu>\n", variables[i]->name, variables[i]->index);
            return variables[i];
        }
    }
    error = true;
    RLOG("error", "%s not found\n", token->name);
    return token;
}

Token *inter(Node *node)
{
    switch (node->token->type)
    {
    case name_:
    {
        node->token = get_var(node->token);
        break;
    }
    case int_:
    {
        if (node->token->declare)
            node->token = new_variable(node->token);
        break;
    }
    case assign_:
    {
        Token *left = inter(node->left);
        Token *right = inter(node->right);
        new_inst(node->token, left, right);
        break;
    }
    case add_:
    {
        Token *left = inter(node->left);
        Token *right = inter(node->right);
        if (!left->ptr && !right->ptr)
        {
            node->token->type = left->type;
            node->token->_int.value = left->_int.value + right->_int.value;
        }
        else
        {
            node->token->ptr = (ptr += 8);
            new_inst(node->token, left, right);
        }
        break;
    }
    }
    return node->token;
}

char *to_inst(Type type)
{
    switch (type)
    {
    case add_:
        return "ADD";
    case assign_:
        return "ASSIGN";
    default:
        break;
    }
    return to_string(type);
}

void print_inst(Token *token, bool end)
{
    if (token)
    {
        printf("[%s] ", to_inst(token->type));
        if (token->name)
            printf("ident [%s] ", token->name ? token->name : "");
        else
            switch (token->type)
            {
            case int_:
                printf("value [%lld] ", token->_int.value);
                break;
            case name_:
                printf("name [%s] ", token->name);
                break;
            default:
                break;
            }
        if (token->ptr)
            printf("ptr [%zu] ", token->ptr);
    }
    else
        printf("token [NULL] ");
    if (end)
        printf("\n");
}

void visualize()
{
    printf("\n%sVISUALIZE%s\n", SPLIT, SPLIT);
    for (size_t i = 0; i < inst_pos; i++)
    {
        print_inst(insts[i]->token, 1);
        printf("    ");
        print_inst(insts[i]->left, 1);
        printf("    ");
        print_inst(insts[i]->right, 1);
    }
    printf("\n%s%s\n", SPLIT, SPLIT);
}

void compile(char *input)
{
    Node *head = new_node(NULL);
    Node *curr = head;
    tokens = calloc(tk_len, sizeof(Token *));
    insts = calloc(inst_len, sizeof(Inst *));
    variables = calloc(vlen, sizeof(Token *));

    if (tokenize(input) == 0)
    {
        printf("\n%sBUILD TREE%s\n", SPLIT, SPLIT);
        curr->left = expr();
        while (tokens[exe_pos]->type != end_ && !error)
        {
            curr->right = new_node(NULL);
            curr = curr->right;
            curr->left = expr();
        }
        if (!error)
        {
            printf("\n%sPRINT TREE%s\n", SPLIT, SPLIT);
            curr = head;
            while (curr)
            {
                print_node(curr->left, NULL, 0);
                curr = curr->right;
            }
#if 1
            printf("\n%sINTERMEDIATE REPR%s\n", SPLIT, SPLIT);
            curr = head;
            while (curr)
            {
                inter(curr->left);
                curr = curr->right;
            }
            visualize();
            printf("\n%sGENERATE%s\n", SPLIT, SPLIT);
            // generate();
#endif
            printf("\n%sEND%s\n", SPLIT, SPLIT);
        }
    }
    free_node(head);
    free_tokens();
    free_insts();
}

int main()
{
    FILE *file = fopen("file.w", "r");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = calloc(size + 1, sizeof(char));
    fread(input, size, sizeof(char), file);
    fclose(file);
    compile(input);
    // fclose(asm_fd);
    if (error)
        exit(1);
    free(input);
}