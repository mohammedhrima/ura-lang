#include "header.h"

double clock_now() {
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC, &ts);
   return ts.tv_sec + ts.tv_nsec / 1e9;
}

char *signal_name(int sig) {
   switch (sig) {
      case SIGSEGV: return "SIGSEGV";
      case SIGABRT: return "SIGABRT";
      case SIGTRAP: return "SIGTRAP";
      case SIGILL:  return "SIGILL";
      case SIGFPE:  return "SIGFPE";
      case SIGBUS:  return "SIGBUS";
      default:      return "signal";
   }
}

char *format(const char *fmt, ...) {
	char  *buf  = NULL;
	size_t size = 0;
	File   out  = open_memstream(&buf, &size);
	if (CHECK(!out, "format: open_memstream failed")) return NULL;

	va_list ap;
	va_start(ap, fmt);
	_vprint(out, fmt, ap);
	va_end(ap);
	fclose(out);
	return buf;
}

void *allocate(int len, int size) {
	void *res = calloc(len, size);
	TODO(!res, "allocate did failed");
	return res;
}

bool is_dir(char *path) {
	struct stat st;
	return path && stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

char *exe_dir() {
	char buf[PATH_MAX] = {0};
#if defined(__APPLE__)
	uint32_t size = sizeof(buf);
	if (_NSGetExecutablePath(buf, &size) != 0) return NULL;
#elif defined(__linux__)
	ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (n <= 0) return NULL;
	buf[n] = '\0';
#endif
	char *full = realpath(buf, NULL);
	if (!full) return NULL;
	char *copy = strdup(full);
	char *dir  = strdup(dirname(copy));
	free(copy);
	free(full);
	return dir;
}

char *find_ura_lib() {
	char *dir = exe_dir();
	if (!dir) return NULL;
	char *beside = format("%s/ura-lib", dir);
	if (is_dir(beside)) { free(dir); return beside; }
	free(beside);
	char *above = format("%s/../ura-lib", dir);
	free(dir);
	if (is_dir(above)) return above;
	free(above);
	return NULL;
}

void ura_lib_missing(int line, int s, int e) {
	char *env   = getenv("URA_LIB");
	char *dir   = exe_dir();
	char *one   = env ? strdup(env) : strdup("$URA_LIB (not set)");
	char *two   = dir ? format("%s/ura-lib", dir) : strdup("<unknown>");
	char *three = dir ? format("%s/../ura-lib", dir) : strdup("<unknown>");
	tokenize_error(line, s, e, ERR_NO_URA_LIB, one, two, three);
	free(dir);
	free(one);
	free(two);
	free(three);
}

void new_source(char *file_name) {
	char *full = realpath(file_name, NULL);
	if (!full) {
		parse_error(NULL, "Cannot find file '%s'", file_name);
		return;
	}
	for (int i = 0; i < ura.sources_count; i++)
		if (strcmp(ura.sources[i]->pathname, full) == 0) {
			if (ura.sources[i]->loading)
				parse_warn(NULL, "import cycle: '%s' imports itself through"
				                 " '%s'", file_name, ura.current->filename);
			free(full);
			return;
		}
	File file = fopen(full, "r");
	if (!file) {
		parse_error(NULL, "Cannot open file '%s'", file_name);
		free(full);
		return;
	}
	char   *dir   = strdup(full);
	Source *src   = allocate(1, sizeof(Source));
	src->pathname = full;
	src->filename = strdup(file_name);
	src->dirname  = strdup(dirname(dir));
	free(dir);

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	src->content = allocate((size + 1), sizeof(char));
	fread(src->content, size, sizeof(char), file);
	fclose(file);

	resize_array(ura.sources, Source *);
	ura.sources[ura.sources_count++] = src;
}

bool includes(Type to_find, ...) {
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

void free_node(Node *node) {
   if (!node) return;
   if (!includes(node->token->type, BREAK, CONTINUE, 0))
      free_node(node->left);
   free_node(node->right);
   for (int i = 0; i < node->children_count; i++)
      free_node(node->children[i]);
   free(node->children);
   free(node->variables);
   free(node->functions);
   free(node->structs);
   free(node->modules);
   free(node);
}

void free_memory() {
   free_node(ura.head);
   for (int i = 0; i < ura.tokens_count; i++) {
      Token *token = ura.tokens[i];
      if (token) {
         free(token->name);
         free(token->Chars.value);
         free(token->llvm.dims);
         free(token);
      }
   }
   free(ura.tokens);
   for (int i = 0; i < ura.sources_count; i++) {
      free(ura.sources[i]->content);
      free(ura.sources[i]->filename);
      free(ura.sources[i]->dirname);
      free(ura.sources[i]->pathname);
      free(ura.sources[i]);
   }
   free(ura.sources);
   free(ura.scopes);
   if (ura.context) {
      LLVMDisposeBuilder(ura.builder);
      LLVMDisposeModule(ura.module);
      LLVMContextDispose(ura.context);
   }
}
