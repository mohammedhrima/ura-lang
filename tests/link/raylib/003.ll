; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Color = type { i8, i8, i8, i8 }

@STR0 = private unnamed_addr constant [11 x i8] c"Ura Raylib\00", align 1

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

declare void @BeginDrawing()

declare void @EndDrawing()

declare void @ClearBackground(i64)

declare void @DrawRectangle(i32, i32, i32, i32, i64)

define i32 @main() !dbg !8 {
entry:
  call void @InitWindow(i32 800, i32 600, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR0, i32 0, i32 0)), !dbg !9
  br label %while.start, !dbg !9

while.start:                                      ; preds = %while.then, %entry
  br i1 true, label %while.then, label %while.end, !dbg !9

while.then:                                       ; preds = %while.start
  call void @BeginDrawing(), !dbg !10
  %setColor = call %struct.Color @setColor(i32 30, i32 30, i32 150, i32 255), !dbg !11
  %st_slot = alloca %struct.Color, align 8, !dbg !11
  store %struct.Color %setColor, %struct.Color* %st_slot, align 1, !dbg !11
  %i64_slot = alloca i64, align 8, !dbg !11
  %0 = bitcast i64* %i64_slot to i8*, !dbg !11
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !11
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !11
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !11
  call void @ClearBackground(i64 %i64_arg), !dbg !11
  %setColor1 = call %struct.Color @setColor(i32 255, i32 220, i32 0, i32 255), !dbg !12
  %st_slot2 = alloca %struct.Color, align 8, !dbg !12
  store %struct.Color %setColor1, %struct.Color* %st_slot2, align 1, !dbg !12
  %i64_slot3 = alloca i64, align 8, !dbg !12
  %2 = bitcast i64* %i64_slot3 to i8*, !dbg !12
  %3 = bitcast %struct.Color* %st_slot2 to i8*, !dbg !12
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i1 false), !dbg !12
  %i64_arg4 = load i64, i64* %i64_slot3, align 4, !dbg !12
  call void @DrawRectangle(i32 350, i32 250, i32 100, i32 100, i64 %i64_arg4), !dbg !12
  call void @EndDrawing(), !dbg !13
  br label %while.start, !dbg !13

while.end:                                        ; preds = %while.start
  call void @CloseWindow(), !dbg !14
  ret i32 0, !dbg !14
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
!10 = !DILocation(line: 8, scope: !8)
!11 = !DILocation(line: 9, scope: !8)
!12 = !DILocation(line: 10, scope: !8)
!13 = !DILocation(line: 11, scope: !8)
!14 = !DILocation(line: 12, scope: !8)
