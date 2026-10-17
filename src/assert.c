
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

    int s0 = s; // the line, without its indentation
    while (s0 > 0 && buff[s0 - 1] != '\n')
        s0--;
    while (buff[s0] == ' ')
        s0++;
    error((Error){
        .s = s,
        .e = e,
        .line = line,
        .msg = format("unclosed %s", quote == '"' ? "string" : "character"),
        .help = format("use %.*s%c", e - s0, buff + s0, quote), // the line, closed
    });
    return false;
}

bool check_char(int s, int e, int line) {
    error((Error){
        .s = s,
        .e = e,
        .line = line,
        .msg = "invalid character",
    });
    return false;
}

// TODO: to be implemented later on
bool check_number_fits(int s, int e, int line) {
    // // clang-format off
    // struct { char *name; long min; long max; } range[END + 1] = {
    //     [I8]  = { "i8",  -128,    127     },
    //     [I32] = { "i32", INT_MIN, INT_MAX },
    // };
    // // clang-format on
    // long value = number->i32.value;
    // if (value >= range[type].min && value <= range[type].max)
    //     return true;
    // error("'%K' doesn't fit in '%s'", number, range[type].name);
    // return false;
    return true;
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

bool check_next(Type type, char *msg) {
    Token *token = peek(0);
    if (token->type == type)
        return true;
    Token *prev = peek(-1);
    Token *err_token = token->line == prev->line ? token : prev;
    error((Error){
        .token = err_token,
        .msg = msg,
    });
    return false;
}