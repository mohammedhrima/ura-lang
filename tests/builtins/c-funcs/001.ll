; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [11 x i8] c"Hero:  %s\0A\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"HP:    %d\0A\00", align 1
@STR3 = private unnamed_addr constant [11 x i8] c"Grade: %c\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"Speed: %f\0A\00", align 1
@STR5 = private unnamed_addr constant [24 x i8] c"%s \E2\80\94 HP %d, Grade %c\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %name = alloca i8*, align 8, !dbg !7
  %hp = alloca i32, align 4, !dbg !7
  %grade = alloca i8, align 1, !dbg !7
  %speed = alloca float, align 4, !dbg !7
  store i8* null, i8** %name, align 8, !dbg !7
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  store i32 0, i32* %hp, align 4, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  store i8 0, i8* %grade, align 1, !dbg !7
  store i8 83, i8* %grade, align 1, !dbg !7
  store float 0.000000e+00, float* %speed, align 4, !dbg !7
  store float 1.750000e+00, float* %speed, align 4, !dbg !7
  %name1 = load i8*, i8** %name, align 8, !dbg !8
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR1, i32 0, i32 0), i32 1, i8* %name1), !dbg !8
  %hp2 = load i32, i32* %hp, align 4, !dbg !9
  %printf3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR2, i32 0, i32 0), i32 1, i32 %hp2), !dbg !9
  %grade4 = load i8, i8* %grade, align 1, !dbg !10
  %printf5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR3, i32 0, i32 0), i32 1, i8 %grade4), !dbg !10
  %speed6 = load float, float* %speed, align 4, !dbg !11
  %printf7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR4, i32 0, i32 0), i32 1, float %speed6), !dbg !11
  %name8 = load i8*, i8** %name, align 8, !dbg !12
  %hp9 = load i32, i32* %hp, align 4, !dbg !12
  %grade10 = load i8, i8* %grade, align 1, !dbg !12
  %printf11 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @STR5, i32 0, i32 0), i32 3, i8* %name8, i32 %hp9, i8 %grade10), !dbg !12
  ret i32 0, !dbg !12
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 11, scope: !4)
!9 = !DILocation(line: 12, scope: !4)
!10 = !DILocation(line: 13, scope: !4)
!11 = !DILocation(line: 14, scope: !4)
!12 = !DILocation(line: 15, scope: !4)
