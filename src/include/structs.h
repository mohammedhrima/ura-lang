#pragma once
#include "header.h"

typedef enum
{
   START = 1,
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
   EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
   ADD, SUB, MUL, DIV, MOD,
   AND, OR,
   RPAR, LPAR, COMA, DOT, DOTS,
   RETURN,
   IF, ELIF, ELSE, WHILE,
   FDEC, FCALL,
   VOID, INT, CHARS, CHAR, BOOL, FLOAT,
   STRUCT, ID, BLOC, END_BLOC,
   JNE, JE, JMP,
   PUSH, POP,
   END
} Type;

typedef struct
{
   Type type;
   Type retType;
   char *name;
   size_t ptr;
   bool declare;
   size_t space;
   bool remove;
   size_t reg;
   char *creg;
   size_t index;
   bool isCond;
   // size_t rsp;
   // bool isarg;

   struct
   {
      // integer
      struct
      {
         long long value;
         int power;
         struct Int *next;
      } Int;
      // float
      struct
      {
         float value;
      } Float;
      // boolean
      struct
      {
         bool value;
         char c;
      } Bool;
      // chars
      struct
      {
         char *value;
      } Chars;
      // char
      struct
      {
         char value;
      } Char;
   };
} Token;

typedef struct Node
{
   struct Node *left;
   struct Node *right;
   Token *token;
} Node;

typedef struct
{
   char *value;
   Type type;
} Specials;

typedef struct
{
   char *name;

   Node **functions;
   size_t fpos;
   size_t fsize;

   Token **vars;
   size_t vpos;
   size_t vsize;

} Scoop;

typedef struct
{
   Token *token;
   Token *left;
   Token *right;
} Inst;

