#include "header.h"

size_t tk_pos = 0;
size_t tk_len = 10;
size_t exe_pos = 0;
Token **tokens;
bool error;

Token **variables;
size_t vpos = 0;
size_t vlen = 10;
size_t var_index;
size_t ptr;

Token *evaluate(Node *node)
{
    // switch(node->token->type)
    // {
    //     case
    // }
    return node->token;
}

void compile(char *input)
{
    Node *head = new_node(NULL);
    Node *curr = head;
    tokens = calloc(tk_len, sizeof(Token *));
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
#if 0
            // generate();
#endif
            printf("\n%sEND%s\n", SPLIT, SPLIT);
        }
    }
    free_node(head);
    free_tokens();
}


int main()
{
    char *input = open_file("file.w");
    compile(input);
    free(input);
    if (error)
        exit(1);
}
