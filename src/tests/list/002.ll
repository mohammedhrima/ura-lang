; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/list/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/list/002.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.__list_char = type { i8*, i32, i32 }

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [8 x i8] c"%c%c%c\0A\00", align 1
@STR1 = private unnamed_addr constant [5 x i8] c"len=\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [8 x i8] c"len=%d\0A\00", align 1

define void @__list_char.delete(%struct.__list_char* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.__list_char*, align 8, !dbg !7
  store %struct.__list_char* %self, %struct.__list_char** %self1, align 8, !dbg !7
  %self2 = load %struct.__list_char*, %struct.__list_char** %self1, align 8, !dbg !8
  %data = getelementptr %struct.__list_char, %struct.__list_char* %self2, i32 0, i32 0, !dbg !8
  %DOT = load i8*, i8** %data, align 8, !dbg !8
  call void @free(i8* %DOT), !dbg !8
  %self3 = load %struct.__list_char*, %struct.__list_char** %self1, align 8, !dbg !8
  ret void, !dbg !8
}

define void @__list_char.push(i8 %e, %struct.__list_char* %self) !dbg !9 {
entry:
  %e1 = alloca i8, align 1, !dbg !10
  %self2 = alloca %struct.__list_char*, align 8, !dbg !10
  store i8 %e, i8* %e1, align 1, !dbg !10
  store %struct.__list_char* %self, %struct.__list_char** %self2, align 8, !dbg !10
  br label %if.start, !dbg !10

if.start:                                         ; preds = %entry
  %self3 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !10
  %__len = getelementptr %struct.__list_char, %struct.__list_char* %self3, i32 0, i32 1, !dbg !10
  %self4 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !10
  %__cap = getelementptr %struct.__list_char, %struct.__list_char* %self4, i32 0, i32 2, !dbg !10
  %DOT = load i32, i32* %__len, align 4, !dbg !10
  %DOT5 = load i32, i32* %__cap, align 4, !dbg !10
  %GE = icmp sge i32 %DOT, %DOT5, !dbg !10
  br i1 %GE, label %if.then, label %if.end, !dbg !10

if.end:                                           ; preds = %if.end7, %if.start
  %self25 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !11
  %data26 = getelementptr %struct.__list_char, %struct.__list_char* %self25, i32 0, i32 0, !dbg !11
  %self27 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !11
  %__len28 = getelementptr %struct.__list_char, %struct.__list_char* %self27, i32 0, i32 1, !dbg !11
  %DOT29 = load i8*, i8** %data26, align 8, !dbg !11
  %DOT30 = load i32, i32* %__len28, align 4, !dbg !11
  %ACCESS = getelementptr i8, i8* %DOT29, i32 %DOT30, !dbg !11
  %e31 = load i8, i8* %e1, align 1, !dbg !11
  store i8 %e31, i8* %ACCESS, align 1, !dbg !11
  %self32 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !11
  %__len33 = getelementptr %struct.__list_char, %struct.__list_char* %self32, i32 0, i32 1, !dbg !11
  %DOT34 = load i32, i32* %__len33, align 4, !dbg !11
  %ADD = add i32 %DOT34, 1, !dbg !11
  store i32 %ADD, i32* %__len33, align 4, !dbg !11
  ret void, !dbg !11

if.then:                                          ; preds = %if.start
  br label %if.start6, !dbg !10

if.start6:                                        ; preds = %if.then
  %self9 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !10
  %__cap10 = getelementptr %struct.__list_char, %struct.__list_char* %self9, i32 0, i32 2, !dbg !10
  %DOT11 = load i32, i32* %__cap10, align 4, !dbg !10
  %EQ = icmp eq i32 %DOT11, 0, !dbg !10
  br i1 %EQ, label %if.then8, label %if.else, !dbg !10

if.end7:                                          ; preds = %if.else, %if.then8
  %self17 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !10
  %data = getelementptr %struct.__list_char, %struct.__list_char* %self17, i32 0, i32 0, !dbg !10
  %self18 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !12
  %data19 = getelementptr %struct.__list_char, %struct.__list_char* %self18, i32 0, i32 0, !dbg !12
  %DOT20 = load i8*, i8** %data19, align 8, !dbg !12
  %self21 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !12
  %__cap22 = getelementptr %struct.__list_char, %struct.__list_char* %self21, i32 0, i32 2, !dbg !12
  %DOT23 = load i32, i32* %__cap22, align 4, !dbg !12
  %MUL24 = mul i32 %DOT23, 1, !dbg !12
  %realloc = call i8* @realloc(i8* %DOT20, i32 %MUL24), !dbg !12
  store i8* %realloc, i8** %data, align 8, !dbg !12
  br label %if.end, !dbg !12

if.then8:                                         ; preds = %if.start6
  %self12 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !10
  %__cap13 = getelementptr %struct.__list_char, %struct.__list_char* %self12, i32 0, i32 2, !dbg !10
  store i32 8, i32* %__cap13, align 4, !dbg !10
  br label %if.end7, !dbg !10

if.else:                                          ; preds = %if.start6
  %self14 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !10
  %__cap15 = getelementptr %struct.__list_char, %struct.__list_char* %self14, i32 0, i32 2, !dbg !10
  %DOT16 = load i32, i32* %__cap15, align 4, !dbg !10
  %MUL = mul i32 %DOT16, 2, !dbg !10
  store i32 %MUL, i32* %__cap15, align 4, !dbg !10
  br label %if.end7, !dbg !10
}

