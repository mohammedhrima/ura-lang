; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op/comparison.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op/comparison.ura"
target triple = "arm64-apple-macosx16.0.0"

@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"%s%s\0A\00", align 1
@STR1 = private unnamed_addr constant [20 x i8] c"eq  var==var (F):  \00", align 1
@STR2 = private unnamed_addr constant [20 x i8] c"eq  var==val (T):  \00", align 1
@STR3 = private unnamed_addr constant [20 x i8] c"eq  var==fn  (T):  \00", align 1
@STR4 = private unnamed_addr constant [20 x i8] c"eq  val==var (T):  \00", align 1
@STR5 = private unnamed_addr constant [20 x i8] c"eq  val==val (T):  \00", align 1
@STR6 = private unnamed_addr constant [20 x i8] c"eq  fn==fn   (F):  \00", align 1
@STR7 = private unnamed_addr constant [20 x i8] c"eq  fn==fn   (T):  \00", align 1
@STR8 = private unnamed_addr constant [20 x i8] c"neq var!=var (T):  \00", align 1
@STR9 = private unnamed_addr constant [20 x i8] c"neq var!=val (F):  \00", align 1
@STR10 = private unnamed_addr constant [20 x i8] c"neq var!=fn  (F):  \00", align 1
@STR11 = private unnamed_addr constant [20 x i8] c"neq val!=val (F):  \00", align 1
@STR12 = private unnamed_addr constant [20 x i8] c"neq fn!=fn   (T):  \00", align 1
@STR13 = private unnamed_addr constant [20 x i8] c"lt  var<var  (T):  \00", align 1
@STR14 = private unnamed_addr constant [20 x i8] c"lt  var<val  (F):  \00", align 1
@STR15 = private unnamed_addr constant [20 x i8] c"lt  var<fn   (F):  \00", align 1
@STR16 = private unnamed_addr constant [20 x i8] c"lt  val<val  (T):  \00", align 1
@STR17 = private unnamed_addr constant [20 x i8] c"lt  fn<fn    (T):  \00", align 1
@STR18 = private unnamed_addr constant [20 x i8] c"le  var<=var (T):  \00", align 1
@STR19 = private unnamed_addr constant [20 x i8] c"le  var<=val (T):  \00", align 1
@STR20 = private unnamed_addr constant [20 x i8] c"le  var<=fn  (T):  \00", align 1
@STR21 = private unnamed_addr constant [20 x i8] c"le  val<=val (T):  \00", align 1
@STR22 = private unnamed_addr constant [20 x i8] c"le  fn<=fn   (T):  \00", align 1
@STR23 = private unnamed_addr constant [20 x i8] c"le  fn<=fn   (F):  \00", align 1
@STR24 = private unnamed_addr constant [20 x i8] c"gt  var>var  (F):  \00", align 1
@STR25 = private unnamed_addr constant [20 x i8] c"gt  var>val  (T):  \00", align 1
@STR26 = private unnamed_addr constant [20 x i8] c"gt  var>fn   (T):  \00", align 1
@STR27 = private unnamed_addr constant [20 x i8] c"gt  val>val  (T):  \00", align 1
@STR28 = private unnamed_addr constant [20 x i8] c"gt  fn>fn    (F):  \00", align 1
@STR29 = private unnamed_addr constant [20 x i8] c"ge  var>=var (T):  \00", align 1
@STR30 = private unnamed_addr constant [20 x i8] c"ge  var>=val (T):  \00", align 1
@STR31 = private unnamed_addr constant [20 x i8] c"ge  var>=fn  (T):  \00", align 1
@STR32 = private unnamed_addr constant [20 x i8] c"ge  val>=val (T):  \00", align 1
@STR33 = private unnamed_addr constant [20 x i8] c"ge  fn>=fn   (T):  \00", align 1
@STR34 = private unnamed_addr constant [20 x i8] c"ge  fn>=fn   (F):  \00", align 1
@STR35 = private unnamed_addr constant [20 x i8] c"is  var is var(F): \00", align 1
@STR36 = private unnamed_addr constant [20 x i8] c"is  fn  is fn (F): \00", align 1
@STR37 = private unnamed_addr constant [20 x i8] c"is  fn  is fn (T): \00", align 1
@STR38 = private unnamed_addr constant [29 x i8] c"chain: a<b and a==foo: True\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [29 x i8] c"chain: a<b and a==foo: True\0A\00", align 1

