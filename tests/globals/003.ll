; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/003.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.KillCounter = type { i32, i8* }

@STR0 = private unnamed_addr constant [6 x i8] c"kills\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c": \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [8 x i8] c"%s: %d\0A\00", align 1
@counter = global %struct.KillCounter zeroinitializer

declare void @free(i8*)

declare i8* @strdup(i8*)

define void @KillCounter.init(%struct.KillCounter* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.KillCounter*, align 8, !dbg !7
  store %struct.KillCounter* %self, %struct.KillCounter** %self1, align 8, !dbg !7
  %self2 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !7
  %self3 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !7
  %value = getelementptr %struct.KillCounter, %struct.KillCounter* %self3, i32 0, i32 0, !dbg !7
  store i32 0, i32* %value, align 4, !dbg !7
  %self4 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !7
  %label = getelementptr %struct.KillCounter, %struct.KillCounter* %self4, i32 0, i32 1, !dbg !7
  %strdup = call i8* @strdup(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0)), !dbg !8
  store i8* %strdup, i8** %label, align 8, !dbg !8
  ret void, !dbg !8
}

define void @KillCounter.tick(%struct.KillCounter* %self) !dbg !9 {
entry:
  %self1 = alloca %struct.KillCounter*, align 8, !dbg !10
  store %struct.KillCounter* %self, %struct.KillCounter** %self1, align 8, !dbg !10
  %self2 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !10
  %value = getelementptr %struct.KillCounter, %struct.KillCounter* %self2, i32 0, i32 0, !dbg !10
  %DOT = load i32, i32* %value, align 4, !dbg !10
  %ADD = add i32 %DOT, 1, !dbg !10
  store i32 %ADD, i32* %value, align 4, !dbg !10
  ret void, !dbg !10
}

define void @KillCounter.print(%struct.KillCounter* %self) !dbg !11 {
entry:
  %self1 = alloca %struct.KillCounter*, align 8, !dbg !12
  store %struct.KillCounter* %self, %struct.KillCounter** %self1, align 8, !dbg !12
  %self2 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !12
  %label = getelementptr %struct.KillCounter, %struct.KillCounter* %self2, i32 0, i32 1, !dbg !12
  %DOT = load i8*, i8** %label, align 8, !dbg !12
  %self3 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !12
  %value = getelementptr %struct.KillCounter, %struct.KillCounter* %self3, i32 0, i32 0, !dbg !12
  %DOT4 = load i32, i32* %value, align 4, !dbg !12
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %DOT, i32 %DOT4), !dbg !12
  ret void, !dbg !12
}

declare i32 @printf(i8*, i32, ...)

define void @KillCounter.clean(%struct.KillCounter* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.KillCounter*, align 8, !dbg !14
  store %struct.KillCounter* %self, %struct.KillCounter** %self1, align 8, !dbg !14
  %self2 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !15
  %label = getelementptr %struct.KillCounter, %struct.KillCounter* %self2, i32 0, i32 1, !dbg !15
  %DOT = load i8*, i8** %label, align 8, !dbg !15
  call void @free(i8* %DOT), !dbg !15
  %self3 = load %struct.KillCounter*, %struct.KillCounter** %self1, align 8, !dbg !15
  ret void, !dbg !15
}

define i32 @main() !dbg !16 {
entry:
  call void @KillCounter.init(%struct.KillCounter* @counter), !dbg !17
  call void @KillCounter.tick(%struct.KillCounter* @counter), !dbg !18
  call void @KillCounter.tick(%struct.KillCounter* @counter), !dbg !19
  call void @KillCounter.tick(%struct.KillCounter* @counter), !dbg !20
  call void @KillCounter.print(%struct.KillCounter* @counter), !dbg !21
  call void @KillCounter.clean(%struct.KillCounter* @counter), !dbg !21
  ret i32 0, !dbg !21
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals")
!4 = distinct !DISubprogram(name: "KillCounter.init", linkageName: "KillCounter.init", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 9, scope: !4)
!8 = !DILocation(line: 11, scope: !4)
!9 = distinct !DISubprogram(name: "KillCounter.tick", linkageName: "KillCounter.tick", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 13, scope: !9)
!11 = distinct !DISubprogram(name: "KillCounter.print", linkageName: "KillCounter.print", scope: null, file: !3, line: 16, type: !5, scopeLine: 16, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!12 = !DILocation(line: 16, scope: !11)
!13 = distinct !DISubprogram(name: "KillCounter.clean", linkageName: "KillCounter.clean", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 19, scope: !13)
!15 = !DILocation(line: 20, scope: !13)
!16 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 24, type: !5, scopeLine: 24, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 24, scope: !16)
!18 = !DILocation(line: 25, scope: !16)
!19 = !DILocation(line: 26, scope: !16)
!20 = !DILocation(line: 27, scope: !16)
!21 = !DILocation(line: 28, scope: !16)
