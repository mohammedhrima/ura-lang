; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op/logical.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op/logical.ura"
target triple = "arm64-apple-macosx16.0.0"

@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"%s%s\0A\00", align 1
@STR1 = private unnamed_addr constant [19 x i8] c"and var&&var (F): \00", align 1
@STR2 = private unnamed_addr constant [19 x i8] c"and var&&val (T): \00", align 1
@STR3 = private unnamed_addr constant [19 x i8] c"and var&&fn  (T): \00", align 1
@STR4 = private unnamed_addr constant [19 x i8] c"and val&&var (F): \00", align 1
@STR5 = private unnamed_addr constant [19 x i8] c"and val&&val (T): \00", align 1
@STR6 = private unnamed_addr constant [19 x i8] c"and val&&fn  (F): \00", align 1
@STR7 = private unnamed_addr constant [19 x i8] c"and fn&&var  (T): \00", align 1
@STR8 = private unnamed_addr constant [19 x i8] c"and fn&&val  (F): \00", align 1
@STR9 = private unnamed_addr constant [19 x i8] c"and fn&&fn   (F): \00", align 1
@STR10 = private unnamed_addr constant [19 x i8] c"and keyword  (F): \00", align 1
@STR11 = private unnamed_addr constant [19 x i8] c"and False&&fn(F): \00", align 1
@STR12 = private unnamed_addr constant [19 x i8] c"or  var||var (T): \00", align 1
@STR13 = private unnamed_addr constant [19 x i8] c"or  var||val (T): \00", align 1
@STR14 = private unnamed_addr constant [19 x i8] c"or  var||fn  (T): \00", align 1
@STR15 = private unnamed_addr constant [19 x i8] c"or  val||var (T): \00", align 1
@STR16 = private unnamed_addr constant [19 x i8] c"or  val||val (F): \00", align 1
@STR17 = private unnamed_addr constant [19 x i8] c"or  val||fn  (F): \00", align 1
@STR18 = private unnamed_addr constant [19 x i8] c"or  fn||var  (T): \00", align 1
@STR19 = private unnamed_addr constant [19 x i8] c"or  fn||val  (T): \00", align 1
@STR20 = private unnamed_addr constant [19 x i8] c"or  fn||fn   (T): \00", align 1
@STR21 = private unnamed_addr constant [19 x i8] c"or  keyword  (T): \00", align 1
@STR22 = private unnamed_addr constant [19 x i8] c"or  True||fn (T): \00", align 1
@STR23 = private unnamed_addr constant [19 x i8] c"not !var     (F): \00", align 1
@STR24 = private unnamed_addr constant [19 x i8] c"not !var     (T): \00", align 1
@STR25 = private unnamed_addr constant [19 x i8] c"not !val     (F): \00", align 1
@STR26 = private unnamed_addr constant [19 x i8] c"not !val     (T): \00", align 1
@STR27 = private unnamed_addr constant [19 x i8] c"not !fn      (F): \00", align 1
@STR28 = private unnamed_addr constant [19 x i8] c"not !fn      (T): \00", align 1
@STR29 = private unnamed_addr constant [19 x i8] c"not !!var    (T): \00", align 1
@STR30 = private unnamed_addr constant [19 x i8] c"not !!var    (F): \00", align 1
@STR31 = private unnamed_addr constant [19 x i8] c"not !!val    (T): \00", align 1
@STR32 = private unnamed_addr constant [19 x i8] c"not !!fn     (T): \00", align 1
@STR33 = private unnamed_addr constant [19 x i8] c"not !!fn     (F): \00", align 1
@STR34 = private unnamed_addr constant [19 x i8] c"comb !a||b   (F): \00", align 1
@STR35 = private unnamed_addr constant [19 x i8] c"comb a&&!b   (T): \00", align 1
@STR36 = private unnamed_addr constant [19 x i8] c"comb !a&&!b  (F): \00", align 1
@STR37 = private unnamed_addr constant [19 x i8] c"comb !f()&&t (T): \00", align 1
@STR38 = private unnamed_addr constant [19 x i8] c"comb a||b&&b (F): \00", align 1
@STR39 = private unnamed_addr constant [16 x i8] c"if a&&!b: True\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [16 x i8] c"if a&&!b: True\0A\00", align 1
@STR40 = private unnamed_addr constant [28 x i8] c"if !a||b: should not print\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [28 x i8] c"if !a||b: should not print\0A\00", align 1
@STR41 = private unnamed_addr constant [27 x i8] c"if !a||b: False (correct)\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [27 x i8] c"if !a||b: False (correct)\0A\00", align 1

