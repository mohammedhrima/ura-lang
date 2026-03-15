; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Color = type { i8, i8, i8, i8 }

@STR0 = private unnamed_addr constant [17 x i8] c"Moving Rectangle\00", align 1
@STR1 = private unnamed_addr constant [8 x i8] c"Moving!\00", align 1

define void @Color.init(%struct.Color* %0) {
entry:
  store %struct.Color zeroinitializer, %struct.Color* %0, align 1
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
  store %struct.Color zeroinitializer, %struct.Color* %c, align 1, !dbg !7
  call void @Color.init(%struct.Color* %c), !dbg !7
  %r5 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 0, !dbg !7
  %r6 = load i32, i32* %r1, align 4, !dbg !7
  %as = trunc i32 %r6 to i8, !dbg !7
  store i8 %as, i8* %r5, align 1, !dbg !7
  %g7 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 1, !dbg !7
  %g8 = load i32, i32* %g2, align 4, !dbg !7
  %as9 = trunc i32 %g8 to i8, !dbg !7
  store i8 %as9, i8* %g7, align 1, !dbg !7
  %b10 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 2, !dbg !7
  %b11 = load i32, i32* %b3, align 4, !dbg !7
  %as12 = trunc i32 %b11 to i8, !dbg !7
  store i8 %as12, i8* %b10, align 1, !dbg !7
  %a13 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 3, !dbg !7
  %a14 = load i32, i32* %a4, align 4, !dbg !7
  %as15 = trunc i32 %a14 to i8, !dbg !7
  store i8 %as15, i8* %a13, align 1, !dbg !7
  call void @Color.clean(%struct.Color* %c), !dbg !7
  %c16 = load %struct.Color, %struct.Color* %c, align 1, !dbg !7
  ret %struct.Color %c16, !dbg !7
}

declare void @InitWindow(i32, i32, i8*)

declare void @CloseWindow()

declare i1 @WindowShouldClose()

declare void @SetTargetFPS(i32)

declare void @BeginDrawing()

declare void @EndDrawing()

declare void @ClearBackground(i64)

declare void @DrawRectangle(i32, i32, i32, i32, i64)

declare void @DrawText(i8*, i32, i32, i32, i64)

define i32 @main() !dbg !8 {
entry:
  %x = alloca i32, align 4, !dbg !9
  %speed = alloca i32, align 4, !dbg !9
  call void @InitWindow(i32 800, i32 600, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @STR0, i32 0, i32 0)), !dbg !9
  call void @SetTargetFPS(i32 60), !dbg !10
  store i32 0, i32* %x, align 4, !dbg !9
  store i32 0, i32* %x, align 4, !dbg !9
  store i32 0, i32* %speed, align 4, !dbg !9
  store i32 3, i32* %speed, align 4, !dbg !9
  br label %while.start, !dbg !9

while.start:                                      ; preds = %if.end, %entry
  %WindowShouldClose = call i1 @WindowShouldClose(), !dbg !11
  %EQ = icmp eq i1 %WindowShouldClose, false, !dbg !11
  br i1 %EQ, label %while.then, label %while.end, !dbg !11

while.then:                                       ; preds = %while.start
  %x1 = load i32, i32* %x, align 4, !dbg !11
  %speed2 = load i32, i32* %speed, align 4, !dbg !11
  %ADD = add i32 %x1, %speed2, !dbg !11
  store i32 %ADD, i32* %x, align 4, !dbg !11
  br label %if.start, !dbg !11

while.end:                                        ; preds = %while.start
  call void @CloseWindow(), !dbg !12
  ret i32 0, !dbg !12

if.start:                                         ; preds = %while.then
  %x3 = load i32, i32* %x, align 4, !dbg !11
  %GT = icmp sgt i32 %x3, 800, !dbg !11
  br i1 %GT, label %if.then, label %if.end, !dbg !11

if.end:                                           ; preds = %if.then, %if.start
  call void @BeginDrawing(), !dbg !13
  %setColor = call %struct.Color @setColor(i32 30, i32 30, i32 150, i32 255), !dbg !14
  %st_slot = alloca %struct.Color, align 8, !dbg !14
  store %struct.Color %setColor, %struct.Color* %st_slot, align 1, !dbg !14
  %i64_slot = alloca i64, align 8, !dbg !14
  %0 = bitcast i64* %i64_slot to i8*, !dbg !14
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !14
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !14
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !14
  call void @ClearBackground(i64 %i64_arg), !dbg !14
  %x4 = load i32, i32* %x, align 4, !dbg !15
  %setColor5 = call %struct.Color @setColor(i32 255, i32 220, i32 0, i32 255), !dbg !15
  %st_slot6 = alloca %struct.Color, align 8, !dbg !15
  store %struct.Color %setColor5, %struct.Color* %st_slot6, align 1, !dbg !15
  %i64_slot7 = alloca i64, align 8, !dbg !15
  %2 = bitcast i64* %i64_slot7 to i8*, !dbg !15
  %3 = bitcast %struct.Color* %st_slot6 to i8*, !dbg !15
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i1 false), !dbg !15
  %i64_arg8 = load i64, i64* %i64_slot7, align 4, !dbg !15
  call void @DrawRectangle(i32 %x4, i32 250, i32 100, i32 100, i64 %i64_arg8), !dbg !15
  %setColor9 = call %struct.Color @setColor(i32 255, i32 255, i32 255, i32 255), !dbg !16
  %st_slot10 = alloca %struct.Color, align 8, !dbg !16
  store %struct.Color %setColor9, %struct.Color* %st_slot10, align 1, !dbg !16
  %i64_slot11 = alloca i64, align 8, !dbg !16
  %4 = bitcast i64* %i64_slot11 to i8*, !dbg !16
  %5 = bitcast %struct.Color* %st_slot10 to i8*, !dbg !16
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* %5, i64 4, i1 false), !dbg !16
  %i64_arg12 = load i64, i64* %i64_slot11, align 4, !dbg !16
  call void @DrawText(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR1, i32 0, i32 0), i32 320, i32 20, i32 30, i64 %i64_arg12), !dbg !16
  call void @EndDrawing(), !dbg !17
  br label %while.start, !dbg !17

if.then:                                          ; preds = %if.start
  store i32 0, i32* %x, align 4, !dbg !11
  br label %if.end, !dbg !11
}

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nounwind willreturn }

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
!11 = !DILocation(line: 10, scope: !8)
!12 = !DILocation(line: 19, scope: !8)
!13 = !DILocation(line: 14, scope: !8)
!14 = !DILocation(line: 15, scope: !8)
!15 = !DILocation(line: 16, scope: !8)
!16 = !DILocation(line: 17, scope: !8)
!17 = !DILocation(line: 18, scope: !8)
