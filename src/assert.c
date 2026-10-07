
// TOKENIZE
bool check_multi_comment(uraFile *file, int s, int e) {
    char *buff = file->buff;
    if (ura_ncmp(buff + e, "*/", 2))
        return true;
    error(file->name, ura.curr_line, s - 2, s, "inclosed comment");
    return false;
}

bool check_quote(uraFile *file, int s, int e) {
    if (file->buff[s] == file->buff[e])
        return true;
    error(file->name, ura.curr_line, s, e, "inclosed string");
    return false;
}

bool check_char(uraFile *file, int s, int e) {
    error(file->name, ura.curr_line, s, e, "invalid character");
    return false;
}

// TODO: to be implemented later on
bool check_number_fits(uraFile *file, int s, int e) {
    // // clang-format off
    // struct { char *name; long min; long max; } range[END + 1] = {
    //     [I8]  = { "i8",  -128,    127     },
    //     [I32] = { "i32", INT_MIN, INT_MAX },
    // };
    // // clang-format on
    // long value = number->i32.value;
    // if (value >= range[type].min && value <= range[type].max)
    //     return true;
    // error(file->name, "'%K' doesn't fit in '%s'", number, range[type].name);
    // return false;
    return true;
}

bool check_unkown_char(uraFile *file, int s, int e) {
    error(file->name, ura.curr_line, s, e, "unkown char");
    return false;
}