; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic/002.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Weapon = type { i8*, i32 }
%struct.Enemy = type { i8*, i32, %struct.Weapon }
%struct.Room = type { i32, %struct.Enemy }

@STR0 = private unnamed_addr constant [11 x i8] c"Orc Shaman\00", align 1
@STR1 = private unnamed_addr constant [14 x i8] c"Staff of Pain\00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"Floor \00", align 1
@STR3 = private unnamed_addr constant [3 x i8] c": \00", align 1
@STR4 = private unnamed_addr constant [9 x i8] c" wields \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [24 x i8] c"Floor %d: %s wields %s\0A\00", align 1
@STR6 = private unnamed_addr constant [16 x i8] c"Weapon damage: \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [19 x i8] c"Weapon damage: %d\0A\00", align 1

define void @Weapon.init(%struct.Weapon* %0) {
entry:
  store %struct.Weapon zeroinitializer, %struct.Weapon* %0, align 8
  ret void
}

define void @Weapon.clean(%struct.Weapon* %0) {
entry:
  ret void
}

define void @Enemy.init(%struct.Enemy* %0) {
entry:
  store %struct.Enemy zeroinitializer, %struct.Enemy* %0, align 8
  %weapon = getelementptr %struct.Enemy, %struct.Enemy* %0, i32 0, i32 2
  call void @Weapon.init(%struct.Weapon* %weapon)
  ret void
}

define void @Enemy.clean(%struct.Enemy* %0) {
entry:
  %weapon = getelementptr %struct.Enemy, %struct.Enemy* %0, i32 0, i32 2
  call void @Weapon.clean(%struct.Weapon* %weapon)
  ret void
}

define void @Room.init(%struct.Room* %0) {
entry:
  store %struct.Room zeroinitializer, %struct.Room* %0, align 8
  %enemy = getelementptr %struct.Room, %struct.Room* %0, i32 0, i32 1
  call void @Enemy.init(%struct.Enemy* %enemy)
  ret void
}

define void @Room.clean(%struct.Room* %0) {
entry:
  %enemy = getelementptr %struct.Room, %struct.Room* %0, i32 0, i32 1
  call void @Enemy.clean(%struct.Enemy* %enemy)
  ret void
}

define i32 @main() !dbg !4 {
entry:
  %r = alloca %struct.Room, align 8, !dbg !7
  store %struct.Room zeroinitializer, %struct.Room* %r, align 8, !dbg !7
  call void @Room.init(%struct.Room* %r), !dbg !7
  %floor = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 0, !dbg !7
  store i32 3, i32* %floor, align 4, !dbg !7
  %enemy = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 1, !dbg !7
  %name = getelementptr %struct.Enemy, %struct.Enemy* %enemy, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  %enemy1 = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 1, !dbg !7
  %hp = getelementptr %struct.Enemy, %struct.Enemy* %enemy1, i32 0, i32 1, !dbg !7
  store i32 80, i32* %hp, align 4, !dbg !7
  %enemy2 = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 1, !dbg !7
  %weapon = getelementptr %struct.Enemy, %struct.Enemy* %enemy2, i32 0, i32 2, !dbg !7
  %name3 = getelementptr %struct.Weapon, %struct.Weapon* %weapon, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR1, i32 0, i32 0), i8** %name3, align 8, !dbg !7
  %enemy4 = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 1, !dbg !7
  %weapon5 = getelementptr %struct.Enemy, %struct.Enemy* %enemy4, i32 0, i32 2, !dbg !7
  %damage = getelementptr %struct.Weapon, %struct.Weapon* %weapon5, i32 0, i32 1, !dbg !7
  store i32 22, i32* %damage, align 4, !dbg !7
  %floor6 = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 0, !dbg !7
  %DOT = load i32, i32* %floor6, align 4, !dbg !7
  %enemy7 = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 1, !dbg !7
  %name8 = getelementptr %struct.Enemy, %struct.Enemy* %enemy7, i32 0, i32 0, !dbg !7
  %DOT9 = load i8*, i8** %name8, align 8, !dbg !7
  %enemy10 = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 1, !dbg !7
  %weapon11 = getelementptr %struct.Enemy, %struct.Enemy* %enemy10, i32 0, i32 2, !dbg !7
  %name12 = getelementptr %struct.Weapon, %struct.Weapon* %weapon11, i32 0, i32 0, !dbg !7
  %DOT13 = load i8*, i8** %name12, align 8, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @output_fmt, i32 0, i32 0), i32 3, i32 %DOT, i8* %DOT9, i8* %DOT13), !dbg !7
  %enemy14 = getelementptr %struct.Room, %struct.Room* %r, i32 0, i32 1, !dbg !7
  %weapon15 = getelementptr %struct.Enemy, %struct.Enemy* %enemy14, i32 0, i32 2, !dbg !7
  %damage16 = getelementptr %struct.Weapon, %struct.Weapon* %weapon15, i32 0, i32 1, !dbg !7
  %DOT17 = load i32, i32* %damage16, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %DOT17), !dbg !7
  call void @Room.clean(%struct.Room* %r), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 16, type: !5, scopeLine: 16, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 16, scope: !4)
