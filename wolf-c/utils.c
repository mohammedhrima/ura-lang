#include "header.h"

char *to_string(Type type)
{
    switch (type)
    {
    case add_:
        return "+";
    case sub_:
        return "-";
    case mul_:
        return "*";
    case div_:
        return "/";
    case lpar_:
        return "(";
    case rpar_:
        return ")";
    case int_:
        return "int";
    case float_:
        return "float";
    case char_:
        return "char";
    case string_:
        return "string";
    case array_:
        return "array";
    case bool_:
        return "bool";
    case name_:
        return "name";
    }
    return "";
}

void free_tokens()
{
    for (size_t i = 0; i < pos0; i++)
        free(tokens[i]);
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

void print_token(Token *token)
{
    printf("token type [%s] ", to_string(token->type));
    switch (token->type)
    {
    case int_:
        printf("value [%ld]", token->_int.value);
        break;
    case float_:
        // printf("value [%ld]", token->_int.value);
        break;
    case string_:
        printf("value [%s]", token->_string.value);
        break;
    case char_:
        printf("value [%c]", token->_char.value);
        break;
    default:
        break;
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
            printf("%s", side);
        printf("node has ");
        print_token(node->token);
        print_node(node->left, "LEFT : ", space + 6);
        print_node(node->right, "RIGHT: ", space + 6);
    }
}

void pasm(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    if (!strchr(fmt, ':') && !strstr(fmt, ".section	.note.GNU-stack,\"\",@progbits") &&
        !strstr(fmt, ".intel_syntax noprefix") && !strstr(fmt, ".include") &&
        !strstr(fmt, ".text") && !strstr(fmt, ".globl	main"))
        fprintf(asm_fd, "   ");
    vfprintf(asm_fd, fmt, ap);
}
