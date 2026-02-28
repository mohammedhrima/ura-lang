; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [8 x i8] c"%sTrue\0A\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c"%sFalse\0A\00", align 1
@STR2 = private unnamed_addr constant [19 x i8] c"and var&&var (F): \00", align 1
@STR3 = private unnamed_addr constant [19 x i8] c"and var&&val (T): \00", align 1
@STR4 = private unnamed_addr constant [19 x i8] c"and var&&fn  (T): \00", align 1
@STR5 = private unnamed_addr constant [19 x i8] c"and val&&var (F): \00", align 1
@STR6 = private unnamed_addr constant [19 x i8] c"and val&&val (T): \00", align 1
@STR7 = private unnamed_addr constant [19 x i8] c"and val&&fn  (F): \00", align 1
@STR8 = private unnamed_addr constant [19 x i8] c"and fn&&var  (T): \00", align 1
@STR9 = private unnamed_addr constant [19 x i8] c"and fn&&val  (F): \00", align 1
@STR10 = private unnamed_addr constant [19 x i8] c"and fn&&fn   (F): \00", align 1
@STR11 = private unnamed_addr constant [19 x i8] c"and keyword  (F): \00", align 1
@STR12 = private unnamed_addr constant [19 x i8] c"and False&&fn(F): \00", align 1
@STR13 = private unnamed_addr constant [19 x i8] c"or  var||var (T): \00", align 1
@STR14 = private unnamed_addr constant [19 x i8] c"or  var||val (T): \00", align 1
@STR15 = private unnamed_addr constant [19 x i8] c"or  var||fn  (T): \00", align 1
@STR16 = private unnamed_addr constant [19 x i8] c"or  val||var (T): \00", align 1
@STR17 = private unnamed_addr constant [19 x i8] c"or  val||val (F): \00", align 1
@STR18 = private unnamed_addr constant [19 x i8] c"or  val||fn  (F): \00", align 1
@STR19 = private unnamed_addr constant [19 x i8] c"or  fn||var  (T): \00", align 1
@STR20 = private unnamed_addr constant [19 x i8] c"or  fn||val  (T): \00", align 1
@STR21 = private unnamed_addr constant [19 x i8] c"or  fn||fn   (T): \00", align 1
@STR22 = private unnamed_addr constant [19 x i8] c"or  keyword  (T): \00", align 1
@STR23 = private unnamed_addr constant [19 x i8] c"or  True||fn (T): \00", align 1
@STR24 = private unnamed_addr constant [19 x i8] c"not !var     (F): \00", align 1
@STR25 = private unnamed_addr constant [19 x i8] c"not !var     (T): \00", align 1
@STR26 = private unnamed_addr constant [19 x i8] c"not !val     (F): \00", align 1
@STR27 = private unnamed_addr constant [19 x i8] c"not !val     (T): \00", align 1
@STR28 = private unnamed_addr constant [19 x i8] c"not !fn      (F): \00", align 1
@STR29 = private unnamed_addr constant [19 x i8] c"not !fn      (T): \00", align 1
@STR30 = private unnamed_addr constant [19 x i8] c"not !!var    (T): \00", align 1
@STR31 = private unnamed_addr constant [19 x i8] c"not !!var    (F): \00", align 1
@STR32 = private unnamed_addr constant [19 x i8] c"not !!val    (T): \00", align 1
@STR33 = private unnamed_addr constant [19 x i8] c"not !!fn     (T): \00", align 1
@STR34 = private unnamed_addr constant [19 x i8] c"not !!fn     (F): \00", align 1
@STR35 = private unnamed_addr constant [19 x i8] c"comb !a||b   (F): \00", align 1
@STR36 = private unnamed_addr constant [19 x i8] c"comb a&&!b   (T): \00", align 1
@STR37 = private unnamed_addr constant [19 x i8] c"comb !a&&!b  (F): \00", align 1
@STR38 = private unnamed_addr constant [19 x i8] c"comb !f()&&t (T): \00", align 1
@STR39 = private unnamed_addr constant [19 x i8] c"comb a||b&&b (F): \00", align 1
@STR40 = private unnamed_addr constant [16 x i8] c"if a&&!b: True\0A\00", align 1
@STR41 = private unnamed_addr constant [28 x i8] c"if !a||b: should not print\0A\00", align 1
@STR42 = private unnamed_addr constant [27 x i8] c"if !a||b: False (correct)\0A\00", align 1

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

define i1 @t() !dbg !4 {
entry:
  ret i1 true, !dbg !7
}

define i1 @f() !dbg !8 {
entry:
  ret i1 false, !dbg !9
}

