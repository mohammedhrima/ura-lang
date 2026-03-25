; ModuleID = 'tests/dungeon/boss.ura'
source_filename = "tests/dungeon/boss.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Stats = type { i32, i32, i32 }
%struct.Hero = type { i8*, %struct.Stats, i32 }

@score = global i32 0
@floor = global i32 1
@ORC = constant i32 0
@SPIDER = constant i32 1
@DRAGON = constant i32 2
@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c"  \00", align 1
@STR2 = private unnamed_addr constant [4 x i8] c" lv\00", align 1
@STR3 = private unnamed_addr constant [6 x i8] c"  HP:\00", align 1
@STR4 = private unnamed_addr constant [7 x i8] c"  ATK:\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [26 x i8] c"  %s lv%d  HP:%d  ATK:%d\0A\00", align 1
@STR6 = private unnamed_addr constant [7 x i8] c"Dragon\00", align 1
@STR7 = private unnamed_addr constant [7 x i8] c"Spider\00", align 1
@STR8 = private unnamed_addr constant [4 x i8] c"Orc\00", align 1
@STR9 = private unnamed_addr constant [4 x i8] c"  [\00", align 1
@STR10 = private unnamed_addr constant [9 x i8] c" \E2\80\94 HP:\00", align 1
@STR11 = private unnamed_addr constant [6 x i8] c" ATK:\00", align 1
@STR12 = private unnamed_addr constant [3 x i8] c"]\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [25 x i8] c"  [%s \E2\80\94 HP:%d ATK:%d]\0A\00", align 1
@STR13 = private unnamed_addr constant [5 x i8] c"=== \00", align 1
@STR14 = private unnamed_addr constant [25 x i8] c" enters the dungeon ===\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [31 x i8] c"=== %s enters the dungeon ===\0A\00", align 1
@STR15 = private unnamed_addr constant [7 x i8] c"Floor \00", align 1
@STR16 = private unnamed_addr constant [12 x i8] c" encounter \00", align 1
@STR17 = private unnamed_addr constant [3 x i8] c":\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [24 x i8] c"Floor %d encounter %d:\0A\00", align 1
@STR18 = private unnamed_addr constant [12 x i8] c"  Victory!\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [12 x i8] c"  Victory!\0A\00", align 1
@STR19 = private unnamed_addr constant [14 x i8] c"  Hero fell.\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [14 x i8] c"  Hero fell.\0A\00", align 1
@STR20 = private unnamed_addr constant [27 x i8] c"=== Run complete | Score: \00", align 1
@STR21 = private unnamed_addr constant [11 x i8] c" | Floor: \00", align 1
@STR22 = private unnamed_addr constant [6 x i8] c" ===\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [46 x i8] c"=== Run complete | Score: %d | Floor: %d ===\0A\00", align 1

define void @Stats.init(%struct.Stats* %0) {
entry:
  store %struct.Stats zeroinitializer, %struct.Stats* %0, align 4
  ret void
}

define void @Stats.clean(%struct.Stats* %0) {
entry:
  ret void
}

define void @Hero.clean(%struct.Hero* %0) {
entry:
  %stats = getelementptr %struct.Hero, %struct.Hero* %0, i32 0, i32 1
  call void @Stats.clean(%struct.Stats* %stats)
  ret void
}

define void @Hero.init(%struct.Hero* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !7
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !7
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %stats = getelementptr %struct.Hero, %struct.Hero* %self2, i32 0, i32 1, !dbg !7
  call void @Stats.init(%struct.Stats* %stats), !dbg !7
  %self3 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %name = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  %self4 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %stats5 = getelementptr %struct.Hero, %struct.Hero* %self4, i32 0, i32 1, !dbg !7
  %hp = getelementptr %struct.Stats, %struct.Stats* %stats5, i32 0, i32 0, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  %self6 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %stats7 = getelementptr %struct.Hero, %struct.Hero* %self6, i32 0, i32 1, !dbg !7
  %atk = getelementptr %struct.Stats, %struct.Stats* %stats7, i32 0, i32 1, !dbg !7
  store i32 20, i32* %atk, align 4, !dbg !7
  %self8 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %stats9 = getelementptr %struct.Hero, %struct.Hero* %self8, i32 0, i32 1, !dbg !7
  %def = getelementptr %struct.Stats, %struct.Stats* %stats9, i32 0, i32 2, !dbg !7
  store i32 5, i32* %def, align 4, !dbg !7
  %self10 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %level = getelementptr %struct.Hero, %struct.Hero* %self10, i32 0, i32 2, !dbg !7
  store i32 1, i32* %level, align 4, !dbg !7
  ret void, !dbg !7
}

