; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Color = type { i32, i32, i32, i32 }

@STR0 = private unnamed_addr constant [16 x i8] c"Hello from Ura!\00", align 1
@STR1 = private unnamed_addr constant [16 x i8] c"Hello from Ura!\00", align 1

define void @Color.init(%struct.Color* %0) {
entry:
  store %struct.Color zeroinitializer, %struct.Color* %0, align 4
  ret void
}

define void @Color.clean(%struct.Color* %0) {
entry:
  ret void
}

define %struct.Color @setColor(i32 %r, i32 %g, i32 %b, i32 %a) !dbg !4 {
entry:
  %r1 = alloca i32, align 4, !dbg !7
  %g2 = alloca i32, align 4, !dbg !7
  %b3 = alloca i32, align 4, !dbg !7
  %a4 = alloca i32, align 4, !dbg !7
  %c = alloca %struct.Color, align 8, !dbg !7
  store i32 %r, i32* %r1, align 4, !dbg !7
  store i32 %g, i32* %g2, align 4, !dbg !7
  store i32 %b, i32* %b3, align 4, !dbg !7
  store i32 %a, i32* %a4, align 4, !dbg !7
  store %struct.Color zeroinitializer, %struct.Color* %c, align 4, !dbg !7
  call void @Color.init(%struct.Color* %c), !dbg !7
  %r5 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 0, !dbg !7
  %r6 = load i32, i32* %r1, align 4, !dbg !7
  store i32 %r6, i32* %r5, align 4, !dbg !7
  %g7 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 1, !dbg !7
  %g8 = load i32, i32* %g2, align 4, !dbg !7
  store i32 %g8, i32* %g7, align 4, !dbg !7
  %b9 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 2, !dbg !7
  %b10 = load i32, i32* %b3, align 4, !dbg !7
  store i32 %b10, i32* %b9, align 4, !dbg !7
  %a11 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 3, !dbg !7
  %a12 = load i32, i32* %a4, align 4, !dbg !7
  store i32 %a12, i32* %a11, align 4, !dbg !7
  call void @Color.clean(%struct.Color* %c), !dbg !7
  %c13 = load %struct.Color, %struct.Color* %c, align 4, !dbg !7
  ret %struct.Color %c13, !dbg !7
}

declare void @InitWindow(i32, i32, i8*)

declare void @CloseWindow()

declare i1 @WindowShouldClose()

declare void @SetTargetFPS(i32)

declare void @BeginDrawing()

declare void @EndDrawing()

declare void @ClearBackground(%struct.Color)

declare void @DrawText(i8*, i32, i32, i32, %struct.Color)

define i32 @main() !dbg !8 {
entry:
  call void @InitWindow(i32 800, i32 600, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR0, i32 0, i32 0)), !dbg !9
  call void @SetTargetFPS(i32 60), !dbg !10
  br label %while.start, !dbg !10

while.start:                                      ; preds = %while.then, %entry
  %WindowShouldClose = call i1 @WindowShouldClose(), !dbg !11
  %EQ = icmp eq i1 %WindowShouldClose, false, !dbg !11
  br i1 %EQ, label %while.then, label %while.end, !dbg !11

while.then:                                       ; preds = %while.start
  call void @BeginDrawing(), !dbg !12
  %setColor = call %struct.Color @setColor(i32 20, i32 20, i32 20, i32 255), !dbg !13
  call void @ClearBackground(%struct.Color %setColor), !dbg !13
  %setColor1 = call %struct.Color @setColor(i32 255, i32 255, i32 255, i32 255), !dbg !14
  call void @DrawText(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR1, i32 0, i32 0), i32 250, i32 270, i32 40, %struct.Color %setColor1), !dbg !14
  call void @EndDrawing(), !dbg !15
  br label %while.start, !dbg !15

while.end:                                        ; preds = %while.start
  call void @CloseWindow(), !dbg !16
  ret i32 0, !dbg !16
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "setColor", linkageName: "setColor", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 9, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 7, scope: !8)
!11 = !DILocation(line: 9, scope: !8)
!12 = !DILocation(line: 10, scope: !8)
!13 = !DILocation(line: 11, scope: !8)
!14 = !DILocation(line: 12, scope: !8)
!15 = !DILocation(line: 13, scope: !8)
!16 = !DILocation(line: 15, scope: !8)
