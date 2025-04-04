#include "./include/header.h"

void add_builtins()
{
   struct { char *name; Type *attrs; Type ret;} builtins[] = {
      //----------------------
      // Memory Management
      //----------------------
      {"malloc", (Type[]){LONG, 0}, PTR},
      {"calloc", (Type[]){LONG, LONG, 0}, PTR},
      {"realloc", (Type[]){PTR, LONG, 0}, PTR},
      {"free", (Type[]){PTR, 0}, VOID},

      //----------------------
      // String Operations
      //----------------------
      {"strlen", (Type[]){CHARS, 0}, INT},
      {"strcpy", (Type[]){CHARS, CHARS, 0}, CHARS},
      {"strncpy", (Type[]){CHARS, CHARS, LONG, 0}, CHARS},
      {"strcat", (Type[]){CHARS, CHARS, 0}, CHARS},
      {"strncat", (Type[]){CHARS, CHARS, LONG, 0}, CHARS},
      {"strcmp", (Type[]){CHARS, CHARS, 0}, INT},
      {"strncmp", (Type[]){CHARS, CHARS, LONG, 0}, INT},
      {"strdup", (Type[]){CHARS, 0}, CHARS},
      {"strchr", (Type[]){CHARS, INT, 0}, CHARS},
      {"strstr", (Type[]){CHARS, CHARS, 0}, CHARS},

      //----------------------
      // I/O Operations
      //----------------------
      {"read", (Type[]){INT, CHARS, INT, 0}, INT},
      {"write", (Type[]){INT, CHARS, INT, 0}, INT},
      // {"printf",  (Type[]){CHARS, VARARG, 0}, INT},
      // {"scanf",   (Type[]){CHARS, VARARG, 0}, INT},
      {"putstr", (Type[]){CHARS, 0}, INT},
      {"putnbr", (Type[]){INT, 0}, INT},
      {"puts", (Type[]){CHARS, 0}, INT},
      {"putchar", (Type[]){INT, 0}, INT},
      {"getchar", (Type[]){0}, INT},
      // {"fopen",   (Type[]){CHARS, CHARS, 0}, FILE_PTR},
      // {"fclose",  (Type[]){FILE_PTR, 0},               INT},

      //----------------------
      // Math Functions
      //----------------------
      {"abs", (Type[]){INT, 0}, INT},
      {"labs", (Type[]){LONG, 0}, LONG},
      // {"sqrt",    (Type[]){DOUBLE, 0},                 DOUBLE},
      // {"pow",     (Type[]){DOUBLE, DOUBLE, 0},         DOUBLE},
      // {"sin",     (Type[]){DOUBLE, 0},                 DOUBLE},
      // {"cos",     (Type[]){DOUBLE, 0},                 DOUBLE},

      //----------------------
      // System/Process
      //----------------------
      {"exit", (Type[]){INT, 0}, VOID},
      // {"system", (Type[]){CHARS, 0}, INT},
      // {"atexit",  (Type[]){VOID_FUNC_PTR, 0},          INT},

      //----------------------
      // Utility
      //----------------------
      // {"rand", (Type[]){0}, INT},
      // {"srand",   (Type[]){UNSIGNED_INT, 0},           VOID},
      // {"qsort",   (Type[]){PTR, LONG, LONG, COMPAR_FUNC_PTR, 0}, VOID},

      // Sentinel
      {NULL, NULL, 0}
   };
   for (int i = 0; builtins[i].name; i++)
      create_builtin(builtins[i].name, builtins[i].attrs, builtins[i].ret);
}

void config()
{
   add_builtins();
}