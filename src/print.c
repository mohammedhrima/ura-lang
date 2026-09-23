#include "header.h"

int print_type(File fp, Node *type) {
    if (!type)
        return fprintf(fp, "void");
    // clang-format off
    switch (type->token->type) {
    case REF:        return fprintf(fp, "&") + print_type(fp, type->left);
    case STRUCT_DEC: return fprintf(fp, "%s", type->token->name);
    case BOOL:       return fprintf(fp, "b1");
    case I8:         return fprintf(fp, "i8");
    case I32:        return fprintf(fp, "i32");
    case NULL_:      return fprintf(fp, "null");
    case ARRAY:      return print_type(fp, type->left) + fprintf(fp, "[]");
    default:         return fprintf(fp, "%s", to_string(type->token->type));
    }
    // clang-format on
}

int print_token(File fp, Token *token) {
    if (!token)
        return fprintf(fp, "(null token)");
    int r = fprintf(fp, "%s", to_string(token->type));
    if (token->name)
        r += fprintf(fp, " name (%s)", token->name);

    if (!token->name && !token->is_type) {
        // clang-format off
        switch (token->type) {
        case I32:   r += fprintf(fp, " value (%ld)", token->i32.value); break;
        case I8:    r += fprintf(fp, " value (%c)", token->i8.value); break;
        case CHARS: r += fprintf(fp, " value (%s)", token->chars.value); break;
        case BOOL: { // clang-format on
            char *b1 = token->b1.value ? "True" : "False";
            r += fprintf(fp, " value (%s)", b1);
            break;
        }
        default:
            break;
        }
    }
    r += fprintf(fp, " space (%ld)", token->space);
    if (token->ret_type)
        r += fprintf(fp, " ret (%s)", to_string(token->ret_type));
    return r;
}

int _print(File fp, const char *fmt, va_list args) {
    int r = 0;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            continue;
        }
        if (!fmt[i + 1])
            break;
        Token *token = NULL;
        int len = 0;
        switch (fmt[++i]) { // clang-format off
        case 's': r += fprintf(fp, "%s", va_arg(args, char *)); break;
        case 'd': r += fprintf(fp, "%d", va_arg(args, int)); break;
        case 'c': r += fprintf(fp, "%c", va_arg(args, int)); break;
        case 'f': r += fprintf(fp, "%f", va_arg(args, double)); break;
        case 'z': r += fprintf(fp, "%zu", va_arg(args, size_t)); i++; break;
        case 'i': r += fprintf(fp, "%*s", va_arg(args, int), ""); break;
        case '%': r += fprintf(fp, "%%"); break;
        case 't': r += fprintf(fp, "%s", to_string(va_arg(args, Type))); break;
        case 'N': r += print_type(fp, va_arg(args, Node *)); break;
        case 'k': r += print_token(fp, va_arg(args, Token *)); break;
        case 'n': r += print_node(fp, va_arg(args, Node *)); break;
        case 'K': {
            token = va_arg(args, Token *);
            len = (int)(token->e - token->s);
            r += fprintf(fp, "%.*s", len, ura.curr_content + token->s);
            break;
        }
        default:  r += fprintf(fp, "%%%c", fmt[i]); break;
        } // clang-format on
    }
    return r;
}

int _eprint(char *file, const char *func, int line, char *fmt, ...) {
    ura.errors_count++;
    va_list args;
    va_start(args, fmt);
    int r = fprintf(stderr, RED("%s:%d %s") " ", file, line, func);
    r += _print(stderr, fmt, args);
    va_end(args);
    return r;
}

int print(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = _print(stdout, fmt, args);
    va_end(args);
    return r;
}

char *format(char *fmt, ...) {
    char *buf = NULL;
    size_t size = 0;
    File out = open_memstream(&buf, &size);
    if (!out) {
        eprint("format: open_memstream failed\n");
        return NULL;
    }

    va_list ap;
    va_start(ap, fmt);
    _print(out, fmt, ap);
    va_end(ap);
    fclose(out);
    char *res = ura_strdup(buf);
    free(buf);
    return res;
}

void print_helper(NodePrint *elems, Node *node, int depth) {
    if (!node)
        return;

    push_back(elems->nodes, node);
    push_back(elems->depths, depth);

    if (!includes(node->token->type, BRK, CNT, 0)) {
        Node *left = node->left;
        bool struct_type = left && left->token->type == STRUCT_DEC;
        if (includes(node->token->type, ID, VAR, REF, 0) && struct_type) {
            push_back(elems->nodes, left);
            push_back(elems->depths, depth + 3);
        } else {
            print_helper(elems, left, depth + 3);
        }
    }
    if (!includes(node->token->type, FN_CALL, 0)) {
        print_helper(elems, node->right, depth + 3);
    }
    for (size_t i = 0; i < node->children_count; i++) {
        print_helper(elems, node->children[i], depth + 3);
    }
}

int still_open(NodePrint *elems, size_t i, int level) {
    for (size_t j = i + 1; j < elems->nodes_count; j++)
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    return 0;
}

int print_node(File fp, Node *node) {
    NodePrint elems = {};
    int r = 0;
    print_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            r += fprintf(fp, "%s", still_open(&elems, i, level) ? "│ " : " ");
        if (depth)
            r += fprintf(fp, "%s", still_open(&elems, i, depth) ? "├─" : "└─");
        r += print_token(fp, elems.nodes[i]->token) + fprintf(fp, "\n");
    }
    return r;
}

void print_nodes(char *text) {
    if (ura.errors_count)
        return;
    print(text);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        print_node(stdout, ura.ast->children[i]);
}
