#include "./include/header.h"

void generate_asm(char *name)
{
   for (int i = 0; insts[i]; i++)
   {
      Token *curr = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      asm_space(curr->space);
      switch (curr->type)
      {
      case INT: case BOOL: case CHARS: case CHAR: case FLOAT:
      {
         if (curr->declare)
         {
            pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); asm_space(curr->space);
         }
         break;
      }
      case ADD_ASSIGN:
      {
         // TODO: check this
         char *inst = "add";
         pasm("%i%a, %v", inst, left, right);
         if (left->name) {pasm(" ;// add_assign [%s]", left->name); asm_space(curr->space);}
         break;
      }
      case ASSIGN:
      {
         // char *inst = left->type == FLOAT ? "movss " : "mov ";
         debug("LEFT : %k\n", left);
         debug("RIGHT: %k\n", right);
#if 1
         if (left->ptr && left->is_ref && !left->has_ref)
         {
            left->has_ref = true;
            if (right->ptr)
            {
               //check(1, "found"); exit(1);
               // int a = 1 ref int b = a
               pasm("%irax, -%d[rbp]", "lea", right->ptr); asm_space(curr->space);
               if (left->ptr) pasm("%iQWORD PTR -%d[rbp], rax", "mov", left->ptr);
               else pasm("%irdi, rax", "mov"); // is function argument
            }
            else if (right->creg) // I added for function arguments that are references
            {
               /*
                  func int m(ref int a):
                     a = 1
                     return 1
                  main():
                     int x = 2
                     m(x)
                     putnbr(x)
               */
               if (left->ptr) pasm("%iQWORD PTR -%d[rbp], %ra", "mov", left->ptr, right);
               else check(1, "handle this case");
            }
            else
            {
               check(1, "handle this case");
            }
         }
         else if (left->ptr && left->is_ref && left->has_ref)
         {
            if (right->ptr)
            {
               //check(1, "found"); exit(1);
               // int a = 1 int c = 2 ref int b = a b = c putnbr(a)
               pasm("%irax, %a", "mov", left); asm_space(curr->space);
               pasm("%i%rd, %a", "mov", left, right); asm_space(curr->space);
               pasm("%i%ma, %rd", "mov", left, right);
            }
            else if (right->creg)
            {
               //check(1, "handle this case");
               // int a = 1 + 2 ref int b = a b = 2 + a putnbr(a)
               pasm("%i%rb, %ra", "mov", left, right); asm_space(curr->space);
               pasm("%irax, %a", "mov", left); asm_space(curr->space);
               pasm("%i%ma, %rb", "mov", left, left);
            }
            // else if(right->type == CHARS)
            // {
            //    check(1, "found"); exit(1);
            // }
            else // right is value
            {
               // int a = 1 ref int b = a b = 3
               pasm("%irax, %a", "mov", left, left); asm_space(curr->space);
               pasm("%i%ma, %v", "mov", left,  right);
               check(1, "found"); exit(1);
            }
         }
         else if (left->ptr && !left->is_ref)
         {
            if (right->is_ref)
            {
               // check(1, "found"); exit(1);
               // int a = 1 + 2 ref int b = a int c = b putnbr(c)
               pasm("%irax, %a", "mov", right); asm_space(curr->space);
               pasm("%i%ra, %ma", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 int b = a
               pasm("%i%ra, %a", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "found"); exit(1);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->type == CHARS)
            {
               // check(1, "found"); exit(1);
               pasm("%i%ra, .STR%d[rip]", "lea", left, right->index); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else // right is value
            {
               // check(1, "found"); exit(1);
               // int a = 1
               pasm("%i%a, %v", "mov", left, right, left->name);
            }
         }
         else if (left->creg) // function parameter
         {
            if (right->is_ref)
            {
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a putnbr(a)
               pasm("%irax, %a", "mov", right); asm_space(curr->space);
               pasm("%i%ra, %ma", "mov", left, right);
            }
            else if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a b = 3 putnbr(a)
               pasm("%i%ra, %a", "mov", left, right); //asm_space(curr->space);
               // pasm("%i%ra, %ra", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "found"); exit(1);
               // putnbr(1 + 2)
               pasm("%i%ra, %ra", "mov", left, right);
            }
            else if (right->type == CHARS)
            {
               // check(1, "found"); exit(1);
               // putstr("cond 1\n")
               pasm("%i%ra, .STR%d[rip]", "lea", left, right->index);
               // pasm("%i%ra, %ra", "mov", left, right);
            }
            else // right is value
            {
               // check(1, "found"); exit(1);
               pasm("%i%ra, %v", "mov", left, right, left->name);
            }
         }
         else
         {
            check(1, "handle this case");
            debug("left:%k, right:%k\n", left, right);
            // check(1, "handle this case left:%k, right:%k", left, right);
         }
         break;
#endif
      }
      case ADD: case SUB: case MUL: case DIV: // TODO: check all math_op operations
      {
         // TODO: use rax for long etc...
         // TODO: something wrong here, fix it
         // Type type = curr->type;
         char *inst = left->type == FLOAT ? "movss" : "mov";
         char *inst2 = NULL;
         switch (curr->type)
         {
         case ADD: inst2 = left->type == FLOAT ? "addss " : "add "; break;
         case SUB: inst2 = left->type == FLOAT ? "subss " : "sub "; break;
         case MUL: inst2 = left->type == FLOAT ? "imulss " : "imul "; break;
         case DIV: inst2 = left->type == FLOAT ? "divss " : "div "; break;
         default: break;
         }
         pasm("%i%ra, ", inst, left);
         if (left->ptr) pasm("%a", left);
         else if (left->creg) pasm("%ra", left) ;
         else pasm("%v", left);
         asm_space(curr->space);
         pasm("%i%ra, ", inst2, right);
         if (right->ptr) pasm("%a", right);
         else if (right->creg) pasm("%ra", right) ;
         else pasm("%v", right);
         // curr->type = getTypeleft->type;
         break;
      }
      case EQUAL: case NOT_EQUAL: case LESS: case MORE: case MORE_EQUAL:
      {
         if (curr->is_cond)
         {
            char *inst = NULL;
            switch (curr->type)
            {
            case EQUAL: inst = "jne"; break;
            case NOT_EQUAL: inst = "je"; break;
            case LESS: inst = "jge"; break;
            case LESS_EQUAL: inst = "jg"; break;
            case MORE: inst = "jle"; break;
            case MORE_EQUAL: inst = "jl"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            asm_space(curr->space); pasm("%i", "cmp");
            if (left->ptr) pasm("%a", left);
            else if (left->creg) pasm("%ra", left);
            else if (!left->creg) pasm("%v", left);

            // asm_space(curr->space);
            if (right->ptr) pasm(", %a", right);
            else if (right->creg) pasm(", %ra", right);
            else if (!right->creg) pasm(", %v", right);
            asm_space(curr->space); pasm("%i .%s%d", inst, curr->name ? curr->name : "(null)", curr->index);
         }
         else
         {
            char *inst = left->type == FLOAT ? "movss" : "mov";
            if (left->ptr) pasm("%i%ra, %a", inst, left, left);
            else if (left->creg /*&& strcmp(left->creg, r->creg)*/) pasm("%i%ra, %ra", inst, left, left);
            else if (!left->creg) pasm("%i%ra, %v", inst, left, left);

            char *reg = NULL;
            switch (left->type)
            {
            case INT: reg = "ebx"; break;
            case FLOAT: reg = "xmm1"; break;
            case CHAR: reg = "bl"; break;
            case BOOL: reg = "ebx"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            asm_space(curr->space);
            if (right->ptr) pasm("%i%s, %a", inst, reg, right);
            else if (right->creg) pasm("%i%s, %ra", inst, reg, right);
            else if (!right->creg) pasm("%i%s, %v", inst, reg, right);

            inst = left->type == FLOAT ? "ucomiss" : "cmp";
            asm_space(curr->space);
            pasm("%i%ra, %s", inst, left, reg);
            switch (curr->type)
            {
            case EQUAL: inst = "sete"; break;
            case NOT_EQUAL: inst = "setne"; break;
            case LESS: inst = "setl"; break;
            case LESS_EQUAL: inst = "setle"; break;
            case MORE: inst = "setg"; break;
            case MORE_EQUAL: inst = "setge"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            curr->retType = BOOL;
            setReg(curr, "al");
            asm_space(curr->space); pasm("%i%ra", inst, curr);
         }
         break;
      }
      case FDEC:
      {
         pasm("%s:", curr->name);
         asm_space(curr->space + TAB); pasm("%irbp", "push");
         asm_space(curr->space + TAB); pasm("%irbp, rsp", "mov");
         asm_space(curr->space + TAB); pasm("%irsp, %d", "sub", (((curr->ptr) + 15) / 16) * 16);
         break;
      }
      case RETURN:
      {
         /*
         TODO: handle reference return
         func int m(ref int a):
            a = 1
            return a
         */
         if (left->ptr) pasm("%i%ra, %a", "mov", left, left);
         else if (left->creg)
         {
            // TODO: check the type
            if (strcmp(left->creg, "eax")) pasm("%i%ra, %a", "mov", left, left);
         }
         else
         {
            switch (left->type)
            {
            case INT: pasm("%i%ra, %ld", "mov", left, left->Int.value); break;
            case VOID: pasm("%ieax, 0", "mov"); break;
            default: check(1, "handle this case [%s]\n", to_string(left->type)); break;
            }
         }
         asm_space(curr->space); pasm("%i", "leave");
         asm_space(curr->space); pasm("%i", "ret");
         break;
      }
      case JE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%d", "je", curr->name, curr->index); break;
      case JNE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%d", "jne", curr->name, curr->index); break;
      case JMP: pasm("%i.%s%d", "jmp", curr->name, curr->index); break;
      case FCALL: pasm("%i%s", "call", curr->name); break;
      case BLOC: pasm(".%s%d:", (curr->name ? curr->name : "(null)"), curr->index); break;
      case END_BLOC: pasm(".end%s:", curr->name); break;
      case STRUCT_CALL: break;
      default: check(1, "handle this case (%s)\n", to_string(curr->type)); break;
      }
   }
}
