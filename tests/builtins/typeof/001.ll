; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/typeof/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/typeof/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [15 x i8] c"typeof hp:    \00", align 1
@typeof = private unnamed_addr constant [4 x i8] c"INT\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [18 x i8] c"typeof hp:    %s\0A\00", align 1
@STR3 = private unnamed_addr constant [15 x i8] c"typeof speed: \00", align 1
@typeof.1 = private unnamed_addr constant [6 x i8] c"FLOAT\00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [18 x i8] c"typeof speed: %s\0A\00", align 1
@STR5 = private unnamed_addr constant [15 x i8] c"typeof name:  \00", align 1
@typeof.3 = private unnamed_addr constant [6 x i8] c"CHARS\00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [18 x i8] c"typeof name:  %s\0A\00", align 1
@STR7 = private unnamed_addr constant [15 x i8] c"typeof alive: \00", align 1
@typeof.5 = private unnamed_addr constant [5 x i8] c"BOOL\00", align 1
@STR8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [18 x i8] c"typeof alive: %s\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %hp = alloca i32, align 4, !dbg !7
  %speed = alloca float, align 4, !dbg !7
  %name = alloca i8*, align 8, !dbg !7
  %alive = alloca i1, align 1, !dbg !7
  store i32 0, i32* %hp, align 4, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  store float 0.000000e+00, float* %speed, align 4, !dbg !7
  store float 1.500000e+00, float* %speed, align 4, !dbg !7
  store i8* null, i8** %name, align 8, !dbg !7
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  store i1 false, i1* %alive, align 1, !dbg !7
  store i1 true, i1* %alive, align 1, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @typeof, i32 0, i32 0)), !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @typeof.1, i32 0, i32 0)), !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @typeof.3, i32 0, i32 0)), !dbg !7
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @typeof.5, i32 0, i32 0)), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/typeof")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
