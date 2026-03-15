; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/tuples/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/tuples/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [9 x i8] c"Damage: \00", align 1
@STR1 = private unnamed_addr constant [10 x i8] c" | Crit: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [23 x i8] c"Damage: %d | Crit: %s\0A\00", align 1

define { i32, i1 } @attack(i32 %atk, i32 %def) !dbg !4 {
entry:
  %atk1 = alloca i32, align 4, !dbg !7
  %def2 = alloca i32, align 4, !dbg !7
  %dmg = alloca i32, align 4, !dbg !7
  %is_crit = alloca i1, align 1, !dbg !7
  store i32 %atk, i32* %atk1, align 4, !dbg !7
  store i32 %def, i32* %def2, align 4, !dbg !7
  store i32 0, i32* %dmg, align 4, !dbg !7
  %atk3 = load i32, i32* %atk1, align 4, !dbg !7
  %def4 = load i32, i32* %def2, align 4, !dbg !7
  %SUB = sub i32 %atk3, %def4, !dbg !7
  store i32 %SUB, i32* %dmg, align 4, !dbg !7
  store i1 false, i1* %is_crit, align 1, !dbg !7
  %dmg5 = load i32, i32* %dmg, align 4, !dbg !7
  %GT = icmp sgt i32 %dmg5, 15, !dbg !7
  store i1 %GT, i1* %is_crit, align 1, !dbg !7
  %dmg6 = load i32, i32* %dmg, align 4, !dbg !7
  %0 = insertvalue { i32, i1 } undef, i32 %dmg6, 0, !dbg !7
  %is_crit7 = load i1, i1* %is_crit, align 1, !dbg !7
  %1 = insertvalue { i32, i1 } %0, i1 %is_crit7, 1, !dbg !7
  ret { i32, i1 } %1, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %damage = alloca i32, align 4, !dbg !9
  %crit = alloca i1, align 1, !dbg !9
  %attack = call { i32, i1 } @attack(i32 20, i32 4), !dbg !9
  store i32 0, i32* %damage, align 4, !dbg !9
  %damage1 = extractvalue { i32, i1 } %attack, 0, !dbg !9
  store i32 %damage1, i32* %damage, align 4, !dbg !9
  store i1 false, i1* %crit, align 1, !dbg !9
  %crit2 = extractvalue { i32, i1 } %attack, 1, !dbg !9
  store i1 %crit2, i1* %crit, align 1, !dbg !9
  %damage3 = load i32, i32* %damage, align 4, !dbg !9
  %crit4 = load i1, i1* %crit, align 1, !dbg !9
  %bool_str = select i1 %crit4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %damage3, i8* %bool_str), !dbg !9
  ret i32 0, !dbg !9
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/tuples")
!4 = distinct !DISubprogram(name: "attack", linkageName: "attack", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 9, scope: !8)
