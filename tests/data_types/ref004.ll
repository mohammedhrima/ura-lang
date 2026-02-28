; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define void @ref_assign(ptr %0, ptr %1, i32 %2) {
entry:
  %current = load ptr, ptr %0, align 8
  %is_null = icmp eq ptr %current, null
  br i1 %is_null, label %bind, label %store

bind:                                             ; preds = %entry
  store ptr %1, ptr %0, align 8
  br label %ret

store:                                            ; preds = %entry
  %bound = load ptr, ptr %0, align 8
  call void @llvm.memcpy.p0.p0.i32(ptr %bound, ptr %1, i32 %2, i1 false)
  br label %ret

ret:                                              ; preds = %store, %bind
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i32(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i32, i1 immarg) #0

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

define i32 @main() !dbg !4 {
entry:
  %a = alloca i32, align 4, !dbg !7
  store i32 0, ptr %a, align 4, !dbg !7
  store i32 1, ptr %a, align 4, !dbg !7
  %b = alloca i32, align 4, !dbg !7
  store i32 0, ptr %b, align 4, !dbg !7
  store i32 2, ptr %b, align 4, !dbg !7
  %choice = alloca i32, align 4, !dbg !7
  store i32 0, ptr %choice, align 4, !dbg !7
  store i32 1, ptr %choice, align 4, !dbg !7
  %r = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %r, align 8, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %choice1 = load i32, ptr %choice, align 4, !dbg !7
  %EQ = icmp eq i32 %choice1, 1, !dbg !7
  br i1 %EQ, label %if.then, label %if.else, !dbg !7

if.end:                                           ; preds = %if.else, %if.then
  %ref_ptr = load ptr, ptr %r, align 8, !dbg !7
  store i32 100, ptr %ref_ptr, align 4, !dbg !7
  %a2 = load i32, ptr %a, align 4, !dbg !8
  %b3 = load i32, ptr %b, align 4, !dbg !8
  %ADD = add i32 %a2, %b3, !dbg !8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, i32 %ADD), !dbg !8
  ret i32 0, !dbg !8

if.then:                                          ; preds = %if.start
  store ptr %a, ptr %r, align 8, !dbg !7
  br label %if.end, !dbg !7

if.else:                                          ; preds = %if.start
  store ptr %b, ptr %r, align 8, !dbg !7
  br label %if.end, !dbg !7
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 16, scope: !4)
