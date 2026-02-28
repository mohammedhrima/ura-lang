; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/data_types/ref005.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/data_types/ref005.ura"
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

define i32 @main() !dbg !4 {
entry:
  %val1 = alloca i32, align 4, !dbg !7
  store i32 0, ptr %val1, align 4, !dbg !7
  store i32 10, ptr %val1, align 4, !dbg !7
  %val2 = alloca i32, align 4, !dbg !7
  store i32 0, ptr %val2, align 4, !dbg !7
  store i32 20, ptr %val2, align 4, !dbg !7
  %val3 = alloca i32, align 4, !dbg !7
  store i32 0, ptr %val3, align 4, !dbg !7
  store i32 30, ptr %val3, align 4, !dbg !7
  %current = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %current, align 8, !dbg !7
  store ptr %val1, ptr %current, align 8, !dbg !7
  %ref_ptr = load ptr, ptr %current, align 8, !dbg !7
  store i32 15, ptr %ref_ptr, align 4, !dbg !7
  store ptr %val2, ptr %current, align 8, !dbg !7
  %ref_ptr1 = load ptr, ptr %current, align 8, !dbg !7
  store i32 25, ptr %ref_ptr1, align 4, !dbg !7
  store ptr %val3, ptr %current, align 8, !dbg !7
  %ptr = load ptr, ptr %current, align 8, !dbg !7
  %current2 = load i32, ptr %ptr, align 4, !dbg !7
  %ADD = add i32 %current2, 5, !dbg !7
  %ref_ptr3 = load ptr, ptr %current, align 8, !dbg !7
  store i32 %ADD, ptr %ref_ptr3, align 4, !dbg !7
  %sum = alloca i32, align 4, !dbg !7
  store i32 0, ptr %sum, align 4, !dbg !7
  %val14 = load i32, ptr %val1, align 4, !dbg !7
  %val25 = load i32, ptr %val2, align 4, !dbg !7
  %ADD6 = add i32 %val14, %val25, !dbg !7
  %val37 = load i32, ptr %val3, align 4, !dbg !7
  %ADD8 = add i32 %ADD6, %val37, !dbg !7
  store i32 %ADD8, ptr %sum, align 4, !dbg !7
  %sum9 = load i32, ptr %sum, align 4, !dbg !8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, i32 %sum9), !dbg !8
  ret i32 0, !dbg !8
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "ref005.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/data_types")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 25, scope: !4)