define i1 @Hero.is_alive(%struct.Hero* %self) !dbg !8 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !9
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !9
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !9
  %stats = getelementptr %struct.Hero, %struct.Hero* %self2, i32 0, i32 1, !dbg !9
  %hp = getelementptr %struct.Stats, %struct.Stats* %stats, i32 0, i32 0, !dbg !9
  %DOT = load i32, i32* %hp, align 4, !dbg !9
  %GT = icmp sgt i32 %DOT, 0, !dbg !9
  ret i1 %GT, !dbg !9
}

define void @Hero.take_hit(i32 %dmg, %struct.Hero* %self) !dbg !10 {
entry:
  %dmg1 = alloca i32, align 4, !dbg !11
  %self2 = alloca %struct.Hero*, align 8, !dbg !11
  %actual = alloca i32, align 4, !dbg !11
  store i32 %dmg, i32* %dmg1, align 4, !dbg !11
  store %struct.Hero* %self, %struct.Hero** %self2, align 8, !dbg !11
  store i32 0, i32* %actual, align 4, !dbg !11
  %self3 = load %struct.Hero*, %struct.Hero** %self2, align 8, !dbg !11
  %stats = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 1, !dbg !11
  %def = getelementptr %struct.Stats, %struct.Stats* %stats, i32 0, i32 2, !dbg !11
  %dmg4 = load i32, i32* %dmg1, align 4, !dbg !11
  %DOT = load i32, i32* %def, align 4, !dbg !11
  %SUB = sub i32 %dmg4, %DOT, !dbg !11
  store i32 %SUB, i32* %actual, align 4, !dbg !11
  br label %if.start, !dbg !11

if.start:                                         ; preds = %entry
  %actual5 = load i32, i32* %actual, align 4, !dbg !11
  %LT = icmp slt i32 %actual5, 1, !dbg !11
  br i1 %LT, label %if.then, label %if.end, !dbg !11

if.end:                                           ; preds = %if.then, %if.start
  %self6 = load %struct.Hero*, %struct.Hero** %self2, align 8, !dbg !11
  %stats7 = getelementptr %struct.Hero, %struct.Hero* %self6, i32 0, i32 1, !dbg !11
  %hp = getelementptr %struct.Stats, %struct.Stats* %stats7, i32 0, i32 0, !dbg !11
  %self8 = load %struct.Hero*, %struct.Hero** %self2, align 8, !dbg !11
  %stats9 = getelementptr %struct.Hero, %struct.Hero* %self8, i32 0, i32 1, !dbg !11
  %hp10 = getelementptr %struct.Stats, %struct.Stats* %stats9, i32 0, i32 0, !dbg !11
  %DOT11 = load i32, i32* %hp10, align 4, !dbg !11
  %actual12 = load i32, i32* %actual, align 4, !dbg !11
  %SUB13 = sub i32 %DOT11, %actual12, !dbg !11
  store i32 %SUB13, i32* %hp, align 4, !dbg !11
  ret void, !dbg !11

if.then:                                          ; preds = %if.start
  store i32 1, i32* %actual, align 4, !dbg !11
  br label %if.end, !dbg !11
}

