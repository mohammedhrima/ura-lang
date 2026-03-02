; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/op/comparison.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/op/comparison.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [8 x i8] c"%sTrue\0A\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c"%sFalse\0A\00", align 1
@STR2 = private unnamed_addr constant [20 x i8] c"eq  var==var (F):  \00", align 1
@STR3 = private unnamed_addr constant [20 x i8] c"eq  var==val (T):  \00", align 1
@STR4 = private unnamed_addr constant [20 x i8] c"eq  var==fn  (T):  \00", align 1
@STR5 = private unnamed_addr constant [20 x i8] c"eq  val==var (T):  \00", align 1
@STR6 = private unnamed_addr constant [20 x i8] c"eq  val==val (T):  \00", align 1
@STR7 = private unnamed_addr constant [20 x i8] c"eq  fn==fn   (F):  \00", align 1
@STR8 = private unnamed_addr constant [20 x i8] c"eq  fn==fn   (T):  \00", align 1
@STR9 = private unnamed_addr constant [20 x i8] c"neq var!=var (T):  \00", align 1
@STR10 = private unnamed_addr constant [20 x i8] c"neq var!=val (F):  \00", align 1
@STR11 = private unnamed_addr constant [20 x i8] c"neq var!=fn  (F):  \00", align 1
@STR12 = private unnamed_addr constant [20 x i8] c"neq val!=val (F):  \00", align 1
@STR13 = private unnamed_addr constant [20 x i8] c"neq fn!=fn   (T):  \00", align 1
@STR14 = private unnamed_addr constant [20 x i8] c"lt  var<var  (T):  \00", align 1
@STR15 = private unnamed_addr constant [20 x i8] c"lt  var<val  (F):  \00", align 1
@STR16 = private unnamed_addr constant [20 x i8] c"lt  var<fn   (F):  \00", align 1
@STR17 = private unnamed_addr constant [20 x i8] c"lt  val<val  (T):  \00", align 1
@STR18 = private unnamed_addr constant [20 x i8] c"lt  fn<fn    (T):  \00", align 1
@STR19 = private unnamed_addr constant [20 x i8] c"le  var<=var (T):  \00", align 1
@STR20 = private unnamed_addr constant [20 x i8] c"le  var<=val (T):  \00", align 1
@STR21 = private unnamed_addr constant [20 x i8] c"le  var<=fn  (T):  \00", align 1
@STR22 = private unnamed_addr constant [20 x i8] c"le  val<=val (T):  \00", align 1
@STR23 = private unnamed_addr constant [20 x i8] c"le  fn<=fn   (T):  \00", align 1
@STR24 = private unnamed_addr constant [20 x i8] c"le  fn<=fn   (F):  \00", align 1
@STR25 = private unnamed_addr constant [20 x i8] c"gt  var>var  (F):  \00", align 1
@STR26 = private unnamed_addr constant [20 x i8] c"gt  var>val  (T):  \00", align 1
@STR27 = private unnamed_addr constant [20 x i8] c"gt  var>fn   (T):  \00", align 1
@STR28 = private unnamed_addr constant [20 x i8] c"gt  val>val  (T):  \00", align 1
@STR29 = private unnamed_addr constant [20 x i8] c"gt  fn>fn    (F):  \00", align 1
@STR30 = private unnamed_addr constant [20 x i8] c"ge  var>=var (T):  \00", align 1
@STR31 = private unnamed_addr constant [20 x i8] c"ge  var>=val (T):  \00", align 1
@STR32 = private unnamed_addr constant [20 x i8] c"ge  var>=fn  (T):  \00", align 1
@STR33 = private unnamed_addr constant [20 x i8] c"ge  val>=val (T):  \00", align 1
@STR34 = private unnamed_addr constant [20 x i8] c"ge  fn>=fn   (T):  \00", align 1
@STR35 = private unnamed_addr constant [20 x i8] c"ge  fn>=fn   (F):  \00", align 1
@STR36 = private unnamed_addr constant [20 x i8] c"is  var is var(F): \00", align 1
@STR37 = private unnamed_addr constant [20 x i8] c"is  fn  is fn (F): \00", align 1
@STR38 = private unnamed_addr constant [20 x i8] c"is  fn  is fn (T): \00", align 1
@STR39 = private unnamed_addr constant [29 x i8] c"chain: a<b and a==foo: True\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @foo() !dbg !4 {
entry:
  ret i32 10, !dbg !7
}

