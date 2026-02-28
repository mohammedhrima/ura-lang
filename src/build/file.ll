; ModuleID = 'file.ura'
source_filename = "file.ura"
target triple = "arm64-apple-darwin25.3.0"

@llvm.used = appending global [1 x ptr] [ptr @asan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 1, ptr @asan.module_ctor, ptr null }]

declare ptr @fopen(ptr, ptr)

declare ptr @freopen(ptr, ptr, ptr)

declare i32 @fclose(ptr)

declare i32 @fflush(ptr)

declare i32 @fgetc(ptr)

declare i32 @fputc(i32, ptr)

declare i32 @getc(ptr)

declare i32 @putc(i32, ptr)

declare i32 @getchar()

declare i32 @putchar(i8)

declare i32 @ungetc(i32, ptr)

declare ptr @fgets(ptr, i32, ptr)

declare i32 @fputs(ptr, ptr)

declare i32 @puts(ptr)

declare ptr @gets(ptr)

declare i32 @fread(ptr, i32, i32, ptr)

declare i32 @fwrite(ptr, i32, i32, ptr)

declare i32 @write(i32, ptr, i32)

declare i32 @read(i32, ptr, i32)

declare i32 @fseek(ptr, i64, i32)

declare i64 @ftell(ptr)

declare void @rewind(ptr)

declare i32 @fgetpos(ptr, ptr)

declare i32 @fsetpos(ptr, ptr)

declare i32 @feof(ptr)

declare i32 @ferror(ptr)

declare void @clearerr(ptr)

declare void @perror(ptr)

declare i32 @setvbuf(ptr, ptr, i32, i32)

declare void @setbuf(ptr, ptr)

declare i32 @remove(ptr)

declare i32 @rename(ptr, ptr)

declare ptr @tmpfile()

declare ptr @tmpnam(ptr)

declare i32 @fileno(ptr)

declare ptr @fdopen(i32, ptr)

declare i32 @printf(ptr, i32, ...)

declare i32 @fprintf(ptr, ptr)

declare i32 @sprintf(ptr, ptr)

declare i32 @snprintf(ptr, i32, ptr)

declare i32 @dprintf(i32, ptr)

declare i32 @vprintf(ptr, ptr)

declare i32 @vfprintf(ptr, ptr, ptr)

declare i32 @vsprintf(ptr, ptr, ptr)

declare i32 @vsnprintf(ptr, i32, ptr, ptr)

declare i32 @vdprintf(i32, ptr, ptr)

declare i32 @scanf(ptr)

declare i32 @fscanf(ptr, ptr)

declare i32 @sscanf(ptr, ptr)

declare i32 @vscanf(ptr, ptr)

declare i32 @vfscanf(ptr, ptr, ptr)

declare i32 @vsscanf(ptr, ptr, ptr)

declare ptr @malloc(i32)

declare ptr @calloc(i32, i32)

declare ptr @realloc(ptr, i32)

declare void @free(ptr)

declare ptr @aligned_alloc(i32, i32)

declare ptr @signal(i32, ptr)

declare i32 @raise(i32)

declare i32 @kill(i32, i32)

declare i32 @sigaction(i32, ptr, ptr)

declare i32 @sigprocmask(i32, ptr, ptr)

declare i32 @sigsuspend(ptr)

declare void @abort()

declare void @exit(i32)

declare void @quick_exit(i32)

declare void @_Exit(i32)

declare ptr @getenv(ptr)

declare i32 @setenv(ptr, ptr, i32)

declare i32 @unsetenv(ptr)

declare i32 @putenv(ptr)

declare i32 @system(ptr)

declare i32 @rand()

declare void @srand(i32)

declare ptr @bsearch(ptr, ptr, i32, i32, ptr)

declare void @qsort(ptr, i32, i32, ptr)

declare i32 @abs(i32)

declare i64 @labs(i64)

declare ptr @div(i32, i32)

declare ptr @ldiv(i64, i64)

declare i32 @atoi(ptr)

declare i64 @atol(ptr)

declare float @atof(ptr)

declare i64 @strtol(ptr, ptr, i32)

declare i64 @strtoul(ptr, ptr, i32)

declare float @strtof(ptr, ptr)

declare float @strtod(ptr, ptr)

declare i64 @strtoll(ptr, ptr, i32)

declare i64 @strtoull(ptr, ptr, i32)

declare i32 @mblen(ptr, i32)

declare i32 @mbtowc(ptr, ptr, i32)

declare i32 @wctomb(ptr, i32)

