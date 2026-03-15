; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/003.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Counter = type { i32, i8* }

@STR0 = private unnamed_addr constant [8 x i8] c"counter\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c":\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [7 x i8] c"%s:%d\0A\00", align 1
@c = global %struct.Counter zeroinitializer

declare void @free(i8*)

declare i8* @strdup(i8*)

define void @Counter.init(%struct.Counter* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Counter*, align 8, !dbg !7
  store %struct.Counter* %self, %struct.Counter** %self1, align 8, !dbg !7
  %self2 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !7
  %self3 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !7
  %value = getelementptr %struct.Counter, %struct.Counter* %self3, i32 0, i32 0, !dbg !7
  store i32 0, i32* %value, align 4, !dbg !7
  %self4 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !7
  %label = getelementptr %struct.Counter, %struct.Counter* %self4, i32 0, i32 1, !dbg !7
  %strdup = call i8* @strdup(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR0, i32 0, i32 0)), !dbg !8
  store i8* %strdup, i8** %label, align 8, !dbg !8
  ret void, !dbg !8
}

define void @Counter.tick(%struct.Counter* %self) !dbg !9 {
entry:
  %self1 = alloca %struct.Counter*, align 8, !dbg !10
  store %struct.Counter* %self, %struct.Counter** %self1, align 8, !dbg !10
  %self2 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !10
  %value = getelementptr %struct.Counter, %struct.Counter* %self2, i32 0, i32 0, !dbg !10
  %current = load i32, i32* %value, align 4, !dbg !10
  %ADD = add i32 %current, 1, !dbg !10
  store i32 %ADD, i32* %value, align 4, !dbg !10
  ret void, !dbg !10
}

define void @Counter.print(%struct.Counter* %self) !dbg !11 {
entry:
  %self1 = alloca %struct.Counter*, align 8, !dbg !12
  store %struct.Counter* %self, %struct.Counter** %self1, align 8, !dbg !12
  %self2 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !12
  %label = getelementptr %struct.Counter, %struct.Counter* %self2, i32 0, i32 1, !dbg !12
  %0 = load i8*, i8** %label, align 8, !dbg !12
  %self3 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !12
  %value = getelementptr %struct.Counter, %struct.Counter* %self3, i32 0, i32 0, !dbg !12
  %1 = load i32, i32* %value, align 4, !dbg !12
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %0, i32 %1), !dbg !12
  ret void, !dbg !12
}

declare i32 @printf(i8*, i32, ...)

define void @Counter.clean(%struct.Counter* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.Counter*, align 8, !dbg !14
  store %struct.Counter* %self, %struct.Counter** %self1, align 8, !dbg !14
  %self2 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !15
  %label = getelementptr %struct.Counter, %struct.Counter* %self2, i32 0, i32 1, !dbg !15
  %DOT = load i8*, i8** %label, align 8, !dbg !15
  call void @free(i8* %DOT), !dbg !15
  %self3 = load %struct.Counter*, %struct.Counter** %self1, align 8, !dbg !15
  ret void, !dbg !15
}

define i32 @main() !dbg !16 {
entry:
  call void @Counter.init(%struct.Counter* @c), !dbg !17
  call void @Counter.tick(%struct.Counter* @c), !dbg !18
  call void @Counter.tick(%struct.Counter* @c), !dbg !19
  call void @Counter.tick(%struct.Counter* @c), !dbg !20
  call void @Counter.print(%struct.Counter* @c), !dbg !21
  call void @Counter.clean(%struct.Counter* @c), !dbg !21
  ret i32 0, !dbg !21
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals")
!4 = distinct !DISubprogram(name: "Counter.init", linkageName: "Counter.init", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
!8 = !DILocation(line: 10, scope: !4)
!9 = distinct !DISubprogram(name: "Counter.tick", linkageName: "Counter.tick", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 12, scope: !9)
!11 = distinct !DISubprogram(name: "Counter.print", linkageName: "Counter.print", scope: null, file: !3, line: 15, type: !5, scopeLine: 15, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!12 = !DILocation(line: 15, scope: !11)
!13 = distinct !DISubprogram(name: "Counter.clean", linkageName: "Counter.clean", scope: null, file: !3, line: 18, type: !5, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 18, scope: !13)
!15 = !DILocation(line: 19, scope: !13)
!16 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 23, scope: !16)
!18 = !DILocation(line: 24, scope: !16)
!19 = !DILocation(line: 25, scope: !16)
!20 = !DILocation(line: 26, scope: !16)
!21 = !DILocation(line: 27, scope: !16)