define i32 @foo() !dbg !4 {
entry:
  ret i32 10, !dbg !7
}

define i32 @bar() !dbg !8 {
entry:
  ret i32 20, !dbg !9
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
  %a = alloca i32, align 4, !dbg !13
  %b = alloca i32, align 4, !dbg !13
  store i32 0, i32* %a, align 4, !dbg !13
  store i32 10, i32* %a, align 4, !dbg !13
  store i32 0, i32* %b, align 4, !dbg !13
  store i32 20, i32* %b, align 4, !dbg !13
  %a1 = load i32, i32* %a, align 4, !dbg !14
  %b2 = load i32, i32* %b, align 4, !dbg !14
  %EQ = icmp eq i32 %a1, %b2, !dbg !14
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR1, i32 0, i32 0), i1 %EQ), !dbg !14
  %a3 = load i32, i32* %a, align 4, !dbg !15
  %EQ4 = icmp eq i32 %a3, 10, !dbg !15
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR2, i32 0, i32 0), i1 %EQ4), !dbg !15
  %foo = call i32 @foo(), !dbg !16
  %a5 = load i32, i32* %a, align 4, !dbg !16
  %EQ6 = icmp eq i32 %a5, %foo, !dbg !16
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR3, i32 0, i32 0), i1 %EQ6), !dbg !16
  %b7 = load i32, i32* %b, align 4, !dbg !17
  %EQ8 = icmp eq i32 20, %b7, !dbg !17
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR4, i32 0, i32 0), i1 %EQ8), !dbg !17
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR5, i32 0, i32 0), i1 true), !dbg !18
  %foo9 = call i32 @foo(), !dbg !19
  %bar = call i32 @bar(), !dbg !19
  %EQ10 = icmp eq i32 %foo9, %bar, !dbg !19
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR6, i32 0, i32 0), i1 %EQ10), !dbg !19
  %foo11 = call i32 @foo(), !dbg !20
  %foo12 = call i32 @foo(), !dbg !20
  %EQ13 = icmp eq i32 %foo11, %foo12, !dbg !20
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR7, i32 0, i32 0), i1 %EQ13), !dbg !20
  %a14 = load i32, i32* %a, align 4, !dbg !21
  %b15 = load i32, i32* %b, align 4, !dbg !21
  %NEQ = icmp ne i32 %a14, %b15, !dbg !21
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR8, i32 0, i32 0), i1 %NEQ), !dbg !21
  %a16 = load i32, i32* %a, align 4, !dbg !22
  %NEQ17 = icmp ne i32 %a16, 10, !dbg !22
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR9, i32 0, i32 0), i1 %NEQ17), !dbg !22
  %foo18 = call i32 @foo(), !dbg !23
  %a19 = load i32, i32* %a, align 4, !dbg !23
  %NEQ20 = icmp ne i32 %a19, %foo18, !dbg !23
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR10, i32 0, i32 0), i1 %NEQ20), !dbg !23
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR11, i32 0, i32 0), i1 false), !dbg !24
  %foo21 = call i32 @foo(), !dbg !25
  %bar22 = call i32 @bar(), !dbg !25
  %NEQ23 = icmp ne i32 %foo21, %bar22, !dbg !25
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR12, i32 0, i32 0), i1 %NEQ23), !dbg !25
  %a24 = load i32, i32* %a, align 4, !dbg !26
  %b25 = load i32, i32* %b, align 4, !dbg !26
  %LT = icmp slt i32 %a24, %b25, !dbg !26
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR13, i32 0, i32 0), i1 %LT), !dbg !26
  %a26 = load i32, i32* %a, align 4, !dbg !27
  %LT27 = icmp slt i32 %a26, 5, !dbg !27
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR14, i32 0, i32 0), i1 %LT27), !dbg !27
  %foo28 = call i32 @foo(), !dbg !28
  %b29 = load i32, i32* %b, align 4, !dbg !28
  %LT30 = icmp slt i32 %b29, %foo28, !dbg !28
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR15, i32 0, i32 0), i1 %LT30), !dbg !28
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR16, i32 0, i32 0), i1 true), !dbg !29
  %foo31 = call i32 @foo(), !dbg !30
  %bar32 = call i32 @bar(), !dbg !30
  %LT33 = icmp slt i32 %foo31, %bar32, !dbg !30
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR17, i32 0, i32 0), i1 %LT33), !dbg !30
  %a34 = load i32, i32* %a, align 4, !dbg !31
  %b35 = load i32, i32* %b, align 4, !dbg !31
  %LE = icmp sle i32 %a34, %b35, !dbg !31
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR18, i32 0, i32 0), i1 %LE), !dbg !31
  %a36 = load i32, i32* %a, align 4, !dbg !32
  %LE37 = icmp sle i32 %a36, 10, !dbg !32
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR19, i32 0, i32 0), i1 %LE37), !dbg !32
  %foo38 = call i32 @foo(), !dbg !33
  %a39 = load i32, i32* %a, align 4, !dbg !33
  %LE40 = icmp sle i32 %a39, %foo38, !dbg !33
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR20, i32 0, i32 0), i1 %LE40), !dbg !33
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR21, i32 0, i32 0), i1 true), !dbg !34
  %foo41 = call i32 @foo(), !dbg !35
  %bar42 = call i32 @bar(), !dbg !35
  %LE43 = icmp sle i32 %foo41, %bar42, !dbg !35
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR22, i32 0, i32 0), i1 %LE43), !dbg !35
  %bar44 = call i32 @bar(), !dbg !36
  %foo45 = call i32 @foo(), !dbg !36
  %LE46 = icmp sle i32 %bar44, %foo45, !dbg !36
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR23, i32 0, i32 0), i1 %LE46), !dbg !36
  %a47 = load i32, i32* %a, align 4, !dbg !37
  %b48 = load i32, i32* %b, align 4, !dbg !37
  %GT = icmp sgt i32 %a47, %b48, !dbg !37
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR24, i32 0, i32 0), i1 %GT), !dbg !37
  %b49 = load i32, i32* %b, align 4, !dbg !38
  %GT50 = icmp sgt i32 %b49, 10, !dbg !38
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR25, i32 0, i32 0), i1 %GT50), !dbg !38
  %foo51 = call i32 @foo(), !dbg !39
  %b52 = load i32, i32* %b, align 4, !dbg !39
  %GT53 = icmp sgt i32 %b52, %foo51, !dbg !39
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR26, i32 0, i32 0), i1 %GT53), !dbg !39
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR27, i32 0, i32 0), i1 true), !dbg !40
  %foo54 = call i32 @foo(), !dbg !41
  %bar55 = call i32 @bar(), !dbg !41
  %GT56 = icmp sgt i32 %foo54, %bar55, !dbg !41
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR28, i32 0, i32 0), i1 %GT56), !dbg !41
  %b57 = load i32, i32* %b, align 4, !dbg !42
  %a58 = load i32, i32* %a, align 4, !dbg !42
  %GE = icmp sge i32 %b57, %a58, !dbg !42
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR29, i32 0, i32 0), i1 %GE), !dbg !42
  %a59 = load i32, i32* %a, align 4, !dbg !43
  %GE60 = icmp sge i32 %a59, 10, !dbg !43
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR30, i32 0, i32 0), i1 %GE60), !dbg !43
  %bar61 = call i32 @bar(), !dbg !44
  %b62 = load i32, i32* %b, align 4, !dbg !44
  %GE63 = icmp sge i32 %b62, %bar61, !dbg !44
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR31, i32 0, i32 0), i1 %GE63), !dbg !44
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR32, i32 0, i32 0), i1 true), !dbg !45
  %bar64 = call i32 @bar(), !dbg !46
  %foo65 = call i32 @foo(), !dbg !46
  %GE66 = icmp sge i32 %bar64, %foo65, !dbg !46
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR33, i32 0, i32 0), i1 %GE66), !dbg !46
  %foo67 = call i32 @foo(), !dbg !47
  %bar68 = call i32 @bar(), !dbg !47
  %GE69 = icmp sge i32 %foo67, %bar68, !dbg !47
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR34, i32 0, i32 0), i1 %GE69), !dbg !47
  %a70 = load i32, i32* %a, align 4, !dbg !48
  %b71 = load i32, i32* %b, align 4, !dbg !48
  %EQ72 = icmp eq i32 %a70, %b71, !dbg !48
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR35, i32 0, i32 0), i1 %EQ72), !dbg !48
  %foo73 = call i32 @foo(), !dbg !49
  %bar74 = call i32 @bar(), !dbg !49
  %EQ75 = icmp eq i32 %foo73, %bar74, !dbg !49
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR36, i32 0, i32 0), i1 %EQ75), !dbg !49
  %foo76 = call i32 @foo(), !dbg !50
  %foo77 = call i32 @foo(), !dbg !50
  %EQ78 = icmp eq i32 %foo76, %foo77, !dbg !50
  call void @print_bool(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR37, i32 0, i32 0), i1 %EQ78), !dbg !50
  br label %if.start, !dbg !50

