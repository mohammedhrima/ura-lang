#include "header.h"

char *open_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = calloc(size + 1, sizeof(char));
    fread(input, size, sizeof(char), file);
    fclose(file);
    return input;
}

char *to_string(Type type)
{
    sType **arr = (sType *[]){
        dataTypes,
        // blocTypes,
        symbTypes,
        randTypes,
        NULL,
    };
    for (size_t i = 0; arr[i]; i++)
    {
        sType *elem = arr[i];
        for (size_t j = 0; elem[j].value; j++)
            if (elem[j].type == type)
                return elem[j].value;
    }
    return "";
}

void free_tokens()
{
    for (size_t i = 0; i < tk_pos; i++)
    {
        if (tokens[i]->name)
            free(tokens[i]->name);
        // if (tokens[i]->_string.value)
        //     free(tokens[i]->_string.value);
        free(tokens[i]);
    }
    free(tokens);
}

void free_node(Node *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

void print_token(Token *token, bool end)
{
    if (token)
    {
        printf("token type [%s] ", to_string(token->type));
        if (token->declare)
            printf("[declare] ");
        if (token->name)
            printf("name [%s] ", token->name ? token->name : "");
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

void print_node(Node *node, char *side, int space)
{
    int i = 0;
    if (node)
    {
        while (i < space)
            i += printf(" ");
        if (side)
            printf("%s%s%s: ", CYAN, side, RESET);
        printf("node has ");
        print_token(node->token, true);
        print_node(node->left, " LEFT", space + 4);
        print_node(node->right, "RIGHT", space + 4);
    }
}
