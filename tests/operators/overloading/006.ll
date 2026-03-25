; ModuleID = 'tests/operators/006.ura'
source_filename = "tests/operators/006.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Buffer = type { i8* }

@STR0 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@STR1 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@STR2 = private unnamed_addr constant [5 x i8] c"b1: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [8 x i8] c"b1: %s\0A\00", align 1
@STR4 = private unnamed_addr constant [5 x i8] c"b2: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [8 x i8] c"b2: %s\0A\00", align 1

declare void @free(i8*)

declare i8* @strcpy(i8*, i8*)

define void @Buffer.init(%struct.Buffer* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Buffer*, align 8, !dbg !7
  store %struct.Buffer* %self, %struct.Buffer** %self1, align 8, !dbg !7
  %self2 = load %struct.Buffer*, %struct.Buffer** %self1, align 8, !dbg !7
  %self3 = load %struct.Buffer*, %struct.Buffer** %self1, align 8, !dbg !7
  %value = getelementptr %struct.Buffer, %struct.Buffer* %self3, i32 0, i32 0, !dbg !7
  %heap = call i8* @calloc(i64 64, i64 1), !dbg !7
  store i8* %heap, i8** %value, align 8, !dbg !7
  ret void, !dbg !7
}

declare i8* @calloc(i64, i64)

define void @Buffer.clean(%struct.Buffer* %self) !dbg !8 {
entry:
  %self1 = alloca %struct.Buffer*, align 8, !dbg !9
  store %struct.Buffer* %self, %struct.Buffer** %self1, align 8, !dbg !9
  %self2 = load %struct.Buffer*, %struct.Buffer** %self1, align 8, !dbg !10
  %value = getelementptr %struct.Buffer, %struct.Buffer* %self2, i32 0, i32 0, !dbg !10
  %DOT = load i8*, i8** %value, align 8, !dbg !10
  call void @free(i8* %DOT), !dbg !10
  %self3 = load %struct.Buffer*, %struct.Buffer** %self1, align 8, !dbg !10
  ret void, !dbg !10
}

define void @Buffer.operator.ASSIGN.CHARS(i8* %s, %struct.Buffer* %self) !dbg !11 {
entry:
  %s1 = alloca i8*, align 8, !dbg !12
  %self2 = alloca %struct.Buffer*, align 8, !dbg !12
  store i8* %s, i8** %s1, align 8, !dbg !12
  store %struct.Buffer* %self, %struct.Buffer** %self2, align 8, !dbg !12
  %self3 = load %struct.Buffer*, %struct.Buffer** %self2, align 8, !dbg !13
  %value = getelementptr %struct.Buffer, %struct.Buffer* %self3, i32 0, i32 0, !dbg !13
  %DOT = load i8*, i8** %value, align 8, !dbg !13
  %s4 = load i8*, i8** %s1, align 8, !dbg !13
  %strcpy = call i8* @strcpy(i8* %DOT, i8* %s4), !dbg !13
  ret void, !dbg !13
}

define void @Buffer.operator.ASSIGN.Buffer(%struct.Buffer* %b, %struct.Buffer* %self) !dbg !14 {
entry:
  %b1 = alloca %struct.Buffer*, align 8, !dbg !15
  %self2 = alloca %struct.Buffer*, align 8, !dbg !15
  store %struct.Buffer* %b, %struct.Buffer** %b1, align 8, !dbg !15
  store %struct.Buffer* %self, %struct.Buffer** %self2, align 8, !dbg !15
  %self3 = load %struct.Buffer*, %struct.Buffer** %self2, align 8, !dbg !16
  %value = getelementptr %struct.Buffer, %struct.Buffer* %self3, i32 0, i32 0, !dbg !16
  %DOT = load i8*, i8** %value, align 8, !dbg !16
  %b4 = load %struct.Buffer*, %struct.Buffer** %b1, align 8, !dbg !16
  %value5 = getelementptr %struct.Buffer, %struct.Buffer* %b4, i32 0, i32 0, !dbg !16
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !16
  %strcpy = call i8* @strcpy(i8* %DOT, i8* %DOT6), !dbg !16
  ret void, !dbg !16
}

define i32 @main() !dbg !17 {
entry:
  %b1 = alloca %struct.Buffer, align 8, !dbg !18
  %b2 = alloca %struct.Buffer, align 8, !dbg !18
  store %struct.Buffer zeroinitializer, %struct.Buffer* %b1, align 8, !dbg !18
  call void @Buffer.init(%struct.Buffer* %b1), !dbg !18
  call void @Buffer.operator.ASSIGN.CHARS(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), %struct.Buffer* %b1), !dbg !18
  store %struct.Buffer zeroinitializer, %struct.Buffer* %b2, align 8, !dbg !18
  call void @Buffer.init(%struct.Buffer* %b2), !dbg !18
  call void @Buffer.operator.ASSIGN.Buffer(%struct.Buffer* %b1, %struct.Buffer* %b2), !dbg !18
  %value = getelementptr %struct.Buffer, %struct.Buffer* %b2, i32 0, i32 0, !dbg !19
  %DOT = load i8*, i8** %value, align 8, !dbg !19
  %strcpy = call i8* @strcpy(i8* %DOT, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR1, i32 0, i32 0)), !dbg !19
  %value1 = getelementptr %struct.Buffer, %struct.Buffer* %b1, i32 0, i32 0, !dbg !19
  %DOT2 = load i8*, i8** %value1, align 8, !dbg !19
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %DOT2), !dbg !19
  %value3 = getelementptr %struct.Buffer, %struct.Buffer* %b2, i32 0, i32 0, !dbg !19
  %DOT4 = load i8*, i8** %value3, align 8, !dbg !19
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %DOT4), !dbg !19
  call void @Buffer.clean(%struct.Buffer* %b1), !dbg !19
  call void @Buffer.clean(%struct.Buffer* %b2), !dbg !19
  ret i32 0, !dbg !19
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "006.ura", directory: "tests/operators")
!4 = distinct !DISubprogram(name: "Buffer.init", linkageName: "Buffer.init", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
!8 = distinct !DISubprogram(name: "Buffer.clean", linkageName: "Buffer.clean", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 11, scope: !8)
!10 = !DILocation(line: 12, scope: !8)
!11 = distinct !DISubprogram(name: "Buffer.operator.ASSIGN.CHARS", linkageName: "Buffer.operator.ASSIGN.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!12 = !DILocation(line: 0, scope: !11)
!13 = !DILocation(line: 15, scope: !11)
!14 = distinct !DISubprogram(name: "Buffer.operator.ASSIGN.Buffer", linkageName: "Buffer.operator.ASSIGN.Buffer", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 0, scope: !14)
!16 = !DILocation(line: 18, scope: !14)
!17 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 20, type: !5, scopeLine: 20, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!18 = !DILocation(line: 20, scope: !17)
!19 = !DILocation(line: 27, scope: !17)
