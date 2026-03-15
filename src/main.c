#include "ura.h"

bool             found_error;

Token          **tokens;
int              tk_pos;
int              tk_len;

Node            *global;
int              exe_pos;

Node           **Gscope;
Node            *scope;
int              scope_len;
int              scope_pos;

char           **used_files;
int              used_len;
int              used_pos;

Context          context;
Module           module;
Builder          builder;
TypeRef          vd, f32, i1, i8, i16, i32, i64, p8, p32;
File             asm_fd;
bool             enable_bounds_check = false;
char            *passes;
bool             enable_asan;
char            *g_argv0;

char            *link_keys[64];
int              link_key_count;

void add_link_key(char *key)
{
   // deduplicate
   for (int i = 0; i < link_key_count; i++)
      if (strcmp(link_keys[i], key) == 0) return;
   if (link_key_count < 64)
      link_keys[link_key_count++] = strdup(key);
}

LLVMDIBuilderRef di_builder;
LLVMMetadataRef  di_compile_unit;
LLVMMetadataRef  di_file;
LLVMMetadataRef  di_current_scope;

char *compile(char *filename)
{
   debug("%s===========================================\n", RED);
   debug("START COMPILATION\n");
   debug("===========================================\n%s",RESET);
   // reset global state for each file
   found_error = false;
   tk_pos      = 0; exe_pos = 0;
   scope_pos   = 0; scope = NULL;
   global      = NULL;

   tokenize(filename);
   if (found_error) return NULL;

#if AST
   if (found_error) return NULL;
   global = new_node(new_token(ID, -TAB));
   setName(global->token, "ura-scope");
   enter_scope(global);
   while (!find(END, 0) && !found_error)
      add_child(global, expr_node());
   debug("%s===========================================\n", GREEN);
   debug("AFTER PARSING\n");
   debug("===========================================\n%s",RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");
   if (found_error) return NULL;
#endif

#if IR
   if (found_error) return NULL;
   for (int i = 0; !found_error && i < global->cpos; i++)
      gen_ir(global->children[i]);
   if (found_error) return NULL;
   debug("%s===========================================\n", GREEN);
   debug("AFTER IR\n");
   debug("===========================================\n%s",RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");

#if OPTIMIZE
   bool changed = true;
   int  op      = 0;
   while(changed)
   {
      changed = false;
      optimize_ir(global, &changed);
      if(changed) op++;
   }
   debug("%s===========================================\n", GREEN);
   debug("AFTER OPTMIZING\n");
   debug("===========================================\n%s",RESET);
   for (int i = 0; op && !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");

#endif
#endif


#if ASM
   if (found_error) return NULL;

   // build/  directory next to the source file
   char *dir  = dirname(strdup(filename));
   char *base = basename(strdup(filename));
   // strip .ura extension
   char *dot  = strrchr(base, '.');
   if (dot) *dot = '\0';

   // mkdir build/
   char *build_dir = strjoin(dir, "/build", NULL);
   mkdir(build_dir, 0755);

   char *ll_path    = strjoin(build_dir, "/", strjoin(base, ".ll", NULL));

   char *moduleName = resolve_path(filename);
   init(moduleName);
   free(moduleName);

   for (int i = 0; !found_error && i < global->cpos; i++)
      gen_asm(global->children[i]);

   finalize(ll_path);
   free(build_dir);
   return found_error ? NULL : ll_path;
#endif
   return NULL;
}

int main(int argc, char **argv)
{
   g_argv0 = argv[0];
   if(check(argc < 2, "usage: ura <file.ura> [file2.ura ...]"
            "[-O0|-O1|-O2|-O3|-Os|-Oz] [-san] [-o output]"))
      return 1;

   debug("recieved %d files compiling\n", argc - 1);
   for(int i = 1; i < argc; i++)
   {
      debug("%s\n", argv[i]);
   };
   char  *output       = "exe.out";
   char **src_files    = NULL;
   int    src_count    = 0;
   bool   testing_mode = false;
   bool   no_exec      = false;

   for (int i = 1; i < argc; i++)
   {
      if      (strcmp(argv[i], "-O0")     == 0) passes = PASSES_O0;
      else if (strcmp(argv[i], "-O1")     == 0) passes = PASSES_O1;
      else if (strcmp(argv[i], "-O2")     == 0) passes = PASSES_O2;
      else if (strcmp(argv[i], "-O3")     == 0) passes = PASSES_O3;
      else if (strcmp(argv[i], "-Os")     == 0) passes = PASSES_Os;
      else if (strcmp(argv[i], "-Oz")     == 0) passes = PASSES_Oz;
      else if (strcmp(argv[i], "-san")    == 0) enable_asan = true;
      else if (strcmp(argv[i], "-testing") == 0) testing_mode = true;
      else if (strcmp(argv[i], "-no-exec") == 0) no_exec = true;
      else if (strcmp(argv[i], "-o")      == 0)
      {
         check(i + 1 >= argc, "-o requires an argument");
         output = argv[++i];
      }
      else if (argv[i][0] != '-')
      {
         src_files = realloc(src_files, (src_count + 1) * sizeof(char *));
         src_files[src_count++] = argv[i];
      }
      else
         fprintf(stderr, "unknown flag: %s\n", argv[i]);
   }

   bool link_ok = true;
   char final_cmd[8192];
   int  pos     = 0;

   pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, "clang");
   if (enable_asan)
      pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos,
                      " -fsanitize=address,undefined -fno-omit-frame-pointer -g");

   for (int i = 0; i < src_count && link_ok; i++)
   {
      char *ll = compile(src_files[i]);
      #if ASM
      if (!ll)
      {
         link_ok = false;
         break;
      }
      if (testing_mode)
      {
         free(ll);
         free_memory();
         continue;
      }
      if (enable_asan)
         pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", ll);
      else
      {
         char  s[4096];
         snprintf(s, sizeof(s), "%s", ll);
         char *dot = strrchr(s, '.');
         if (dot) strcpy(dot, ".s");

         char llc_cmd[4096];
         snprintf(llc_cmd, sizeof(llc_cmd), "llc \"%s\" -o \"%s\"", ll, s);
         if (system(llc_cmd) != 0)
         {
            fprintf(stderr, RED "llc failed for %s\n" RESET, ll);
            link_ok = false;
            free(ll);
            break;
         }
         pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", s);
      }
      free(ll);
      #endif

      free_memory();
   }

   // append link flags from link directives (e.g. link "@/raylib")
   for (int i = 0; i < link_key_count; i++)
   {
      char env_name[128];
      snprintf(env_name, sizeof(env_name), "URA_LINK_%s", link_keys[i]);
      char *flags = getenv(env_name);
      if (flags)
         pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " %s", flags);
   }

   if (!testing_mode && link_ok && !no_exec)
   {
      pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " -o \"%s\"", output);
      if (system(final_cmd) != 0)
         fprintf(stderr, RED "linking failed\n" RESET);
      else
      {
         char full[4096];
         realpath(output, full);

         if (enable_asan)
         {
            char *asan_file = getenv("ASAN_FILE");
            if (asan_file)
               setenv("ASAN_OPTIONS", "detect_leaks=1", 1),
               setenv("LSAN_OPTIONS", strjoin("suppressions=", asan_file, NULL), 1);
            else
               setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
         }

         fprintf(stderr, GREEN "running %s...\n" RESET, full);

         char run[8192];
         snprintf(run, sizeof(run), "\"%s\"", full);

         int status    = system(run);
         int exit_code = WEXITSTATUS(status);

         fprintf(stderr, exit_code == 0
                 ? GREEN "exit code: %d\n" RESET
                 : RED   "exit code: %d\n" RESET, exit_code);
      }
   }

   free(src_files);
   return found_error || !link_ok;
}
