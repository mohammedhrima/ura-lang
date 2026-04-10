#include "header.h"

// generate_list_source — generates Ura source code for a list struct.
// elem_type_name: the element type as it appears in Ura source (e.g., "int", "__list_int")
// struct_name:    the struct name to generate (e.g., "__list_int")
// Returns a heap-allocated string that must be freed by the caller.
char *generate_list_source(const char *elem_type_name, const char *struct_name) {
	char *src = allocate(4096, sizeof(char));
	int   pos = 0;

	bool  elem_is_struct =
	    (elem_type_name[0] == '_') || (elem_type_name[0] >= 'A' && elem_type_name[0] <= 'Z');

	// struct __list_T:
	pos += snprintf(src + pos, 4096 - pos, "struct %s:\n", struct_name);

	// fields — data, __len, __cap
	if (elem_is_struct)
		pos += snprintf(src + pos, 4096 - pos, "   data array[%s]\n", elem_type_name);
	else pos += snprintf(src + pos, 4096 - pos, "   data array[%s]\n", elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "   __len int\n");
	pos += snprintf(src + pos, 4096 - pos, "   __cap int\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// operator delete
	pos += snprintf(src + pos, 4096 - pos, "   operator delete() void:\n");
	pos += snprintf(src + pos, 4096 - pos, "      free(self.data as pointer)\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn push(e T) void
	pos += snprintf(src + pos, 4096 - pos, "   fn push(e %s) void:\n", elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "      if self.__len >= self.__cap:\n");
	pos += snprintf(src + pos, 4096 - pos, "         if self.__cap == 0: self.__cap = 8\n");
	pos += snprintf(src + pos, 4096 - pos, "         else: self.__cap *= 2\n");
	pos += snprintf(src + pos, 4096 - pos,
	                "         self.data = realloc(self.data as pointer, self.__cap * sizeof(%s))\n",
	                elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "      self.data[self.__len] = e\n");
	pos += snprintf(src + pos, 4096 - pos, "      self.__len += 1\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn pop() T
	pos += snprintf(src + pos, 4096 - pos, "   fn pop() %s:\n", elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "      self.__len -= 1\n");
	pos += snprintf(src + pos, 4096 - pos, "      return self.data[self.__len]\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn len() int
	pos += snprintf(src + pos, 4096 - pos, "   fn len() int:\n");
	pos += snprintf(src + pos, 4096 - pos, "      return self.__len\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn cap() int
	pos += snprintf(src + pos, 4096 - pos, "   fn cap() int:\n");
	pos += snprintf(src + pos, 4096 - pos, "      return self.__cap\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	return src;
}
