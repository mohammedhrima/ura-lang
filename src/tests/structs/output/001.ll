; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/output/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/output/001.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Point = type { i32, i32 }
%struct.Color = type { i32, i32, i32 }

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [18 x i8] c"{ x: %d, y: %d }\0A\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [25 x i8] c"{ r: %d, g: %d, b: %d }\0A\00", align 1

define void @Point.delete(%struct.Point* %0) {
entry:
  ret void
}

define void @Color.delete(%struct.Color* %0) {
entry:
  ret void
}

define i32 @main() !dbg !4 {
entry:
  %p = alloca %struct.Point, align 8, !dbg !7
  %c = alloca %struct.Color, align 8, !dbg !7
  store %struct.Point zeroinitializer, %struct.Point* %p, align 4, !dbg !7
  %x = getelementptr %struct.Point, %struct.Point* %p, i32 0, i32 0, !dbg !7
  store i32 3, i32* %x, align 4, !dbg !7
  %y = getelementptr %struct.Point, %struct.Point* %p, i32 0, i32 1, !dbg !7
  store i32 4, i32* %y, align 4, !dbg !7
  %x1 = getelementptr %struct.Point, %struct.Point* %p, i32 0, i32 0, !dbg !7
  %x2 = load i32, i32* %x1, align 4, !dbg !7
  %y3 = getelementptr %struct.Point, %struct.Point* %p, i32 0, i32 1, !dbg !7
  %y4 = load i32, i32* %y3, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %x2, i32 %y4), !dbg !7
  store %struct.Color zeroinitializer, %struct.Color* %c, align 4, !dbg !7
  %r = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 0, !dbg !7
  store i32 255, i32* %r, align 4, !dbg !7
  %g = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 1, !dbg !7
  store i32 128, i32* %g, align 4, !dbg !7
  %b = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 2, !dbg !7
  store i32 0, i32* %b, align 4, !dbg !7
  %r5 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 0, !dbg !7
  %r6 = load i32, i32* %r5, align 4, !dbg !7
  %g7 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 1, !dbg !7
  %g8 = load i32, i32* %g7, align 4, !dbg !7
  %b9 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 2, !dbg !7
  %b10 = load i32, i32* %b9, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @output_fmt.1, i32 0, i32 0), i32 3, i32 %r6, i32 %g8, i32 %b10), !dbg !7
  call void @Point.delete(%struct.Point* %p), !dbg !7
  call void @Color.delete(%struct.Color* %c), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/output")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 12, scope: !4)
