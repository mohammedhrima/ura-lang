#include "./include/header.h"

void generate_asm(char *name)
{
   if (found_error) return;
   char *outfile = strdup(name);
   outfile[strlen(outfile) - 1] = 's';
   asm_fd = fopen(outfile, "w+");
   check(!asm_fd, "openning %s\n", outfile);
   if (found_error) return;
   free(outfile);
   initialize();
   copy_insts();
   for (int i = 0; insts[i]; i++)
   {
      Token *curr = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      switch (curr->type)
      {
      case INT: case BOOL: case CHARS: case CHAR: case FLOAT: case LONG:
      {
         // if (curr->declare)
         // {
         //    pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); asm_space(curr->space);
         // }
         break;
      }
      case ADD_ASSIGN:
      {
         asm_space(curr->space);
         // TODO: check this
         char *inst = "add";
         pasm("%i%a, %v", inst, left, right);
         if (left->name) {pasm(" ;// add_assign [%s]", left->name); }
         break;
      }
      case ASSIGN:
      {
         // char *inst = left->type == FLOAT ? "movss " : "mov ";
         // debug("LEFT : %k\n", left);
         // debug("RIGHT: %k\n", right);
         // if (right->type == DEFAULT)
         // {
         //    pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); asm_space(curr->space);
         //    break;
         // }
         asm_space(curr->space);
         switch (curr->assign_type)
         {
         case ID_VAL:
         {
            // todo(1, "handle this case");
            // left ptr, creg
            // right value, creg
            if (left->ptr)
            {
               if (right->creg)
               {
                  pasm("%i%a, %r", "mov", left, right); // left ptr, right creg
               }
               else // left ptr, right value
               {
                  if (right->type == CHARS)
                  {
                     pasm("%i%ra, .STR%d[rip]", "lea", left, right->index);
                     asm_space(curr->space);
                     pasm("%i%a, %ra", "mov", left, right);
                  }
                  else
                  {
                     pasm("%i%a, %v", "mov", left, right, left->name);
                  }
               }
            }
            else if (left->creg)
            {
               if (right->creg) // left creg, right creg
               {
                  todo(1, "handle this case", "");
               }
               else // left creg, right value
               {
                  if (right->type == CHARS)
                  {
                     //todo(1, "handle this case", "");
                     // I used %r for this:
                     // chars planet = "Mars"
                     // strcmp(planet, "Earth")
                     pasm("%i%r, .STR%d[rip]", "lea", left, right->index);
                  }
                  else
                  {
                     // todo(1, "handle this case", "");
                     pasm("%i%r, %v", "mov", left, right, left->name);
                  }
               }
            }
            else
            {
               todo(1, "handle this case", "");
            }
            break;
         }
         case ID_ID:
         {
            // left ptr, creg
            // right ptr
            if (left->ptr && right->ptr)
            {
               pasm("%i%ra, %a", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            // functions parameters
            else if (left->creg && right->ptr)
            {
               pasm("%i%r, %a", "mov", left, right);
            }
            else
            {
               todo(1, "handle this case", "");
            }
            break;
         }
         case REF_HOLD_ID:
         {
            // left ptr, creg
            // right ptr
            if (left->creg)
            {
               // todo(1, "handle this case");
               pasm("%i%r, -%d[rbp]", "lea", left,  right->ptr);
            }
            else if (left->ptr)
            {
               // debug("left : %k\n", left);
               // debug("right: %k\n", right);
               // todo(1, "handle this case");
               pasm("%irax, -%d[rbp]", "lea", right->ptr); asm_space(curr->space);
               pasm("%iQWORD PTR -%d[rbp], rax", "mov", left->ptr);
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case ID_REF:
         {
            // left ptr, creg
            // right ptr, creg
            if (left->ptr)
            {
               if (right->ptr)
               {
                  pasm("%i%ra, %a", "mov", right, right); asm_space(curr->space);
                  pasm("%i%ra, %ma", "mov", left, left);  asm_space(curr->space);
                  pasm("%i%a, %ra", "mov", left, left);
               }
               else if (right->creg)
               {
                  todo(1, "handle this case");

               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else if (left->creg)
            {
               if (right->ptr)
               {
                  // todo(1, "handle this case");
                  pasm("%irax, %a", "mov", right); asm_space(curr->space);
                  pasm("%i%r, %ma", "mov", left, left);
               }
               else if (right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case REF_ID:
         {
            // left ptr (has_refrence), creg
            // right ptr
            if (left->ptr && right->ptr)
            {
               pasm("%irax, %a", "mov", left, right); asm_space(curr->space);
               left->is_ref = false;
               pasm("%i%rb, %a", "mov", left, right); asm_space(curr->space);
               left->is_ref = true;
               pasm("%i%ma, %rb", "mov", left, right);
            }
            else if (left->creg && right->ptr)
            {
               todo(1, "handle this case");
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case REF_VAL:
         {
            // left ptr (has_refrence) , creg
            // right creg, value
            if (left->ptr)
            {
               if (right->creg)
               {
                  pasm("%i%a, %r", "mov", left, right);
               }
               else
               {
                  pasm("%irax, %a", "mov", left, left); asm_space(curr->space);
                  pasm("%i%ma, %v", "mov", left,  right);
               }
            }
            else if (left->creg)
            {
               if (right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case REF_REF:
         {
//            todo(1, "handle this case");

            // left has_ref, right has_ref
            // left ptr (has_refrence), creg
            // right ptr, creg
            if (left->ptr)
            {
               if (right->ptr)
               {
                  // todo(1, "handle this case");
                  pasm("%i%ra, %a", "mov", left, right); asm_space(curr->space);
                  left->is_ref = false;
                  pasm("%i%ra, %ma", "mov", left, right); asm_space(curr->space);
                  left->is_ref = true;
                  pasm("%i%rb, %a", "mov", left, left); asm_space(curr->space);
                  right->is_ref = false;
                  pasm("%i%mb, %ra", "mov", left, right);
                  right->is_ref = true;
               }
               else if (right->creg)
               {
                  // todo(1, "handle this case");
                  // pasm("%i%ra, %r", "mov", left, right); asm_space(curr->space);
                  pasm("%i%a, %r", "mov", left, right);
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else if (left->creg)
            {
               if (right->ptr)
               {
                  todo(1, "handle this case");
               }
               else if (right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case REF_HOLD_REF:
         {
            // left ptr (don't have refrence), creg
            // right ptr, creg
            if (left->ptr)
            {
               if (right->ptr)
               {
                  todo(1, "handle this case");
               }
               else if (right->creg)
               {
                  pasm("%iQWORD PTR -%d[rbp], %ri", "mov", left->ptr, right);
                  // todo(1, "handle this case");
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else if (left->creg)
            {
               if (right->ptr)
               {
                  todo(1, "handle this case");
               }
               else if (right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         default:
         {
            todo(1, "handle this case [%d]", curr->assign_type);
            break;
         }
         }
         if (left->name) pasm(" ;// assign [%s]", left->name);
         else if (left->creg) pasm(" ;// assign [%s]", left->creg);
         if (left->is_ref) pasm(" is_ref");
         break;

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
         asm_space(curr->space);
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
            asm_space(curr->space);
            pasm("%i", "cmp");
            if (left->ptr) pasm("%a", left);
            else if (left->creg) pasm("%ra", left);
            else if (!left->creg) pasm("%v", left);

            if (right->ptr) pasm(", %a", right);
            else if (right->creg) pasm(", %ra", right);
            else if (!right->creg) pasm(", %v", right);
            asm_space(curr->space);
            pasm("%i .%s%d", inst, curr->name ? curr->name : "(null)", curr->index);
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
            asm_space(curr->space);
            pasm("%i%ra", inst, curr);
         }
         break;
      }
      case FDEC:
      {
         asm_space(curr->space); pasm("%s:", curr->name);
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
         asm_space(curr->space);
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
      case JE:
      {
         asm_space(curr->space);
         pasm("%ial, 1", "cmp");
         asm_space(curr->space);
         pasm("%i.%s%d", "je", curr->name, curr->index);
         break;
      }
      case JNE:
      {
         asm_space(curr->space);
         pasm("%ial, 1", "cmp");
         asm_space(curr->space);
         pasm("%i.%s%d", "jne", curr->name, curr->index);
         break;
      }
      case JMP:
      {
         asm_space(curr->space);
         pasm("%i.%s%d", "jmp", curr->name, curr->index);
         break;
      }
      case FCALL:
      {
         asm_space(curr->space);
         pasm("%i%s", "call", curr->name);
         break;
      }
      case BLOC:
      {
         asm_space(curr->space);
         pasm(".%s%d:", (curr->name ? curr->name : "(null)"), curr->index);
         break;
      }
      case END_BLOC:
      {
         asm_space(curr->space);
         pasm(".end%s:", curr->name);
         asm_space(curr->space);
         break;
      }
      case STRUCT_CALL: break;
      default: check(1, "handle this case (%s)\n", to_string(curr->type)); break;
      }
   }
   finalize();
}