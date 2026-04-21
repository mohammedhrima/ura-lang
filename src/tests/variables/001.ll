; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/variables/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/variables/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [5 x i8] c"=== \00", align 1
@STR2 = private unnamed_addr constant [25 x i8] c" enters the dungeon ===\0A\00", align 1
@output_fmt = private unnamed_addr constant [31 x i8] c"=== %s enters the dungeon ===\0A\00", align 1
@STR3 = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@STR4 = private unnamed_addr constant [8 x i8] c" | MP: \00", align 1
@STR5 = private unnamed_addr constant [11 x i8] c" | Speed: \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [29 x i8] c"HP: %d | MP: %d | Speed: %f\0A\00", align 1
@STR7 = private unnamed_addr constant [8 x i8] c"Alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %name = alloca i8*, align 8, !dbg !7
  %hp = alloca i32, align 4, !dbg !7
  %mp = alloca i32, align 4, !dbg !7
  %alive = alloca i1, align 1, !dbg !7
  %speed = alloca float, align 4, !dbg !7
  store i8* null, i8** %name, align 8, !dbg !7
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  store i32 0, i32* %hp, align 4, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  store i32 0, i32* %mp, align 4, !dbg !7
  store i32 50, i32* %mp, align 4, !dbg !7
  store i1 false, i1* %alive, align 1, !dbg !7
  store i1 true, i1* %alive, align 1, !dbg !7
  store float 0.000000e+00, float* %speed, align 4, !dbg !7
  store float 1.500000e+00, float* %speed, align 4, !dbg !7
  %name1 = load i8*, i8** %name, align 8, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %name1), !dbg !7
  %hp2 = load i32, i32* %hp, align 4, !dbg !7
  %mp3 = load i32, i32* %mp, align 4, !dbg !7
  %speed4 = load float, float* %speed, align 4, !dbg !7
  %f2d = fpext float %speed4 to double, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @output_fmt.1, i32 0, i32 0), i32 3, i32 %hp2, i32 %mp3, double %f2d), !dbg !7
  %alive5 = load i1, i1* %alive, align 1, !dbg !7
  %bool_str = select i1 %alive5, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* %bool_str), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/variables")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
