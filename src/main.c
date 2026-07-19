#include "header.h"

#include "utils/memory.c"
#include "utils/diagnostics.c"
#include "frontend/lexer.c"
#include "frontend/parser.c"
#include "backend/analyze.c"
#include "backend/typecheck.c"
#include "backend/codegen.c"

UraGlobal ura;

void parse_arguments(int argc, char **argv)
{
   ura.lib = getenv("URA_LIB");
   if (!is_dir(ura.lib)) ura.lib = find_ura_lib();
   ura.output = "exe.out";
   
   for (int i = 1; i < argc && !ura.error_count; i++) {
      char *arg = argv[i];
#define MATCH(name, field, val) if (strcmp(arg, name) == 0) { field = val; continue; }
      MATCH("-debug", ura.enable_debug, true);
      MATCH("-exec",  ura.enable_exec,  true);
      MATCH("-san",   ura.enable_san,   true);
      MATCH("-testing", ura.no_color,   true);
      MATCH("-tree",  ura.enable_tree,  true);
      MATCH("-ll",    ura.enable_ll,    true);
      MATCH("-O0", ura.flags, PASSES_O0);   
      MATCH("-O1", ura.flags, PASSES_O1);
      MATCH("-O2", ura.flags, PASSES_O2);   
      MATCH("-O3", ura.flags, PASSES_O3);
      MATCH("-Os", ura.flags, PASSES_Os);   
      MATCH("-Oz", ura.flags, PASSES_Oz);
#undef MATCH
      if (strcmp(arg, "-o") == 0) {
         if (i + 1 >= argc) {
            parse_error(NULL, ERR_MISSING_O_ARG);
            return;
         }
         ura.output = argv[++i];
      } else if (arg[0] == '-') {
         parse_error(NULL, "Unknown flag '%s'", arg);
      } else {
         size_t n = strlen(arg);
         bool is_ura = n > 4 && strcmp(arg + n - 4, ".ura") == 0;
         if (!is_ura) parse_error(NULL, "Invalid file '%s'", arg);
         else new_source(arg);
      }
   }
   if (ura.error_count) return;
   if (!ura.sources) parse_error(NULL, ERR_NO_INPUT);
}

void load_common() {
   if (!ura.lib) {
      parse_error(NULL, ERR_NO_STDLIB);
      return;
   }
   char *path   = format("%s/common.ura", ura.lib);
   int   before = ura.sources_count;
   new_source(path);
   free(path);
   if (ura.error_count || ura.sources_count == before) return;
   ura.calling_use++;
   tokenize(0);
   ura.calling_use--;
}

void tokenize(int default_indent) {
   if (ura.error_count || !ura.sources) return;
   Source *prev = ura.current;
   ura.current  = ura.sources[ura.sources_count - 1];
   ura.current->loading = true;
   if (!ura.calling_use) load_common();
   char *content = ura.current->content;
   int line = 1;
   int indent = default_indent;
   
   int s = 0;
   int i = 0;
   while (content && content[i] && !ura.error_count) {
      s = i;
      char c = content[i];
      if (lex_spaces(content, &i, &line, &indent, default_indent)) continue;
      if (lex_multi_comment(content, &i, &line)) continue;
      if (lex_comment(content, &i)) continue;
      if (lex_chars(content, &i, line, indent)) continue;
      if (lex_char(content, &i, line, indent)) continue;
      if (lex_number(content, &i, line, indent)) continue;
      if (lex_identifier(content, &i, line, indent, default_indent)) continue;
      if (lex_symbol(content, &i, line, &indent)) continue;
      tokenize_error(line, i, i + 1, "Unexpected character '%c'", c);
   }
   if (!ura.calling_use)
   {
      parse_token(line, s, i, END, -1);
      for(int i = 0; i < ura.tokens_count && ura.enable_debug; i++)
         debug("token %k\n", ura.tokens[i]);
   }
   ura.current->loading = false;
   ura.current = prev;
}

