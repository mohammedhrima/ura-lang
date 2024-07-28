#include "header.h"

char *to_string(Type type)
{
    sType **arr = (sType *[]){
        dataTypes,
        blocTypes,
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
    for (size_t i = 0; i < pos0; i++)
    {
        if (tokens[i]->name)
            free(tokens[i]->name);
        if (tokens[i]->_string.value)
            free(tokens[i]->_string.value);
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
        if (token->declaration)
            printf("[declaration] ");
        if (token->name)
            printf("name [%s] ", token->name ? token->name : "");
        else
            switch (token->type)
            {
            case int_:
                printf("value [%lld] ", token->_int.value);
                break;
            case float_:
                // printf("value [%ld]", token->_int.value);
                break;
            case string_:
                printf("value [%s] ", token->_string.value);
                break;
            case char_:
                printf("value [%c] ", token->_char.value);
                break;
            case name_:
                printf("name [%s] ", token->name);
                break;
            default:
                break;
            }
        if (token->index)
            printf("index [%zu] ", token->index);
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
