#pragma once

#define ERR_MISSING_O_ARG "Missing argument for '-o'"

#define ERR_NO_INPUT \
	"No input file (usage: ura <file.ura> [-o out] [-O0..-Oz]" \
	" [-san] [-debug] [-tree])"

#define ERR_CASE_TYPE_MISMATCH \
	"This case value is %s but the subject is %s;" \
	" they must be the same type"

#define ERR_ARG_TYPE_MISMATCH \
	"Argument %d type mismatch in call to '%s'"

#define ERR_ARG_NEEDS_REF \
	"Argument %d to '%s' must be passed by reference (ref x)"

#define ERR_ARG_NO_REF \
	"Argument %d to '%s' does not take a reference"

#define ERR_REF_TYPE_MISMATCH \
	"Reference type mismatch: expected %s, got %s"

#define ERR_BITWISE_NEEDS_INT \
	"Bitwise and shift operators require integer operands"

#define ERR_NOT_AN_ARRAY \
	"Cannot index '%s', it is not an array"

#define ERR_INDEX_NOT_INT \
	"Array index must be an integer, got %s"

#define ERR_FOR_NOT_ITERABLE \
	"'for %s in ...' expects a range (a..b) or an array"

#define ERR_FOR_REF_NEEDS_ARRAY \
	"'for ref' needs an array; a range yields values, not storage"

#define ERR_CANNOT_CAST "Cannot cast %s to %s"

#define ERR_LEN_NOT_ARRAY \
	"'.len' is only valid on an array, not %s"

#define ERR_UNTERM_BLOCK_COMMENT \
	"Unterminated block comment, expected '*/'"

#define ERR_UNTERM_STRING_LITERAL \
	"Unterminated string literal, expected '\"'"

#define ERR_UNTERM_CHAR_LITERAL \
	"Unterminated character literal, expected \"'\""

#define ERR_UNTERM_USE_PATH \
	"Unterminated 'use' path, expected closing '\"'"

#define ERR_UNTERM_LINK_PATH \
	"Unterminated 'link' path, expected closing '\"'"

#define ERR_UNKNOWN_ESCAPE "Unknown escape character: \\%c"

#define ERR_FN_EXPECTED_LPAREN "Expected '(' after function %s"

#define ERR_FN_EXPECTED_RPAREN "Expected ')' after function %s"

#define ERR_FN_EXPECTED_COLON "Expected ':' after function %s"

#define ERR_FN_EXPECTED_PARAM_NAME \
	"Expected parameter name in function %s"

#define ERR_FN_EXPECTED_RET_TYPE \
	"Expected <data type> after function %s"

#define ERR_EXPECTED_BODY_COLON "Expected ':' to open the '%s' body"

#define ERR_CALL_EXPECTED_RPAREN "Expected ')' after %s arguments"

#define ERR_STRUCT_EXPECTED_COLON "Expected ':' after struct %s"

#define ERR_UNKNOWN_TYPE "Unknown type '%s'"

#define ERR_STRUCT_RECURSIVE \
	"Struct %s cannot contain itself by value;" \
	" use an array or a 'ref?' so the size stays finite"

#define ERR_STRUCT_EMPTY \
	"Struct %s must declare at least one field"

#define ERR_STRUCT_DUP_FIELD \
	"Field '%s' is already declared in this struct;" \
	" rename it or remove the duplicate"

#define ERR_REDECL_VARIABLE "Redeclaration of variable '%s'"

#define ERR_REF_MUST_BE_BOUND \
	"A reference must be bound when declared" \
	" (use 'ref?' for an optional reference)"

#define ERR_NEW_EXPECTED_ARRAY_TYPE \
	"Expected an array type after 'new' (e.g. new int[n])"

#define ERR_REDECL_FUNCTION "Redeclaration of function '%s'"

#define ERR_UNDECLARED_VARIABLE "Undeclared variable '%s'"

#define ERR_REF_TO_NON_VARIABLE \
	"Cannot take a reference to a non-variable"

#define ERR_CAPTURE_NOT_ALLOWED \
	"Cannot use '%s' from an enclosing function" \
	" - pass it as a parameter"

#define ERR_WRONG_ARG_COUNT "Wrong number of arguments to '%s'"

#define ERR_REF_NEEDS_VARIABLE \
	"A reference must be bound to a variable (ref x)"

#define ERR_ARRAY_SIZE_NOT_INT "Array size must be an integer"

#define ERR_UNKNOWN_MEMBER "Unknown member '.%s'"

#define ERR_UNKNOWN_FIELD \
	"Struct %s has no field '%s';" \
	" check the spelling or declare it in the struct"

#define ERR_TYPE_HAS_NO_FIELDS \
	"Cannot read '.%s' from %s;" \
	" only a struct has fields"

#define ERR_UNKNOWN_FIELD \
	"Struct %s has no field '%s';" \
	" check the spelling or declare it in the struct"

#define ASSERT_TYPECHECK_NODE "type_check: unhandled node '%s'"

#define ASSERT_CODEGEN_NODE "code_gen: unhandled node '%s'"