define i8 @__list_char.pop(%struct.__list_char* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.__list_char*, align 8, !dbg !14
  store %struct.__list_char* %self, %struct.__list_char** %self1, align 8, !dbg !14
  %self2 = load %struct.__list_char*, %struct.__list_char** %self1, align 8, !dbg !14
  %__len = getelementptr %struct.__list_char, %struct.__list_char* %self2, i32 0, i32 1, !dbg !14
  %DOT = load i32, i32* %__len, align 4, !dbg !14
  %SUB = sub i32 %DOT, 1, !dbg !14
  store i32 %SUB, i32* %__len, align 4, !dbg !14
  %self3 = load %struct.__list_char*, %struct.__list_char** %self1, align 8, !dbg !15
  %data = getelementptr %struct.__list_char, %struct.__list_char* %self3, i32 0, i32 0, !dbg !15
  %self4 = load %struct.__list_char*, %struct.__list_char** %self1, align 8, !dbg !15
  %__len5 = getelementptr %struct.__list_char, %struct.__list_char* %self4, i32 0, i32 1, !dbg !15
  %DOT6 = load i8*, i8** %data, align 8, !dbg !15
  %DOT7 = load i32, i32* %__len5, align 4, !dbg !15
  %ACCESS = getelementptr i8, i8* %DOT6, i32 %DOT7, !dbg !15
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !15
  ret i8 %ACC, !dbg !15
}

define i32 @__list_char.len(%struct.__list_char* %self) !dbg !16 {
entry:
  %self1 = alloca %struct.__list_char*, align 8, !dbg !17
  store %struct.__list_char* %self, %struct.__list_char** %self1, align 8, !dbg !17
  %self2 = load %struct.__list_char*, %struct.__list_char** %self1, align 8, !dbg !17
  %__len = getelementptr %struct.__list_char, %struct.__list_char* %self2, i32 0, i32 1, !dbg !17
  %DOT = load i32, i32* %__len, align 4, !dbg !17
  ret i32 %DOT, !dbg !17
}

define i32 @__list_char.cap(%struct.__list_char* %self) !dbg !18 {
entry:
  %self1 = alloca %struct.__list_char*, align 8, !dbg !19
  store %struct.__list_char* %self, %struct.__list_char** %self1, align 8, !dbg !19
  %self2 = load %struct.__list_char*, %struct.__list_char** %self1, align 8, !dbg !19
  %__cap = getelementptr %struct.__list_char, %struct.__list_char* %self2, i32 0, i32 2, !dbg !19
  %DOT = load i32, i32* %__cap, align 4, !dbg !19
  ret i32 %DOT, !dbg !19
}

