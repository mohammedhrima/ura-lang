; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/logical/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/logical/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [14 x i8] c"can_proceed: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [17 x i8] c"can_proceed: %s\0A\00", align 1
@STR2 = private unnamed_addr constant [14 x i8] c"escape:      \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [17 x i8] c"escape:      %s\0A\00", align 1
@STR4 = private unnamed_addr constant [14 x i8] c"blocked:     \00", align 1
@true_str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [17 x i8] c"blocked:     %s\0A\00", align 1
@STR6 = private unnamed_addr constant [13 x i8] c"hero fights\0A\00", align 1
@output_fmt.7 = private unnamed_addr constant [13 x i8] c"hero fights\0A\00", align 1
@STR7 = private unnamed_addr constant [15 x i8] c"hero retreats\0A\00", align 1
@output_fmt.8 = private unnamed_addr constant [15 x i8] c"hero retreats\0A\00", align 1
@STR8 = private unnamed_addr constant [15 x i8] c"hero advances\0A\00", align 1
@output_fmt.9 = private unnamed_addr constant [15 x i8] c"hero advances\0A\00", align 1
@STR9 = private unnamed_addr constant [10 x i8] c"!!alive: \00", align 1
@true_str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.12 = private unnamed_addr constant [13 x i8] c"!!alive: %s\0A\00", align 1

define i1 @is_alive(i32 %hp) !dbg !4 {
entry:
  %hp1 = alloca i32, align 4, !dbg !7
  store i32 %hp, i32* %hp1, align 4, !dbg !7
  %hp2 = load i32, i32* %hp1, align 4, !dbg !7
  %GT = icmp sgt i32 %hp2, 0, !dbg !7
  ret i1 %GT, !dbg !7
}

define i1 @has_key(i32 %keys) !dbg !8 {
entry:
  %keys1 = alloca i32, align 4, !dbg !9
  store i32 %keys, i32* %keys1, align 4, !dbg !9
  %keys2 = load i32, i32* %keys1, align 4, !dbg !9
  %GT = icmp sgt i32 %keys2, 0, !dbg !9
  ret i1 %GT, !dbg !9
}

define i32 @main() !dbg !10 {
entry:
  %hp = alloca i32, align 4, !dbg !11
  %keys = alloca i32, align 4, !dbg !11
  %exhausted = alloca i1, align 1, !dbg !11
  %can_proceed = alloca i1, align 1, !dbg !11
  %escape = alloca i1, align 1, !dbg !11
  %blocked = alloca i1, align 1, !dbg !11
  %alive = alloca i1, align 1, !dbg !11
  store i32 0, i32* %hp, align 4, !dbg !11
  store i32 60, i32* %hp, align 4, !dbg !11
  store i32 0, i32* %keys, align 4, !dbg !11
  store i32 1, i32* %keys, align 4, !dbg !11
  store i1 false, i1* %exhausted, align 1, !dbg !11
  store i1 false, i1* %exhausted, align 1, !dbg !11
  store i1 false, i1* %can_proceed, align 1, !dbg !11
  %hp1 = load i32, i32* %hp, align 4, !dbg !12
  %is_alive = call i1 @is_alive(i32 %hp1), !dbg !12
  %keys2 = load i32, i32* %keys, align 4, !dbg !12
  %has_key = call i1 @has_key(i32 %keys2), !dbg !12
  %AND = and i1 %is_alive, %has_key, !dbg !12
  %exhausted3 = load i1, i1* %exhausted, align 1, !dbg !12
  %NOT = xor i1 %exhausted3, true, !dbg !12
  %AND4 = and i1 %AND, %NOT, !dbg !12
  store i1 %AND4, i1* %can_proceed, align 1, !dbg !12
  %can_proceed5 = load i1, i1* %can_proceed, align 1, !dbg !12
  %bool_str = select i1 %can_proceed5, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !12
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %bool_str), !dbg !12
  store i1 false, i1* %escape, align 1, !dbg !11
  %hp6 = load i32, i32* %hp, align 4, !dbg !11
  %LT = icmp slt i32 %hp6, 10, !dbg !11
  %keys7 = load i32, i32* %keys, align 4, !dbg !11
  %EQ = icmp eq i32 %keys7, 0, !dbg !11
  %OR = or i1 %LT, %EQ, !dbg !11
  store i1 %OR, i1* %escape, align 1, !dbg !11
  %escape8 = load i1, i1* %escape, align 1, !dbg !11
  %bool_str9 = select i1 %escape8, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0), !dbg !11
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i8* %bool_str9), !dbg !11
  store i1 false, i1* %blocked, align 1, !dbg !11
  %can_proceed10 = load i1, i1* %can_proceed, align 1, !dbg !11
  %NOT11 = xor i1 %can_proceed10, true, !dbg !11
  store i1 %NOT11, i1* %blocked, align 1, !dbg !11
  %blocked12 = load i1, i1* %blocked, align 1, !dbg !11
  %bool_str13 = select i1 %blocked12, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.4, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.5, i32 0, i32 0), !dbg !11
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i8* %bool_str13), !dbg !11
  br label %if.start, !dbg !11

