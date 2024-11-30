// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

// MACROS
#define DEBUG 1
#define EXIT_STATUS 0
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define LINE __LINE__
#define FUNC __func__
#define FILE __FILE__

#define TOKENIZE 1

#if TOKENIZE
#define AST 1
#endif

#if AST
#define IR 1
#else
#define IR 0
#endif

#define MAX_OPTIMIZATION 3
#define WITH_COMMENTS 1

#if IR
#define BUILTINS 0
#ifndef OPTIMIZE
#define OPTIMIZE 0
#endif
#define ASM 0
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

void check(bool cond, char *fmt, ...);

// STRUCTS
typedef enum
{
	START = 11,
    ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
    EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
    ADD, SUB, MUL, DIV, MOD, AND, OR, RPAR, LPAR, COMA,
    RETURN, DOT, DOTS, IF, ELIF, ELSE, WHILE, FDEC, FCALL,
    INT, VOID, CHARS, CHAR, BOOL, FLOAT, STRUCT, ID, END_BLOC,
    END
} Type;

char *to_string(Type type)
{
    switch (type)
    {
    case ASSIGN: return "ASSIGN";
    case ADD_ASSIGN: return "ADD ASSIGN";
    case SUB_ASSIGN: return "SUB ASSIGN";
    case MUL_ASSIGN: return "MUL ASSIGN";
    case DIV_ASSIGN: return "DIV ASSIGN";

    case EQUAL: return "EQUAL";
    case NOT_EQUAL: return "NOT EQUAL";
    case LESS_EQUAL: return "LESS THAN OR EQUAL";
    case MORE_EQUAL: return "MORE THAN OR EQUAL";
    case LESS: return "LESS THAN";
    case MORE: return "MORE THAN";

    case ADD: return "ADD";
    case SUB: return "SUB";
    case MUL: return "MUL";
    case DIV: return "DIV";
    case MOD: return "MOD";

    case AND: return "AND";
    case OR: return "OR";

    case RPAR: return "R_PAR";
    case LPAR: return "L_PAR";
    case COMA: return "COMMA";

    case RETURN: return "RETURN";
    case DOT: return "DOT";
    case DOTS: return "DOTS";

    case IF: return "IF";
    case ELIF: return "ELIF";
    case ELSE: return "ELSE";
    case WHILE: return "WHILE";

    case FDEC: return "F_DEC";
    case FCALL: return "F_CALL";

    case INT: return "INT";
    case VOID: return "VOID";
    case CHARS: return "CHARS";
    case CHAR: return "CHAR";
    case BOOL: return "BOOL";
    case FLOAT: return "FLOAT";

    case STRUCT: return "STRUCT";
    case ID: return "ID";
    case END_BLOC: return "END_BLOC";
    case END: return "END";

    default:
    {
        check(1, "Unknown type [%d]\n", type);
        return "UNKNOWN";
    };
    }
    return NULL;
}

typedef struct
{
    Type type;
	Type retType;
    char *name;
    size_t ptr;
    bool declare;
    size_t space;
	bool remove;
	size_t reg;

    struct
    {
        // integer
        struct
        {
            long long value;
            int power;
            struct Int *next;
        } Int;
        // float
        struct
        {
            float value;
        } Float;
        // boolean
        struct
        {
            bool value;
            char c;
        } Bool;
        // chars
        struct
        {
            char *value;
        } Chars;
        // char
        struct
        {
            char value;
        } Char;
    };
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;
} Node;

typedef struct
{
    char *value;
    Type type;
} Specials;

