; ModuleID = 'tests/dungeon/floor21.ura'
source_filename = "tests/dungeon/floor21.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.__list_int = type { i32*, i32, i32 }

@STR0 = private unnamed_addr constant [12 x i8] c"Inventory (\00", align 1
@STR1 = private unnamed_addr constant [10 x i8] c" items): \00", align 1
@output_fmt = private unnamed_addr constant [23 x i8] c"Inventory (%d items): \00", align 1
@STR2 = private unnamed_addr constant [3 x i8] c", \00", align 1
@output_fmt.1 = private unnamed_addr constant [3 x i8] c", \00", align 1
@output_fmt.2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@STR3 = private unnamed_addr constant [7 x i8] c" gold\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [7 x i8] c" gold\0A\00", align 1
@STR4 = private unnamed_addr constant [13 x i8] c"Total gold: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [16 x i8] c"Total gold: %d\0A\00", align 1
@STR6 = private unnamed_addr constant [9 x i8] c"Dropped \00", align 1
@STR7 = private unnamed_addr constant [7 x i8] c" gold\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [17 x i8] c"Dropped %d gold\0A\00", align 1
@STR8 = private unnamed_addr constant [17 x i8] c"Sold item worth \00", align 1
@STR9 = private unnamed_addr constant [7 x i8] c" gold\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [25 x i8] c"Sold item worth %d gold\0A\00", align 1
@STR10 = private unnamed_addr constant [12 x i8] c"Remaining: \00", align 1
@STR11 = private unnamed_addr constant [9 x i8] c" items, \00", align 1
@output_fmt.7 = private unnamed_addr constant [22 x i8] c"Remaining: %d items, \00", align 1
@STR12 = private unnamed_addr constant [7 x i8] c" gold\0A\00", align 1
@output_fmt.8 = private unnamed_addr constant [9 x i8] c"%d gold\0A\00", align 1

declare i8* @realloc(i8*, i32)

declare void @free(i8*)

define void @__list_int.delete(%struct.__list_int* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !7
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !7
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !8
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 0, !dbg !8
  %DOT = load i32*, i32** %data, align 8, !dbg !8
  %as = bitcast i32* %DOT to i8*, !dbg !8
  call void @free(i8* %as), !dbg !8
  %self3 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !8
  ret void, !dbg !8
}