void generate_ast() {
   if(ura.error_count) return;
   Node *head = new_node(new_token(ID, -TAB));
   ura.head = head;
   enter_scope(head);
   while (!find(END, 0)) {
      int before = ura.error_count;
      Node *child = expr_node(0);
      if (ura.error_count > before) {
         parser_recover(0);
         if (ura.error_count > ura.max_errors) break;
         continue;
      }
      resize_array(head->children, Node *);
      head->children[head->children_count++] = child;
      // TODO: only function declarations and
      // struct declaration are allowed here
   }
   if (ura.error_count > 0 || !ura.enable_debug) return;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER PARSING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; i < head->children_count; i++)
		pnode(head->children[i], "");
}

void generate_ir() {
   if(ura.error_count || !ura.head) return;
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == STRUCT_DEF)
         declare_struct(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == FDEC)
         declare_function(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++) {
      Token *global = global_decl(ura.head->children[i]);
      if (!global) continue;
      global->is_global = true;
      declare_variable(global);
   }
   for (int i = 0; i < ura.head->children_count; i++)
      analyze(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      type_check(ura.head->children[i]);
   if (!ura.enable_tree) return;
   print_ast(ura.head);
}

void generate_asm() {
   if (ura.error_count || !ura.head) return;
   setup_paths(ura.sources[0]->filename);
   init_module(ura.output);
   for (int i = 0; i < ura.head->children_count; i++) {
      Token *global = global_decl(ura.head->children[i]);
      if (global) llvm_global(global);
   }
   for (int i = 0; i < ura.head->children_count; i++)
      if (!global_decl(ura.head->children[i]))
         code_gen(ura.head->children[i]);
   finalize_module(ura.ll_path);
}

void compile_executable() {
   if (ura.error_count || !ura.head) return;

   fprintf(stderr, CYAN("%-9s") " " BLUE("%s (%s)") "\n", 
         "Compiling", ura.base, ura.sources[0]->filename);
   char *cc  = ura.enable_san ? "/usr/bin/clang" : "clang";
   char *san = ura.enable_san ? " -fsanitize=address,undefined -fno-omit-frame-pointer -g" : "";
   char *cmd = format("%s%s %s -o %s 2>/dev/null", cc, san, ura.ll_path, ura.output);
   system(cmd);
   free(cmd);
   if (!ura.enable_exec) return;
   char *opt = ura.flags ? "optimized" : "unoptimized";
   char *tag = ura.enable_san ? " + sanitized" : "";
   fprintf(stderr, CYAN("%-9s") " \033[2m[%s%s]\033[0m in %.2fs\n",
           "Finished", opt, tag, clock_now() - ura.time_start);
}

void run_executable() {
   if (!ura.enable_exec || ura.error_count || !ura.head) return;
   char  *run = strchr(ura.output, '/') ? ura.output : format("./%s", ura.output);
   fprintf(stderr, CYAN("%-9s") " " BLUE("%s") "\n", "Running", run);
   double start = clock_now();
   pid_t  pid = fork();
   if (pid == 0) {
      execl(run, run, NULL);
      _exit(127);
   }
   if (pid < 0) return;
   int status = 0;
   waitpid(pid, &status, 0);
   double exec = clock_now() - start;
   if (WIFSIGNALED(status))
      fprintf(stderr, RED("%-9s") " %s (signal %d) in %.2fs\n",
              "\nCrashed", signal_name(WTERMSIG(status)), WTERMSIG(status), exec);
   else
      fprintf(stderr, CYAN("%-9s") " with code %d in %.2fs\n",
              "\nExited", WEXITSTATUS(status), exec);
}

int main(int argc, char**argv) {
   ura.time_start = clock_now();
   ura.max_errors = 20;
   ura.enable_debug = false;
   parse_arguments(argc, argv);
#if TOKENIZE
   tokenize(0);
#endif
#if AST
   generate_ast();
#endif
#if IR
   generate_ir();
#endif
#if ASM
   generate_asm();
   compile_executable();
   run_executable();
#endif
   free_memory();
   return ura.error_count != 0;
}
