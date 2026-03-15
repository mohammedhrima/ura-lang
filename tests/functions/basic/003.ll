; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"Orc takes \00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c" damage\0A\00", align 1
@output_fmt = private unnamed_addr constant [21 x i8] c"Orc takes %d damage\0A\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"Orc dead: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [14 x i8] c"Orc dead: %s\0A\00", align 1

define i32 @clamp(i32 %val, i32 %lo, i32 %hi) !dbg !4 {
entry:
  %val1 = alloca i32, align 4, !dbg !7
  %lo2 = alloca i32, align 4, !dbg !7
  %hi3 = alloca i32, align 4, !dbg !7
  store i32 %val, i32* %val1, align 4, !dbg !7
  store i32 %lo, i32* %lo2, align 4, !dbg !7
  store i32 %hi, i32* %hi3, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %val4 = load i32, i32* %val1, align 4, !dbg !7
  %lo5 = load i32, i32* %lo2, align 4, !dbg !7
  %LT = icmp slt i32 %val4, %lo5, !dbg !7
  br i1 %LT, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.start
  br label %if.start7, !dbg !7

if.then:                                          ; preds = %if.start
  %lo6 = load i32, i32* %lo2, align 4, !dbg !7
  ret i32 %lo6, !dbg !7

if.start7:                                        ; preds = %if.end
  %val10 = load i32, i32* %val1, align 4, !dbg !7
  %hi11 = load i32, i32* %hi3, align 4, !dbg !7
  %GT = icmp sgt i32 %val10, %hi11, !dbg !7
  br i1 %GT, label %if.then9, label %if.end8, !dbg !7

if.end8:                                          ; preds = %if.start7
  %val13 = load i32, i32* %val1, align 4, !dbg !7
  ret i32 %val13, !dbg !7

if.then9:                                         ; preds = %if.start7
  %hi12 = load i32, i32* %hi3, align 4, !dbg !7
  ret i32 %hi12, !dbg !7
}

define i32 @damage(i32 %atk, i32 %def) !dbg !8 {
entry:
  %atk1 = alloca i32, align 4, !dbg !9
  %def2 = alloca i32, align 4, !dbg !9
  store i32 %atk, i32* %atk1, align 4, !dbg !9
  store i32 %def, i32* %def2, align 4, !dbg !9
  %atk3 = load i32, i32* %atk1, align 4, !dbg !10
  %def4 = load i32, i32* %def2, align 4, !dbg !10
  %SUB = sub i32 %atk3, %def4, !dbg !10
  %clamp = call i32 @clamp(i32 %SUB, i32 0, i32 999), !dbg !10
  ret i32 %clamp, !dbg !10
}

define i1 @is_dead(i32 %hp) !dbg !11 {
entry:
  %hp1 = alloca i32, align 4, !dbg !12
  store i32 %hp, i32* %hp1, align 4, !dbg !12
  %hp2 = load i32, i32* %hp1, align 4, !dbg !12
  %LE = icmp sle i32 %hp2, 0, !dbg !12
  ret i1 %LE, !dbg !12
}

define i32 @main() !dbg !13 {
entry:
  %d = alloca i32, align 4, !dbg !14
  store i32 0, i32* %d, align 4, !dbg !14
  %damage = call i32 @damage(i32 25, i32 8), !dbg !15
  store i32 %damage, i32* %d, align 4, !dbg !15
  %d1 = load i32, i32* %d, align 4, !dbg !15
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %d1), !dbg !15
  %d2 = load i32, i32* %d, align 4, !dbg !16
  %SUB = sub i32 %d2, 60, !dbg !16
  %is_dead = call i1 @is_dead(i32 %SUB), !dbg !16
  %bool_str = select i1 %is_dead, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !16
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %bool_str), !dbg !16
  ret i32 0, !dbg !16
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic")
!4 = distinct !DISubprogram(name: "clamp", linkageName: "clamp", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "damage", linkageName: "damage", scope: null, file: !3, line: 10, type: !5, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 10, scope: !8)
!10 = !DILocation(line: 11, scope: !8)
!11 = distinct !DISubprogram(name: "is_dead", linkageName: "is_dead", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!12 = !DILocation(line: 13, scope: !11)
!13 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 16, type: !5, scopeLine: 16, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 16, scope: !13)
!15 = !DILocation(line: 17, scope: !13)
!16 = !DILocation(line: 19, scope: !13)