if.start:                                         ; preds = %entry
  %a79 = load i32, i32* %a, align 4, !dbg !50
  %b80 = load i32, i32* %b, align 4, !dbg !50
  %LT81 = icmp slt i32 %a79, %b80, !dbg !50
  br i1 %LT81, label %if.then, label %if.end, !dbg !50

if.end:                                           ; preds = %if.end83, %if.start
  ret i32 0, !dbg !51

if.then:                                          ; preds = %if.start
  br label %if.start82, !dbg !50

if.start82:                                       ; preds = %if.then
  %foo85 = call i32 @foo(), !dbg !51
  %a86 = load i32, i32* %a, align 4, !dbg !51
  %EQ87 = icmp eq i32 %a86, %foo85, !dbg !51
  br i1 %EQ87, label %if.then84, label %if.end83, !dbg !51

if.end83:                                         ; preds = %if.then84, %if.start82
  br label %if.end, !dbg !51

if.then84:                                        ; preds = %if.start82
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !51
  br label %if.end83, !dbg !51
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "comparison.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op")
!4 = distinct !DISubprogram(name: "foo", linkageName: "foo", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "bar", linkageName: "bar", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
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
!21 = !DILocation(line: 26, scope: !12)
!22 = !DILocation(line: 27, scope: !12)
!23 = !DILocation(line: 28, scope: !12)
!24 = !DILocation(line: 29, scope: !12)
!25 = !DILocation(line: 30, scope: !12)
!26 = !DILocation(line: 33, scope: !12)
!27 = !DILocation(line: 34, scope: !12)
!28 = !DILocation(line: 35, scope: !12)
!29 = !DILocation(line: 36, scope: !12)
!30 = !DILocation(line: 37, scope: !12)
!31 = !DILocation(line: 40, scope: !12)
!32 = !DILocation(line: 41, scope: !12)
!33 = !DILocation(line: 42, scope: !12)
!34 = !DILocation(line: 43, scope: !12)
!35 = !DILocation(line: 44, scope: !12)
!36 = !DILocation(line: 45, scope: !12)
!37 = !DILocation(line: 48, scope: !12)
!38 = !DILocation(line: 49, scope: !12)
!39 = !DILocation(line: 50, scope: !12)
!40 = !DILocation(line: 51, scope: !12)
!41 = !DILocation(line: 52, scope: !12)
!42 = !DILocation(line: 55, scope: !12)
!43 = !DILocation(line: 56, scope: !12)
!44 = !DILocation(line: 57, scope: !12)
!45 = !DILocation(line: 58, scope: !12)
!46 = !DILocation(line: 59, scope: !12)
!47 = !DILocation(line: 60, scope: !12)
!48 = !DILocation(line: 63, scope: !12)
!49 = !DILocation(line: 64, scope: !12)
!50 = !DILocation(line: 65, scope: !12)
!51 = !DILocation(line: 69, scope: !12)
