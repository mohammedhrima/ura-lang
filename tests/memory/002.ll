; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [7 x i8] c"Hero: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [10 x i8] c"Hero: %s\0A\00", align 1

declare i8* @strcpy(i8*, i8*)

define i32 @main() !dbg !4 {
entry:
  %name = alloca i8*, align 8, !dbg !7
  store i8* null, i8** %name, align 8, !dbg !7
  %stack = alloca [32 x i8], align 1, !dbg !7
  %stack1 = getelementptr [32 x i8], [32 x i8]* %stack, i32 0, i32 0, !dbg !7
  store i8* %stack1, i8** %name, align 8, !dbg !7
  %name2 = load i8*, i8** %name, align 8, !dbg !8
  %strcpy = call i8* @strcpy(i8* %name2, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0)), !dbg !8
  %name3 = load i8*, i8** %name, align 8, !dbg !8
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %name3), !dbg !8
  ret i32 0, !dbg !8
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
