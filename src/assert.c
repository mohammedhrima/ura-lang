
// TOKENIZE
bool check_multi_comment(int s, int e, int line) {
    char *buff = ura.scope->token->file.buff;
    if (ura_ncmp(buff + e, "*/", 2))
        return true;
    error((Error){
        .s = s - 2,
        .e = s,
        .line = line,
        .msg = "unclosed comment",
        .help = "add */ to close it",
    });
    return false;
}

bool check_quote(int s, int e, int line) {
    char *buff = ura.scope->token->file.buff;
    char quote = buff[s];
    if (buff[e] == quote)
        return true;

    error((Error){
        .s = s,
        .e = e,
        .line = line,
        .msg = format("unclosed %s", quote == '"' ? "string" : "character"),
        .help = fix_line(e, 0, format("%c", quote)), // the line, closed
    });
    return false;
}

bool check_char(int s, int e, int line, size_t len) {
    char *buff = ura.scope->token->file.buff;
    if (len <= 1)
        return true;
    error((Error){
        .s = s,
        .e = e,
        .line = line,
        .msg = format("a character holds one letter, not %d", (int)len),
        .help = format("use \"%.*s\" for a string", e - s - 2, buff + s + 1),
    });
    return false;
}

bool check_escape(int s, int line) {
    char *buff = ura.scope->token->file.buff;
    error((Error){
        .s = s,
        .e = s + 2,
        .line = line,
        .msg = format("unknown escape '\\%c'", buff[s + 1]),
        .help = "the escapes are \\n \\t \\r \\0 \\\\ \\\" \\'",
    });
    return false;
}

bool check_number_fits(int s, int e, int line) {
    char *buff = ura.scope->token->file.buff;
    long value = strtol(buff + s, NULL, 10); // saturates at LONG_MAX: still too big
    if (value <= INT_MAX)
        return true;
    error((Error){
        .s = s,
        .e = e,
        .line = line,
        .msg = format("%.*s doesn't fit in i32", e - s, buff + s),
        .help = format("i32 goes from %d to %d", INT_MIN, INT_MAX),
    });
    return false;
}

// a line starts with buff[s..e) spaces, space of them counted
bool check_indent(int s, int e, int line, int space) {
    char *buff = ura.scope->token->file.buff;
    if (space % TAB == 0 || ura_ncmp(buff + e, "//", 2)) // comments can go anywhere
        return true;
    error((Error){
        .s = s,
        .e = e,
        .line = line,
        .msg = format("indentation of %d spaces", space),
        .help = format("indent with a multiple of %d spaces", TAB),
    });
    return false;
}

bool check_unknown_char(int s, int e, int line) {
    char *buff = ura.scope->token->file.buff;
    error((Error){
        .s = s,
        .e = e,
        .line = line,
        .msg = format("unknown character '%c'", buff[s]),
        .help = "remove it",
    });
    return false;
}

bool check_next(Type type, char *msg, char *insert) {
    Token *token = peek(0);
    if (token->type == type)
        return true;
    Token *prev = peek(-1);
    Token *err_token = token->line == prev->line ? token : prev;
    error((Error){
        .token = err_token,
        .msg = msg,
        .help = insert ? fix_line(prev->e, 0, insert) : NULL,
    });
    return false;
}

// HELPERS
char *fix_line(int at, int remove, char *put) {
    char *buff = curr_file()->token->file.buff;
    int s = at; // the line, without its indentation
    while (s > 0 && buff[s - 1] != '\n')
        s--;
    while (buff[s] == ' ')
        s++;
    int e = at + remove;
    int end = e;
    while (buff[end] && buff[end] != '\n')
        end++;
    return format("use %.*s%s%.*s", at - s, buff + s, put, end - e, buff + e);
}

// the source text of a token, like "+" or "else"
char *token_text(Token *token) {
    char *buff = curr_file()->token->file.buff;
    return format("%.*s", token->e - token->s, buff + token->s);
}

