#pragma once

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
extern bool             found_error;

extern Token          **tokens;
extern int              tk_pos;
extern int              tk_len;

extern Node            *global;
extern int              exe_pos;

extern Node           **Gscope;
extern Node            *scope;
extern int              scope_len;
extern int              scope_pos;

extern char           **used_files;
extern int              used_len;
extern int              used_pos;

extern Context          context;
extern Module           module;
extern Builder          builder;
extern TypeRef          vd, f32, i1, i8, i16, i32, i64, p8, p32;
extern File             asm_fd;

extern char            *passes;
extern bool             enable_asan;
extern bool             enable_bounds_check;
extern char            *g_argv0;

extern LLVMDIBuilderRef di_builder;
extern LLVMMetadataRef  di_compile_unit;
extern LLVMMetadataRef  di_file;
extern LLVMMetadataRef  di_current_scope;