define void @__list_int.push(i32 %e, %struct.__list_int* %self) !dbg !9 {
entry:
  %e1 = alloca i32, align 4, !dbg !10
  %self2 = alloca %struct.__list_int*, align 8, !dbg !10
  store i32 %e, i32* %e1, align 4, !dbg !10
  store %struct.__list_int* %self, %struct.__list_int** %self2, align 8, !dbg !10
  br label %if.start, !dbg !10

if.start:                                         ; preds = %entry
  %self3 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self3, i32 0, i32 1, !dbg !10
  %self4 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap = getelementptr %struct.__list_int, %struct.__list_int* %self4, i32 0, i32 2, !dbg !10
  %DOT = load i32, i32* %__len, align 4, !dbg !10
  %DOT5 = load i32, i32* %__cap, align 4, !dbg !10
  %GE = icmp sge i32 %DOT, %DOT5, !dbg !10
  br i1 %GE, label %if.then, label %if.end, !dbg !10

if.end:                                           ; preds = %if.end7, %if.start
  %self25 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !11
  %data26 = getelementptr %struct.__list_int, %struct.__list_int* %self25, i32 0, i32 0, !dbg !11
  %self27 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !11
  %__len28 = getelementptr %struct.__list_int, %struct.__list_int* %self27, i32 0, i32 1, !dbg !11
  %DOT29 = load i32*, i32** %data26, align 8, !dbg !11
  %DOT30 = load i32, i32* %__len28, align 4, !dbg !11
  %ACCESS = getelementptr i32, i32* %DOT29, i32 %DOT30, !dbg !11
  %e31 = load i32, i32* %e1, align 4, !dbg !11
  store i32 %e31, i32* %ACCESS, align 4, !dbg !11
  %self32 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !11
  %__len33 = getelementptr %struct.__list_int, %struct.__list_int* %self32, i32 0, i32 1, !dbg !11
  %DOT34 = load i32, i32* %__len33, align 4, !dbg !11
  %ADD = add i32 %DOT34, 1, !dbg !11
  store i32 %ADD, i32* %__len33, align 4, !dbg !11
  ret void, !dbg !11

if.then:                                          ; preds = %if.start
  br label %if.start6, !dbg !10

if.start6:                                        ; preds = %if.then
  %self9 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap10 = getelementptr %struct.__list_int, %struct.__list_int* %self9, i32 0, i32 2, !dbg !10
  %DOT11 = load i32, i32* %__cap10, align 4, !dbg !10
  %EQ = icmp eq i32 %DOT11, 0, !dbg !10
  br i1 %EQ, label %if.then8, label %if.else, !dbg !10

if.end7:                                          ; preds = %if.else, %if.then8
  %self17 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self17, i32 0, i32 0, !dbg !10
  %self18 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !12
  %data19 = getelementptr %struct.__list_int, %struct.__list_int* %self18, i32 0, i32 0, !dbg !12
  %DOT20 = load i32*, i32** %data19, align 8, !dbg !12
  %as = bitcast i32* %DOT20 to i8*, !dbg !12
  %self21 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !12
  %__cap22 = getelementptr %struct.__list_int, %struct.__list_int* %self21, i32 0, i32 2, !dbg !12
  %DOT23 = load i32, i32* %__cap22, align 4, !dbg !12
  %MUL24 = mul i32 %DOT23, 4, !dbg !12
  %realloc = call i8* @realloc(i8* %as, i32 %MUL24), !dbg !12
  %ptr_cast = bitcast i8* %realloc to i32*, !dbg !12
  store i32* %ptr_cast, i32** %data, align 8, !dbg !12
  br label %if.end, !dbg !12

if.then8:                                         ; preds = %if.start6
  %self12 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap13 = getelementptr %struct.__list_int, %struct.__list_int* %self12, i32 0, i32 2, !dbg !10
  store i32 8, i32* %__cap13, align 4, !dbg !10
  br label %if.end7, !dbg !10

if.else:                                          ; preds = %if.start6
  %self14 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap15 = getelementptr %struct.__list_int, %struct.__list_int* %self14, i32 0, i32 2, !dbg !10
  %DOT16 = load i32, i32* %__cap15, align 4, !dbg !10
  %MUL = mul i32 %DOT16, 2, !dbg !10
  store i32 %MUL, i32* %__cap15, align 4, !dbg !10
  br label %if.end7, !dbg !10
}

define i32 @__list_int.pop(%struct.__list_int* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !14
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !14
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !14
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 1, !dbg !14
  %DOT = load i32, i32* %__len, align 4, !dbg !14
  %SUB = sub i32 %DOT, 1, !dbg !14
  store i32 %SUB, i32* %__len, align 4, !dbg !14
  %self3 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !15
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self3, i32 0, i32 0, !dbg !15
  %self4 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !15
  %__len5 = getelementptr %struct.__list_int, %struct.__list_int* %self4, i32 0, i32 1, !dbg !15
  %DOT6 = load i32*, i32** %data, align 8, !dbg !15
  %DOT7 = load i32, i32* %__len5, align 4, !dbg !15
  %ACCESS = getelementptr i32, i32* %DOT6, i32 %DOT7, !dbg !15
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !15
  ret i32 %ACC, !dbg !15
}

define i32 @__list_int.len(%struct.__list_int* %self) !dbg !16 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !17
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !17
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !17
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 1, !dbg !17
  %DOT = load i32, i32* %__len, align 4, !dbg !17
  ret i32 %DOT, !dbg !17
}

define i32 @__list_int.cap(%struct.__list_int* %self) !dbg !18 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !19
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !19
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !19
  %__cap = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 2, !dbg !19
  %DOT = load i32, i32* %__cap, align 4, !dbg !19
  ret i32 %DOT, !dbg !19
}

define void @print_inventory(i32* %items, i32 %count) !dbg !20 {
entry:
  %items1 = alloca i32*, align 8, !dbg !21
  %count2 = alloca i32, align 4, !dbg !21
  %i = alloca i32, align 4, !dbg !21
  store i32* %items, i32** %items1, align 8, !dbg !21
  store i32 %count, i32* %count2, align 4, !dbg !21
  %count3 = load i32, i32* %count2, align 4, !dbg !21
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %count3), !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  br label %while.start, !dbg !21