define void @__list_char.foreach(void (i8)* %cb, %struct.__list_char* %self) !dbg !20 {
entry:
  %cb1 = alloca void (i8)*, align 8, !dbg !21
  %self2 = alloca %struct.__list_char*, align 8, !dbg !21
  %i = alloca i32, align 4, !dbg !21
  store void (i8)* %cb, void (i8)** %cb1, align 8, !dbg !21
  store %struct.__list_char* %self, %struct.__list_char** %self2, align 8, !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  br label %while.start, !dbg !21

while.start:                                      ; preds = %while.then, %entry
  %self3 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !21
  %__len = getelementptr %struct.__list_char, %struct.__list_char* %self3, i32 0, i32 1, !dbg !21
  %i4 = load i32, i32* %i, align 4, !dbg !21
  %DOT = load i32, i32* %__len, align 4, !dbg !21
  %LT = icmp slt i32 %i4, %DOT, !dbg !21
  br i1 %LT, label %while.then, label %while.end, !dbg !21

while.then:                                       ; preds = %while.start
  %fn_ptr = load void (i8)*, void (i8)** %cb1, align 8, !dbg !22
  %self5 = load %struct.__list_char*, %struct.__list_char** %self2, align 8, !dbg !22
  %data = getelementptr %struct.__list_char, %struct.__list_char* %self5, i32 0, i32 0, !dbg !22
  %DOT6 = load i8*, i8** %data, align 8, !dbg !22
  %i7 = load i32, i32* %i, align 4, !dbg !22
  %ACCESS = getelementptr i8, i8* %DOT6, i32 %i7, !dbg !22
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !22
  call void %fn_ptr(i8 %ACC), !dbg !22
  %i8 = load i32, i32* %i, align 4, !dbg !22
  %ADD = add i32 %i8, 1, !dbg !22
  store i32 %ADD, i32* %i, align 4, !dbg !22
  br label %while.start, !dbg !22

while.end:                                        ; preds = %while.start
  ret void, !dbg !22
}

declare void @free(i8*)

declare i8* @realloc(i8*, i32)

define i32 @main() !dbg !23 {
entry:
  %c = alloca %struct.__list_char, align 8, !dbg !24
  store %struct.__list_char zeroinitializer, %struct.__list_char* %c, align 8, !dbg !24
  call void @__list_char.push(i8 72, %struct.__list_char* %c), !dbg !25
  call void @__list_char.push(i8 105, %struct.__list_char* %c), !dbg !26
  call void @__list_char.push(i8 33, %struct.__list_char* %c), !dbg !27
  %data = getelementptr %struct.__list_char, %struct.__list_char* %c, i32 0, i32 0, !dbg !28
  %DOT = load i8*, i8** %data, align 8, !dbg !28
  %ACCESS = getelementptr i8, i8* %DOT, i32 0, !dbg !28
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !28
  %data1 = getelementptr %struct.__list_char, %struct.__list_char* %c, i32 0, i32 0, !dbg !28
  %DOT2 = load i8*, i8** %data1, align 8, !dbg !28
  %ACCESS3 = getelementptr i8, i8* %DOT2, i32 1, !dbg !28
  %ACC4 = load i8, i8* %ACCESS3, align 1, !dbg !28
  %data5 = getelementptr %struct.__list_char, %struct.__list_char* %c, i32 0, i32 0, !dbg !28
  %DOT6 = load i8*, i8** %data5, align 8, !dbg !28
  %ACCESS7 = getelementptr i8, i8* %DOT6, i32 2, !dbg !28
  %ACC8 = load i8, i8* %ACCESS7, align 1, !dbg !28
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt, i32 0, i32 0), i8 %ACC, i8 %ACC4, i8 %ACC8), !dbg !28
  %__list_char.len = call i32 @__list_char.len(%struct.__list_char* %c), !dbg !29
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt.1, i32 0, i32 0), i32 %__list_char.len), !dbg !29
  call void @__list_char.delete(%struct.__list_char* %c), !dbg !29
  ret i32 0, !dbg !29
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/list")
!4 = distinct !DISubprogram(name: "__list_char.delete", linkageName: "__list_char.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = !DILocation(line: 9, scope: !4)
!9 = distinct !DISubprogram(name: "__list_char.push", linkageName: "__list_char.push", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 11, scope: !9)
!11 = !DILocation(line: 16, scope: !9)
!12 = !DILocation(line: 15, scope: !9)
!13 = distinct !DISubprogram(name: "__list_char.pop", linkageName: "__list_char.pop", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 19, scope: !13)
!15 = !DILocation(line: 21, scope: !13)
!16 = distinct !DISubprogram(name: "__list_char.len", linkageName: "__list_char.len", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 23, scope: !16)
!18 = distinct !DISubprogram(name: "__list_char.cap", linkageName: "__list_char.cap", scope: null, file: !3, line: 26, type: !5, scopeLine: 26, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 26, scope: !18)
!20 = distinct !DISubprogram(name: "__list_char.foreach", linkageName: "__list_char.foreach", scope: null, file: !3, line: 29, type: !5, scopeLine: 29, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 29, scope: !20)
!22 = !DILocation(line: 32, scope: !20)
!23 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!24 = !DILocation(line: 4, scope: !23)
!25 = !DILocation(line: 6, scope: !23)
!26 = !DILocation(line: 7, scope: !23)
!27 = !DILocation(line: 8, scope: !23)
!28 = !DILocation(line: 9, scope: !23)
!29 = !DILocation(line: 10, scope: !23)