Specials *specials = (Specials[]){
    {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
    {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL}, 
    {"==", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL}, {"<", LESS},
    {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB}, {"*", MUL},
    {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR}, {",", COMA},
    {"if", IF}, {"elif", ELIF}, {"else", ELSE}, {"while", WHILE}, 
    {"func", FDEC}, {"return", RETURN}, {"and", AND}, {"&&", AND},
    {"or", OR}, {"||", OR}, {0, (Type)0}};

Specials *dataTypes = (Specials[]){
    {"int", INT}, {"func", FDEC},
    {0,(Type)0}};

// UTILS
void debug(char *fmt, ...)
{
#if DEBUG
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

bool found_error;
void check(bool cond, char *fmt, ...)
{
    if (!cond)
        return;
    found_error = true;
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%sError:%s ", RED, RESET);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
	exit(1);
}


void ptoken(Token *token)
{
    debug("token: space [%.2d] [%-6s] ", token->space, to_string(token->type));
    switch (token->type)
    {
    case VOID: case CHARS: case CHAR: 
    case INT: case BOOL: case FLOAT:
    {
        if (token->name) debug(" name [%-4s] ", token->name);
        if (token->declare) debug("[declare] ");
        if(!token->name && !token->declare)
        {
            if (token->type == INT)
                debug("value [%lld] ", token->Int.value);
            else if (token->type == CHARS)
                debug("value [%s] ", token->Chars.value);
            else if (token->type == CHAR)
                debug("value [%c] ", token->Char.value);
            else if (token->type == BOOL)
                debug("value [%d] ", token->Bool.value);
            else if (token->type == FLOAT)
                debug("value [%f] ", token->Float.value);
            // else if(token->type == chars_)
            //     debug(" value [%s]", token->Chars.value);
            // else if(token->type == BOOL)
            //     debug(" value [%d]", token->Bool.value);
        }
        break;
    }
    case FCALL: case FDEC: case ID:
        debug(" name [%-4s] ", token->name);
        break;
    default:
    {
        // for (int i = 0; specials[i].value; i++)
        // {
        //     if (specials[i].type == token->type)
        //     {
        //         debug("[%s]", specials[i].value);
        //         break;
        //     }
        // }
        break;
    }
    }
    debug("\n");
}

void *allocate_func(size_t line, size_t len, size_t size)
{
    void *ptr = calloc(len, size);
    check(!ptr, "allocate did failed in line %zu\n", line);
    return ptr;
}

#define allocate(len, size) allocate_func(LINE, len, size)

char *open_file(char *filename)
{
    struct _IO_FILE *file = fopen(filename, "r");
    check(!filename, "failed to open file %s\n", filename);
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = allocate((size + 1), sizeof(char));
    if (input)
        fread(input, size, sizeof(char), file);
    fclose(file);
    return input;
}

// TOKENIZE
Token **tokens;
void add_token(Token *token)
{
    static size_t pos;
    static size_t len;
    if(len == 0)
    {
        len = 10;
        tokens = allocate(len, sizeof(Token*));
    }
    else if (pos + 1 == len)
    {
        Token **tmp = allocate(len * 2, sizeof(Token *));
        memcpy(tmp, tokens, len * sizeof(Token *));
        free(tokens);
        tokens = tmp;
        len *= 2;
    }
    tokens[pos++] = token;
}

Token *new_token(char *input, size_t s, size_t e, Type type, size_t space)
{
    Token *new = allocate(1, sizeof(Token));
    new->type = type;
    new->space = space;
    switch (type)
    {
    case INT:
    {
        while (s < e)
            new->Int.value = new->Int.value * 10 + input[s++] - '0';
        break;
    }
    case ID:
    {
        new->name = allocate(e - s + 1, sizeof(char));
        strncpy(new->name, input + s, e - s);
        break;
    }
    case CHAR:
    {
        new->Char.value = input[s];
        break;
    }
    default:
        break;
    }
    add_token(new);
    return new;
}

Token **tokenize(char *input)
{
    if(!TOKENIZE) return NULL;
	debug(GREEN"=========== TOKENIZE ===========\n"RESET);
    // size_t len = 10;
    // size_t pos = 0;
    size_t i = 0;
    size_t space = 0;
    bool inc_space = true;
    while (input[i] && !found_error)
    {
        size_t s = i;
        if (isspace(input[i]))
        {
            if(input[i] == '\n') inc_space = true;
            else if(inc_space) space++;
            i++;
            continue;
        }
        inc_space = false;
        bool found = false;
        for (int j = 0; specials[j].value; j++)
        {
            if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
            {
                new_token(NULL, 0, 0, specials[j].type, space);
                found = true;
                i += strlen(specials[j].value);
                break;
            }
        }
        for (int j = 0; !found && dataTypes[j].value; j++)
        {
            if (strncmp(dataTypes[j].value, input + i, strlen(dataTypes[j].value)) == 0)
            {
                Token *token = new_token(NULL, 0, 0, dataTypes[j].type, space);
                token->declare = true;
                free(token->name);
                token->name = NULL;
                found = true;
                i += strlen(dataTypes[j].value);
                break;
            }
        }
        if (found)
            continue;
        if (isalpha(input[i]))
        {
            while (isalnum(input[i]))
                i++;
            new_token(input, s, i, ID, space);
            continue;
        }
        if (isdigit(input[i]))
        {
            while (isdigit(input[i]))
                i++;
            new_token(input, s, i, INT, space);
            continue;
        }
        check(input[i], "Syntax error <%c>\n", input[i]);
    }
    new_token(input, 0, 0, END, space);
    for (size_t i = 0; tokens[i]; i++)
        ptoken(tokens[i]);
    return tokens;
}

void free_token(Token *token)
{
    if(token->name) free(token->name);
    free(token);
}

// AST
Node *expr();
Node *assign();
Node *logic();
Node *equality();
Node *comparison();
Node *add_sub();
Node *mul_div();
Node *dot();
Node *sign();
Node *prime();

size_t exe_pos;
Token *find(Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == tokens[exe_pos]->type) return tokens[exe_pos++];
        type = va_arg(ap, Type);
    }
    return NULL;
};

