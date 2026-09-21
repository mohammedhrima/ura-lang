#include "header.h"

void diag_flush(void) {
    if (ura.error_file) {
        char *file = ura.error_file;
        int line = ura.error_line;
        const char *func = ura.error_func;
        fprintf(stderr, DIM("raised at %s:%d %s") "\n", file, line, func);
    }
    ura.error_file = NULL;
}

void _error_at(char *file, const char *func, int line, Token *token, char *fmt, ...) {
    diag_flush();
    ura.errors_count++;
    ura.error_file = file;
    ura.error_func = func;
    ura.error_line = line;

    char *content = ura.curr_content;
    size_t s = token->s;
    while (s > 0 && content[s - 1] != '\n')
        s--;
    size_t e = token->s;
    while (content[e] && content[e] != '\n')
        e++;
    size_t column = token->s - s + 1;
    char *name = ura.curr_file->name;

    fprintf(stderr, RED("error:") " %s:%zu:%zu ", name, token->line, column);
    va_list args;
    va_start(args, fmt);
    _print(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    int width = token->space;
    fprintf(stderr, "%*s |\n", width, "");
    fprintf(stderr, "%zu | %.*s\n", token->line, (int)(e - s), content + s);
    fprintf(stderr, "%*s | %*s", width, "", (int)(token->s - s), "");
    for (size_t i = token->s; i < token->e || i == token->s; i++)
        fputc('^', stderr);
    fprintf(stderr, "\n");
}

void help(char *fmt, ...) {
    fprintf(stderr, CYAN("help:") " ");
    va_list args;
    va_start(args, fmt);
    _print(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void report_bad_call(Node *call) {
    char *name = call->token->name;
    Node *fn = NULL;
    size_t count = 0;
    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *curr = scope->children[j];
            if (!includes(curr->token->type, FN_DEC, PROTO, 0))
                continue;
            if (strcmp(curr->token->name, name) != 0)
                continue;
            fn = curr;
            count++;
        }
    }
    if (count == 0) {
        error_at(call->token, "function '%s' not found", name);
        return;
    }
    error_at(call->token, "no overload of '%s' takes these arguments", name);
    if (count > 1) {
        help("%zu functions are named '%s', none fits these arguments", count, name);
        return;
    }

    size_t want = fn->left->children_count;
    size_t got = call->left->children_count;
    if (fn->token->is_variadic ? got < want : got != want) {
        help("'%s' takes %zu arguments, got %zu", name, want, got);
        return;
    }
    for (size_t i = 0; i < want; i++) {
        Node *var = fn->left->children[i]->left;
        Node *arg = call->left->children[i];
        Node *expected = var->left;
        Node *actual = type_of(arg);
        if (same_type(expected, actual))
            continue;
        Token *given = arg->token;
        help("`%S` is `%T`, but `%S` wants `%T`", given, actual, var->token, expected);
        if (expected->token->type == REF && same_type(expected->left, actual))
            help("pass its address: `&%S`", given);
        return;
    }
}
