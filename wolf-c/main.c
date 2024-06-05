#include "header.h"

size_t tk_pos = 0;
size_t tk_len = 10;
size_t exe_pos = 0;
Token **tokens;
bool error;

Inst **insts;
size_t inst_pos = 0;
size_t inst_len = 10;

void new_inst(Node *node)
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
    // new->cmd = strdup(cmd);
    new->node = node;
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
    token->index = ++var_index;
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
    case add_:
    {
        node->left->token = inter(node->left);
        node->right->token = inter(node->right);
        if (!node->left->token->index && !node->right->token->index)
        {
            node->token->type = int_;
            node->token->_int.value = node->left->token->_int.value + node->right->token->_int.value;
        }
        else
        {
            // node->token->index = ++var_index;
            new_inst(node);
        }
        // if (node->left->token->index || node->right->token->index)
        // printf("v%zu: v%zu %s ", node->token->index, left->index, to_string(node->token->type));
        // if (right->index)
        //     printf("v%zu\n", right->index);
        // else
        //     printf("%lld\n", right->_int.value);

        break;
    }
    case assign_:
    {
        node->left->token = inter(node->left);
        node->right->token = inter(node->right);
        node->token->index = node->left->token->index;
        new_inst(node);

        // printf("v%zu: v%zu %s ", node->token->index, left->index, to_string(node->token->type));
        // if (right->index)
        //     printf("v%zu\n", right->index);
        // else
        //     printf("%lld\n", right->_int.value);
        break;
    }
    }
    return node->token;
}

void generate()
{
    for (size_t i = 0; i < inst_pos; i++)
    {
        while (insts[i]->node->token->type == add_)
        {

            Node *node = insts[i]->node;
            Token *token = node->token;
            Token *right = node->right->token;
            Token *left = node->left->token;
            if (!left->index && !right->index)
            {

                token->_int.value = left->_int.value + right->_int.value;
                token->type = int_;
                token->index = 0;
            }
            else
            {
#if 0
                printf("v%zu: ", token->index);
                if (left->index)
                    printf("v%zu", left->index);
                else
                    printf("%lld", left->_int.value);
                printf(" %s ", to_string(token->type));
                if (right->index)
                    printf("v%zu\n", right->index);
                else
                    printf("%lld\n", right->_int.value);
#else

                if (left->ptr)
                    printf("add rax, QWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->reg)
                {
                    printf("add rax, r%cx ///\n", left->reg);
                }
                else
                    printf("add rax, %lld\n", left->_int.value);

                if (right->ptr)
                    printf("add rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->reg)
                    printf("add rax, r%cx\n", right->reg);
                else
                    printf("add rax, %lld\n", right->_int.value);

                token->reg = 'a';
                token->ptr = 0;
#endif
            }
            i++;
        }
        if (insts[i]->node->token->type == assign_)
        {
            Node *node = insts[i]->node;
            Token *token = node->token;
            Token *right = node->right->token;
            Token *left = node->left->token;
#if 0
            printf("v%zu: v%zu %s ", token->index, left->index, to_string(token->type));
            if (right->index)
                printf("v%zu\n", right->index);
            else
                printf("%lld\n", right->_int.value);
#else
            printf("/*assign %s*/\n", left->name);
            if (right->ptr)
            {
                printf("mov rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                printf("mov QWORD PTR -%zu[rbp], rax\n", left->ptr);
                printf("mov rax, 0\n");
            }
            else if (right->reg)
            {
                printf("mov QWORD PTR -%zu[rbp], r%cx\n", left->ptr, right->reg);
                printf("mov r%cx, 0\n", right->reg);
            }
            else
            {
                printf("mov QWORD PTR -%zu[rbp], %lld\n", left->ptr, right->_int.value);
            }
#endif
        }
    }
}

/*
    + tokenize
    + create tree
    + intermediate
    + generate my own IR
    + generate assembly from the intermediate result
*/

void compile(char *input)
{
    Node *head = new_node(NULL);
    Node *curr = head;
    tokens = calloc(tk_len, sizeof(Token *));
    insts = calloc(inst_len, sizeof(Inst *));
    variables = calloc(vlen, sizeof(Token *));

    printf("\n%sTOKENIZE%s\n", SPLIT, SPLIT);
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
            generate();
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