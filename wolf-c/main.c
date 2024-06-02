#include "header.h"

size_t tk_pos = 0;
size_t tk_len = 10;
size_t exe_pos = 0;
Token **tokens;
bool error;

Inst **insts;
size_t inst_pos = 0;
size_t inst_len = 10;

void new_inst(char *cmd, Token *left, Token *right, size_t index)
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
    new->cmd = strdup(cmd);
    new->left = left;
    new->right = right;
    new->index = index;
}

void free_insts()
{
    for (size_t i = 0; i < inst_pos; i++)
    {
        free(insts[i]->cmd);
        free(insts[i]);
    }
    free(insts);
}

Token *inter(Node *node)
{
    switch (node->token->type)
    {
    case name_:
    {
        break;
    }
    case int_:
    {
        break;
    }
    case add_:
    {
        break;
    }
    case assign_:
    {
        break;
    }
    }
    return node->token;
}

void compile(char *input)
{
    Node *head = new_node(NULL);
    Node *curr = head;
    tokens = calloc(tk_len, sizeof(Token *));
    insts = calloc(inst_len, sizeof(Inst *));

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
#if 0
            printf("\n%sINTERMEDIATE REPR%s\n", SPLIT, SPLIT);
            curr = head;
            while (curr)
            {
                inter(curr->left);
                curr = curr->right;
            }
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