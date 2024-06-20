#include "header.h"

char *input;
Token **tokens;
size_t pos0;
size_t pos1;
size_t len;
bool error;
FILE *asm_fd;
size_t line;
size_t ptr;
size_t bloc_index;
size_t var_index;

sType *dataTypes = (sType[]){
    {"int", int_},
    {"float", float_},
    {"char", char_},
    {"string", string_},
    {"array", array_},
    {"bool", bool_},
    {0, 0},
};

sType *blocTypes = (sType[]){
    {"for", for_},
    {"in", in_},
    {"if", if_},
    {"else", else_},
    {"while", while_},
    {0, 0},
};

sType *symbTypes = (sType[]){
    {"==", equal_},
    {"=", assign_},
    {"+", add_},
    {"-", sub_},
    {"*", mul_},
    {"/", div_},
    {"(", lpar_},
    {")", rpar_},
    {"<", less_},
    {">", more_},
    {",", semi_},
    {":", dots_},
    {0, 0},
};

sType *randTypes = (sType[]){
    {"name", name_},
    {"END", end_},
    {0, 0},
};

Token **variables;
size_t vpos;
size_t vlen;

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
    token->declaration = false;
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

void print_space(int space)
{
    int i = 0;
    while (i < space)
        i += printf(" ");
}

Token *inter(Node *node)
{
    switch (node->token->type)
    {
    case name_:
    {
        node->token = get_var(node->token);
        add_to_ir(node);
        break;
    }
    case add_:
    case sub_:
    case div_:
    case mul_:
    {
        Token *left = inter(node->left);
        Token *right = inter(node->right);

        node->left->token = left;
        node->right->token = right;

        add_to_ir(node);
        break;
    }
    case assign_:
    {
        Token *left = inter(node->left);
        Token *right = inter(node->right);

        node->left->token = left;
        node->right->token = right;
        node->token->index = left->index;

        add_to_ir(node);
        break;
    }
    case int_:
    {
        if (node->token->declaration)
        {
            node->token = new_variable(node->token);
            add_to_ir(node);
        }
        break;
    }
    default:
        break;
    }
    return node->token;
}

Token *generate(Node *node)
{
    switch (node->token->type)
    {
    case name_:
    {
        break;
    }
    case add_:
    case sub_:
    case div_:
    case mul_:
    {
        Token *left = generate(node->left);
        Token *right = generate(node->right);
        printf("v%zu: v%zu %s v%zu\n", node->token->index, left->index, to_string(node->token->type), right->index);
        break;
    }
    case assign_:
    {
        Token *left = generate(node->left);
        Token *right = generate(node->right);
        printf("v%zu: v%zu %s v%zu\n", node->token->index, left->index, to_string(node->token->type), right->index);
        break;
    }
    case int_:
    {
        // printf("///////////");
        // if (node->token->name)
        //     printf("%s", node->token->name);
        // else
        //     printf("%lld", node->token->_int.value);
        // return node->token;
        break;
    }
    default:
        break;
    }
    return node->token;
}

Node **ir;
size_t ir_pos;
size_t ir_len = 10;
void add_to_ir(Node *node)
{
    if (ir_pos + 1 > ir_len)
    {
        Node **tmp = calloc(ir_len * 2, sizeof(Node *));
        memcpy(tmp, ir, ir_pos * sizeof(Node *));
        free(ir);
        ir = tmp;
        ir_len *= 2;
    }
    ir[ir_pos] = node;
    ir_pos++;
}

bool ir_found(Node *to_find, Node *curr)
{
    to_find->token->keep = curr->token->index == to_find->token->index;
    return to_find->token->keep;
}

void check_ir()
{
    size_t i = 0;
    while (i < ir_pos)
    {
        // printf("check ");
        // print_node(ir[i], NULL, 0);
        size_t j = i + 1;
        while (j < ir_pos && !ir[i]->token->keep && !ir_found(ir[i], ir[j]))
        {
            j++;
        }
        i++;
    }
}

void visualize()
{
    CLOG("visualize", "\n");
    size_t i = 0;
    while (i < ir_pos)
    {
        // print_node(ir[i], NULL, 0);
        if (!ir[i]->token->keep)
        {
            // GLOG("NOT remove", "\n");
            // generate(ir[i]);
            RLOG("remove", "%s\n", to_string(ir[i]->token->type));
        }
        else
        {
            // RLOG("remove", "\n");
            // free_node(ir[i]);
        }
        generate(ir[i]);

        i++;
    }
}

void compile()
{
    var_index = 1;
    pos0 = 0;
    pos1 = 0;
    len = 10;
    vpos = 0;
    vlen = 10;

    Node *head = new_node(NULL);
    Node *curr = head;
    tokens = calloc(len, sizeof(Token *));
    variables = calloc(vlen, sizeof(Token *));
    ir = calloc(ir_len, sizeof(Node));

    printf("\n%sTOKENIZE%s\n", SPLIT, SPLIT);
    if (tokenize() == 0)
    {
        printf("\n%sBUILD TREE%s\n", SPLIT, SPLIT);
        curr->left = expr();
        while (tokens[pos1]->type != end_ && !error)
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
            printf("\n%sINTERMEDIATE REPR%s\n", SPLIT, SPLIT);
            curr = head;
            while (curr)
            {
                inter(curr->left);
                curr = curr->right;
            }
            printf("\n%sEND%s\n", SPLIT, SPLIT);
#if 0
            GLOG("ir_pos", "%zu\n", ir_pos);
            check_ir();
            printf("%s", SPLIT);
            visualize();
#endif
        }
    }
    free_node(head);
    free_tokens();
}

int main(int argc, char **argv)
{
    // asm_fd = fopen("file.s", "w");
    FILE *file = fopen("file.w", "r");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    input = calloc(size + 1, sizeof(char));
    fread(input, size, sizeof(char), file);
    fclose(file);
    compile();
    // fclose(asm_fd);
    if (error)
        exit(1);
}