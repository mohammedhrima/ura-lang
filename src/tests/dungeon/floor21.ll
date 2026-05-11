; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor21.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor21.ura"
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

define void @__list_int.foreach(void (i32)* %cb, %struct.__list_int* %self) !dbg !20 {
entry:
  %cb1 = alloca void (i32)*, align 8, !dbg !21
  %self2 = alloca %struct.__list_int*, align 8, !dbg !21
  %i = alloca i32, align 4, !dbg !21
  store void (i32)* %cb, void (i32)** %cb1, align 8, !dbg !21
  store %struct.__list_int* %self, %struct.__list_int** %self2, align 8, !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  br label %while.start, !dbg !21

while.start:                                      ; preds = %while.then, %entry
  %self3 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !21
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self3, i32 0, i32 1, !dbg !21
  %i4 = load i32, i32* %i, align 4, !dbg !21
  %DOT = load i32, i32* %__len, align 4, !dbg !21
  %LT = icmp slt i32 %i4, %DOT, !dbg !21
  br i1 %LT, label %while.then, label %while.end, !dbg !21

while.then:                                       ; preds = %while.start
  %fn_ptr = load void (i32)*, void (i32)** %cb1, align 8, !dbg !22
  %self5 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !22
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self5, i32 0, i32 0, !dbg !22
  %DOT6 = load i32*, i32** %data, align 8, !dbg !22
  %i7 = load i32, i32* %i, align 4, !dbg !22
  %ACCESS = getelementptr i32, i32* %DOT6, i32 %i7, !dbg !22
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !22
  call void %fn_ptr(i32 %ACC), !dbg !22
  %i8 = load i32, i32* %i, align 4, !dbg !22
  %ADD = add i32 %i8, 1, !dbg !22
  store i32 %ADD, i32* %i, align 4, !dbg !22
  br label %while.start, !dbg !22

while.end:                                        ; preds = %while.start
  ret void, !dbg !22
}

declare void @free(i8*)

declare i8* @realloc(i8*, i32)

define void @print_inventory(i32* %items, i32 %count) !dbg !23 {
entry:
  %items1 = alloca i32*, align 8, !dbg !24
  %count2 = alloca i32, align 4, !dbg !24
  %i = alloca i32, align 4, !dbg !24
  store i32* %items, i32** %items1, align 8, !dbg !24
  store i32 %count, i32* %count2, align 4, !dbg !24
  %count3 = load i32, i32* %count2, align 4, !dbg !24
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt, i32 0, i32 0), i32 %count3), !dbg !24
  store i32 0, i32* %i, align 4, !dbg !24
  store i32 0, i32* %i, align 4, !dbg !24
  br label %while.start, !dbg !24

while.start:                                      ; preds = %if.end, %entry
  %i4 = load i32, i32* %i, align 4, !dbg !24
  %count5 = load i32, i32* %count2, align 4, !dbg !24
  %LT = icmp slt i32 %i4, %count5, !dbg !24
  br i1 %LT, label %while.then, label %while.end, !dbg !24

while.then:                                       ; preds = %while.start
  br label %if.start, !dbg !24

while.end:                                        ; preds = %while.start
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @output_fmt.3, i32 0, i32 0)), !dbg !25
  ret void, !dbg !25

if.start:                                         ; preds = %while.then
  %i6 = load i32, i32* %i, align 4, !dbg !24
  %GT = icmp sgt i32 %i6, 0, !dbg !24
  br i1 %GT, label %if.then, label %if.end, !dbg !24

if.end:                                           ; preds = %if.then, %if.start
  %items7 = load i32*, i32** %items1, align 8, !dbg !25
  %i8 = load i32, i32* %i, align 4, !dbg !25
  %ACCESS = getelementptr i32, i32* %items7, i32 %i8, !dbg !25
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !25
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @output_fmt.2, i32 0, i32 0), i32 %ACC), !dbg !25
  %i9 = load i32, i32* %i, align 4, !dbg !25
  %ADD = add i32 %i9, 1, !dbg !25
  store i32 %ADD, i32* %i, align 4, !dbg !25
  br label %while.start, !dbg !25

if.then:                                          ; preds = %if.start
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @output_fmt.1, i32 0, i32 0)), !dbg !24
  br label %if.end, !dbg !24
}

