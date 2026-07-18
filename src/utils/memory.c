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

void new_source(char *file_name) {
	// TODO: handle imported same file in multiple files
	Source *src   = allocate(1, sizeof(Source));
	src->filename = file_name;
	src->dirname = dirname(file_name);
	debug("open file %s\n", file_name);

	file_name = realpath(file_name, NULL);
	if (!file_name) {
		parse_error(NULL, "Cannot find file '%s'", src->filename);
		return;
	}
	File file = fopen(file_name, "r");
	if (!file) {
		parse_error(NULL, "Cannot open file '%s'", file_name);
		free(file_name);
		return;
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	src->content = allocate((size + 1), sizeof(char));
	fread(src->content, size, sizeof(char), file);
	fclose(file);
	free(file_name);

	resize_array(ura.sources, Source *);
	ura.sources[ura.sources_count++] = src;
	ura.sources_pos++;
}

void exit_source() {
	ura.sources_pos--;
}

bool includes(Type to_find, ...) {
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

void free_token(Token *token) {
   if (!token) return;
   free(token->name);
   free(token->Chars.value);
   free(token->llvm.dims);
   free(token);
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
   for (int i = 0; i < ura.tokens_count; i++)
      free_token(ura.tokens[i]);
   free(ura.tokens);
   for (int i = 0; i < ura.sources_count; i++) {
      free(ura.sources[i]->content);
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