while.start:                                      ; preds = %if.end, %entry
  %i4 = load i32, i32* %i, align 4, !dbg !21
  %count5 = load i32, i32* %count2, align 4, !dbg !21
  %LT = icmp slt i32 %i4, %count5, !dbg !21
  br i1 %LT, label %while.then, label %while.end, !dbg !21

while.then:                                       ; preds = %while.start
  br label %if.start, !dbg !21

while.end:                                        ; preds = %while.start
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @output_fmt.3, i32 0, i32 0), i32 0), !dbg !22
  ret void, !dbg !22

if.start:                                         ; preds = %while.then
  %i6 = load i32, i32* %i, align 4, !dbg !21
  %GT = icmp sgt i32 %i6, 0, !dbg !21
  br i1 %GT, label %if.then, label %if.end, !dbg !21

if.end:                                           ; preds = %if.then, %if.start
  %items7 = load i32*, i32** %items1, align 8, !dbg !22
  %i8 = load i32, i32* %i, align 4, !dbg !22
  %ACCESS = getelementptr i32, i32* %items7, i32 %i8, !dbg !22
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !22
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %ACC), !dbg !22
  %i9 = load i32, i32* %i, align 4, !dbg !22
  %ADD = add i32 %i9, 1, !dbg !22
  store i32 %ADD, i32* %i, align 4, !dbg !22
  br label %while.start, !dbg !22

if.then:                                          ; preds = %if.start
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !21
  br label %if.end, !dbg !21
}

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !23 {
entry:
  %loot = alloca %struct.__list_int, align 8, !dbg !24
  %total = alloca i32, align 4, !dbg !24
  %i = alloca i32, align 4, !dbg !24
  %dropped = alloca i32, align 4, !dbg !24
  %sold = alloca i32, align 4, !dbg !24
  %remaining = alloca i32, align 4, !dbg !24
  store %struct.__list_int zeroinitializer, %struct.__list_int* %loot, align 8, !dbg !24
  call void @__list_int.push(i32 15, %struct.__list_int* %loot), !dbg !25
  call void @__list_int.push(i32 30, %struct.__list_int* %loot), !dbg !26
  call void @__list_int.push(i32 100, %struct.__list_int* %loot), !dbg !27
  call void @__list_int.push(i32 8, %struct.__list_int* %loot), !dbg !28
  %data = getelementptr %struct.__list_int, %struct.__list_int* %loot, i32 0, i32 0, !dbg !29
  %DOT = load i32*, i32** %data, align 8, !dbg !29
  %__list_int.len = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !29
  call void @print_inventory(i32* %DOT, i32 %__list_int.len), !dbg !29
  store i32 0, i32* %total, align 4, !dbg !24
  store i32 0, i32* %total, align 4, !dbg !24
  store i32 0, i32* %i, align 4, !dbg !24
  store i32 0, i32* %i, align 4, !dbg !24
  br label %while.start, !dbg !24

while.start:                                      ; preds = %while.then, %entry
  %__list_int.len1 = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !30
  %i2 = load i32, i32* %i, align 4, !dbg !30
  %LT = icmp slt i32 %i2, %__list_int.len1, !dbg !30
  br i1 %LT, label %while.then, label %while.end, !dbg !30

while.then:                                       ; preds = %while.start
  %data3 = getelementptr %struct.__list_int, %struct.__list_int* %loot, i32 0, i32 0, !dbg !31
  %DOT4 = load i32*, i32** %data3, align 8, !dbg !31
  %i5 = load i32, i32* %i, align 4, !dbg !31
  %ACCESS = getelementptr i32, i32* %DOT4, i32 %i5, !dbg !31
  %total6 = load i32, i32* %total, align 4, !dbg !31
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !31
  %ADD = add i32 %total6, %ACC, !dbg !31
  store i32 %ADD, i32* %total, align 4, !dbg !31
  %i7 = load i32, i32* %i, align 4, !dbg !31
  %ADD8 = add i32 %i7, 1, !dbg !31
  store i32 %ADD8, i32* %i, align 4, !dbg !31
  br label %while.start, !dbg !31

while.end:                                        ; preds = %while.start
  %total9 = load i32, i32* %total, align 4, !dbg !31
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i32 %total9), !dbg !31
  store i32 0, i32* %dropped, align 4, !dbg !24
  %__list_int.pop = call i32 @__list_int.pop(%struct.__list_int* %loot), !dbg !32
  store i32 %__list_int.pop, i32* %dropped, align 4, !dbg !32
  %dropped10 = load i32, i32* %dropped, align 4, !dbg !32
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.5, i32 0, i32 0), i32 1, i32 %dropped10), !dbg !32
  store i32 0, i32* %sold, align 4, !dbg !24
  %__list_int.pop11 = call i32 @__list_int.pop(%struct.__list_int* %loot), !dbg !33
  store i32 %__list_int.pop11, i32* %sold, align 4, !dbg !33
  %sold12 = load i32, i32* %sold, align 4, !dbg !33
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i32 %sold12), !dbg !33
  %__list_int.len13 = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !34
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @output_fmt.7, i32 0, i32 0), i32 1, i32 %__list_int.len13), !dbg !34
  store i32 0, i32* %remaining, align 4, !dbg !24
  store i32 0, i32* %remaining, align 4, !dbg !24
  store i32 0, i32* %i, align 4, !dbg !24
  br label %while.start14, !dbg !24