declare i32 @mbstowcs(ptr, ptr, i32)

declare i32 @wcstombs(ptr, ptr, i32)

declare i32 @strlen(ptr)

declare i32 @strnlen(ptr, i32)

declare i32 @strcmp(ptr, ptr)

declare i32 @strncmp(ptr, ptr, i32)

declare i32 @strcasecmp(ptr, ptr)

declare i32 @strncasecmp(ptr, ptr, i32)

declare i32 @strcoll(ptr, ptr)

declare i32 @strxfrm(ptr, ptr, i32)

declare ptr @strcpy(ptr, ptr)

declare ptr @strncpy(ptr, ptr, i32)

declare ptr @strdup(ptr)

declare ptr @strndup(ptr, i32)

declare ptr @strcat(ptr, ptr)

declare ptr @strncat(ptr, ptr, i32)

declare ptr @strchr(ptr, i32)

declare ptr @strrchr(ptr, i32)

declare ptr @strstr(ptr, ptr)

declare ptr @strcasestr(ptr, ptr)

declare i32 @strspn(ptr, ptr)

declare i32 @strcspn(ptr, ptr)

declare ptr @strpbrk(ptr, ptr)

declare ptr @strtok(ptr, ptr)

declare ptr @strtok_r(ptr, ptr, ptr)

declare ptr @strerror(i32)

declare i32 @strerror_r(i32, ptr, i32)

declare ptr @memchr(ptr, i32, i32)

declare ptr @memrchr(ptr, i32, i32)

declare i32 @memcmp(ptr, ptr, i32)

declare ptr @memcpy(ptr, ptr, i32)

declare ptr @memmove(ptr, ptr, i32)

declare ptr @memset(ptr, i32, i32)

declare ptr @memccpy(ptr, ptr, i32, i32)

declare i32 @strlcpy(ptr, ptr, i32)

declare i32 @strlcat(ptr, ptr, i32)

declare ptr @strsignal(i32)

declare ptr @memset_s(ptr, i32, i32, i32)

declare i64 @time(ptr)

declare float @difftime(i64, i64)

declare i64 @clock()

declare ptr @gmtime(ptr)

declare ptr @localtime(ptr)

declare i64 @mktime(ptr)

declare i32 @strftime(ptr, i32, ptr, ptr)

declare ptr @asctime(ptr)

declare ptr @ctime(ptr)

declare i32 @clock_gettime(i32, ptr)

declare i32 @clock_settime(i32, ptr)

declare i32 @nanosleep(ptr, ptr)

define i32 @main() !dbg !5 {
entry:
  %str = alloca ptr, align 8, !dbg !8
  store ptr null, ptr %str, align 8, !dbg !8
  %calloc = call ptr @calloc(i32 1, i32 1), !dbg !8
  store ptr %calloc, ptr %str, align 8, !dbg !8
  %c = alloca i8, align 1, !dbg !9
  store i8 0, ptr %c, align 1, !dbg !9
  %str1 = load ptr, ptr %str, align 8, !dbg !9
  %ACCESS = getelementptr i8, ptr %str1, i32 11, !dbg !9
  %str2 = load i8, ptr %ACCESS, align 1, !dbg !9
  store i8 %str2, ptr %c, align 1, !dbg !9
  ret i32 0, !dbg !10
}

declare void @__asan_before_dynamic_init(i64)

declare void @__asan_after_dynamic_init()

declare void @__asan_register_globals(i64, i64)

declare void @__asan_unregister_globals(i64, i64)

declare void @__asan_register_image_globals(i64)

declare void @__asan_unregister_image_globals(i64)

declare void @__asan_register_elf_globals(i64, i64, i64)

declare void @__asan_unregister_elf_globals(i64, i64, i64)

declare void @__asan_init()

; Function Attrs: nounwind
define internal void @asan.module_ctor() #0 {
  call void @__asan_init()
  call void @__asan_version_mismatch_check_v8()
  ret void
}

declare void @__asan_version_mismatch_check_v8()

attributes #0 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.dbg.cu = !{!3}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 4, !"nosanitize_address", i32 1}
!3 = distinct !DICompileUnit(language: DW_LANG_C, file: !4, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!4 = !DIFile(filename: "file.ura", directory: ".")
!5 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !4, line: 4, type: !6, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !3)
!6 = !DISubroutineType(types: !7)
!7 = !{}
!8 = !DILocation(line: 5, scope: !5)
!9 = !DILocation(line: 7, scope: !5)
!10 = !DILocation(line: 0, scope: !5)