define void @print_bool(ptr %label, i1 %val) !dbg !10 {
entry:
  %label1 = alloca ptr, align 8, !dbg !11
  store ptr null, ptr %label1, align 8, !dbg !11
  store ptr %label, ptr %label1, align 8, !dbg !11
  %val2 = alloca i1, align 1, !dbg !11
  store i1 false, ptr %val2, align 1, !dbg !11
  store i1 %val, ptr %val2, align 1, !dbg !11
  br label %if.start, !dbg !11

if.start:                                         ; preds = %entry
  %val3 = load i1, ptr %val2, align 1, !dbg !11
  br i1 %val3, label %if.then, label %if.else, !dbg !11

if.end:                                           ; preds = %if.else, %if.then
  ret void, !dbg !12

if.then:                                          ; preds = %if.start
  %label4 = load ptr, ptr %label1, align 8, !dbg !13
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, ptr %label4), !dbg !13
  br label %if.end, !dbg !13

if.else:                                          ; preds = %if.start
  %label5 = load ptr, ptr %label1, align 8, !dbg !12
  %printf6 = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 1, ptr %label5), !dbg !12
  br label %if.end, !dbg !12
}

define i32 @main() !dbg !14 {
entry:
  %a = alloca i1, align 1, !dbg !15
  store i1 false, ptr %a, align 1, !dbg !15
  store i1 true, ptr %a, align 1, !dbg !15
  %b = alloca i1, align 1, !dbg !15
  store i1 false, ptr %b, align 1, !dbg !15
  store i1 false, ptr %b, align 1, !dbg !15
  %a1 = load i1, ptr %a, align 1, !dbg !16
  %b2 = load i1, ptr %b, align 1, !dbg !16
  %AND = and i1 %a1, %b2, !dbg !16
  call void @print_bool(ptr @STR2, i1 %AND), !dbg !16
  %a3 = load i1, ptr %a, align 1, !dbg !17
  %AND4 = and i1 %a3, true, !dbg !17
  call void @print_bool(ptr @STR3, i1 %AND4), !dbg !17
  %t = call i1 @t(), !dbg !18
  %a5 = load i1, ptr %a, align 1, !dbg !18
  %AND6 = and i1 %a5, %t, !dbg !18
  call void @print_bool(ptr @STR4, i1 %AND6), !dbg !18
  %a7 = load i1, ptr %a, align 1, !dbg !19
  %AND8 = and i1 false, %a7, !dbg !19
  call void @print_bool(ptr @STR5, i1 %AND8), !dbg !19
  call void @print_bool(ptr @STR6, i1 true), !dbg !20
  %f = call i1 @f(), !dbg !21
  %AND9 = and i1 true, %f, !dbg !21
  call void @print_bool(ptr @STR7, i1 %AND9), !dbg !21
  %t10 = call i1 @t(), !dbg !22
  %a11 = load i1, ptr %a, align 1, !dbg !22
  %AND12 = and i1 %t10, %a11, !dbg !22
  call void @print_bool(ptr @STR8, i1 %AND12), !dbg !22
  %f13 = call i1 @f(), !dbg !23
  %AND14 = and i1 %f13, true, !dbg !23
  call void @print_bool(ptr @STR9, i1 %AND14), !dbg !23
  %t15 = call i1 @t(), !dbg !24
  %f16 = call i1 @f(), !dbg !24
  %AND17 = and i1 %t15, %f16, !dbg !24
  call void @print_bool(ptr @STR10, i1 %AND17), !dbg !24
  %a18 = load i1, ptr %a, align 1, !dbg !25
  %b19 = load i1, ptr %b, align 1, !dbg !25
  %AND20 = and i1 %a18, %b19, !dbg !25
  call void @print_bool(ptr @STR11, i1 %AND20), !dbg !25
  %t21 = call i1 @t(), !dbg !26
  %AND22 = and i1 false, %t21, !dbg !26
  call void @print_bool(ptr @STR12, i1 %AND22), !dbg !26
  %a23 = load i1, ptr %a, align 1, !dbg !27
  %b24 = load i1, ptr %b, align 1, !dbg !27
  %OR = or i1 %a23, %b24, !dbg !27
  call void @print_bool(ptr @STR13, i1 %OR), !dbg !27
  %b25 = load i1, ptr %b, align 1, !dbg !28
  %OR26 = or i1 %b25, true, !dbg !28
  call void @print_bool(ptr @STR14, i1 %OR26), !dbg !28
  %t27 = call i1 @t(), !dbg !29
  %b28 = load i1, ptr %b, align 1, !dbg !29
  %OR29 = or i1 %b28, %t27, !dbg !29
  call void @print_bool(ptr @STR15, i1 %OR29), !dbg !29
  %a30 = load i1, ptr %a, align 1, !dbg !30
  %OR31 = or i1 false, %a30, !dbg !30
  call void @print_bool(ptr @STR16, i1 %OR31), !dbg !30
  call void @print_bool(ptr @STR17, i1 false), !dbg !31
  %f32 = call i1 @f(), !dbg !32
  %OR33 = or i1 false, %f32, !dbg !32
  call void @print_bool(ptr @STR18, i1 %OR33), !dbg !32
  %f34 = call i1 @f(), !dbg !33
  %a35 = load i1, ptr %a, align 1, !dbg !33
  %OR36 = or i1 %f34, %a35, !dbg !33
  call void @print_bool(ptr @STR19, i1 %OR36), !dbg !33
  %t37 = call i1 @t(), !dbg !34
  %OR38 = or i1 %t37, false, !dbg !34
  call void @print_bool(ptr @STR20, i1 %OR38), !dbg !34
  %f39 = call i1 @f(), !dbg !35
  %t40 = call i1 @t(), !dbg !35
  %OR41 = or i1 %f39, %t40, !dbg !35
  call void @print_bool(ptr @STR21, i1 %OR41), !dbg !35
  %b42 = load i1, ptr %b, align 1, !dbg !36
  %a43 = load i1, ptr %a, align 1, !dbg !36
  %OR44 = or i1 %b42, %a43, !dbg !36
  call void @print_bool(ptr @STR22, i1 %OR44), !dbg !36
  %f45 = call i1 @f(), !dbg !37
  %OR46 = or i1 true, %f45, !dbg !37
  call void @print_bool(ptr @STR23, i1 %OR46), !dbg !37
  %a47 = load i1, ptr %a, align 1, !dbg !38
  %NOT = xor i1 %a47, true, !dbg !38
  call void @print_bool(ptr @STR24, i1 %NOT), !dbg !38
  %b48 = load i1, ptr %b, align 1, !dbg !39
  %NOT49 = xor i1 %b48, true, !dbg !39
  call void @print_bool(ptr @STR25, i1 %NOT49), !dbg !39
  call void @print_bool(ptr @STR26, i1 false), !dbg !40
  call void @print_bool(ptr @STR27, i1 true), !dbg !41
  %t50 = call i1 @t(), !dbg !42
  %NOT51 = xor i1 %t50, true, !dbg !42
  call void @print_bool(ptr @STR28, i1 %NOT51), !dbg !42
  %f52 = call i1 @f(), !dbg !43
  %NOT53 = xor i1 %f52, true, !dbg !43
  call void @print_bool(ptr @STR29, i1 %NOT53), !dbg !43
  %a54 = load i1, ptr %a, align 1, !dbg !44
  %NOT55 = xor i1 %a54, true, !dbg !44
  %NOT56 = xor i1 %NOT55, true, !dbg !44
  call void @print_bool(ptr @STR30, i1 %NOT56), !dbg !44
  %b57 = load i1, ptr %b, align 1, !dbg !45
  %NOT58 = xor i1 %b57, true, !dbg !45
  %NOT59 = xor i1 %NOT58, true, !dbg !45
  call void @print_bool(ptr @STR31, i1 %NOT59), !dbg !45
  call void @print_bool(ptr @STR32, i1 true), !dbg !46
  %t60 = call i1 @t(), !dbg !47
  %NOT61 = xor i1 %t60, true, !dbg !47
  %NOT62 = xor i1 %NOT61, true, !dbg !47
  call void @print_bool(ptr @STR33, i1 %NOT62), !dbg !47
  %f63 = call i1 @f(), !dbg !48
  %NOT64 = xor i1 %f63, true, !dbg !48
  %NOT65 = xor i1 %NOT64, true, !dbg !48
  call void @print_bool(ptr @STR34, i1 %NOT65), !dbg !48
  %a66 = load i1, ptr %a, align 1, !dbg !49
  %b67 = load i1, ptr %b, align 1, !dbg !49
  %OR68 = or i1 %a66, %b67, !dbg !49
  %NOT69 = xor i1 %OR68, true, !dbg !49
  call void @print_bool(ptr @STR35, i1 %NOT69), !dbg !49
  %b70 = load i1, ptr %b, align 1, !dbg !50
  %NOT71 = xor i1 %b70, true, !dbg !50
  %a72 = load i1, ptr %a, align 1, !dbg !50
  %AND73 = and i1 %a72, %NOT71, !dbg !50
  call void @print_bool(ptr @STR36, i1 %AND73), !dbg !50
  %b74 = load i1, ptr %b, align 1, !dbg !51
  %NOT75 = xor i1 %b74, true, !dbg !51
  %a76 = load i1, ptr %a, align 1, !dbg !51
  %AND77 = and i1 %a76, %NOT75, !dbg !51
  %NOT78 = xor i1 %AND77, true, !dbg !51
  call void @print_bool(ptr @STR37, i1 %NOT78), !dbg !51
  %f79 = call i1 @f(), !dbg !52
  %t80 = call i1 @t(), !dbg !52
  %AND81 = and i1 %f79, %t80, !dbg !52
  %NOT82 = xor i1 %AND81, true, !dbg !52
  call void @print_bool(ptr @STR38, i1 %NOT82), !dbg !52
  %a83 = load i1, ptr %a, align 1, !dbg !53
  %b84 = load i1, ptr %b, align 1, !dbg !53
  %OR85 = or i1 %a83, %b84, !dbg !53
  %b86 = load i1, ptr %b, align 1, !dbg !53
  %AND87 = and i1 %OR85, %b86, !dbg !53
  call void @print_bool(ptr @STR39, i1 %AND87), !dbg !53
  br label %if.start, !dbg !53

if.start:                                         ; preds = %entry
  %b88 = load i1, ptr %b, align 1, !dbg !53
  %NOT89 = xor i1 %b88, true, !dbg !53
  %a90 = load i1, ptr %a, align 1, !dbg !53
  %AND91 = and i1 %a90, %NOT89, !dbg !53
  br i1 %AND91, label %if.then, label %if.end, !dbg !53

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start92, !dbg !54

if.then:                                          ; preds = %if.start
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR40, i32 0), !dbg !54
  br label %if.end, !dbg !54

