; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods/002.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Inventory = type { i8*, i32 }

@STR0 = private unnamed_addr constant [8 x i8] c"Items: \00", align 1
@STR1 = private unnamed_addr constant [10 x i8] c" | Last: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [22 x i8] c"Items: %d | Last: %s\0A\00", align 1
@STR3 = private unnamed_addr constant [14 x i8] c"Health Potion\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"Iron Sword\00", align 1
@STR5 = private unnamed_addr constant [9 x i8] c"Gold Key\00", align 1

declare void @free(i8*)

declare i8* @strcpy(i8*, i8*)

define void @Inventory.init(%struct.Inventory* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Inventory*, align 8, !dbg !7
  store %struct.Inventory* %self, %struct.Inventory** %self1, align 8, !dbg !7
  %self2 = load %struct.Inventory*, %struct.Inventory** %self1, align 8, !dbg !7
  %self3 = load %struct.Inventory*, %struct.Inventory** %self1, align 8, !dbg !7
  %last = getelementptr %struct.Inventory, %struct.Inventory* %self3, i32 0, i32 0, !dbg !7
  %heap = call i8* @calloc(i64 32, i64 1), !dbg !7
  store i8* %heap, i8** %last, align 8, !dbg !7
  %self4 = load %struct.Inventory*, %struct.Inventory** %self1, align 8, !dbg !7
  %count = getelementptr %struct.Inventory, %struct.Inventory* %self4, i32 0, i32 1, !dbg !7
  store i32 0, i32* %count, align 4, !dbg !7
  ret void, !dbg !7
}

declare i8* @calloc(i64, i64)

define void @Inventory.add(i8* %item, %struct.Inventory* %self) !dbg !8 {
entry:
  %item1 = alloca i8*, align 8, !dbg !9
  %self2 = alloca %struct.Inventory*, align 8, !dbg !9
  store i8* %item, i8** %item1, align 8, !dbg !9
  store %struct.Inventory* %self, %struct.Inventory** %self2, align 8, !dbg !9
  %self3 = load %struct.Inventory*, %struct.Inventory** %self2, align 8, !dbg !10
  %last = getelementptr %struct.Inventory, %struct.Inventory* %self3, i32 0, i32 0, !dbg !10
  %DOT = load i8*, i8** %last, align 8, !dbg !10
  %item4 = load i8*, i8** %item1, align 8, !dbg !10
  %strcpy = call i8* @strcpy(i8* %DOT, i8* %item4), !dbg !10
  %self5 = load %struct.Inventory*, %struct.Inventory** %self2, align 8, !dbg !10
  %count = getelementptr %struct.Inventory, %struct.Inventory* %self5, i32 0, i32 1, !dbg !10
  %self6 = load %struct.Inventory*, %struct.Inventory** %self2, align 8, !dbg !10
  %count7 = getelementptr %struct.Inventory, %struct.Inventory* %self6, i32 0, i32 1, !dbg !10
  %DOT8 = load i32, i32* %count7, align 4, !dbg !10
  %ADD = add i32 %DOT8, 1, !dbg !10
  store i32 %ADD, i32* %count, align 4, !dbg !10
  ret void, !dbg !10
}

define void @Inventory.print(%struct.Inventory* %self) !dbg !11 {
entry:
  %self1 = alloca %struct.Inventory*, align 8, !dbg !12
  store %struct.Inventory* %self, %struct.Inventory** %self1, align 8, !dbg !12
  %self2 = load %struct.Inventory*, %struct.Inventory** %self1, align 8, !dbg !12
  %count = getelementptr %struct.Inventory, %struct.Inventory* %self2, i32 0, i32 1, !dbg !12
  %DOT = load i32, i32* %count, align 4, !dbg !12
  %self3 = load %struct.Inventory*, %struct.Inventory** %self1, align 8, !dbg !12
  %last = getelementptr %struct.Inventory, %struct.Inventory* %self3, i32 0, i32 0, !dbg !12
  %DOT4 = load i8*, i8** %last, align 8, !dbg !12
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %DOT, i8* %DOT4), !dbg !12
  ret void, !dbg !12
}

declare i32 @printf(i8*, i32, ...)

define void @Inventory.clean(%struct.Inventory* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.Inventory*, align 8, !dbg !14
  store %struct.Inventory* %self, %struct.Inventory** %self1, align 8, !dbg !14
  %self2 = load %struct.Inventory*, %struct.Inventory** %self1, align 8, !dbg !15
  %last = getelementptr %struct.Inventory, %struct.Inventory* %self2, i32 0, i32 0, !dbg !15
  %DOT = load i8*, i8** %last, align 8, !dbg !15
  call void @free(i8* %DOT), !dbg !15
  %self3 = load %struct.Inventory*, %struct.Inventory** %self1, align 8, !dbg !15
  ret void, !dbg !15
}

define i32 @main() !dbg !16 {
entry:
  %inv = alloca %struct.Inventory, align 8, !dbg !17
  store %struct.Inventory zeroinitializer, %struct.Inventory* %inv, align 8, !dbg !17
  call void @Inventory.init(%struct.Inventory* %inv), !dbg !17
  call void @Inventory.add(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR3, i32 0, i32 0), %struct.Inventory* %inv), !dbg !18
  call void @Inventory.add(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR4, i32 0, i32 0), %struct.Inventory* %inv), !dbg !19
  call void @Inventory.add(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR5, i32 0, i32 0), %struct.Inventory* %inv), !dbg !20
  call void @Inventory.print(%struct.Inventory* %inv), !dbg !21
  call void @Inventory.clean(%struct.Inventory* %inv), !dbg !21
  ret i32 0, !dbg !21
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods")
!4 = distinct !DISubprogram(name: "Inventory.init", linkageName: "Inventory.init", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 9, scope: !4)
!8 = distinct !DISubprogram(name: "Inventory.add", linkageName: "Inventory.add", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 13, scope: !8)
!10 = !DILocation(line: 14, scope: !8)
!11 = distinct !DISubprogram(name: "Inventory.print", linkageName: "Inventory.print", scope: null, file: !3, line: 17, type: !5, scopeLine: 17, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!12 = !DILocation(line: 17, scope: !11)
!13 = distinct !DISubprogram(name: "Inventory.clean", linkageName: "Inventory.clean", scope: null, file: !3, line: 20, type: !5, scopeLine: 20, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 20, scope: !13)
!15 = !DILocation(line: 21, scope: !13)
!16 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 23, scope: !16)
!18 = !DILocation(line: 25, scope: !16)
!19 = !DILocation(line: 26, scope: !16)
!20 = !DILocation(line: 27, scope: !16)
!21 = !DILocation(line: 28, scope: !16)
