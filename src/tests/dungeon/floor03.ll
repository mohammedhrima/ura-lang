; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor03.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor03.ura"
target triple = "arm64-apple-macosx16.0.0"

@ORC = constant i32 0
@SPIDER = constant i32 1
@DRAGON = constant i32 2
@GOLEM = constant i32 3
@PHYSICAL = constant i32 0
@FIRE = constant i32 1
@ICE = constant i32 2
@POISON = constant i32 3
@STR0 = private unnamed_addr constant [7 x i8] c"Dragon\00", align 1
@output_fmt = private unnamed_addr constant [7 x i8] c"Dragon\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"Orc\00", align 1
@output_fmt.1 = private unnamed_addr constant [4 x i8] c"Orc\00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"Spider\00", align 1
@output_fmt.2 = private unnamed_addr constant [7 x i8] c"Spider\00", align 1
@STR3 = private unnamed_addr constant [6 x i8] c"Golem\00", align 1
@output_fmt.3 = private unnamed_addr constant [6 x i8] c"Golem\00", align 1
@STR4 = private unnamed_addr constant [6 x i8] c" \E2\80\94 \00", align 1
@output_fmt.4 = private unnamed_addr constant [6 x i8] c" \E2\80\94 \00", align 1
@STR5 = private unnamed_addr constant [6 x i8] c"fire\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [6 x i8] c"fire\0A\00", align 1
@STR6 = private unnamed_addr constant [5 x i8] c"ice\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [5 x i8] c"ice\0A\00", align 1
@STR7 = private unnamed_addr constant [8 x i8] c"poison\0A\00", align 1
@output_fmt.7 = private unnamed_addr constant [8 x i8] c"poison\0A\00", align 1
@STR8 = private unnamed_addr constant [10 x i8] c"physical\0A\00", align 1
@output_fmt.8 = private unnamed_addr constant [10 x i8] c"physical\0A\00", align 1

define void @announce(i32 %enemy, i32 %dtype) !dbg !4 {
entry:
  %enemy1 = alloca i32, align 4, !dbg !7
  %dtype2 = alloca i32, align 4, !dbg !7
  store i32 %enemy, i32* %enemy1, align 4, !dbg !7
  store i32 %dtype, i32* %dtype2, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %enemy3 = load i32, i32* %enemy1, align 4, !dbg !7
  %DRAGON = load i32, i32* @DRAGON, align 4, !dbg !7
  %EQ = icmp eq i32 %enemy3, %DRAGON, !dbg !7
  br i1 %EQ, label %if.then, label %elif.start, !dbg !7

if.end:                                           ; preds = %if.else, %elif.then7, %elif.then, %if.then
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt.4, i32 0, i32 0)), !dbg !7
  br label %if.start10, !dbg !7

if.then:                                          ; preds = %if.start
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @output_fmt, i32 0, i32 0)), !dbg !7
  br label %if.end, !dbg !7

elif.start:                                       ; preds = %if.start
  %enemy5 = load i32, i32* %enemy1, align 4, !dbg !7
  %ORC = load i32, i32* @ORC, align 4, !dbg !7
  %EQ6 = icmp eq i32 %enemy5, %ORC, !dbg !7
  br i1 %EQ6, label %elif.then, label %elif.start4, !dbg !7

elif.then:                                        ; preds = %elif.start
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt.1, i32 0, i32 0)), !dbg !7
  br label %if.end, !dbg !7

elif.start4:                                      ; preds = %elif.start
  %enemy8 = load i32, i32* %enemy1, align 4, !dbg !7
  %SPIDER = load i32, i32* @SPIDER, align 4, !dbg !7
  %EQ9 = icmp eq i32 %enemy8, %SPIDER, !dbg !7
  br i1 %EQ9, label %elif.then7, label %if.else, !dbg !7

elif.then7:                                       ; preds = %elif.start4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @output_fmt.2, i32 0, i32 0)), !dbg !7
  br label %if.end, !dbg !7

if.else:                                          ; preds = %elif.start4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt.3, i32 0, i32 0)), !dbg !7
  br label %if.end, !dbg !7

if.start10:                                       ; preds = %if.end
  %dtype14 = load i32, i32* %dtype2, align 4, !dbg !7
  %FIRE = load i32, i32* @FIRE, align 4, !dbg !7
  %EQ15 = icmp eq i32 %dtype14, %FIRE, !dbg !7
  br i1 %EQ15, label %if.then12, label %elif.start13, !dbg !7

if.end11:                                         ; preds = %if.else21, %elif.then20, %elif.then16, %if.then12
  ret void, !dbg !7

if.then12:                                        ; preds = %if.start10
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt.5, i32 0, i32 0)), !dbg !7
  br label %if.end11, !dbg !7

elif.start13:                                     ; preds = %if.start10
  %dtype18 = load i32, i32* %dtype2, align 4, !dbg !7
  %ICE = load i32, i32* @ICE, align 4, !dbg !7
  %EQ19 = icmp eq i32 %dtype18, %ICE, !dbg !7
  br i1 %EQ19, label %elif.then16, label %elif.start17, !dbg !7

elif.then16:                                      ; preds = %elif.start13
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @output_fmt.6, i32 0, i32 0)), !dbg !7
  br label %if.end11, !dbg !7

elif.start17:                                     ; preds = %elif.start13
  %dtype22 = load i32, i32* %dtype2, align 4, !dbg !7
  %POISON = load i32, i32* @POISON, align 4, !dbg !7
  %EQ23 = icmp eq i32 %dtype22, %POISON, !dbg !7
  br i1 %EQ23, label %elif.then20, label %if.else21, !dbg !7

elif.then20:                                      ; preds = %elif.start17
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt.7, i32 0, i32 0)), !dbg !7
  br label %if.end11, !dbg !7

if.else21:                                        ; preds = %elif.start17
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @output_fmt.8, i32 0, i32 0)), !dbg !7
  br label %if.end11, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %DRAGON = load i32, i32* @DRAGON, align 4, !dbg !9
  %FIRE = load i32, i32* @FIRE, align 4, !dbg !9
  call void @announce(i32 %DRAGON, i32 %FIRE), !dbg !9
  %ORC = load i32, i32* @ORC, align 4, !dbg !10
  %PHYSICAL = load i32, i32* @PHYSICAL, align 4, !dbg !10
  call void @announce(i32 %ORC, i32 %PHYSICAL), !dbg !10
  %SPIDER = load i32, i32* @SPIDER, align 4, !dbg !11
  %POISON = load i32, i32* @POISON, align 4, !dbg !11
  call void @announce(i32 %SPIDER, i32 %POISON), !dbg !11
  ret i32 0, !dbg !11
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor03.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
!4 = distinct !DISubprogram(name: "announce", linkageName: "announce", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 20, scope: !8)
!10 = !DILocation(line: 21, scope: !8)
!11 = !DILocation(line: 22, scope: !8)