define i1 @t() !dbg !4 {
entry:
  ret i1 true, !dbg !7
}

define i1 @f() !dbg !8 {
entry:
  ret i1 false, !dbg !9
}

define void @print_bool(i8* %label, i1 %val) !dbg !10 {
entry:
  %label1 = alloca i8*, align 8, !dbg !11
  %val2 = alloca i1, align 1, !dbg !11
  store i8* %label, i8** %label1, align 8, !dbg !11
  store i1 %val, i1* %val2, align 1, !dbg !11
  %label3 = load i8*, i8** %label1, align 8, !dbg !11
  %val4 = load i1, i1* %val2, align 1, !dbg !11
  %bool_str = select i1 %val4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !11
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %label3, i8* %bool_str), !dbg !11
  ret void, !dbg !11
}

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !12 {
entry:
  %a = alloca i1, align 1, !dbg !13
  %b = alloca i1, align 1, !dbg !13
  store i1 false, i1* %a, align 1, !dbg !13
  store i1 true, i1* %a, align 1, !dbg !13
  store i1 false, i1* %b, align 1, !dbg !13
  store i1 false, i1* %b, align 1, !dbg !13
  %a1 = load i1, i1* %a, align 1, !dbg !14
  %b2 = load i1, i1* %b, align 1, !dbg !14
  %AND = and i1 %a1, %b2, !dbg !14
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR1, i32 0, i32 0), i1 %AND), !dbg !14
  %a3 = load i1, i1* %a, align 1, !dbg !15
  %AND4 = and i1 %a3, true, !dbg !15
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR2, i32 0, i32 0), i1 %AND4), !dbg !15
  %t = call i1 @t(), !dbg !16
  %a5 = load i1, i1* %a, align 1, !dbg !16
  %AND6 = and i1 %a5, %t, !dbg !16
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR3, i32 0, i32 0), i1 %AND6), !dbg !16
  %a7 = load i1, i1* %a, align 1, !dbg !17
  %AND8 = and i1 false, %a7, !dbg !17
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR4, i32 0, i32 0), i1 %AND8), !dbg !17
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR5, i32 0, i32 0), i1 true), !dbg !18
  %f = call i1 @f(), !dbg !19
  %AND9 = and i1 true, %f, !dbg !19
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR6, i32 0, i32 0), i1 %AND9), !dbg !19
  %t10 = call i1 @t(), !dbg !20
  %a11 = load i1, i1* %a, align 1, !dbg !20
  %AND12 = and i1 %t10, %a11, !dbg !20
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR7, i32 0, i32 0), i1 %AND12), !dbg !20
  %f13 = call i1 @f(), !dbg !21
  %AND14 = and i1 %f13, true, !dbg !21
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR8, i32 0, i32 0), i1 %AND14), !dbg !21
  %t15 = call i1 @t(), !dbg !22
  %f16 = call i1 @f(), !dbg !22
  %AND17 = and i1 %t15, %f16, !dbg !22
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR9, i32 0, i32 0), i1 %AND17), !dbg !22
  %a18 = load i1, i1* %a, align 1, !dbg !23
  %b19 = load i1, i1* %b, align 1, !dbg !23
  %AND20 = and i1 %a18, %b19, !dbg !23
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR10, i32 0, i32 0), i1 %AND20), !dbg !23
  %t21 = call i1 @t(), !dbg !24
  %AND22 = and i1 false, %t21, !dbg !24
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR11, i32 0, i32 0), i1 %AND22), !dbg !24
  %a23 = load i1, i1* %a, align 1, !dbg !25
  %b24 = load i1, i1* %b, align 1, !dbg !25
  %OR = or i1 %a23, %b24, !dbg !25
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR12, i32 0, i32 0), i1 %OR), !dbg !25
  %b25 = load i1, i1* %b, align 1, !dbg !26
  %OR26 = or i1 %b25, true, !dbg !26
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR13, i32 0, i32 0), i1 %OR26), !dbg !26
  %t27 = call i1 @t(), !dbg !27
  %b28 = load i1, i1* %b, align 1, !dbg !27
  %OR29 = or i1 %b28, %t27, !dbg !27
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR14, i32 0, i32 0), i1 %OR29), !dbg !27
  %a30 = load i1, i1* %a, align 1, !dbg !28
  %OR31 = or i1 false, %a30, !dbg !28
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR15, i32 0, i32 0), i1 %OR31), !dbg !28
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR16, i32 0, i32 0), i1 false), !dbg !29
  %f32 = call i1 @f(), !dbg !30
  %OR33 = or i1 false, %f32, !dbg !30
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR17, i32 0, i32 0), i1 %OR33), !dbg !30
  %f34 = call i1 @f(), !dbg !31
  %a35 = load i1, i1* %a, align 1, !dbg !31
  %OR36 = or i1 %f34, %a35, !dbg !31
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR18, i32 0, i32 0), i1 %OR36), !dbg !31
  %t37 = call i1 @t(), !dbg !32
  %OR38 = or i1 %t37, false, !dbg !32
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR19, i32 0, i32 0), i1 %OR38), !dbg !32
  %f39 = call i1 @f(), !dbg !33
  %t40 = call i1 @t(), !dbg !33
  %OR41 = or i1 %f39, %t40, !dbg !33
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR20, i32 0, i32 0), i1 %OR41), !dbg !33
  %b42 = load i1, i1* %b, align 1, !dbg !34
  %a43 = load i1, i1* %a, align 1, !dbg !34
  %OR44 = or i1 %b42, %a43, !dbg !34
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR21, i32 0, i32 0), i1 %OR44), !dbg !34
  %f45 = call i1 @f(), !dbg !35
  %OR46 = or i1 true, %f45, !dbg !35
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR22, i32 0, i32 0), i1 %OR46), !dbg !35
  %a47 = load i1, i1* %a, align 1, !dbg !36
  %NOT = xor i1 %a47, true, !dbg !36
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR23, i32 0, i32 0), i1 %NOT), !dbg !36
  %b48 = load i1, i1* %b, align 1, !dbg !37
  %NOT49 = xor i1 %b48, true, !dbg !37
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR24, i32 0, i32 0), i1 %NOT49), !dbg !37
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR25, i32 0, i32 0), i1 false), !dbg !38
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR26, i32 0, i32 0), i1 true), !dbg !39
  %t50 = call i1 @t(), !dbg !40
  %NOT51 = xor i1 %t50, true, !dbg !40
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR27, i32 0, i32 0), i1 %NOT51), !dbg !40
  %f52 = call i1 @f(), !dbg !41
  %NOT53 = xor i1 %f52, true, !dbg !41
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR28, i32 0, i32 0), i1 %NOT53), !dbg !41
  %a54 = load i1, i1* %a, align 1, !dbg !42
  %NOT55 = xor i1 %a54, true, !dbg !42
  %NOT56 = xor i1 %NOT55, true, !dbg !42
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR29, i32 0, i32 0), i1 %NOT56), !dbg !42
  %b57 = load i1, i1* %b, align 1, !dbg !43
  %NOT58 = xor i1 %b57, true, !dbg !43
  %NOT59 = xor i1 %NOT58, true, !dbg !43
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR30, i32 0, i32 0), i1 %NOT59), !dbg !43
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR31, i32 0, i32 0), i1 true), !dbg !44
  %t60 = call i1 @t(), !dbg !45
  %NOT61 = xor i1 %t60, true, !dbg !45
  %NOT62 = xor i1 %NOT61, true, !dbg !45
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR32, i32 0, i32 0), i1 %NOT62), !dbg !45
  %f63 = call i1 @f(), !dbg !46
  %NOT64 = xor i1 %f63, true, !dbg !46
  %NOT65 = xor i1 %NOT64, true, !dbg !46
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR33, i32 0, i32 0), i1 %NOT65), !dbg !46
  %a66 = load i1, i1* %a, align 1, !dbg !47
  %b67 = load i1, i1* %b, align 1, !dbg !47
  %OR68 = or i1 %a66, %b67, !dbg !47
  %NOT69 = xor i1 %OR68, true, !dbg !47
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR34, i32 0, i32 0), i1 %NOT69), !dbg !47
  %b70 = load i1, i1* %b, align 1, !dbg !48
  %NOT71 = xor i1 %b70, true, !dbg !48
  %a72 = load i1, i1* %a, align 1, !dbg !48
  %AND73 = and i1 %a72, %NOT71, !dbg !48
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR35, i32 0, i32 0), i1 %AND73), !dbg !48
  %b74 = load i1, i1* %b, align 1, !dbg !49
  %NOT75 = xor i1 %b74, true, !dbg !49
  %a76 = load i1, i1* %a, align 1, !dbg !49
  %AND77 = and i1 %a76, %NOT75, !dbg !49
  %NOT78 = xor i1 %AND77, true, !dbg !49
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR36, i32 0, i32 0), i1 %NOT78), !dbg !49
  %f79 = call i1 @f(), !dbg !50
  %t80 = call i1 @t(), !dbg !50
  %AND81 = and i1 %f79, %t80, !dbg !50
  %NOT82 = xor i1 %AND81, true, !dbg !50
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR37, i32 0, i32 0), i1 %NOT82), !dbg !50
  %a83 = load i1, i1* %a, align 1, !dbg !51
  %b84 = load i1, i1* %b, align 1, !dbg !51
  %OR85 = or i1 %a83, %b84, !dbg !51
  %b86 = load i1, i1* %b, align 1, !dbg !51
  %AND87 = and i1 %OR85, %b86, !dbg !51
  call void @print_bool(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR38, i32 0, i32 0), i1 %AND87), !dbg !51
  br label %if.start, !dbg !51