define void @Hero.level_up(%struct.Hero* %self) !dbg !12 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !13
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !13
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %level = getelementptr %struct.Hero, %struct.Hero* %self2, i32 0, i32 2, !dbg !13
  %self3 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %level4 = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 2, !dbg !13
  %DOT = load i32, i32* %level4, align 4, !dbg !13
  %ADD = add i32 %DOT, 1, !dbg !13
  store i32 %ADD, i32* %level, align 4, !dbg !13
  %self5 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %stats = getelementptr %struct.Hero, %struct.Hero* %self5, i32 0, i32 1, !dbg !13
  %atk = getelementptr %struct.Stats, %struct.Stats* %stats, i32 0, i32 1, !dbg !13
  %self6 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %stats7 = getelementptr %struct.Hero, %struct.Hero* %self6, i32 0, i32 1, !dbg !13
  %atk8 = getelementptr %struct.Stats, %struct.Stats* %stats7, i32 0, i32 1, !dbg !13
  %DOT9 = load i32, i32* %atk8, align 4, !dbg !13
  %ADD10 = add i32 %DOT9, 5, !dbg !13
  store i32 %ADD10, i32* %atk, align 4, !dbg !13
  %self11 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %stats12 = getelementptr %struct.Hero, %struct.Hero* %self11, i32 0, i32 1, !dbg !13
  %def = getelementptr %struct.Stats, %struct.Stats* %stats12, i32 0, i32 2, !dbg !13
  %self13 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %stats14 = getelementptr %struct.Hero, %struct.Hero* %self13, i32 0, i32 1, !dbg !13
  %def15 = getelementptr %struct.Stats, %struct.Stats* %stats14, i32 0, i32 2, !dbg !13
  %DOT16 = load i32, i32* %def15, align 4, !dbg !13
  %ADD17 = add i32 %DOT16, 2, !dbg !13
  store i32 %ADD17, i32* %def, align 4, !dbg !13
  %self18 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %stats19 = getelementptr %struct.Hero, %struct.Hero* %self18, i32 0, i32 1, !dbg !13
  %hp = getelementptr %struct.Stats, %struct.Stats* %stats19, i32 0, i32 0, !dbg !13
  %self20 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %stats21 = getelementptr %struct.Hero, %struct.Hero* %self20, i32 0, i32 1, !dbg !13
  %hp22 = getelementptr %struct.Stats, %struct.Stats* %stats21, i32 0, i32 0, !dbg !13
  %DOT23 = load i32, i32* %hp22, align 4, !dbg !13
  %ADD24 = add i32 %DOT23, 20, !dbg !13
  store i32 %ADD24, i32* %hp, align 4, !dbg !13
  ret void, !dbg !13
}

define void @Hero.status(%struct.Hero* %self) !dbg !14 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !15
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !15
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !15
  %name = getelementptr %struct.Hero, %struct.Hero* %self2, i32 0, i32 0, !dbg !15
  %DOT = load i8*, i8** %name, align 8, !dbg !15
  %self3 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !15
  %level = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 2, !dbg !15
  %DOT4 = load i32, i32* %level, align 4, !dbg !15
  %self5 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !15
  %stats = getelementptr %struct.Hero, %struct.Hero* %self5, i32 0, i32 1, !dbg !15
  %hp = getelementptr %struct.Stats, %struct.Stats* %stats, i32 0, i32 0, !dbg !15
  %DOT6 = load i32, i32* %hp, align 4, !dbg !15
  %self7 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !15
  %stats8 = getelementptr %struct.Hero, %struct.Hero* %self7, i32 0, i32 1, !dbg !15
  %atk = getelementptr %struct.Stats, %struct.Stats* %stats8, i32 0, i32 1, !dbg !15
  %DOT9 = load i32, i32* %atk, align 4, !dbg !15
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @output_fmt, i32 0, i32 0), i32 4, i8* %DOT, i32 %DOT4, i32 %DOT6, i32 %DOT9), !dbg !15
  ret void, !dbg !15
}

declare i32 @printf(i8*, i32, ...)

