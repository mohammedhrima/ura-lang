#include "./ura_header.h"

bool found_error;

Token **tokens;
int tk_pos;
int tk_len;

Node *global;
int exe_pos;
Inst **OrgInsts;
Inst **insts;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos;

#if defined(__APPLE__)
struct __sFILE *asm_fd;
#elif defined(__linux__)
struct _IO_FILE *asm_fd;
#endif

void parse() 
{
   if (found_error) return;
#if AST
   debug(GREEN BOLD"AST:\n" RESET);
   global = new_node(new_token(ID, -TAB - 1));
   setName(global->token, "ura-scoop");
   enter_scoop(global);
   while (!find(END, 0) && !found_error)
      add_child(global, expr());
   print_ast(global);
#endif
}

void build_ir() 
{
   if (found_error) return;
#if IR
   debug(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      generate_ir(global->children[i]);
   print_ir();
#endif
}

void optimize_ir() 
{
   if (found_error) return;
}
void code_gen() 
{
   if (found_error) return;
}

void compile(char *filename)
{
   tokenize(filename);
   parse();
   build_ir();
   optimize_ir();
   code_gen();
}

int main(int argc, char **argv)
{
   check(argc < 2, "require argument, usage ura <file.ura>");
   compile(argv[1]);
   free_memory();
#if !IR
   debug(RED"NO IR GENERATION\n"RESET);
#endif
#if !ASM
   debug(RED"NO ASSEMBLY GENERATION\n"RESET);
#endif
   return (found_error || !ASM || !IR);
}