define i32 @bar() !dbg !8 {
entry:
  ret i32 20, !dbg !9
}

define void @print_bool(ptr %label, i1 %val) !dbg !10 {
entry:
  %label1 = alloca ptr, align 8, !dbg !11
  %val2 = alloca i1, align 1, !dbg !11
  store i1 false, ptr %val2, align 1, !dbg !11
  store ptr null, ptr %label1, align 8, !dbg !11
  store ptr %label, ptr %label1, align 8, !dbg !11
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
  %a = alloca i32, align 4, !dbg !15
  %b = alloca i32, align 4, !dbg !15
  store i32 0, ptr %b, align 4, !dbg !15
  store i32 0, ptr %a, align 4, !dbg !15
  store i32 10, ptr %a, align 4, !dbg !15
  store i32 20, ptr %b, align 4, !dbg !15
  %a1 = load i32, ptr %a, align 4, !dbg !16
  %b2 = load i32, ptr %b, align 4, !dbg !16
  %EQ = icmp eq i32 %a1, %b2, !dbg !16
  call void @print_bool(ptr @STR2, i1 %EQ), !dbg !16
  %a3 = load i32, ptr %a, align 4, !dbg !17
  %EQ4 = icmp eq i32 %a3, 10, !dbg !17
  call void @print_bool(ptr @STR3, i1 %EQ4), !dbg !17
  %foo = call i32 @foo(), !dbg !18
  %a5 = load i32, ptr %a, align 4, !dbg !18
  %EQ6 = icmp eq i32 %a5, %foo, !dbg !18
  call void @print_bool(ptr @STR4, i1 %EQ6), !dbg !18
  %b7 = load i32, ptr %b, align 4, !dbg !19
  %EQ8 = icmp eq i32 20, %b7, !dbg !19
  call void @print_bool(ptr @STR5, i1 %EQ8), !dbg !19
  call void @print_bool(ptr @STR6, i1 true), !dbg !20
  %foo9 = call i32 @foo(), !dbg !21
  %bar = call i32 @bar(), !dbg !21
  %EQ10 = icmp eq i32 %foo9, %bar, !dbg !21
  call void @print_bool(ptr @STR7, i1 %EQ10), !dbg !21
  %foo11 = call i32 @foo(), !dbg !22
  %foo12 = call i32 @foo(), !dbg !22
  %EQ13 = icmp eq i32 %foo11, %foo12, !dbg !22
  call void @print_bool(ptr @STR8, i1 %EQ13), !dbg !22
  %a14 = load i32, ptr %a, align 4, !dbg !23
  %b15 = load i32, ptr %b, align 4, !dbg !23
  %NEQ = icmp ne i32 %a14, %b15, !dbg !23
  call void @print_bool(ptr @STR9, i1 %NEQ), !dbg !23
  %a16 = load i32, ptr %a, align 4, !dbg !24
  %NEQ17 = icmp ne i32 %a16, 10, !dbg !24
  call void @print_bool(ptr @STR10, i1 %NEQ17), !dbg !24
  %foo18 = call i32 @foo(), !dbg !25
  %a19 = load i32, ptr %a, align 4, !dbg !25
  %NEQ20 = icmp ne i32 %a19, %foo18, !dbg !25
  call void @print_bool(ptr @STR11, i1 %NEQ20), !dbg !25
  call void @print_bool(ptr @STR12, i1 false), !dbg !26
  %foo21 = call i32 @foo(), !dbg !27
  %bar22 = call i32 @bar(), !dbg !27
  %NEQ23 = icmp ne i32 %foo21, %bar22, !dbg !27
  call void @print_bool(ptr @STR13, i1 %NEQ23), !dbg !27
  %a24 = load i32, ptr %a, align 4, !dbg !28
  %b25 = load i32, ptr %b, align 4, !dbg !28
  %LT = icmp slt i32 %a24, %b25, !dbg !28
  call void @print_bool(ptr @STR14, i1 %LT), !dbg !28
  %a26 = load i32, ptr %a, align 4, !dbg !29
  %LT27 = icmp slt i32 %a26, 5, !dbg !29
  call void @print_bool(ptr @STR15, i1 %LT27), !dbg !29
  %foo28 = call i32 @foo(), !dbg !30
  %b29 = load i32, ptr %b, align 4, !dbg !30
  %LT30 = icmp slt i32 %b29, %foo28, !dbg !30
  call void @print_bool(ptr @STR16, i1 %LT30), !dbg !30
  call void @print_bool(ptr @STR17, i1 true), !dbg !31
  %foo31 = call i32 @foo(), !dbg !32
  %bar32 = call i32 @bar(), !dbg !32
  %LT33 = icmp slt i32 %foo31, %bar32, !dbg !32
  call void @print_bool(ptr @STR18, i1 %LT33), !dbg !32
  %a34 = load i32, ptr %a, align 4, !dbg !33
  %b35 = load i32, ptr %b, align 4, !dbg !33
  %LE = icmp sle i32 %a34, %b35, !dbg !33
  call void @print_bool(ptr @STR19, i1 %LE), !dbg !33
  %a36 = load i32, ptr %a, align 4, !dbg !34
  %LE37 = icmp sle i32 %a36, 10, !dbg !34
  call void @print_bool(ptr @STR20, i1 %LE37), !dbg !34
  %foo38 = call i32 @foo(), !dbg !35
  %a39 = load i32, ptr %a, align 4, !dbg !35
  %LE40 = icmp sle i32 %a39, %foo38, !dbg !35
  call void @print_bool(ptr @STR21, i1 %LE40), !dbg !35
  call void @print_bool(ptr @STR22, i1 true), !dbg !36
  %foo41 = call i32 @foo(), !dbg !37
  %bar42 = call i32 @bar(), !dbg !37
  %LE43 = icmp sle i32 %foo41, %bar42, !dbg !37
  call void @print_bool(ptr @STR23, i1 %LE43), !dbg !37
  %bar44 = call i32 @bar(), !dbg !38
  %foo45 = call i32 @foo(), !dbg !38
  %LE46 = icmp sle i32 %bar44, %foo45, !dbg !38
  call void @print_bool(ptr @STR24, i1 %LE46), !dbg !38
  %a47 = load i32, ptr %a, align 4, !dbg !39
  %b48 = load i32, ptr %b, align 4, !dbg !39
  %GT = icmp sgt i32 %a47, %b48, !dbg !39
  call void @print_bool(ptr @STR25, i1 %GT), !dbg !39
  %b49 = load i32, ptr %b, align 4, !dbg !40
  %GT50 = icmp sgt i32 %b49, 10, !dbg !40
  call void @print_bool(ptr @STR26, i1 %GT50), !dbg !40
  %foo51 = call i32 @foo(), !dbg !41
  %b52 = load i32, ptr %b, align 4, !dbg !41
  %GT53 = icmp sgt i32 %b52, %foo51, !dbg !41
  call void @print_bool(ptr @STR27, i1 %GT53), !dbg !41
  call void @print_bool(ptr @STR28, i1 true), !dbg !42
  %foo54 = call i32 @foo(), !dbg !43
  %bar55 = call i32 @bar(), !dbg !43
  %GT56 = icmp sgt i32 %foo54, %bar55, !dbg !43
  call void @print_bool(ptr @STR29, i1 %GT56), !dbg !43
  %b57 = load i32, ptr %b, align 4, !dbg !44
  %a58 = load i32, ptr %a, align 4, !dbg !44
  %GE = icmp sge i32 %b57, %a58, !dbg !44
  call void @print_bool(ptr @STR30, i1 %GE), !dbg !44
  %a59 = load i32, ptr %a, align 4, !dbg !45
  %GE60 = icmp sge i32 %a59, 10, !dbg !45
  call void @print_bool(ptr @STR31, i1 %GE60), !dbg !45
  %bar61 = call i32 @bar(), !dbg !46
  %b62 = load i32, ptr %b, align 4, !dbg !46
  %GE63 = icmp sge i32 %b62, %bar61, !dbg !46
  call void @print_bool(ptr @STR32, i1 %GE63), !dbg !46
  call void @print_bool(ptr @STR33, i1 true), !dbg !47
  %bar64 = call i32 @bar(), !dbg !48
  %foo65 = call i32 @foo(), !dbg !48
  %GE66 = icmp sge i32 %bar64, %foo65, !dbg !48
  call void @print_bool(ptr @STR34, i1 %GE66), !dbg !48
  %foo67 = call i32 @foo(), !dbg !49
  %bar68 = call i32 @bar(), !dbg !49
  %GE69 = icmp sge i32 %foo67, %bar68, !dbg !49
  call void @print_bool(ptr @STR35, i1 %GE69), !dbg !49
  %a70 = load i32, ptr %a, align 4, !dbg !50
  %b71 = load i32, ptr %b, align 4, !dbg !50
  %EQ72 = icmp eq i32 %a70, %b71, !dbg !50
  call void @print_bool(ptr @STR36, i1 %EQ72), !dbg !50
  %foo73 = call i32 @foo(), !dbg !51
  %bar74 = call i32 @bar(), !dbg !51
  %EQ75 = icmp eq i32 %foo73, %bar74, !dbg !51
  call void @print_bool(ptr @STR37, i1 %EQ75), !dbg !51
  %foo76 = call i32 @foo(), !dbg !52
  %foo77 = call i32 @foo(), !dbg !52
  %EQ78 = icmp eq i32 %foo76, %foo77, !dbg !52
  call void @print_bool(ptr @STR38, i1 %EQ78), !dbg !52
  br label %if.start, !dbg !52

if.start:                                         ; preds = %entry
  %a79 = load i32, ptr %a, align 4, !dbg !52
  %b80 = load i32, ptr %b, align 4, !dbg !52
  %LT81 = icmp slt i32 %a79, %b80, !dbg !52
  br i1 %LT81, label %if.then, label %if.end, !dbg !52

if.end:                                           ; preds = %if.end83, %if.start
  ret i32 0, !dbg !53

if.then:                                          ; preds = %if.start
  br label %if.start82, !dbg !52

if.start82:                                       ; preds = %if.then
  %foo85 = call i32 @foo(), !dbg !54
  %a86 = load i32, ptr %a, align 4, !dbg !54
  %EQ87 = icmp eq i32 %a86, %foo85, !dbg !54
  br i1 %EQ87, label %if.then84, label %if.end83, !dbg !54

if.end83:                                         ; preds = %if.then84, %if.start82
  br label %if.end, !dbg !53

if.then84:                                        ; preds = %if.start82
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR39, i32 0), !dbg !53
  br label %if.end83, !dbg !53
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "comparison.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/op")
!4 = distinct !DISubprogram(name: "foo", linkageName: "foo", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "bar", linkageName: "bar", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2)
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
!23 = !DILocation(line: 29, scope: !14)
!24 = !DILocation(line: 30, scope: !14)
!25 = !DILocation(line: 31, scope: !14)
!26 = !DILocation(line: 32, scope: !14)
!27 = !DILocation(line: 33, scope: !14)
!28 = !DILocation(line: 36, scope: !14)
!29 = !DILocation(line: 37, scope: !14)
!30 = !DILocation(line: 38, scope: !14)
!31 = !DILocation(line: 39, scope: !14)
!32 = !DILocation(line: 40, scope: !14)
!33 = !DILocation(line: 43, scope: !14)
!34 = !DILocation(line: 44, scope: !14)
!35 = !DILocation(line: 45, scope: !14)
!36 = !DILocation(line: 46, scope: !14)
!37 = !DILocation(line: 47, scope: !14)
!38 = !DILocation(line: 48, scope: !14)
!39 = !DILocation(line: 51, scope: !14)
!40 = !DILocation(line: 52, scope: !14)
!41 = !DILocation(line: 53, scope: !14)
!42 = !DILocation(line: 54, scope: !14)
!43 = !DILocation(line: 55, scope: !14)
!44 = !DILocation(line: 58, scope: !14)
!45 = !DILocation(line: 59, scope: !14)
!46 = !DILocation(line: 60, scope: !14)
!47 = !DILocation(line: 61, scope: !14)
!48 = !DILocation(line: 62, scope: !14)
!49 = !DILocation(line: 63, scope: !14)
!50 = !DILocation(line: 66, scope: !14)
!51 = !DILocation(line: 67, scope: !14)
!52 = !DILocation(line: 68, scope: !14)
!53 = !DILocation(line: 73, scope: !14)
!54 = !DILocation(line: 72, scope: !14)
