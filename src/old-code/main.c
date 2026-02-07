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
int scoop_pos;

char **used_files;
int used_size = 0;
int used_pos = 0;

File asm_fd;

LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMContextRef context;

void parse()
{
   if (found_error) return;
#if AST
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"AST:\n" RESET);
   global = new_node(new_token(ID, -TAB - 1));
   setName(global->token, "ura-scoop");
   enter_scoop(global);
   while (!find(END, 0) && !found_error)
      add_child(global, expr_node());
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
   copy_insts();
   print_ir();
#endif
}

void optimize_ir()
{
   if (found_error) return;
#if OPTIMIZE
   debug(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   while (did_opimize() && !found_error);
   copy_insts();
   print_ir();
#endif
}

void code_gen(char *filename)
{
   if (found_error) return;
#if IR
   debug(GREEN BOLD"GENERATE ASSEMBLY CODE:\n" RESET);
   debug(GREEN BOLD SPLIT RESET);
   copy_insts();
   optimize_ir();
#endif

#if ASM
   char *moduleName = resolve_path(filename);
   init(moduleName);
   for (int i = 0; insts[i] && !found_error; i++) generate_asm(insts[i]);

   int len = strlen(moduleName);
   strcpy(moduleName + len - 3, "ll");
   moduleName[len - 1] = '\0';
   finalize(moduleName);
   free(moduleName);
#endif
}

void compile(char *filename)
{
   tokenize(filename);
   parse();
   build_ir();
   optimize_ir();
   code_gen(filename);
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