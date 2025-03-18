#pragma once
#include "header.h"

#define EXIT_STATUS 0
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define BOLD "\e[1m"
#define BLUE "\x1b[34m"
#define RESET "\033[0m"
#define LINE __LINE__
#define FUNC __func__
#define FILE __FILE__

#define TOKENIZE 1
#define TAB 4

#if TOKENIZE
#define AST 1
#endif

#if AST
#define IR 1
#else
#define IR 0
#endif

#define WITH_COMMENTS 1

#if IR
#define BUILTINS 0
#ifndef OPTIMIZE
#define OPTIMIZE 1
#endif

#define ASM 1
#else
#define ASM 0
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#define allocate(len, size) allocate_func(LINE, len, size)
#define check(cond, fmt, ...) check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define to_string(type) to_string_(FILE, LINE, type)
#define new_token(input, s, e, type, space) new_token_(FILE, LINE, input, s, e, type, space)
#define ptoken(token) ptoken_(FILE, LINE, token)