define i32 @main() !dbg !26 {
entry:
  %loot = alloca %struct.__list_int, align 8, !dbg !27
  %total = alloca i32, align 4, !dbg !27
  %i = alloca i32, align 4, !dbg !27
  %dropped = alloca i32, align 4, !dbg !27
  %sold = alloca i32, align 4, !dbg !27
  %remaining = alloca i32, align 4, !dbg !27
  store %struct.__list_int zeroinitializer, %struct.__list_int* %loot, align 8, !dbg !27
  call void @__list_int.push(i32 15, %struct.__list_int* %loot), !dbg !28
  call void @__list_int.push(i32 30, %struct.__list_int* %loot), !dbg !29
  call void @__list_int.push(i32 100, %struct.__list_int* %loot), !dbg !30
  call void @__list_int.push(i32 8, %struct.__list_int* %loot), !dbg !31
  %data = getelementptr %struct.__list_int, %struct.__list_int* %loot, i32 0, i32 0, !dbg !32
  %DOT = load i32*, i32** %data, align 8, !dbg !32
  %__list_int.len = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !32
  call void @print_inventory(i32* %DOT, i32 %__list_int.len), !dbg !32
  store i32 0, i32* %total, align 4, !dbg !27
  store i32 0, i32* %total, align 4, !dbg !27
  store i32 0, i32* %i, align 4, !dbg !27
  store i32 0, i32* %i, align 4, !dbg !27
  br label %while.start, !dbg !27

while.start:                                      ; preds = %while.then, %entry
  %__list_int.len1 = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !33
  %i2 = load i32, i32* %i, align 4, !dbg !33
  %LT = icmp slt i32 %i2, %__list_int.len1, !dbg !33
  br i1 %LT, label %while.then, label %while.end, !dbg !33

while.then:                                       ; preds = %while.start
  %data3 = getelementptr %struct.__list_int, %struct.__list_int* %loot, i32 0, i32 0, !dbg !34
  %DOT4 = load i32*, i32** %data3, align 8, !dbg !34
  %i5 = load i32, i32* %i, align 4, !dbg !34
  %ACCESS = getelementptr i32, i32* %DOT4, i32 %i5, !dbg !34
  %total6 = load i32, i32* %total, align 4, !dbg !34
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !34
  %ADD = add i32 %total6, %ACC, !dbg !34
  store i32 %ADD, i32* %total, align 4, !dbg !34
  %i7 = load i32, i32* %i, align 4, !dbg !34
  %ADD8 = add i32 %i7, 1, !dbg !34
  store i32 %ADD8, i32* %i, align 4, !dbg !34
  br label %while.start, !dbg !34

while.end:                                        ; preds = %while.start
  %total9 = load i32, i32* %total, align 4, !dbg !34
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.4, i32 0, i32 0), i32 %total9), !dbg !34
  store i32 0, i32* %dropped, align 4, !dbg !27
  %__list_int.pop = call i32 @__list_int.pop(%struct.__list_int* %loot), !dbg !35
  store i32 %__list_int.pop, i32* %dropped, align 4, !dbg !35
  %dropped10 = load i32, i32* %dropped, align 4, !dbg !35
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.5, i32 0, i32 0), i32 %dropped10), !dbg !35
  store i32 0, i32* %sold, align 4, !dbg !27
  %__list_int.pop11 = call i32 @__list_int.pop(%struct.__list_int* %loot), !dbg !36
  store i32 %__list_int.pop11, i32* %sold, align 4, !dbg !36
  %sold12 = load i32, i32* %sold, align 4, !dbg !36
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @output_fmt.6, i32 0, i32 0), i32 %sold12), !dbg !36
  %__list_int.len13 = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !37
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @output_fmt.7, i32 0, i32 0), i32 %__list_int.len13), !dbg !37
  store i32 0, i32* %remaining, align 4, !dbg !27
  store i32 0, i32* %remaining, align 4, !dbg !27
  store i32 0, i32* %i, align 4, !dbg !27
  br label %while.start14, !dbg !27

while.start14:                                    ; preds = %while.then15, %while.end
  %__list_int.len17 = call i32 @__list_int.len(%struct.__list_int* %loot), !dbg !38
  %i18 = load i32, i32* %i, align 4, !dbg !38
  %LT19 = icmp slt i32 %i18, %__list_int.len17, !dbg !38
  br i1 %LT19, label %while.then15, label %while.end16, !dbg !38

while.then15:                                     ; preds = %while.start14
  %data20 = getelementptr %struct.__list_int, %struct.__list_int* %loot, i32 0, i32 0, !dbg !39
  %DOT21 = load i32*, i32** %data20, align 8, !dbg !39
  %i22 = load i32, i32* %i, align 4, !dbg !39
  %ACCESS23 = getelementptr i32, i32* %DOT21, i32 %i22, !dbg !39
  %remaining24 = load i32, i32* %remaining, align 4, !dbg !39
  %ACC25 = load i32, i32* %ACCESS23, align 4, !dbg !39
  %ADD26 = add i32 %remaining24, %ACC25, !dbg !39
  store i32 %ADD26, i32* %remaining, align 4, !dbg !39
  %i27 = load i32, i32* %i, align 4, !dbg !39
  %ADD28 = add i32 %i27, 1, !dbg !39
  store i32 %ADD28, i32* %i, align 4, !dbg !39
  br label %while.start14, !dbg !39

while.end16:                                      ; preds = %while.start14
  %remaining29 = load i32, i32* %remaining, align 4, !dbg !39
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @output_fmt.8, i32 0, i32 0), i32 %remaining29), !dbg !39
  call void @__list_int.delete(%struct.__list_int* %loot), !dbg !39
  ret i32 0, !dbg !39
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor21.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
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
!20 = distinct !DISubprogram(name: "__list_int.foreach", linkageName: "__list_int.foreach", scope: null, file: !3, line: 29, type: !5, scopeLine: 29, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 29, scope: !20)
!22 = !DILocation(line: 32, scope: !20)
!23 = distinct !DISubprogram(name: "print_inventory", linkageName: "print_inventory", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!24 = !DILocation(line: 4, scope: !23)
!25 = !DILocation(line: 9, scope: !23)
!26 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!27 = !DILocation(line: 13, scope: !26)
!28 = !DILocation(line: 15, scope: !26)
!29 = !DILocation(line: 16, scope: !26)
!30 = !DILocation(line: 17, scope: !26)
!31 = !DILocation(line: 18, scope: !26)
!32 = !DILocation(line: 20, scope: !26)
!33 = !DILocation(line: 24, scope: !26)
!34 = !DILocation(line: 25, scope: !26)
!35 = !DILocation(line: 29, scope: !26)
!36 = !DILocation(line: 32, scope: !26)
!37 = !DILocation(line: 35, scope: !26)
!38 = !DILocation(line: 38, scope: !26)
!39 = !DILocation(line: 39, scope: !26)