if.start:                                         ; preds = %entry
  %b88 = load i1, i1* %b, align 1, !dbg !51
  %NOT89 = xor i1 %b88, true, !dbg !51
  %a90 = load i1, i1* %a, align 1, !dbg !51
  %AND91 = and i1 %a90, %NOT89, !dbg !51
  br i1 %AND91, label %if.then, label %if.end, !dbg !51

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start92, !dbg !51

if.then:                                          ; preds = %if.start
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !51
  br label %if.end, !dbg !51

if.start92:                                       ; preds = %if.end
  %a95 = load i1, i1* %a, align 1, !dbg !51
  %b96 = load i1, i1* %b, align 1, !dbg !51
  %OR97 = or i1 %a95, %b96, !dbg !51
  %NOT98 = xor i1 %OR97, true, !dbg !51
  br i1 %NOT98, label %if.then94, label %if.else, !dbg !51

if.end93:                                         ; preds = %if.else, %if.then94
  ret i32 0, !dbg !51

if.then94:                                        ; preds = %if.start92
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @output_fmt.2, i32 0, i32 0), i32 0), !dbg !51
  br label %if.end93, !dbg !51

if.else:                                          ; preds = %if.start92
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @output_fmt.3, i32 0, i32 0), i32 0), !dbg !51
  br label %if.end93, !dbg !51
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "logical.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op")
!4 = distinct !DISubprogram(name: "t", linkageName: "t", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "f", linkageName: "f", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "print_bool", linkageName: "print_bool", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 9, scope: !10)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 12, scope: !12)
!14 = !DILocation(line: 17, scope: !12)
!15 = !DILocation(line: 18, scope: !12)
!16 = !DILocation(line: 19, scope: !12)
!17 = !DILocation(line: 20, scope: !12)
!18 = !DILocation(line: 21, scope: !12)
!19 = !DILocation(line: 22, scope: !12)
!20 = !DILocation(line: 23, scope: !12)
!21 = !DILocation(line: 24, scope: !12)
!22 = !DILocation(line: 25, scope: !12)
!23 = !DILocation(line: 26, scope: !12)
!24 = !DILocation(line: 27, scope: !12)
!25 = !DILocation(line: 30, scope: !12)
!26 = !DILocation(line: 31, scope: !12)
!27 = !DILocation(line: 32, scope: !12)
!28 = !DILocation(line: 33, scope: !12)
!29 = !DILocation(line: 34, scope: !12)
!30 = !DILocation(line: 35, scope: !12)
!31 = !DILocation(line: 36, scope: !12)
!32 = !DILocation(line: 37, scope: !12)
!33 = !DILocation(line: 38, scope: !12)
!34 = !DILocation(line: 39, scope: !12)
!35 = !DILocation(line: 40, scope: !12)
!36 = !DILocation(line: 43, scope: !12)
!37 = !DILocation(line: 44, scope: !12)
!38 = !DILocation(line: 45, scope: !12)
!39 = !DILocation(line: 46, scope: !12)
!40 = !DILocation(line: 47, scope: !12)
!41 = !DILocation(line: 48, scope: !12)
!42 = !DILocation(line: 49, scope: !12)
!43 = !DILocation(line: 50, scope: !12)
!44 = !DILocation(line: 51, scope: !12)
!45 = !DILocation(line: 52, scope: !12)
!46 = !DILocation(line: 53, scope: !12)
!47 = !DILocation(line: 56, scope: !12)
!48 = !DILocation(line: 57, scope: !12)
!49 = !DILocation(line: 58, scope: !12)
!50 = !DILocation(line: 59, scope: !12)
!51 = !DILocation(line: 60, scope: !12)
