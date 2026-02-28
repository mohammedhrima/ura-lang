; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@STR1 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

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

define i32 @strlen(ptr %str) !dbg !4 {
entry:
  %str1 = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %str1, align 8, !dbg !7
  store ptr %str, ptr %str1, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i32 0, ptr %i, align 4, !dbg !7
  store i32 0, ptr %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %str2 = load ptr, ptr %str1, align 8, !dbg !8
  %i3 = load i32, ptr %i, align 4, !dbg !8
  %ACCESS = getelementptr i8, ptr %str2, i32 %i3, !dbg !8
  %ACC = load i8, ptr %ACCESS, align 1, !dbg !8
  %NEQ = icmp ne i8 %ACC, 0, !dbg !8
  br i1 %NEQ, label %while.then, label %while.end, !dbg !8

while.then:                                       ; preds = %while.start
  %current = load i32, ptr %i, align 4, !dbg !8
  %ADD = add i32 %current, 1, !dbg !8
  store i32 %ADD, ptr %i, align 4, !dbg !8
  br label %while.start, !dbg !8

while.end:                                        ; preds = %while.start
  %i4 = load i32, ptr %i, align 4, !dbg !8
  ret i32 %i4, !dbg !8
}

define i32 @main() !dbg !9 {
entry:
  %len = alloca i32, align 4, !dbg !10
  store i32 0, ptr %len, align 4, !dbg !10
  %strlen = call i32 @strlen(ptr @STR0), !dbg !11
  store i32 %strlen, ptr %len, align 4, !dbg !11
  %len1 = load i32, ptr %len, align 4, !dbg !12
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 1, i32 %len1), !dbg !12
  ret i32 0, !dbg !12
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "strlen", linkageName: "strlen", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2)
!10 = !DILocation(line: 9, scope: !9)
!11 = !DILocation(line: 10, scope: !9)
!12 = !DILocation(line: 11, scope: !9)
