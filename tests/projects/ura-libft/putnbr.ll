; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"0123456789\00", align 1

declare i8* @calloc(i32, i32)

declare i32 @write(i32, i8*, i32)

declare i32 @free(i8*)

define i32 @putchar(i8 %c) !dbg !4 {
entry:
  %c1 = alloca i8, align 1, !dbg !7
  %str = alloca i8*, align 8, !dbg !7
  store i8 %c, i8* %c1, align 1, !dbg !7
  store i8* null, i8** %str, align 8, !dbg !7
  %calloc = call i8* @calloc(i32 2, i32 1), !dbg !8
  store i8* %calloc, i8** %str, align 8, !dbg !8
  %str2 = load i8*, i8** %str, align 8, !dbg !9
  %ACCESS = getelementptr i8, i8* %str2, i32 0, !dbg !9
  %c3 = load i8, i8* %c1, align 1, !dbg !9
  store i8 %c3, i8* %ACCESS, align 1, !dbg !9
  %str4 = load i8*, i8** %str, align 8, !dbg !10
  %write = call i32 @write(i32 1, i8* %str4, i32 1), !dbg !10
  %str5 = load i8*, i8** %str, align 8, !dbg !11
  %free = call i32 @free(i8* %str5), !dbg !11
  ret i32 0, !dbg !11
}

define void @putnbr(i32 %n) !dbg !12 {
entry:
  %n1 = alloca i32, align 4, !dbg !13
  %digits = alloca i8*, align 8, !dbg !13
  %temp = alloca i32, align 4, !dbg !13
  %digit_count = alloca i32, align 4, !dbg !13
  %buffer = alloca i8*, align 8, !dbg !13
  %i = alloca i32, align 4, !dbg !13
  %digit = alloca i32, align 4, !dbg !13
  store i32 %n, i32* %n1, align 4, !dbg !13
  br label %if.start, !dbg !13

if.start:                                         ; preds = %entry
  %n2 = load i32, i32* %n1, align 4, !dbg !13
  %LT = icmp slt i32 %n2, 0, !dbg !13
  br i1 %LT, label %if.then, label %if.end, !dbg !13

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start4, !dbg !14

if.then:                                          ; preds = %if.start
  %putchar = call i32 @putchar(i8 45), !dbg !14
  %n3 = load i32, i32* %n1, align 4, !dbg !14
  %MUL = mul i32 %n3, -1, !dbg !14
  store i32 %MUL, i32* %n1, align 4, !dbg !14
  br label %if.end, !dbg !14

if.start4:                                        ; preds = %if.end
  %n7 = load i32, i32* %n1, align 4, !dbg !14
  %EQ = icmp eq i32 %n7, 0, !dbg !14
  br i1 %EQ, label %if.then6, label %if.end5, !dbg !14

if.end5:                                          ; preds = %if.start4
  store i8* null, i8** %digits, align 8, !dbg !13
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR0, i32 0, i32 0), i8** %digits, align 8, !dbg !13
  store i32 0, i32* %temp, align 4, !dbg !13
  %n9 = load i32, i32* %n1, align 4, !dbg !13
  store i32 %n9, i32* %temp, align 4, !dbg !13
  store i32 0, i32* %digit_count, align 4, !dbg !13
  store i32 0, i32* %digit_count, align 4, !dbg !13
  br label %while.start, !dbg !13

if.then6:                                         ; preds = %if.start4
  %putchar8 = call i32 @putchar(i8 48), !dbg !15
  ret void, !dbg !15

while.start:                                      ; preds = %while.then, %if.end5
  %temp10 = load i32, i32* %temp, align 4, !dbg !13
  %GT = icmp sgt i32 %temp10, 0, !dbg !13
  br i1 %GT, label %while.then, label %while.end, !dbg !13

while.then:                                       ; preds = %while.start
  %current = load i32, i32* %digit_count, align 4, !dbg !13
  %ADD = add i32 %current, 1, !dbg !13
  store i32 %ADD, i32* %digit_count, align 4, !dbg !13
  %temp11 = load i32, i32* %temp, align 4, !dbg !13
  %DIV = sdiv i32 %temp11, 10, !dbg !13
  store i32 %DIV, i32* %temp, align 4, !dbg !13
  br label %while.start, !dbg !13

while.end:                                        ; preds = %while.start
  store i8* null, i8** %buffer, align 8, !dbg !13
  %digit_count12 = load i32, i32* %digit_count, align 4, !dbg !16
  %ADD13 = add i32 %digit_count12, 1, !dbg !16
  %calloc = call i8* @calloc(i32 %ADD13, i32 1), !dbg !16
  store i8* %calloc, i8** %buffer, align 8, !dbg !16
  store i32 0, i32* %i, align 4, !dbg !13
  %digit_count14 = load i32, i32* %digit_count, align 4, !dbg !13
  %SUB = sub i32 %digit_count14, 1, !dbg !13
  store i32 %SUB, i32* %i, align 4, !dbg !13
  br label %while.start15, !dbg !13

