#include "header.h"

// TOKENIZE GLOBALS
Token **tokens;
int tk_size;
int tk_pos;
int exe_pos;

// VARIABLES GLOBALS
Token **vars;
int var_size;
int var_pos;

// INSTRUCTIONS
Inst **first_insts;
Inst **insts;
int inst_size;
int inst_pos;

// SIMULATION
Inst **regs;
int reg_pos;
int reg_size;

// DEBUG
Specials *specials = (Specials[]){{"!=", not_equal_},{"==", equal_},{"<=", less_equal_},
    {">=", more_equal_}, {"<", less_}, {">", more_}, {"=", assign_}, {"+", add_},
    {"-", sub_}, {"*", mul_}, {"/", div_}, {"(", lpar_}, {")", rpar_}, {",", coma_}, 
    {":", dots_}, {"if", if_}, {0, (Type)0}};

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
            printf("[int] value [%lld]", token->Int.value);
        break;
    }
    case bool_:
    {
        if (token->name)
        {
            printf("[bool] name [%s]", token->name);
            if (token->declare)
                printf(" [declare]");
        }
        else
            printf("[bool] value [%d]", token->Bool.value);
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
    case end_:
        printf("[end]");
        break;
    default:
    {
        for (int i = 0; specials[i].value; i++)
        {
            if (specials[i].type == token->type)
            {
                printf("[%s]", specials[i].value);
                break;
            }
        }
        break;
    }
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
            printf("%s: ", side);
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
    case add_: return "ADD   ";
    case sub_: return "SUB   ";
    case mul_: return "MUL   ";
    case div_: return "DIV   ";
    case equal_: return "EQUAL";
    case less_: return "LESS THAN";
    case more_: return "MORE THAN";
    case more_equal_: return "MORE THAN OR EQUAL";
    case less_equal_: return "LESS THAN OR EQUAL";
    case int_: return "INT   ";
    case assign_: return "ASSIGN";
    case lpar_: return "LPARENT";
    case rpar_: return "RPARENT";
    case fcall_: return "FUNC CALL";
    case end_: return "END";
    default:
        break;
    }
    return NULL;
}

#if AST
void clear(
    Node *head, char *input)
#else
void clear(char *input)
#endif
{
#if TOKENIZE
    for (int i = 0; i < tk_pos; i++)
    {
        if (tokens[i]->name)
            free(tokens[i]->name);
        free(tokens[i]);
    }
#endif

#if AST
    free_node(head);
#endif

#if IR
    for (int i = 0; i < inst_pos; i++)
        free(first_insts[i]);
    free(first_insts);
    free(regs);
#endif
    free(input);
}

bool check_type(Type *types, Type type)
{
    for (int i = 0; types[i]; i++)
    {
        if (types[i] == type)
            return true;
    }
    return false;
}

void copy_insts()
{
    if (insts)
        free(insts);
    insts = calloc(inst_size, sizeof(Inst *)); // TODO: protect it if no instruction created
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        if (!first_insts[i]->token->remove)
            insts[j++] = first_insts[i];
    }
}