if.start:                                         ; preds = %entry
  %hp14 = load i32, i32* %hp, align 4, !dbg !13
  %is_alive15 = call i1 @is_alive(i32 %hp14), !dbg !13
  %exhausted16 = load i1, i1* %exhausted, align 1, !dbg !13
  %NOT17 = xor i1 %exhausted16, true, !dbg !13
  %AND18 = and i1 %is_alive15, %NOT17, !dbg !13
  br i1 %AND18, label %if.then, label %if.end, !dbg !13

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start19, !dbg !13

if.then:                                          ; preds = %if.start
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt.7, i32 0, i32 0), i32 0), !dbg !13
  br label %if.end, !dbg !13

if.start19:                                       ; preds = %if.end
  %hp22 = load i32, i32* %hp, align 4, !dbg !13
  %LT23 = icmp slt i32 %hp22, 10, !dbg !13
  %keys24 = load i32, i32* %keys, align 4, !dbg !13
  %EQ25 = icmp eq i32 %keys24, 0, !dbg !13
  %OR26 = or i1 %LT23, %EQ25, !dbg !13
  br i1 %OR26, label %if.then21, label %if.else, !dbg !13

if.end20:                                         ; preds = %if.else, %if.then21
  store i1 false, i1* %alive, align 1, !dbg !11
  %hp27 = load i32, i32* %hp, align 4, !dbg !14
  %is_alive28 = call i1 @is_alive(i32 %hp27), !dbg !14
  store i1 %is_alive28, i1* %alive, align 1, !dbg !14
  %alive29 = load i1, i1* %alive, align 1, !dbg !14
  %NOT30 = xor i1 %alive29, true, !dbg !14
  %NOT31 = xor i1 %NOT30, true, !dbg !14
  %bool_str32 = select i1 %NOT31, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.10, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.11, i32 0, i32 0), !dbg !14
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt.12, i32 0, i32 0), i32 1, i8* %bool_str32), !dbg !14
  ret i32 0, !dbg !14

if.then21:                                        ; preds = %if.start19
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.8, i32 0, i32 0), i32 0), !dbg !13
  br label %if.end20, !dbg !13

if.else:                                          ; preds = %if.start19
  %6 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.9, i32 0, i32 0), i32 0), !dbg !13
  br label %if.end20, !dbg !13
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/logical")
!4 = distinct !DISubprogram(name: "is_alive", linkageName: "is_alive", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "has_key", linkageName: "has_key", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 9, scope: !10)
!12 = !DILocation(line: 15, scope: !10)
!13 = !DILocation(line: 27, scope: !10)
!14 = !DILocation(line: 36, scope: !10)
