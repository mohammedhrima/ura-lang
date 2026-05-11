; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/structs/forward_ref/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/structs/forward_ref/001.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Calc = type { i32 }

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define void @Calc.delete(%struct.Calc* %0) {
entry:
  ret void
}

define %struct.Calc @Calc.new(i32 %v) !dbg !4 {
entry:
  %v1 = alloca i32, align 4, !dbg !7
  %c = alloca %struct.Calc, align 8, !dbg !7
  store i32 %v, i32* %v1, align 4, !dbg !7
  store %struct.Calc zeroinitializer, %struct.Calc* %c, align 4, !dbg !7
  %v2 = load i32, i32* %v1, align 4, !dbg !8
  call void @Calc.set(i32 %v2, %struct.Calc* %c), !dbg !8
  %c3 = load %struct.Calc, %struct.Calc* %c, align 4, !dbg !8
  ret %struct.Calc %c3, !dbg !8
}

define void @Calc.double(%struct.Calc* %self) !dbg !9 {
entry:
  %self1 = alloca %struct.Calc*, align 8, !dbg !10
  store %struct.Calc* %self, %struct.Calc** %self1, align 8, !dbg !10
  %self2 = load %struct.Calc*, %struct.Calc** %self1, align 8, !dbg !10
  %result = getelementptr %struct.Calc, %struct.Calc* %self2, i32 0, i32 0, !dbg !10
  %ref_arg = load %struct.Calc*, %struct.Calc** %self1, align 8, !dbg !11
  %Calc.get = call i32 @Calc.get(%struct.Calc* %ref_arg), !dbg !11
  %MUL = mul i32 %Calc.get, 2, !dbg !11
  store i32 %MUL, i32* %result, align 4, !dbg !11
  ret void, !dbg !11
}

define void @Calc.set(i32 %v, %struct.Calc* %self) !dbg !12 {
entry:
  %v1 = alloca i32, align 4, !dbg !13
  %self2 = alloca %struct.Calc*, align 8, !dbg !13
  store i32 %v, i32* %v1, align 4, !dbg !13
  store %struct.Calc* %self, %struct.Calc** %self2, align 8, !dbg !13
  %self3 = load %struct.Calc*, %struct.Calc** %self2, align 8, !dbg !13
  %result = getelementptr %struct.Calc, %struct.Calc* %self3, i32 0, i32 0, !dbg !13
  %v4 = load i32, i32* %v1, align 4, !dbg !13
  store i32 %v4, i32* %result, align 4, !dbg !13
  ret void, !dbg !13
}

define i32 @Calc.get(%struct.Calc* %self) !dbg !14 {
entry:
  %self1 = alloca %struct.Calc*, align 8, !dbg !15
  store %struct.Calc* %self, %struct.Calc** %self1, align 8, !dbg !15
  %self2 = load %struct.Calc*, %struct.Calc** %self1, align 8, !dbg !15
  %result = getelementptr %struct.Calc, %struct.Calc* %self2, i32 0, i32 0, !dbg !15
  %DOT = load i32, i32* %result, align 4, !dbg !15
  ret i32 %DOT, !dbg !15
}

define i32 @main() !dbg !16 {
entry:
  %c = alloca %struct.Calc, align 8, !dbg !17
  store %struct.Calc zeroinitializer, %struct.Calc* %c, align 4, !dbg !17
  %0 = call %struct.Calc @Calc.new(i32 21), !dbg !18
  %tmp_struct = alloca %struct.Calc, align 8, !dbg !18
  store %struct.Calc %0, %struct.Calc* %tmp_struct, align 4, !dbg !18
  store %struct.Calc %0, %struct.Calc* %c, align 4, !dbg !18
  call void @Calc.double(%struct.Calc* %c), !dbg !19
  %Calc.get = call i32 @Calc.get(%struct.Calc* %c), !dbg !20
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 %Calc.get), !dbg !20
  call void @Calc.delete(%struct.Calc* %c), !dbg !20
  call void @Calc.delete(%struct.Calc* %tmp_struct), !dbg !20
  ret i32 0, !dbg !20
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/structs/forward_ref")
!4 = distinct !DISubprogram(name: "Calc.new", linkageName: "Calc.new", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = distinct !DISubprogram(name: "Calc.double", linkageName: "Calc.double", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 11, scope: !9)
!11 = !DILocation(line: 12, scope: !9)
!12 = distinct !DISubprogram(name: "Calc.set", linkageName: "Calc.set", scope: null, file: !3, line: 14, type: !5, scopeLine: 14, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 14, scope: !12)
!14 = distinct !DISubprogram(name: "Calc.get", linkageName: "Calc.get", scope: null, file: !3, line: 17, type: !5, scopeLine: 17, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 17, scope: !14)
!16 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 20, type: !5, scopeLine: 20, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 20, scope: !16)
!18 = !DILocation(line: 21, scope: !16)
!19 = !DILocation(line: 22, scope: !16)
!20 = !DILocation(line: 23, scope: !16)