while.start15:                                    ; preds = %while.then16, %while.end
  %i18 = load i32, i32* %i, align 4, !dbg !13
  %GE = icmp sge i32 %i18, 0, !dbg !13
  br i1 %GE, label %while.then16, label %while.end17, !dbg !13

while.then16:                                     ; preds = %while.start15
  store i32 0, i32* %digit, align 4, !dbg !13
  %n19 = load i32, i32* %n1, align 4, !dbg !13
  %MOD = srem i32 %n19, 10, !dbg !13
  store i32 %MOD, i32* %digit, align 4, !dbg !13
  %buffer20 = load i8*, i8** %buffer, align 8, !dbg !17
  %i21 = load i32, i32* %i, align 4, !dbg !17
  %ACCESS = getelementptr i8, i8* %buffer20, i32 %i21, !dbg !17
  %digits22 = load i8*, i8** %digits, align 8, !dbg !17
  %digit23 = load i32, i32* %digit, align 4, !dbg !17
  %ACCESS24 = getelementptr i8, i8* %digits22, i32 %digit23, !dbg !17
  %ACC = load i8, i8* %ACCESS24, align 1, !dbg !17
  store i8 %ACC, i8* %ACCESS, align 1, !dbg !17
  %n25 = load i32, i32* %n1, align 4, !dbg !17
  %DIV26 = sdiv i32 %n25, 10, !dbg !17
  store i32 %DIV26, i32* %n1, align 4, !dbg !17
  %current27 = load i32, i32* %i, align 4, !dbg !17
  %SUB28 = sub i32 %current27, 1, !dbg !17
  store i32 %SUB28, i32* %i, align 4, !dbg !17
  br label %while.start15, !dbg !17

while.end17:                                      ; preds = %while.start15
  store i32 0, i32* %i, align 4, !dbg !17
  br label %while.start29, !dbg !17

while.start29:                                    ; preds = %while.then30, %while.end17
  %i32 = load i32, i32* %i, align 4, !dbg !17
  %digit_count33 = load i32, i32* %digit_count, align 4, !dbg !17
  %LT34 = icmp slt i32 %i32, %digit_count33, !dbg !17
  br i1 %LT34, label %while.then30, label %while.end31, !dbg !17

while.then30:                                     ; preds = %while.start29
  %buffer35 = load i8*, i8** %buffer, align 8, !dbg !18
  %i36 = load i32, i32* %i, align 4, !dbg !18
  %ACCESS37 = getelementptr i8, i8* %buffer35, i32 %i36, !dbg !18
  %ACC38 = load i8, i8* %ACCESS37, align 1, !dbg !18
  %putchar39 = call i32 @putchar(i8 %ACC38), !dbg !18
  %current40 = load i32, i32* %i, align 4, !dbg !18
  %ADD41 = add i32 %current40, 1, !dbg !18
  store i32 %ADD41, i32* %i, align 4, !dbg !18
  br label %while.start29, !dbg !18

while.end31:                                      ; preds = %while.start29
  %buffer42 = load i8*, i8** %buffer, align 8, !dbg !19
  %free = call i32 @free(i8* %buffer42), !dbg !19
  ret void, !dbg !19
}

define i32 @main() !dbg !20 {
entry:
  call void @putnbr(i32 123), !dbg !21
  %putchar = call i32 @putchar(i8 10), !dbg !22
  call void @putnbr(i32 -456), !dbg !23
  %putchar1 = call i32 @putchar(i8 10), !dbg !24
  call void @putnbr(i32 0), !dbg !25
  %putchar2 = call i32 @putchar(i8 10), !dbg !26
  ret i32 0, !dbg !26
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "putchar", linkageName: "putchar", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
!9 = !DILocation(line: 8, scope: !4)
!10 = !DILocation(line: 9, scope: !4)
!11 = !DILocation(line: 10, scope: !4)
!12 = distinct !DISubprogram(name: "putnbr", linkageName: "putnbr", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 13, scope: !12)
!14 = !DILocation(line: 15, scope: !12)
!15 = !DILocation(line: 19, scope: !12)
!16 = !DILocation(line: 32, scope: !12)
!17 = !DILocation(line: 38, scope: !12)
!18 = !DILocation(line: 44, scope: !12)
!19 = !DILocation(line: 47, scope: !12)
!20 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 50, type: !5, scopeLine: 50, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 51, scope: !20)
!22 = !DILocation(line: 52, scope: !20)
!23 = !DILocation(line: 53, scope: !20)
!24 = !DILocation(line: 54, scope: !20)
!25 = !DILocation(line: 55, scope: !20)
!26 = !DILocation(line: 56, scope: !20)