while.start14:                                    ; preds = %while.then15, %while.end
  %__list_int.len17 = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !35
  %i18 = load i32, i32* %i, align 4, !dbg !35
  %LT19 = icmp slt i32 %i18, %__list_int.len17, !dbg !35
  br i1 %LT19, label %while.then15, label %while.end16, !dbg !35

while.then15:                                     ; preds = %while.start14
  %data20 = getelementptr %struct.__list_int, %struct.__list_int* %loot, i32 0, i32 0, !dbg !36
  %DOT21 = load i32*, i32** %data20, align 8, !dbg !36
  %i22 = load i32, i32* %i, align 4, !dbg !36
  %ACCESS23 = getelementptr i32, i32* %DOT21, i32 %i22, !dbg !36
  %remaining24 = load i32, i32* %remaining, align 4, !dbg !36
  %ACC25 = load i32, i32* %ACCESS23, align 4, !dbg !36
  %ADD26 = add i32 %remaining24, %ACC25, !dbg !36
  store i32 %ADD26, i32* %remaining, align 4, !dbg !36
  %i27 = load i32, i32* %i, align 4, !dbg !36
  %ADD28 = add i32 %i27, 1, !dbg !36
  store i32 %ADD28, i32* %i, align 4, !dbg !36
  br label %while.start14, !dbg !36

while.end16:                                      ; preds = %while.start14
  %remaining29 = load i32, i32* %remaining, align 4, !dbg !36
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @output_fmt.8, i32 0, i32 0), i32 1, i32 %remaining29), !dbg !36
  call void @__list_int.delete(%struct.__list_int* %loot), !dbg !36
  ret i32 0, !dbg !36
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor21.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "__list_int.delete", linkageName: "__list_int.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = !DILocation(line: 9, scope: !4)
!9 = distinct !DISubprogram(name: "__list_int.push", linkageName: "__list_int.push", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 11, scope: !9)
!11 = !DILocation(line: 16, scope: !9)
!12 = !DILocation(line: 15, scope: !9)
!13 = distinct !DISubprogram(name: "__list_int.pop", linkageName: "__list_int.pop", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 19, scope: !13)
!15 = !DILocation(line: 21, scope: !13)
!16 = distinct !DISubprogram(name: "__list_int.len", linkageName: "__list_int.len", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 23, scope: !16)
!18 = distinct !DISubprogram(name: "__list_int.cap", linkageName: "__list_int.cap", scope: null, file: !3, line: 26, type: !5, scopeLine: 26, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 26, scope: !18)
!20 = distinct !DISubprogram(name: "print_inventory", linkageName: "print_inventory", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 4, scope: !20)
!22 = !DILocation(line: 9, scope: !20)
!23 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!24 = !DILocation(line: 13, scope: !23)
!25 = !DILocation(line: 15, scope: !23)
!26 = !DILocation(line: 16, scope: !23)
!27 = !DILocation(line: 17, scope: !23)
!28 = !DILocation(line: 18, scope: !23)
!29 = !DILocation(line: 20, scope: !23)
!30 = !DILocation(line: 24, scope: !23)
!31 = !DILocation(line: 25, scope: !23)
!32 = !DILocation(line: 29, scope: !23)
!33 = !DILocation(line: 32, scope: !23)
!34 = !DILocation(line: 35, scope: !23)
!35 = !DILocation(line: 38, scope: !23)
!36 = !DILocation(line: 39, scope: !23)