define i32 @enemy_hp(i32 %t) !dbg !16 {
entry:
  %t1 = alloca i32, align 4, !dbg !17
  store i32 %t, i32* %t1, align 4, !dbg !17
  br label %if.start, !dbg !17

if.start:                                         ; preds = %entry
  %t2 = load i32, i32* %t1, align 4, !dbg !17
  %DRAGON = load i32, i32* @DRAGON, align 4, !dbg !17
  %EQ = icmp eq i32 %t2, %DRAGON, !dbg !17
  br i1 %EQ, label %if.then, label %if.end, !dbg !17

if.end:                                           ; preds = %if.start
  br label %if.start3, !dbg !17

if.then:                                          ; preds = %if.start
  ret i32 120, !dbg !17

if.start3:                                        ; preds = %if.end
  %t6 = load i32, i32* %t1, align 4, !dbg !17
  %SPIDER = load i32, i32* @SPIDER, align 4, !dbg !17
  %EQ7 = icmp eq i32 %t6, %SPIDER, !dbg !17
  br i1 %EQ7, label %if.then5, label %if.end4, !dbg !17

if.end4:                                          ; preds = %if.start3
  ret i32 60, !dbg !17

if.then5:                                         ; preds = %if.start3
  ret i32 40, !dbg !17
}

define i32 @enemy_atk(i32 %t) !dbg !18 {
entry:
  %t1 = alloca i32, align 4, !dbg !19
  store i32 %t, i32* %t1, align 4, !dbg !19
  br label %if.start, !dbg !19

if.start:                                         ; preds = %entry
  %t2 = load i32, i32* %t1, align 4, !dbg !19
  %DRAGON = load i32, i32* @DRAGON, align 4, !dbg !19
  %EQ = icmp eq i32 %t2, %DRAGON, !dbg !19
  br i1 %EQ, label %if.then, label %if.end, !dbg !19

if.end:                                           ; preds = %if.start
  br label %if.start3, !dbg !19

if.then:                                          ; preds = %if.start
  ret i32 22, !dbg !19

if.start3:                                        ; preds = %if.end
  %t6 = load i32, i32* %t1, align 4, !dbg !19
  %SPIDER = load i32, i32* @SPIDER, align 4, !dbg !19
  %EQ7 = icmp eq i32 %t6, %SPIDER, !dbg !19
  br i1 %EQ7, label %if.then5, label %if.end4, !dbg !19

if.end4:                                          ; preds = %if.start3
  ret i32 12, !dbg !19

if.then5:                                         ; preds = %if.start3
  ret i32 8, !dbg !19
}

define i8* @enemy_name(i32 %t) !dbg !20 {
entry:
  %t1 = alloca i32, align 4, !dbg !21
  store i32 %t, i32* %t1, align 4, !dbg !21
  br label %if.start, !dbg !21

if.start:                                         ; preds = %entry
  %t2 = load i32, i32* %t1, align 4, !dbg !21
  %DRAGON = load i32, i32* @DRAGON, align 4, !dbg !21
  %EQ = icmp eq i32 %t2, %DRAGON, !dbg !21
  br i1 %EQ, label %if.then, label %if.end, !dbg !21

if.end:                                           ; preds = %if.start
  br label %if.start3, !dbg !21

if.then:                                          ; preds = %if.start
  ret i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR6, i32 0, i32 0), !dbg !21

if.start3:                                        ; preds = %if.end
  %t6 = load i32, i32* %t1, align 4, !dbg !21
  %SPIDER = load i32, i32* @SPIDER, align 4, !dbg !21
  %EQ7 = icmp eq i32 %t6, %SPIDER, !dbg !21
  br i1 %EQ7, label %if.then5, label %if.end4, !dbg !21

if.end4:                                          ; preds = %if.start3
  ret i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR8, i32 0, i32 0), !dbg !21

if.then5:                                         ; preds = %if.start3
  ret i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR7, i32 0, i32 0), !dbg !21
}

