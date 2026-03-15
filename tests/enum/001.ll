; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@I = constant i32 0
@O = constant i32 1
@T = constant i32 2
@S = constant i32 3
@Z = constant i32 4
@J = constant i32 5
@L = constant i32 6
@STR0 = private unnamed_addr constant [3 x i8] c"I=\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c" O=\00", align 1
@STR2 = private unnamed_addr constant [4 x i8] c" T=\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [16 x i8] c"I=%d O=%d T=%d\0A\00", align 1
@STR4 = private unnamed_addr constant [3 x i8] c"S=\00", align 1
@STR5 = private unnamed_addr constant [4 x i8] c" Z=\00", align 1
@STR6 = private unnamed_addr constant [4 x i8] c" J=\00", align 1
@STR7 = private unnamed_addr constant [4 x i8] c" L=\00", align 1
@STR8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [21 x i8] c"S=%d Z=%d J=%d L=%d\0A\00", align 1
@STR9 = private unnamed_addr constant [12 x i8] c"piece is T\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [12 x i8] c"piece is T\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %p = alloca i32, align 4, !dbg !7
  %I = load i32, i32* @I, align 4, !dbg !7
  %O = load i32, i32* @O, align 4, !dbg !7
  %T = load i32, i32* @T, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt, i32 0, i32 0), i32 3, i32 %I, i32 %O, i32 %T), !dbg !7
  %S = load i32, i32* @S, align 4, !dbg !7
  %Z = load i32, i32* @Z, align 4, !dbg !7
  %J = load i32, i32* @J, align 4, !dbg !7
  %L = load i32, i32* @L, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.1, i32 0, i32 0), i32 4, i32 %S, i32 %Z, i32 %J, i32 %L), !dbg !7
  store i32 0, i32* %p, align 4, !dbg !7
  %T1 = load i32, i32* @T, align 4, !dbg !7
  store i32 %T1, i32* %p, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %p2 = load i32, i32* %p, align 4, !dbg !7
  %T3 = load i32, i32* @T, align 4, !dbg !7
  %EQ = icmp eq i32 %p2, %T3, !dbg !7
  br i1 %EQ, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.then, %if.start
  ret i32 0, !dbg !7

if.then:                                          ; preds = %if.start
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.2, i32 0, i32 0), i32 0), !dbg !7
  br label %if.end, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
