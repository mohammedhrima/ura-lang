#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// MACROS
#define EXPAND(type, name) type* name; size_t name##_size; size_t name##_pos;
#define TAB 4

#if defined(__APPLE__)
#include <mach-o/dyld.h>
typedef struct __sFILE *_File;
#elif defined(__linux__)
typedef struct _IO_FILE *_File;
#endif

#define RESIZE(type, elem)                                  \
   if(elem##_size == 0)                                     \
   {                                                        \
      elem##_size = 10;                                     \
      elem = calloc(elem##_size, sizeof(type));             \
   }                                                        \
   else if(elem##_pos + 1 == elem##_size)                   \
   {                                                        \
      type*tmp = calloc(elem##_size *= 2, sizeof(type));    \
      memcpy(tmp, elem, elem##_pos * sizeof(type));         \
      free(elem);                                           \
      elem = tmp;                                           \
   }                                                        \

#define POS __FILE__, __func__, __LINE__

// STRUCTS / ENUMS
typedef enum Type Type;
typedef struct Token Token;
typedef struct Node Node;

enum Type 
{
   ID = 1,
 
   I32,
   
   ASSIGN,
   
   LPAR, RPAR,
   
   ADD, SUB, MUL, DIV,
   
   FDEC,
   
   DOTS,
   
   END,
};

struct Token
{
   Type type;
   Type ret_type;
   bool is_dec; // i32, i64 ... (data type)
   bool init; // initialize variable

   char *name;
   int indent;

   struct { int value; } i32;
};

struct 
{
   bool found_error;
   int  error_count;
   char *filename; // TODO: to be removed
   char *content;
   EXPAND(Token*, tokens);
   size_t exe_pos;

   EXPAND(Node*, nodes);
} ura;

// FUNCTIONS PROTOTYPES
void free_node(Node *node);
Node *expr_node(int min_op);

// PRINTING
char *to_string(Type type)
{
   char*types[END + 1] = {
      [ID] = "ID", [I32] = "I32",
      [LPAR] = "LPAR", [RPAR] = "RPAR", [DOTS] ="DOTS",

      [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV",
      [ASSIGN] = "ASSIGN",
      [FDEC] = "FDEC", 
      [END] = "END",
   };
   char *res = types[type];
   if (res) return res;
   return "(NOT FOUND)";
}

int print(_File f, const char *conv, ...) {
	int res = 0;
   va_list args;
   va_start(args, conv);

   if(f == stderr) fprintf(f, "\033[1m""\033[0;31m""Error: ""\033[0m");
	for (int i = 0; conv[i]; i++) {
		if (conv[i] != '%') {
			res += fprintf(f, "%c", conv[i]);
			continue;
		}
      i++;
      switch (conv[i]) {
			case 'c': res += fprintf(f, "%c", va_arg(args, int)); break;
			case 's': {
				char *str = va_arg(args, char *);
				if (!str) str = "(null)";
            res += fprintf(f, "%s", str);
				break;
			}
			case 'd': {
				int value = va_arg(args, int);
				res += fprintf(f, "%d", value);
				break;
			}
			case 'f': {
				double value = va_arg(args, double);
				res += fprintf(f, "%f", value);
				break;
			}
			case 'k': {
				Token *token = va_arg(args, Token *);
				if (!token) {
					res += fprintf(f, "(null)");
					break;
				}
				res += fprintf(f, "[%s] ", to_string(token->type));
            if (token->is_dec) res += fprintf(f, "is dec ");
            else {
               if (token->name) res += fprintf(f, ": name %s ", token->name);
               switch (token->type) {
                  case I32: {
                     if(!token->name) res += fprintf(f, ": value (%d) ", token->i32.value); 
                     break;
                  }
                  default: break;
               }
               
               if (token->init) res += fprintf(f, "initialize ");
               if (token->ret_type) res += fprintf(f, "ret [%s] ", to_string(token->ret_type));
               res += fprintf(f, "indent: %d ", token->indent);
            }
				break;
			}
			default: {
            res += print(stderr, "invalid format specifier [%c]\n", conv[i]);
            exit(1);
         }
			}
	}
	return res;
}

// TOKENIZE
Token* new_token(Type type, size_t s, size_t e, int indent)
{
   Token *new = calloc(1, sizeof(Token));
   new->type = type;
   new->indent = indent;
   if(e > s)
   {
      switch(type)
      {
         case ID: {
            struct { Type type; char *value; bool is_dec; } keywords[] = {
               {I32, "i32", 1}, {FDEC, "fn", 0}, {0, 0, 0},
            };
            bool found = false;
            for(int i = 0; keywords[i].value; i++)
            {
               char *value = keywords[i].value;
               Type type = keywords[i].type;
               bool is_dec = keywords[i].is_dec;
               size_t len = strlen(value);
               if(strncmp(ura.content + s, value, len) == 0)
               {
                  new->type = type;
                  new->is_dec = is_dec;
                  found = true;
                  break;
               }
            }
            if(found) break;
            new->name = calloc(e - s + 1, sizeof(char));
            strncpy(new->name, ura.content + s, e - s);
            break;
         }
         case I32: {
            new->i32.value = atol(ura.content + s);
            break;
         }
         default: break;
      }
   }
   print(stdout, "new %k\n", new);
   RESIZE(Token*, ura.tokens);
   ura.tokens[ura.tokens_pos++] = new;
   return new;
}

void free_token(Token *token)
{
   if(token->name) free(token->name);
   free(token);
}

void ura_clean()
{
   for(size_t i = 0; ura.tokens && ura.tokens[i]; i++)
      free_token(ura.tokens[i]);
   for(size_t i = 0; i < ura.nodes_pos; i++)
      free_node(ura.nodes[i]);
   free(ura.nodes);
   free(ura.tokens);
   free(ura.content);
}

void tokenize()
{
   if(ura.found_error) return;
   char *content = ura.content;
   int indent = 0;
   for(size_t s = 0; content[s];)
   {
      size_t e = s;
      // skip spaces
      if(isspace(content[s]))
      {
         bool new_line = false;
         if(content[e] == '\n')
         {
            indent = 0;
            new_line = true;
            e++;
         }
         s = e;
         while(content[e] == ' ') e++;
         if (new_line && e - s > 0)
            indent = (((e - s) / TAB ) + ((e - s) % TAB ? 1 : 0)) * TAB;
         s = e;
         continue;
      }
      if(strncmp(content + s, "//", 2) == 0) // comment
      {
         while(content[s] && content[s] != '\n') s++;
         continue;
      }
      if(isalpha(content[s]))
      {
         while(content[s] && (isalnum(content[e]) || strchr("_", content[e]))) e++;
         new_token(ID, s, e, indent);
         s = e;
         continue;
      }
      if(isdigit(content[e]))
      {
         while(isdigit(content[e])) e++;
         new_token(I32, s, e, indent);
         s = e;
         continue;
      }
      // printf(">> [%c]\n", content[e]);
      bool found = false;
      struct {
         Type type;
         char *value;
      } specials[] = {
         {LPAR, "("}, {RPAR, ")"}, {DOTS, ":"}, {ADD, "+"},
         {SUB, "-"}, {MUL, "*"}, {DIV, "/"}, {ASSIGN, "="},
         {0, 0},
      };
      for(int i = 0; specials[i].value; i++)
      {
         char *value = specials[i].value;
         Type type = specials[i].type;
         size_t len = strlen(value);
         // printf(">>> %s\n", value);
         if(strncmp(value, content + s, len) == 0)
         {
            found = true;
            new_token(type, 0, 0, indent);
            s += len;
            break;
         }
      }
      if(found) continue;
      printf("unkown [%c]", content[s]);
      exit(1);
   }   
   new_token(END, 0, 0, 0);
}

struct Node 
{
   Token *token;
   Node *left;
   Node *right;

   EXPAND(Node*, children);
};

Node *new_node(Token *token)
{
   Node *node = calloc(1, sizeof(Node));
   node->token = token;
   printf("new node %s\n", to_string(token->type));
   return node;
}

Token *peek(int index)
{
   return ura.tokens[ura.exe_pos + index];
}

Token *next()
{
   // TODO: protect it
   Token *token = peek(0);
   ura.exe_pos++;
   return token;
}

bool within(int indent)
{
   return peek(0)->type != END && peek(0)->indent >= indent;
}

Node *prime_node()
{
   Node *node = NULL;
   Token *token = next();
   switch(token->type)
   {
      case I32: node = new_node(token); break;
      case ID:
      {
         node = new_node(token);
         if(peek(0)->is_dec)
         {
            node->token->type = next()->type;
            node->token->init = true;
         }
         break;      
      }
      case FDEC:
      {
         node = new_node(token);
         Token *id = peek(0);
         if(id->type != ID)
         {
            printf("Error: expected id after fdec\n");
            ura.found_error = true;
         }
         node->token->name = id->name;
         id->name = NULL;
         // printf(">>>> %s\n", to_string(peek(0)->type));
         next(); // skip id 
         
         // printf(">>>> %s\n", to_string(peek(0)->type));
         if(next()->type != LPAR)
         {
            printf("Error: expected ( after fdec %s\n", to_string(peek(0)->type));
            ura.found_error = true;
            break;
         }
         // printf(">>>> %s\n", to_string(peek(0)->type));
         if(next()->type != RPAR)
         {
            printf("Error: expected ) after fdec %s\n", to_string(peek(0)->type));
            ura.found_error = true;
            break;
         }
         if(peek(0)->type != I32)
         {
            printf("Error: expected data type after fdec %s\n", to_string(peek(0)->type));
            ura.found_error = true;
            break;
         }
         node->token->ret_type = next()->type;
         if(peek(0)->type != DOTS)
         {
            printf("Error: expected : after fdec %s\n", to_string(peek(0)->type));
            ura.found_error = true;
            break;
         }
         next();
         while(within(node->token->indent + TAB))
         {
            RESIZE(Node*, node->children);
            node->children[node->children_pos++] = expr_node(0);
         }
         break;
      }
      default:
         print(stderr, "%s:%d: unexpected token %k\n", __FILE__, __LINE__, token);
         break;
   }
   return node;
}

Node *expr_node(int min_op)
{
   if(ura.found_error) return NULL;
   Node *left = prime_node();
   
   while(true)
   {
      static int ops[END + 1] = {
         [ASSIGN] = 1,
         [ADD] = 10, [SUB] = 10,
         [MUL] = 11, [DIV] = 11,
      };
      Token *curr = ura.tokens[ura.exe_pos];
      if(curr->type == END) break;
      int op = ops[curr->type];
      if(op < min_op || !op) break;
  
      Node *node = new_node(next());
      node->left = left;
      node->right = expr_node(op);
      left = node;
   }
   return left;
}

void pnode(Node *node, int indent)
{
   if(!node || ura.found_error) return;
   for(int i = 0; i < indent; i++) printf(" ");
   print(stdout, "node %k\n", node->token);
   pnode(node->left, indent + TAB);
   pnode(node->right, indent + TAB);
   if(node->children)
   {
      for(int i = 0; i < indent; i++) printf(" ");
      printf("children:\n");
      for(size_t i = 0; i < node->children_pos; i++)
         pnode(node->children[i], indent + TAB);
   }
}

void free_node(Node *node)
{
   if(!node) return;
   free_node(node->left);
   free_node(node->right);
   for(size_t i = 0; i < node->children_pos; i++)
      free_node(node->children[i]);
   free(node->children);
   free(node);
}

char *format(const char *fmt, ...) {
	char  *buf  = NULL;
	size_t size = 0;
	_File   out  = open_memstream(&buf, &size);
	if (!out) {
      print(stderr, "format: open_memstream failed");
      return NULL;
   }

	va_list ap;
	va_start(ap, fmt);
	print(out, fmt, ap);
	va_end(ap);
	fclose(out);
	return buf;
}

void *allocate(int len, int size) {
	void *res = calloc(len, size);
   if(!res) {
      print(stderr, POS, "allocate did failed");
      exit(1);
   }
	return res;
}

char *open_file(char *filename)
{
   if(ura.found_error) return NULL;
   FILE* file = fopen(filename, "r");
   char *content = NULL;
   if(!file) // TODO: refactor it
   {
      ura.found_error = true;
      printf("error opening file %s\n", filename);
      return content;
   }
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   if(size == 0)
   {
      ura.found_error = true;
      printf("error empty file\n");
      return content;
   }
   else
   {
      fseek(file, 0, SEEK_SET);
      content = calloc(size + 1, sizeof(char));
      fread(content, sizeof(char), size, file);
      printf("===============================\n"
             "%s\n"
             "===============================\n"
             ,content);
   }
   fclose(file);
   return content;
}

int main(int argc, char** argv)
{
   if(argc < 2)
   {
      ura.found_error = true;
      printf("error: expected an agument\n");
   }
   ura.content = open_file(argv[1]);
   tokenize();
   while(peek(0)->type != END && !ura.found_error)
   {
      RESIZE(Node*, ura.nodes);
      ura.nodes[ura.nodes_pos++] = expr_node(0); 
   }
   printf("===============================\n");
   for(size_t i = 0; i < ura.nodes_pos; i++)
      pnode(ura.nodes[i], 0);
   printf("===============================\n");

   // clean up
   ura_clean();
}