if.start92:                                       ; preds = %if.end
  %a95 = load i1, ptr %a, align 1, !dbg !54
  %b96 = load i1, ptr %b, align 1, !dbg !54
  %OR97 = or i1 %a95, %b96, !dbg !54
  %NOT98 = xor i1 %OR97, true, !dbg !54
  br i1 %NOT98, label %if.then94, label %if.else, !dbg !54

if.end93:                                         ; preds = %if.else, %if.then94
  ret i32 0, !dbg !55

if.then94:                                        ; preds = %if.start92
  %printf99 = call i32 (ptr, i32, ...) @printf(ptr @STR41, i32 0), !dbg !56
  br label %if.end93, !dbg !56

if.else:                                          ; preds = %if.start92
  %printf100 = call i32 (ptr, i32, ...) @printf(ptr @STR42, i32 0), !dbg !55
  br label %if.end93, !dbg !55
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "t", linkageName: "t", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "f", linkageName: "f", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "print_bool", linkageName: "print_bool", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2)
!11 = !DILocation(line: 9, scope: !10)
!12 = !DILocation(line: 13, scope: !10)
!13 = !DILocation(line: 11, scope: !10)
!14 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 15, type: !5, scopeLine: 15, spFlags: DISPFlagDefinition, unit: !2)
!15 = !DILocation(line: 15, scope: !14)
!16 = !DILocation(line: 20, scope: !14)
!17 = !DILocation(line: 21, scope: !14)
!18 = !DILocation(line: 22, scope: !14)
!19 = !DILocation(line: 23, scope: !14)
!20 = !DILocation(line: 24, scope: !14)
!21 = !DILocation(line: 25, scope: !14)
!22 = !DILocation(line: 26, scope: !14)
!23 = !DILocation(line: 27, scope: !14)
!24 = !DILocation(line: 28, scope: !14)
!25 = !DILocation(line: 29, scope: !14)
!26 = !DILocation(line: 30, scope: !14)
!27 = !DILocation(line: 33, scope: !14)
!28 = !DILocation(line: 34, scope: !14)
!29 = !DILocation(line: 35, scope: !14)
!30 = !DILocation(line: 36, scope: !14)
!31 = !DILocation(line: 37, scope: !14)
!32 = !DILocation(line: 38, scope: !14)
!33 = !DILocation(line: 39, scope: !14)
!34 = !DILocation(line: 40, scope: !14)
!35 = !DILocation(line: 41, scope: !14)
!36 = !DILocation(line: 42, scope: !14)
!37 = !DILocation(line: 43, scope: !14)
!38 = !DILocation(line: 46, scope: !14)
!39 = !DILocation(line: 47, scope: !14)
!40 = !DILocation(line: 48, scope: !14)
!41 = !DILocation(line: 49, scope: !14)
!42 = !DILocation(line: 50, scope: !14)
!43 = !DILocation(line: 51, scope: !14)
!44 = !DILocation(line: 52, scope: !14)
!45 = !DILocation(line: 53, scope: !14)
!46 = !DILocation(line: 54, scope: !14)
!47 = !DILocation(line: 55, scope: !14)
!48 = !DILocation(line: 56, scope: !14)
!49 = !DILocation(line: 59, scope: !14)
!50 = !DILocation(line: 60, scope: !14)
!51 = !DILocation(line: 61, scope: !14)
!52 = !DILocation(line: 62, scope: !14)
!53 = !DILocation(line: 63, scope: !14)
!54 = !DILocation(line: 67, scope: !14)
!55 = !DILocation(line: 71, scope: !14)
!56 = !DILocation(line: 69, scope: !14)