// the line of a token as written, without its indentation and its ending ':'
char *line_of(Token *token) {
    char *buff = curr_file()->token->file.buff;
    int s = token->s;
    int e = token->s;
    while (s > 0 && buff[s - 1] != '\n')
        s--;
    while (buff[s] == ' ')
        s++;
    while (buff[e] && buff[e] != '\n')
        e++;
    while (e > s && (buff[e - 1] == ':' || buff[e - 1] == ' '))
        e--;
    return format("%.*s", e - s, buff + s);
}

// how many one-letter edits turn a into b (Levenshtein distance)
int edit_distance(char *a, char *b) {
    int n = strlen(a);
    int m = strlen(b);
    int *prev = ura_alloc(m + 1, sizeof(int));
    int *curr = ura_alloc(m + 1, sizeof(int));
    for (int j = 0; j <= m; j++)
        prev[j] = j;
    for (int i = 1; i <= n; i++) {
        curr[0] = i;
        for (int j = 1; j <= m; j++) {
            int best = prev[j - 1] + (a[i - 1] != b[j - 1]); // replace (or keep)
            if (prev[j] + 1 < best)
                best = prev[j] + 1; // remove
            if (curr[j - 1] + 1 < best)
                best = curr[j - 1] + 1; // add
            curr[j] = best;
        }
        int *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    return prev[m];
}

char *did_you_mean(char *name, bool function) {
    char *best = NULL;
    int best_edits = 3;
    for (int i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            char *candidate = NULL;
            if (function && includes(child->token->type, FN_DEC, PROTO, 0))
                candidate = child->token->name;
            if (!function && child->token->type == DECLARE &&
                child->left->token->type == VAR)
                candidate = child->left->token->name;
            if (candidate == NULL)
                continue;
            int edits = edit_distance(name, candidate);
            if (edits < best_edits && edits * 2 <= (int)strlen(name)) {
                best = candidate;
                best_edits = edits;
            }
        }
    }
    return best ? format("did you mean '%s'?", best) : NULL;
}

// ANALYZE
void undeclared_variable(Token *token) {
    char *name = token->name;
    if (ura_cmp(name, "self")) {
        error((Error){ .token = token, .msg = "'self' only exists inside a method" });
        return;
    }
    // declared, but further down: its DECLARE is not analyzed yet
    for (int i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            if (child->token->type != DECLARE ||
                !ura_cmp(child->left->token->name, name))
                continue;
            error((Error){
                .token = token,
                .msg = format("'%s' is used before its declaration, on line %d", name,
                              child->left->token->line + 1),
                .help = "move the declaration above this line",
            });
            return;
        }
    }
    char *help = did_you_mean(name, false);
    error((Error){
        .token = token,
        .msg = format("undeclared variable '%s'", name),
        .help = help ? help : format("declare it first, like: %s i32 = 0", name),
    });
}

// no function takes the arguments of call: say why, as precisely as possible
void no_function(Node *call) {
    char *name = call->token->name;
    Node *same = NULL; // a function with this name, but other parameters
    for (int i = ura.scopes_count; i > 0 && same == NULL; i--) {
        Node *scope = ura.scopes[i - 1];
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            if (includes(child->token->type, FN_DEC, PROTO, 0) &&
                ura_cmp(child->token->name, name))
                same = child;
        }
    }
    if (same) {
        int want = same->right->children_count;
        int got = call->right->children_count;
        char *msg = format("the arguments of '%s' have other types", name);
        if (same->token->is_variadic ? got < want : got != want)
            msg = format("'%s' takes %s%d argument%s, not %d", name,
                         same->token->is_variadic ? "at least " : "", want,
                         want == 1 ? "" : "s", got);
        error(
            (Error){ .token = call->token, .msg = msg, .help = line_of(same->token) });
        return;
    }
    if (find_variable(name)) {
        error((Error){ .token = call->token,
                       .msg = format("'%s' is a variable, not a function", name) });
        return;
    }
    error((Error){
        .token = call->token,
        .msg = format("no function named '%s'", name),
        .help = did_you_mean(name, true),
    });
}