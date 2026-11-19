#include "./ucg.c"

bool run_test(int p) {
    ucg_clear();
    switch (p) {
    case 0: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            Ucg *b = ucg_var("b", ucg_i32_type());

            ucg_assign(a, ucg_i32_value(10));

            ucg_assign(b, ucg_op(MUL, a, ucg_i32_value(2)));
            ucg_assign(a, ucg_op(ADD, a, b));

            ucg_op(ADD_ASSIGN, a, ucg_i32_value(5));
            ucg_op(SUB_ASSIGN, a, ucg_i32_value(3));
            ucg_op(MUL_ASSIGN, a, ucg_i32_value(2));
            ucg_op(DIV_ASSIGN, a, ucg_i32_value(4));
            ucg_op(MOD_ASSIGN, a, ucg_i32_value(5));

            ucg_ret(ucg_op(ADD, a, b));
        }
        ucg_end(fn_main);
        break;
    }
    case 1: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *x = ucg_var("a", ucg_i32_type());
            Ucg *b = ucg_var("b", ucg_bool_type());
            Ucg *c = ucg_var("c", ucg_char_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    OR,
                    ucg_op(EQ, b, ucg_bool_value(1)),
                    ucg_op(NQ, c, ucg_char_value(0))
                )
            );
            {
                ucg_ret(ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(x);
        }
        ucg_end(fn_main);
        break;
    }
    case 2: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *flag = ucg_var("flag", ucg_bool_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, flag, ucg_bool_value(1)));
            {
                ucg_ret(ucg_i32_value(1));
            }
            Ucg *else_stmt = ucg_else(if_stmt);
            ucg_then(else_stmt, NULL);
            {
                ucg_ret(ucg_i32_value(2));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 3: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            Ucg *big = ucg_var("big", ucg_bool_type());

            ucg_assign(big, ucg_op(LT, a, ucg_i32_value(3)));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_assign(big, ucg_op(LT, a, ucg_i32_value(9)));

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_assign(big, ucg_bool_value(0));

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(100));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(0)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 4: {
        Ucg *is_even = ucg_fn(
            "is_even",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *n = ucg_get_param(is_even, 0);
            ucg_ret(ucg_op(EQ, ucg_op(MOD, n, ucg_i32_value(2)), ucg_i32_value(0)));
        }
        ucg_end(is_even);

        Ucg *both = ucg_fn(
            "both",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("a", ucg_bool_type()),
                ucg_param("b", ucg_bool_type()),
            },
            2
        );
        {
            Ucg *a = ucg_get_param(both, 0);
            Ucg *b = ucg_get_param(both, 1);
            ucg_ret(ucg_op(AND, a, b));
        }
        ucg_end(both);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 5: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *flag = ucg_var("flag", ucg_bool_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, flag, ucg_bool_value(1)));
            {
                ucg_ret(ucg_i32_value(1));
            }
            Ucg *elif_stmt = ucg_elif(if_stmt);
            ucg_then(elif_stmt, ucg_op(EQ, flag, ucg_bool_value(0)));
            {
                ucg_ret(ucg_i32_value(2));
            }
            Ucg *else_stmt = ucg_else(if_stmt);
            ucg_then(else_stmt, NULL);
            {
                ucg_ret(ucg_i32_value(3));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 6: {
        Ucg *is_even = ucg_fn(
            "is_even",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("n1", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *n = ucg_get_param(is_even, 0);
            ucg_ret(ucg_op(EQ, ucg_op(MOD, n, ucg_i32_value(2)), ucg_i32_value(0)));
        }
        ucg_end(is_even);

        Ucg *both = ucg_fn(
            "both",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("a", ucg_bool_type()),
                ucg_param("b", ucg_bool_type()),
            },
            2
        );
        {
            Ucg *a = ucg_get_param(both, 0);
            Ucg *b = ucg_get_param(both, 1);
            ucg_ret(ucg_op(AND, a, b));
        }
        ucg_end(both);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *n0 = ucg_var("n0", ucg_i32_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        is_even,
                        (Ucg *[]){
                            ucg_i32_value(4),
                        },
                        1
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        both,
                        (Ucg *[]){
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(2) }, 1),
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(6) }, 1),
                        },
                        2
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(2));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        both,
                        (Ucg *[]){
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(2) }, 1),
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(7) }, 1),
                        },
                        2
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_ret(n0);
        }
        ucg_end(fn_main);
        break;
    }
    case 7: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_bool_type());
            Ucg *b = ucg_var("b", ucg_bool_type());
            Ucg *n = ucg_var("n", ucg_i32_type());

            ucg_assign(a, ucg_bool_value(0));
            ucg_assign(a, ucg_bool_value(1));
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(OR, a, b));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(5));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(AND, a, b));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(50));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 8: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            ucg_assign(a, ucg_i32_value(0));

            Ucg *loop = ucg_loop();
            ucg_then(loop, ucg_op(LT, a, ucg_i32_value(5)));
            {
                ucg_op(ADD_ASSIGN, a, ucg_i32_value(1));
            }
            ucg_end(loop);

            ucg_ret(a);
        }
        ucg_end(fn_main);
        break;
    }
    case 9: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *i = ucg_var("i", ucg_i32_type());
            ucg_assign(i, ucg_i32_value(0));

            Ucg *going = ucg_var("going", ucg_bool_type());
            ucg_assign(going, ucg_bool_value(1));

            Ucg *loop = ucg_loop();
            ucg_then(loop, ucg_op(EQ, going, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
                ucg_assign(going, ucg_op(LT, i, ucg_i32_value(4)));
            }
            ucg_end(loop);

            ucg_ret(i);
        }
        ucg_end(fn_main);
        break;
    }
    case 10: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_char_type());
            Ucg *b = ucg_var("b", ucg_char_type());

            ucg_assign(a, ucg_char_value('a'));
            ucg_assign(b, ucg_char_value('b'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("<%c%c>\n"),
                    a,
                    b,
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 11: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *c = ucg_var("c", ucg_char_type());
            ucg_assign(c, ucg_char_value('x'));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, c, ucg_char_value('x')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(NQ, c, ucg_char_value('y')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, c, ucg_char_value('z')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 12: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *c = ucg_var("c", ucg_char_type());
            ucg_assign(c, ucg_char_value('m'));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(GT, c, ucg_char_value('a')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(LT, c, ucg_char_value('z')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    AND,
                    ucg_op(GE, c, ucg_char_value('m')),
                    ucg_op(LE, c, ucg_char_value('m'))
                )
            );
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 13: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *nl = ucg_var("nl", ucg_char_type());
            ucg_assign(nl, ucg_char_value('\n'));

            Ucg *tab = ucg_var("tab", ucg_char_type());
            ucg_assign(tab, ucg_char_value('\t'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    nl,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    tab,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c] [%c]\n"),
                    ucg_char_value('\''),
                    ucg_char_value('\\'),
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 14: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *nl = ucg_var("nl", ucg_char_type());
            ucg_assign(nl, ucg_char_value('\n'));

            Ucg *tab = ucg_var("tab", ucg_char_type());
            ucg_assign(tab, ucg_char_value('\t'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    nl,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    tab,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c] [%c]\n"),
                    ucg_char_value('\''),
                    ucg_char_value('\\'),
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 15: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *answer = ucg_fn(
            "answer",
            ucg_char_type(),
            (Ucg *[]){
                ucg_param("yes", ucg_bool_type()),
            },
            1
        );
        {
            Ucg *yes = ucg_get_param(answer, 0);
            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, yes, ucg_bool_value(1)));
            {
                ucg_ret(ucg_char_value('Y'));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_char_value('N'));
        }
        ucg_end(answer);

        Ucg *show = ucg_fn(
            "show",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("c", ucg_char_type()),
            },
            1
        );
        {
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("%c\n"),
                    ucg_get_param(show, 0),
                },
                2
            );
        }
        ucg_end(show);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            ucg_fn_call(
                show,
                (Ucg *[]){
                    ucg_fn_call(answer, (Ucg *[]){ ucg_bool_value(1) }, 1),
                },
                1
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("\n"),
                },
                1
            );
            ucg_fn_call(
                show,
                (Ucg *[]){
                    ucg_fn_call(answer, (Ucg *[]){ ucg_bool_value(0) }, 1),
                },
                1
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("\n"),
                },
                1
            );
        }
        ucg_end(fn_main);
        break;
    }
    case 16: {
        Ucg *fact = ucg_fn(
            "fact",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *stmt = ucg_if();
            {
                ucg_then(stmt, ucg_op(LE, ucg_get_param(fact, 0), ucg_i32_value(1)));
                ucg_ret(ucg_i32_value(1));
            }
            ucg_end(stmt);
            ucg_ret(ucg_op(
                MUL,
                ucg_get_param(fact, 0),
                ucg_fn_call(
                    fact,
                    (Ucg *[]){
                        ucg_op(SUB, ucg_get_param(fact, 0), ucg_i32_value(1)),
                    },
                    1
                )
            ));
        }
        ucg_end(fact);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);

        Ucg *call = ucg_fn_call(
            fact,
            (Ucg *[]){
                ucg_i32_value(5),
            },
            1
        );
        ucg_ret(call);
        ucg_end(fn_main);
        break;
    }
    case 17: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, a, ucg_i32_value(10)));
            Ucg *stmt = ucg_if();
            {
                ucg_then(stmt, ucg_op(EQ, a, ucg_i32_value(3)));
                ucg_brk(loop);
            }
            ucg_end(stmt);
            ucg_op(ADD_ASSIGN, a, ucg_i32_value(1));
        }
        ucg_end(loop);

        ucg_ret(a);
        ucg_end(fn_main);
        break;
    }
    case 18: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *i = ucg_var("i", ucg_i32_type());
        Ucg *sum = ucg_var("sum", ucg_i32_type());
        ucg_assign(i, ucg_i32_value(0));
        ucg_assign(sum, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, i, ucg_i32_value(10)));
            ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
            Ucg *stmt = ucg_if();
            {
                ucg_then(
                    stmt,
                    ucg_op(EQ, ucg_op(MOD, i, ucg_i32_value(2)), ucg_i32_value(0))
                );
                ucg_cnt(loop);
            }
            ucg_end(stmt);
            ucg_op(ADD_ASSIGN, sum, i);
        }
        ucg_end(loop);

        ucg_ret(sum);
        ucg_end(fn_main);
        break;
    }
    case 19: {
        Ucg *count = ucg_global("count", ucg_i32_type());

        Ucg *tick = ucg_fn("tick", ucg_void_type(), NULL, 0);
        ucg_op(ADD_ASSIGN, count, ucg_i32_value(1));
        ucg_end(tick);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *i = ucg_var("i", ucg_i32_type());
        ucg_assign(count, ucg_i32_value(0));
        ucg_assign(i, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, i, ucg_i32_value(4)));
            ucg_fn_call(tick, NULL, 0);
            ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
        }
        ucg_end(loop);

        ucg_ret(count);
        ucg_end(fn_main);
        break;
    }
    case 20: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(5));
        ucg_ret(ucg_op(ADD, ucg_neg(a), ucg_i32_value(12)));
        ucg_end(fn_main);
        break;
    }
    case 21: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(3));

        Ucg *help = ucg_fn("help", ucg_i32_type(), NULL, 0);
        ucg_ret(ucg_i32_value(4));
        ucg_end(help);

        ucg_ret(ucg_op(ADD, a, ucg_fn_call(help, NULL, 0)));
        ucg_end(fn_main);
        break;
    }
    case 22: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_float_type(32));
        Ucg *b = ucg_var("b", ucg_float_type(64));
        ucg_assign(a, ucg_float_value(32, 1.5));
        ucg_assign(b, ucg_float_value(64, 2.25));
        ucg_assign(b, ucg_op(MUL, b, ucg_cast(a, ucg_float_type(64))));
        ucg_fn_call(
            printf,
            (Ucg *[]){
                ucg_string_value("%f %f\n"),
                a,
                b,
            },
            3
        );

        Ucg *stmt = ucg_if();
        {
            ucg_then(stmt, ucg_op(GT, b, ucg_float_value(64, 3.0)));
            ucg_ret(ucg_cast(ucg_neg(b), ucg_i32_type()));
        }
        ucg_end(stmt);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 23: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *rect = ucg_struct("Rect");
        ucg_attrs(
            rect,
            (Ucg *[]){
                ucg_attr("a", point),
                ucg_attr("b", point),
            },
            2
        );

        Ucg *make = ucg_fn(
            "make",
            point,
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        Ucg *p = ucg_var("p", point);
        ucg_assign(ucg_get_attr(p, 0), ucg_get_param(make, 0));
        ucg_ret(p);
        ucg_end(make);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *r = ucg_var("r", rect);
        ucg_assign(ucg_get_attr(ucg_get_attr(r, 0), 0), ucg_i32_value(1));
        ucg_assign(
            ucg_get_attr(r, 1),
            ucg_fn_call(
                make,
                (Ucg *[]){
                    ucg_i32_value(5),
                },
                1
            )
        );
        ucg_assign(ucg_get_attr(ucg_get_attr(r, 1), 1), ucg_i32_value(2));
        Ucg *diff = ucg_op(
            SUB,
            ucg_get_attr(ucg_get_attr(r, 1), 0),
            ucg_get_attr(ucg_get_attr(r, 0), 0)
        );
        ucg_ret(ucg_op(ADD, diff, ucg_get_attr(ucg_get_attr(r, 1), 1)));
        ucg_end(fn_main);
        break;
    }
    case 24: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *p = ucg_var("p", point);
        Ucg *n = ucg_var("n", ucg_i32_type());
        Ucg *sum = ucg_op(ADD, ucg_get_attr(p, 0), ucg_get_attr(p, 1));
        ucg_ret(ucg_op(ADD, sum, n));
        ucg_end(fn_main);
        break;
    }
    case 25: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *p = ucg_var("p", point);
        ucg_assign(
            p,
            ucg_struct_value(
                point,
                (Ucg *[]){
                    ucg_i32_value(10),
                },
                1
            )
        );
        ucg_ret(ucg_op(ADD, ucg_get_attr(p, 0), ucg_get_attr(p, 1)));
        ucg_end(fn_main);
        break;
    }
    case 26: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *swap = ucg_fn(
            "swap",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("x", ucg_ref_type(ucg_i32_type())),
                ucg_param("y", ucg_ref_type(ucg_i32_type())),
            },
            2
        );
        Ucg *x = ucg_get_param(swap, 0);
        Ucg *y = ucg_get_param(swap, 1);
        Ucg *t = ucg_var("t", ucg_i32_type());
        ucg_assign(t, ucg_point_to(x));
        ucg_assign(ucg_point_to(x), ucg_point_to(y));
        ucg_assign(ucg_point_to(y), t);
        ucg_end(swap);

        Ucg *move = ucg_fn(
            "move",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("p", ucg_ref_type(point)),
            },
            1
        );
        ucg_op(
            ADD_ASSIGN,
            ucg_get_attr(ucg_point_to(ucg_get_param(move, 0)), 0),
            ucg_i32_value(5)
        );
        ucg_end(move);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        Ucg *b = ucg_var("b", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(1));
        ucg_assign(b, ucg_i32_value(2));
        ucg_fn_call(
            swap,
            (Ucg *[]){
                ucg_ref(a),
                ucg_ref(b),
            },
            2
        );

        Ucg *p = ucg_var("p", point);
        ucg_fn_call(
            move,
            (Ucg *[]){
                ucg_ref(p),
            },
            1
        );

        Ucg *r = ucg_var("r", ucg_ref_type(ucg_i32_type()));
        ucg_assign(r, ucg_ref(a));
        ucg_assign(ucg_point_to(r), ucg_i32_value(7));

        Ucg *sum = ucg_op(ADD, ucg_op(MUL, a, ucg_i32_value(10)), b);
        ucg_ret(ucg_op(ADD, sum, ucg_get_attr(p, 0)));
        ucg_end(fn_main);
        break;
    }
    case 27: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *hp = ucg_var("hp", ucg_i32_type());
        Ucg *speed = ucg_var("speed", ucg_float_type(32));
        Ucg *alive = ucg_var("alive", ucg_i1_type());
        Ucg *grade = ucg_var("grade", ucg_char_type());
        ucg_assign(hp, ucg_i32_value(100));
        ucg_assign(speed, ucg_float_value(32, 1.5));
        ucg_assign(alive, ucg_i1_value(1));
        ucg_assign(grade, ucg_char_value('S'));
        Ucg *values[] = {
            ucg_string_value("hp: "),     hp,    ucg_string_value(" speed: "), speed,
            ucg_string_value(" alive: "), alive, ucg_string_value(" grade: "), grade,
            ucg_string_value(" 100%\n"),
        };
        ucg_write(ucg_i32_value(1), values, 9);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 28: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *first = ucg_var("first", ucg_string_type());
        Ucg *second = ucg_var("second", ucg_string_type());
        ucg_assign(first, ucg_read(ucg_i32_value(0)));
        ucg_assign(second, ucg_read(ucg_i32_value(0)));
        Ucg *values[] = {
            ucg_string_value("["),   first, ucg_string_value("] ["), second,
            ucg_string_value("]\n"),
        };
        ucg_write(ucg_i32_value(1), values, 5);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 29: {
        Ucg *i32 = ucg_i32_type();
        Ucg *malloc = ucg_proto(
            "malloc",
            ucg_string_type(),
            (Ucg *[]){
                ucg_param("size", i32),
            },
            1
        );
        Ucg *fn_main = ucg_fn("main", i32, NULL, 0);
        Ucg *arr = ucg_var("arr", ucg_ref_type(i32));
        Ucg *size = ucg_op(MUL, ucg_sizeof(i32), ucg_i32_value(4));
        ucg_assign(
            arr,
            ucg_cast(
                ucg_fn_call(
                    malloc,
                    (Ucg *[]){
                        size,
                    },
                    1
                ),
                ucg_ref_type(i32)
            )
        );
        ucg_assign(ucg_access(arr, ucg_i32_value(2)), ucg_i32_value(7));
        ucg_assign(
            ucg_access(arr, ucg_i32_value(0)),
            ucg_op(ADD, ucg_access(arr, ucg_i32_value(2)), ucg_i32_value(1))
        );
        Ucg *values[] = {
            ucg_typeof(ucg_ref_type(i32)),
            ucg_string_value(" "),
            ucg_access(arr, ucg_i32_value(0)),
            ucg_string_value(" "),
            ucg_sizeof(i32),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 6);
        ucg_ret(ucg_access(arr, ucg_i32_value(0)));
        ucg_end(fn_main);
        break;
    }
    case 30: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *small = ucg_var("small", ucg_uint_type(8));
        ucg_assign(small, ucg_int_value(8, 250));
        ucg_op(ADD_ASSIGN, small, ucg_int_value(8, 10));
        Ucg *big = ucg_var("big", ucg_uint_type(32));
        ucg_assign(big, ucg_int_value(32, 4000000000));
        Ucg *half = ucg_var("half", ucg_uint_type(32));
        ucg_assign(half, ucg_op(DIV, big, ucg_int_value(32, 2)));
        Ucg *is_big = ucg_var("is_big", ucg_bool_type());
        ucg_assign(is_big, ucg_op(GT, big, ucg_int_value(32, 100)));
        Ucg *values[] = {
            ucg_typeof(ucg_uint_type(8)),
            ucg_string_value(" "),
            small,
            ucg_string_value(" "),
            big,
            ucg_string_value(" "),
            half,
            ucg_string_value(" "),
            is_big,
            ucg_string_value(" "),
            ucg_cast(big, ucg_float_type(64)),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 12);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 31: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *x = ucg_var("x", ucg_i32_type());
        ucg_assign(x, ucg_i32_value(5));
        Ucg *small = ucg_var("small", ucg_bool_type());
        ucg_assign(small, ucg_not(ucg_op(GT, x, ucg_i32_value(3))));
        Ucg *values[] = {
            small,
            ucg_string_value(" "),
            ucg_not(small),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 4);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 32: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_clean(a);
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_string_value("clean a\n"),
            },
            1
        );
        ucg_clean_end();
        Ucg *i = ucg_var("i", ucg_i32_type());
        ucg_assign(i, ucg_i32_value(0));
        Ucg *loop = ucg_loop();
        ucg_then(loop, ucg_op(LT, i, ucg_i32_value(3)));
        Ucg *b = ucg_var("b", ucg_i32_type());
        ucg_assign(b, i);
        ucg_clean(b);
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_string_value("clean b "),
                b,
                ucg_string_value("\n"),
            },
            3
        );
        ucg_clean_end();
        ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
        Ucg *stop = ucg_if();
        ucg_then(stop, ucg_op(EQ, i, ucg_i32_value(2)));
        ucg_brk(loop);
        ucg_end(stop);
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_string_value("loop\n"),
            },
            1
        );
        ucg_end(loop);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 33: {
        ucg_throws();
        Ucg *boom = ucg_fn(
            "boom",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        Ucg *pick = ucg_if();
        ucg_then(pick, ucg_op(EQ, ucg_get_param(boom, 0), ucg_i32_value(1)));
        ucg_throw(ucg_i32_value(7));
        ucg_then(ucg_else(pick), NULL);
        ucg_throw(ucg_float_value(64, 2.5));
        ucg_end(pick);
        ucg_end(boom);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        for (int n = 1; n <= 2; n++) {
            Ucg *try_stmt = ucg_try();
            Ucg *x = ucg_var("x", ucg_i32_type());
            ucg_clean(x);
            ucg_write(
                ucg_i32_value(1),
                (Ucg *[]){
                    ucg_string_value("clean x\n"),
                },
                1
            );
            ucg_clean_end();
            ucg_fn_call(
                boom,
                (Ucg *[]){
                    ucg_i32_value(n),
                },
                1
            );
            Ucg *as_int = ucg_catch(try_stmt, ucg_i32_type());
            ucg_write(
                ucg_i32_value(1),
                (Ucg *[]){
                    ucg_string_value("caught i32 "),
                    ucg_error(ucg_i32_type()),
                    ucg_string_value("\n"),
                },
                3
            );
            ucg_catch(as_int, ucg_float_type(64));
            ucg_write(
                ucg_i32_value(1),
                (Ucg *[]){
                    ucg_string_value("caught f64 "),
                    ucg_error(ucg_float_type(64)),
                    ucg_string_value("\n"),
                },
                3
            );
            ucg_end_catch(as_int);
        }
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 34: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_ref_type(ucg_i32_type()));
        Ucg *b = ucg_var("b", ucg_ref_type(ucg_i32_type()));
        ucg_assign(a, ucg_alloc(ucg_i32_type(), ucg_i32_value(3)));
        ucg_assign(b, ucg_alloc(ucg_i32_type(), ucg_i32_value(3)));
        for (int i = 0; i < 3; i++)
            ucg_assign(ucg_access(a, ucg_i32_value(i)), ucg_i32_value(10 * (i + 1)));
        ucg_copy(b, a, ucg_i32_value(3));
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_access(b, ucg_i32_value(0)),
                ucg_string_value(" "),
                ucg_access(b, ucg_i32_value(1)),
                ucg_string_value(" "),
                ucg_access(b, ucg_i32_value(2)),
                ucg_string_value("\n"),
            },
            6
        );
        ucg_free(a);
        ucg_free(b);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 35: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *s = ucg_var("s", ucg_string_type());
        ucg_assign(s, ucg_string_array("hello"));
        ucg_assign(ucg_access(s, ucg_i32_value(0)), ucg_char_value('j'));
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                s,
                ucg_string_value("\n"),
            },
            2
        );
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 36: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *first = ucg_access(ucg_argv(), ucg_i32_value(0));
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_argc(),
                ucg_string_value(" "),
                first,
                ucg_string_value("\n"),
            },
            4
        );
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    default: {
        return false;
    }
    }
    return true;
}

bool compile_test(int p) {
    print(CYAN("Test %d: ================================\n"), p);
    ucg_start(format("test%d", p));
    bool r = run_test(p);
    print_ucgs();
    ucg_compile();
    ucg_path("build/out.ll");
    ucg_bin_path(r ? "build/exe.out" : NULL);
    ucg_gen();
    if (r)
        ucg_run_bin();
    ucg_finish();
    return r;
}

int main(int ac, char **av) {
    mkdir2("build");

    if (ac > 1)
        compile_test(atoi(av[1]));
    else
        for (int i = 0; compile_test(i); i++)
            ;
}
