; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor01.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor01.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [5 x i8] c"=== \00", align 1
@STR2 = private unnamed_addr constant [25 x i8] c" enters the dungeon ===\0A\00", align 1
@output_fmt = private unnamed_addr constant [31 x i8] c"=== %s enters the dungeon ===\0A\00", align 1
@STR3 = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@STR4 = private unnamed_addr constant [7 x i8] c"  MP: \00", align 1
@STR5 = private unnamed_addr constant [10 x i8] c"  Speed: \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [27 x i8] c"HP: %d  MP: %d  Speed: %f\0A\00", align 1
@STR7 = private unnamed_addr constant [12 x i8] c"typeof hp: \00", align 1
@typeof = private unnamed_addr constant [4 x i8] c"INT\00", align 1
@STR8 = private unnamed_addr constant [21 x i8] c"  sizeof chars ptr: \00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [37 x i8] c"typeof hp: %s  sizeof chars ptr: %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %hero = alloca i8*, align 8, !dbg !7
  %hp = alloca i32, align 4, !dbg !7
  %mp = alloca i32, align 4, !dbg !7
  %alive = alloca i1, align 1, !dbg !7
  %speed = alloca float, align 4, !dbg !7
  store i8* null, i8** %hero, align 8, !dbg !7
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %hero, align 8, !dbg !7
  store i32 0, i32* %hp, align 4, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  store i32 0, i32* %mp, align 4, !dbg !7
  store i32 50, i32* %mp, align 4, !dbg !7
  store i1 false, i1* %alive, align 1, !dbg !7
  store i1 true, i1* %alive, align 1, !dbg !7
  store float 0.000000e+00, float* %speed, align 4, !dbg !7
  store float 1.500000e+00, float* %speed, align 4, !dbg !7
  %hero1 = load i8*, i8** %hero, align 8, !dbg !7
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @output_fmt, i32 0, i32 0), i8* %hero1), !dbg !7
  %hp2 = load i32, i32* %hp, align 4, !dbg !7
  %mp3 = load i32, i32* %mp, align 4, !dbg !7
  %speed4 = load float, float* %speed, align 4, !dbg !7
  %f2d = fpext float %speed4 to double, !dbg !7
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @output_fmt.1, i32 0, i32 0), i32 %hp2, i32 %mp3, double %f2d), !dbg !7
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @output_fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @typeof, i32 0, i32 0), i32 8), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor01.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 2, type: !5, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 2, scope: !4)
