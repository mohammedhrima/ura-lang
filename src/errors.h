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