define i1 @fight(%struct.Hero* %hero, i32 %enemy) !dbg !22 {
entry:
  %hero1 = alloca %struct.Hero*, align 8, !dbg !23
  %enemy2 = alloca i32, align 4, !dbg !23
  %ehp = alloca i32, align 4, !dbg !23
  %eatk = alloca i32, align 4, !dbg !23
  %round = alloca i32, align 4, !dbg !23
  store %struct.Hero* %hero, %struct.Hero** %hero1, align 8, !dbg !23
  store i32 %enemy, i32* %enemy2, align 4, !dbg !23
  store i32 0, i32* %ehp, align 4, !dbg !23
  %enemy3 = load i32, i32* %enemy2, align 4, !dbg !24
  %enemy_hp = call i32 @enemy_hp(i32 %enemy3), !dbg !24
  store i32 %enemy_hp, i32* %ehp, align 4, !dbg !24
  store i32 0, i32* %eatk, align 4, !dbg !23
  %enemy4 = load i32, i32* %enemy2, align 4, !dbg !25
  %enemy_atk = call i32 @enemy_atk(i32 %enemy4), !dbg !25
  store i32 %enemy_atk, i32* %eatk, align 4, !dbg !25
  %enemy5 = load i32, i32* %enemy2, align 4, !dbg !26
  %enemy_name = call i8* @enemy_name(i32 %enemy5), !dbg !26
  %ehp6 = load i32, i32* %ehp, align 4, !dbg !26
  %eatk7 = load i32, i32* %eatk, align 4, !dbg !26
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @output_fmt.1, i32 0, i32 0), i32 3, i8* %enemy_name, i32 %ehp6, i32 %eatk7), !dbg !26
  store i32 0, i32* %round, align 4, !dbg !23
  store i32 0, i32* %round, align 4, !dbg !23
  br label %while.start, !dbg !23

while.start:                                      ; preds = %if.end, %entry
  %ref_arg = load %struct.Hero*, %struct.Hero** %hero1, align 8, !dbg !27
  %Hero.is_alive = call i1 @Hero.is_alive(%struct.Hero* %ref_arg), !dbg !27
  %ehp8 = load i32, i32* %ehp, align 4, !dbg !27
  %GT = icmp sgt i32 %ehp8, 0, !dbg !27
  %AND = and i1 %Hero.is_alive, %GT, !dbg !27
  br i1 %AND, label %while.then, label %while.end, !dbg !27

while.then:                                       ; preds = %while.start
  %round9 = load i32, i32* %round, align 4, !dbg !27
  %ADD = add i32 %round9, 1, !dbg !27
  store i32 %ADD, i32* %round, align 4, !dbg !27
  %hero10 = load %struct.Hero*, %struct.Hero** %hero1, align 8, !dbg !27
  %stats = getelementptr %struct.Hero, %struct.Hero* %hero10, i32 0, i32 1, !dbg !27
  %atk = getelementptr %struct.Stats, %struct.Stats* %stats, i32 0, i32 1, !dbg !27
  %ehp11 = load i32, i32* %ehp, align 4, !dbg !27
  %DOT = load i32, i32* %atk, align 4, !dbg !27
  %SUB = sub i32 %ehp11, %DOT, !dbg !27
  store i32 %SUB, i32* %ehp, align 4, !dbg !27
  br label %if.start, !dbg !27

while.end:                                        ; preds = %while.start
  br label %if.start16, !dbg !28

if.start:                                         ; preds = %while.then
  %ehp12 = load i32, i32* %ehp, align 4, !dbg !27
  %GT13 = icmp sgt i32 %ehp12, 0, !dbg !27
  br i1 %GT13, label %if.then, label %if.end, !dbg !27

if.end:                                           ; preds = %if.then, %if.start
  br label %while.start, !dbg !28

if.then:                                          ; preds = %if.start
  %eatk14 = load i32, i32* %eatk, align 4, !dbg !28
  %ref_arg15 = load %struct.Hero*, %struct.Hero** %hero1, align 8, !dbg !28
  call void @Hero.take_hit(i32 %eatk14, %struct.Hero* %ref_arg15), !dbg !28
  br label %if.end, !dbg !28

