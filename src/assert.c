
// TOKENIZE
bool check_multi_comment(int s, int e, int line) {
    char *buff = ura.scope->token->file.buff;
    if (ura_ncmp(buff + e, "*/", 2))
        return true;
    error(s - 2, s, line, "inclosed comment"); // TODO: fix this ugly shit
    return false;
}

bool check_quote(int s, int e, int line) {
    char *buff = ura.scope->token->file.buff;
    if (buff[s] == buff[e])
        return true;
    error(s, e, line, "inclosed string");
    return false;
}

bool check_char(int s, int e, int line) {
    error(s, e, line, "invalid character");
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

bool check_unkown_char(int s, int e, int line) {
    error(s, e, line, "unkown char");
    return false;
}