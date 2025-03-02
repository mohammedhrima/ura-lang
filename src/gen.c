#include "./include/header.h"

Token *generate_ir()
{
   Inst *inst = NULL;
   
}

void optimize_ir()
{
   if (obj.is_error) return;
}

void generate_asm()
{
   if (obj.is_error) return;
}

void generate()
{
   if (obj.is_error) return;
   debug(GREEN "========== GENERATE IR =========\n" RESET);
   enter_scoop("");
   Node *curr = obj.head;
   while (curr && !obj.is_error)
   {
      generate_ir(curr->left);
      curr = curr->right;
   }

   optimize_ir();
   generate_asm();
}