if.start16:                                       ; preds = %while.end
  %ref_arg19 = load %struct.Hero*, %struct.Hero** %hero1, align 8, !dbg !29
  %Hero.is_alive20 = call i1 @Hero.is_alive(%struct.Hero* %ref_arg19), !dbg !29
  br i1 %Hero.is_alive20, label %if.then18, label %if.end17, !dbg !29

if.end17:                                         ; preds = %if.start16
  ret i1 false, !dbg !29

if.then18:                                        ; preds = %if.start16
  %floor = load i32, i32* @floor, align 4, !dbg !29
  %MUL = mul i32 50, %floor, !dbg !29
  %score = load i32, i32* @score, align 4, !dbg !29
  %ADD21 = add i32 %score, %MUL, !dbg !29
  store i32 %ADD21, i32* @score, align 4, !dbg !29
  ret i1 true, !dbg !29
}

define i32 @main() !dbg !30 {
entry:
  %hero = alloca %struct.Hero, align 8, !dbg !31
  %encounters = alloca i32, align 4, !dbg !31
  %i = alloca i32, align 4, !dbg !31
  %enemy = alloca i32, align 4, !dbg !31
  %won = alloca i1, align 1, !dbg !31
  store %struct.Hero zeroinitializer, %struct.Hero* %hero, align 8, !dbg !31
  call void @Hero.init(%struct.Hero* %hero), !dbg !31
  %name = getelementptr %struct.Hero, %struct.Hero* %hero, i32 0, i32 0, !dbg !31
  %DOT = load i8*, i8** %name, align 8, !dbg !31
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* %DOT), !dbg !31
  call void @Hero.status(%struct.Hero* %hero), !dbg !32
  store i32 0, i32* %encounters, align 4, !dbg !31
  store i32 3, i32* %encounters, align 4, !dbg !31
  store i32 0, i32* %i, align 4, !dbg !31
  store i32 0, i32* %i, align 4, !dbg !31
  br label %while.start, !dbg !31

while.start:                                      ; preds = %if.end, %entry
  %i1 = load i32, i32* %i, align 4, !dbg !31
  %encounters2 = load i32, i32* %encounters, align 4, !dbg !31
  %LT = icmp slt i32 %i1, %encounters2, !dbg !31
  %Hero.is_alive = call i1 @Hero.is_alive(%struct.Hero* %hero), !dbg !33
  %AND = and i1 %LT, %Hero.is_alive, !dbg !33
  br i1 %AND, label %while.then, label %while.end, !dbg !33

while.then:                                       ; preds = %while.start
  store i32 0, i32* %enemy, align 4, !dbg !31
  %i3 = load i32, i32* %i, align 4, !dbg !31
  %MOD = srem i32 %i3, 3, !dbg !31
  store i32 %MOD, i32* %enemy, align 4, !dbg !31
  %floor = load i32, i32* @floor, align 4, !dbg !31
  %i4 = load i32, i32* %i, align 4, !dbg !31
  %ADD = add i32 %i4, 1, !dbg !31
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @output_fmt.3, i32 0, i32 0), i32 2, i32 %floor, i32 %ADD), !dbg !31
  store i1 false, i1* %won, align 1, !dbg !31
  %enemy5 = load i32, i32* %enemy, align 4, !dbg !34
  %fight = call i1 @fight(%struct.Hero* %hero, i32 %enemy5), !dbg !34
  store i1 %fight, i1* %won, align 1, !dbg !34
  br label %if.start, !dbg !34

while.end:                                        ; preds = %while.start
  %score = load i32, i32* @score, align 4, !dbg !35
  %floor16 = load i32, i32* @floor, align 4, !dbg !35
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @output_fmt.6, i32 0, i32 0), i32 2, i32 %score, i32 %floor16), !dbg !35
  call void @Hero.clean(%struct.Hero* %hero), !dbg !35
  ret i32 0, !dbg !35

if.start:                                         ; preds = %while.then
  %won6 = load i1, i1* %won, align 1, !dbg !34
  br i1 %won6, label %if.then, label %if.else, !dbg !34