void pnode(Node *node, char *side, int space)
{
	if(node)
	{
		int i = 0;
		while(i < space) i += printf(" ");
		if(side) debug("%s", side);
		debug("node: ");
		if(node->token) ptoken(node->token);
		else debug("(NULL)\n");
		pnode(node->left, "L:", space + 2);
		pnode(node->right, "R:", space + 2);
	}
}

Node *new_node(Token *token)
{
    Node *new = allocate(1, sizeof(Node));
    new->token = token;
    // pnode(new, NULL, 0);
    return new;
}

void free_node(Node *node)
{
	if(node)
	{
		free_node(node->left);
		free_node(node->right);
		free(node);
	}
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = logic();
    Token *token;
    while((token = find(ASSIGN, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = logic();
        left = node;
    }
    return left;
}

Node *logic()
{
	return equality();
}

Node *equality()
{
	return comparison();
}

Node *comparison()
{
	return add_sub();
}

Node *add_sub()
{
	Node *left = mul_div();
    Token *token;
    while((token = find(ADD, SUB, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
    return left;
}

Node *mul_div()
{
	Node *left = dot();
    Token *token;
    while((token = find(MUL, DIV, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = dot();
        left = node;
    }
    return left;
}

Node *dot()
{
	return sign();
}

Node *sign()
{
	return prime();
}

Node *prime()
{
    Token *token;
    Node *node = NULL;
    if((token = find(ID, INT, 0)))
    {
        if(token->declare) // int num
        {
			debug("is declare %d\n", token->declare);
            Token *tmp = find(ID, 0);
			token->name = strdup(tmp->name);
			ptoken(token);
            check(!token, "Expected variable name after [%s] symbol\n", to_string(token->type));
        }
        else if(token->type == ID && token->name && find(LPAR, 0)) // id
        {
            if(strcmp(token->name, "main") == 0)
            {
                debug(RED"found [%s]\n"RESET, token->name);
                // TODO: expect ( and )
                check(!find(RPAR, 0), "expected ) after main declaration");
                check(!find(DOTS, 0), "expected : after main() declaration");
                node = new_node(token);
                token->type = FDEC;
                size_t space = token->space;
                Node *curr = node;
                while(tokens[exe_pos]->space > space && tokens[exe_pos]->type != END)
                {
                    curr->right = new_node(NULL);
                    curr = curr->right;
                    curr->left = expr();
                }
                return node;
            }
            else
            {

            }
        }
        node = new_node(token);
    }
    else if((token = find(LPAR, 0)))
    {
        node = expr();
        check(!find(RPAR, 0), "Expected )\n");
    }
    return node;
}

Node *ast()
{
    if(!AST) return NULL;
    debug(GREEN"===========   AST    ===========\n"RESET);
	Node *head = new_node(NULL);
	Node *curr = head;
	curr->left = expr();
	while(curr->left)
	{
		curr->right = new_node(NULL);
		curr = curr->right;
		curr->left = expr();
	}
    debug(GREEN"=========== PRINT AST ==========\n"RESET);
	curr = head;
	while(curr)
	{
		pnode(curr->left, NULL, 0);
		curr = curr->right;
	}
    return head;
}

// IR
typedef struct
{
    Token *token;
    Token *left;
    Token *right;
} Inst;

Inst **OrgInsts;
Inst **insts;

void clone_insts()
{
    size_t pos = 0;
    size_t len = 100;
    free(insts);
    insts = allocate(len, sizeof(Inst*));

    for(size_t i = 0; OrgInsts[i]; i++)
    {
        if(!OrgInsts[i]->token->remove)
        {
            insts[pos++] = OrgInsts[i];
            if(pos + 1 == len)
            {
                Inst **tmp = allocate((len *= 2), sizeof(Inst*));
                memcpy(tmp, insts, pos * sizeof(Inst*));
                free(insts);
                insts = tmp;
            }
        }
    }
}

void add_inst(Inst *inst)
{
    static size_t pos;
    static size_t len;
    if(len == 0)
    {
        len = 10;
        OrgInsts = allocate(len, sizeof(Inst*));
    }
    else if (pos + 1 == len)
    {
        Inst **tmp = allocate(len * 2, sizeof(Inst *));
        memcpy(tmp, OrgInsts, len * sizeof(Inst *));
        free(OrgInsts);
        OrgInsts = tmp;
        len *= 2;
    }
    OrgInsts[pos++] = inst;
}

int sizeofToken(Token* token)
{
    switch(token->type)
    {
        case INT :  return sizeof(int);
        case FLOAT: return sizeof(float);
        case CHARS: return sizeof(char*);
        case CHAR : return sizeof(char);
        case BOOL : return sizeof(bool);
        // case struct_: return token->size;
        default:
            check(1, "%s %s:%d add this type [%s]\n", FILE, FUNC, LINE, to_string(token->type));
    }
    return 0;
}

size_t ptr;
Inst *new_inst(Token *token)
{
	static size_t reg;
	
	Inst *new = allocate(1, sizeof(Inst));
	new->token = token;
	debug("new inst has ");
	ptoken(new->token);
	token->reg =  ++reg;
	if(token->declare) token->ptr = (ptr += sizeofToken(token));
	add_inst(new);
	return new;
}
// TODO: implement it
bool are_compatible(Token *left, Token *right)
{
	return true;
}

typedef struct
{
    char *name;
    
    Node **functions;
    size_t fpos;
    size_t fsize;

    Token **vars;
    size_t vpos;
    size_t vsize;

} Scoop;

Scoop *Gscoop;
Scoop *scoop;
size_t scoopSize;
size_t scoopPos;

#define ENTER 1
#define EXIT  2

void enter_scoop(char *name)
{
    debug(CYAN"Enter Scoop [%s]\n"RESET, name);
    if(Gscoop == NULL)
    {
        scoopSize = 10;
        Gscoop = allocate(scoopSize, sizeof(Scoop));
    }
    else if(scoopPos + 1 == scoopSize)
    {
        Scoop *tmp = allocate(scoopSize * 2, sizeof(Scoop));
        memcpy(tmp, Gscoop, scoopPos * sizeof(Scoop));
        scoopSize *= 2;
        free(Gscoop);
        Gscoop = tmp;
    }
    scoopPos++;
    Gscoop[scoopPos] = (Scoop){};
    Gscoop[scoopPos].name = name;
    scoop = &Gscoop[scoopPos];
}

void exit_scoop()
{
    debug(CYAN"Exit Scoop [%s]\n"RESET, scoop->name);
    free(scoop->functions);
    free(scoop->vars);
    scoop[scoopPos] = (Scoop){};
    scoop = &Gscoop[--scoopPos];
}

Node *new_function(Node *node)
{
    debug("new func %s in %s scoop\n", node->token->name, scoop->name);
#if 0
    char *builtins[] = {"output", 0};
    for(int i = 0; builtins[i]; i++)
    {
        if(strcmp(node->token->name, builtins[i]) == 0)
        {
            error("%s is a built in function\n", node->token->name);
            exit(1);
        }
    }
#endif
    for(size_t i = 0; i < scoop->fpos; i++)
    {
        Node *func = scoop->functions[i];
        bool cond = strcmp(func->token->name, node->token->name) == 0;
        check(cond, "Redefinition of %s\n", node->token->name);
    }
    if(scoop->functions == NULL)
    {
        scoop->fsize = 10;
        scoop->functions = allocate(scoop->fsize, sizeof(Node*));
    }
    else if(scoop->fpos + 1 == scoop->fsize)
    {
        scoop->fsize *= 2;
        Node **tmp = allocate(scoop->fsize, sizeof(Node*));
        memcpy(tmp, scoop->functions, scoop->fpos * sizeof(Node*));
        free(scoop->functions);
        scoop->functions = tmp;
    }
    scoop->functions[scoop->fpos++] = node;
    return node;
}

Token *generate_ir(Node *node)
{
	Inst *inst = NULL;
	switch(node->token->type)
	{
	case INT:
	{
		inst = new_inst(node->token);
		break;
	}
	case ASSIGN: case ADD: case SUB: case MUL: case DIV:
	{
		Token *left = generate_ir(node->left);
		Token *right = generate_ir(node->right);
		check(!are_compatible(left, right), "incompatible type for %s op\n", to_string(node->token->type));
		inst = new_inst(node->token);
		inst->left = left;
		inst->right = right;
		inst->token->retType = left->type;
		break;
	}
    case FDEC:
    {
        new_function(node);
        enter_scoop(node->token->name);
        inst = new_inst(node->token);
        Node *curr = node->right;
        while(curr)
        {
            generate_ir(curr->left);
            curr = curr->right;
        }
        Token *new = new_token(NULL, 0, 0, END_BLOC, node->token->space);
        new->name = strdup(node->token->name);
        new_inst(new);
        exit_scoop();
        break;
    }
	default: break;
	}
	return inst->token;
}

void print_ir()
{
    debug(GREEN"==========   PRINT IR  =========\n"RESET);
    int i = 0;
    clone_insts();
	for(i = 0; insts[i]; i++)
	{
		Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;
        debug("r%.2d:", curr->reg);
        int k = 0;
        while(k < curr->space)
            k += printf(" ");
		switch(curr->type)
		{
		case ASSIGN: case ADD: case SUB: case MUL: case DIV:
		{
			curr->reg = left->reg;
			debug("[%-6s] r%.2d to ", to_string(curr->type), left->reg);
			if (right->reg)
                debug("r%.2d", right->reg);
            else
            {
                switch (right->type)
                { // TODO: handle the other cases
                case INT:  debug("%lld", right->Int.value); break;
                case BOOL: debug("%s", right->Bool.value ? "True" : "False"); break;
                case FLOAT:debug("%f", right->Float.value); break;
                case CHAR: debug("%c", right->Char.value); break;
                case CHARS:debug("%s", right->Chars.value); break;
                default: check(1, "%s:%d: handle this case\n", FUNC, LINE); break;
                }
            }
            debug("\n");
			break;
		}
		case INT:
		{
			debug("[%-6s] ", to_string(curr->type));
			if (curr->declare)  debug("declare [%s] PTR=[%zu]", curr->name, curr->ptr);
            else if(curr->name) debug("variable %s ", curr->name);
            else if(curr->type == INT) debug("value %lld ", curr->Int.value);
            // else if(curr->type == char_) debug("value %c ", curr->Char.value);
            // else if(curr->type == bool_) debug("value %s ", curr->Bool.value ? "True" : "False");
            // else if(curr->type == float_)
            // {
            //     curr->index = ++float_index;
            //     debug("value %f ", curr->Float.value);
            // }
            // else if(curr->type == chars_)
            // {
            //     curr->index = ++str_index;
            //     debug("value %s in STR%zu ", curr->Chars.value, curr->index);
            // }
            else check(1, "handle this case in generate ir line %d\n", LINE);
			debug("\n");
			break;
		}
        case FDEC:
        {
            debug("[%s] bloc\n", curr->name);
            break;
        }
        case END_BLOC:
        {
            debug("[%s] endbloc\n", curr->name);
            break;
        }
		default: break;
		}
	}
    debug("total instructions [%d]\n", i);
}

bool includes(Type *types, Type type)
{
    for (int i = 0; types[i]; i++) if (types[i] == type) return true;
    return false;
}

bool optimize_ir(int op)
{
	bool optimize = false;
	switch(op)
	{
		case 0:
		{
			debug(CYAN"OP[%d] calculate operations on constants\n"RESET, op);
			for (int i = 0; insts[i]; i++)
			{
				Token *token = insts[i]->token;
	            Token *left = insts[i]->left;
    	        Token *right = insts[i]->right;
				if(includes((Type[]){ADD, SUB, MUL, DIV, 0}, token->type))
				{
					Type types[] = {INT, FLOAT, CHAR, 0};
					if 
                    (includes(types, left->type) && includes(types, right->type) && 
                    !left->name && !right->name)
					{
					switch(left->type)
                    {
                    case INT:
                        switch (token->type)
                        {
                        case ADD: token->Int.value = left->Int.value + right->Int.value; break;
                        case SUB: token->Int.value = left->Int.value - right->Int.value; break;
                        case MUL: token->Int.value = left->Int.value * right->Int.value; break;
                        case DIV: token->Int.value = left->Int.value / right->Int.value; break;
                        default: break;
                        }
                        break;
                    case FLOAT:
                        // left->index = 0;
                        // right->index = 0;
                        // token->index = ++float_index;
                        // switch (token->type)
                        // {
                        // case ADD: token->Float.value = left->Float.value + right->Float.value; break;
                        // case SUB: token->Float.value = left->Float.value - right->Float.value; break;
                        // case MUL: token->Float.value = left->Float.value * right->Float.value; break;
                        // case DIV: token->Float.value = left->Float.value / right->Float.value; break;
                        // default: break;
                        // }
						break;
                    // case chars_:
                    //     switch(token->type)
                    //     {
                    //     case add_: token->Chars.value = strjoin(left, right); break;
                    //     default:
                    //         error("Invalid %s op in chars\n", to_string(token->type)); break;
                    //     }
                    default: check(1, "%s: %d handle this case\n", FUNC, LINE); break;
                    }
					}
					token->type = left->type;
                    left->remove = true;
                    right->remove = true;
                    clone_insts();
                    i = 0;
                    optimize = true;
				}
			}
			break;
		}
        case 1:
        {
			debug(CYAN"OP[%d] calculate operations on constants\n"RESET, op);
            int i = 1;
            while (insts[i])
            {
                Token *token = insts[i]->token;
                Token *left = insts[i]->left;
                Token *right = insts[i]->right;
                if (
                    //  TODO: handle string also here X'D ma fiyach daba
                    includes((Type[]){ADD, SUB, MUL, DIV, 0}, token->type) &&
                    insts[i - 1]->token->type == ADD &&
                    left == insts[i - 1]->token &&
                    !insts[i - 1]->right->name &&
                    !right->name)
                {
                    // debug("%sfound %s\n", RED, RESET);
                    token->remove = true;
                    switch(token->type)
                    {
                    case ADD: insts[i - 1]->right->Int.value += right->Int.value; break;
                    case SUB: insts[i - 1]->right->Int.value -= right->Int.value; break;
                    case MUL: insts[i - 1]->right->Int.value *= right->Int.value; break;
                    case DIV: insts[i - 1]->right->Int.value /= right->Int.value; break;
                    default: break;
                    }
                    // debug("value is %lld\n", insts[i - 1]->right->Int.value);
                    if (insts[i + 1]->left == token) insts[i + 1]->left = insts[i - 1]->token;
                    i = 1;
                    clone_insts();
                    optimize = true;
                    continue;
                }
                i++;
            }
            break;
        }
        case 2:
        {
			debug(CYAN"OP[%d] remove reassigned variables\n"RESET, op);
            for (int i = 0; insts[i]; i++)
            {
                if (insts[i]->token->declare)
                {
                    int j = i + 1;
                    while (insts[j] && insts[j]->token->space == insts[i]->token->space)
                    {
                        debug("line %d: ", LINE);
                        ptoken(insts[j]->token);
                        if (insts[j]->token->type == ASSIGN && insts[j]->left == insts[i]->token)
                        {
                            insts[i]->token->declare = false;
                            insts[i]->token->remove = true;
                            optimize = true;
                            break;
                        }
                        if 
                        (
                            (insts[j]->left && insts[j]->left->reg == insts[i]->token->reg)
                            ||
                            (insts[j]->right && insts[j]->right->reg == insts[i]->token->reg)
                        )
                            break;
                        j++;
                    }
                }
                else if (insts[i]->token->type == ASSIGN)
                {
                    int j = i + 1;
                    while (insts[j] && insts[j]->token->space == insts[i]->token->space)
                    {
                        if(!insts[j]->left || !insts[j]->right || !insts[i]->token)
                        {
                            j++;
                            continue;
                        }
                        if 
                        (
                            insts[j]->token->type == ASSIGN && 
                            insts[j]->left == insts[i]->left
                        )
                        {
                            insts[i]->token->remove = true;
                            optimize = true;
                            break;
                        }
                        // if the variable is used some where
                        else if 
                        (
                            insts[j]->left->reg == insts[i]->token->reg || 
                            insts[j]->right->reg == insts[i]->token->reg
                        )
                            break;
                        j++;
                    }
                }
            }
            break;
        }
		default: break;
	}
	return optimize;
}

void ir(Node *head)
{
    if(!IR) return;
    
    debug(GREEN"========== GENERATE IR =========\n"RESET);
    enter_scoop("");
	Node *curr = head;
	while (curr->left)
	{
		generate_ir(curr->left);
		curr = curr->right;
	}
    exit_scoop();

    int i = 0;
    bool optimized = false;
    while(i < MAX_OPTIMIZATION)
    {
        print_ir();
        optimized = optimize_ir(i++) || optimized;
        if(i == MAX_OPTIMIZATION && optimized)
        {
            optimized = false;
            i = 0;
        }
    }
    print_ir();
}
// ASSEMBLY

// MAIN
int main()
{
    char *input = open_file("file.w");
	// debug("===========    IR    ===========\n");
    Token **tokens = tokenize(input);
	Node *head = ast();
    ir(head);
	free_node(head);
	for (size_t i = 0; OrgInsts && OrgInsts[i]; i++)
        free(OrgInsts[i]);
	for (size_t i = 0; tokens && tokens[i]; i++)
        free_token(tokens[i]);
    free(tokens);
    free(input);
}

/*
15:00 => 16:29
17:20 => 18:20

*/