#include "header.h"

// DEBUG
void print_token(Token *token)
{
    printf("token ");
    switch (token->type)
    {
    case int_:
    {
        if (token->name)
        {
            printf("[int] name [%s]", token->name);
            if (token->declare)
                printf(" [declare]");
        }
        else
            printf("[int] value [%d]", token->value);
        break;
    }
    case fcall_:
    {
        printf("[func call] name [%s]", token->name);
        break;
    }
    case id_:
        printf("[id] name [%s]", token->name);
        break;
    default:
        printf("[%c]", token->type);
    }
    printf("\n");
}

void print_node(Node *node, char *side, int space)
{
    if (node)
    {
        int i = 0;
        while (i < space)
            i += printf(" ");
        if (side)
            printf("%s:", side);
        if (node->token)
        {
            printf("node: ");
            print_token(node->token);
            print_node(node->left, "LEFT ", space + 5);
            print_node(node->right, "RIGHT", space + 5);
        }
        else
            printf("\n");
    }
}

// UTILS
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

void free_node(Node *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

char *to_string(Type type)
{
    switch (type)
    {
    case add_:
        return "ADD   ";
    case sub_:
        return "SUB   ";
    case mul_:
        return "MUL   ";
    case div_:
        return "DIV   ";
    case int_:
        return "INT   ";
    case assign_:
        return "ASSIGN";
    case lpar_:
        return "LPARENT";
    case rpar_:
        return "RPARENT";
    case fcall_:
        return "FUNC CALL";
    case end_:
        return "END";
    default:
        break;
    }
    return NULL;
}

void clear(Node *head, char *input)
{
    free_node(head);
    for (int i = 0; i < tk_pos; i++)
    {
        if (tokens[i]->name)
            free(tokens[i]->name);
        free(tokens[i]);
    }
    for (int i = 0; i < inst_pos; i++)
    {
        // if(insts)
        free(insts[i]);
    }
    free(insts);
    free(input);
    free(regs);
}