if.end:                                           ; preds = %if.else, %if.end8
  call void @Hero.status(%struct.Hero* %hero), !dbg !35
  %i14 = load i32, i32* %i, align 4, !dbg !35
  %ADD15 = add i32 %i14, 1, !dbg !35
  store i32 %ADD15, i32* %i, align 4, !dbg !35
  br label %while.start, !dbg !35

if.then:                                          ; preds = %if.start
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.4, i32 0, i32 0), i32 0), !dbg !34
  br label %if.start7, !dbg !34

if.else:                                          ; preds = %if.start
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.5, i32 0, i32 0), i32 0), !dbg !36
  br label %if.end, !dbg !36

if.start7:                                        ; preds = %if.then
  %encounters10 = load i32, i32* %encounters, align 4, !dbg !34
  %SUB = sub i32 %encounters10, 1, !dbg !34
  %i11 = load i32, i32* %i, align 4, !dbg !34
  %EQ = icmp eq i32 %i11, %SUB, !dbg !34
  br i1 %EQ, label %if.then9, label %if.end8, !dbg !34

if.end8:                                          ; preds = %if.then9, %if.start7
  br label %if.end, !dbg !36

if.then9:                                         ; preds = %if.start7
  call void @Hero.level_up(%struct.Hero* %hero), !dbg !36
  %floor12 = load i32, i32* @floor, align 4, !dbg !36
  %ADD13 = add i32 %floor12, 1, !dbg !36
  store i32 %ADD13, i32* @floor, align 4, !dbg !36
  br label %if.end8, !dbg !36
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "boss.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "Hero.init", linkageName: "Hero.init", scope: null, file: !3, line: 18, type: !5, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 18, scope: !4)
!8 = distinct !DISubprogram(name: "Hero.is_alive", linkageName: "Hero.is_alive", scope: null, file: !3, line: 25, type: !5, scopeLine: 25, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 25, scope: !8)
!10 = distinct !DISubprogram(name: "Hero.take_hit", linkageName: "Hero.take_hit", scope: null, file: !3, line: 28, type: !5, scopeLine: 28, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 28, scope: !10)
!12 = distinct !DISubprogram(name: "Hero.level_up", linkageName: "Hero.level_up", scope: null, file: !3, line: 33, type: !5, scopeLine: 33, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 33, scope: !12)
!14 = distinct !DISubprogram(name: "Hero.status", linkageName: "Hero.status", scope: null, file: !3, line: 39, type: !5, scopeLine: 39, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 39, scope: !14)
!16 = distinct !DISubprogram(name: "enemy_hp", linkageName: "enemy_hp", scope: null, file: !3, line: 44, type: !5, scopeLine: 44, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 44, scope: !16)
!18 = distinct !DISubprogram(name: "enemy_atk", linkageName: "enemy_atk", scope: null, file: !3, line: 49, type: !5, scopeLine: 49, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 49, scope: !18)
!20 = distinct !DISubprogram(name: "enemy_name", linkageName: "enemy_name", scope: null, file: !3, line: 54, type: !5, scopeLine: 54, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 54, scope: !20)
!22 = distinct !DISubprogram(name: "fight", linkageName: "fight", scope: null, file: !3, line: 59, type: !5, scopeLine: 59, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!23 = !DILocation(line: 59, scope: !22)
!24 = !DILocation(line: 60, scope: !22)
!25 = !DILocation(line: 61, scope: !22)
!26 = !DILocation(line: 62, scope: !22)
!27 = !DILocation(line: 64, scope: !22)
!28 = !DILocation(line: 68, scope: !22)
!29 = !DILocation(line: 69, scope: !22)
!30 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 74, type: !5, scopeLine: 74, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!31 = !DILocation(line: 74, scope: !30)
!32 = !DILocation(line: 77, scope: !30)
!33 = !DILocation(line: 81, scope: !30)
!34 = !DILocation(line: 84, scope: !30)
!35 = !DILocation(line: 92, scope: !30)
!36 = !DILocation(line: 88, scope: !30)
