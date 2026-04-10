; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/examples/tetris.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/examples/tetris.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Color = type { i8, i8, i8, i8 }
%struct.Game = type { i32*, i32*, %struct.Piece, %struct.Piece, i32, i32, i32, float, float, i1, i1 }
%struct.Piece = type { i32, i32, i32, i32 }
%struct.String = type { i8*, i32, i32 }

@KEY_LEFT = global i32 263
@KEY_RIGHT = global i32 262
@KEY_UP = global i32 265
@KEY_DOWN = global i32 264
@KEY_SPACE = global i32 32
@KEY_P = global i32 80
@KEY_R = global i32 82
@BOARD_COLS = global i32 10
@BOARD_ROWS = global i32 20
@CELL = global i32 30
@BOARD_X = global i32 20
@BOARD_Y = global i32 20
@WIN_W = global i32 530
@WIN_H = global i32 640
@PIECE_COUNT = global i32 7
@PI = constant i32 0
@PO = constant i32 1
@PT = constant i32 2
@PS = constant i32 3
@PZ = constant i32 4
@PJ = constant i32 5
@PL = constant i32 6
@STR0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@STR1 = private unnamed_addr constant [7 x i8] c"Tetris\00", align 1
@Game.board.dim0 = global i32 0
@Game.board.dim1 = global i32 0
@Game.shapes.dim0 = global i32 0
@Game.shapes.dim1 = global i32 0
@Game.shapes.dim2 = global i32 0
@STR2 = private unnamed_addr constant [6 x i8] c"SCORE\00", align 1
@STR3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@STR4 = private unnamed_addr constant [6 x i8] c"LEVEL\00", align 1
@STR5 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@STR6 = private unnamed_addr constant [6 x i8] c"LINES\00", align 1
@STR7 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@STR8 = private unnamed_addr constant [5 x i8] c"NEXT\00", align 1
@STR9 = private unnamed_addr constant [12 x i8] c"arrows move\00", align 1
@STR10 = private unnamed_addr constant [14 x i8] c"up/spc rotate\00", align 1
@STR11 = private unnamed_addr constant [15 x i8] c"down hard drop\00", align 1
@STR12 = private unnamed_addr constant [8 x i8] c"P pause\00", align 1
@STR13 = private unnamed_addr constant [10 x i8] c"GAME OVER\00", align 1
@STR14 = private unnamed_addr constant [10 x i8] c"Score: %d\00", align 1
@STR15 = private unnamed_addr constant [19 x i8] c"Press R to restart\00", align 1
@STR16 = private unnamed_addr constant [7 x i8] c"PAUSED\00", align 1

declare i8* @calloc(i64, i64)

declare i8* @realloc(i8*, i32)

declare void @free(i8*)

declare void @srand(i32)

declare i32 @atoi(i8*)

declare i32 @strlen(i8*)

declare i32 @strcmp(i8*, i8*)

declare i32 @strncmp(i8*, i8*, i32)

declare i8* @strcpy(i8*, i8*)

declare void @bzero(i8*, i64)

declare i64 @time(i8*)

declare void @InitWindow(i32, i32, i8*)

declare void @CloseWindow()

declare i1 @WindowShouldClose()

declare void @SetTargetFPS(i32)

declare float @GetFrameTime()

declare void @BeginDrawing()

declare void @EndDrawing()

declare void @ClearBackground(i64)

declare void @DrawRectangle(i32, i32, i32, i32, i64)

declare void @DrawRectangleLines(i32, i32, i32, i32, i64)

declare void @DrawLine(i32, i32, i32, i32, i64)

declare void @DrawText(i8*, i32, i32, i32, i64)

declare i32 @MeasureText(i8*, i32)

declare i1 @IsKeyPressed(i32)

declare i64 @ColorBrightness(i64, float)

declare i8* @TextFormat(i8*, i32, ...)

declare i32 @GetRandomValue(i32, i32)

define i8* @strjoin(i8* %left, i8* %right) !dbg !4 {
entry:
  %left1 = alloca i8*, align 8, !dbg !7
  %right2 = alloca i8*, align 8, !dbg !7
  %len1 = alloca i32, align 4, !dbg !7
  %len2 = alloca i32, align 4, !dbg !7
  %str = alloca i8*, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  %j = alloca i32, align 4, !dbg !7
  store i8* %left, i8** %left1, align 8, !dbg !7
  store i8* %right, i8** %right2, align 8, !dbg !7
  store i32 0, i32* %len1, align 4, !dbg !7
  store i32 0, i32* %len1, align 4, !dbg !7
  store i32 0, i32* %len2, align 4, !dbg !7
  store i32 0, i32* %len2, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %left3 = load i8*, i8** %left1, align 8, !dbg !7
  %NEQ = icmp ne i8* %left3, null, !dbg !7
  br i1 %NEQ, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start5, !dbg !8

if.then:                                          ; preds = %if.start
  %left4 = load i8*, i8** %left1, align 8, !dbg !8
  %strlen = call i32 @strlen(i8* %left4), !dbg !8
  store i32 %strlen, i32* %len1, align 4, !dbg !8
  br label %if.end, !dbg !8

if.start5:                                        ; preds = %if.end
  %right8 = load i8*, i8** %right2, align 8, !dbg !8
  %NEQ9 = icmp ne i8* %right8, null, !dbg !8
  br i1 %NEQ9, label %if.then7, label %if.end6, !dbg !8

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %str, align 8, !dbg !7
  %len112 = load i32, i32* %len1, align 4, !dbg !9
  %len213 = load i32, i32* %len2, align 4, !dbg !9
  %ADD = add i32 %len112, %len213, !dbg !9
  %ADD14 = add i32 %ADD, 1, !dbg !9
  %as = sext i32 %ADD14 to i64, !dbg !9
  %calloc = call i8* @calloc(i64 %as, i64 1), !dbg !9
  store i8* %calloc, i8** %str, align 8, !dbg !9
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

if.then7:                                         ; preds = %if.start5
  %right10 = load i8*, i8** %right2, align 8, !dbg !10
  %strlen11 = call i32 @strlen(i8* %right10), !dbg !10
  store i32 %strlen11, i32* %len2, align 4, !dbg !10
  br label %if.end6, !dbg !10

while.start:                                      ; preds = %while.then, %if.end6
  %i15 = load i32, i32* %i, align 4, !dbg !7
  %len116 = load i32, i32* %len1, align 4, !dbg !7
  %LT = icmp slt i32 %i15, %len116, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %str17 = load i8*, i8** %str, align 8, !dbg !11
  %i18 = load i32, i32* %i, align 4, !dbg !11
  %ACCESS = getelementptr i8, i8* %str17, i32 %i18, !dbg !11
  %left19 = load i8*, i8** %left1, align 8, !dbg !11
  %i20 = load i32, i32* %i, align 4, !dbg !11
  %ACCESS21 = getelementptr i8, i8* %left19, i32 %i20, !dbg !11
  %ACC = load i8, i8* %ACCESS21, align 1, !dbg !11
  store i8 %ACC, i8* %ACCESS, align 1, !dbg !11
  %i22 = load i32, i32* %i, align 4, !dbg !11
  %ADD23 = add i32 %i22, 1, !dbg !11
  store i32 %ADD23, i32* %i, align 4, !dbg !11
  br label %while.start, !dbg !11

while.end:                                        ; preds = %while.start
  store i32 0, i32* %j, align 4, !dbg !7
  store i32 0, i32* %j, align 4, !dbg !7
  br label %while.start24, !dbg !7

while.start24:                                    ; preds = %while.then25, %while.end
  %j27 = load i32, i32* %j, align 4, !dbg !7
  %len228 = load i32, i32* %len2, align 4, !dbg !7
  %LT29 = icmp slt i32 %j27, %len228, !dbg !7
  br i1 %LT29, label %while.then25, label %while.end26, !dbg !7

while.then25:                                     ; preds = %while.start24
  %i30 = load i32, i32* %i, align 4, !dbg !12
  %j31 = load i32, i32* %j, align 4, !dbg !12
  %ADD32 = add i32 %i30, %j31, !dbg !12
  %str33 = load i8*, i8** %str, align 8, !dbg !12
  %ACCESS34 = getelementptr i8, i8* %str33, i32 %ADD32, !dbg !12
  %right35 = load i8*, i8** %right2, align 8, !dbg !12
  %j36 = load i32, i32* %j, align 4, !dbg !12
  %ACCESS37 = getelementptr i8, i8* %right35, i32 %j36, !dbg !12
  %ACC38 = load i8, i8* %ACCESS37, align 1, !dbg !12
  store i8 %ACC38, i8* %ACCESS34, align 1, !dbg !12
  %j39 = load i32, i32* %j, align 4, !dbg !12
  %ADD40 = add i32 %j39, 1, !dbg !12
  store i32 %ADD40, i32* %j, align 4, !dbg !12
  br label %while.start24, !dbg !12

while.end26:                                      ; preds = %while.start24
  %str41 = load i8*, i8** %str, align 8, !dbg !12
  ret i8* %str41, !dbg !12
}

define %struct.Color @setColor(i32 %r, i32 %g, i32 %b, i32 %a) !dbg !13 {
entry:
  %r1 = alloca i32, align 4, !dbg !14
  %g2 = alloca i32, align 4, !dbg !14
  %b3 = alloca i32, align 4, !dbg !14
  %a4 = alloca i32, align 4, !dbg !14
  %c = alloca %struct.Color, align 8, !dbg !14
  store i32 %r, i32* %r1, align 4, !dbg !14
  store i32 %g, i32* %g2, align 4, !dbg !14
  store i32 %b, i32* %b3, align 4, !dbg !14
  store i32 %a, i32* %a4, align 4, !dbg !14
  store %struct.Color zeroinitializer, %struct.Color* %c, align 1, !dbg !14
  %r5 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 0, !dbg !14
  %r6 = load i32, i32* %r1, align 4, !dbg !14
  %as = trunc i32 %r6 to i8, !dbg !14
  store i8 %as, i8* %r5, align 1, !dbg !14
  %g7 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 1, !dbg !14
  %g8 = load i32, i32* %g2, align 4, !dbg !14
  %as9 = trunc i32 %g8 to i8, !dbg !14
  store i8 %as9, i8* %g7, align 1, !dbg !14
  %b10 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 2, !dbg !14
  %b11 = load i32, i32* %b3, align 4, !dbg !14
  %as12 = trunc i32 %b11 to i8, !dbg !14
  store i8 %as12, i8* %b10, align 1, !dbg !14
  %a13 = getelementptr %struct.Color, %struct.Color* %c, i32 0, i32 3, !dbg !14
  %a14 = load i32, i32* %a4, align 4, !dbg !14
  %as15 = trunc i32 %a14 to i8, !dbg !14
  store i8 %as15, i8* %a13, align 1, !dbg !14
  %c16 = load %struct.Color, %struct.Color* %c, align 1, !dbg !14
  ret %struct.Color %c16, !dbg !14
}

define void @init_shapes(%struct.Game* %gs) !dbg !15 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !16
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !16
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !17
  %shapes = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 1, !dbg !17
  %dim = load i32, i32* @Game.shapes.dim0, align 4, !dbg !17
  %dim3 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !17
  %dim4 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !17
  %DOT = load i32*, i32** %shapes, align 8, !dbg !17
  %PI = load i32, i32* @PI, align 4, !dbg !17
  %stride = mul i32 1, %dim3, !dbg !17
  %stride5 = mul i32 %stride, %dim4, !dbg !17
  %flat_idx = mul i32 %PI, %stride5, !dbg !17
  %row = getelementptr i32, i32* %DOT, i32 %flat_idx, !dbg !17
  %stride6 = mul i32 1, %dim4, !dbg !17
  %flat_idx7 = mul i32 0, %stride6, !dbg !17
  %row8 = getelementptr i32, i32* %row, i32 %flat_idx7, !dbg !17
  %ACCESS = getelementptr i32, i32* %row8, i32 0, !dbg !17
  store i32 0, i32* %ACCESS, align 4, !dbg !17
  %gs9 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !17
  %shapes10 = getelementptr %struct.Game, %struct.Game* %gs9, i32 0, i32 1, !dbg !17
  %dim11 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !17
  %dim12 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !17
  %dim13 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !17
  %DOT14 = load i32*, i32** %shapes10, align 8, !dbg !17
  %PI15 = load i32, i32* @PI, align 4, !dbg !17
  %stride16 = mul i32 1, %dim12, !dbg !17
  %stride17 = mul i32 %stride16, %dim13, !dbg !17
  %flat_idx18 = mul i32 %PI15, %stride17, !dbg !17
  %row19 = getelementptr i32, i32* %DOT14, i32 %flat_idx18, !dbg !17
  %stride20 = mul i32 1, %dim13, !dbg !17
  %flat_idx21 = mul i32 0, %stride20, !dbg !17
  %row22 = getelementptr i32, i32* %row19, i32 %flat_idx21, !dbg !17
  %ACCESS23 = getelementptr i32, i32* %row22, i32 1, !dbg !17
  store i32 15, i32* %ACCESS23, align 4, !dbg !17
  %gs24 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !17
  %shapes25 = getelementptr %struct.Game, %struct.Game* %gs24, i32 0, i32 1, !dbg !17
  %dim26 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !17
  %dim27 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !17
  %dim28 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !17
  %DOT29 = load i32*, i32** %shapes25, align 8, !dbg !17
  %PI30 = load i32, i32* @PI, align 4, !dbg !17
  %stride31 = mul i32 1, %dim27, !dbg !17
  %stride32 = mul i32 %stride31, %dim28, !dbg !17
  %flat_idx33 = mul i32 %PI30, %stride32, !dbg !17
  %row34 = getelementptr i32, i32* %DOT29, i32 %flat_idx33, !dbg !17
  %stride35 = mul i32 1, %dim28, !dbg !17
  %flat_idx36 = mul i32 0, %stride35, !dbg !17
  %row37 = getelementptr i32, i32* %row34, i32 %flat_idx36, !dbg !17
  %ACCESS38 = getelementptr i32, i32* %row37, i32 2, !dbg !17
  store i32 0, i32* %ACCESS38, align 4, !dbg !17
  %gs39 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !17
  %shapes40 = getelementptr %struct.Game, %struct.Game* %gs39, i32 0, i32 1, !dbg !17
  %dim41 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !17
  %dim42 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !17
  %dim43 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !17
  %DOT44 = load i32*, i32** %shapes40, align 8, !dbg !17
  %PI45 = load i32, i32* @PI, align 4, !dbg !17
  %stride46 = mul i32 1, %dim42, !dbg !17
  %stride47 = mul i32 %stride46, %dim43, !dbg !17
  %flat_idx48 = mul i32 %PI45, %stride47, !dbg !17
  %row49 = getelementptr i32, i32* %DOT44, i32 %flat_idx48, !dbg !17
  %stride50 = mul i32 1, %dim43, !dbg !17
  %flat_idx51 = mul i32 0, %stride50, !dbg !17
  %row52 = getelementptr i32, i32* %row49, i32 %flat_idx51, !dbg !17
  %ACCESS53 = getelementptr i32, i32* %row52, i32 3, !dbg !17
  store i32 0, i32* %ACCESS53, align 4, !dbg !17
  %gs54 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !18
  %shapes55 = getelementptr %struct.Game, %struct.Game* %gs54, i32 0, i32 1, !dbg !18
  %dim56 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !18
  %dim57 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !18
  %dim58 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !18
  %DOT59 = load i32*, i32** %shapes55, align 8, !dbg !18
  %PI60 = load i32, i32* @PI, align 4, !dbg !18
  %stride61 = mul i32 1, %dim57, !dbg !18
  %stride62 = mul i32 %stride61, %dim58, !dbg !18
  %flat_idx63 = mul i32 %PI60, %stride62, !dbg !18
  %row64 = getelementptr i32, i32* %DOT59, i32 %flat_idx63, !dbg !18
  %stride65 = mul i32 1, %dim58, !dbg !18
  %flat_idx66 = mul i32 1, %stride65, !dbg !18
  %row67 = getelementptr i32, i32* %row64, i32 %flat_idx66, !dbg !18
  %ACCESS68 = getelementptr i32, i32* %row67, i32 0, !dbg !18
  store i32 2, i32* %ACCESS68, align 4, !dbg !18
  %gs69 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !18
  %shapes70 = getelementptr %struct.Game, %struct.Game* %gs69, i32 0, i32 1, !dbg !18
  %dim71 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !18
  %dim72 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !18
  %dim73 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !18
  %DOT74 = load i32*, i32** %shapes70, align 8, !dbg !18
  %PI75 = load i32, i32* @PI, align 4, !dbg !18
  %stride76 = mul i32 1, %dim72, !dbg !18
  %stride77 = mul i32 %stride76, %dim73, !dbg !18
  %flat_idx78 = mul i32 %PI75, %stride77, !dbg !18
  %row79 = getelementptr i32, i32* %DOT74, i32 %flat_idx78, !dbg !18
  %stride80 = mul i32 1, %dim73, !dbg !18
  %flat_idx81 = mul i32 1, %stride80, !dbg !18
  %row82 = getelementptr i32, i32* %row79, i32 %flat_idx81, !dbg !18
  %ACCESS83 = getelementptr i32, i32* %row82, i32 1, !dbg !18
  store i32 2, i32* %ACCESS83, align 4, !dbg !18
  %gs84 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !18
  %shapes85 = getelementptr %struct.Game, %struct.Game* %gs84, i32 0, i32 1, !dbg !18
  %dim86 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !18
  %dim87 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !18
  %dim88 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !18
  %DOT89 = load i32*, i32** %shapes85, align 8, !dbg !18
  %PI90 = load i32, i32* @PI, align 4, !dbg !18
  %stride91 = mul i32 1, %dim87, !dbg !18
  %stride92 = mul i32 %stride91, %dim88, !dbg !18
  %flat_idx93 = mul i32 %PI90, %stride92, !dbg !18
  %row94 = getelementptr i32, i32* %DOT89, i32 %flat_idx93, !dbg !18
  %stride95 = mul i32 1, %dim88, !dbg !18
  %flat_idx96 = mul i32 1, %stride95, !dbg !18
  %row97 = getelementptr i32, i32* %row94, i32 %flat_idx96, !dbg !18
  %ACCESS98 = getelementptr i32, i32* %row97, i32 2, !dbg !18
  store i32 2, i32* %ACCESS98, align 4, !dbg !18
  %gs99 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !18
  %shapes100 = getelementptr %struct.Game, %struct.Game* %gs99, i32 0, i32 1, !dbg !18
  %dim101 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !18
  %dim102 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !18
  %dim103 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !18
  %DOT104 = load i32*, i32** %shapes100, align 8, !dbg !18
  %PI105 = load i32, i32* @PI, align 4, !dbg !18
  %stride106 = mul i32 1, %dim102, !dbg !18
  %stride107 = mul i32 %stride106, %dim103, !dbg !18
  %flat_idx108 = mul i32 %PI105, %stride107, !dbg !18
  %row109 = getelementptr i32, i32* %DOT104, i32 %flat_idx108, !dbg !18
  %stride110 = mul i32 1, %dim103, !dbg !18
  %flat_idx111 = mul i32 1, %stride110, !dbg !18
  %row112 = getelementptr i32, i32* %row109, i32 %flat_idx111, !dbg !18
  %ACCESS113 = getelementptr i32, i32* %row112, i32 3, !dbg !18
  store i32 2, i32* %ACCESS113, align 4, !dbg !18
  %gs114 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !19
  %shapes115 = getelementptr %struct.Game, %struct.Game* %gs114, i32 0, i32 1, !dbg !19
  %dim116 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !19
  %dim117 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !19
  %dim118 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !19
  %DOT119 = load i32*, i32** %shapes115, align 8, !dbg !19
  %PI120 = load i32, i32* @PI, align 4, !dbg !19
  %stride121 = mul i32 1, %dim117, !dbg !19
  %stride122 = mul i32 %stride121, %dim118, !dbg !19
  %flat_idx123 = mul i32 %PI120, %stride122, !dbg !19
  %row124 = getelementptr i32, i32* %DOT119, i32 %flat_idx123, !dbg !19
  %stride125 = mul i32 1, %dim118, !dbg !19
  %flat_idx126 = mul i32 2, %stride125, !dbg !19
  %row127 = getelementptr i32, i32* %row124, i32 %flat_idx126, !dbg !19
  %ACCESS128 = getelementptr i32, i32* %row127, i32 0, !dbg !19
  store i32 0, i32* %ACCESS128, align 4, !dbg !19
  %gs129 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !19
  %shapes130 = getelementptr %struct.Game, %struct.Game* %gs129, i32 0, i32 1, !dbg !19
  %dim131 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !19
  %dim132 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !19
  %dim133 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !19
  %DOT134 = load i32*, i32** %shapes130, align 8, !dbg !19
  %PI135 = load i32, i32* @PI, align 4, !dbg !19
  %stride136 = mul i32 1, %dim132, !dbg !19
  %stride137 = mul i32 %stride136, %dim133, !dbg !19
  %flat_idx138 = mul i32 %PI135, %stride137, !dbg !19
  %row139 = getelementptr i32, i32* %DOT134, i32 %flat_idx138, !dbg !19
  %stride140 = mul i32 1, %dim133, !dbg !19
  %flat_idx141 = mul i32 2, %stride140, !dbg !19
  %row142 = getelementptr i32, i32* %row139, i32 %flat_idx141, !dbg !19
  %ACCESS143 = getelementptr i32, i32* %row142, i32 1, !dbg !19
  store i32 15, i32* %ACCESS143, align 4, !dbg !19
  %gs144 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !19
  %shapes145 = getelementptr %struct.Game, %struct.Game* %gs144, i32 0, i32 1, !dbg !19
  %dim146 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !19
  %dim147 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !19
  %dim148 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !19
  %DOT149 = load i32*, i32** %shapes145, align 8, !dbg !19
  %PI150 = load i32, i32* @PI, align 4, !dbg !19
  %stride151 = mul i32 1, %dim147, !dbg !19
  %stride152 = mul i32 %stride151, %dim148, !dbg !19
  %flat_idx153 = mul i32 %PI150, %stride152, !dbg !19
  %row154 = getelementptr i32, i32* %DOT149, i32 %flat_idx153, !dbg !19
  %stride155 = mul i32 1, %dim148, !dbg !19
  %flat_idx156 = mul i32 2, %stride155, !dbg !19
  %row157 = getelementptr i32, i32* %row154, i32 %flat_idx156, !dbg !19
  %ACCESS158 = getelementptr i32, i32* %row157, i32 2, !dbg !19
  store i32 0, i32* %ACCESS158, align 4, !dbg !19
  %gs159 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !19
  %shapes160 = getelementptr %struct.Game, %struct.Game* %gs159, i32 0, i32 1, !dbg !19
  %dim161 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !19
  %dim162 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !19
  %dim163 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !19
  %DOT164 = load i32*, i32** %shapes160, align 8, !dbg !19
  %PI165 = load i32, i32* @PI, align 4, !dbg !19
  %stride166 = mul i32 1, %dim162, !dbg !19
  %stride167 = mul i32 %stride166, %dim163, !dbg !19
  %flat_idx168 = mul i32 %PI165, %stride167, !dbg !19
  %row169 = getelementptr i32, i32* %DOT164, i32 %flat_idx168, !dbg !19
  %stride170 = mul i32 1, %dim163, !dbg !19
  %flat_idx171 = mul i32 2, %stride170, !dbg !19
  %row172 = getelementptr i32, i32* %row169, i32 %flat_idx171, !dbg !19
  %ACCESS173 = getelementptr i32, i32* %row172, i32 3, !dbg !19
  store i32 0, i32* %ACCESS173, align 4, !dbg !19
  %gs174 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !20
  %shapes175 = getelementptr %struct.Game, %struct.Game* %gs174, i32 0, i32 1, !dbg !20
  %dim176 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !20
  %dim177 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !20
  %dim178 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !20
  %DOT179 = load i32*, i32** %shapes175, align 8, !dbg !20
  %PI180 = load i32, i32* @PI, align 4, !dbg !20
  %stride181 = mul i32 1, %dim177, !dbg !20
  %stride182 = mul i32 %stride181, %dim178, !dbg !20
  %flat_idx183 = mul i32 %PI180, %stride182, !dbg !20
  %row184 = getelementptr i32, i32* %DOT179, i32 %flat_idx183, !dbg !20
  %stride185 = mul i32 1, %dim178, !dbg !20
  %flat_idx186 = mul i32 3, %stride185, !dbg !20
  %row187 = getelementptr i32, i32* %row184, i32 %flat_idx186, !dbg !20
  %ACCESS188 = getelementptr i32, i32* %row187, i32 0, !dbg !20
  store i32 2, i32* %ACCESS188, align 4, !dbg !20
  %gs189 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !20
  %shapes190 = getelementptr %struct.Game, %struct.Game* %gs189, i32 0, i32 1, !dbg !20
  %dim191 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !20
  %dim192 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !20
  %dim193 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !20
  %DOT194 = load i32*, i32** %shapes190, align 8, !dbg !20
  %PI195 = load i32, i32* @PI, align 4, !dbg !20
  %stride196 = mul i32 1, %dim192, !dbg !20
  %stride197 = mul i32 %stride196, %dim193, !dbg !20
  %flat_idx198 = mul i32 %PI195, %stride197, !dbg !20
  %row199 = getelementptr i32, i32* %DOT194, i32 %flat_idx198, !dbg !20
  %stride200 = mul i32 1, %dim193, !dbg !20
  %flat_idx201 = mul i32 3, %stride200, !dbg !20
  %row202 = getelementptr i32, i32* %row199, i32 %flat_idx201, !dbg !20
  %ACCESS203 = getelementptr i32, i32* %row202, i32 1, !dbg !20
  store i32 2, i32* %ACCESS203, align 4, !dbg !20
  %gs204 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !20
  %shapes205 = getelementptr %struct.Game, %struct.Game* %gs204, i32 0, i32 1, !dbg !20
  %dim206 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !20
  %dim207 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !20
  %dim208 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !20
  %DOT209 = load i32*, i32** %shapes205, align 8, !dbg !20
  %PI210 = load i32, i32* @PI, align 4, !dbg !20
  %stride211 = mul i32 1, %dim207, !dbg !20
  %stride212 = mul i32 %stride211, %dim208, !dbg !20
  %flat_idx213 = mul i32 %PI210, %stride212, !dbg !20
  %row214 = getelementptr i32, i32* %DOT209, i32 %flat_idx213, !dbg !20
  %stride215 = mul i32 1, %dim208, !dbg !20
  %flat_idx216 = mul i32 3, %stride215, !dbg !20
  %row217 = getelementptr i32, i32* %row214, i32 %flat_idx216, !dbg !20
  %ACCESS218 = getelementptr i32, i32* %row217, i32 2, !dbg !20
  store i32 2, i32* %ACCESS218, align 4, !dbg !20
  %gs219 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !20
  %shapes220 = getelementptr %struct.Game, %struct.Game* %gs219, i32 0, i32 1, !dbg !20
  %dim221 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !20
  %dim222 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !20
  %dim223 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !20
  %DOT224 = load i32*, i32** %shapes220, align 8, !dbg !20
  %PI225 = load i32, i32* @PI, align 4, !dbg !20
  %stride226 = mul i32 1, %dim222, !dbg !20
  %stride227 = mul i32 %stride226, %dim223, !dbg !20
  %flat_idx228 = mul i32 %PI225, %stride227, !dbg !20
  %row229 = getelementptr i32, i32* %DOT224, i32 %flat_idx228, !dbg !20
  %stride230 = mul i32 1, %dim223, !dbg !20
  %flat_idx231 = mul i32 3, %stride230, !dbg !20
  %row232 = getelementptr i32, i32* %row229, i32 %flat_idx231, !dbg !20
  %ACCESS233 = getelementptr i32, i32* %row232, i32 3, !dbg !20
  store i32 2, i32* %ACCESS233, align 4, !dbg !20
  %gs234 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !21
  %shapes235 = getelementptr %struct.Game, %struct.Game* %gs234, i32 0, i32 1, !dbg !21
  %dim236 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !21
  %dim237 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !21
  %dim238 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !21
  %DOT239 = load i32*, i32** %shapes235, align 8, !dbg !21
  %PO = load i32, i32* @PO, align 4, !dbg !21
  %stride240 = mul i32 1, %dim237, !dbg !21
  %stride241 = mul i32 %stride240, %dim238, !dbg !21
  %flat_idx242 = mul i32 %PO, %stride241, !dbg !21
  %row243 = getelementptr i32, i32* %DOT239, i32 %flat_idx242, !dbg !21
  %stride244 = mul i32 1, %dim238, !dbg !21
  %flat_idx245 = mul i32 0, %stride244, !dbg !21
  %row246 = getelementptr i32, i32* %row243, i32 %flat_idx245, !dbg !21
  %ACCESS247 = getelementptr i32, i32* %row246, i32 0, !dbg !21
  store i32 6, i32* %ACCESS247, align 4, !dbg !21
  %gs248 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !21
  %shapes249 = getelementptr %struct.Game, %struct.Game* %gs248, i32 0, i32 1, !dbg !21
  %dim250 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !21
  %dim251 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !21
  %dim252 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !21
  %DOT253 = load i32*, i32** %shapes249, align 8, !dbg !21
  %PO254 = load i32, i32* @PO, align 4, !dbg !21
  %stride255 = mul i32 1, %dim251, !dbg !21
  %stride256 = mul i32 %stride255, %dim252, !dbg !21
  %flat_idx257 = mul i32 %PO254, %stride256, !dbg !21
  %row258 = getelementptr i32, i32* %DOT253, i32 %flat_idx257, !dbg !21
  %stride259 = mul i32 1, %dim252, !dbg !21
  %flat_idx260 = mul i32 0, %stride259, !dbg !21
  %row261 = getelementptr i32, i32* %row258, i32 %flat_idx260, !dbg !21
  %ACCESS262 = getelementptr i32, i32* %row261, i32 1, !dbg !21
  store i32 6, i32* %ACCESS262, align 4, !dbg !21
  %gs263 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !21
  %shapes264 = getelementptr %struct.Game, %struct.Game* %gs263, i32 0, i32 1, !dbg !21
  %dim265 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !21
  %dim266 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !21
  %dim267 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !21
  %DOT268 = load i32*, i32** %shapes264, align 8, !dbg !21
  %PO269 = load i32, i32* @PO, align 4, !dbg !21
  %stride270 = mul i32 1, %dim266, !dbg !21
  %stride271 = mul i32 %stride270, %dim267, !dbg !21
  %flat_idx272 = mul i32 %PO269, %stride271, !dbg !21
  %row273 = getelementptr i32, i32* %DOT268, i32 %flat_idx272, !dbg !21
  %stride274 = mul i32 1, %dim267, !dbg !21
  %flat_idx275 = mul i32 0, %stride274, !dbg !21
  %row276 = getelementptr i32, i32* %row273, i32 %flat_idx275, !dbg !21
  %ACCESS277 = getelementptr i32, i32* %row276, i32 2, !dbg !21
  store i32 0, i32* %ACCESS277, align 4, !dbg !21
  %gs278 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !21
  %shapes279 = getelementptr %struct.Game, %struct.Game* %gs278, i32 0, i32 1, !dbg !21
  %dim280 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !21
  %dim281 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !21
  %dim282 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !21
  %DOT283 = load i32*, i32** %shapes279, align 8, !dbg !21
  %PO284 = load i32, i32* @PO, align 4, !dbg !21
  %stride285 = mul i32 1, %dim281, !dbg !21
  %stride286 = mul i32 %stride285, %dim282, !dbg !21
  %flat_idx287 = mul i32 %PO284, %stride286, !dbg !21
  %row288 = getelementptr i32, i32* %DOT283, i32 %flat_idx287, !dbg !21
  %stride289 = mul i32 1, %dim282, !dbg !21
  %flat_idx290 = mul i32 0, %stride289, !dbg !21
  %row291 = getelementptr i32, i32* %row288, i32 %flat_idx290, !dbg !21
  %ACCESS292 = getelementptr i32, i32* %row291, i32 3, !dbg !21
  store i32 0, i32* %ACCESS292, align 4, !dbg !21
  %gs293 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !22
  %shapes294 = getelementptr %struct.Game, %struct.Game* %gs293, i32 0, i32 1, !dbg !22
  %dim295 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !22
  %dim296 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !22
  %dim297 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !22
  %DOT298 = load i32*, i32** %shapes294, align 8, !dbg !22
  %PO299 = load i32, i32* @PO, align 4, !dbg !22
  %stride300 = mul i32 1, %dim296, !dbg !22
  %stride301 = mul i32 %stride300, %dim297, !dbg !22
  %flat_idx302 = mul i32 %PO299, %stride301, !dbg !22
  %row303 = getelementptr i32, i32* %DOT298, i32 %flat_idx302, !dbg !22
  %stride304 = mul i32 1, %dim297, !dbg !22
  %flat_idx305 = mul i32 1, %stride304, !dbg !22
  %row306 = getelementptr i32, i32* %row303, i32 %flat_idx305, !dbg !22
  %ACCESS307 = getelementptr i32, i32* %row306, i32 0, !dbg !22
  store i32 6, i32* %ACCESS307, align 4, !dbg !22
  %gs308 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !22
  %shapes309 = getelementptr %struct.Game, %struct.Game* %gs308, i32 0, i32 1, !dbg !22
  %dim310 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !22
  %dim311 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !22
  %dim312 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !22
  %DOT313 = load i32*, i32** %shapes309, align 8, !dbg !22
  %PO314 = load i32, i32* @PO, align 4, !dbg !22
  %stride315 = mul i32 1, %dim311, !dbg !22
  %stride316 = mul i32 %stride315, %dim312, !dbg !22
  %flat_idx317 = mul i32 %PO314, %stride316, !dbg !22
  %row318 = getelementptr i32, i32* %DOT313, i32 %flat_idx317, !dbg !22
  %stride319 = mul i32 1, %dim312, !dbg !22
  %flat_idx320 = mul i32 1, %stride319, !dbg !22
  %row321 = getelementptr i32, i32* %row318, i32 %flat_idx320, !dbg !22
  %ACCESS322 = getelementptr i32, i32* %row321, i32 1, !dbg !22
  store i32 6, i32* %ACCESS322, align 4, !dbg !22
  %gs323 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !22
  %shapes324 = getelementptr %struct.Game, %struct.Game* %gs323, i32 0, i32 1, !dbg !22
  %dim325 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !22
  %dim326 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !22
  %dim327 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !22
  %DOT328 = load i32*, i32** %shapes324, align 8, !dbg !22
  %PO329 = load i32, i32* @PO, align 4, !dbg !22
  %stride330 = mul i32 1, %dim326, !dbg !22
  %stride331 = mul i32 %stride330, %dim327, !dbg !22
  %flat_idx332 = mul i32 %PO329, %stride331, !dbg !22
  %row333 = getelementptr i32, i32* %DOT328, i32 %flat_idx332, !dbg !22
  %stride334 = mul i32 1, %dim327, !dbg !22
  %flat_idx335 = mul i32 1, %stride334, !dbg !22
  %row336 = getelementptr i32, i32* %row333, i32 %flat_idx335, !dbg !22
  %ACCESS337 = getelementptr i32, i32* %row336, i32 2, !dbg !22
  store i32 0, i32* %ACCESS337, align 4, !dbg !22
  %gs338 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !22
  %shapes339 = getelementptr %struct.Game, %struct.Game* %gs338, i32 0, i32 1, !dbg !22
  %dim340 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !22
  %dim341 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !22
  %dim342 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !22
  %DOT343 = load i32*, i32** %shapes339, align 8, !dbg !22
  %PO344 = load i32, i32* @PO, align 4, !dbg !22
  %stride345 = mul i32 1, %dim341, !dbg !22
  %stride346 = mul i32 %stride345, %dim342, !dbg !22
  %flat_idx347 = mul i32 %PO344, %stride346, !dbg !22
  %row348 = getelementptr i32, i32* %DOT343, i32 %flat_idx347, !dbg !22
  %stride349 = mul i32 1, %dim342, !dbg !22
  %flat_idx350 = mul i32 1, %stride349, !dbg !22
  %row351 = getelementptr i32, i32* %row348, i32 %flat_idx350, !dbg !22
  %ACCESS352 = getelementptr i32, i32* %row351, i32 3, !dbg !22
  store i32 0, i32* %ACCESS352, align 4, !dbg !22
  %gs353 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !23
  %shapes354 = getelementptr %struct.Game, %struct.Game* %gs353, i32 0, i32 1, !dbg !23
  %dim355 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !23
  %dim356 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !23
  %dim357 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !23
  %DOT358 = load i32*, i32** %shapes354, align 8, !dbg !23
  %PO359 = load i32, i32* @PO, align 4, !dbg !23
  %stride360 = mul i32 1, %dim356, !dbg !23
  %stride361 = mul i32 %stride360, %dim357, !dbg !23
  %flat_idx362 = mul i32 %PO359, %stride361, !dbg !23
  %row363 = getelementptr i32, i32* %DOT358, i32 %flat_idx362, !dbg !23
  %stride364 = mul i32 1, %dim357, !dbg !23
  %flat_idx365 = mul i32 2, %stride364, !dbg !23
  %row366 = getelementptr i32, i32* %row363, i32 %flat_idx365, !dbg !23
  %ACCESS367 = getelementptr i32, i32* %row366, i32 0, !dbg !23
  store i32 6, i32* %ACCESS367, align 4, !dbg !23
  %gs368 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !23
  %shapes369 = getelementptr %struct.Game, %struct.Game* %gs368, i32 0, i32 1, !dbg !23
  %dim370 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !23
  %dim371 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !23
  %dim372 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !23
  %DOT373 = load i32*, i32** %shapes369, align 8, !dbg !23
  %PO374 = load i32, i32* @PO, align 4, !dbg !23
  %stride375 = mul i32 1, %dim371, !dbg !23
  %stride376 = mul i32 %stride375, %dim372, !dbg !23
  %flat_idx377 = mul i32 %PO374, %stride376, !dbg !23
  %row378 = getelementptr i32, i32* %DOT373, i32 %flat_idx377, !dbg !23
  %stride379 = mul i32 1, %dim372, !dbg !23
  %flat_idx380 = mul i32 2, %stride379, !dbg !23
  %row381 = getelementptr i32, i32* %row378, i32 %flat_idx380, !dbg !23
  %ACCESS382 = getelementptr i32, i32* %row381, i32 1, !dbg !23
  store i32 6, i32* %ACCESS382, align 4, !dbg !23
  %gs383 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !23
  %shapes384 = getelementptr %struct.Game, %struct.Game* %gs383, i32 0, i32 1, !dbg !23
  %dim385 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !23
  %dim386 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !23
  %dim387 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !23
  %DOT388 = load i32*, i32** %shapes384, align 8, !dbg !23
  %PO389 = load i32, i32* @PO, align 4, !dbg !23
  %stride390 = mul i32 1, %dim386, !dbg !23
  %stride391 = mul i32 %stride390, %dim387, !dbg !23
  %flat_idx392 = mul i32 %PO389, %stride391, !dbg !23
  %row393 = getelementptr i32, i32* %DOT388, i32 %flat_idx392, !dbg !23
  %stride394 = mul i32 1, %dim387, !dbg !23
  %flat_idx395 = mul i32 2, %stride394, !dbg !23
  %row396 = getelementptr i32, i32* %row393, i32 %flat_idx395, !dbg !23
  %ACCESS397 = getelementptr i32, i32* %row396, i32 2, !dbg !23
  store i32 0, i32* %ACCESS397, align 4, !dbg !23
  %gs398 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !23
  %shapes399 = getelementptr %struct.Game, %struct.Game* %gs398, i32 0, i32 1, !dbg !23
  %dim400 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !23
  %dim401 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !23
  %dim402 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !23
  %DOT403 = load i32*, i32** %shapes399, align 8, !dbg !23
  %PO404 = load i32, i32* @PO, align 4, !dbg !23
  %stride405 = mul i32 1, %dim401, !dbg !23
  %stride406 = mul i32 %stride405, %dim402, !dbg !23
  %flat_idx407 = mul i32 %PO404, %stride406, !dbg !23
  %row408 = getelementptr i32, i32* %DOT403, i32 %flat_idx407, !dbg !23
  %stride409 = mul i32 1, %dim402, !dbg !23
  %flat_idx410 = mul i32 2, %stride409, !dbg !23
  %row411 = getelementptr i32, i32* %row408, i32 %flat_idx410, !dbg !23
  %ACCESS412 = getelementptr i32, i32* %row411, i32 3, !dbg !23
  store i32 0, i32* %ACCESS412, align 4, !dbg !23
  %gs413 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !24
  %shapes414 = getelementptr %struct.Game, %struct.Game* %gs413, i32 0, i32 1, !dbg !24
  %dim415 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !24
  %dim416 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !24
  %dim417 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !24
  %DOT418 = load i32*, i32** %shapes414, align 8, !dbg !24
  %PO419 = load i32, i32* @PO, align 4, !dbg !24
  %stride420 = mul i32 1, %dim416, !dbg !24
  %stride421 = mul i32 %stride420, %dim417, !dbg !24
  %flat_idx422 = mul i32 %PO419, %stride421, !dbg !24
  %row423 = getelementptr i32, i32* %DOT418, i32 %flat_idx422, !dbg !24
  %stride424 = mul i32 1, %dim417, !dbg !24
  %flat_idx425 = mul i32 3, %stride424, !dbg !24
  %row426 = getelementptr i32, i32* %row423, i32 %flat_idx425, !dbg !24
  %ACCESS427 = getelementptr i32, i32* %row426, i32 0, !dbg !24
  store i32 6, i32* %ACCESS427, align 4, !dbg !24
  %gs428 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !24
  %shapes429 = getelementptr %struct.Game, %struct.Game* %gs428, i32 0, i32 1, !dbg !24
  %dim430 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !24
  %dim431 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !24
  %dim432 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !24
  %DOT433 = load i32*, i32** %shapes429, align 8, !dbg !24
  %PO434 = load i32, i32* @PO, align 4, !dbg !24
  %stride435 = mul i32 1, %dim431, !dbg !24
  %stride436 = mul i32 %stride435, %dim432, !dbg !24
  %flat_idx437 = mul i32 %PO434, %stride436, !dbg !24
  %row438 = getelementptr i32, i32* %DOT433, i32 %flat_idx437, !dbg !24
  %stride439 = mul i32 1, %dim432, !dbg !24
  %flat_idx440 = mul i32 3, %stride439, !dbg !24
  %row441 = getelementptr i32, i32* %row438, i32 %flat_idx440, !dbg !24
  %ACCESS442 = getelementptr i32, i32* %row441, i32 1, !dbg !24
  store i32 6, i32* %ACCESS442, align 4, !dbg !24
  %gs443 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !24
  %shapes444 = getelementptr %struct.Game, %struct.Game* %gs443, i32 0, i32 1, !dbg !24
  %dim445 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !24
  %dim446 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !24
  %dim447 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !24
  %DOT448 = load i32*, i32** %shapes444, align 8, !dbg !24
  %PO449 = load i32, i32* @PO, align 4, !dbg !24
  %stride450 = mul i32 1, %dim446, !dbg !24
  %stride451 = mul i32 %stride450, %dim447, !dbg !24
  %flat_idx452 = mul i32 %PO449, %stride451, !dbg !24
  %row453 = getelementptr i32, i32* %DOT448, i32 %flat_idx452, !dbg !24
  %stride454 = mul i32 1, %dim447, !dbg !24
  %flat_idx455 = mul i32 3, %stride454, !dbg !24
  %row456 = getelementptr i32, i32* %row453, i32 %flat_idx455, !dbg !24
  %ACCESS457 = getelementptr i32, i32* %row456, i32 2, !dbg !24
  store i32 0, i32* %ACCESS457, align 4, !dbg !24
  %gs458 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !24
  %shapes459 = getelementptr %struct.Game, %struct.Game* %gs458, i32 0, i32 1, !dbg !24
  %dim460 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !24
  %dim461 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !24
  %dim462 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !24
  %DOT463 = load i32*, i32** %shapes459, align 8, !dbg !24
  %PO464 = load i32, i32* @PO, align 4, !dbg !24
  %stride465 = mul i32 1, %dim461, !dbg !24
  %stride466 = mul i32 %stride465, %dim462, !dbg !24
  %flat_idx467 = mul i32 %PO464, %stride466, !dbg !24
  %row468 = getelementptr i32, i32* %DOT463, i32 %flat_idx467, !dbg !24
  %stride469 = mul i32 1, %dim462, !dbg !24
  %flat_idx470 = mul i32 3, %stride469, !dbg !24
  %row471 = getelementptr i32, i32* %row468, i32 %flat_idx470, !dbg !24
  %ACCESS472 = getelementptr i32, i32* %row471, i32 3, !dbg !24
  store i32 0, i32* %ACCESS472, align 4, !dbg !24
  %gs473 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !25
  %shapes474 = getelementptr %struct.Game, %struct.Game* %gs473, i32 0, i32 1, !dbg !25
  %dim475 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !25
  %dim476 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !25
  %dim477 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !25
  %DOT478 = load i32*, i32** %shapes474, align 8, !dbg !25
  %PT = load i32, i32* @PT, align 4, !dbg !25
  %stride479 = mul i32 1, %dim476, !dbg !25
  %stride480 = mul i32 %stride479, %dim477, !dbg !25
  %flat_idx481 = mul i32 %PT, %stride480, !dbg !25
  %row482 = getelementptr i32, i32* %DOT478, i32 %flat_idx481, !dbg !25
  %stride483 = mul i32 1, %dim477, !dbg !25
  %flat_idx484 = mul i32 0, %stride483, !dbg !25
  %row485 = getelementptr i32, i32* %row482, i32 %flat_idx484, !dbg !25
  %ACCESS486 = getelementptr i32, i32* %row485, i32 0, !dbg !25
  store i32 4, i32* %ACCESS486, align 4, !dbg !25
  %gs487 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !25
  %shapes488 = getelementptr %struct.Game, %struct.Game* %gs487, i32 0, i32 1, !dbg !25
  %dim489 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !25
  %dim490 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !25
  %dim491 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !25
  %DOT492 = load i32*, i32** %shapes488, align 8, !dbg !25
  %PT493 = load i32, i32* @PT, align 4, !dbg !25
  %stride494 = mul i32 1, %dim490, !dbg !25
  %stride495 = mul i32 %stride494, %dim491, !dbg !25
  %flat_idx496 = mul i32 %PT493, %stride495, !dbg !25
  %row497 = getelementptr i32, i32* %DOT492, i32 %flat_idx496, !dbg !25
  %stride498 = mul i32 1, %dim491, !dbg !25
  %flat_idx499 = mul i32 0, %stride498, !dbg !25
  %row500 = getelementptr i32, i32* %row497, i32 %flat_idx499, !dbg !25
  %ACCESS501 = getelementptr i32, i32* %row500, i32 1, !dbg !25
  store i32 14, i32* %ACCESS501, align 4, !dbg !25
  %gs502 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !25
  %shapes503 = getelementptr %struct.Game, %struct.Game* %gs502, i32 0, i32 1, !dbg !25
  %dim504 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !25
  %dim505 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !25
  %dim506 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !25
  %DOT507 = load i32*, i32** %shapes503, align 8, !dbg !25
  %PT508 = load i32, i32* @PT, align 4, !dbg !25
  %stride509 = mul i32 1, %dim505, !dbg !25
  %stride510 = mul i32 %stride509, %dim506, !dbg !25
  %flat_idx511 = mul i32 %PT508, %stride510, !dbg !25
  %row512 = getelementptr i32, i32* %DOT507, i32 %flat_idx511, !dbg !25
  %stride513 = mul i32 1, %dim506, !dbg !25
  %flat_idx514 = mul i32 0, %stride513, !dbg !25
  %row515 = getelementptr i32, i32* %row512, i32 %flat_idx514, !dbg !25
  %ACCESS516 = getelementptr i32, i32* %row515, i32 2, !dbg !25
  store i32 0, i32* %ACCESS516, align 4, !dbg !25
  %gs517 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !25
  %shapes518 = getelementptr %struct.Game, %struct.Game* %gs517, i32 0, i32 1, !dbg !25
  %dim519 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !25
  %dim520 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !25
  %dim521 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !25
  %DOT522 = load i32*, i32** %shapes518, align 8, !dbg !25
  %PT523 = load i32, i32* @PT, align 4, !dbg !25
  %stride524 = mul i32 1, %dim520, !dbg !25
  %stride525 = mul i32 %stride524, %dim521, !dbg !25
  %flat_idx526 = mul i32 %PT523, %stride525, !dbg !25
  %row527 = getelementptr i32, i32* %DOT522, i32 %flat_idx526, !dbg !25
  %stride528 = mul i32 1, %dim521, !dbg !25
  %flat_idx529 = mul i32 0, %stride528, !dbg !25
  %row530 = getelementptr i32, i32* %row527, i32 %flat_idx529, !dbg !25
  %ACCESS531 = getelementptr i32, i32* %row530, i32 3, !dbg !25
  store i32 0, i32* %ACCESS531, align 4, !dbg !25
  %gs532 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !26
  %shapes533 = getelementptr %struct.Game, %struct.Game* %gs532, i32 0, i32 1, !dbg !26
  %dim534 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !26
  %dim535 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !26
  %dim536 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !26
  %DOT537 = load i32*, i32** %shapes533, align 8, !dbg !26
  %PT538 = load i32, i32* @PT, align 4, !dbg !26
  %stride539 = mul i32 1, %dim535, !dbg !26
  %stride540 = mul i32 %stride539, %dim536, !dbg !26
  %flat_idx541 = mul i32 %PT538, %stride540, !dbg !26
  %row542 = getelementptr i32, i32* %DOT537, i32 %flat_idx541, !dbg !26
  %stride543 = mul i32 1, %dim536, !dbg !26
  %flat_idx544 = mul i32 1, %stride543, !dbg !26
  %row545 = getelementptr i32, i32* %row542, i32 %flat_idx544, !dbg !26
  %ACCESS546 = getelementptr i32, i32* %row545, i32 0, !dbg !26
  store i32 4, i32* %ACCESS546, align 4, !dbg !26
  %gs547 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !26
  %shapes548 = getelementptr %struct.Game, %struct.Game* %gs547, i32 0, i32 1, !dbg !26
  %dim549 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !26
  %dim550 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !26
  %dim551 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !26
  %DOT552 = load i32*, i32** %shapes548, align 8, !dbg !26
  %PT553 = load i32, i32* @PT, align 4, !dbg !26
  %stride554 = mul i32 1, %dim550, !dbg !26
  %stride555 = mul i32 %stride554, %dim551, !dbg !26
  %flat_idx556 = mul i32 %PT553, %stride555, !dbg !26
  %row557 = getelementptr i32, i32* %DOT552, i32 %flat_idx556, !dbg !26
  %stride558 = mul i32 1, %dim551, !dbg !26
  %flat_idx559 = mul i32 1, %stride558, !dbg !26
  %row560 = getelementptr i32, i32* %row557, i32 %flat_idx559, !dbg !26
  %ACCESS561 = getelementptr i32, i32* %row560, i32 1, !dbg !26
  store i32 6, i32* %ACCESS561, align 4, !dbg !26
  %gs562 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !26
  %shapes563 = getelementptr %struct.Game, %struct.Game* %gs562, i32 0, i32 1, !dbg !26
  %dim564 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !26
  %dim565 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !26
  %dim566 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !26
  %DOT567 = load i32*, i32** %shapes563, align 8, !dbg !26
  %PT568 = load i32, i32* @PT, align 4, !dbg !26
  %stride569 = mul i32 1, %dim565, !dbg !26
  %stride570 = mul i32 %stride569, %dim566, !dbg !26
  %flat_idx571 = mul i32 %PT568, %stride570, !dbg !26
  %row572 = getelementptr i32, i32* %DOT567, i32 %flat_idx571, !dbg !26
  %stride573 = mul i32 1, %dim566, !dbg !26
  %flat_idx574 = mul i32 1, %stride573, !dbg !26
  %row575 = getelementptr i32, i32* %row572, i32 %flat_idx574, !dbg !26
  %ACCESS576 = getelementptr i32, i32* %row575, i32 2, !dbg !26
  store i32 4, i32* %ACCESS576, align 4, !dbg !26
  %gs577 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !26
  %shapes578 = getelementptr %struct.Game, %struct.Game* %gs577, i32 0, i32 1, !dbg !26
  %dim579 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !26
  %dim580 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !26
  %dim581 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !26
  %DOT582 = load i32*, i32** %shapes578, align 8, !dbg !26
  %PT583 = load i32, i32* @PT, align 4, !dbg !26
  %stride584 = mul i32 1, %dim580, !dbg !26
  %stride585 = mul i32 %stride584, %dim581, !dbg !26
  %flat_idx586 = mul i32 %PT583, %stride585, !dbg !26
  %row587 = getelementptr i32, i32* %DOT582, i32 %flat_idx586, !dbg !26
  %stride588 = mul i32 1, %dim581, !dbg !26
  %flat_idx589 = mul i32 1, %stride588, !dbg !26
  %row590 = getelementptr i32, i32* %row587, i32 %flat_idx589, !dbg !26
  %ACCESS591 = getelementptr i32, i32* %row590, i32 3, !dbg !26
  store i32 0, i32* %ACCESS591, align 4, !dbg !26
  %gs592 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !27
  %shapes593 = getelementptr %struct.Game, %struct.Game* %gs592, i32 0, i32 1, !dbg !27
  %dim594 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !27
  %dim595 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !27
  %dim596 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !27
  %DOT597 = load i32*, i32** %shapes593, align 8, !dbg !27
  %PT598 = load i32, i32* @PT, align 4, !dbg !27
  %stride599 = mul i32 1, %dim595, !dbg !27
  %stride600 = mul i32 %stride599, %dim596, !dbg !27
  %flat_idx601 = mul i32 %PT598, %stride600, !dbg !27
  %row602 = getelementptr i32, i32* %DOT597, i32 %flat_idx601, !dbg !27
  %stride603 = mul i32 1, %dim596, !dbg !27
  %flat_idx604 = mul i32 2, %stride603, !dbg !27
  %row605 = getelementptr i32, i32* %row602, i32 %flat_idx604, !dbg !27
  %ACCESS606 = getelementptr i32, i32* %row605, i32 0, !dbg !27
  store i32 0, i32* %ACCESS606, align 4, !dbg !27
  %gs607 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !27
  %shapes608 = getelementptr %struct.Game, %struct.Game* %gs607, i32 0, i32 1, !dbg !27
  %dim609 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !27
  %dim610 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !27
  %dim611 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !27
  %DOT612 = load i32*, i32** %shapes608, align 8, !dbg !27
  %PT613 = load i32, i32* @PT, align 4, !dbg !27
  %stride614 = mul i32 1, %dim610, !dbg !27
  %stride615 = mul i32 %stride614, %dim611, !dbg !27
  %flat_idx616 = mul i32 %PT613, %stride615, !dbg !27
  %row617 = getelementptr i32, i32* %DOT612, i32 %flat_idx616, !dbg !27
  %stride618 = mul i32 1, %dim611, !dbg !27
  %flat_idx619 = mul i32 2, %stride618, !dbg !27
  %row620 = getelementptr i32, i32* %row617, i32 %flat_idx619, !dbg !27
  %ACCESS621 = getelementptr i32, i32* %row620, i32 1, !dbg !27
  store i32 14, i32* %ACCESS621, align 4, !dbg !27
  %gs622 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !27
  %shapes623 = getelementptr %struct.Game, %struct.Game* %gs622, i32 0, i32 1, !dbg !27
  %dim624 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !27
  %dim625 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !27
  %dim626 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !27
  %DOT627 = load i32*, i32** %shapes623, align 8, !dbg !27
  %PT628 = load i32, i32* @PT, align 4, !dbg !27
  %stride629 = mul i32 1, %dim625, !dbg !27
  %stride630 = mul i32 %stride629, %dim626, !dbg !27
  %flat_idx631 = mul i32 %PT628, %stride630, !dbg !27
  %row632 = getelementptr i32, i32* %DOT627, i32 %flat_idx631, !dbg !27
  %stride633 = mul i32 1, %dim626, !dbg !27
  %flat_idx634 = mul i32 2, %stride633, !dbg !27
  %row635 = getelementptr i32, i32* %row632, i32 %flat_idx634, !dbg !27
  %ACCESS636 = getelementptr i32, i32* %row635, i32 2, !dbg !27
  store i32 4, i32* %ACCESS636, align 4, !dbg !27
  %gs637 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !27
  %shapes638 = getelementptr %struct.Game, %struct.Game* %gs637, i32 0, i32 1, !dbg !27
  %dim639 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !27
  %dim640 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !27
  %dim641 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !27
  %DOT642 = load i32*, i32** %shapes638, align 8, !dbg !27
  %PT643 = load i32, i32* @PT, align 4, !dbg !27
  %stride644 = mul i32 1, %dim640, !dbg !27
  %stride645 = mul i32 %stride644, %dim641, !dbg !27
  %flat_idx646 = mul i32 %PT643, %stride645, !dbg !27
  %row647 = getelementptr i32, i32* %DOT642, i32 %flat_idx646, !dbg !27
  %stride648 = mul i32 1, %dim641, !dbg !27
  %flat_idx649 = mul i32 2, %stride648, !dbg !27
  %row650 = getelementptr i32, i32* %row647, i32 %flat_idx649, !dbg !27
  %ACCESS651 = getelementptr i32, i32* %row650, i32 3, !dbg !27
  store i32 0, i32* %ACCESS651, align 4, !dbg !27
  %gs652 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !28
  %shapes653 = getelementptr %struct.Game, %struct.Game* %gs652, i32 0, i32 1, !dbg !28
  %dim654 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !28
  %dim655 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !28
  %dim656 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !28
  %DOT657 = load i32*, i32** %shapes653, align 8, !dbg !28
  %PT658 = load i32, i32* @PT, align 4, !dbg !28
  %stride659 = mul i32 1, %dim655, !dbg !28
  %stride660 = mul i32 %stride659, %dim656, !dbg !28
  %flat_idx661 = mul i32 %PT658, %stride660, !dbg !28
  %row662 = getelementptr i32, i32* %DOT657, i32 %flat_idx661, !dbg !28
  %stride663 = mul i32 1, %dim656, !dbg !28
  %flat_idx664 = mul i32 3, %stride663, !dbg !28
  %row665 = getelementptr i32, i32* %row662, i32 %flat_idx664, !dbg !28
  %ACCESS666 = getelementptr i32, i32* %row665, i32 0, !dbg !28
  store i32 4, i32* %ACCESS666, align 4, !dbg !28
  %gs667 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !28
  %shapes668 = getelementptr %struct.Game, %struct.Game* %gs667, i32 0, i32 1, !dbg !28
  %dim669 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !28
  %dim670 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !28
  %dim671 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !28
  %DOT672 = load i32*, i32** %shapes668, align 8, !dbg !28
  %PT673 = load i32, i32* @PT, align 4, !dbg !28
  %stride674 = mul i32 1, %dim670, !dbg !28
  %stride675 = mul i32 %stride674, %dim671, !dbg !28
  %flat_idx676 = mul i32 %PT673, %stride675, !dbg !28
  %row677 = getelementptr i32, i32* %DOT672, i32 %flat_idx676, !dbg !28
  %stride678 = mul i32 1, %dim671, !dbg !28
  %flat_idx679 = mul i32 3, %stride678, !dbg !28
  %row680 = getelementptr i32, i32* %row677, i32 %flat_idx679, !dbg !28
  %ACCESS681 = getelementptr i32, i32* %row680, i32 1, !dbg !28
  store i32 12, i32* %ACCESS681, align 4, !dbg !28
  %gs682 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !28
  %shapes683 = getelementptr %struct.Game, %struct.Game* %gs682, i32 0, i32 1, !dbg !28
  %dim684 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !28
  %dim685 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !28
  %dim686 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !28
  %DOT687 = load i32*, i32** %shapes683, align 8, !dbg !28
  %PT688 = load i32, i32* @PT, align 4, !dbg !28
  %stride689 = mul i32 1, %dim685, !dbg !28
  %stride690 = mul i32 %stride689, %dim686, !dbg !28
  %flat_idx691 = mul i32 %PT688, %stride690, !dbg !28
  %row692 = getelementptr i32, i32* %DOT687, i32 %flat_idx691, !dbg !28
  %stride693 = mul i32 1, %dim686, !dbg !28
  %flat_idx694 = mul i32 3, %stride693, !dbg !28
  %row695 = getelementptr i32, i32* %row692, i32 %flat_idx694, !dbg !28
  %ACCESS696 = getelementptr i32, i32* %row695, i32 2, !dbg !28
  store i32 4, i32* %ACCESS696, align 4, !dbg !28
  %gs697 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !28
  %shapes698 = getelementptr %struct.Game, %struct.Game* %gs697, i32 0, i32 1, !dbg !28
  %dim699 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !28
  %dim700 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !28
  %dim701 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !28
  %DOT702 = load i32*, i32** %shapes698, align 8, !dbg !28
  %PT703 = load i32, i32* @PT, align 4, !dbg !28
  %stride704 = mul i32 1, %dim700, !dbg !28
  %stride705 = mul i32 %stride704, %dim701, !dbg !28
  %flat_idx706 = mul i32 %PT703, %stride705, !dbg !28
  %row707 = getelementptr i32, i32* %DOT702, i32 %flat_idx706, !dbg !28
  %stride708 = mul i32 1, %dim701, !dbg !28
  %flat_idx709 = mul i32 3, %stride708, !dbg !28
  %row710 = getelementptr i32, i32* %row707, i32 %flat_idx709, !dbg !28
  %ACCESS711 = getelementptr i32, i32* %row710, i32 3, !dbg !28
  store i32 0, i32* %ACCESS711, align 4, !dbg !28
  %gs712 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !29
  %shapes713 = getelementptr %struct.Game, %struct.Game* %gs712, i32 0, i32 1, !dbg !29
  %dim714 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !29
  %dim715 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !29
  %dim716 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !29
  %DOT717 = load i32*, i32** %shapes713, align 8, !dbg !29
  %PS = load i32, i32* @PS, align 4, !dbg !29
  %stride718 = mul i32 1, %dim715, !dbg !29
  %stride719 = mul i32 %stride718, %dim716, !dbg !29
  %flat_idx720 = mul i32 %PS, %stride719, !dbg !29
  %row721 = getelementptr i32, i32* %DOT717, i32 %flat_idx720, !dbg !29
  %stride722 = mul i32 1, %dim716, !dbg !29
  %flat_idx723 = mul i32 0, %stride722, !dbg !29
  %row724 = getelementptr i32, i32* %row721, i32 %flat_idx723, !dbg !29
  %ACCESS725 = getelementptr i32, i32* %row724, i32 0, !dbg !29
  store i32 6, i32* %ACCESS725, align 4, !dbg !29
  %gs726 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !29
  %shapes727 = getelementptr %struct.Game, %struct.Game* %gs726, i32 0, i32 1, !dbg !29
  %dim728 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !29
  %dim729 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !29
  %dim730 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !29
  %DOT731 = load i32*, i32** %shapes727, align 8, !dbg !29
  %PS732 = load i32, i32* @PS, align 4, !dbg !29
  %stride733 = mul i32 1, %dim729, !dbg !29
  %stride734 = mul i32 %stride733, %dim730, !dbg !29
  %flat_idx735 = mul i32 %PS732, %stride734, !dbg !29
  %row736 = getelementptr i32, i32* %DOT731, i32 %flat_idx735, !dbg !29
  %stride737 = mul i32 1, %dim730, !dbg !29
  %flat_idx738 = mul i32 0, %stride737, !dbg !29
  %row739 = getelementptr i32, i32* %row736, i32 %flat_idx738, !dbg !29
  %ACCESS740 = getelementptr i32, i32* %row739, i32 1, !dbg !29
  store i32 12, i32* %ACCESS740, align 4, !dbg !29
  %gs741 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !29
  %shapes742 = getelementptr %struct.Game, %struct.Game* %gs741, i32 0, i32 1, !dbg !29
  %dim743 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !29
  %dim744 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !29
  %dim745 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !29
  %DOT746 = load i32*, i32** %shapes742, align 8, !dbg !29
  %PS747 = load i32, i32* @PS, align 4, !dbg !29
  %stride748 = mul i32 1, %dim744, !dbg !29
  %stride749 = mul i32 %stride748, %dim745, !dbg !29
  %flat_idx750 = mul i32 %PS747, %stride749, !dbg !29
  %row751 = getelementptr i32, i32* %DOT746, i32 %flat_idx750, !dbg !29
  %stride752 = mul i32 1, %dim745, !dbg !29
  %flat_idx753 = mul i32 0, %stride752, !dbg !29
  %row754 = getelementptr i32, i32* %row751, i32 %flat_idx753, !dbg !29
  %ACCESS755 = getelementptr i32, i32* %row754, i32 2, !dbg !29
  store i32 0, i32* %ACCESS755, align 4, !dbg !29
  %gs756 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !29
  %shapes757 = getelementptr %struct.Game, %struct.Game* %gs756, i32 0, i32 1, !dbg !29
  %dim758 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !29
  %dim759 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !29
  %dim760 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !29
  %DOT761 = load i32*, i32** %shapes757, align 8, !dbg !29
  %PS762 = load i32, i32* @PS, align 4, !dbg !29
  %stride763 = mul i32 1, %dim759, !dbg !29
  %stride764 = mul i32 %stride763, %dim760, !dbg !29
  %flat_idx765 = mul i32 %PS762, %stride764, !dbg !29
  %row766 = getelementptr i32, i32* %DOT761, i32 %flat_idx765, !dbg !29
  %stride767 = mul i32 1, %dim760, !dbg !29
  %flat_idx768 = mul i32 0, %stride767, !dbg !29
  %row769 = getelementptr i32, i32* %row766, i32 %flat_idx768, !dbg !29
  %ACCESS770 = getelementptr i32, i32* %row769, i32 3, !dbg !29
  store i32 0, i32* %ACCESS770, align 4, !dbg !29
  %gs771 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !30
  %shapes772 = getelementptr %struct.Game, %struct.Game* %gs771, i32 0, i32 1, !dbg !30
  %dim773 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !30
  %dim774 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !30
  %dim775 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !30
  %DOT776 = load i32*, i32** %shapes772, align 8, !dbg !30
  %PS777 = load i32, i32* @PS, align 4, !dbg !30
  %stride778 = mul i32 1, %dim774, !dbg !30
  %stride779 = mul i32 %stride778, %dim775, !dbg !30
  %flat_idx780 = mul i32 %PS777, %stride779, !dbg !30
  %row781 = getelementptr i32, i32* %DOT776, i32 %flat_idx780, !dbg !30
  %stride782 = mul i32 1, %dim775, !dbg !30
  %flat_idx783 = mul i32 1, %stride782, !dbg !30
  %row784 = getelementptr i32, i32* %row781, i32 %flat_idx783, !dbg !30
  %ACCESS785 = getelementptr i32, i32* %row784, i32 0, !dbg !30
  store i32 4, i32* %ACCESS785, align 4, !dbg !30
  %gs786 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !30
  %shapes787 = getelementptr %struct.Game, %struct.Game* %gs786, i32 0, i32 1, !dbg !30
  %dim788 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !30
  %dim789 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !30
  %dim790 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !30
  %DOT791 = load i32*, i32** %shapes787, align 8, !dbg !30
  %PS792 = load i32, i32* @PS, align 4, !dbg !30
  %stride793 = mul i32 1, %dim789, !dbg !30
  %stride794 = mul i32 %stride793, %dim790, !dbg !30
  %flat_idx795 = mul i32 %PS792, %stride794, !dbg !30
  %row796 = getelementptr i32, i32* %DOT791, i32 %flat_idx795, !dbg !30
  %stride797 = mul i32 1, %dim790, !dbg !30
  %flat_idx798 = mul i32 1, %stride797, !dbg !30
  %row799 = getelementptr i32, i32* %row796, i32 %flat_idx798, !dbg !30
  %ACCESS800 = getelementptr i32, i32* %row799, i32 1, !dbg !30
  store i32 6, i32* %ACCESS800, align 4, !dbg !30
  %gs801 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !30
  %shapes802 = getelementptr %struct.Game, %struct.Game* %gs801, i32 0, i32 1, !dbg !30
  %dim803 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !30
  %dim804 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !30
  %dim805 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !30
  %DOT806 = load i32*, i32** %shapes802, align 8, !dbg !30
  %PS807 = load i32, i32* @PS, align 4, !dbg !30
  %stride808 = mul i32 1, %dim804, !dbg !30
  %stride809 = mul i32 %stride808, %dim805, !dbg !30
  %flat_idx810 = mul i32 %PS807, %stride809, !dbg !30
  %row811 = getelementptr i32, i32* %DOT806, i32 %flat_idx810, !dbg !30
  %stride812 = mul i32 1, %dim805, !dbg !30
  %flat_idx813 = mul i32 1, %stride812, !dbg !30
  %row814 = getelementptr i32, i32* %row811, i32 %flat_idx813, !dbg !30
  %ACCESS815 = getelementptr i32, i32* %row814, i32 2, !dbg !30
  store i32 2, i32* %ACCESS815, align 4, !dbg !30
  %gs816 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !30
  %shapes817 = getelementptr %struct.Game, %struct.Game* %gs816, i32 0, i32 1, !dbg !30
  %dim818 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !30
  %dim819 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !30
  %dim820 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !30
  %DOT821 = load i32*, i32** %shapes817, align 8, !dbg !30
  %PS822 = load i32, i32* @PS, align 4, !dbg !30
  %stride823 = mul i32 1, %dim819, !dbg !30
  %stride824 = mul i32 %stride823, %dim820, !dbg !30
  %flat_idx825 = mul i32 %PS822, %stride824, !dbg !30
  %row826 = getelementptr i32, i32* %DOT821, i32 %flat_idx825, !dbg !30
  %stride827 = mul i32 1, %dim820, !dbg !30
  %flat_idx828 = mul i32 1, %stride827, !dbg !30
  %row829 = getelementptr i32, i32* %row826, i32 %flat_idx828, !dbg !30
  %ACCESS830 = getelementptr i32, i32* %row829, i32 3, !dbg !30
  store i32 0, i32* %ACCESS830, align 4, !dbg !30
  %gs831 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !31
  %shapes832 = getelementptr %struct.Game, %struct.Game* %gs831, i32 0, i32 1, !dbg !31
  %dim833 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !31
  %dim834 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !31
  %dim835 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !31
  %DOT836 = load i32*, i32** %shapes832, align 8, !dbg !31
  %PS837 = load i32, i32* @PS, align 4, !dbg !31
  %stride838 = mul i32 1, %dim834, !dbg !31
  %stride839 = mul i32 %stride838, %dim835, !dbg !31
  %flat_idx840 = mul i32 %PS837, %stride839, !dbg !31
  %row841 = getelementptr i32, i32* %DOT836, i32 %flat_idx840, !dbg !31
  %stride842 = mul i32 1, %dim835, !dbg !31
  %flat_idx843 = mul i32 2, %stride842, !dbg !31
  %row844 = getelementptr i32, i32* %row841, i32 %flat_idx843, !dbg !31
  %ACCESS845 = getelementptr i32, i32* %row844, i32 0, !dbg !31
  store i32 6, i32* %ACCESS845, align 4, !dbg !31
  %gs846 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !31
  %shapes847 = getelementptr %struct.Game, %struct.Game* %gs846, i32 0, i32 1, !dbg !31
  %dim848 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !31
  %dim849 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !31
  %dim850 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !31
  %DOT851 = load i32*, i32** %shapes847, align 8, !dbg !31
  %PS852 = load i32, i32* @PS, align 4, !dbg !31
  %stride853 = mul i32 1, %dim849, !dbg !31
  %stride854 = mul i32 %stride853, %dim850, !dbg !31
  %flat_idx855 = mul i32 %PS852, %stride854, !dbg !31
  %row856 = getelementptr i32, i32* %DOT851, i32 %flat_idx855, !dbg !31
  %stride857 = mul i32 1, %dim850, !dbg !31
  %flat_idx858 = mul i32 2, %stride857, !dbg !31
  %row859 = getelementptr i32, i32* %row856, i32 %flat_idx858, !dbg !31
  %ACCESS860 = getelementptr i32, i32* %row859, i32 1, !dbg !31
  store i32 12, i32* %ACCESS860, align 4, !dbg !31
  %gs861 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !31
  %shapes862 = getelementptr %struct.Game, %struct.Game* %gs861, i32 0, i32 1, !dbg !31
  %dim863 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !31
  %dim864 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !31
  %dim865 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !31
  %DOT866 = load i32*, i32** %shapes862, align 8, !dbg !31
  %PS867 = load i32, i32* @PS, align 4, !dbg !31
  %stride868 = mul i32 1, %dim864, !dbg !31
  %stride869 = mul i32 %stride868, %dim865, !dbg !31
  %flat_idx870 = mul i32 %PS867, %stride869, !dbg !31
  %row871 = getelementptr i32, i32* %DOT866, i32 %flat_idx870, !dbg !31
  %stride872 = mul i32 1, %dim865, !dbg !31
  %flat_idx873 = mul i32 2, %stride872, !dbg !31
  %row874 = getelementptr i32, i32* %row871, i32 %flat_idx873, !dbg !31
  %ACCESS875 = getelementptr i32, i32* %row874, i32 2, !dbg !31
  store i32 0, i32* %ACCESS875, align 4, !dbg !31
  %gs876 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !31
  %shapes877 = getelementptr %struct.Game, %struct.Game* %gs876, i32 0, i32 1, !dbg !31
  %dim878 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !31
  %dim879 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !31
  %dim880 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !31
  %DOT881 = load i32*, i32** %shapes877, align 8, !dbg !31
  %PS882 = load i32, i32* @PS, align 4, !dbg !31
  %stride883 = mul i32 1, %dim879, !dbg !31
  %stride884 = mul i32 %stride883, %dim880, !dbg !31
  %flat_idx885 = mul i32 %PS882, %stride884, !dbg !31
  %row886 = getelementptr i32, i32* %DOT881, i32 %flat_idx885, !dbg !31
  %stride887 = mul i32 1, %dim880, !dbg !31
  %flat_idx888 = mul i32 2, %stride887, !dbg !31
  %row889 = getelementptr i32, i32* %row886, i32 %flat_idx888, !dbg !31
  %ACCESS890 = getelementptr i32, i32* %row889, i32 3, !dbg !31
  store i32 0, i32* %ACCESS890, align 4, !dbg !31
  %gs891 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !32
  %shapes892 = getelementptr %struct.Game, %struct.Game* %gs891, i32 0, i32 1, !dbg !32
  %dim893 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !32
  %dim894 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !32
  %dim895 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !32
  %DOT896 = load i32*, i32** %shapes892, align 8, !dbg !32
  %PS897 = load i32, i32* @PS, align 4, !dbg !32
  %stride898 = mul i32 1, %dim894, !dbg !32
  %stride899 = mul i32 %stride898, %dim895, !dbg !32
  %flat_idx900 = mul i32 %PS897, %stride899, !dbg !32
  %row901 = getelementptr i32, i32* %DOT896, i32 %flat_idx900, !dbg !32
  %stride902 = mul i32 1, %dim895, !dbg !32
  %flat_idx903 = mul i32 3, %stride902, !dbg !32
  %row904 = getelementptr i32, i32* %row901, i32 %flat_idx903, !dbg !32
  %ACCESS905 = getelementptr i32, i32* %row904, i32 0, !dbg !32
  store i32 4, i32* %ACCESS905, align 4, !dbg !32
  %gs906 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !32
  %shapes907 = getelementptr %struct.Game, %struct.Game* %gs906, i32 0, i32 1, !dbg !32
  %dim908 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !32
  %dim909 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !32
  %dim910 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !32
  %DOT911 = load i32*, i32** %shapes907, align 8, !dbg !32
  %PS912 = load i32, i32* @PS, align 4, !dbg !32
  %stride913 = mul i32 1, %dim909, !dbg !32
  %stride914 = mul i32 %stride913, %dim910, !dbg !32
  %flat_idx915 = mul i32 %PS912, %stride914, !dbg !32
  %row916 = getelementptr i32, i32* %DOT911, i32 %flat_idx915, !dbg !32
  %stride917 = mul i32 1, %dim910, !dbg !32
  %flat_idx918 = mul i32 3, %stride917, !dbg !32
  %row919 = getelementptr i32, i32* %row916, i32 %flat_idx918, !dbg !32
  %ACCESS920 = getelementptr i32, i32* %row919, i32 1, !dbg !32
  store i32 6, i32* %ACCESS920, align 4, !dbg !32
  %gs921 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !32
  %shapes922 = getelementptr %struct.Game, %struct.Game* %gs921, i32 0, i32 1, !dbg !32
  %dim923 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !32
  %dim924 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !32
  %dim925 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !32
  %DOT926 = load i32*, i32** %shapes922, align 8, !dbg !32
  %PS927 = load i32, i32* @PS, align 4, !dbg !32
  %stride928 = mul i32 1, %dim924, !dbg !32
  %stride929 = mul i32 %stride928, %dim925, !dbg !32
  %flat_idx930 = mul i32 %PS927, %stride929, !dbg !32
  %row931 = getelementptr i32, i32* %DOT926, i32 %flat_idx930, !dbg !32
  %stride932 = mul i32 1, %dim925, !dbg !32
  %flat_idx933 = mul i32 3, %stride932, !dbg !32
  %row934 = getelementptr i32, i32* %row931, i32 %flat_idx933, !dbg !32
  %ACCESS935 = getelementptr i32, i32* %row934, i32 2, !dbg !32
  store i32 2, i32* %ACCESS935, align 4, !dbg !32
  %gs936 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !32
  %shapes937 = getelementptr %struct.Game, %struct.Game* %gs936, i32 0, i32 1, !dbg !32
  %dim938 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !32
  %dim939 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !32
  %dim940 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !32
  %DOT941 = load i32*, i32** %shapes937, align 8, !dbg !32
  %PS942 = load i32, i32* @PS, align 4, !dbg !32
  %stride943 = mul i32 1, %dim939, !dbg !32
  %stride944 = mul i32 %stride943, %dim940, !dbg !32
  %flat_idx945 = mul i32 %PS942, %stride944, !dbg !32
  %row946 = getelementptr i32, i32* %DOT941, i32 %flat_idx945, !dbg !32
  %stride947 = mul i32 1, %dim940, !dbg !32
  %flat_idx948 = mul i32 3, %stride947, !dbg !32
  %row949 = getelementptr i32, i32* %row946, i32 %flat_idx948, !dbg !32
  %ACCESS950 = getelementptr i32, i32* %row949, i32 3, !dbg !32
  store i32 0, i32* %ACCESS950, align 4, !dbg !32
  %gs951 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !33
  %shapes952 = getelementptr %struct.Game, %struct.Game* %gs951, i32 0, i32 1, !dbg !33
  %dim953 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !33
  %dim954 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !33
  %dim955 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !33
  %DOT956 = load i32*, i32** %shapes952, align 8, !dbg !33
  %PZ = load i32, i32* @PZ, align 4, !dbg !33
  %stride957 = mul i32 1, %dim954, !dbg !33
  %stride958 = mul i32 %stride957, %dim955, !dbg !33
  %flat_idx959 = mul i32 %PZ, %stride958, !dbg !33
  %row960 = getelementptr i32, i32* %DOT956, i32 %flat_idx959, !dbg !33
  %stride961 = mul i32 1, %dim955, !dbg !33
  %flat_idx962 = mul i32 0, %stride961, !dbg !33
  %row963 = getelementptr i32, i32* %row960, i32 %flat_idx962, !dbg !33
  %ACCESS964 = getelementptr i32, i32* %row963, i32 0, !dbg !33
  store i32 12, i32* %ACCESS964, align 4, !dbg !33
  %gs965 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !33
  %shapes966 = getelementptr %struct.Game, %struct.Game* %gs965, i32 0, i32 1, !dbg !33
  %dim967 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !33
  %dim968 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !33
  %dim969 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !33
  %DOT970 = load i32*, i32** %shapes966, align 8, !dbg !33
  %PZ971 = load i32, i32* @PZ, align 4, !dbg !33
  %stride972 = mul i32 1, %dim968, !dbg !33
  %stride973 = mul i32 %stride972, %dim969, !dbg !33
  %flat_idx974 = mul i32 %PZ971, %stride973, !dbg !33
  %row975 = getelementptr i32, i32* %DOT970, i32 %flat_idx974, !dbg !33
  %stride976 = mul i32 1, %dim969, !dbg !33
  %flat_idx977 = mul i32 0, %stride976, !dbg !33
  %row978 = getelementptr i32, i32* %row975, i32 %flat_idx977, !dbg !33
  %ACCESS979 = getelementptr i32, i32* %row978, i32 1, !dbg !33
  store i32 6, i32* %ACCESS979, align 4, !dbg !33
  %gs980 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !33
  %shapes981 = getelementptr %struct.Game, %struct.Game* %gs980, i32 0, i32 1, !dbg !33
  %dim982 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !33
  %dim983 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !33
  %dim984 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !33
  %DOT985 = load i32*, i32** %shapes981, align 8, !dbg !33
  %PZ986 = load i32, i32* @PZ, align 4, !dbg !33
  %stride987 = mul i32 1, %dim983, !dbg !33
  %stride988 = mul i32 %stride987, %dim984, !dbg !33
  %flat_idx989 = mul i32 %PZ986, %stride988, !dbg !33
  %row990 = getelementptr i32, i32* %DOT985, i32 %flat_idx989, !dbg !33
  %stride991 = mul i32 1, %dim984, !dbg !33
  %flat_idx992 = mul i32 0, %stride991, !dbg !33
  %row993 = getelementptr i32, i32* %row990, i32 %flat_idx992, !dbg !33
  %ACCESS994 = getelementptr i32, i32* %row993, i32 2, !dbg !33
  store i32 0, i32* %ACCESS994, align 4, !dbg !33
  %gs995 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !33
  %shapes996 = getelementptr %struct.Game, %struct.Game* %gs995, i32 0, i32 1, !dbg !33
  %dim997 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !33
  %dim998 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !33
  %dim999 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !33
  %DOT1000 = load i32*, i32** %shapes996, align 8, !dbg !33
  %PZ1001 = load i32, i32* @PZ, align 4, !dbg !33
  %stride1002 = mul i32 1, %dim998, !dbg !33
  %stride1003 = mul i32 %stride1002, %dim999, !dbg !33
  %flat_idx1004 = mul i32 %PZ1001, %stride1003, !dbg !33
  %row1005 = getelementptr i32, i32* %DOT1000, i32 %flat_idx1004, !dbg !33
  %stride1006 = mul i32 1, %dim999, !dbg !33
  %flat_idx1007 = mul i32 0, %stride1006, !dbg !33
  %row1008 = getelementptr i32, i32* %row1005, i32 %flat_idx1007, !dbg !33
  %ACCESS1009 = getelementptr i32, i32* %row1008, i32 3, !dbg !33
  store i32 0, i32* %ACCESS1009, align 4, !dbg !33
  %gs1010 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !34
  %shapes1011 = getelementptr %struct.Game, %struct.Game* %gs1010, i32 0, i32 1, !dbg !34
  %dim1012 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !34
  %dim1013 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !34
  %dim1014 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !34
  %DOT1015 = load i32*, i32** %shapes1011, align 8, !dbg !34
  %PZ1016 = load i32, i32* @PZ, align 4, !dbg !34
  %stride1017 = mul i32 1, %dim1013, !dbg !34
  %stride1018 = mul i32 %stride1017, %dim1014, !dbg !34
  %flat_idx1019 = mul i32 %PZ1016, %stride1018, !dbg !34
  %row1020 = getelementptr i32, i32* %DOT1015, i32 %flat_idx1019, !dbg !34
  %stride1021 = mul i32 1, %dim1014, !dbg !34
  %flat_idx1022 = mul i32 1, %stride1021, !dbg !34
  %row1023 = getelementptr i32, i32* %row1020, i32 %flat_idx1022, !dbg !34
  %ACCESS1024 = getelementptr i32, i32* %row1023, i32 0, !dbg !34
  store i32 2, i32* %ACCESS1024, align 4, !dbg !34
  %gs1025 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !34
  %shapes1026 = getelementptr %struct.Game, %struct.Game* %gs1025, i32 0, i32 1, !dbg !34
  %dim1027 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !34
  %dim1028 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !34
  %dim1029 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !34
  %DOT1030 = load i32*, i32** %shapes1026, align 8, !dbg !34
  %PZ1031 = load i32, i32* @PZ, align 4, !dbg !34
  %stride1032 = mul i32 1, %dim1028, !dbg !34
  %stride1033 = mul i32 %stride1032, %dim1029, !dbg !34
  %flat_idx1034 = mul i32 %PZ1031, %stride1033, !dbg !34
  %row1035 = getelementptr i32, i32* %DOT1030, i32 %flat_idx1034, !dbg !34
  %stride1036 = mul i32 1, %dim1029, !dbg !34
  %flat_idx1037 = mul i32 1, %stride1036, !dbg !34
  %row1038 = getelementptr i32, i32* %row1035, i32 %flat_idx1037, !dbg !34
  %ACCESS1039 = getelementptr i32, i32* %row1038, i32 1, !dbg !34
  store i32 6, i32* %ACCESS1039, align 4, !dbg !34
  %gs1040 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !34
  %shapes1041 = getelementptr %struct.Game, %struct.Game* %gs1040, i32 0, i32 1, !dbg !34
  %dim1042 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !34
  %dim1043 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !34
  %dim1044 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !34
  %DOT1045 = load i32*, i32** %shapes1041, align 8, !dbg !34
  %PZ1046 = load i32, i32* @PZ, align 4, !dbg !34
  %stride1047 = mul i32 1, %dim1043, !dbg !34
  %stride1048 = mul i32 %stride1047, %dim1044, !dbg !34
  %flat_idx1049 = mul i32 %PZ1046, %stride1048, !dbg !34
  %row1050 = getelementptr i32, i32* %DOT1045, i32 %flat_idx1049, !dbg !34
  %stride1051 = mul i32 1, %dim1044, !dbg !34
  %flat_idx1052 = mul i32 1, %stride1051, !dbg !34
  %row1053 = getelementptr i32, i32* %row1050, i32 %flat_idx1052, !dbg !34
  %ACCESS1054 = getelementptr i32, i32* %row1053, i32 2, !dbg !34
  store i32 4, i32* %ACCESS1054, align 4, !dbg !34
  %gs1055 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !34
  %shapes1056 = getelementptr %struct.Game, %struct.Game* %gs1055, i32 0, i32 1, !dbg !34
  %dim1057 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !34
  %dim1058 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !34
  %dim1059 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !34
  %DOT1060 = load i32*, i32** %shapes1056, align 8, !dbg !34
  %PZ1061 = load i32, i32* @PZ, align 4, !dbg !34
  %stride1062 = mul i32 1, %dim1058, !dbg !34
  %stride1063 = mul i32 %stride1062, %dim1059, !dbg !34
  %flat_idx1064 = mul i32 %PZ1061, %stride1063, !dbg !34
  %row1065 = getelementptr i32, i32* %DOT1060, i32 %flat_idx1064, !dbg !34
  %stride1066 = mul i32 1, %dim1059, !dbg !34
  %flat_idx1067 = mul i32 1, %stride1066, !dbg !34
  %row1068 = getelementptr i32, i32* %row1065, i32 %flat_idx1067, !dbg !34
  %ACCESS1069 = getelementptr i32, i32* %row1068, i32 3, !dbg !34
  store i32 0, i32* %ACCESS1069, align 4, !dbg !34
  %gs1070 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !35
  %shapes1071 = getelementptr %struct.Game, %struct.Game* %gs1070, i32 0, i32 1, !dbg !35
  %dim1072 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !35
  %dim1073 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !35
  %dim1074 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !35
  %DOT1075 = load i32*, i32** %shapes1071, align 8, !dbg !35
  %PZ1076 = load i32, i32* @PZ, align 4, !dbg !35
  %stride1077 = mul i32 1, %dim1073, !dbg !35
  %stride1078 = mul i32 %stride1077, %dim1074, !dbg !35
  %flat_idx1079 = mul i32 %PZ1076, %stride1078, !dbg !35
  %row1080 = getelementptr i32, i32* %DOT1075, i32 %flat_idx1079, !dbg !35
  %stride1081 = mul i32 1, %dim1074, !dbg !35
  %flat_idx1082 = mul i32 2, %stride1081, !dbg !35
  %row1083 = getelementptr i32, i32* %row1080, i32 %flat_idx1082, !dbg !35
  %ACCESS1084 = getelementptr i32, i32* %row1083, i32 0, !dbg !35
  store i32 12, i32* %ACCESS1084, align 4, !dbg !35
  %gs1085 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !35
  %shapes1086 = getelementptr %struct.Game, %struct.Game* %gs1085, i32 0, i32 1, !dbg !35
  %dim1087 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !35
  %dim1088 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !35
  %dim1089 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !35
  %DOT1090 = load i32*, i32** %shapes1086, align 8, !dbg !35
  %PZ1091 = load i32, i32* @PZ, align 4, !dbg !35
  %stride1092 = mul i32 1, %dim1088, !dbg !35
  %stride1093 = mul i32 %stride1092, %dim1089, !dbg !35
  %flat_idx1094 = mul i32 %PZ1091, %stride1093, !dbg !35
  %row1095 = getelementptr i32, i32* %DOT1090, i32 %flat_idx1094, !dbg !35
  %stride1096 = mul i32 1, %dim1089, !dbg !35
  %flat_idx1097 = mul i32 2, %stride1096, !dbg !35
  %row1098 = getelementptr i32, i32* %row1095, i32 %flat_idx1097, !dbg !35
  %ACCESS1099 = getelementptr i32, i32* %row1098, i32 1, !dbg !35
  store i32 6, i32* %ACCESS1099, align 4, !dbg !35
  %gs1100 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !35
  %shapes1101 = getelementptr %struct.Game, %struct.Game* %gs1100, i32 0, i32 1, !dbg !35
  %dim1102 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !35
  %dim1103 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !35
  %dim1104 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !35
  %DOT1105 = load i32*, i32** %shapes1101, align 8, !dbg !35
  %PZ1106 = load i32, i32* @PZ, align 4, !dbg !35
  %stride1107 = mul i32 1, %dim1103, !dbg !35
  %stride1108 = mul i32 %stride1107, %dim1104, !dbg !35
  %flat_idx1109 = mul i32 %PZ1106, %stride1108, !dbg !35
  %row1110 = getelementptr i32, i32* %DOT1105, i32 %flat_idx1109, !dbg !35
  %stride1111 = mul i32 1, %dim1104, !dbg !35
  %flat_idx1112 = mul i32 2, %stride1111, !dbg !35
  %row1113 = getelementptr i32, i32* %row1110, i32 %flat_idx1112, !dbg !35
  %ACCESS1114 = getelementptr i32, i32* %row1113, i32 2, !dbg !35
  store i32 0, i32* %ACCESS1114, align 4, !dbg !35
  %gs1115 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !35
  %shapes1116 = getelementptr %struct.Game, %struct.Game* %gs1115, i32 0, i32 1, !dbg !35
  %dim1117 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !35
  %dim1118 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !35
  %dim1119 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !35
  %DOT1120 = load i32*, i32** %shapes1116, align 8, !dbg !35
  %PZ1121 = load i32, i32* @PZ, align 4, !dbg !35
  %stride1122 = mul i32 1, %dim1118, !dbg !35
  %stride1123 = mul i32 %stride1122, %dim1119, !dbg !35
  %flat_idx1124 = mul i32 %PZ1121, %stride1123, !dbg !35
  %row1125 = getelementptr i32, i32* %DOT1120, i32 %flat_idx1124, !dbg !35
  %stride1126 = mul i32 1, %dim1119, !dbg !35
  %flat_idx1127 = mul i32 2, %stride1126, !dbg !35
  %row1128 = getelementptr i32, i32* %row1125, i32 %flat_idx1127, !dbg !35
  %ACCESS1129 = getelementptr i32, i32* %row1128, i32 3, !dbg !35
  store i32 0, i32* %ACCESS1129, align 4, !dbg !35
  %gs1130 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !36
  %shapes1131 = getelementptr %struct.Game, %struct.Game* %gs1130, i32 0, i32 1, !dbg !36
  %dim1132 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !36
  %dim1133 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !36
  %dim1134 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !36
  %DOT1135 = load i32*, i32** %shapes1131, align 8, !dbg !36
  %PZ1136 = load i32, i32* @PZ, align 4, !dbg !36
  %stride1137 = mul i32 1, %dim1133, !dbg !36
  %stride1138 = mul i32 %stride1137, %dim1134, !dbg !36
  %flat_idx1139 = mul i32 %PZ1136, %stride1138, !dbg !36
  %row1140 = getelementptr i32, i32* %DOT1135, i32 %flat_idx1139, !dbg !36
  %stride1141 = mul i32 1, %dim1134, !dbg !36
  %flat_idx1142 = mul i32 3, %stride1141, !dbg !36
  %row1143 = getelementptr i32, i32* %row1140, i32 %flat_idx1142, !dbg !36
  %ACCESS1144 = getelementptr i32, i32* %row1143, i32 0, !dbg !36
  store i32 2, i32* %ACCESS1144, align 4, !dbg !36
  %gs1145 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !36
  %shapes1146 = getelementptr %struct.Game, %struct.Game* %gs1145, i32 0, i32 1, !dbg !36
  %dim1147 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !36
  %dim1148 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !36
  %dim1149 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !36
  %DOT1150 = load i32*, i32** %shapes1146, align 8, !dbg !36
  %PZ1151 = load i32, i32* @PZ, align 4, !dbg !36
  %stride1152 = mul i32 1, %dim1148, !dbg !36
  %stride1153 = mul i32 %stride1152, %dim1149, !dbg !36
  %flat_idx1154 = mul i32 %PZ1151, %stride1153, !dbg !36
  %row1155 = getelementptr i32, i32* %DOT1150, i32 %flat_idx1154, !dbg !36
  %stride1156 = mul i32 1, %dim1149, !dbg !36
  %flat_idx1157 = mul i32 3, %stride1156, !dbg !36
  %row1158 = getelementptr i32, i32* %row1155, i32 %flat_idx1157, !dbg !36
  %ACCESS1159 = getelementptr i32, i32* %row1158, i32 1, !dbg !36
  store i32 6, i32* %ACCESS1159, align 4, !dbg !36
  %gs1160 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !36
  %shapes1161 = getelementptr %struct.Game, %struct.Game* %gs1160, i32 0, i32 1, !dbg !36
  %dim1162 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !36
  %dim1163 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !36
  %dim1164 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !36
  %DOT1165 = load i32*, i32** %shapes1161, align 8, !dbg !36
  %PZ1166 = load i32, i32* @PZ, align 4, !dbg !36
  %stride1167 = mul i32 1, %dim1163, !dbg !36
  %stride1168 = mul i32 %stride1167, %dim1164, !dbg !36
  %flat_idx1169 = mul i32 %PZ1166, %stride1168, !dbg !36
  %row1170 = getelementptr i32, i32* %DOT1165, i32 %flat_idx1169, !dbg !36
  %stride1171 = mul i32 1, %dim1164, !dbg !36
  %flat_idx1172 = mul i32 3, %stride1171, !dbg !36
  %row1173 = getelementptr i32, i32* %row1170, i32 %flat_idx1172, !dbg !36
  %ACCESS1174 = getelementptr i32, i32* %row1173, i32 2, !dbg !36
  store i32 4, i32* %ACCESS1174, align 4, !dbg !36
  %gs1175 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !36
  %shapes1176 = getelementptr %struct.Game, %struct.Game* %gs1175, i32 0, i32 1, !dbg !36
  %dim1177 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !36
  %dim1178 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !36
  %dim1179 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !36
  %DOT1180 = load i32*, i32** %shapes1176, align 8, !dbg !36
  %PZ1181 = load i32, i32* @PZ, align 4, !dbg !36
  %stride1182 = mul i32 1, %dim1178, !dbg !36
  %stride1183 = mul i32 %stride1182, %dim1179, !dbg !36
  %flat_idx1184 = mul i32 %PZ1181, %stride1183, !dbg !36
  %row1185 = getelementptr i32, i32* %DOT1180, i32 %flat_idx1184, !dbg !36
  %stride1186 = mul i32 1, %dim1179, !dbg !36
  %flat_idx1187 = mul i32 3, %stride1186, !dbg !36
  %row1188 = getelementptr i32, i32* %row1185, i32 %flat_idx1187, !dbg !36
  %ACCESS1189 = getelementptr i32, i32* %row1188, i32 3, !dbg !36
  store i32 0, i32* %ACCESS1189, align 4, !dbg !36
  %gs1190 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !37
  %shapes1191 = getelementptr %struct.Game, %struct.Game* %gs1190, i32 0, i32 1, !dbg !37
  %dim1192 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !37
  %dim1193 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !37
  %dim1194 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !37
  %DOT1195 = load i32*, i32** %shapes1191, align 8, !dbg !37
  %PJ = load i32, i32* @PJ, align 4, !dbg !37
  %stride1196 = mul i32 1, %dim1193, !dbg !37
  %stride1197 = mul i32 %stride1196, %dim1194, !dbg !37
  %flat_idx1198 = mul i32 %PJ, %stride1197, !dbg !37
  %row1199 = getelementptr i32, i32* %DOT1195, i32 %flat_idx1198, !dbg !37
  %stride1200 = mul i32 1, %dim1194, !dbg !37
  %flat_idx1201 = mul i32 0, %stride1200, !dbg !37
  %row1202 = getelementptr i32, i32* %row1199, i32 %flat_idx1201, !dbg !37
  %ACCESS1203 = getelementptr i32, i32* %row1202, i32 0, !dbg !37
  store i32 8, i32* %ACCESS1203, align 4, !dbg !37
  %gs1204 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !37
  %shapes1205 = getelementptr %struct.Game, %struct.Game* %gs1204, i32 0, i32 1, !dbg !37
  %dim1206 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !37
  %dim1207 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !37
  %dim1208 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !37
  %DOT1209 = load i32*, i32** %shapes1205, align 8, !dbg !37
  %PJ1210 = load i32, i32* @PJ, align 4, !dbg !37
  %stride1211 = mul i32 1, %dim1207, !dbg !37
  %stride1212 = mul i32 %stride1211, %dim1208, !dbg !37
  %flat_idx1213 = mul i32 %PJ1210, %stride1212, !dbg !37
  %row1214 = getelementptr i32, i32* %DOT1209, i32 %flat_idx1213, !dbg !37
  %stride1215 = mul i32 1, %dim1208, !dbg !37
  %flat_idx1216 = mul i32 0, %stride1215, !dbg !37
  %row1217 = getelementptr i32, i32* %row1214, i32 %flat_idx1216, !dbg !37
  %ACCESS1218 = getelementptr i32, i32* %row1217, i32 1, !dbg !37
  store i32 14, i32* %ACCESS1218, align 4, !dbg !37
  %gs1219 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !37
  %shapes1220 = getelementptr %struct.Game, %struct.Game* %gs1219, i32 0, i32 1, !dbg !37
  %dim1221 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !37
  %dim1222 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !37
  %dim1223 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !37
  %DOT1224 = load i32*, i32** %shapes1220, align 8, !dbg !37
  %PJ1225 = load i32, i32* @PJ, align 4, !dbg !37
  %stride1226 = mul i32 1, %dim1222, !dbg !37
  %stride1227 = mul i32 %stride1226, %dim1223, !dbg !37
  %flat_idx1228 = mul i32 %PJ1225, %stride1227, !dbg !37
  %row1229 = getelementptr i32, i32* %DOT1224, i32 %flat_idx1228, !dbg !37
  %stride1230 = mul i32 1, %dim1223, !dbg !37
  %flat_idx1231 = mul i32 0, %stride1230, !dbg !37
  %row1232 = getelementptr i32, i32* %row1229, i32 %flat_idx1231, !dbg !37
  %ACCESS1233 = getelementptr i32, i32* %row1232, i32 2, !dbg !37
  store i32 0, i32* %ACCESS1233, align 4, !dbg !37
  %gs1234 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !37
  %shapes1235 = getelementptr %struct.Game, %struct.Game* %gs1234, i32 0, i32 1, !dbg !37
  %dim1236 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !37
  %dim1237 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !37
  %dim1238 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !37
  %DOT1239 = load i32*, i32** %shapes1235, align 8, !dbg !37
  %PJ1240 = load i32, i32* @PJ, align 4, !dbg !37
  %stride1241 = mul i32 1, %dim1237, !dbg !37
  %stride1242 = mul i32 %stride1241, %dim1238, !dbg !37
  %flat_idx1243 = mul i32 %PJ1240, %stride1242, !dbg !37
  %row1244 = getelementptr i32, i32* %DOT1239, i32 %flat_idx1243, !dbg !37
  %stride1245 = mul i32 1, %dim1238, !dbg !37
  %flat_idx1246 = mul i32 0, %stride1245, !dbg !37
  %row1247 = getelementptr i32, i32* %row1244, i32 %flat_idx1246, !dbg !37
  %ACCESS1248 = getelementptr i32, i32* %row1247, i32 3, !dbg !37
  store i32 0, i32* %ACCESS1248, align 4, !dbg !37
  %gs1249 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !38
  %shapes1250 = getelementptr %struct.Game, %struct.Game* %gs1249, i32 0, i32 1, !dbg !38
  %dim1251 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !38
  %dim1252 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !38
  %dim1253 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !38
  %DOT1254 = load i32*, i32** %shapes1250, align 8, !dbg !38
  %PJ1255 = load i32, i32* @PJ, align 4, !dbg !38
  %stride1256 = mul i32 1, %dim1252, !dbg !38
  %stride1257 = mul i32 %stride1256, %dim1253, !dbg !38
  %flat_idx1258 = mul i32 %PJ1255, %stride1257, !dbg !38
  %row1259 = getelementptr i32, i32* %DOT1254, i32 %flat_idx1258, !dbg !38
  %stride1260 = mul i32 1, %dim1253, !dbg !38
  %flat_idx1261 = mul i32 1, %stride1260, !dbg !38
  %row1262 = getelementptr i32, i32* %row1259, i32 %flat_idx1261, !dbg !38
  %ACCESS1263 = getelementptr i32, i32* %row1262, i32 0, !dbg !38
  store i32 6, i32* %ACCESS1263, align 4, !dbg !38
  %gs1264 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !38
  %shapes1265 = getelementptr %struct.Game, %struct.Game* %gs1264, i32 0, i32 1, !dbg !38
  %dim1266 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !38
  %dim1267 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !38
  %dim1268 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !38
  %DOT1269 = load i32*, i32** %shapes1265, align 8, !dbg !38
  %PJ1270 = load i32, i32* @PJ, align 4, !dbg !38
  %stride1271 = mul i32 1, %dim1267, !dbg !38
  %stride1272 = mul i32 %stride1271, %dim1268, !dbg !38
  %flat_idx1273 = mul i32 %PJ1270, %stride1272, !dbg !38
  %row1274 = getelementptr i32, i32* %DOT1269, i32 %flat_idx1273, !dbg !38
  %stride1275 = mul i32 1, %dim1268, !dbg !38
  %flat_idx1276 = mul i32 1, %stride1275, !dbg !38
  %row1277 = getelementptr i32, i32* %row1274, i32 %flat_idx1276, !dbg !38
  %ACCESS1278 = getelementptr i32, i32* %row1277, i32 1, !dbg !38
  store i32 4, i32* %ACCESS1278, align 4, !dbg !38
  %gs1279 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !38
  %shapes1280 = getelementptr %struct.Game, %struct.Game* %gs1279, i32 0, i32 1, !dbg !38
  %dim1281 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !38
  %dim1282 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !38
  %dim1283 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !38
  %DOT1284 = load i32*, i32** %shapes1280, align 8, !dbg !38
  %PJ1285 = load i32, i32* @PJ, align 4, !dbg !38
  %stride1286 = mul i32 1, %dim1282, !dbg !38
  %stride1287 = mul i32 %stride1286, %dim1283, !dbg !38
  %flat_idx1288 = mul i32 %PJ1285, %stride1287, !dbg !38
  %row1289 = getelementptr i32, i32* %DOT1284, i32 %flat_idx1288, !dbg !38
  %stride1290 = mul i32 1, %dim1283, !dbg !38
  %flat_idx1291 = mul i32 1, %stride1290, !dbg !38
  %row1292 = getelementptr i32, i32* %row1289, i32 %flat_idx1291, !dbg !38
  %ACCESS1293 = getelementptr i32, i32* %row1292, i32 2, !dbg !38
  store i32 4, i32* %ACCESS1293, align 4, !dbg !38
  %gs1294 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !38
  %shapes1295 = getelementptr %struct.Game, %struct.Game* %gs1294, i32 0, i32 1, !dbg !38
  %dim1296 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !38
  %dim1297 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !38
  %dim1298 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !38
  %DOT1299 = load i32*, i32** %shapes1295, align 8, !dbg !38
  %PJ1300 = load i32, i32* @PJ, align 4, !dbg !38
  %stride1301 = mul i32 1, %dim1297, !dbg !38
  %stride1302 = mul i32 %stride1301, %dim1298, !dbg !38
  %flat_idx1303 = mul i32 %PJ1300, %stride1302, !dbg !38
  %row1304 = getelementptr i32, i32* %DOT1299, i32 %flat_idx1303, !dbg !38
  %stride1305 = mul i32 1, %dim1298, !dbg !38
  %flat_idx1306 = mul i32 1, %stride1305, !dbg !38
  %row1307 = getelementptr i32, i32* %row1304, i32 %flat_idx1306, !dbg !38
  %ACCESS1308 = getelementptr i32, i32* %row1307, i32 3, !dbg !38
  store i32 0, i32* %ACCESS1308, align 4, !dbg !38
  %gs1309 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !39
  %shapes1310 = getelementptr %struct.Game, %struct.Game* %gs1309, i32 0, i32 1, !dbg !39
  %dim1311 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !39
  %dim1312 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !39
  %dim1313 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !39
  %DOT1314 = load i32*, i32** %shapes1310, align 8, !dbg !39
  %PJ1315 = load i32, i32* @PJ, align 4, !dbg !39
  %stride1316 = mul i32 1, %dim1312, !dbg !39
  %stride1317 = mul i32 %stride1316, %dim1313, !dbg !39
  %flat_idx1318 = mul i32 %PJ1315, %stride1317, !dbg !39
  %row1319 = getelementptr i32, i32* %DOT1314, i32 %flat_idx1318, !dbg !39
  %stride1320 = mul i32 1, %dim1313, !dbg !39
  %flat_idx1321 = mul i32 2, %stride1320, !dbg !39
  %row1322 = getelementptr i32, i32* %row1319, i32 %flat_idx1321, !dbg !39
  %ACCESS1323 = getelementptr i32, i32* %row1322, i32 0, !dbg !39
  store i32 0, i32* %ACCESS1323, align 4, !dbg !39
  %gs1324 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !39
  %shapes1325 = getelementptr %struct.Game, %struct.Game* %gs1324, i32 0, i32 1, !dbg !39
  %dim1326 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !39
  %dim1327 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !39
  %dim1328 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !39
  %DOT1329 = load i32*, i32** %shapes1325, align 8, !dbg !39
  %PJ1330 = load i32, i32* @PJ, align 4, !dbg !39
  %stride1331 = mul i32 1, %dim1327, !dbg !39
  %stride1332 = mul i32 %stride1331, %dim1328, !dbg !39
  %flat_idx1333 = mul i32 %PJ1330, %stride1332, !dbg !39
  %row1334 = getelementptr i32, i32* %DOT1329, i32 %flat_idx1333, !dbg !39
  %stride1335 = mul i32 1, %dim1328, !dbg !39
  %flat_idx1336 = mul i32 2, %stride1335, !dbg !39
  %row1337 = getelementptr i32, i32* %row1334, i32 %flat_idx1336, !dbg !39
  %ACCESS1338 = getelementptr i32, i32* %row1337, i32 1, !dbg !39
  store i32 14, i32* %ACCESS1338, align 4, !dbg !39
  %gs1339 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !39
  %shapes1340 = getelementptr %struct.Game, %struct.Game* %gs1339, i32 0, i32 1, !dbg !39
  %dim1341 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !39
  %dim1342 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !39
  %dim1343 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !39
  %DOT1344 = load i32*, i32** %shapes1340, align 8, !dbg !39
  %PJ1345 = load i32, i32* @PJ, align 4, !dbg !39
  %stride1346 = mul i32 1, %dim1342, !dbg !39
  %stride1347 = mul i32 %stride1346, %dim1343, !dbg !39
  %flat_idx1348 = mul i32 %PJ1345, %stride1347, !dbg !39
  %row1349 = getelementptr i32, i32* %DOT1344, i32 %flat_idx1348, !dbg !39
  %stride1350 = mul i32 1, %dim1343, !dbg !39
  %flat_idx1351 = mul i32 2, %stride1350, !dbg !39
  %row1352 = getelementptr i32, i32* %row1349, i32 %flat_idx1351, !dbg !39
  %ACCESS1353 = getelementptr i32, i32* %row1352, i32 2, !dbg !39
  store i32 2, i32* %ACCESS1353, align 4, !dbg !39
  %gs1354 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !39
  %shapes1355 = getelementptr %struct.Game, %struct.Game* %gs1354, i32 0, i32 1, !dbg !39
  %dim1356 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !39
  %dim1357 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !39
  %dim1358 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !39
  %DOT1359 = load i32*, i32** %shapes1355, align 8, !dbg !39
  %PJ1360 = load i32, i32* @PJ, align 4, !dbg !39
  %stride1361 = mul i32 1, %dim1357, !dbg !39
  %stride1362 = mul i32 %stride1361, %dim1358, !dbg !39
  %flat_idx1363 = mul i32 %PJ1360, %stride1362, !dbg !39
  %row1364 = getelementptr i32, i32* %DOT1359, i32 %flat_idx1363, !dbg !39
  %stride1365 = mul i32 1, %dim1358, !dbg !39
  %flat_idx1366 = mul i32 2, %stride1365, !dbg !39
  %row1367 = getelementptr i32, i32* %row1364, i32 %flat_idx1366, !dbg !39
  %ACCESS1368 = getelementptr i32, i32* %row1367, i32 3, !dbg !39
  store i32 0, i32* %ACCESS1368, align 4, !dbg !39
  %gs1369 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !40
  %shapes1370 = getelementptr %struct.Game, %struct.Game* %gs1369, i32 0, i32 1, !dbg !40
  %dim1371 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !40
  %dim1372 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !40
  %dim1373 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !40
  %DOT1374 = load i32*, i32** %shapes1370, align 8, !dbg !40
  %PJ1375 = load i32, i32* @PJ, align 4, !dbg !40
  %stride1376 = mul i32 1, %dim1372, !dbg !40
  %stride1377 = mul i32 %stride1376, %dim1373, !dbg !40
  %flat_idx1378 = mul i32 %PJ1375, %stride1377, !dbg !40
  %row1379 = getelementptr i32, i32* %DOT1374, i32 %flat_idx1378, !dbg !40
  %stride1380 = mul i32 1, %dim1373, !dbg !40
  %flat_idx1381 = mul i32 3, %stride1380, !dbg !40
  %row1382 = getelementptr i32, i32* %row1379, i32 %flat_idx1381, !dbg !40
  %ACCESS1383 = getelementptr i32, i32* %row1382, i32 0, !dbg !40
  store i32 4, i32* %ACCESS1383, align 4, !dbg !40
  %gs1384 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !40
  %shapes1385 = getelementptr %struct.Game, %struct.Game* %gs1384, i32 0, i32 1, !dbg !40
  %dim1386 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !40
  %dim1387 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !40
  %dim1388 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !40
  %DOT1389 = load i32*, i32** %shapes1385, align 8, !dbg !40
  %PJ1390 = load i32, i32* @PJ, align 4, !dbg !40
  %stride1391 = mul i32 1, %dim1387, !dbg !40
  %stride1392 = mul i32 %stride1391, %dim1388, !dbg !40
  %flat_idx1393 = mul i32 %PJ1390, %stride1392, !dbg !40
  %row1394 = getelementptr i32, i32* %DOT1389, i32 %flat_idx1393, !dbg !40
  %stride1395 = mul i32 1, %dim1388, !dbg !40
  %flat_idx1396 = mul i32 3, %stride1395, !dbg !40
  %row1397 = getelementptr i32, i32* %row1394, i32 %flat_idx1396, !dbg !40
  %ACCESS1398 = getelementptr i32, i32* %row1397, i32 1, !dbg !40
  store i32 4, i32* %ACCESS1398, align 4, !dbg !40
  %gs1399 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !40
  %shapes1400 = getelementptr %struct.Game, %struct.Game* %gs1399, i32 0, i32 1, !dbg !40
  %dim1401 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !40
  %dim1402 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !40
  %dim1403 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !40
  %DOT1404 = load i32*, i32** %shapes1400, align 8, !dbg !40
  %PJ1405 = load i32, i32* @PJ, align 4, !dbg !40
  %stride1406 = mul i32 1, %dim1402, !dbg !40
  %stride1407 = mul i32 %stride1406, %dim1403, !dbg !40
  %flat_idx1408 = mul i32 %PJ1405, %stride1407, !dbg !40
  %row1409 = getelementptr i32, i32* %DOT1404, i32 %flat_idx1408, !dbg !40
  %stride1410 = mul i32 1, %dim1403, !dbg !40
  %flat_idx1411 = mul i32 3, %stride1410, !dbg !40
  %row1412 = getelementptr i32, i32* %row1409, i32 %flat_idx1411, !dbg !40
  %ACCESS1413 = getelementptr i32, i32* %row1412, i32 2, !dbg !40
  store i32 12, i32* %ACCESS1413, align 4, !dbg !40
  %gs1414 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !40
  %shapes1415 = getelementptr %struct.Game, %struct.Game* %gs1414, i32 0, i32 1, !dbg !40
  %dim1416 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !40
  %dim1417 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !40
  %dim1418 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !40
  %DOT1419 = load i32*, i32** %shapes1415, align 8, !dbg !40
  %PJ1420 = load i32, i32* @PJ, align 4, !dbg !40
  %stride1421 = mul i32 1, %dim1417, !dbg !40
  %stride1422 = mul i32 %stride1421, %dim1418, !dbg !40
  %flat_idx1423 = mul i32 %PJ1420, %stride1422, !dbg !40
  %row1424 = getelementptr i32, i32* %DOT1419, i32 %flat_idx1423, !dbg !40
  %stride1425 = mul i32 1, %dim1418, !dbg !40
  %flat_idx1426 = mul i32 3, %stride1425, !dbg !40
  %row1427 = getelementptr i32, i32* %row1424, i32 %flat_idx1426, !dbg !40
  %ACCESS1428 = getelementptr i32, i32* %row1427, i32 3, !dbg !40
  store i32 0, i32* %ACCESS1428, align 4, !dbg !40
  %gs1429 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !41
  %shapes1430 = getelementptr %struct.Game, %struct.Game* %gs1429, i32 0, i32 1, !dbg !41
  %dim1431 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !41
  %dim1432 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !41
  %dim1433 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !41
  %DOT1434 = load i32*, i32** %shapes1430, align 8, !dbg !41
  %PL = load i32, i32* @PL, align 4, !dbg !41
  %stride1435 = mul i32 1, %dim1432, !dbg !41
  %stride1436 = mul i32 %stride1435, %dim1433, !dbg !41
  %flat_idx1437 = mul i32 %PL, %stride1436, !dbg !41
  %row1438 = getelementptr i32, i32* %DOT1434, i32 %flat_idx1437, !dbg !41
  %stride1439 = mul i32 1, %dim1433, !dbg !41
  %flat_idx1440 = mul i32 0, %stride1439, !dbg !41
  %row1441 = getelementptr i32, i32* %row1438, i32 %flat_idx1440, !dbg !41
  %ACCESS1442 = getelementptr i32, i32* %row1441, i32 0, !dbg !41
  store i32 2, i32* %ACCESS1442, align 4, !dbg !41
  %gs1443 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !41
  %shapes1444 = getelementptr %struct.Game, %struct.Game* %gs1443, i32 0, i32 1, !dbg !41
  %dim1445 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !41
  %dim1446 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !41
  %dim1447 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !41
  %DOT1448 = load i32*, i32** %shapes1444, align 8, !dbg !41
  %PL1449 = load i32, i32* @PL, align 4, !dbg !41
  %stride1450 = mul i32 1, %dim1446, !dbg !41
  %stride1451 = mul i32 %stride1450, %dim1447, !dbg !41
  %flat_idx1452 = mul i32 %PL1449, %stride1451, !dbg !41
  %row1453 = getelementptr i32, i32* %DOT1448, i32 %flat_idx1452, !dbg !41
  %stride1454 = mul i32 1, %dim1447, !dbg !41
  %flat_idx1455 = mul i32 0, %stride1454, !dbg !41
  %row1456 = getelementptr i32, i32* %row1453, i32 %flat_idx1455, !dbg !41
  %ACCESS1457 = getelementptr i32, i32* %row1456, i32 1, !dbg !41
  store i32 14, i32* %ACCESS1457, align 4, !dbg !41
  %gs1458 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !41
  %shapes1459 = getelementptr %struct.Game, %struct.Game* %gs1458, i32 0, i32 1, !dbg !41
  %dim1460 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !41
  %dim1461 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !41
  %dim1462 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !41
  %DOT1463 = load i32*, i32** %shapes1459, align 8, !dbg !41
  %PL1464 = load i32, i32* @PL, align 4, !dbg !41
  %stride1465 = mul i32 1, %dim1461, !dbg !41
  %stride1466 = mul i32 %stride1465, %dim1462, !dbg !41
  %flat_idx1467 = mul i32 %PL1464, %stride1466, !dbg !41
  %row1468 = getelementptr i32, i32* %DOT1463, i32 %flat_idx1467, !dbg !41
  %stride1469 = mul i32 1, %dim1462, !dbg !41
  %flat_idx1470 = mul i32 0, %stride1469, !dbg !41
  %row1471 = getelementptr i32, i32* %row1468, i32 %flat_idx1470, !dbg !41
  %ACCESS1472 = getelementptr i32, i32* %row1471, i32 2, !dbg !41
  store i32 0, i32* %ACCESS1472, align 4, !dbg !41
  %gs1473 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !41
  %shapes1474 = getelementptr %struct.Game, %struct.Game* %gs1473, i32 0, i32 1, !dbg !41
  %dim1475 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !41
  %dim1476 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !41
  %dim1477 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !41
  %DOT1478 = load i32*, i32** %shapes1474, align 8, !dbg !41
  %PL1479 = load i32, i32* @PL, align 4, !dbg !41
  %stride1480 = mul i32 1, %dim1476, !dbg !41
  %stride1481 = mul i32 %stride1480, %dim1477, !dbg !41
  %flat_idx1482 = mul i32 %PL1479, %stride1481, !dbg !41
  %row1483 = getelementptr i32, i32* %DOT1478, i32 %flat_idx1482, !dbg !41
  %stride1484 = mul i32 1, %dim1477, !dbg !41
  %flat_idx1485 = mul i32 0, %stride1484, !dbg !41
  %row1486 = getelementptr i32, i32* %row1483, i32 %flat_idx1485, !dbg !41
  %ACCESS1487 = getelementptr i32, i32* %row1486, i32 3, !dbg !41
  store i32 0, i32* %ACCESS1487, align 4, !dbg !41
  %gs1488 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !42
  %shapes1489 = getelementptr %struct.Game, %struct.Game* %gs1488, i32 0, i32 1, !dbg !42
  %dim1490 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !42
  %dim1491 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !42
  %dim1492 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !42
  %DOT1493 = load i32*, i32** %shapes1489, align 8, !dbg !42
  %PL1494 = load i32, i32* @PL, align 4, !dbg !42
  %stride1495 = mul i32 1, %dim1491, !dbg !42
  %stride1496 = mul i32 %stride1495, %dim1492, !dbg !42
  %flat_idx1497 = mul i32 %PL1494, %stride1496, !dbg !42
  %row1498 = getelementptr i32, i32* %DOT1493, i32 %flat_idx1497, !dbg !42
  %stride1499 = mul i32 1, %dim1492, !dbg !42
  %flat_idx1500 = mul i32 1, %stride1499, !dbg !42
  %row1501 = getelementptr i32, i32* %row1498, i32 %flat_idx1500, !dbg !42
  %ACCESS1502 = getelementptr i32, i32* %row1501, i32 0, !dbg !42
  store i32 4, i32* %ACCESS1502, align 4, !dbg !42
  %gs1503 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !42
  %shapes1504 = getelementptr %struct.Game, %struct.Game* %gs1503, i32 0, i32 1, !dbg !42
  %dim1505 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !42
  %dim1506 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !42
  %dim1507 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !42
  %DOT1508 = load i32*, i32** %shapes1504, align 8, !dbg !42
  %PL1509 = load i32, i32* @PL, align 4, !dbg !42
  %stride1510 = mul i32 1, %dim1506, !dbg !42
  %stride1511 = mul i32 %stride1510, %dim1507, !dbg !42
  %flat_idx1512 = mul i32 %PL1509, %stride1511, !dbg !42
  %row1513 = getelementptr i32, i32* %DOT1508, i32 %flat_idx1512, !dbg !42
  %stride1514 = mul i32 1, %dim1507, !dbg !42
  %flat_idx1515 = mul i32 1, %stride1514, !dbg !42
  %row1516 = getelementptr i32, i32* %row1513, i32 %flat_idx1515, !dbg !42
  %ACCESS1517 = getelementptr i32, i32* %row1516, i32 1, !dbg !42
  store i32 4, i32* %ACCESS1517, align 4, !dbg !42
  %gs1518 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !42
  %shapes1519 = getelementptr %struct.Game, %struct.Game* %gs1518, i32 0, i32 1, !dbg !42
  %dim1520 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !42
  %dim1521 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !42
  %dim1522 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !42
  %DOT1523 = load i32*, i32** %shapes1519, align 8, !dbg !42
  %PL1524 = load i32, i32* @PL, align 4, !dbg !42
  %stride1525 = mul i32 1, %dim1521, !dbg !42
  %stride1526 = mul i32 %stride1525, %dim1522, !dbg !42
  %flat_idx1527 = mul i32 %PL1524, %stride1526, !dbg !42
  %row1528 = getelementptr i32, i32* %DOT1523, i32 %flat_idx1527, !dbg !42
  %stride1529 = mul i32 1, %dim1522, !dbg !42
  %flat_idx1530 = mul i32 1, %stride1529, !dbg !42
  %row1531 = getelementptr i32, i32* %row1528, i32 %flat_idx1530, !dbg !42
  %ACCESS1532 = getelementptr i32, i32* %row1531, i32 2, !dbg !42
  store i32 6, i32* %ACCESS1532, align 4, !dbg !42
  %gs1533 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !42
  %shapes1534 = getelementptr %struct.Game, %struct.Game* %gs1533, i32 0, i32 1, !dbg !42
  %dim1535 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !42
  %dim1536 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !42
  %dim1537 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !42
  %DOT1538 = load i32*, i32** %shapes1534, align 8, !dbg !42
  %PL1539 = load i32, i32* @PL, align 4, !dbg !42
  %stride1540 = mul i32 1, %dim1536, !dbg !42
  %stride1541 = mul i32 %stride1540, %dim1537, !dbg !42
  %flat_idx1542 = mul i32 %PL1539, %stride1541, !dbg !42
  %row1543 = getelementptr i32, i32* %DOT1538, i32 %flat_idx1542, !dbg !42
  %stride1544 = mul i32 1, %dim1537, !dbg !42
  %flat_idx1545 = mul i32 1, %stride1544, !dbg !42
  %row1546 = getelementptr i32, i32* %row1543, i32 %flat_idx1545, !dbg !42
  %ACCESS1547 = getelementptr i32, i32* %row1546, i32 3, !dbg !42
  store i32 0, i32* %ACCESS1547, align 4, !dbg !42
  %gs1548 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !43
  %shapes1549 = getelementptr %struct.Game, %struct.Game* %gs1548, i32 0, i32 1, !dbg !43
  %dim1550 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !43
  %dim1551 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !43
  %dim1552 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !43
  %DOT1553 = load i32*, i32** %shapes1549, align 8, !dbg !43
  %PL1554 = load i32, i32* @PL, align 4, !dbg !43
  %stride1555 = mul i32 1, %dim1551, !dbg !43
  %stride1556 = mul i32 %stride1555, %dim1552, !dbg !43
  %flat_idx1557 = mul i32 %PL1554, %stride1556, !dbg !43
  %row1558 = getelementptr i32, i32* %DOT1553, i32 %flat_idx1557, !dbg !43
  %stride1559 = mul i32 1, %dim1552, !dbg !43
  %flat_idx1560 = mul i32 2, %stride1559, !dbg !43
  %row1561 = getelementptr i32, i32* %row1558, i32 %flat_idx1560, !dbg !43
  %ACCESS1562 = getelementptr i32, i32* %row1561, i32 0, !dbg !43
  store i32 0, i32* %ACCESS1562, align 4, !dbg !43
  %gs1563 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !43
  %shapes1564 = getelementptr %struct.Game, %struct.Game* %gs1563, i32 0, i32 1, !dbg !43
  %dim1565 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !43
  %dim1566 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !43
  %dim1567 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !43
  %DOT1568 = load i32*, i32** %shapes1564, align 8, !dbg !43
  %PL1569 = load i32, i32* @PL, align 4, !dbg !43
  %stride1570 = mul i32 1, %dim1566, !dbg !43
  %stride1571 = mul i32 %stride1570, %dim1567, !dbg !43
  %flat_idx1572 = mul i32 %PL1569, %stride1571, !dbg !43
  %row1573 = getelementptr i32, i32* %DOT1568, i32 %flat_idx1572, !dbg !43
  %stride1574 = mul i32 1, %dim1567, !dbg !43
  %flat_idx1575 = mul i32 2, %stride1574, !dbg !43
  %row1576 = getelementptr i32, i32* %row1573, i32 %flat_idx1575, !dbg !43
  %ACCESS1577 = getelementptr i32, i32* %row1576, i32 1, !dbg !43
  store i32 14, i32* %ACCESS1577, align 4, !dbg !43
  %gs1578 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !43
  %shapes1579 = getelementptr %struct.Game, %struct.Game* %gs1578, i32 0, i32 1, !dbg !43
  %dim1580 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !43
  %dim1581 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !43
  %dim1582 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !43
  %DOT1583 = load i32*, i32** %shapes1579, align 8, !dbg !43
  %PL1584 = load i32, i32* @PL, align 4, !dbg !43
  %stride1585 = mul i32 1, %dim1581, !dbg !43
  %stride1586 = mul i32 %stride1585, %dim1582, !dbg !43
  %flat_idx1587 = mul i32 %PL1584, %stride1586, !dbg !43
  %row1588 = getelementptr i32, i32* %DOT1583, i32 %flat_idx1587, !dbg !43
  %stride1589 = mul i32 1, %dim1582, !dbg !43
  %flat_idx1590 = mul i32 2, %stride1589, !dbg !43
  %row1591 = getelementptr i32, i32* %row1588, i32 %flat_idx1590, !dbg !43
  %ACCESS1592 = getelementptr i32, i32* %row1591, i32 2, !dbg !43
  store i32 8, i32* %ACCESS1592, align 4, !dbg !43
  %gs1593 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !43
  %shapes1594 = getelementptr %struct.Game, %struct.Game* %gs1593, i32 0, i32 1, !dbg !43
  %dim1595 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !43
  %dim1596 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !43
  %dim1597 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !43
  %DOT1598 = load i32*, i32** %shapes1594, align 8, !dbg !43
  %PL1599 = load i32, i32* @PL, align 4, !dbg !43
  %stride1600 = mul i32 1, %dim1596, !dbg !43
  %stride1601 = mul i32 %stride1600, %dim1597, !dbg !43
  %flat_idx1602 = mul i32 %PL1599, %stride1601, !dbg !43
  %row1603 = getelementptr i32, i32* %DOT1598, i32 %flat_idx1602, !dbg !43
  %stride1604 = mul i32 1, %dim1597, !dbg !43
  %flat_idx1605 = mul i32 2, %stride1604, !dbg !43
  %row1606 = getelementptr i32, i32* %row1603, i32 %flat_idx1605, !dbg !43
  %ACCESS1607 = getelementptr i32, i32* %row1606, i32 3, !dbg !43
  store i32 0, i32* %ACCESS1607, align 4, !dbg !43
  %gs1608 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !44
  %shapes1609 = getelementptr %struct.Game, %struct.Game* %gs1608, i32 0, i32 1, !dbg !44
  %dim1610 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !44
  %dim1611 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !44
  %dim1612 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !44
  %DOT1613 = load i32*, i32** %shapes1609, align 8, !dbg !44
  %PL1614 = load i32, i32* @PL, align 4, !dbg !44
  %stride1615 = mul i32 1, %dim1611, !dbg !44
  %stride1616 = mul i32 %stride1615, %dim1612, !dbg !44
  %flat_idx1617 = mul i32 %PL1614, %stride1616, !dbg !44
  %row1618 = getelementptr i32, i32* %DOT1613, i32 %flat_idx1617, !dbg !44
  %stride1619 = mul i32 1, %dim1612, !dbg !44
  %flat_idx1620 = mul i32 3, %stride1619, !dbg !44
  %row1621 = getelementptr i32, i32* %row1618, i32 %flat_idx1620, !dbg !44
  %ACCESS1622 = getelementptr i32, i32* %row1621, i32 0, !dbg !44
  store i32 12, i32* %ACCESS1622, align 4, !dbg !44
  %gs1623 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !44
  %shapes1624 = getelementptr %struct.Game, %struct.Game* %gs1623, i32 0, i32 1, !dbg !44
  %dim1625 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !44
  %dim1626 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !44
  %dim1627 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !44
  %DOT1628 = load i32*, i32** %shapes1624, align 8, !dbg !44
  %PL1629 = load i32, i32* @PL, align 4, !dbg !44
  %stride1630 = mul i32 1, %dim1626, !dbg !44
  %stride1631 = mul i32 %stride1630, %dim1627, !dbg !44
  %flat_idx1632 = mul i32 %PL1629, %stride1631, !dbg !44
  %row1633 = getelementptr i32, i32* %DOT1628, i32 %flat_idx1632, !dbg !44
  %stride1634 = mul i32 1, %dim1627, !dbg !44
  %flat_idx1635 = mul i32 3, %stride1634, !dbg !44
  %row1636 = getelementptr i32, i32* %row1633, i32 %flat_idx1635, !dbg !44
  %ACCESS1637 = getelementptr i32, i32* %row1636, i32 1, !dbg !44
  store i32 4, i32* %ACCESS1637, align 4, !dbg !44
  %gs1638 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !44
  %shapes1639 = getelementptr %struct.Game, %struct.Game* %gs1638, i32 0, i32 1, !dbg !44
  %dim1640 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !44
  %dim1641 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !44
  %dim1642 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !44
  %DOT1643 = load i32*, i32** %shapes1639, align 8, !dbg !44
  %PL1644 = load i32, i32* @PL, align 4, !dbg !44
  %stride1645 = mul i32 1, %dim1641, !dbg !44
  %stride1646 = mul i32 %stride1645, %dim1642, !dbg !44
  %flat_idx1647 = mul i32 %PL1644, %stride1646, !dbg !44
  %row1648 = getelementptr i32, i32* %DOT1643, i32 %flat_idx1647, !dbg !44
  %stride1649 = mul i32 1, %dim1642, !dbg !44
  %flat_idx1650 = mul i32 3, %stride1649, !dbg !44
  %row1651 = getelementptr i32, i32* %row1648, i32 %flat_idx1650, !dbg !44
  %ACCESS1652 = getelementptr i32, i32* %row1651, i32 2, !dbg !44
  store i32 4, i32* %ACCESS1652, align 4, !dbg !44
  %gs1653 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !44
  %shapes1654 = getelementptr %struct.Game, %struct.Game* %gs1653, i32 0, i32 1, !dbg !44
  %dim1655 = load i32, i32* @Game.shapes.dim0, align 4, !dbg !44
  %dim1656 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !44
  %dim1657 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !44
  %DOT1658 = load i32*, i32** %shapes1654, align 8, !dbg !44
  %PL1659 = load i32, i32* @PL, align 4, !dbg !44
  %stride1660 = mul i32 1, %dim1656, !dbg !44
  %stride1661 = mul i32 %stride1660, %dim1657, !dbg !44
  %flat_idx1662 = mul i32 %PL1659, %stride1661, !dbg !44
  %row1663 = getelementptr i32, i32* %DOT1658, i32 %flat_idx1662, !dbg !44
  %stride1664 = mul i32 1, %dim1657, !dbg !44
  %flat_idx1665 = mul i32 3, %stride1664, !dbg !44
  %row1666 = getelementptr i32, i32* %row1663, i32 %flat_idx1665, !dbg !44
  %ACCESS1667 = getelementptr i32, i32* %row1666, i32 3, !dbg !44
  store i32 0, i32* %ACCESS1667, align 4, !dbg !44
  ret void, !dbg !44
}

define i1 @cell_occupied(%struct.Game* %gs, i32 %t, i32 %rot, i32 %r, i32 %c) !dbg !45 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !46
  %t2 = alloca i32, align 4, !dbg !46
  %rot3 = alloca i32, align 4, !dbg !46
  %r4 = alloca i32, align 4, !dbg !46
  %c5 = alloca i32, align 4, !dbg !46
  %mask = alloca i32, align 4, !dbg !46
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !46
  store i32 %t, i32* %t2, align 4, !dbg !46
  store i32 %rot, i32* %rot3, align 4, !dbg !46
  store i32 %r, i32* %r4, align 4, !dbg !46
  store i32 %c, i32* %c5, align 4, !dbg !46
  store i32 0, i32* %mask, align 4, !dbg !46
  %gs6 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !47
  %shapes = getelementptr %struct.Game, %struct.Game* %gs6, i32 0, i32 1, !dbg !47
  %dim = load i32, i32* @Game.shapes.dim0, align 4, !dbg !47
  %dim7 = load i32, i32* @Game.shapes.dim1, align 4, !dbg !47
  %dim8 = load i32, i32* @Game.shapes.dim2, align 4, !dbg !47
  %DOT = load i32*, i32** %shapes, align 8, !dbg !47
  %t9 = load i32, i32* %t2, align 4, !dbg !47
  %stride = mul i32 1, %dim7, !dbg !47
  %stride10 = mul i32 %stride, %dim8, !dbg !47
  %flat_idx = mul i32 %t9, %stride10, !dbg !47
  %row = getelementptr i32, i32* %DOT, i32 %flat_idx, !dbg !47
  %rot11 = load i32, i32* %rot3, align 4, !dbg !47
  %stride12 = mul i32 1, %dim8, !dbg !47
  %flat_idx13 = mul i32 %rot11, %stride12, !dbg !47
  %row14 = getelementptr i32, i32* %row, i32 %flat_idx13, !dbg !47
  %r15 = load i32, i32* %r4, align 4, !dbg !47
  %ACCESS = getelementptr i32, i32* %row14, i32 %r15, !dbg !47
  %c16 = load i32, i32* %c5, align 4, !dbg !47
  %SUB = sub i32 3, %c16, !dbg !47
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !47
  %RSHIFT = ashr i32 %ACC, %SUB, !dbg !47
  %BAND = and i32 %RSHIFT, 1, !dbg !47
  store i32 %BAND, i32* %mask, align 4, !dbg !47
  %mask17 = load i32, i32* %mask, align 4, !dbg !47
  %NEQ = icmp ne i32 %mask17, 0, !dbg !47
  ret i1 %NEQ, !dbg !47
}

define %struct.Color @piece_color(i32 %t) !dbg !48 {
entry:
  %t1 = alloca i32, align 4, !dbg !49
  store i32 %t, i32* %t1, align 4, !dbg !49
  br label %if.start, !dbg !49

if.start:                                         ; preds = %entry
  %t2 = load i32, i32* %t1, align 4, !dbg !49
  %PI = load i32, i32* @PI, align 4, !dbg !49
  %EQ = icmp eq i32 %t2, %PI, !dbg !49
  br i1 %EQ, label %if.then, label %if.end, !dbg !49

if.end:                                           ; preds = %if.start
  br label %if.start3, !dbg !50

if.then:                                          ; preds = %if.start
  %setColor = call %struct.Color @setColor(i32 102, i32 191, i32 255, i32 255), !dbg !50
  %tmp_struct = alloca %struct.Color, align 8, !dbg !50
  store %struct.Color %setColor, %struct.Color* %tmp_struct, align 1, !dbg !50
  ret %struct.Color %setColor, !dbg !50

if.start3:                                        ; preds = %if.end
  %t6 = load i32, i32* %t1, align 4, !dbg !50
  %PO = load i32, i32* @PO, align 4, !dbg !50
  %EQ7 = icmp eq i32 %t6, %PO, !dbg !50
  br i1 %EQ7, label %if.then5, label %if.end4, !dbg !50

if.end4:                                          ; preds = %if.start3
  br label %if.start10, !dbg !51

if.then5:                                         ; preds = %if.start3
  %setColor8 = call %struct.Color @setColor(i32 253, i32 249, i32 0, i32 255), !dbg !51
  %tmp_struct9 = alloca %struct.Color, align 8, !dbg !51
  store %struct.Color %setColor8, %struct.Color* %tmp_struct9, align 1, !dbg !51
  ret %struct.Color %setColor8, !dbg !51

if.start10:                                       ; preds = %if.end4
  %t13 = load i32, i32* %t1, align 4, !dbg !51
  %PT = load i32, i32* @PT, align 4, !dbg !51
  %EQ14 = icmp eq i32 %t13, %PT, !dbg !51
  br i1 %EQ14, label %if.then12, label %if.end11, !dbg !51

if.end11:                                         ; preds = %if.start10
  br label %if.start17, !dbg !52

if.then12:                                        ; preds = %if.start10
  %setColor15 = call %struct.Color @setColor(i32 200, i32 122, i32 255, i32 255), !dbg !52
  %tmp_struct16 = alloca %struct.Color, align 8, !dbg !52
  store %struct.Color %setColor15, %struct.Color* %tmp_struct16, align 1, !dbg !52
  ret %struct.Color %setColor15, !dbg !52

if.start17:                                       ; preds = %if.end11
  %t20 = load i32, i32* %t1, align 4, !dbg !52
  %PS = load i32, i32* @PS, align 4, !dbg !52
  %EQ21 = icmp eq i32 %t20, %PS, !dbg !52
  br i1 %EQ21, label %if.then19, label %if.end18, !dbg !52

if.end18:                                         ; preds = %if.start17
  br label %if.start24, !dbg !53

if.then19:                                        ; preds = %if.start17
  %setColor22 = call %struct.Color @setColor(i32 0, i32 228, i32 48, i32 255), !dbg !53
  %tmp_struct23 = alloca %struct.Color, align 8, !dbg !53
  store %struct.Color %setColor22, %struct.Color* %tmp_struct23, align 1, !dbg !53
  ret %struct.Color %setColor22, !dbg !53

if.start24:                                       ; preds = %if.end18
  %t27 = load i32, i32* %t1, align 4, !dbg !53
  %PZ = load i32, i32* @PZ, align 4, !dbg !53
  %EQ28 = icmp eq i32 %t27, %PZ, !dbg !53
  br i1 %EQ28, label %if.then26, label %if.end25, !dbg !53

if.end25:                                         ; preds = %if.start24
  br label %if.start31, !dbg !54

if.then26:                                        ; preds = %if.start24
  %setColor29 = call %struct.Color @setColor(i32 230, i32 41, i32 55, i32 255), !dbg !54
  %tmp_struct30 = alloca %struct.Color, align 8, !dbg !54
  store %struct.Color %setColor29, %struct.Color* %tmp_struct30, align 1, !dbg !54
  ret %struct.Color %setColor29, !dbg !54

if.start31:                                       ; preds = %if.end25
  %t34 = load i32, i32* %t1, align 4, !dbg !54
  %PJ = load i32, i32* @PJ, align 4, !dbg !54
  %EQ35 = icmp eq i32 %t34, %PJ, !dbg !54
  br i1 %EQ35, label %if.then33, label %if.end32, !dbg !54

if.end32:                                         ; preds = %if.start31
  %setColor38 = call %struct.Color @setColor(i32 255, i32 161, i32 0, i32 255), !dbg !55
  %tmp_struct39 = alloca %struct.Color, align 8, !dbg !55
  store %struct.Color %setColor38, %struct.Color* %tmp_struct39, align 1, !dbg !55
  ret %struct.Color %setColor38, !dbg !55

if.then33:                                        ; preds = %if.start31
  %setColor36 = call %struct.Color @setColor(i32 0, i32 121, i32 241, i32 255), !dbg !56
  %tmp_struct37 = alloca %struct.Color, align 8, !dbg !56
  store %struct.Color %setColor36, %struct.Color* %tmp_struct37, align 1, !dbg !56
  ret %struct.Color %setColor36, !dbg !56
}

define %struct.Piece @spawn_piece(i32 %t) !dbg !57 {
entry:
  %t1 = alloca i32, align 4, !dbg !58
  %p = alloca %struct.Piece, align 8, !dbg !58
  store i32 %t, i32* %t1, align 4, !dbg !58
  store %struct.Piece zeroinitializer, %struct.Piece* %p, align 4, !dbg !58
  %type = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 0, !dbg !58
  %t2 = load i32, i32* %t1, align 4, !dbg !58
  store i32 %t2, i32* %type, align 4, !dbg !58
  %rotation = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 1, !dbg !58
  store i32 0, i32* %rotation, align 4, !dbg !58
  %col = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 2, !dbg !58
  store i32 3, i32* %col, align 4, !dbg !58
  %row = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 3, !dbg !58
  store i32 0, i32* %row, align 4, !dbg !58
  %p3 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !58
  ret %struct.Piece %p3, !dbg !58
}

define i32 @random_piece() !dbg !59 {
entry:
  %PIECE_COUNT = load i32, i32* @PIECE_COUNT, align 4, !dbg !60
  %SUB = sub i32 %PIECE_COUNT, 1, !dbg !60
  %GetRandomValue = call i32 @GetRandomValue(i32 0, i32 %SUB), !dbg !60
  ret i32 %GetRandomValue, !dbg !60
}

define float @tick_interval(i32 %level) !dbg !61 {
entry:
  %level1 = alloca i32, align 4, !dbg !62
  %t = alloca float, align 4, !dbg !62
  store i32 %level, i32* %level1, align 4, !dbg !62
  store float 0.000000e+00, float* %t, align 4, !dbg !62
  %level2 = load i32, i32* %level1, align 4, !dbg !62
  %SUB = sub i32 %level2, 1, !dbg !62
  %as = sitofp i32 %SUB to float, !dbg !62
  %MUL = fmul float %as, 0x3FB1EB8520000000, !dbg !62
  %SUB3 = fsub float 0x3FE99999A0000000, %MUL, !dbg !62
  store float %SUB3, float* %t, align 4, !dbg !62
  br label %if.start, !dbg !62

if.start:                                         ; preds = %entry
  %t4 = load float, float* %t, align 4, !dbg !62
  %LT = fcmp olt float %t4, 0x3FA99999A0000000, !dbg !62
  br i1 %LT, label %if.then, label %if.end, !dbg !62

if.end:                                           ; preds = %if.start
  %t5 = load float, float* %t, align 4, !dbg !62
  ret float %t5, !dbg !62

if.then:                                          ; preds = %if.start
  ret float 0x3FA99999A0000000, !dbg !62
}

define i1 @is_valid(%struct.Game* %gs, %struct.Piece* %p) !dbg !63 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !64
  %p2 = alloca %struct.Piece*, align 8, !dbg !64
  %r = alloca i32, align 4, !dbg !64
  %c = alloca i32, align 4, !dbg !64
  %br = alloca i32, align 4, !dbg !64
  %bc = alloca i32, align 4, !dbg !64
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !64
  store %struct.Piece* %p, %struct.Piece** %p2, align 8, !dbg !64
  store i32 0, i32* %r, align 4, !dbg !64
  store i32 0, i32* %r, align 4, !dbg !64
  br label %while.start, !dbg !64

while.start:                                      ; preds = %while.end6, %entry
  %r3 = load i32, i32* %r, align 4, !dbg !64
  %LT = icmp slt i32 %r3, 4, !dbg !64
  br i1 %LT, label %while.then, label %while.end, !dbg !64

while.then:                                       ; preds = %while.start
  store i32 0, i32* %c, align 4, !dbg !64
  store i32 0, i32* %c, align 4, !dbg !64
  br label %while.start4, !dbg !64

while.end:                                        ; preds = %while.start
  ret i1 true, !dbg !65

while.start4:                                     ; preds = %if.end, %while.then
  %c7 = load i32, i32* %c, align 4, !dbg !64
  %LT8 = icmp slt i32 %c7, 4, !dbg !64
  br i1 %LT8, label %while.then5, label %while.end6, !dbg !64

while.then5:                                      ; preds = %while.start4
  br label %if.start, !dbg !64

while.end6:                                       ; preds = %while.start4
  %r46 = load i32, i32* %r, align 4, !dbg !65
  %ADD47 = add i32 %r46, 1, !dbg !65
  store i32 %ADD47, i32* %r, align 4, !dbg !65
  br label %while.start, !dbg !65

if.start:                                         ; preds = %while.then5
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !66
  %p9 = load %struct.Piece*, %struct.Piece** %p2, align 8, !dbg !66
  %type = getelementptr %struct.Piece, %struct.Piece* %p9, i32 0, i32 0, !dbg !66
  %DOT = load i32, i32* %type, align 4, !dbg !66
  %p10 = load %struct.Piece*, %struct.Piece** %p2, align 8, !dbg !66
  %rotation = getelementptr %struct.Piece, %struct.Piece* %p10, i32 0, i32 1, !dbg !66
  %DOT11 = load i32, i32* %rotation, align 4, !dbg !66
  %r12 = load i32, i32* %r, align 4, !dbg !66
  %c13 = load i32, i32* %c, align 4, !dbg !66
  %cell_occupied = call i1 @cell_occupied(%struct.Game* %ref_arg, i32 %DOT, i32 %DOT11, i32 %r12, i32 %c13), !dbg !66
  br i1 %cell_occupied, label %if.then, label %if.end, !dbg !66

if.end:                                           ; preds = %if.end36, %if.start
  %c44 = load i32, i32* %c, align 4, !dbg !65
  %ADD45 = add i32 %c44, 1, !dbg !65
  store i32 %ADD45, i32* %c, align 4, !dbg !65
  br label %while.start4, !dbg !65

if.then:                                          ; preds = %if.start
  store i32 0, i32* %br, align 4, !dbg !64
  %p14 = load %struct.Piece*, %struct.Piece** %p2, align 8, !dbg !64
  %row = getelementptr %struct.Piece, %struct.Piece* %p14, i32 0, i32 3, !dbg !64
  %DOT15 = load i32, i32* %row, align 4, !dbg !64
  %r16 = load i32, i32* %r, align 4, !dbg !64
  %ADD = add i32 %DOT15, %r16, !dbg !64
  store i32 %ADD, i32* %br, align 4, !dbg !64
  store i32 0, i32* %bc, align 4, !dbg !64
  %p17 = load %struct.Piece*, %struct.Piece** %p2, align 8, !dbg !64
  %col = getelementptr %struct.Piece, %struct.Piece* %p17, i32 0, i32 2, !dbg !64
  %DOT18 = load i32, i32* %col, align 4, !dbg !64
  %c19 = load i32, i32* %c, align 4, !dbg !64
  %ADD20 = add i32 %DOT18, %c19, !dbg !64
  store i32 %ADD20, i32* %bc, align 4, !dbg !64
  br label %if.start21, !dbg !64

if.start21:                                       ; preds = %if.then
  %br24 = load i32, i32* %br, align 4, !dbg !64
  %LT25 = icmp slt i32 %br24, 0, !dbg !64
  %br26 = load i32, i32* %br, align 4, !dbg !64
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !64
  %GE = icmp sge i32 %br26, %BOARD_ROWS, !dbg !64
  %OR = or i1 %LT25, %GE, !dbg !64
  br i1 %OR, label %if.then23, label %if.end22, !dbg !64

if.end22:                                         ; preds = %if.start21
  br label %if.start27, !dbg !64

if.then23:                                        ; preds = %if.start21
  ret i1 false, !dbg !64

if.start27:                                       ; preds = %if.end22
  %bc30 = load i32, i32* %bc, align 4, !dbg !64
  %LT31 = icmp slt i32 %bc30, 0, !dbg !64
  %bc32 = load i32, i32* %bc, align 4, !dbg !64
  %BOARD_COLS = load i32, i32* @BOARD_COLS, align 4, !dbg !64
  %GE33 = icmp sge i32 %bc32, %BOARD_COLS, !dbg !64
  %OR34 = or i1 %LT31, %GE33, !dbg !64
  br i1 %OR34, label %if.then29, label %if.end28, !dbg !64

if.end28:                                         ; preds = %if.start27
  br label %if.start35, !dbg !64

if.then29:                                        ; preds = %if.start27
  ret i1 false, !dbg !64

if.start35:                                       ; preds = %if.end28
  %gs38 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !65
  %board = getelementptr %struct.Game, %struct.Game* %gs38, i32 0, i32 0, !dbg !65
  %dim = load i32, i32* @Game.board.dim0, align 4, !dbg !65
  %dim39 = load i32, i32* @Game.board.dim1, align 4, !dbg !65
  %DOT40 = load i32*, i32** %board, align 8, !dbg !65
  %br41 = load i32, i32* %br, align 4, !dbg !65
  %stride = mul i32 1, %dim39, !dbg !65
  %flat_idx = mul i32 %br41, %stride, !dbg !65
  %row42 = getelementptr i32, i32* %DOT40, i32 %flat_idx, !dbg !65
  %bc43 = load i32, i32* %bc, align 4, !dbg !65
  %ACCESS = getelementptr i32, i32* %row42, i32 %bc43, !dbg !65
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !65
  %NEQ = icmp ne i32 %ACC, 0, !dbg !65
  br i1 %NEQ, label %if.then37, label %if.end36, !dbg !65

if.end36:                                         ; preds = %if.start35
  br label %if.end, !dbg !65

if.then37:                                        ; preds = %if.start35
  ret i1 false, !dbg !65
}

define i32 @clear_lines(%struct.Game* %gs) !dbg !67 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !68
  %cleared = alloca i32, align 4, !dbg !68
  %r = alloca i32, align 4, !dbg !68
  %full = alloca i1, align 1, !dbg !68
  %c = alloca i32, align 4, !dbg !68
  %rr = alloca i32, align 4, !dbg !68
  %cc = alloca i32, align 4, !dbg !68
  %cc55 = alloca i32, align 4, !dbg !68
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !68
  store i32 0, i32* %cleared, align 4, !dbg !68
  store i32 0, i32* %cleared, align 4, !dbg !68
  store i32 0, i32* %r, align 4, !dbg !68
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !68
  %SUB = sub i32 %BOARD_ROWS, 1, !dbg !68
  store i32 %SUB, i32* %r, align 4, !dbg !68
  br label %while.start, !dbg !68

while.start:                                      ; preds = %if.end13, %entry
  %r2 = load i32, i32* %r, align 4, !dbg !68
  %GE = icmp sge i32 %r2, 0, !dbg !68
  br i1 %GE, label %while.then, label %while.end, !dbg !68

while.then:                                       ; preds = %while.start
  store i1 false, i1* %full, align 1, !dbg !68
  store i1 true, i1* %full, align 1, !dbg !68
  store i32 0, i32* %c, align 4, !dbg !68
  store i32 0, i32* %c, align 4, !dbg !68
  br label %while.start3, !dbg !68

while.end:                                        ; preds = %while.start
  %cleared78 = load i32, i32* %cleared, align 4, !dbg !69
  ret i32 %cleared78, !dbg !69

while.start3:                                     ; preds = %if.end, %while.then
  %c6 = load i32, i32* %c, align 4, !dbg !68
  %BOARD_COLS = load i32, i32* @BOARD_COLS, align 4, !dbg !68
  %LT = icmp slt i32 %c6, %BOARD_COLS, !dbg !68
  br i1 %LT, label %while.then4, label %while.end5, !dbg !68

while.then4:                                      ; preds = %while.start3
  br label %if.start, !dbg !68

while.end5:                                       ; preds = %if.then, %while.start3
  br label %if.start12, !dbg !70

if.start:                                         ; preds = %while.then4
  %gs7 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !70
  %board = getelementptr %struct.Game, %struct.Game* %gs7, i32 0, i32 0, !dbg !70
  %dim = load i32, i32* @Game.board.dim0, align 4, !dbg !70
  %dim8 = load i32, i32* @Game.board.dim1, align 4, !dbg !70
  %DOT = load i32*, i32** %board, align 8, !dbg !70
  %r9 = load i32, i32* %r, align 4, !dbg !70
  %stride = mul i32 1, %dim8, !dbg !70
  %flat_idx = mul i32 %r9, %stride, !dbg !70
  %row = getelementptr i32, i32* %DOT, i32 %flat_idx, !dbg !70
  %c10 = load i32, i32* %c, align 4, !dbg !70
  %ACCESS = getelementptr i32, i32* %row, i32 %c10, !dbg !70
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !70
  %EQ = icmp eq i32 %ACC, 0, !dbg !70
  br i1 %EQ, label %if.then, label %if.end, !dbg !70

if.end:                                           ; preds = %if.start
  %c11 = load i32, i32* %c, align 4, !dbg !70
  %ADD = add i32 %c11, 1, !dbg !70
  store i32 %ADD, i32* %c, align 4, !dbg !70
  br label %while.start3, !dbg !70

if.then:                                          ; preds = %if.start
  store i1 false, i1* %full, align 1, !dbg !70
  br label %while.end5, !dbg !70

if.start12:                                       ; preds = %while.end5
  %full15 = load i1, i1* %full, align 1, !dbg !70
  br i1 %full15, label %if.then14, label %if.else, !dbg !70

if.end13:                                         ; preds = %if.else, %while.end58
  br label %while.start, !dbg !69

if.then14:                                        ; preds = %if.start12
  store i32 0, i32* %rr, align 4, !dbg !68
  %r16 = load i32, i32* %r, align 4, !dbg !68
  store i32 %r16, i32* %rr, align 4, !dbg !68
  br label %while.start17, !dbg !68

if.else:                                          ; preds = %if.start12
  %r76 = load i32, i32* %r, align 4, !dbg !69
  %SUB77 = sub i32 %r76, 1, !dbg !69
  store i32 %SUB77, i32* %r, align 4, !dbg !69
  br label %if.end13, !dbg !69

while.start17:                                    ; preds = %while.end23, %if.then14
  %rr20 = load i32, i32* %rr, align 4, !dbg !68
  %GT = icmp sgt i32 %rr20, 0, !dbg !68
  br i1 %GT, label %while.then18, label %while.end19, !dbg !68

while.then18:                                     ; preds = %while.start17
  store i32 0, i32* %cc, align 4, !dbg !68
  store i32 0, i32* %cc, align 4, !dbg !68
  br label %while.start21, !dbg !68

while.end19:                                      ; preds = %while.start17
  store i32 0, i32* %cc55, align 4, !dbg !68
  store i32 0, i32* %cc55, align 4, !dbg !68
  br label %while.start56, !dbg !68

while.start21:                                    ; preds = %while.then22, %while.then18
  %cc24 = load i32, i32* %cc, align 4, !dbg !68
  %BOARD_COLS25 = load i32, i32* @BOARD_COLS, align 4, !dbg !68
  %LT26 = icmp slt i32 %cc24, %BOARD_COLS25, !dbg !68
  br i1 %LT26, label %while.then22, label %while.end23, !dbg !68

while.then22:                                     ; preds = %while.start21
  %gs27 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !71
  %board28 = getelementptr %struct.Game, %struct.Game* %gs27, i32 0, i32 0, !dbg !71
  %dim29 = load i32, i32* @Game.board.dim0, align 4, !dbg !71
  %dim30 = load i32, i32* @Game.board.dim1, align 4, !dbg !71
  %DOT31 = load i32*, i32** %board28, align 8, !dbg !71
  %rr32 = load i32, i32* %rr, align 4, !dbg !71
  %stride33 = mul i32 1, %dim30, !dbg !71
  %flat_idx34 = mul i32 %rr32, %stride33, !dbg !71
  %row35 = getelementptr i32, i32* %DOT31, i32 %flat_idx34, !dbg !71
  %cc36 = load i32, i32* %cc, align 4, !dbg !71
  %ACCESS37 = getelementptr i32, i32* %row35, i32 %cc36, !dbg !71
  %gs38 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !71
  %board39 = getelementptr %struct.Game, %struct.Game* %gs38, i32 0, i32 0, !dbg !71
  %dim40 = load i32, i32* @Game.board.dim0, align 4, !dbg !71
  %dim41 = load i32, i32* @Game.board.dim1, align 4, !dbg !71
  %rr42 = load i32, i32* %rr, align 4, !dbg !71
  %SUB43 = sub i32 %rr42, 1, !dbg !71
  %DOT44 = load i32*, i32** %board39, align 8, !dbg !71
  %stride45 = mul i32 1, %dim41, !dbg !71
  %flat_idx46 = mul i32 %SUB43, %stride45, !dbg !71
  %row47 = getelementptr i32, i32* %DOT44, i32 %flat_idx46, !dbg !71
  %cc48 = load i32, i32* %cc, align 4, !dbg !71
  %ACCESS49 = getelementptr i32, i32* %row47, i32 %cc48, !dbg !71
  %ACC50 = load i32, i32* %ACCESS49, align 4, !dbg !71
  store i32 %ACC50, i32* %ACCESS37, align 4, !dbg !71
  %cc51 = load i32, i32* %cc, align 4, !dbg !71
  %ADD52 = add i32 %cc51, 1, !dbg !71
  store i32 %ADD52, i32* %cc, align 4, !dbg !71
  br label %while.start21, !dbg !71

while.end23:                                      ; preds = %while.start21
  %rr53 = load i32, i32* %rr, align 4, !dbg !71
  %SUB54 = sub i32 %rr53, 1, !dbg !71
  store i32 %SUB54, i32* %rr, align 4, !dbg !71
  br label %while.start17, !dbg !71

while.start56:                                    ; preds = %while.then57, %while.end19
  %cc59 = load i32, i32* %cc55, align 4, !dbg !68
  %BOARD_COLS60 = load i32, i32* @BOARD_COLS, align 4, !dbg !68
  %LT61 = icmp slt i32 %cc59, %BOARD_COLS60, !dbg !68
  br i1 %LT61, label %while.then57, label %while.end58, !dbg !68

while.then57:                                     ; preds = %while.start56
  %gs62 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !69
  %board63 = getelementptr %struct.Game, %struct.Game* %gs62, i32 0, i32 0, !dbg !69
  %dim64 = load i32, i32* @Game.board.dim0, align 4, !dbg !69
  %dim65 = load i32, i32* @Game.board.dim1, align 4, !dbg !69
  %DOT66 = load i32*, i32** %board63, align 8, !dbg !69
  %stride67 = mul i32 1, %dim65, !dbg !69
  %flat_idx68 = mul i32 0, %stride67, !dbg !69
  %row69 = getelementptr i32, i32* %DOT66, i32 %flat_idx68, !dbg !69
  %cc70 = load i32, i32* %cc55, align 4, !dbg !69
  %ACCESS71 = getelementptr i32, i32* %row69, i32 %cc70, !dbg !69
  store i32 0, i32* %ACCESS71, align 4, !dbg !69
  %cc72 = load i32, i32* %cc55, align 4, !dbg !69
  %ADD73 = add i32 %cc72, 1, !dbg !69
  store i32 %ADD73, i32* %cc55, align 4, !dbg !69
  br label %while.start56, !dbg !69

while.end58:                                      ; preds = %while.start56
  %cleared74 = load i32, i32* %cleared, align 4, !dbg !69
  %ADD75 = add i32 %cleared74, 1, !dbg !69
  store i32 %ADD75, i32* %cleared, align 4, !dbg !69
  br label %if.end13, !dbg !69
}

define void @update_score(%struct.Game* %gs, i32 %n) !dbg !72 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !73
  %n2 = alloca i32, align 4, !dbg !73
  %pts = alloca i32*, align 8, !dbg !73
  %count = alloca i32, align 4, !dbg !73
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !73
  store i32 %n, i32* %n2, align 4, !dbg !73
  store i32* null, i32** %pts, align 8, !dbg !73
  %array_lit = call i8* @calloc(i64 5, i64 4), !dbg !73
  %lit_data = bitcast i8* %array_lit to i32*, !dbg !73
  %lit_elem = getelementptr i32, i32* %lit_data, i32 0, !dbg !73
  store i32 0, i32* %lit_elem, align 4, !dbg !73
  %lit_elem3 = getelementptr i32, i32* %lit_data, i32 1, !dbg !73
  store i32 100, i32* %lit_elem3, align 4, !dbg !73
  %lit_elem4 = getelementptr i32, i32* %lit_data, i32 2, !dbg !73
  store i32 300, i32* %lit_elem4, align 4, !dbg !73
  %lit_elem5 = getelementptr i32, i32* %lit_data, i32 3, !dbg !73
  store i32 500, i32* %lit_elem5, align 4, !dbg !73
  %lit_elem6 = getelementptr i32, i32* %lit_data, i32 4, !dbg !73
  store i32 800, i32* %lit_elem6, align 4, !dbg !73
  store i32* %lit_data, i32** %pts, align 8, !dbg !73
  store i32 0, i32* %count, align 4, !dbg !73
  %n7 = load i32, i32* %n2, align 4, !dbg !73
  store i32 %n7, i32* %count, align 4, !dbg !73
  br label %if.start, !dbg !73

if.start:                                         ; preds = %entry
  %count8 = load i32, i32* %count, align 4, !dbg !73
  %GT = icmp sgt i32 %count8, 4, !dbg !73
  br i1 %GT, label %if.then, label %if.end, !dbg !73

if.end:                                           ; preds = %if.then, %if.start
  %gs9 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !73
  %score = getelementptr %struct.Game, %struct.Game* %gs9, i32 0, i32 4, !dbg !73
  %pts10 = load i32*, i32** %pts, align 8, !dbg !74
  %count11 = load i32, i32* %count, align 4, !dbg !74
  %ACCESS = getelementptr i32, i32* %pts10, i32 %count11, !dbg !74
  %gs12 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !74
  %level = getelementptr %struct.Game, %struct.Game* %gs12, i32 0, i32 5, !dbg !74
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !74
  %DOT = load i32, i32* %level, align 4, !dbg !74
  %MUL = mul i32 %ACC, %DOT, !dbg !74
  %DOT13 = load i32, i32* %score, align 4, !dbg !74
  %ADD = add i32 %DOT13, %MUL, !dbg !74
  store i32 %ADD, i32* %score, align 4, !dbg !74
  %gs14 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !74
  %lines = getelementptr %struct.Game, %struct.Game* %gs14, i32 0, i32 6, !dbg !74
  %DOT15 = load i32, i32* %lines, align 4, !dbg !74
  %n16 = load i32, i32* %n2, align 4, !dbg !74
  %ADD17 = add i32 %DOT15, %n16, !dbg !74
  store i32 %ADD17, i32* %lines, align 4, !dbg !74
  %gs18 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !74
  %level19 = getelementptr %struct.Game, %struct.Game* %gs18, i32 0, i32 5, !dbg !74
  %gs20 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !74
  %lines21 = getelementptr %struct.Game, %struct.Game* %gs20, i32 0, i32 6, !dbg !74
  %DOT22 = load i32, i32* %lines21, align 4, !dbg !74
  %DIV = sdiv i32 %DOT22, 10, !dbg !74
  %ADD23 = add i32 %DIV, 1, !dbg !74
  store i32 %ADD23, i32* %level19, align 4, !dbg !74
  %gs24 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !74
  %interval = getelementptr %struct.Game, %struct.Game* %gs24, i32 0, i32 8, !dbg !74
  %gs25 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !75
  %level26 = getelementptr %struct.Game, %struct.Game* %gs25, i32 0, i32 5, !dbg !75
  %DOT27 = load i32, i32* %level26, align 4, !dbg !75
  %tick_interval = call float @tick_interval(i32 %DOT27), !dbg !75
  store float %tick_interval, float* %interval, align 4, !dbg !75
  ret void, !dbg !75

if.then:                                          ; preds = %if.start
  store i32 4, i32* %count, align 4, !dbg !73
  br label %if.end, !dbg !73
}

define void @lock_piece(%struct.Game* %gs) !dbg !76 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !77
  %r = alloca i32, align 4, !dbg !77
  %c = alloca i32, align 4, !dbg !77
  %br = alloca i32, align 4, !dbg !77
  %bc = alloca i32, align 4, !dbg !77
  %n = alloca i32, align 4, !dbg !77
  %check = alloca %struct.Piece, align 8, !dbg !77
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !77
  store i32 0, i32* %r, align 4, !dbg !77
  store i32 0, i32* %r, align 4, !dbg !77
  br label %while.start, !dbg !77

while.start:                                      ; preds = %while.end5, %entry
  %r2 = load i32, i32* %r, align 4, !dbg !77
  %LT = icmp slt i32 %r2, 4, !dbg !77
  br i1 %LT, label %while.then, label %while.end, !dbg !77

while.then:                                       ; preds = %while.start
  store i32 0, i32* %c, align 4, !dbg !77
  store i32 0, i32* %c, align 4, !dbg !77
  br label %while.start3, !dbg !77

while.end:                                        ; preds = %while.start
  store i32 0, i32* %n, align 4, !dbg !77
  %ref_arg44 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !78
  %clear_lines = call i32 @clear_lines(%struct.Game* %ref_arg44), !dbg !78
  store i32 %clear_lines, i32* %n, align 4, !dbg !78
  br label %if.start45, !dbg !78

while.start3:                                     ; preds = %if.end, %while.then
  %c6 = load i32, i32* %c, align 4, !dbg !77
  %LT7 = icmp slt i32 %c6, 4, !dbg !77
  br i1 %LT7, label %while.then4, label %while.end5, !dbg !77

while.then4:                                      ; preds = %while.start3
  br label %if.start, !dbg !77

while.end5:                                       ; preds = %while.start3
  %r42 = load i32, i32* %r, align 4, !dbg !79
  %ADD43 = add i32 %r42, 1, !dbg !79
  store i32 %ADD43, i32* %r, align 4, !dbg !79
  br label %while.start, !dbg !79

if.start:                                         ; preds = %while.then4
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !80
  %gs8 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !80
  %current = getelementptr %struct.Game, %struct.Game* %gs8, i32 0, i32 2, !dbg !80
  %type = getelementptr %struct.Piece, %struct.Piece* %current, i32 0, i32 0, !dbg !80
  %DOT = load i32, i32* %type, align 4, !dbg !80
  %gs9 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !80
  %current10 = getelementptr %struct.Game, %struct.Game* %gs9, i32 0, i32 2, !dbg !80
  %rotation = getelementptr %struct.Piece, %struct.Piece* %current10, i32 0, i32 1, !dbg !80
  %DOT11 = load i32, i32* %rotation, align 4, !dbg !80
  %r12 = load i32, i32* %r, align 4, !dbg !80
  %c13 = load i32, i32* %c, align 4, !dbg !80
  %cell_occupied = call i1 @cell_occupied(%struct.Game* %ref_arg, i32 %DOT, i32 %DOT11, i32 %r12, i32 %c13), !dbg !80
  br i1 %cell_occupied, label %if.then, label %if.end, !dbg !80

if.end:                                           ; preds = %if.end24, %if.start
  %c40 = load i32, i32* %c, align 4, !dbg !79
  %ADD41 = add i32 %c40, 1, !dbg !79
  store i32 %ADD41, i32* %c, align 4, !dbg !79
  br label %while.start3, !dbg !79

if.then:                                          ; preds = %if.start
  store i32 0, i32* %br, align 4, !dbg !77
  %gs14 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !77
  %current15 = getelementptr %struct.Game, %struct.Game* %gs14, i32 0, i32 2, !dbg !77
  %row = getelementptr %struct.Piece, %struct.Piece* %current15, i32 0, i32 3, !dbg !77
  %DOT16 = load i32, i32* %row, align 4, !dbg !77
  %r17 = load i32, i32* %r, align 4, !dbg !77
  %ADD = add i32 %DOT16, %r17, !dbg !77
  store i32 %ADD, i32* %br, align 4, !dbg !77
  store i32 0, i32* %bc, align 4, !dbg !77
  %gs18 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !77
  %current19 = getelementptr %struct.Game, %struct.Game* %gs18, i32 0, i32 2, !dbg !77
  %col = getelementptr %struct.Piece, %struct.Piece* %current19, i32 0, i32 2, !dbg !77
  %DOT20 = load i32, i32* %col, align 4, !dbg !77
  %c21 = load i32, i32* %c, align 4, !dbg !77
  %ADD22 = add i32 %DOT20, %c21, !dbg !77
  store i32 %ADD22, i32* %bc, align 4, !dbg !77
  br label %if.start23, !dbg !77

if.start23:                                       ; preds = %if.then
  %br26 = load i32, i32* %br, align 4, !dbg !77
  %LT27 = icmp slt i32 %br26, 0, !dbg !77
  br i1 %LT27, label %if.then25, label %if.end24, !dbg !77

if.end24:                                         ; preds = %if.start23
  %gs29 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !79
  %board = getelementptr %struct.Game, %struct.Game* %gs29, i32 0, i32 0, !dbg !79
  %dim = load i32, i32* @Game.board.dim0, align 4, !dbg !79
  %dim30 = load i32, i32* @Game.board.dim1, align 4, !dbg !79
  %DOT31 = load i32*, i32** %board, align 8, !dbg !79
  %br32 = load i32, i32* %br, align 4, !dbg !79
  %stride = mul i32 1, %dim30, !dbg !79
  %flat_idx = mul i32 %br32, %stride, !dbg !79
  %row33 = getelementptr i32, i32* %DOT31, i32 %flat_idx, !dbg !79
  %bc34 = load i32, i32* %bc, align 4, !dbg !79
  %ACCESS = getelementptr i32, i32* %row33, i32 %bc34, !dbg !79
  %gs35 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !79
  %current36 = getelementptr %struct.Game, %struct.Game* %gs35, i32 0, i32 2, !dbg !79
  %type37 = getelementptr %struct.Piece, %struct.Piece* %current36, i32 0, i32 0, !dbg !79
  %DOT38 = load i32, i32* %type37, align 4, !dbg !79
  %ADD39 = add i32 %DOT38, 1, !dbg !79
  store i32 %ADD39, i32* %ACCESS, align 4, !dbg !79
  br label %if.end, !dbg !79

if.then25:                                        ; preds = %if.start23
  %gs28 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !77
  %over = getelementptr %struct.Game, %struct.Game* %gs28, i32 0, i32 9, !dbg !77
  store i1 true, i1* %over, align 1, !dbg !77
  ret void, !dbg !77

if.start45:                                       ; preds = %while.end
  %n48 = load i32, i32* %n, align 4, !dbg !78
  %GT = icmp sgt i32 %n48, 0, !dbg !78
  br i1 %GT, label %if.then47, label %if.end46, !dbg !78

if.end46:                                         ; preds = %if.then47, %if.start45
  %gs51 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !81
  %current52 = getelementptr %struct.Game, %struct.Game* %gs51, i32 0, i32 2, !dbg !81
  %gs53 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !81
  %next = getelementptr %struct.Game, %struct.Game* %gs53, i32 0, i32 3, !dbg !81
  %DOT54 = load %struct.Piece, %struct.Piece* %next, align 4, !dbg !81
  store %struct.Piece %DOT54, %struct.Piece* %current52, align 4, !dbg !81
  %gs55 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !81
  %next56 = getelementptr %struct.Game, %struct.Game* %gs55, i32 0, i32 3, !dbg !81
  %random_piece = call i32 @random_piece(), !dbg !82
  %spawn_piece = call %struct.Piece @spawn_piece(i32 %random_piece), !dbg !82
  %tmp_struct = alloca %struct.Piece, align 8, !dbg !82
  store %struct.Piece %spawn_piece, %struct.Piece* %tmp_struct, align 4, !dbg !82
  store %struct.Piece %spawn_piece, %struct.Piece* %next56, align 4, !dbg !82
  store %struct.Piece zeroinitializer, %struct.Piece* %check, align 4, !dbg !77
  %gs57 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !77
  %current58 = getelementptr %struct.Game, %struct.Game* %gs57, i32 0, i32 2, !dbg !77
  %DOT59 = load %struct.Piece, %struct.Piece* %current58, align 4, !dbg !77
  store %struct.Piece %DOT59, %struct.Piece* %check, align 4, !dbg !77
  br label %if.start60, !dbg !77

if.then47:                                        ; preds = %if.start45
  %ref_arg49 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !81
  %n50 = load i32, i32* %n, align 4, !dbg !81
  call void @update_score(%struct.Game* %ref_arg49, i32 %n50), !dbg !81
  br label %if.end46, !dbg !81

if.start60:                                       ; preds = %if.end46
  %ref_arg63 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !83
  %is_valid = call i1 @is_valid(%struct.Game* %ref_arg63, %struct.Piece* %check), !dbg !83
  %NOT = xor i1 %is_valid, true, !dbg !83
  br i1 %NOT, label %if.then62, label %if.end61, !dbg !83

if.end61:                                         ; preds = %if.then62, %if.start60
  call void @Piece.delete(%struct.Piece* %check), !dbg !83
  call void @Piece.delete(%struct.Piece* %tmp_struct), !dbg !83
  ret void, !dbg !83

if.then62:                                        ; preds = %if.start60
  %gs64 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !83
  %over65 = getelementptr %struct.Game, %struct.Game* %gs64, i32 0, i32 9, !dbg !83
  store i1 true, i1* %over65, align 1, !dbg !83
  br label %if.end61, !dbg !83
}

define i1 @move_left(%struct.Game* %gs) !dbg !84 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !85
  %p = alloca %struct.Piece, align 8, !dbg !85
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !85
  store %struct.Piece zeroinitializer, %struct.Piece* %p, align 4, !dbg !85
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !85
  %current = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 2, !dbg !85
  %DOT = load %struct.Piece, %struct.Piece* %current, align 4, !dbg !85
  store %struct.Piece %DOT, %struct.Piece* %p, align 4, !dbg !85
  %col = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 2, !dbg !85
  %DOT3 = load i32, i32* %col, align 4, !dbg !85
  %SUB = sub i32 %DOT3, 1, !dbg !85
  store i32 %SUB, i32* %col, align 4, !dbg !85
  br label %if.start, !dbg !85

if.start:                                         ; preds = %entry
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !86
  %is_valid = call i1 @is_valid(%struct.Game* %ref_arg, %struct.Piece* %p), !dbg !86
  %NOT = xor i1 %is_valid, true, !dbg !86
  br i1 %NOT, label %if.then, label %if.end, !dbg !86

if.end:                                           ; preds = %if.start
  %gs4 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !86
  %current5 = getelementptr %struct.Game, %struct.Game* %gs4, i32 0, i32 2, !dbg !86
  %p6 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !86
  store %struct.Piece %p6, %struct.Piece* %current5, align 4, !dbg !86
  call void @Piece.delete(%struct.Piece* %p), !dbg !86
  ret i1 true, !dbg !86

if.then:                                          ; preds = %if.start
  ret i1 false, !dbg !86
}

define i1 @move_right(%struct.Game* %gs) !dbg !87 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !88
  %p = alloca %struct.Piece, align 8, !dbg !88
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !88
  store %struct.Piece zeroinitializer, %struct.Piece* %p, align 4, !dbg !88
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !88
  %current = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 2, !dbg !88
  %DOT = load %struct.Piece, %struct.Piece* %current, align 4, !dbg !88
  store %struct.Piece %DOT, %struct.Piece* %p, align 4, !dbg !88
  %col = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 2, !dbg !88
  %DOT3 = load i32, i32* %col, align 4, !dbg !88
  %ADD = add i32 %DOT3, 1, !dbg !88
  store i32 %ADD, i32* %col, align 4, !dbg !88
  br label %if.start, !dbg !88

if.start:                                         ; preds = %entry
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !89
  %is_valid = call i1 @is_valid(%struct.Game* %ref_arg, %struct.Piece* %p), !dbg !89
  %NOT = xor i1 %is_valid, true, !dbg !89
  br i1 %NOT, label %if.then, label %if.end, !dbg !89

if.end:                                           ; preds = %if.start
  %gs4 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !89
  %current5 = getelementptr %struct.Game, %struct.Game* %gs4, i32 0, i32 2, !dbg !89
  %p6 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !89
  store %struct.Piece %p6, %struct.Piece* %current5, align 4, !dbg !89
  call void @Piece.delete(%struct.Piece* %p), !dbg !89
  ret i1 true, !dbg !89

if.then:                                          ; preds = %if.start
  ret i1 false, !dbg !89
}

define i1 @move_down(%struct.Game* %gs) !dbg !90 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !91
  %p = alloca %struct.Piece, align 8, !dbg !91
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !91
  store %struct.Piece zeroinitializer, %struct.Piece* %p, align 4, !dbg !91
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !91
  %current = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 2, !dbg !91
  %DOT = load %struct.Piece, %struct.Piece* %current, align 4, !dbg !91
  store %struct.Piece %DOT, %struct.Piece* %p, align 4, !dbg !91
  %row = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 3, !dbg !91
  %DOT3 = load i32, i32* %row, align 4, !dbg !91
  %ADD = add i32 %DOT3, 1, !dbg !91
  store i32 %ADD, i32* %row, align 4, !dbg !91
  br label %if.start, !dbg !91

if.start:                                         ; preds = %entry
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !92
  %is_valid = call i1 @is_valid(%struct.Game* %ref_arg, %struct.Piece* %p), !dbg !92
  %NOT = xor i1 %is_valid, true, !dbg !92
  br i1 %NOT, label %if.then, label %if.end, !dbg !92

if.end:                                           ; preds = %if.start
  %gs4 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !92
  %current5 = getelementptr %struct.Game, %struct.Game* %gs4, i32 0, i32 2, !dbg !92
  %p6 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !92
  store %struct.Piece %p6, %struct.Piece* %current5, align 4, !dbg !92
  call void @Piece.delete(%struct.Piece* %p), !dbg !92
  ret i1 true, !dbg !92

if.then:                                          ; preds = %if.start
  ret i1 false, !dbg !92
}

define void @rotate_cw(%struct.Game* %gs) !dbg !93 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !94
  %p = alloca %struct.Piece, align 8, !dbg !94
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !94
  store %struct.Piece zeroinitializer, %struct.Piece* %p, align 4, !dbg !94
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !94
  %current = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 2, !dbg !94
  %DOT = load %struct.Piece, %struct.Piece* %current, align 4, !dbg !94
  store %struct.Piece %DOT, %struct.Piece* %p, align 4, !dbg !94
  %rotation = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 1, !dbg !94
  %rotation3 = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 1, !dbg !94
  %DOT4 = load i32, i32* %rotation3, align 4, !dbg !94
  %ADD = add i32 %DOT4, 1, !dbg !94
  %MOD = srem i32 %ADD, 4, !dbg !94
  store i32 %MOD, i32* %rotation, align 4, !dbg !94
  br label %if.start, !dbg !94

if.start:                                         ; preds = %entry
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !95
  %is_valid = call i1 @is_valid(%struct.Game* %ref_arg, %struct.Piece* %p), !dbg !95
  br i1 %is_valid, label %if.then, label %if.end, !dbg !95

if.end:                                           ; preds = %if.start
  %col = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 2, !dbg !95
  %DOT8 = load i32, i32* %col, align 4, !dbg !95
  %SUB = sub i32 %DOT8, 1, !dbg !95
  store i32 %SUB, i32* %col, align 4, !dbg !95
  br label %if.start9, !dbg !95

if.then:                                          ; preds = %if.start
  %gs5 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !95
  %current6 = getelementptr %struct.Game, %struct.Game* %gs5, i32 0, i32 2, !dbg !95
  %p7 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !95
  store %struct.Piece %p7, %struct.Piece* %current6, align 4, !dbg !95
  ret void, !dbg !95

if.start9:                                        ; preds = %if.end
  %ref_arg12 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !96
  %is_valid13 = call i1 @is_valid(%struct.Game* %ref_arg12, %struct.Piece* %p), !dbg !96
  br i1 %is_valid13, label %if.then11, label %if.end10, !dbg !96

if.end10:                                         ; preds = %if.start9
  %col17 = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 2, !dbg !96
  %DOT18 = load i32, i32* %col17, align 4, !dbg !96
  %ADD19 = add i32 %DOT18, 2, !dbg !96
  store i32 %ADD19, i32* %col17, align 4, !dbg !96
  br label %if.start20, !dbg !96

if.then11:                                        ; preds = %if.start9
  %gs14 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !96
  %current15 = getelementptr %struct.Game, %struct.Game* %gs14, i32 0, i32 2, !dbg !96
  %p16 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !96
  store %struct.Piece %p16, %struct.Piece* %current15, align 4, !dbg !96
  ret void, !dbg !96

if.start20:                                       ; preds = %if.end10
  %ref_arg23 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !97
  %is_valid24 = call i1 @is_valid(%struct.Game* %ref_arg23, %struct.Piece* %p), !dbg !97
  br i1 %is_valid24, label %if.then22, label %if.end21, !dbg !97

if.end21:                                         ; preds = %if.start20
  %col28 = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 2, !dbg !97
  %DOT29 = load i32, i32* %col28, align 4, !dbg !97
  %SUB30 = sub i32 %DOT29, 3, !dbg !97
  store i32 %SUB30, i32* %col28, align 4, !dbg !97
  br label %if.start31, !dbg !97

if.then22:                                        ; preds = %if.start20
  %gs25 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !97
  %current26 = getelementptr %struct.Game, %struct.Game* %gs25, i32 0, i32 2, !dbg !97
  %p27 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !97
  store %struct.Piece %p27, %struct.Piece* %current26, align 4, !dbg !97
  ret void, !dbg !97

if.start31:                                       ; preds = %if.end21
  %ref_arg34 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !98
  %is_valid35 = call i1 @is_valid(%struct.Game* %ref_arg34, %struct.Piece* %p), !dbg !98
  br i1 %is_valid35, label %if.then33, label %if.end32, !dbg !98

if.end32:                                         ; preds = %if.then33, %if.start31
  call void @Piece.delete(%struct.Piece* %p), !dbg !98
  ret void, !dbg !98

if.then33:                                        ; preds = %if.start31
  %gs36 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !98
  %current37 = getelementptr %struct.Game, %struct.Game* %gs36, i32 0, i32 2, !dbg !98
  %p38 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !98
  store %struct.Piece %p38, %struct.Piece* %current37, align 4, !dbg !98
  br label %if.end32, !dbg !98
}

define void @hard_drop(%struct.Game* %gs) !dbg !99 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !100
  %dropped = alloca i32, align 4, !dbg !100
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !100
  store i32 0, i32* %dropped, align 4, !dbg !100
  store i32 0, i32* %dropped, align 4, !dbg !100
  br label %while.start, !dbg !100

while.start:                                      ; preds = %while.then, %entry
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !101
  %move_down = call i1 @move_down(%struct.Game* %ref_arg), !dbg !101
  br i1 %move_down, label %while.then, label %while.end, !dbg !101

while.then:                                       ; preds = %while.start
  %dropped2 = load i32, i32* %dropped, align 4, !dbg !101
  %ADD = add i32 %dropped2, 1, !dbg !101
  store i32 %ADD, i32* %dropped, align 4, !dbg !101
  br label %while.start, !dbg !101

while.end:                                        ; preds = %while.start
  %gs3 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !101
  %score = getelementptr %struct.Game, %struct.Game* %gs3, i32 0, i32 4, !dbg !101
  %dropped4 = load i32, i32* %dropped, align 4, !dbg !101
  %MUL = mul i32 %dropped4, 2, !dbg !101
  %DOT = load i32, i32* %score, align 4, !dbg !101
  %ADD5 = add i32 %DOT, %MUL, !dbg !101
  store i32 %ADD5, i32* %score, align 4, !dbg !101
  %ref_arg6 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !102
  call void @lock_piece(%struct.Game* %ref_arg6), !dbg !102
  ret void, !dbg !102
}

define i32 @ghost_row(%struct.Game* %gs) !dbg !103 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !104
  %p = alloca %struct.Piece, align 8, !dbg !104
  %nxt = alloca %struct.Piece, align 8, !dbg !104
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !104
  store %struct.Piece zeroinitializer, %struct.Piece* %p, align 4, !dbg !104
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !104
  %current = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 2, !dbg !104
  %DOT = load %struct.Piece, %struct.Piece* %current, align 4, !dbg !104
  store %struct.Piece %DOT, %struct.Piece* %p, align 4, !dbg !104
  br label %while.start, !dbg !104

while.start:                                      ; preds = %if.end, %entry
  br i1 true, label %while.then, label %while.end, !dbg !104

while.then:                                       ; preds = %while.start
  store %struct.Piece zeroinitializer, %struct.Piece* %nxt, align 4, !dbg !104
  %p3 = load %struct.Piece, %struct.Piece* %p, align 4, !dbg !104
  store %struct.Piece %p3, %struct.Piece* %nxt, align 4, !dbg !104
  %row = getelementptr %struct.Piece, %struct.Piece* %nxt, i32 0, i32 3, !dbg !104
  %DOT4 = load i32, i32* %row, align 4, !dbg !104
  %ADD = add i32 %DOT4, 1, !dbg !104
  store i32 %ADD, i32* %row, align 4, !dbg !104
  br label %if.start, !dbg !104

while.end:                                        ; preds = %if.then, %while.start
  call void @Piece.delete(%struct.Piece* %p), !dbg !105
  %row6 = getelementptr %struct.Piece, %struct.Piece* %p, i32 0, i32 3, !dbg !105
  %DOT7 = load i32, i32* %row6, align 4, !dbg !105
  ret i32 %DOT7, !dbg !105

if.start:                                         ; preds = %while.then
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !105
  %is_valid = call i1 @is_valid(%struct.Game* %ref_arg, %struct.Piece* %nxt), !dbg !105
  %NOT = xor i1 %is_valid, true, !dbg !105
  br i1 %NOT, label %if.then, label %if.end, !dbg !105

if.end:                                           ; preds = %if.start
  %nxt5 = load %struct.Piece, %struct.Piece* %nxt, align 4, !dbg !105
  store %struct.Piece %nxt5, %struct.Piece* %p, align 4, !dbg !105
  br label %while.start, !dbg !105

if.then:                                          ; preds = %if.start
  br label %while.end, !dbg !105
}

define void @handle_input(%struct.Game* %gs) !dbg !106 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !107
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !107
  br label %if.start, !dbg !107

if.start:                                         ; preds = %entry
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !107
  %over = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 9, !dbg !107
  %DOT = load i1, i1* %over, align 1, !dbg !107
  br i1 %DOT, label %if.then, label %if.end, !dbg !107

if.end:                                           ; preds = %if.start
  br label %if.start3, !dbg !107

if.then:                                          ; preds = %if.start
  ret void, !dbg !107

if.start3:                                        ; preds = %if.end
  %KEY_P = load i32, i32* @KEY_P, align 4, !dbg !108
  %IsKeyPressed = call i1 @IsKeyPressed(i32 %KEY_P), !dbg !108
  br i1 %IsKeyPressed, label %if.then5, label %if.end4, !dbg !108

if.end4:                                          ; preds = %if.start3
  br label %if.start10, !dbg !108

if.then5:                                         ; preds = %if.start3
  %gs6 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !108
  %paused = getelementptr %struct.Game, %struct.Game* %gs6, i32 0, i32 10, !dbg !108
  %gs7 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !108
  %paused8 = getelementptr %struct.Game, %struct.Game* %gs7, i32 0, i32 10, !dbg !108
  %DOT9 = load i1, i1* %paused8, align 1, !dbg !108
  %NOT = xor i1 %DOT9, true, !dbg !108
  store i1 %NOT, i1* %paused, align 1, !dbg !108
  ret void, !dbg !108

if.start10:                                       ; preds = %if.end4
  %gs13 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !108
  %paused14 = getelementptr %struct.Game, %struct.Game* %gs13, i32 0, i32 10, !dbg !108
  %DOT15 = load i1, i1* %paused14, align 1, !dbg !108
  br i1 %DOT15, label %if.then12, label %if.end11, !dbg !108

if.end11:                                         ; preds = %if.start10
  br label %if.start16, !dbg !108

if.then12:                                        ; preds = %if.start10
  ret void, !dbg !108

if.start16:                                       ; preds = %if.end11
  %KEY_LEFT = load i32, i32* @KEY_LEFT, align 4, !dbg !109
  %IsKeyPressed19 = call i1 @IsKeyPressed(i32 %KEY_LEFT), !dbg !109
  br i1 %IsKeyPressed19, label %if.then18, label %if.end17, !dbg !109

if.end17:                                         ; preds = %if.then18, %if.start16
  br label %if.start20, !dbg !109

if.then18:                                        ; preds = %if.start16
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !109
  %move_left = call i1 @move_left(%struct.Game* %ref_arg), !dbg !109
  br label %if.end17, !dbg !109

if.start20:                                       ; preds = %if.end17
  %KEY_RIGHT = load i32, i32* @KEY_RIGHT, align 4, !dbg !110
  %IsKeyPressed23 = call i1 @IsKeyPressed(i32 %KEY_RIGHT), !dbg !110
  br i1 %IsKeyPressed23, label %if.then22, label %if.end21, !dbg !110

if.end21:                                         ; preds = %if.then22, %if.start20
  br label %if.start25, !dbg !110

if.then22:                                        ; preds = %if.start20
  %ref_arg24 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !110
  %move_right = call i1 @move_right(%struct.Game* %ref_arg24), !dbg !110
  br label %if.end21, !dbg !110

if.start25:                                       ; preds = %if.end21
  %KEY_UP = load i32, i32* @KEY_UP, align 4, !dbg !111
  %IsKeyPressed28 = call i1 @IsKeyPressed(i32 %KEY_UP), !dbg !111
  br i1 %IsKeyPressed28, label %if.then27, label %if.end26, !dbg !111

if.end26:                                         ; preds = %if.then27, %if.start25
  br label %if.start30, !dbg !111

if.then27:                                        ; preds = %if.start25
  %ref_arg29 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !111
  call void @rotate_cw(%struct.Game* %ref_arg29), !dbg !111
  br label %if.end26, !dbg !111

if.start30:                                       ; preds = %if.end26
  %KEY_SPACE = load i32, i32* @KEY_SPACE, align 4, !dbg !112
  %IsKeyPressed33 = call i1 @IsKeyPressed(i32 %KEY_SPACE), !dbg !112
  br i1 %IsKeyPressed33, label %if.then32, label %if.end31, !dbg !112

if.end31:                                         ; preds = %if.then32, %if.start30
  br label %if.start35, !dbg !112

if.then32:                                        ; preds = %if.start30
  %ref_arg34 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !112
  call void @rotate_cw(%struct.Game* %ref_arg34), !dbg !112
  br label %if.end31, !dbg !112

if.start35:                                       ; preds = %if.end31
  %KEY_DOWN = load i32, i32* @KEY_DOWN, align 4, !dbg !113
  %IsKeyPressed38 = call i1 @IsKeyPressed(i32 %KEY_DOWN), !dbg !113
  br i1 %IsKeyPressed38, label %if.then37, label %if.end36, !dbg !113

if.end36:                                         ; preds = %if.then37, %if.start35
  ret void, !dbg !113

if.then37:                                        ; preds = %if.start35
  %ref_arg39 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !113
  call void @hard_drop(%struct.Game* %ref_arg39), !dbg !113
  br label %if.end36, !dbg !113
}

define void @init_game(%struct.Game* %gs) !dbg !114 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !115
  %r = alloca i32, align 4, !dbg !115
  %c = alloca i32, align 4, !dbg !115
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !115
  store i32 0, i32* %r, align 4, !dbg !115
  store i32 0, i32* %r, align 4, !dbg !115
  br label %while.start, !dbg !115

while.start:                                      ; preds = %while.end5, %entry
  %r2 = load i32, i32* %r, align 4, !dbg !115
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !115
  %LT = icmp slt i32 %r2, %BOARD_ROWS, !dbg !115
  br i1 %LT, label %while.then, label %while.end, !dbg !115

while.then:                                       ; preds = %while.start
  store i32 0, i32* %c, align 4, !dbg !115
  store i32 0, i32* %c, align 4, !dbg !115
  br label %while.start3, !dbg !115

while.end:                                        ; preds = %while.start
  %gs15 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !116
  %score = getelementptr %struct.Game, %struct.Game* %gs15, i32 0, i32 4, !dbg !116
  store i32 0, i32* %score, align 4, !dbg !116
  %gs16 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !116
  %level = getelementptr %struct.Game, %struct.Game* %gs16, i32 0, i32 5, !dbg !116
  store i32 1, i32* %level, align 4, !dbg !116
  %gs17 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !116
  %lines = getelementptr %struct.Game, %struct.Game* %gs17, i32 0, i32 6, !dbg !116
  store i32 0, i32* %lines, align 4, !dbg !116
  %gs18 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !116
  %timer = getelementptr %struct.Game, %struct.Game* %gs18, i32 0, i32 7, !dbg !116
  store float 0.000000e+00, float* %timer, align 4, !dbg !116
  %gs19 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !116
  %interval = getelementptr %struct.Game, %struct.Game* %gs19, i32 0, i32 8, !dbg !116
  %tick_interval = call float @tick_interval(i32 1), !dbg !117
  store float %tick_interval, float* %interval, align 4, !dbg !117
  %gs20 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !117
  %over = getelementptr %struct.Game, %struct.Game* %gs20, i32 0, i32 9, !dbg !117
  store i1 false, i1* %over, align 1, !dbg !117
  %gs21 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !117
  %paused = getelementptr %struct.Game, %struct.Game* %gs21, i32 0, i32 10, !dbg !117
  store i1 false, i1* %paused, align 1, !dbg !117
  %gs22 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !117
  %current = getelementptr %struct.Game, %struct.Game* %gs22, i32 0, i32 2, !dbg !117
  %random_piece = call i32 @random_piece(), !dbg !118
  %spawn_piece = call %struct.Piece @spawn_piece(i32 %random_piece), !dbg !118
  %tmp_struct = alloca %struct.Piece, align 8, !dbg !118
  store %struct.Piece %spawn_piece, %struct.Piece* %tmp_struct, align 4, !dbg !118
  store %struct.Piece %spawn_piece, %struct.Piece* %current, align 4, !dbg !118
  %gs23 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !118
  %next = getelementptr %struct.Game, %struct.Game* %gs23, i32 0, i32 3, !dbg !118
  %random_piece24 = call i32 @random_piece(), !dbg !119
  %spawn_piece25 = call %struct.Piece @spawn_piece(i32 %random_piece24), !dbg !119
  %tmp_struct26 = alloca %struct.Piece, align 8, !dbg !119
  store %struct.Piece %spawn_piece25, %struct.Piece* %tmp_struct26, align 4, !dbg !119
  store %struct.Piece %spawn_piece25, %struct.Piece* %next, align 4, !dbg !119
  call void @Piece.delete(%struct.Piece* %tmp_struct), !dbg !119
  call void @Piece.delete(%struct.Piece* %tmp_struct26), !dbg !119
  ret void, !dbg !119

while.start3:                                     ; preds = %while.then4, %while.then
  %c6 = load i32, i32* %c, align 4, !dbg !115
  %BOARD_COLS = load i32, i32* @BOARD_COLS, align 4, !dbg !115
  %LT7 = icmp slt i32 %c6, %BOARD_COLS, !dbg !115
  br i1 %LT7, label %while.then4, label %while.end5, !dbg !115

while.then4:                                      ; preds = %while.start3
  %gs8 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !116
  %board = getelementptr %struct.Game, %struct.Game* %gs8, i32 0, i32 0, !dbg !116
  %dim = load i32, i32* @Game.board.dim0, align 4, !dbg !116
  %dim9 = load i32, i32* @Game.board.dim1, align 4, !dbg !116
  %DOT = load i32*, i32** %board, align 8, !dbg !116
  %r10 = load i32, i32* %r, align 4, !dbg !116
  %stride = mul i32 1, %dim9, !dbg !116
  %flat_idx = mul i32 %r10, %stride, !dbg !116
  %row = getelementptr i32, i32* %DOT, i32 %flat_idx, !dbg !116
  %c11 = load i32, i32* %c, align 4, !dbg !116
  %ACCESS = getelementptr i32, i32* %row, i32 %c11, !dbg !116
  store i32 0, i32* %ACCESS, align 4, !dbg !116
  %c12 = load i32, i32* %c, align 4, !dbg !116
  %ADD = add i32 %c12, 1, !dbg !116
  store i32 %ADD, i32* %c, align 4, !dbg !116
  br label %while.start3, !dbg !116

while.end5:                                       ; preds = %while.start3
  %r13 = load i32, i32* %r, align 4, !dbg !116
  %ADD14 = add i32 %r13, 1, !dbg !116
  store i32 %ADD14, i32* %r, align 4, !dbg !116
  br label %while.start, !dbg !116
}

define void @update_game(%struct.Game* %gs, float %dt) !dbg !120 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !121
  %dt2 = alloca float, align 4, !dbg !121
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !121
  store float %dt, float* %dt2, align 4, !dbg !121
  %gs3 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !121
  %timer = getelementptr %struct.Game, %struct.Game* %gs3, i32 0, i32 7, !dbg !121
  %gs4 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !121
  %timer5 = getelementptr %struct.Game, %struct.Game* %gs4, i32 0, i32 7, !dbg !121
  %DOT = load float, float* %timer5, align 4, !dbg !121
  %dt6 = load float, float* %dt2, align 4, !dbg !121
  %ADD = fadd float %DOT, %dt6, !dbg !121
  store float %ADD, float* %timer, align 4, !dbg !121
  br label %if.start, !dbg !121

if.start:                                         ; preds = %entry
  %gs7 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !121
  %timer8 = getelementptr %struct.Game, %struct.Game* %gs7, i32 0, i32 7, !dbg !121
  %gs9 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !121
  %interval = getelementptr %struct.Game, %struct.Game* %gs9, i32 0, i32 8, !dbg !121
  %DOT10 = load float, float* %timer8, align 4, !dbg !121
  %DOT11 = load float, float* %interval, align 4, !dbg !121
  %GE = fcmp oge float %DOT10, %DOT11, !dbg !121
  br i1 %GE, label %if.then, label %if.end, !dbg !121

if.end:                                           ; preds = %if.end15, %if.start
  ret void, !dbg !122

if.then:                                          ; preds = %if.start
  %gs12 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !121
  %timer13 = getelementptr %struct.Game, %struct.Game* %gs12, i32 0, i32 7, !dbg !121
  store float 0.000000e+00, float* %timer13, align 4, !dbg !121
  br label %if.start14, !dbg !121

if.start14:                                       ; preds = %if.then
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !123
  %move_down = call i1 @move_down(%struct.Game* %ref_arg), !dbg !123
  %NOT = xor i1 %move_down, true, !dbg !123
  br i1 %NOT, label %if.then16, label %if.end15, !dbg !123

if.end15:                                         ; preds = %if.then16, %if.start14
  br label %if.end, !dbg !122

if.then16:                                        ; preds = %if.start14
  %ref_arg17 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !122
  call void @lock_piece(%struct.Game* %ref_arg17), !dbg !122
  br label %if.end15, !dbg !122
}

define void @draw_cell(i32 %col, i32 %row, %struct.Color %color) !dbg !124 {
entry:
  %col1 = alloca i32, align 4, !dbg !125
  %row2 = alloca i32, align 4, !dbg !125
  %color3 = alloca %struct.Color, align 8, !dbg !125
  %x = alloca i32, align 4, !dbg !125
  %y = alloca i32, align 4, !dbg !125
  store i32 %col, i32* %col1, align 4, !dbg !125
  store i32 %row, i32* %row2, align 4, !dbg !125
  store %struct.Color %color, %struct.Color* %color3, align 1, !dbg !125
  store i32 0, i32* %x, align 4, !dbg !125
  %col4 = load i32, i32* %col1, align 4, !dbg !125
  %CELL = load i32, i32* @CELL, align 4, !dbg !125
  %MUL = mul i32 %col4, %CELL, !dbg !125
  %BOARD_X = load i32, i32* @BOARD_X, align 4, !dbg !125
  %ADD = add i32 %BOARD_X, %MUL, !dbg !125
  store i32 %ADD, i32* %x, align 4, !dbg !125
  store i32 0, i32* %y, align 4, !dbg !125
  %row5 = load i32, i32* %row2, align 4, !dbg !125
  %CELL6 = load i32, i32* @CELL, align 4, !dbg !125
  %MUL7 = mul i32 %row5, %CELL6, !dbg !125
  %BOARD_Y = load i32, i32* @BOARD_Y, align 4, !dbg !125
  %ADD8 = add i32 %BOARD_Y, %MUL7, !dbg !125
  store i32 %ADD8, i32* %y, align 4, !dbg !125
  %x9 = load i32, i32* %x, align 4, !dbg !126
  %ADD10 = add i32 %x9, 1, !dbg !126
  %y11 = load i32, i32* %y, align 4, !dbg !126
  %ADD12 = add i32 %y11, 1, !dbg !126
  %CELL13 = load i32, i32* @CELL, align 4, !dbg !126
  %SUB = sub i32 %CELL13, 2, !dbg !126
  %CELL14 = load i32, i32* @CELL, align 4, !dbg !126
  %SUB15 = sub i32 %CELL14, 2, !dbg !126
  %color16 = load %struct.Color, %struct.Color* %color3, align 1, !dbg !126
  %st_slot = alloca %struct.Color, align 8, !dbg !126
  store %struct.Color %color16, %struct.Color* %st_slot, align 1, !dbg !126
  %i64_slot = alloca i64, align 8, !dbg !126
  %0 = bitcast i64* %i64_slot to i8*, !dbg !126
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !126
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !126
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !126
  call void @DrawRectangle(i32 %ADD10, i32 %ADD12, i32 %SUB, i32 %SUB15, i64 %i64_arg), !dbg !126
  %x17 = load i32, i32* %x, align 4, !dbg !127
  %ADD18 = add i32 %x17, 1, !dbg !127
  %y19 = load i32, i32* %y, align 4, !dbg !127
  %ADD20 = add i32 %y19, 1, !dbg !127
  %CELL21 = load i32, i32* @CELL, align 4, !dbg !127
  %SUB22 = sub i32 %CELL21, 2, !dbg !127
  %CELL23 = load i32, i32* @CELL, align 4, !dbg !127
  %SUB24 = sub i32 %CELL23, 2, !dbg !127
  %color25 = load %struct.Color, %struct.Color* %color3, align 1, !dbg !127
  %st_slot26 = alloca %struct.Color, align 8, !dbg !127
  store %struct.Color %color25, %struct.Color* %st_slot26, align 1, !dbg !127
  %i64_slot27 = alloca i64, align 8, !dbg !127
  %2 = bitcast i64* %i64_slot27 to i8*, !dbg !127
  %3 = bitcast %struct.Color* %st_slot26 to i8*, !dbg !127
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i1 false), !dbg !127
  %i64_arg28 = load i64, i64* %i64_slot27, align 4, !dbg !127
  %ColorBrightness = call i64 @ColorBrightness(i64 %i64_arg28, float 0x3FD3333340000000), !dbg !127
  %ret_i64 = alloca i64, align 8, !dbg !127
  store i64 %ColorBrightness, i64* %ret_i64, align 4, !dbg !127
  %ret_struct = alloca %struct.Color, align 8, !dbg !127
  %4 = bitcast %struct.Color* %ret_struct to i8*, !dbg !127
  %5 = bitcast i64* %ret_i64 to i8*, !dbg !127
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* %5, i64 4, i1 false), !dbg !127
  %ret_struct_val = load %struct.Color, %struct.Color* %ret_struct, align 1, !dbg !127
  %tmp_struct = alloca %struct.Color, align 8, !dbg !127
  store %struct.Color %ret_struct_val, %struct.Color* %tmp_struct, align 1, !dbg !127
  %st_slot29 = alloca %struct.Color, align 8, !dbg !127
  store %struct.Color %ret_struct_val, %struct.Color* %st_slot29, align 1, !dbg !127
  %i64_slot30 = alloca i64, align 8, !dbg !127
  %6 = bitcast i64* %i64_slot30 to i8*, !dbg !127
  %7 = bitcast %struct.Color* %st_slot29 to i8*, !dbg !127
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* %7, i64 4, i1 false), !dbg !127
  %i64_arg31 = load i64, i64* %i64_slot30, align 4, !dbg !127
  call void @DrawRectangleLines(i32 %ADD18, i32 %ADD20, i32 %SUB22, i32 %SUB24, i64 %i64_arg31), !dbg !127
  call void @Color.delete(%struct.Color* %color3), !dbg !127
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !127
  ret void, !dbg !127
}

define void @draw_piece_at(%struct.Game* %gs, i32 %t, i32 %rot, i32 %bcol, i32 %brow, %struct.Color %color) !dbg !128 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !129
  %t2 = alloca i32, align 4, !dbg !129
  %rot3 = alloca i32, align 4, !dbg !129
  %bcol4 = alloca i32, align 4, !dbg !129
  %brow5 = alloca i32, align 4, !dbg !129
  %color6 = alloca %struct.Color, align 8, !dbg !129
  %r = alloca i32, align 4, !dbg !129
  %c = alloca i32, align 4, !dbg !129
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !129
  store i32 %t, i32* %t2, align 4, !dbg !129
  store i32 %rot, i32* %rot3, align 4, !dbg !129
  store i32 %bcol, i32* %bcol4, align 4, !dbg !129
  store i32 %brow, i32* %brow5, align 4, !dbg !129
  store %struct.Color %color, %struct.Color* %color6, align 1, !dbg !129
  store i32 0, i32* %r, align 4, !dbg !129
  store i32 0, i32* %r, align 4, !dbg !129
  br label %while.start, !dbg !129

while.start:                                      ; preds = %while.end10, %entry
  %r7 = load i32, i32* %r, align 4, !dbg !129
  %LT = icmp slt i32 %r7, 4, !dbg !129
  br i1 %LT, label %while.then, label %while.end, !dbg !129

while.then:                                       ; preds = %while.start
  store i32 0, i32* %c, align 4, !dbg !129
  store i32 0, i32* %c, align 4, !dbg !129
  br label %while.start8, !dbg !129

while.end:                                        ; preds = %while.start
  call void @Color.delete(%struct.Color* %color6), !dbg !130
  ret void, !dbg !130

while.start8:                                     ; preds = %if.end, %while.then
  %c11 = load i32, i32* %c, align 4, !dbg !129
  %LT12 = icmp slt i32 %c11, 4, !dbg !129
  br i1 %LT12, label %while.then9, label %while.end10, !dbg !129

while.then9:                                      ; preds = %while.start8
  br label %if.start, !dbg !129

while.end10:                                      ; preds = %while.start8
  %r25 = load i32, i32* %r, align 4, !dbg !130
  %ADD26 = add i32 %r25, 1, !dbg !130
  store i32 %ADD26, i32* %r, align 4, !dbg !130
  br label %while.start, !dbg !130

if.start:                                         ; preds = %while.then9
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !131
  %t13 = load i32, i32* %t2, align 4, !dbg !131
  %rot14 = load i32, i32* %rot3, align 4, !dbg !131
  %r15 = load i32, i32* %r, align 4, !dbg !131
  %c16 = load i32, i32* %c, align 4, !dbg !131
  %cell_occupied = call i1 @cell_occupied(%struct.Game* %ref_arg, i32 %t13, i32 %rot14, i32 %r15, i32 %c16), !dbg !131
  br i1 %cell_occupied, label %if.then, label %if.end, !dbg !131

if.end:                                           ; preds = %if.then, %if.start
  %c23 = load i32, i32* %c, align 4, !dbg !130
  %ADD24 = add i32 %c23, 1, !dbg !130
  store i32 %ADD24, i32* %c, align 4, !dbg !130
  br label %while.start8, !dbg !130

if.then:                                          ; preds = %if.start
  %bcol17 = load i32, i32* %bcol4, align 4, !dbg !130
  %c18 = load i32, i32* %c, align 4, !dbg !130
  %ADD = add i32 %bcol17, %c18, !dbg !130
  %brow19 = load i32, i32* %brow5, align 4, !dbg !130
  %r20 = load i32, i32* %r, align 4, !dbg !130
  %ADD21 = add i32 %brow19, %r20, !dbg !130
  %color22 = load %struct.Color, %struct.Color* %color6, align 1, !dbg !130
  call void @draw_cell(i32 %ADD, i32 %ADD21, %struct.Color %color22), !dbg !130
  br label %if.end, !dbg !130
}

define void @draw_board(%struct.Game* %gs) !dbg !132 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !133
  %r = alloca i32, align 4, !dbg !133
  %c = alloca i32, align 4, !dbg !133
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !133
  %BOARD_X = load i32, i32* @BOARD_X, align 4, !dbg !134
  %BOARD_Y = load i32, i32* @BOARD_Y, align 4, !dbg !134
  %BOARD_COLS = load i32, i32* @BOARD_COLS, align 4, !dbg !134
  %CELL = load i32, i32* @CELL, align 4, !dbg !134
  %MUL = mul i32 %BOARD_COLS, %CELL, !dbg !134
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !134
  %CELL2 = load i32, i32* @CELL, align 4, !dbg !134
  %MUL3 = mul i32 %BOARD_ROWS, %CELL2, !dbg !134
  %setColor = call %struct.Color @setColor(i32 20, i32 20, i32 30, i32 255), !dbg !134
  %tmp_struct = alloca %struct.Color, align 8, !dbg !134
  store %struct.Color %setColor, %struct.Color* %tmp_struct, align 1, !dbg !134
  %st_slot = alloca %struct.Color, align 8, !dbg !134
  store %struct.Color %setColor, %struct.Color* %st_slot, align 1, !dbg !134
  %i64_slot = alloca i64, align 8, !dbg !134
  %0 = bitcast i64* %i64_slot to i8*, !dbg !134
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !134
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !134
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !134
  call void @DrawRectangle(i32 %BOARD_X, i32 %BOARD_Y, i32 %MUL, i32 %MUL3, i64 %i64_arg), !dbg !134
  store i32 0, i32* %r, align 4, !dbg !133
  store i32 0, i32* %r, align 4, !dbg !133
  br label %while.start, !dbg !133

while.start:                                      ; preds = %while.then, %entry
  %r4 = load i32, i32* %r, align 4, !dbg !133
  %BOARD_ROWS5 = load i32, i32* @BOARD_ROWS, align 4, !dbg !133
  %LE = icmp sle i32 %r4, %BOARD_ROWS5, !dbg !133
  br i1 %LE, label %while.then, label %while.end, !dbg !133

while.then:                                       ; preds = %while.start
  %BOARD_X6 = load i32, i32* @BOARD_X, align 4, !dbg !135
  %r7 = load i32, i32* %r, align 4, !dbg !135
  %CELL8 = load i32, i32* @CELL, align 4, !dbg !135
  %MUL9 = mul i32 %r7, %CELL8, !dbg !135
  %BOARD_Y10 = load i32, i32* @BOARD_Y, align 4, !dbg !135
  %ADD = add i32 %BOARD_Y10, %MUL9, !dbg !135
  %BOARD_COLS11 = load i32, i32* @BOARD_COLS, align 4, !dbg !135
  %CELL12 = load i32, i32* @CELL, align 4, !dbg !135
  %MUL13 = mul i32 %BOARD_COLS11, %CELL12, !dbg !135
  %BOARD_X14 = load i32, i32* @BOARD_X, align 4, !dbg !135
  %ADD15 = add i32 %BOARD_X14, %MUL13, !dbg !135
  %r16 = load i32, i32* %r, align 4, !dbg !135
  %CELL17 = load i32, i32* @CELL, align 4, !dbg !135
  %MUL18 = mul i32 %r16, %CELL17, !dbg !135
  %BOARD_Y19 = load i32, i32* @BOARD_Y, align 4, !dbg !135
  %ADD20 = add i32 %BOARD_Y19, %MUL18, !dbg !135
  %setColor21 = call %struct.Color @setColor(i32 40, i32 40, i32 55, i32 255), !dbg !135
  %tmp_struct22 = alloca %struct.Color, align 8, !dbg !135
  store %struct.Color %setColor21, %struct.Color* %tmp_struct22, align 1, !dbg !135
  %st_slot23 = alloca %struct.Color, align 8, !dbg !135
  store %struct.Color %setColor21, %struct.Color* %st_slot23, align 1, !dbg !135
  %i64_slot24 = alloca i64, align 8, !dbg !135
  %2 = bitcast i64* %i64_slot24 to i8*, !dbg !135
  %3 = bitcast %struct.Color* %st_slot23 to i8*, !dbg !135
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i1 false), !dbg !135
  %i64_arg25 = load i64, i64* %i64_slot24, align 4, !dbg !135
  call void @DrawLine(i32 %BOARD_X6, i32 %ADD, i32 %ADD15, i32 %ADD20, i64 %i64_arg25), !dbg !135
  %r26 = load i32, i32* %r, align 4, !dbg !135
  %ADD27 = add i32 %r26, 1, !dbg !135
  store i32 %ADD27, i32* %r, align 4, !dbg !135
  br label %while.start, !dbg !135

while.end:                                        ; preds = %while.start
  store i32 0, i32* %c, align 4, !dbg !133
  store i32 0, i32* %c, align 4, !dbg !133
  br label %while.start28, !dbg !133

while.start28:                                    ; preds = %while.then29, %while.end
  %c31 = load i32, i32* %c, align 4, !dbg !133
  %BOARD_COLS32 = load i32, i32* @BOARD_COLS, align 4, !dbg !133
  %LE33 = icmp sle i32 %c31, %BOARD_COLS32, !dbg !133
  br i1 %LE33, label %while.then29, label %while.end30, !dbg !133

while.then29:                                     ; preds = %while.start28
  %c34 = load i32, i32* %c, align 4, !dbg !136
  %CELL35 = load i32, i32* @CELL, align 4, !dbg !136
  %MUL36 = mul i32 %c34, %CELL35, !dbg !136
  %BOARD_X37 = load i32, i32* @BOARD_X, align 4, !dbg !136
  %ADD38 = add i32 %BOARD_X37, %MUL36, !dbg !136
  %BOARD_Y39 = load i32, i32* @BOARD_Y, align 4, !dbg !136
  %c40 = load i32, i32* %c, align 4, !dbg !136
  %CELL41 = load i32, i32* @CELL, align 4, !dbg !136
  %MUL42 = mul i32 %c40, %CELL41, !dbg !136
  %BOARD_X43 = load i32, i32* @BOARD_X, align 4, !dbg !136
  %ADD44 = add i32 %BOARD_X43, %MUL42, !dbg !136
  %BOARD_ROWS45 = load i32, i32* @BOARD_ROWS, align 4, !dbg !136
  %CELL46 = load i32, i32* @CELL, align 4, !dbg !136
  %MUL47 = mul i32 %BOARD_ROWS45, %CELL46, !dbg !136
  %BOARD_Y48 = load i32, i32* @BOARD_Y, align 4, !dbg !136
  %ADD49 = add i32 %BOARD_Y48, %MUL47, !dbg !136
  %setColor50 = call %struct.Color @setColor(i32 40, i32 40, i32 55, i32 255), !dbg !136
  %tmp_struct51 = alloca %struct.Color, align 8, !dbg !136
  store %struct.Color %setColor50, %struct.Color* %tmp_struct51, align 1, !dbg !136
  %st_slot52 = alloca %struct.Color, align 8, !dbg !136
  store %struct.Color %setColor50, %struct.Color* %st_slot52, align 1, !dbg !136
  %i64_slot53 = alloca i64, align 8, !dbg !136
  %4 = bitcast i64* %i64_slot53 to i8*, !dbg !136
  %5 = bitcast %struct.Color* %st_slot52 to i8*, !dbg !136
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* %5, i64 4, i1 false), !dbg !136
  %i64_arg54 = load i64, i64* %i64_slot53, align 4, !dbg !136
  call void @DrawLine(i32 %ADD38, i32 %BOARD_Y39, i32 %ADD44, i32 %ADD49, i64 %i64_arg54), !dbg !136
  %c55 = load i32, i32* %c, align 4, !dbg !136
  %ADD56 = add i32 %c55, 1, !dbg !136
  store i32 %ADD56, i32* %c, align 4, !dbg !136
  br label %while.start28, !dbg !136

while.end30:                                      ; preds = %while.start28
  store i32 0, i32* %r, align 4, !dbg !136
  br label %while.start57, !dbg !136

while.start57:                                    ; preds = %while.end64, %while.end30
  %r60 = load i32, i32* %r, align 4, !dbg !136
  %BOARD_ROWS61 = load i32, i32* @BOARD_ROWS, align 4, !dbg !136
  %LT = icmp slt i32 %r60, %BOARD_ROWS61, !dbg !136
  br i1 %LT, label %while.then58, label %while.end59, !dbg !136

while.then58:                                     ; preds = %while.start57
  store i32 0, i32* %c, align 4, !dbg !136
  br label %while.start62, !dbg !136

while.end59:                                      ; preds = %while.start57
  %BOARD_X91 = load i32, i32* @BOARD_X, align 4, !dbg !137
  %BOARD_Y92 = load i32, i32* @BOARD_Y, align 4, !dbg !137
  %BOARD_COLS93 = load i32, i32* @BOARD_COLS, align 4, !dbg !137
  %CELL94 = load i32, i32* @CELL, align 4, !dbg !137
  %MUL95 = mul i32 %BOARD_COLS93, %CELL94, !dbg !137
  %BOARD_ROWS96 = load i32, i32* @BOARD_ROWS, align 4, !dbg !137
  %CELL97 = load i32, i32* @CELL, align 4, !dbg !137
  %MUL98 = mul i32 %BOARD_ROWS96, %CELL97, !dbg !137
  %setColor99 = call %struct.Color @setColor(i32 180, i32 180, i32 200, i32 255), !dbg !137
  %tmp_struct100 = alloca %struct.Color, align 8, !dbg !137
  store %struct.Color %setColor99, %struct.Color* %tmp_struct100, align 1, !dbg !137
  %st_slot101 = alloca %struct.Color, align 8, !dbg !137
  store %struct.Color %setColor99, %struct.Color* %st_slot101, align 1, !dbg !137
  %i64_slot102 = alloca i64, align 8, !dbg !137
  %6 = bitcast i64* %i64_slot102 to i8*, !dbg !137
  %7 = bitcast %struct.Color* %st_slot101 to i8*, !dbg !137
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* %7, i64 4, i1 false), !dbg !137
  %i64_arg103 = load i64, i64* %i64_slot102, align 4, !dbg !137
  call void @DrawRectangleLines(i32 %BOARD_X91, i32 %BOARD_Y92, i32 %MUL95, i32 %MUL98, i64 %i64_arg103), !dbg !137
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !137
  call void @Color.delete(%struct.Color* %tmp_struct100), !dbg !137
  ret void, !dbg !137

while.start62:                                    ; preds = %if.end, %while.then58
  %c65 = load i32, i32* %c, align 4, !dbg !136
  %BOARD_COLS66 = load i32, i32* @BOARD_COLS, align 4, !dbg !136
  %LT67 = icmp slt i32 %c65, %BOARD_COLS66, !dbg !136
  br i1 %LT67, label %while.then63, label %while.end64, !dbg !136

while.then63:                                     ; preds = %while.start62
  br label %if.start, !dbg !136

while.end64:                                      ; preds = %while.start62
  %r89 = load i32, i32* %r, align 4, !dbg !138
  %ADD90 = add i32 %r89, 1, !dbg !138
  store i32 %ADD90, i32* %r, align 4, !dbg !138
  br label %while.start57, !dbg !138

if.start:                                         ; preds = %while.then63
  %gs68 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !139
  %board = getelementptr %struct.Game, %struct.Game* %gs68, i32 0, i32 0, !dbg !139
  %dim = load i32, i32* @Game.board.dim0, align 4, !dbg !139
  %dim69 = load i32, i32* @Game.board.dim1, align 4, !dbg !139
  %DOT = load i32*, i32** %board, align 8, !dbg !139
  %r70 = load i32, i32* %r, align 4, !dbg !139
  %stride = mul i32 1, %dim69, !dbg !139
  %flat_idx = mul i32 %r70, %stride, !dbg !139
  %row = getelementptr i32, i32* %DOT, i32 %flat_idx, !dbg !139
  %c71 = load i32, i32* %c, align 4, !dbg !139
  %ACCESS = getelementptr i32, i32* %row, i32 %c71, !dbg !139
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !139
  %NEQ = icmp ne i32 %ACC, 0, !dbg !139
  br i1 %NEQ, label %if.then, label %if.end, !dbg !139

if.end:                                           ; preds = %if.then, %if.start
  %c87 = load i32, i32* %c, align 4, !dbg !138
  %ADD88 = add i32 %c87, 1, !dbg !138
  store i32 %ADD88, i32* %c, align 4, !dbg !138
  br label %while.start62, !dbg !138

if.then:                                          ; preds = %if.start
  %c72 = load i32, i32* %c, align 4, !dbg !138
  %r73 = load i32, i32* %r, align 4, !dbg !138
  %gs74 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !138
  %board75 = getelementptr %struct.Game, %struct.Game* %gs74, i32 0, i32 0, !dbg !138
  %dim76 = load i32, i32* @Game.board.dim0, align 4, !dbg !138
  %dim77 = load i32, i32* @Game.board.dim1, align 4, !dbg !138
  %DOT78 = load i32*, i32** %board75, align 8, !dbg !138
  %r79 = load i32, i32* %r, align 4, !dbg !138
  %stride80 = mul i32 1, %dim77, !dbg !138
  %flat_idx81 = mul i32 %r79, %stride80, !dbg !138
  %row82 = getelementptr i32, i32* %DOT78, i32 %flat_idx81, !dbg !138
  %c83 = load i32, i32* %c, align 4, !dbg !138
  %ACCESS84 = getelementptr i32, i32* %row82, i32 %c83, !dbg !138
  %ACC85 = load i32, i32* %ACCESS84, align 4, !dbg !138
  %SUB = sub i32 %ACC85, 1, !dbg !138
  %piece_color = call %struct.Color @piece_color(i32 %SUB), !dbg !138
  %tmp_struct86 = alloca %struct.Color, align 8, !dbg !138
  store %struct.Color %piece_color, %struct.Color* %tmp_struct86, align 1, !dbg !138
  call void @draw_cell(i32 %c72, i32 %r73, %struct.Color %piece_color), !dbg !138
  br label %if.end, !dbg !138
}

define void @draw_ghost(%struct.Game* %gs) !dbg !140 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !141
  %gr = alloca i32, align 4, !dbg !141
  %gc = alloca %struct.Color, align 8, !dbg !141
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !141
  store i32 0, i32* %gr, align 4, !dbg !141
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !142
  %ghost_row = call i32 @ghost_row(%struct.Game* %ref_arg), !dbg !142
  store i32 %ghost_row, i32* %gr, align 4, !dbg !142
  br label %if.start, !dbg !142

if.start:                                         ; preds = %entry
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !142
  %current = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 2, !dbg !142
  %row = getelementptr %struct.Piece, %struct.Piece* %current, i32 0, i32 3, !dbg !142
  %gr3 = load i32, i32* %gr, align 4, !dbg !142
  %DOT = load i32, i32* %row, align 4, !dbg !142
  %EQ = icmp eq i32 %gr3, %DOT, !dbg !142
  br i1 %EQ, label %if.then, label %if.end, !dbg !142

if.end:                                           ; preds = %if.start
  store %struct.Color zeroinitializer, %struct.Color* %gc, align 1, !dbg !141
  %gs4 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !143
  %current5 = getelementptr %struct.Game, %struct.Game* %gs4, i32 0, i32 2, !dbg !143
  %type = getelementptr %struct.Piece, %struct.Piece* %current5, i32 0, i32 0, !dbg !143
  %DOT6 = load i32, i32* %type, align 4, !dbg !143
  %piece_color = call %struct.Color @piece_color(i32 %DOT6), !dbg !143
  %tmp_struct = alloca %struct.Color, align 8, !dbg !143
  store %struct.Color %piece_color, %struct.Color* %tmp_struct, align 1, !dbg !143
  store %struct.Color %piece_color, %struct.Color* %gc, align 1, !dbg !143
  %a = getelementptr %struct.Color, %struct.Color* %gc, i32 0, i32 3, !dbg !143
  store i8 60, i8* %a, align 1, !dbg !143
  %ref_arg7 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !144
  %gs8 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !144
  %current9 = getelementptr %struct.Game, %struct.Game* %gs8, i32 0, i32 2, !dbg !144
  %type10 = getelementptr %struct.Piece, %struct.Piece* %current9, i32 0, i32 0, !dbg !144
  %DOT11 = load i32, i32* %type10, align 4, !dbg !144
  %gs12 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !144
  %current13 = getelementptr %struct.Game, %struct.Game* %gs12, i32 0, i32 2, !dbg !144
  %rotation = getelementptr %struct.Piece, %struct.Piece* %current13, i32 0, i32 1, !dbg !144
  %DOT14 = load i32, i32* %rotation, align 4, !dbg !144
  %gs15 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !144
  %current16 = getelementptr %struct.Game, %struct.Game* %gs15, i32 0, i32 2, !dbg !144
  %col = getelementptr %struct.Piece, %struct.Piece* %current16, i32 0, i32 2, !dbg !144
  %DOT17 = load i32, i32* %col, align 4, !dbg !144
  %gr18 = load i32, i32* %gr, align 4, !dbg !144
  %gc19 = load %struct.Color, %struct.Color* %gc, align 1, !dbg !144
  call void @draw_piece_at(%struct.Game* %ref_arg7, i32 %DOT11, i32 %DOT14, i32 %DOT17, i32 %gr18, %struct.Color %gc19), !dbg !144
  call void @Color.delete(%struct.Color* %gc), !dbg !144
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !144
  ret void, !dbg !144

if.then:                                          ; preds = %if.start
  ret void, !dbg !142
}

define void @draw_current(%struct.Game* %gs) !dbg !145 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !146
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !146
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !147
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !147
  %current = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 2, !dbg !147
  %type = getelementptr %struct.Piece, %struct.Piece* %current, i32 0, i32 0, !dbg !147
  %DOT = load i32, i32* %type, align 4, !dbg !147
  %gs3 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !147
  %current4 = getelementptr %struct.Game, %struct.Game* %gs3, i32 0, i32 2, !dbg !147
  %rotation = getelementptr %struct.Piece, %struct.Piece* %current4, i32 0, i32 1, !dbg !147
  %DOT5 = load i32, i32* %rotation, align 4, !dbg !147
  %gs6 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !147
  %current7 = getelementptr %struct.Game, %struct.Game* %gs6, i32 0, i32 2, !dbg !147
  %col = getelementptr %struct.Piece, %struct.Piece* %current7, i32 0, i32 2, !dbg !147
  %DOT8 = load i32, i32* %col, align 4, !dbg !147
  %gs9 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !147
  %current10 = getelementptr %struct.Game, %struct.Game* %gs9, i32 0, i32 2, !dbg !147
  %row = getelementptr %struct.Piece, %struct.Piece* %current10, i32 0, i32 3, !dbg !147
  %DOT11 = load i32, i32* %row, align 4, !dbg !147
  %gs12 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !147
  %current13 = getelementptr %struct.Game, %struct.Game* %gs12, i32 0, i32 2, !dbg !147
  %type14 = getelementptr %struct.Piece, %struct.Piece* %current13, i32 0, i32 0, !dbg !147
  %DOT15 = load i32, i32* %type14, align 4, !dbg !147
  %piece_color = call %struct.Color @piece_color(i32 %DOT15), !dbg !147
  %tmp_struct = alloca %struct.Color, align 8, !dbg !147
  store %struct.Color %piece_color, %struct.Color* %tmp_struct, align 1, !dbg !147
  call void @draw_piece_at(%struct.Game* %ref_arg, i32 %DOT, i32 %DOT5, i32 %DOT8, i32 %DOT11, %struct.Color %piece_color), !dbg !147
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !147
  ret void, !dbg !147
}

define void @draw_hud(%struct.Game* %gs) !dbg !148 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !149
  %px = alloca i32, align 4, !dbg !149
  %py = alloca i32, align 4, !dbg !149
  %bx = alloca i32, align 4, !dbg !149
  %by = alloca i32, align 4, !dbg !149
  %cs = alloca i32, align 4, !dbg !149
  %r = alloca i32, align 4, !dbg !149
  %c = alloca i32, align 4, !dbg !149
  %x = alloca i32, align 4, !dbg !149
  %y = alloca i32, align 4, !dbg !149
  %col = alloca %struct.Color, align 8, !dbg !149
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !149
  store i32 0, i32* %px, align 4, !dbg !149
  %PANEL_X = load, align 4, !dbg !149
  store i32 %PANEL_X, i32* %px, align 4, !dbg !149
  store i32 0, i32* %py, align 4, !dbg !149
  %BOARD_Y = load i32, i32* @BOARD_Y, align 4, !dbg !149
  store i32 %BOARD_Y, i32* %py, align 4, !dbg !149
  %px2 = load i32, i32* %px, align 4, !dbg !150
  %SUB = sub i32 %px2, 5, !dbg !150
  %py3 = load i32, i32* %py, align 4, !dbg !150
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !150
  %CELL = load i32, i32* @CELL, align 4, !dbg !150
  %MUL = mul i32 %BOARD_ROWS, %CELL, !dbg !150
  %setColor = call %struct.Color @setColor(i32 25, i32 25, i32 38, i32 255), !dbg !150
  %tmp_struct = alloca %struct.Color, align 8, !dbg !150
  store %struct.Color %setColor, %struct.Color* %tmp_struct, align 1, !dbg !150
  %st_slot = alloca %struct.Color, align 8, !dbg !150
  store %struct.Color %setColor, %struct.Color* %st_slot, align 1, !dbg !150
  %i64_slot = alloca i64, align 8, !dbg !150
  %0 = bitcast i64* %i64_slot to i8*, !dbg !150
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !150
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !150
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !150
  call void @DrawRectangle(i32 %SUB, i32 %py3, i32 155, i32 %MUL, i64 %i64_arg), !dbg !150
  %px4 = load i32, i32* %px, align 4, !dbg !151
  %py5 = load i32, i32* %py, align 4, !dbg !151
  %ADD = add i32 %py5, 10, !dbg !151
  %setColor6 = call %struct.Color @setColor(i32 200, i32 200, i32 200, i32 255), !dbg !151
  %tmp_struct7 = alloca %struct.Color, align 8, !dbg !151
  store %struct.Color %setColor6, %struct.Color* %tmp_struct7, align 1, !dbg !151
  %st_slot8 = alloca %struct.Color, align 8, !dbg !151
  store %struct.Color %setColor6, %struct.Color* %st_slot8, align 1, !dbg !151
  %i64_slot9 = alloca i64, align 8, !dbg !151
  %2 = bitcast i64* %i64_slot9 to i8*, !dbg !151
  %3 = bitcast %struct.Color* %st_slot8 to i8*, !dbg !151
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i1 false), !dbg !151
  %i64_arg10 = load i64, i64* %i64_slot9, align 4, !dbg !151
  call void @DrawText(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR2, i32 0, i32 0), i32 %px4, i32 %ADD, i32 16, i64 %i64_arg10), !dbg !151
  %gs11 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !152
  %score = getelementptr %struct.Game, %struct.Game* %gs11, i32 0, i32 4, !dbg !152
  %DOT = load i32, i32* %score, align 4, !dbg !152
  %TextFormat = call i8* (i8*, i32, ...) @TextFormat(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @STR3, i32 0, i32 0), i32 1, i32 %DOT), !dbg !152
  %px12 = load i32, i32* %px, align 4, !dbg !152
  %py13 = load i32, i32* %py, align 4, !dbg !152
  %ADD14 = add i32 %py13, 30, !dbg !152
  %setColor15 = call %struct.Color @setColor(i32 255, i32 255, i32 255, i32 255), !dbg !152
  %tmp_struct16 = alloca %struct.Color, align 8, !dbg !152
  store %struct.Color %setColor15, %struct.Color* %tmp_struct16, align 1, !dbg !152
  %st_slot17 = alloca %struct.Color, align 8, !dbg !152
  store %struct.Color %setColor15, %struct.Color* %st_slot17, align 1, !dbg !152
  %i64_slot18 = alloca i64, align 8, !dbg !152
  %4 = bitcast i64* %i64_slot18 to i8*, !dbg !152
  %5 = bitcast %struct.Color* %st_slot17 to i8*, !dbg !152
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* %5, i64 4, i1 false), !dbg !152
  %i64_arg19 = load i64, i64* %i64_slot18, align 4, !dbg !152
  call void @DrawText(i8* %TextFormat, i32 %px12, i32 %ADD14, i32 22, i64 %i64_arg19), !dbg !152
  %px20 = load i32, i32* %px, align 4, !dbg !153
  %py21 = load i32, i32* %py, align 4, !dbg !153
  %ADD22 = add i32 %py21, 70, !dbg !153
  %setColor23 = call %struct.Color @setColor(i32 200, i32 200, i32 200, i32 255), !dbg !153
  %tmp_struct24 = alloca %struct.Color, align 8, !dbg !153
  store %struct.Color %setColor23, %struct.Color* %tmp_struct24, align 1, !dbg !153
  %st_slot25 = alloca %struct.Color, align 8, !dbg !153
  store %struct.Color %setColor23, %struct.Color* %st_slot25, align 1, !dbg !153
  %i64_slot26 = alloca i64, align 8, !dbg !153
  %6 = bitcast i64* %i64_slot26 to i8*, !dbg !153
  %7 = bitcast %struct.Color* %st_slot25 to i8*, !dbg !153
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* %7, i64 4, i1 false), !dbg !153
  %i64_arg27 = load i64, i64* %i64_slot26, align 4, !dbg !153
  call void @DrawText(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR4, i32 0, i32 0), i32 %px20, i32 %ADD22, i32 16, i64 %i64_arg27), !dbg !153
  %gs28 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !154
  %level = getelementptr %struct.Game, %struct.Game* %gs28, i32 0, i32 5, !dbg !154
  %DOT29 = load i32, i32* %level, align 4, !dbg !154
  %TextFormat30 = call i8* (i8*, i32, ...) @TextFormat(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @STR5, i32 0, i32 0), i32 1, i32 %DOT29), !dbg !154
  %px31 = load i32, i32* %px, align 4, !dbg !154
  %py32 = load i32, i32* %py, align 4, !dbg !154
  %ADD33 = add i32 %py32, 90, !dbg !154
  %setColor34 = call %struct.Color @setColor(i32 255, i32 255, i32 255, i32 255), !dbg !154
  %tmp_struct35 = alloca %struct.Color, align 8, !dbg !154
  store %struct.Color %setColor34, %struct.Color* %tmp_struct35, align 1, !dbg !154
  %st_slot36 = alloca %struct.Color, align 8, !dbg !154
  store %struct.Color %setColor34, %struct.Color* %st_slot36, align 1, !dbg !154
  %i64_slot37 = alloca i64, align 8, !dbg !154
  %8 = bitcast i64* %i64_slot37 to i8*, !dbg !154
  %9 = bitcast %struct.Color* %st_slot36 to i8*, !dbg !154
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %8, i8* %9, i64 4, i1 false), !dbg !154
  %i64_arg38 = load i64, i64* %i64_slot37, align 4, !dbg !154
  call void @DrawText(i8* %TextFormat30, i32 %px31, i32 %ADD33, i32 22, i64 %i64_arg38), !dbg !154
  %px39 = load i32, i32* %px, align 4, !dbg !155
  %py40 = load i32, i32* %py, align 4, !dbg !155
  %ADD41 = add i32 %py40, 130, !dbg !155
  %setColor42 = call %struct.Color @setColor(i32 200, i32 200, i32 200, i32 255), !dbg !155
  %tmp_struct43 = alloca %struct.Color, align 8, !dbg !155
  store %struct.Color %setColor42, %struct.Color* %tmp_struct43, align 1, !dbg !155
  %st_slot44 = alloca %struct.Color, align 8, !dbg !155
  store %struct.Color %setColor42, %struct.Color* %st_slot44, align 1, !dbg !155
  %i64_slot45 = alloca i64, align 8, !dbg !155
  %10 = bitcast i64* %i64_slot45 to i8*, !dbg !155
  %11 = bitcast %struct.Color* %st_slot44 to i8*, !dbg !155
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %10, i8* %11, i64 4, i1 false), !dbg !155
  %i64_arg46 = load i64, i64* %i64_slot45, align 4, !dbg !155
  call void @DrawText(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR6, i32 0, i32 0), i32 %px39, i32 %ADD41, i32 16, i64 %i64_arg46), !dbg !155
  %gs47 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !156
  %lines = getelementptr %struct.Game, %struct.Game* %gs47, i32 0, i32 6, !dbg !156
  %DOT48 = load i32, i32* %lines, align 4, !dbg !156
  %TextFormat49 = call i8* (i8*, i32, ...) @TextFormat(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @STR7, i32 0, i32 0), i32 1, i32 %DOT48), !dbg !156
  %px50 = load i32, i32* %px, align 4, !dbg !156
  %py51 = load i32, i32* %py, align 4, !dbg !156
  %ADD52 = add i32 %py51, 150, !dbg !156
  %setColor53 = call %struct.Color @setColor(i32 255, i32 255, i32 255, i32 255), !dbg !156
  %tmp_struct54 = alloca %struct.Color, align 8, !dbg !156
  store %struct.Color %setColor53, %struct.Color* %tmp_struct54, align 1, !dbg !156
  %st_slot55 = alloca %struct.Color, align 8, !dbg !156
  store %struct.Color %setColor53, %struct.Color* %st_slot55, align 1, !dbg !156
  %i64_slot56 = alloca i64, align 8, !dbg !156
  %12 = bitcast i64* %i64_slot56 to i8*, !dbg !156
  %13 = bitcast %struct.Color* %st_slot55 to i8*, !dbg !156
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %12, i8* %13, i64 4, i1 false), !dbg !156
  %i64_arg57 = load i64, i64* %i64_slot56, align 4, !dbg !156
  call void @DrawText(i8* %TextFormat49, i32 %px50, i32 %ADD52, i32 22, i64 %i64_arg57), !dbg !156
  %px58 = load i32, i32* %px, align 4, !dbg !157
  %py59 = load i32, i32* %py, align 4, !dbg !157
  %ADD60 = add i32 %py59, 200, !dbg !157
  %setColor61 = call %struct.Color @setColor(i32 200, i32 200, i32 200, i32 255), !dbg !157
  %tmp_struct62 = alloca %struct.Color, align 8, !dbg !157
  store %struct.Color %setColor61, %struct.Color* %tmp_struct62, align 1, !dbg !157
  %st_slot63 = alloca %struct.Color, align 8, !dbg !157
  store %struct.Color %setColor61, %struct.Color* %st_slot63, align 1, !dbg !157
  %i64_slot64 = alloca i64, align 8, !dbg !157
  %14 = bitcast i64* %i64_slot64 to i8*, !dbg !157
  %15 = bitcast %struct.Color* %st_slot63 to i8*, !dbg !157
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %14, i8* %15, i64 4, i1 false), !dbg !157
  %i64_arg65 = load i64, i64* %i64_slot64, align 4, !dbg !157
  call void @DrawText(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR8, i32 0, i32 0), i32 %px58, i32 %ADD60, i32 16, i64 %i64_arg65), !dbg !157
  store i32 0, i32* %bx, align 4, !dbg !149
  %px66 = load i32, i32* %px, align 4, !dbg !149
  store i32 %px66, i32* %bx, align 4, !dbg !149
  store i32 0, i32* %by, align 4, !dbg !149
  %py67 = load i32, i32* %py, align 4, !dbg !149
  %ADD68 = add i32 %py67, 220, !dbg !149
  store i32 %ADD68, i32* %by, align 4, !dbg !149
  store i32 0, i32* %cs, align 4, !dbg !149
  store i32 20, i32* %cs, align 4, !dbg !149
  %bx69 = load i32, i32* %bx, align 4, !dbg !158
  %by70 = load i32, i32* %by, align 4, !dbg !158
  %cs71 = load i32, i32* %cs, align 4, !dbg !158
  %MUL72 = mul i32 4, %cs71, !dbg !158
  %cs73 = load i32, i32* %cs, align 4, !dbg !158
  %MUL74 = mul i32 4, %cs73, !dbg !158
  %setColor75 = call %struct.Color @setColor(i32 20, i32 20, i32 30, i32 255), !dbg !158
  %tmp_struct76 = alloca %struct.Color, align 8, !dbg !158
  store %struct.Color %setColor75, %struct.Color* %tmp_struct76, align 1, !dbg !158
  %st_slot77 = alloca %struct.Color, align 8, !dbg !158
  store %struct.Color %setColor75, %struct.Color* %st_slot77, align 1, !dbg !158
  %i64_slot78 = alloca i64, align 8, !dbg !158
  %16 = bitcast i64* %i64_slot78 to i8*, !dbg !158
  %17 = bitcast %struct.Color* %st_slot77 to i8*, !dbg !158
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %16, i8* %17, i64 4, i1 false), !dbg !158
  %i64_arg79 = load i64, i64* %i64_slot78, align 4, !dbg !158
  call void @DrawRectangle(i32 %bx69, i32 %by70, i32 %MUL72, i32 %MUL74, i64 %i64_arg79), !dbg !158
  %bx80 = load i32, i32* %bx, align 4, !dbg !159
  %by81 = load i32, i32* %by, align 4, !dbg !159
  %cs82 = load i32, i32* %cs, align 4, !dbg !159
  %MUL83 = mul i32 4, %cs82, !dbg !159
  %cs84 = load i32, i32* %cs, align 4, !dbg !159
  %MUL85 = mul i32 4, %cs84, !dbg !159
  %setColor86 = call %struct.Color @setColor(i32 180, i32 180, i32 200, i32 255), !dbg !159
  %tmp_struct87 = alloca %struct.Color, align 8, !dbg !159
  store %struct.Color %setColor86, %struct.Color* %tmp_struct87, align 1, !dbg !159
  %st_slot88 = alloca %struct.Color, align 8, !dbg !159
  store %struct.Color %setColor86, %struct.Color* %st_slot88, align 1, !dbg !159
  %i64_slot89 = alloca i64, align 8, !dbg !159
  %18 = bitcast i64* %i64_slot89 to i8*, !dbg !159
  %19 = bitcast %struct.Color* %st_slot88 to i8*, !dbg !159
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %18, i8* %19, i64 4, i1 false), !dbg !159
  %i64_arg90 = load i64, i64* %i64_slot89, align 4, !dbg !159
  call void @DrawRectangleLines(i32 %bx80, i32 %by81, i32 %MUL83, i32 %MUL85, i64 %i64_arg90), !dbg !159
  store i32 0, i32* %r, align 4, !dbg !149
  store i32 0, i32* %r, align 4, !dbg !149
  br label %while.start, !dbg !149

while.start:                                      ; preds = %while.end94, %entry
  %r91 = load i32, i32* %r, align 4, !dbg !149
  %LT = icmp slt i32 %r91, 4, !dbg !149
  br i1 %LT, label %while.then, label %while.end, !dbg !149

while.then:                                       ; preds = %while.start
  store i32 0, i32* %c, align 4, !dbg !149
  store i32 0, i32* %c, align 4, !dbg !149
  br label %while.start92, !dbg !149

while.end:                                        ; preds = %while.start
  %px149 = load i32, i32* %px, align 4, !dbg !160
  %py150 = load i32, i32* %py, align 4, !dbg !160
  %ADD151 = add i32 %py150, 370, !dbg !160
  %setColor152 = call %struct.Color @setColor(i32 80, i32 80, i32 80, i32 255), !dbg !160
  %tmp_struct153 = alloca %struct.Color, align 8, !dbg !160
  store %struct.Color %setColor152, %struct.Color* %tmp_struct153, align 1, !dbg !160
  %st_slot154 = alloca %struct.Color, align 8, !dbg !160
  store %struct.Color %setColor152, %struct.Color* %st_slot154, align 1, !dbg !160
  %i64_slot155 = alloca i64, align 8, !dbg !160
  %20 = bitcast i64* %i64_slot155 to i8*, !dbg !160
  %21 = bitcast %struct.Color* %st_slot154 to i8*, !dbg !160
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %20, i8* %21, i64 4, i1 false), !dbg !160
  %i64_arg156 = load i64, i64* %i64_slot155, align 4, !dbg !160
  call void @DrawText(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @STR9, i32 0, i32 0), i32 %px149, i32 %ADD151, i32 12, i64 %i64_arg156), !dbg !160
  %px157 = load i32, i32* %px, align 4, !dbg !161
  %py158 = load i32, i32* %py, align 4, !dbg !161
  %ADD159 = add i32 %py158, 386, !dbg !161
  %setColor160 = call %struct.Color @setColor(i32 80, i32 80, i32 80, i32 255), !dbg !161
  %tmp_struct161 = alloca %struct.Color, align 8, !dbg !161
  store %struct.Color %setColor160, %struct.Color* %tmp_struct161, align 1, !dbg !161
  %st_slot162 = alloca %struct.Color, align 8, !dbg !161
  store %struct.Color %setColor160, %struct.Color* %st_slot162, align 1, !dbg !161
  %i64_slot163 = alloca i64, align 8, !dbg !161
  %22 = bitcast i64* %i64_slot163 to i8*, !dbg !161
  %23 = bitcast %struct.Color* %st_slot162 to i8*, !dbg !161
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %22, i8* %23, i64 4, i1 false), !dbg !161
  %i64_arg164 = load i64, i64* %i64_slot163, align 4, !dbg !161
  call void @DrawText(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR10, i32 0, i32 0), i32 %px157, i32 %ADD159, i32 12, i64 %i64_arg164), !dbg !161
  %px165 = load i32, i32* %px, align 4, !dbg !162
  %py166 = load i32, i32* %py, align 4, !dbg !162
  %ADD167 = add i32 %py166, 402, !dbg !162
  %setColor168 = call %struct.Color @setColor(i32 80, i32 80, i32 80, i32 255), !dbg !162
  %tmp_struct169 = alloca %struct.Color, align 8, !dbg !162
  store %struct.Color %setColor168, %struct.Color* %tmp_struct169, align 1, !dbg !162
  %st_slot170 = alloca %struct.Color, align 8, !dbg !162
  store %struct.Color %setColor168, %struct.Color* %st_slot170, align 1, !dbg !162
  %i64_slot171 = alloca i64, align 8, !dbg !162
  %24 = bitcast i64* %i64_slot171 to i8*, !dbg !162
  %25 = bitcast %struct.Color* %st_slot170 to i8*, !dbg !162
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %24, i8* %25, i64 4, i1 false), !dbg !162
  %i64_arg172 = load i64, i64* %i64_slot171, align 4, !dbg !162
  call void @DrawText(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @STR11, i32 0, i32 0), i32 %px165, i32 %ADD167, i32 12, i64 %i64_arg172), !dbg !162
  %px173 = load i32, i32* %px, align 4, !dbg !163
  %py174 = load i32, i32* %py, align 4, !dbg !163
  %ADD175 = add i32 %py174, 418, !dbg !163
  %setColor176 = call %struct.Color @setColor(i32 80, i32 80, i32 80, i32 255), !dbg !163
  %tmp_struct177 = alloca %struct.Color, align 8, !dbg !163
  store %struct.Color %setColor176, %struct.Color* %tmp_struct177, align 1, !dbg !163
  %st_slot178 = alloca %struct.Color, align 8, !dbg !163
  store %struct.Color %setColor176, %struct.Color* %st_slot178, align 1, !dbg !163
  %i64_slot179 = alloca i64, align 8, !dbg !163
  %26 = bitcast i64* %i64_slot179 to i8*, !dbg !163
  %27 = bitcast %struct.Color* %st_slot178 to i8*, !dbg !163
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %26, i8* %27, i64 4, i1 false), !dbg !163
  %i64_arg180 = load i64, i64* %i64_slot179, align 4, !dbg !163
  call void @DrawText(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR12, i32 0, i32 0), i32 %px173, i32 %ADD175, i32 12, i64 %i64_arg180), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct7), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct16), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct24), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct35), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct43), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct54), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct62), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct76), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct87), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct153), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct161), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct169), !dbg !163
  call void @Color.delete(%struct.Color* %tmp_struct177), !dbg !163
  ret void, !dbg !163

while.start92:                                    ; preds = %if.end, %while.then
  %c95 = load i32, i32* %c, align 4, !dbg !149
  %LT96 = icmp slt i32 %c95, 4, !dbg !149
  br i1 %LT96, label %while.then93, label %while.end94, !dbg !149

while.then93:                                     ; preds = %while.start92
  br label %if.start, !dbg !149

while.end94:                                      ; preds = %while.start92
  %r147 = load i32, i32* %r, align 4, !dbg !164
  %ADD148 = add i32 %r147, 1, !dbg !164
  store i32 %ADD148, i32* %r, align 4, !dbg !164
  br label %while.start, !dbg !164

if.start:                                         ; preds = %while.then93
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !165
  %gs97 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !165
  %next = getelementptr %struct.Game, %struct.Game* %gs97, i32 0, i32 3, !dbg !165
  %type = getelementptr %struct.Piece, %struct.Piece* %next, i32 0, i32 0, !dbg !165
  %DOT98 = load i32, i32* %type, align 4, !dbg !165
  %gs99 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !165
  %next100 = getelementptr %struct.Game, %struct.Game* %gs99, i32 0, i32 3, !dbg !165
  %rotation = getelementptr %struct.Piece, %struct.Piece* %next100, i32 0, i32 1, !dbg !165
  %DOT101 = load i32, i32* %rotation, align 4, !dbg !165
  %r102 = load i32, i32* %r, align 4, !dbg !165
  %c103 = load i32, i32* %c, align 4, !dbg !165
  %cell_occupied = call i1 @cell_occupied(%struct.Game* %ref_arg, i32 %DOT98, i32 %DOT101, i32 %r102, i32 %c103), !dbg !165
  br i1 %cell_occupied, label %if.then, label %if.end, !dbg !165

if.end:                                           ; preds = %if.then, %if.start
  %c145 = load i32, i32* %c, align 4, !dbg !164
  %ADD146 = add i32 %c145, 1, !dbg !164
  store i32 %ADD146, i32* %c, align 4, !dbg !164
  br label %while.start92, !dbg !164

if.then:                                          ; preds = %if.start
  store i32 0, i32* %x, align 4, !dbg !149
  %c104 = load i32, i32* %c, align 4, !dbg !149
  %cs105 = load i32, i32* %cs, align 4, !dbg !149
  %MUL106 = mul i32 %c104, %cs105, !dbg !149
  %bx107 = load i32, i32* %bx, align 4, !dbg !149
  %ADD108 = add i32 %bx107, %MUL106, !dbg !149
  %ADD109 = add i32 %ADD108, 1, !dbg !149
  store i32 %ADD109, i32* %x, align 4, !dbg !149
  store i32 0, i32* %y, align 4, !dbg !149
  %r110 = load i32, i32* %r, align 4, !dbg !149
  %cs111 = load i32, i32* %cs, align 4, !dbg !149
  %MUL112 = mul i32 %r110, %cs111, !dbg !149
  %by113 = load i32, i32* %by, align 4, !dbg !149
  %ADD114 = add i32 %by113, %MUL112, !dbg !149
  %ADD115 = add i32 %ADD114, 1, !dbg !149
  store i32 %ADD115, i32* %y, align 4, !dbg !149
  store %struct.Color zeroinitializer, %struct.Color* %col, align 1, !dbg !149
  %gs116 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !166
  %next117 = getelementptr %struct.Game, %struct.Game* %gs116, i32 0, i32 3, !dbg !166
  %type118 = getelementptr %struct.Piece, %struct.Piece* %next117, i32 0, i32 0, !dbg !166
  %DOT119 = load i32, i32* %type118, align 4, !dbg !166
  %piece_color = call %struct.Color @piece_color(i32 %DOT119), !dbg !166
  %tmp_struct120 = alloca %struct.Color, align 8, !dbg !166
  store %struct.Color %piece_color, %struct.Color* %tmp_struct120, align 1, !dbg !166
  store %struct.Color %piece_color, %struct.Color* %col, align 1, !dbg !166
  %x121 = load i32, i32* %x, align 4, !dbg !167
  %y122 = load i32, i32* %y, align 4, !dbg !167
  %cs123 = load i32, i32* %cs, align 4, !dbg !167
  %SUB124 = sub i32 %cs123, 2, !dbg !167
  %cs125 = load i32, i32* %cs, align 4, !dbg !167
  %SUB126 = sub i32 %cs125, 2, !dbg !167
  %col127 = load %struct.Color, %struct.Color* %col, align 1, !dbg !167
  %st_slot128 = alloca %struct.Color, align 8, !dbg !167
  store %struct.Color %col127, %struct.Color* %st_slot128, align 1, !dbg !167
  %i64_slot129 = alloca i64, align 8, !dbg !167
  %28 = bitcast i64* %i64_slot129 to i8*, !dbg !167
  %29 = bitcast %struct.Color* %st_slot128 to i8*, !dbg !167
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %28, i8* %29, i64 4, i1 false), !dbg !167
  %i64_arg130 = load i64, i64* %i64_slot129, align 4, !dbg !167
  call void @DrawRectangle(i32 %x121, i32 %y122, i32 %SUB124, i32 %SUB126, i64 %i64_arg130), !dbg !167
  %x131 = load i32, i32* %x, align 4, !dbg !164
  %y132 = load i32, i32* %y, align 4, !dbg !164
  %cs133 = load i32, i32* %cs, align 4, !dbg !164
  %SUB134 = sub i32 %cs133, 2, !dbg !164
  %cs135 = load i32, i32* %cs, align 4, !dbg !164
  %SUB136 = sub i32 %cs135, 2, !dbg !164
  %col137 = load %struct.Color, %struct.Color* %col, align 1, !dbg !164
  %st_slot138 = alloca %struct.Color, align 8, !dbg !164
  store %struct.Color %col137, %struct.Color* %st_slot138, align 1, !dbg !164
  %i64_slot139 = alloca i64, align 8, !dbg !164
  %30 = bitcast i64* %i64_slot139 to i8*, !dbg !164
  %31 = bitcast %struct.Color* %st_slot138 to i8*, !dbg !164
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %30, i8* %31, i64 4, i1 false), !dbg !164
  %i64_arg140 = load i64, i64* %i64_slot139, align 4, !dbg !164
  %ColorBrightness = call i64 @ColorBrightness(i64 %i64_arg140, float 0x3FD3333340000000), !dbg !164
  %ret_i64 = alloca i64, align 8, !dbg !164
  store i64 %ColorBrightness, i64* %ret_i64, align 4, !dbg !164
  %ret_struct = alloca %struct.Color, align 8, !dbg !164
  %32 = bitcast %struct.Color* %ret_struct to i8*, !dbg !164
  %33 = bitcast i64* %ret_i64 to i8*, !dbg !164
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %32, i8* %33, i64 4, i1 false), !dbg !164
  %ret_struct_val = load %struct.Color, %struct.Color* %ret_struct, align 1, !dbg !164
  %tmp_struct141 = alloca %struct.Color, align 8, !dbg !164
  store %struct.Color %ret_struct_val, %struct.Color* %tmp_struct141, align 1, !dbg !164
  %st_slot142 = alloca %struct.Color, align 8, !dbg !164
  store %struct.Color %ret_struct_val, %struct.Color* %st_slot142, align 1, !dbg !164
  %i64_slot143 = alloca i64, align 8, !dbg !164
  %34 = bitcast i64* %i64_slot143 to i8*, !dbg !164
  %35 = bitcast %struct.Color* %st_slot142 to i8*, !dbg !164
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %34, i8* %35, i64 4, i1 false), !dbg !164
  %i64_arg144 = load i64, i64* %i64_slot143, align 4, !dbg !164
  call void @DrawRectangleLines(i32 %x131, i32 %y132, i32 %SUB134, i32 %SUB136, i64 %i64_arg144), !dbg !164
  br label %if.end, !dbg !164
}

define void @draw_game_over(%struct.Game* %gs) !dbg !168 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !169
  %cx = alloca i32, align 4, !dbg !169
  %cy = alloca i32, align 4, !dbg !169
  %txt = alloca i8*, align 8, !dbg !169
  %stxt = alloca i8*, align 8, !dbg !169
  %rtxt = alloca i8*, align 8, !dbg !169
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !169
  %BOARD_X = load i32, i32* @BOARD_X, align 4, !dbg !170
  %BOARD_Y = load i32, i32* @BOARD_Y, align 4, !dbg !170
  %BOARD_COLS = load i32, i32* @BOARD_COLS, align 4, !dbg !170
  %CELL = load i32, i32* @CELL, align 4, !dbg !170
  %MUL = mul i32 %BOARD_COLS, %CELL, !dbg !170
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !170
  %CELL2 = load i32, i32* @CELL, align 4, !dbg !170
  %MUL3 = mul i32 %BOARD_ROWS, %CELL2, !dbg !170
  %setColor = call %struct.Color @setColor(i32 0, i32 0, i32 0, i32 170), !dbg !170
  %tmp_struct = alloca %struct.Color, align 8, !dbg !170
  store %struct.Color %setColor, %struct.Color* %tmp_struct, align 1, !dbg !170
  %st_slot = alloca %struct.Color, align 8, !dbg !170
  store %struct.Color %setColor, %struct.Color* %st_slot, align 1, !dbg !170
  %i64_slot = alloca i64, align 8, !dbg !170
  %0 = bitcast i64* %i64_slot to i8*, !dbg !170
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !170
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !170
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !170
  call void @DrawRectangle(i32 %BOARD_X, i32 %BOARD_Y, i32 %MUL, i32 %MUL3, i64 %i64_arg), !dbg !170
  store i32 0, i32* %cx, align 4, !dbg !169
  %BOARD_COLS4 = load i32, i32* @BOARD_COLS, align 4, !dbg !169
  %CELL5 = load i32, i32* @CELL, align 4, !dbg !169
  %MUL6 = mul i32 %BOARD_COLS4, %CELL5, !dbg !169
  %DIV = sdiv i32 %MUL6, 2, !dbg !169
  %BOARD_X7 = load i32, i32* @BOARD_X, align 4, !dbg !169
  %ADD = add i32 %BOARD_X7, %DIV, !dbg !169
  store i32 %ADD, i32* %cx, align 4, !dbg !169
  store i32 0, i32* %cy, align 4, !dbg !169
  %BOARD_ROWS8 = load i32, i32* @BOARD_ROWS, align 4, !dbg !169
  %CELL9 = load i32, i32* @CELL, align 4, !dbg !169
  %MUL10 = mul i32 %BOARD_ROWS8, %CELL9, !dbg !169
  %DIV11 = sdiv i32 %MUL10, 2, !dbg !169
  %BOARD_Y12 = load i32, i32* @BOARD_Y, align 4, !dbg !169
  %ADD13 = add i32 %BOARD_Y12, %DIV11, !dbg !169
  store i32 %ADD13, i32* %cy, align 4, !dbg !169
  store i8* null, i8** %txt, align 8, !dbg !169
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR13, i32 0, i32 0), i8** %txt, align 8, !dbg !169
  %txt14 = load i8*, i8** %txt, align 8, !dbg !171
  %txt15 = load i8*, i8** %txt, align 8, !dbg !171
  %MeasureText = call i32 @MeasureText(i8* %txt15, i32 28), !dbg !171
  %DIV16 = sdiv i32 %MeasureText, 2, !dbg !171
  %cx17 = load i32, i32* %cx, align 4, !dbg !171
  %SUB = sub i32 %cx17, %DIV16, !dbg !171
  %cy18 = load i32, i32* %cy, align 4, !dbg !171
  %SUB19 = sub i32 %cy18, 50, !dbg !171
  %setColor20 = call %struct.Color @setColor(i32 230, i32 41, i32 55, i32 255), !dbg !171
  %tmp_struct21 = alloca %struct.Color, align 8, !dbg !171
  store %struct.Color %setColor20, %struct.Color* %tmp_struct21, align 1, !dbg !171
  %st_slot22 = alloca %struct.Color, align 8, !dbg !171
  store %struct.Color %setColor20, %struct.Color* %st_slot22, align 1, !dbg !171
  %i64_slot23 = alloca i64, align 8, !dbg !171
  %2 = bitcast i64* %i64_slot23 to i8*, !dbg !171
  %3 = bitcast %struct.Color* %st_slot22 to i8*, !dbg !171
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i1 false), !dbg !171
  %i64_arg24 = load i64, i64* %i64_slot23, align 4, !dbg !171
  call void @DrawText(i8* %txt14, i32 %SUB, i32 %SUB19, i32 28, i64 %i64_arg24), !dbg !171
  store i8* null, i8** %stxt, align 8, !dbg !169
  %gs25 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !172
  %score = getelementptr %struct.Game, %struct.Game* %gs25, i32 0, i32 4, !dbg !172
  %DOT = load i32, i32* %score, align 4, !dbg !172
  %TextFormat = call i8* (i8*, i32, ...) @TextFormat(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR14, i32 0, i32 0), i32 1, i32 %DOT), !dbg !172
  store i8* %TextFormat, i8** %stxt, align 8, !dbg !172
  %stxt26 = load i8*, i8** %stxt, align 8, !dbg !173
  %stxt27 = load i8*, i8** %stxt, align 8, !dbg !173
  %MeasureText28 = call i32 @MeasureText(i8* %stxt27, i32 18), !dbg !173
  %DIV29 = sdiv i32 %MeasureText28, 2, !dbg !173
  %cx30 = load i32, i32* %cx, align 4, !dbg !173
  %SUB31 = sub i32 %cx30, %DIV29, !dbg !173
  %cy32 = load i32, i32* %cy, align 4, !dbg !173
  %SUB33 = sub i32 %cy32, 10, !dbg !173
  %setColor34 = call %struct.Color @setColor(i32 255, i32 255, i32 255, i32 255), !dbg !173
  %tmp_struct35 = alloca %struct.Color, align 8, !dbg !173
  store %struct.Color %setColor34, %struct.Color* %tmp_struct35, align 1, !dbg !173
  %st_slot36 = alloca %struct.Color, align 8, !dbg !173
  store %struct.Color %setColor34, %struct.Color* %st_slot36, align 1, !dbg !173
  %i64_slot37 = alloca i64, align 8, !dbg !173
  %4 = bitcast i64* %i64_slot37 to i8*, !dbg !173
  %5 = bitcast %struct.Color* %st_slot36 to i8*, !dbg !173
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* %5, i64 4, i1 false), !dbg !173
  %i64_arg38 = load i64, i64* %i64_slot37, align 4, !dbg !173
  call void @DrawText(i8* %stxt26, i32 %SUB31, i32 %SUB33, i32 18, i64 %i64_arg38), !dbg !173
  store i8* null, i8** %rtxt, align 8, !dbg !169
  store i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR15, i32 0, i32 0), i8** %rtxt, align 8, !dbg !169
  %rtxt39 = load i8*, i8** %rtxt, align 8, !dbg !174
  %rtxt40 = load i8*, i8** %rtxt, align 8, !dbg !174
  %MeasureText41 = call i32 @MeasureText(i8* %rtxt40, i32 14), !dbg !174
  %DIV42 = sdiv i32 %MeasureText41, 2, !dbg !174
  %cx43 = load i32, i32* %cx, align 4, !dbg !174
  %SUB44 = sub i32 %cx43, %DIV42, !dbg !174
  %cy45 = load i32, i32* %cy, align 4, !dbg !174
  %ADD46 = add i32 %cy45, 20, !dbg !174
  %setColor47 = call %struct.Color @setColor(i32 200, i32 200, i32 200, i32 255), !dbg !174
  %tmp_struct48 = alloca %struct.Color, align 8, !dbg !174
  store %struct.Color %setColor47, %struct.Color* %tmp_struct48, align 1, !dbg !174
  %st_slot49 = alloca %struct.Color, align 8, !dbg !174
  store %struct.Color %setColor47, %struct.Color* %st_slot49, align 1, !dbg !174
  %i64_slot50 = alloca i64, align 8, !dbg !174
  %6 = bitcast i64* %i64_slot50 to i8*, !dbg !174
  %7 = bitcast %struct.Color* %st_slot49 to i8*, !dbg !174
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* %7, i64 4, i1 false), !dbg !174
  %i64_arg51 = load i64, i64* %i64_slot50, align 4, !dbg !174
  call void @DrawText(i8* %rtxt39, i32 %SUB44, i32 %ADD46, i32 14, i64 %i64_arg51), !dbg !174
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !174
  call void @Color.delete(%struct.Color* %tmp_struct21), !dbg !174
  call void @Color.delete(%struct.Color* %tmp_struct35), !dbg !174
  call void @Color.delete(%struct.Color* %tmp_struct48), !dbg !174
  ret void, !dbg !174
}

define void @draw_pause() !dbg !175 {
entry:
  %cx = alloca i32, align 4, !dbg !176
  %cy = alloca i32, align 4, !dbg !176
  %txt = alloca i8*, align 8, !dbg !176
  %BOARD_X = load i32, i32* @BOARD_X, align 4, !dbg !176
  %BOARD_Y = load i32, i32* @BOARD_Y, align 4, !dbg !176
  %BOARD_COLS = load i32, i32* @BOARD_COLS, align 4, !dbg !176
  %CELL = load i32, i32* @CELL, align 4, !dbg !176
  %MUL = mul i32 %BOARD_COLS, %CELL, !dbg !176
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !176
  %CELL1 = load i32, i32* @CELL, align 4, !dbg !176
  %MUL2 = mul i32 %BOARD_ROWS, %CELL1, !dbg !176
  %setColor = call %struct.Color @setColor(i32 0, i32 0, i32 0, i32 140), !dbg !176
  %tmp_struct = alloca %struct.Color, align 8, !dbg !176
  store %struct.Color %setColor, %struct.Color* %tmp_struct, align 1, !dbg !176
  %st_slot = alloca %struct.Color, align 8, !dbg !176
  store %struct.Color %setColor, %struct.Color* %st_slot, align 1, !dbg !176
  %i64_slot = alloca i64, align 8, !dbg !176
  %0 = bitcast i64* %i64_slot to i8*, !dbg !176
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !176
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !176
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !176
  call void @DrawRectangle(i32 %BOARD_X, i32 %BOARD_Y, i32 %MUL, i32 %MUL2, i64 %i64_arg), !dbg !176
  store i32 0, i32* %cx, align 4, !dbg !176
  %BOARD_COLS3 = load i32, i32* @BOARD_COLS, align 4, !dbg !176
  %CELL4 = load i32, i32* @CELL, align 4, !dbg !176
  %MUL5 = mul i32 %BOARD_COLS3, %CELL4, !dbg !176
  %DIV = sdiv i32 %MUL5, 2, !dbg !176
  %BOARD_X6 = load i32, i32* @BOARD_X, align 4, !dbg !176
  %ADD = add i32 %BOARD_X6, %DIV, !dbg !176
  store i32 %ADD, i32* %cx, align 4, !dbg !176
  store i32 0, i32* %cy, align 4, !dbg !176
  %BOARD_ROWS7 = load i32, i32* @BOARD_ROWS, align 4, !dbg !176
  %CELL8 = load i32, i32* @CELL, align 4, !dbg !176
  %MUL9 = mul i32 %BOARD_ROWS7, %CELL8, !dbg !176
  %DIV10 = sdiv i32 %MUL9, 2, !dbg !176
  %BOARD_Y11 = load i32, i32* @BOARD_Y, align 4, !dbg !176
  %ADD12 = add i32 %BOARD_Y11, %DIV10, !dbg !176
  store i32 %ADD12, i32* %cy, align 4, !dbg !176
  store i8* null, i8** %txt, align 8, !dbg !176
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR16, i32 0, i32 0), i8** %txt, align 8, !dbg !176
  %txt13 = load i8*, i8** %txt, align 8, !dbg !177
  %txt14 = load i8*, i8** %txt, align 8, !dbg !177
  %MeasureText = call i32 @MeasureText(i8* %txt14, i32 28), !dbg !177
  %DIV15 = sdiv i32 %MeasureText, 2, !dbg !177
  %cx16 = load i32, i32* %cx, align 4, !dbg !177
  %SUB = sub i32 %cx16, %DIV15, !dbg !177
  %cy17 = load i32, i32* %cy, align 4, !dbg !177
  %SUB18 = sub i32 %cy17, 14, !dbg !177
  %setColor19 = call %struct.Color @setColor(i32 253, i32 249, i32 0, i32 255), !dbg !177
  %tmp_struct20 = alloca %struct.Color, align 8, !dbg !177
  store %struct.Color %setColor19, %struct.Color* %tmp_struct20, align 1, !dbg !177
  %st_slot21 = alloca %struct.Color, align 8, !dbg !177
  store %struct.Color %setColor19, %struct.Color* %st_slot21, align 1, !dbg !177
  %i64_slot22 = alloca i64, align 8, !dbg !177
  %2 = bitcast i64* %i64_slot22 to i8*, !dbg !177
  %3 = bitcast %struct.Color* %st_slot21 to i8*, !dbg !177
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i1 false), !dbg !177
  %i64_arg23 = load i64, i64* %i64_slot22, align 4, !dbg !177
  call void @DrawText(i8* %txt13, i32 %SUB, i32 %SUB18, i32 28, i64 %i64_arg23), !dbg !177
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !177
  call void @Color.delete(%struct.Color* %tmp_struct20), !dbg !177
  ret void, !dbg !177
}

define void @render(%struct.Game* %gs) !dbg !178 {
entry:
  %gs1 = alloca %struct.Game*, align 8, !dbg !179
  store %struct.Game* %gs, %struct.Game** %gs1, align 8, !dbg !179
  %setColor = call %struct.Color @setColor(i32 15, i32 15, i32 22, i32 255), !dbg !180
  %tmp_struct = alloca %struct.Color, align 8, !dbg !180
  store %struct.Color %setColor, %struct.Color* %tmp_struct, align 1, !dbg !180
  %st_slot = alloca %struct.Color, align 8, !dbg !180
  store %struct.Color %setColor, %struct.Color* %st_slot, align 1, !dbg !180
  %i64_slot = alloca i64, align 8, !dbg !180
  %0 = bitcast i64* %i64_slot to i8*, !dbg !180
  %1 = bitcast %struct.Color* %st_slot to i8*, !dbg !180
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i1 false), !dbg !180
  %i64_arg = load i64, i64* %i64_slot, align 4, !dbg !180
  call void @ClearBackground(i64 %i64_arg), !dbg !180
  %ref_arg = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !181
  call void @draw_board(%struct.Game* %ref_arg), !dbg !181
  br label %if.start, !dbg !181

if.start:                                         ; preds = %entry
  %gs2 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !181
  %over = getelementptr %struct.Game, %struct.Game* %gs2, i32 0, i32 9, !dbg !181
  %DOT = load i1, i1* %over, align 1, !dbg !181
  %NOT = xor i1 %DOT, true, !dbg !181
  br i1 %NOT, label %if.then, label %if.end, !dbg !181

if.end:                                           ; preds = %if.then, %if.start
  %ref_arg5 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !182
  call void @draw_hud(%struct.Game* %ref_arg5), !dbg !182
  br label %if.start6, !dbg !182

if.then:                                          ; preds = %if.start
  %ref_arg3 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !183
  call void @draw_ghost(%struct.Game* %ref_arg3), !dbg !183
  %ref_arg4 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !184
  call void @draw_current(%struct.Game* %ref_arg4), !dbg !184
  br label %if.end, !dbg !184

if.start6:                                        ; preds = %if.end
  %gs9 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !182
  %over10 = getelementptr %struct.Game, %struct.Game* %gs9, i32 0, i32 9, !dbg !182
  %DOT11 = load i1, i1* %over10, align 1, !dbg !182
  br i1 %DOT11, label %if.then8, label %if.end7, !dbg !182

if.end7:                                          ; preds = %if.then8, %if.start6
  br label %if.start13, !dbg !185

if.then8:                                         ; preds = %if.start6
  %ref_arg12 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !185
  call void @draw_game_over(%struct.Game* %ref_arg12), !dbg !185
  br label %if.end7, !dbg !185

if.start13:                                       ; preds = %if.end7
  %gs16 = load %struct.Game*, %struct.Game** %gs1, align 8, !dbg !185
  %paused = getelementptr %struct.Game, %struct.Game* %gs16, i32 0, i32 10, !dbg !185
  %DOT17 = load i1, i1* %paused, align 1, !dbg !185
  br i1 %DOT17, label %if.then15, label %if.end14, !dbg !185

if.end14:                                         ; preds = %if.then15, %if.start13
  call void @Color.delete(%struct.Color* %tmp_struct), !dbg !186
  ret void, !dbg !186

if.then15:                                        ; preds = %if.start13
  call void @draw_pause(), !dbg !186
  br label %if.end14, !dbg !186
}

define i32 @main() !dbg !187 {
entry:
  %gs = alloca %struct.Game, align 8, !dbg !188
  %dt = alloca float, align 4, !dbg !188
  %time = call i64 @time(i8* null), !dbg !188
  %as = trunc i64 %time to i32, !dbg !188
  call void @srand(i32 %as), !dbg !188
  %WIN_W = load i32, i32* @WIN_W, align 4, !dbg !189
  %WIN_H = load i32, i32* @WIN_H, align 4, !dbg !189
  call void @InitWindow(i32 %WIN_W, i32 %WIN_H, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR1, i32 0, i32 0)), !dbg !189
  call void @SetTargetFPS(i32 60), !dbg !190
  store %struct.Game zeroinitializer, %struct.Game* %gs, align 8, !dbg !188
  %board = getelementptr %struct.Game, %struct.Game* %gs, i32 0, i32 0, !dbg !188
  %BOARD_ROWS = load i32, i32* @BOARD_ROWS, align 4, !dbg !188
  %dim = mul i32 1, %BOARD_ROWS, !dbg !188
  %BOARD_COLS = load i32, i32* @BOARD_COLS, align 4, !dbg !188
  %dim1 = mul i32 %dim, %BOARD_COLS, !dbg !188
  %bytes = mul i32 %dim1, 4, !dbg !188
  %count = zext i32 %bytes to i64, !dbg !188
  %heap = call i8* @calloc(i64 %count, i64 4), !dbg !188
  store i32 %BOARD_ROWS, i32* @Game.board.dim0, align 4, !dbg !188
  store i32 %BOARD_COLS, i32* @Game.board.dim1, align 4, !dbg !188
  %field_cast = bitcast i8* %heap to i32*, !dbg !188
  store i32* %field_cast, i32** %board, align 8, !dbg !188
  %shapes = getelementptr %struct.Game, %struct.Game* %gs, i32 0, i32 1, !dbg !188
  %PIECE_COUNT = load i32, i32* @PIECE_COUNT, align 4, !dbg !188
  %dim2 = mul i32 1, %PIECE_COUNT, !dbg !188
  %dim3 = mul i32 %dim2, 4, !dbg !188
  %dim4 = mul i32 %dim3, 4, !dbg !188
  %bytes5 = mul i32 %dim4, 4, !dbg !188
  %count6 = zext i32 %bytes5 to i64, !dbg !188
  %heap7 = call i8* @calloc(i64 %count6, i64 4), !dbg !188
  store i32 %PIECE_COUNT, i32* @Game.shapes.dim0, align 4, !dbg !188
  store i32 4, i32* @Game.shapes.dim1, align 4, !dbg !188
  store i32 4, i32* @Game.shapes.dim2, align 4, !dbg !188
  %field_cast8 = bitcast i8* %heap7 to i32*, !dbg !188
  store i32* %field_cast8, i32** %shapes, align 8, !dbg !188
  call void @init_shapes(%struct.Game* %gs), !dbg !191
  call void @init_game(%struct.Game* %gs), !dbg !192
  br label %while.start, !dbg !192

while.start:                                      ; preds = %if.end10, %entry
  %WindowShouldClose = call i1 @WindowShouldClose(), !dbg !193
  %EQ = icmp eq i1 %WindowShouldClose, false, !dbg !193
  br i1 %EQ, label %while.then, label %while.end, !dbg !193

while.then:                                       ; preds = %while.start
  store float 0.000000e+00, float* %dt, align 4, !dbg !188
  %GetFrameTime = call float @GetFrameTime(), !dbg !194
  store float %GetFrameTime, float* %dt, align 4, !dbg !194
  call void @handle_input(%struct.Game* %gs), !dbg !195
  br label %if.start, !dbg !195

while.end:                                        ; preds = %while.start
  call void @CloseWindow(), !dbg !196
  call void @Game.delete(%struct.Game* %gs), !dbg !196
  ret i32 0, !dbg !196

if.start:                                         ; preds = %while.then
  %over = getelementptr %struct.Game, %struct.Game* %gs, i32 0, i32 9, !dbg !195
  %KEY_R = load i32, i32* @KEY_R, align 4, !dbg !197
  %IsKeyPressed = call i1 @IsKeyPressed(i32 %KEY_R), !dbg !197
  %DOT = load i1, i1* %over, align 1, !dbg !197
  %AND = and i1 %DOT, %IsKeyPressed, !dbg !197
  br i1 %AND, label %if.then, label %if.end, !dbg !197

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start9, !dbg !198

if.then:                                          ; preds = %if.start
  call void @init_game(%struct.Game* %gs), !dbg !198
  br label %if.end, !dbg !198

if.start9:                                        ; preds = %if.end
  %over12 = getelementptr %struct.Game, %struct.Game* %gs, i32 0, i32 9, !dbg !198
  %paused = getelementptr %struct.Game, %struct.Game* %gs, i32 0, i32 10, !dbg !198
  %DOT13 = load i1, i1* %paused, align 1, !dbg !198
  %NOT = xor i1 %DOT13, true, !dbg !198
  %DOT14 = load i1, i1* %over12, align 1, !dbg !198
  %AND15 = and i1 %DOT14, %NOT, !dbg !198
  %NOT16 = xor i1 %AND15, true, !dbg !198
  br i1 %NOT16, label %if.then11, label %if.end10, !dbg !198

if.end10:                                         ; preds = %if.then11, %if.start9
  call void @BeginDrawing(), !dbg !199
  call void @render(%struct.Game* %gs), !dbg !200
  call void @EndDrawing(), !dbg !201
  br label %while.start, !dbg !201

if.then11:                                        ; preds = %if.start9
  %dt17 = load float, float* %dt, align 4, !dbg !202
  call void @update_game(%struct.Game* %gs, float %dt17), !dbg !202
  br label %if.end10, !dbg !202
}

define void @String.delete(%struct.String* %self) !dbg !203 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !204
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !204
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !205
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !205
  %DOT = load i8*, i8** %value, align 8, !dbg !205
  call void @free(i8* %DOT), !dbg !205
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !205
  ret void, !dbg !205
}

define void @String.assign(i8* %str, %struct.String* %self) !dbg !206 {
entry:
  %str1 = alloca i8*, align 8, !dbg !207
  %self2 = alloca %struct.String*, align 8, !dbg !207
  %len = alloca i32, align 4, !dbg !207
  store i8* %str, i8** %str1, align 8, !dbg !207
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !207
  br label %if.start, !dbg !207

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !207
  %EQ = icmp eq i8* %str3, null, !dbg !207
  br i1 %EQ, label %if.then, label %if.end, !dbg !207

if.end:                                           ; preds = %if.then, %if.start
  store i32 0, i32* %len, align 4, !dbg !207
  %str4 = load i8*, i8** %str1, align 8, !dbg !208
  %strlen = call i32 @strlen(i8* %str4), !dbg !208
  store i32 %strlen, i32* %len, align 4, !dbg !208
  br label %if.start5, !dbg !208

if.then:                                          ; preds = %if.start
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @STR0, i32 0, i32 0), i8** %str1, align 8, !dbg !207
  br label %if.end, !dbg !207

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !208
  %value = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 0, !dbg !208
  %DOT = load i8*, i8** %value, align 8, !dbg !208
  %EQ9 = icmp eq i8* %DOT, null, !dbg !208
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !208
  %size = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 2, !dbg !208
  %DOT11 = load i32, i32* %size, align 4, !dbg !208
  %len12 = load i32, i32* %len, align 4, !dbg !208
  %LE = icmp sle i32 %DOT11, %len12, !dbg !208
  %OR = or i1 %EQ9, %LE, !dbg !208
  br i1 %OR, label %if.then7, label %if.end6, !dbg !208

if.end6:                                          ; preds = %while.end, %if.start5
  %self38 = load %struct.String*, %struct.String** %self2, align 8, !dbg !209
  %value39 = getelementptr %struct.String, %struct.String* %self38, i32 0, i32 0, !dbg !209
  %DOT40 = load i8*, i8** %value39, align 8, !dbg !209
  %str41 = load i8*, i8** %str1, align 8, !dbg !209
  %strcpy = call i8* @strcpy(i8* %DOT40, i8* %str41), !dbg !209
  %self42 = load %struct.String*, %struct.String** %self2, align 8, !dbg !210
  %value43 = getelementptr %struct.String, %struct.String* %self42, i32 0, i32 0, !dbg !210
  %DOT44 = load i8*, i8** %value43, align 8, !dbg !210
  %len45 = load i32, i32* %len, align 4, !dbg !210
  %idx = sext i32 %len45 to i64, !dbg !210
  %ADD = getelementptr i8, i8* %DOT44, i64 %idx, !dbg !210
  %self46 = load %struct.String*, %struct.String** %self2, align 8, !dbg !210
  %size47 = getelementptr %struct.String, %struct.String* %self46, i32 0, i32 2, !dbg !210
  %DOT48 = load i32, i32* %size47, align 4, !dbg !210
  %len49 = load i32, i32* %len, align 4, !dbg !210
  %SUB = sub i32 %DOT48, %len49, !dbg !210
  %as = sext i32 %SUB to i64, !dbg !210
  call void @bzero(i8* %ADD, i64 %as), !dbg !210
  %self50 = load %struct.String*, %struct.String** %self2, align 8, !dbg !210
  %count = getelementptr %struct.String, %struct.String* %self50, i32 0, i32 1, !dbg !210
  %len51 = load i32, i32* %len, align 4, !dbg !210
  store i32 %len51, i32* %count, align 4, !dbg !210
  ret void, !dbg !210

if.then7:                                         ; preds = %if.start5
  br label %if.start13, !dbg !208

if.start13:                                       ; preds = %if.then7
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !208
  %size17 = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 2, !dbg !208
  %DOT18 = load i32, i32* %size17, align 4, !dbg !208
  %EQ19 = icmp eq i32 %DOT18, 0, !dbg !208
  br i1 %EQ19, label %if.then15, label %if.end14, !dbg !208

if.end14:                                         ; preds = %if.then15, %if.start13
  br label %while.start, !dbg !208

if.then15:                                        ; preds = %if.start13
  %self20 = load %struct.String*, %struct.String** %self2, align 8, !dbg !208
  %size21 = getelementptr %struct.String, %struct.String* %self20, i32 0, i32 2, !dbg !208
  store i32 10, i32* %size21, align 4, !dbg !208
  br label %if.end14, !dbg !208

while.start:                                      ; preds = %while.then, %if.end14
  %self22 = load %struct.String*, %struct.String** %self2, align 8, !dbg !208
  %size23 = getelementptr %struct.String, %struct.String* %self22, i32 0, i32 2, !dbg !208
  %DOT24 = load i32, i32* %size23, align 4, !dbg !208
  %len25 = load i32, i32* %len, align 4, !dbg !208
  %LE26 = icmp sle i32 %DOT24, %len25, !dbg !208
  br i1 %LE26, label %while.then, label %while.end, !dbg !208

while.then:                                       ; preds = %while.start
  %self27 = load %struct.String*, %struct.String** %self2, align 8, !dbg !208
  %size28 = getelementptr %struct.String, %struct.String* %self27, i32 0, i32 2, !dbg !208
  %DOT29 = load i32, i32* %size28, align 4, !dbg !208
  %MUL = mul i32 %DOT29, 2, !dbg !208
  store i32 %MUL, i32* %size28, align 4, !dbg !208
  br label %while.start, !dbg !208

while.end:                                        ; preds = %while.start
  %self30 = load %struct.String*, %struct.String** %self2, align 8, !dbg !208
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !208
  %self32 = load %struct.String*, %struct.String** %self2, align 8, !dbg !211
  %value33 = getelementptr %struct.String, %struct.String* %self32, i32 0, i32 0, !dbg !211
  %DOT34 = load i8*, i8** %value33, align 8, !dbg !211
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !211
  %size36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 2, !dbg !211
  %DOT37 = load i32, i32* %size36, align 4, !dbg !211
  %realloc = call i8* @realloc(i8* %DOT34, i32 %DOT37), !dbg !211
  store i8* %realloc, i8** %value31, align 8, !dbg !211
  br label %if.end6, !dbg !211
}

define void @String.join(i8* %str, %struct.String* %self) !dbg !212 {
entry:
  %str1 = alloca i8*, align 8, !dbg !213
  %self2 = alloca %struct.String*, align 8, !dbg !213
  %res = alloca i8*, align 8, !dbg !213
  store i8* %str, i8** %str1, align 8, !dbg !213
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !213
  br label %if.start, !dbg !213

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !213
  %EQ = icmp eq i8* %str3, null, !dbg !213
  br i1 %EQ, label %if.then, label %if.end, !dbg !213

if.end:                                           ; preds = %if.start
  store i8* null, i8** %res, align 8, !dbg !213
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !214
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !214
  %DOT = load i8*, i8** %value, align 8, !dbg !214
  %str5 = load i8*, i8** %str1, align 8, !dbg !214
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str5), !dbg !214
  store i8* %strjoin, i8** %res, align 8, !dbg !214
  %res6 = load i8*, i8** %res, align 8, !dbg !215
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !215
  call void @String.assign(i8* %res6, %struct.String* %ref_arg), !dbg !215
  %res7 = load i8*, i8** %res, align 8, !dbg !216
  call void @free(i8* %res7), !dbg !216
  ret void, !dbg !216

if.then:                                          ; preds = %if.start
  ret void, !dbg !213
}

define void @String.clear(%struct.String* %self) !dbg !217 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !218
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !218
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !219
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !219
  %DOT = load i8*, i8** %value, align 8, !dbg !219
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !219
  %size = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 2, !dbg !219
  %DOT4 = load i32, i32* %size, align 4, !dbg !219
  %as = sext i32 %DOT4 to i64, !dbg !219
  call void @bzero(i8* %DOT, i64 %as), !dbg !219
  %self5 = load %struct.String*, %struct.String** %self1, align 8, !dbg !219
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !219
  store i32 0, i32* %count, align 4, !dbg !219
  ret void, !dbg !219
}

define void @String.push(i8 %c, %struct.String* %self) !dbg !220 {
entry:
  %c1 = alloca i8, align 1, !dbg !221
  %self2 = alloca %struct.String*, align 8, !dbg !221
  store i8 %c, i8* %c1, align 1, !dbg !221
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !221
  br label %if.start, !dbg !221

if.start:                                         ; preds = %entry
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !221
  %count = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !221
  %DOT = load i32, i32* %count, align 4, !dbg !221
  %ADD = add i32 %DOT, 1, !dbg !221
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !221
  %size = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 2, !dbg !221
  %DOT5 = load i32, i32* %size, align 4, !dbg !221
  %GE = icmp sge i32 %ADD, %DOT5, !dbg !221
  br i1 %GE, label %if.then, label %if.end, !dbg !221

if.end:                                           ; preds = %if.end7, %if.start
  %self24 = load %struct.String*, %struct.String** %self2, align 8, !dbg !222
  %value25 = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !222
  %self26 = load %struct.String*, %struct.String** %self2, align 8, !dbg !222
  %count27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 1, !dbg !222
  %DOT28 = load i8*, i8** %value25, align 8, !dbg !222
  %DOT29 = load i32, i32* %count27, align 4, !dbg !222
  %ACCESS = getelementptr i8, i8* %DOT28, i32 %DOT29, !dbg !222
  %c30 = load i8, i8* %c1, align 1, !dbg !222
  store i8 %c30, i8* %ACCESS, align 1, !dbg !222
  %self31 = load %struct.String*, %struct.String** %self2, align 8, !dbg !222
  %count32 = getelementptr %struct.String, %struct.String* %self31, i32 0, i32 1, !dbg !222
  %DOT33 = load i32, i32* %count32, align 4, !dbg !222
  %ADD34 = add i32 %DOT33, 1, !dbg !222
  store i32 %ADD34, i32* %count32, align 4, !dbg !222
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !223
  %value36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 0, !dbg !223
  %self37 = load %struct.String*, %struct.String** %self2, align 8, !dbg !223
  %count38 = getelementptr %struct.String, %struct.String* %self37, i32 0, i32 1, !dbg !223
  %DOT39 = load i8*, i8** %value36, align 8, !dbg !223
  %DOT40 = load i32, i32* %count38, align 4, !dbg !223
  %ACCESS41 = getelementptr i8, i8* %DOT39, i32 %DOT40, !dbg !223
  store i8 0, i8* %ACCESS41, align 1, !dbg !223
  ret void, !dbg !223

if.then:                                          ; preds = %if.start
  br label %if.start6, !dbg !221

if.start6:                                        ; preds = %if.then
  %self9 = load %struct.String*, %struct.String** %self2, align 8, !dbg !221
  %size10 = getelementptr %struct.String, %struct.String* %self9, i32 0, i32 2, !dbg !221
  %DOT11 = load i32, i32* %size10, align 4, !dbg !221
  %EQ = icmp eq i32 %DOT11, 0, !dbg !221
  br i1 %EQ, label %if.then8, label %if.else, !dbg !221

if.end7:                                          ; preds = %if.else, %if.then8
  %self17 = load %struct.String*, %struct.String** %self2, align 8, !dbg !221
  %value = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 0, !dbg !221
  %self18 = load %struct.String*, %struct.String** %self2, align 8, !dbg !224
  %value19 = getelementptr %struct.String, %struct.String* %self18, i32 0, i32 0, !dbg !224
  %DOT20 = load i8*, i8** %value19, align 8, !dbg !224
  %self21 = load %struct.String*, %struct.String** %self2, align 8, !dbg !224
  %size22 = getelementptr %struct.String, %struct.String* %self21, i32 0, i32 2, !dbg !224
  %DOT23 = load i32, i32* %size22, align 4, !dbg !224
  %realloc = call i8* @realloc(i8* %DOT20, i32 %DOT23), !dbg !224
  store i8* %realloc, i8** %value, align 8, !dbg !224
  br label %if.end, !dbg !224

if.then8:                                         ; preds = %if.start6
  %self12 = load %struct.String*, %struct.String** %self2, align 8, !dbg !221
  %size13 = getelementptr %struct.String, %struct.String* %self12, i32 0, i32 2, !dbg !221
  store i32 10, i32* %size13, align 4, !dbg !221
  br label %if.end7, !dbg !221

if.else:                                          ; preds = %if.start6
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !221
  %size15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 2, !dbg !221
  %DOT16 = load i32, i32* %size15, align 4, !dbg !221
  %MUL = mul i32 %DOT16, 2, !dbg !221
  store i32 %MUL, i32* %size15, align 4, !dbg !221
  br label %if.end7, !dbg !221
}

define i8 @String.pop(%struct.String* %self) !dbg !225 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !226
  %c = alloca i8, align 1, !dbg !226
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !226
  br label %if.start, !dbg !226

if.start:                                         ; preds = %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !226
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !226
  %DOT = load i32, i32* %count, align 4, !dbg !226
  %EQ = icmp eq i32 %DOT, 0, !dbg !226
  br i1 %EQ, label %if.then, label %if.end, !dbg !226

if.end:                                           ; preds = %if.start
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !226
  %count4 = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !226
  %DOT5 = load i32, i32* %count4, align 4, !dbg !226
  %SUB = sub i32 %DOT5, 1, !dbg !226
  store i32 %SUB, i32* %count4, align 4, !dbg !226
  store i8 0, i8* %c, align 1, !dbg !226
  %self6 = load %struct.String*, %struct.String** %self1, align 8, !dbg !227
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !227
  %self7 = load %struct.String*, %struct.String** %self1, align 8, !dbg !227
  %count8 = getelementptr %struct.String, %struct.String* %self7, i32 0, i32 1, !dbg !227
  %DOT9 = load i8*, i8** %value, align 8, !dbg !227
  %DOT10 = load i32, i32* %count8, align 4, !dbg !227
  %ACCESS = getelementptr i8, i8* %DOT9, i32 %DOT10, !dbg !227
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !227
  store i8 %ACC, i8* %c, align 1, !dbg !227
  %self11 = load %struct.String*, %struct.String** %self1, align 8, !dbg !228
  %value12 = getelementptr %struct.String, %struct.String* %self11, i32 0, i32 0, !dbg !228
  %self13 = load %struct.String*, %struct.String** %self1, align 8, !dbg !228
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !228
  %DOT15 = load i8*, i8** %value12, align 8, !dbg !228
  %DOT16 = load i32, i32* %count14, align 4, !dbg !228
  %ACCESS17 = getelementptr i8, i8* %DOT15, i32 %DOT16, !dbg !228
  store i8 0, i8* %ACCESS17, align 1, !dbg !228
  %c18 = load i8, i8* %c, align 1, !dbg !228
  ret i8 %c18, !dbg !228

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !226
}

define %struct.String @String.new() !dbg !229 {
entry:
  %s = alloca %struct.String, align 8, !dbg !230
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !230
  %size = getelementptr %struct.String, %struct.String* %s, i32 0, i32 2, !dbg !230
  store i32 10, i32* %size, align 4, !dbg !230
  %value = getelementptr %struct.String, %struct.String* %s, i32 0, i32 0, !dbg !230
  %calloc = call i8* @calloc(i64 10, i64 1), !dbg !231
  store i8* %calloc, i8** %value, align 8, !dbg !231
  %s1 = load %struct.String, %struct.String* %s, align 8, !dbg !231
  ret %struct.String %s1, !dbg !231
}

define %struct.String @String.from(i8* %str) !dbg !232 {
entry:
  %str1 = alloca i8*, align 8, !dbg !233
  %s = alloca %struct.String, align 8, !dbg !233
  store i8* %str, i8** %str1, align 8, !dbg !233
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !233
  %0 = call %struct.String @String.new(), !dbg !234
  %tmp_struct = alloca %struct.String, align 8, !dbg !234
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !234
  %tmp_op = alloca %struct.String, align 8, !dbg !234
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !234
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !234
  %str2 = load i8*, i8** %str1, align 8, !dbg !235
  call void @String.assign(i8* %str2, %struct.String* %s), !dbg !235
  call void @String.delete(%struct.String* %tmp_struct), !dbg !235
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !235
  ret %struct.String %s3, !dbg !235
}

define %struct.String @String.from_int(i32 %n) !dbg !236 {
entry:
  %n1 = alloca i32, align 4, !dbg !237
  %s = alloca %struct.String, align 8, !dbg !237
  %neg = alloca i1, align 1, !dbg !237
  %num = alloca i32, align 4, !dbg !237
  %buf = alloca i8*, align 8, !dbg !237
  %i = alloca i32, align 4, !dbg !237
  %j = alloca i32, align 4, !dbg !237
  store i32 %n, i32* %n1, align 4, !dbg !237
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !237
  %0 = call %struct.String @String.new(), !dbg !238
  %tmp_struct = alloca %struct.String, align 8, !dbg !238
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !238
  %tmp_op = alloca %struct.String, align 8, !dbg !238
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !238
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !238
  br label %if.start, !dbg !238

if.start:                                         ; preds = %entry
  %n2 = load i32, i32* %n1, align 4, !dbg !238
  %EQ = icmp eq i32 %n2, 0, !dbg !238
  br i1 %EQ, label %if.then, label %if.end, !dbg !238

if.end:                                           ; preds = %if.start
  store i1 false, i1* %neg, align 1, !dbg !237
  store i1 false, i1* %neg, align 1, !dbg !237
  store i32 0, i32* %num, align 4, !dbg !237
  %n4 = load i32, i32* %n1, align 4, !dbg !237
  store i32 %n4, i32* %num, align 4, !dbg !237
  br label %if.start5, !dbg !237

if.then:                                          ; preds = %if.start
  call void @String.push(i8 48, %struct.String* %s), !dbg !239
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !239
  ret %struct.String %s3, !dbg !239

if.start5:                                        ; preds = %if.end
  %num8 = load i32, i32* %num, align 4, !dbg !237
  %LT = icmp slt i32 %num8, 0, !dbg !237
  br i1 %LT, label %if.then7, label %if.end6, !dbg !237

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %buf, align 8, !dbg !237
  %stack = alloca [20 x i8], align 1, !dbg !237
  %stack10 = getelementptr [20 x i8], [20 x i8]* %stack, i32 0, i32 0, !dbg !237
  store i8* %stack10, i8** %buf, align 8, !dbg !237
  store i32 0, i32* %i, align 4, !dbg !237
  store i32 0, i32* %i, align 4, !dbg !237
  br label %while.start, !dbg !237

if.then7:                                         ; preds = %if.start5
  store i1 true, i1* %neg, align 1, !dbg !237
  %num9 = load i32, i32* %num, align 4, !dbg !237
  %SUB = sub i32 0, %num9, !dbg !237
  store i32 %SUB, i32* %num, align 4, !dbg !237
  br label %if.end6, !dbg !237

while.start:                                      ; preds = %while.then, %if.end6
  %num11 = load i32, i32* %num, align 4, !dbg !237
  %GT = icmp sgt i32 %num11, 0, !dbg !237
  br i1 %GT, label %while.then, label %while.end, !dbg !237

while.then:                                       ; preds = %while.start
  %buf12 = load i8*, i8** %buf, align 8, !dbg !240
  %i13 = load i32, i32* %i, align 4, !dbg !240
  %ACCESS = getelementptr i8, i8* %buf12, i32 %i13, !dbg !240
  %num14 = load i32, i32* %num, align 4, !dbg !240
  %MOD = srem i32 %num14, 10, !dbg !240
  %ADD = add i32 %MOD, 48, !dbg !240
  %as = trunc i32 %ADD to i8, !dbg !240
  store i8 %as, i8* %ACCESS, align 1, !dbg !240
  %num15 = load i32, i32* %num, align 4, !dbg !240
  %DIV = sdiv i32 %num15, 10, !dbg !240
  store i32 %DIV, i32* %num, align 4, !dbg !240
  %i16 = load i32, i32* %i, align 4, !dbg !240
  %ADD17 = add i32 %i16, 1, !dbg !240
  store i32 %ADD17, i32* %i, align 4, !dbg !240
  br label %while.start, !dbg !240

while.end:                                        ; preds = %while.start
  br label %if.start18, !dbg !240

if.start18:                                       ; preds = %while.end
  %neg21 = load i1, i1* %neg, align 1, !dbg !240
  br i1 %neg21, label %if.then20, label %if.end19, !dbg !240

if.end19:                                         ; preds = %if.then20, %if.start18
  store i32 0, i32* %j, align 4, !dbg !237
  %i22 = load i32, i32* %i, align 4, !dbg !237
  %SUB23 = sub i32 %i22, 1, !dbg !237
  store i32 %SUB23, i32* %j, align 4, !dbg !237
  br label %while.start24, !dbg !237

if.then20:                                        ; preds = %if.start18
  call void @String.push(i8 45, %struct.String* %s), !dbg !241
  br label %if.end19, !dbg !241

while.start24:                                    ; preds = %while.then25, %if.end19
  %j27 = load i32, i32* %j, align 4, !dbg !237
  %GE = icmp sge i32 %j27, 0, !dbg !237
  br i1 %GE, label %while.then25, label %while.end26, !dbg !237

while.then25:                                     ; preds = %while.start24
  %buf28 = load i8*, i8** %buf, align 8, !dbg !242
  %j29 = load i32, i32* %j, align 4, !dbg !242
  %ACCESS30 = getelementptr i8, i8* %buf28, i32 %j29, !dbg !242
  %ACC = load i8, i8* %ACCESS30, align 1, !dbg !242
  call void @String.push(i8 %ACC, %struct.String* %s), !dbg !242
  %j31 = load i32, i32* %j, align 4, !dbg !242
  %SUB32 = sub i32 %j31, 1, !dbg !242
  store i32 %SUB32, i32* %j, align 4, !dbg !242
  br label %while.start24, !dbg !242

while.end26:                                      ; preds = %while.start24
  call void @String.delete(%struct.String* %tmp_struct), !dbg !242
  %s33 = load %struct.String, %struct.String* %s, align 8, !dbg !242
  ret %struct.String %s33, !dbg !242
}

define i32 @String.len(%struct.String* %self) !dbg !243 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !244
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !244
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !244
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !244
  %DOT = load i32, i32* %count, align 4, !dbg !244
  ret i32 %DOT, !dbg !244
}

define i1 @String.empty(%struct.String* %self) !dbg !245 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !246
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !246
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !246
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !246
  %DOT = load i32, i32* %count, align 4, !dbg !246
  %EQ = icmp eq i32 %DOT, 0, !dbg !246
  ret i1 %EQ, !dbg !246
}

define i8* @String.c_str(%struct.String* %self) !dbg !247 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !248
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !248
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !248
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !248
  %DOT = load i8*, i8** %value, align 8, !dbg !248
  ret i8* %DOT, !dbg !248
}

define i8 @String.at(i32 %i, %struct.String* %self) !dbg !249 {
entry:
  %i1 = alloca i32, align 4, !dbg !250
  %self2 = alloca %struct.String*, align 8, !dbg !250
  store i32 %i, i32* %i1, align 4, !dbg !250
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !250
  br label %if.start, !dbg !250

if.start:                                         ; preds = %entry
  %i3 = load i32, i32* %i1, align 4, !dbg !250
  %LT = icmp slt i32 %i3, 0, !dbg !250
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !250
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !250
  %i5 = load i32, i32* %i1, align 4, !dbg !250
  %DOT = load i32, i32* %count, align 4, !dbg !250
  %GE = icmp sge i32 %i5, %DOT, !dbg !250
  %OR = or i1 %LT, %GE, !dbg !250
  br i1 %OR, label %if.then, label %if.end, !dbg !250

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !251
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !251
  %DOT7 = load i8*, i8** %value, align 8, !dbg !251
  %i8 = load i32, i32* %i1, align 4, !dbg !251
  %ACCESS = getelementptr i8, i8* %DOT7, i32 %i8, !dbg !251
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !251
  ret i8 %ACC, !dbg !251

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !250
}

define i32 @String.find(i8* %needle, %struct.String* %self) !dbg !252 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !253
  %self2 = alloca %struct.String*, align 8, !dbg !253
  %i = alloca i32, align 4, !dbg !253
  %nlen = alloca i32, align 4, !dbg !253
  store i8* %needle, i8** %needle1, align 8, !dbg !253
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !253
  br label %if.start, !dbg !253

if.start:                                         ; preds = %entry
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !253
  %EQ = icmp eq i8* %needle3, null, !dbg !253
  br i1 %EQ, label %if.then, label %if.end, !dbg !253

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !253
  store i32 0, i32* %i, align 4, !dbg !253
  store i32 0, i32* %nlen, align 4, !dbg !253
  %needle4 = load i8*, i8** %needle1, align 8, !dbg !254
  %strlen = call i32 @strlen(i8* %needle4), !dbg !254
  store i32 %strlen, i32* %nlen, align 4, !dbg !254
  br label %if.start5, !dbg !254

if.then:                                          ; preds = %if.start
  ret i32 -1, !dbg !253

if.start5:                                        ; preds = %if.end
  %nlen8 = load i32, i32* %nlen, align 4, !dbg !254
  %EQ9 = icmp eq i32 %nlen8, 0, !dbg !254
  br i1 %EQ9, label %if.then7, label %if.end6, !dbg !254

if.end6:                                          ; preds = %if.start5
  br label %while.start, !dbg !254

if.then7:                                         ; preds = %if.start5
  ret i32 0, !dbg !254

while.start:                                      ; preds = %if.end14, %if.end6
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !254
  %count = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !254
  %DOT = load i32, i32* %count, align 4, !dbg !254
  %nlen11 = load i32, i32* %nlen, align 4, !dbg !254
  %SUB = sub i32 %DOT, %nlen11, !dbg !254
  %i12 = load i32, i32* %i, align 4, !dbg !254
  %LE = icmp sle i32 %i12, %SUB, !dbg !254
  br i1 %LE, label %while.then, label %while.end, !dbg !254

while.then:                                       ; preds = %while.start
  br label %if.start13, !dbg !254

while.end:                                        ; preds = %while.start
  ret i32 -1, !dbg !255

if.start13:                                       ; preds = %while.then
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !255
  %value = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 0, !dbg !255
  %DOT17 = load i8*, i8** %value, align 8, !dbg !255
  %i18 = load i32, i32* %i, align 4, !dbg !255
  %idx = sext i32 %i18 to i64, !dbg !255
  %ADD = getelementptr i8, i8* %DOT17, i64 %idx, !dbg !255
  %needle19 = load i8*, i8** %needle1, align 8, !dbg !255
  %nlen20 = load i32, i32* %nlen, align 4, !dbg !255
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %needle19, i32 %nlen20), !dbg !255
  %EQ21 = icmp eq i32 %strncmp, 0, !dbg !255
  br i1 %EQ21, label %if.then15, label %if.end14, !dbg !255

if.end14:                                         ; preds = %if.start13
  %i23 = load i32, i32* %i, align 4, !dbg !255
  %ADD24 = add i32 %i23, 1, !dbg !255
  store i32 %ADD24, i32* %i, align 4, !dbg !255
  br label %while.start, !dbg !255

if.then15:                                        ; preds = %if.start13
  %i22 = load i32, i32* %i, align 4, !dbg !255
  ret i32 %i22, !dbg !255
}

define i1 @String.contains(i8* %needle, %struct.String* %self) !dbg !256 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !257
  %self2 = alloca %struct.String*, align 8, !dbg !257
  store i8* %needle, i8** %needle1, align 8, !dbg !257
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !257
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !258
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !258
  %String.find = call i32 @String.find(i8* %needle3, %struct.String* %ref_arg), !dbg !258
  %NEQ = icmp ne i32 %String.find, -1, !dbg !258
  ret i1 %NEQ, !dbg !258
}

define i1 @String.starts_with(i8* %prefix, %struct.String* %self) !dbg !259 {
entry:
  %prefix1 = alloca i8*, align 8, !dbg !260
  %self2 = alloca %struct.String*, align 8, !dbg !260
  %plen = alloca i32, align 4, !dbg !260
  store i8* %prefix, i8** %prefix1, align 8, !dbg !260
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !260
  br label %if.start, !dbg !260

if.start:                                         ; preds = %entry
  %prefix3 = load i8*, i8** %prefix1, align 8, !dbg !260
  %EQ = icmp eq i8* %prefix3, null, !dbg !260
  br i1 %EQ, label %if.then, label %if.end, !dbg !260

if.end:                                           ; preds = %if.start
  store i32 0, i32* %plen, align 4, !dbg !260
  %prefix4 = load i8*, i8** %prefix1, align 8, !dbg !261
  %strlen = call i32 @strlen(i8* %prefix4), !dbg !261
  store i32 %strlen, i32* %plen, align 4, !dbg !261
  br label %if.start5, !dbg !261

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !260

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !261
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !261
  %plen9 = load i32, i32* %plen, align 4, !dbg !261
  %DOT = load i32, i32* %count, align 4, !dbg !261
  %GT = icmp sgt i32 %plen9, %DOT, !dbg !261
  br i1 %GT, label %if.then7, label %if.end6, !dbg !261

if.end6:                                          ; preds = %if.start5
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !262
  %value = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 0, !dbg !262
  %DOT11 = load i8*, i8** %value, align 8, !dbg !262
  %prefix12 = load i8*, i8** %prefix1, align 8, !dbg !262
  %plen13 = load i32, i32* %plen, align 4, !dbg !262
  %strncmp = call i32 @strncmp(i8* %DOT11, i8* %prefix12, i32 %plen13), !dbg !262
  %EQ14 = icmp eq i32 %strncmp, 0, !dbg !262
  ret i1 %EQ14, !dbg !262

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !261
}

define i1 @String.ends_with(i8* %suffix, %struct.String* %self) !dbg !263 {
entry:
  %suffix1 = alloca i8*, align 8, !dbg !264
  %self2 = alloca %struct.String*, align 8, !dbg !264
  %slen = alloca i32, align 4, !dbg !264
  %offset = alloca i32, align 4, !dbg !264
  store i8* %suffix, i8** %suffix1, align 8, !dbg !264
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !264
  br label %if.start, !dbg !264

if.start:                                         ; preds = %entry
  %suffix3 = load i8*, i8** %suffix1, align 8, !dbg !264
  %EQ = icmp eq i8* %suffix3, null, !dbg !264
  br i1 %EQ, label %if.then, label %if.end, !dbg !264

if.end:                                           ; preds = %if.start
  store i32 0, i32* %slen, align 4, !dbg !264
  %suffix4 = load i8*, i8** %suffix1, align 8, !dbg !265
  %strlen = call i32 @strlen(i8* %suffix4), !dbg !265
  store i32 %strlen, i32* %slen, align 4, !dbg !265
  br label %if.start5, !dbg !265

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !264

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !265
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !265
  %slen9 = load i32, i32* %slen, align 4, !dbg !265
  %DOT = load i32, i32* %count, align 4, !dbg !265
  %GT = icmp sgt i32 %slen9, %DOT, !dbg !265
  br i1 %GT, label %if.then7, label %if.end6, !dbg !265

if.end6:                                          ; preds = %if.start5
  store i32 0, i32* %offset, align 4, !dbg !264
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !264
  %count11 = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !264
  %DOT12 = load i32, i32* %count11, align 4, !dbg !264
  %slen13 = load i32, i32* %slen, align 4, !dbg !264
  %SUB = sub i32 %DOT12, %slen13, !dbg !264
  store i32 %SUB, i32* %offset, align 4, !dbg !264
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !266
  %value = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 0, !dbg !266
  %DOT15 = load i8*, i8** %value, align 8, !dbg !266
  %offset16 = load i32, i32* %offset, align 4, !dbg !266
  %idx = sext i32 %offset16 to i64, !dbg !266
  %ADD = getelementptr i8, i8* %DOT15, i64 %idx, !dbg !266
  %suffix17 = load i8*, i8** %suffix1, align 8, !dbg !266
  %slen18 = load i32, i32* %slen, align 4, !dbg !266
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %suffix17, i32 %slen18), !dbg !266
  %EQ19 = icmp eq i32 %strncmp, 0, !dbg !266
  ret i1 %EQ19, !dbg !266

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !265
}

define %struct.String @String.substr(i32 %start, i32 %length, %struct.String* %self) !dbg !267 {
entry:
  %start1 = alloca i32, align 4, !dbg !268
  %length2 = alloca i32, align 4, !dbg !268
  %self3 = alloca %struct.String*, align 8, !dbg !268
  %res = alloca %struct.String, align 8, !dbg !268
  %end = alloca i32, align 4, !dbg !268
  %i = alloca i32, align 4, !dbg !268
  store i32 %start, i32* %start1, align 4, !dbg !268
  store i32 %length, i32* %length2, align 4, !dbg !268
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !268
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !268
  %0 = call %struct.String @String.new(), !dbg !269
  %tmp_struct = alloca %struct.String, align 8, !dbg !269
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !269
  %tmp_op = alloca %struct.String, align 8, !dbg !269
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !269
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !269
  br label %if.start, !dbg !269

if.start:                                         ; preds = %entry
  %start4 = load i32, i32* %start1, align 4, !dbg !269
  %LT = icmp slt i32 %start4, 0, !dbg !269
  %self5 = load %struct.String*, %struct.String** %self3, align 8, !dbg !269
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !269
  %start6 = load i32, i32* %start1, align 4, !dbg !269
  %DOT = load i32, i32* %count, align 4, !dbg !269
  %GE = icmp sge i32 %start6, %DOT, !dbg !269
  %OR = or i1 %LT, %GE, !dbg !269
  br i1 %OR, label %if.then, label %if.end, !dbg !269

if.end:                                           ; preds = %if.start
  store i32 0, i32* %end, align 4, !dbg !268
  %start8 = load i32, i32* %start1, align 4, !dbg !268
  %length9 = load i32, i32* %length2, align 4, !dbg !268
  %ADD = add i32 %start8, %length9, !dbg !268
  store i32 %ADD, i32* %end, align 4, !dbg !268
  br label %if.start10, !dbg !268

if.then:                                          ; preds = %if.start
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !269
  ret %struct.String %res7, !dbg !269

if.start10:                                       ; preds = %if.end
  %self13 = load %struct.String*, %struct.String** %self3, align 8, !dbg !268
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !268
  %end15 = load i32, i32* %end, align 4, !dbg !268
  %DOT16 = load i32, i32* %count14, align 4, !dbg !268
  %GT = icmp sgt i32 %end15, %DOT16, !dbg !268
  br i1 %GT, label %if.then12, label %if.end11, !dbg !268

if.end11:                                         ; preds = %if.then12, %if.start10
  store i32 0, i32* %i, align 4, !dbg !268
  %start20 = load i32, i32* %start1, align 4, !dbg !268
  store i32 %start20, i32* %i, align 4, !dbg !268
  br label %while.start, !dbg !268

if.then12:                                        ; preds = %if.start10
  %self17 = load %struct.String*, %struct.String** %self3, align 8, !dbg !268
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !268
  %DOT19 = load i32, i32* %count18, align 4, !dbg !268
  store i32 %DOT19, i32* %end, align 4, !dbg !268
  br label %if.end11, !dbg !268

while.start:                                      ; preds = %while.then, %if.end11
  %i21 = load i32, i32* %i, align 4, !dbg !268
  %end22 = load i32, i32* %end, align 4, !dbg !268
  %LT23 = icmp slt i32 %i21, %end22, !dbg !268
  br i1 %LT23, label %while.then, label %while.end, !dbg !268

while.then:                                       ; preds = %while.start
  %self24 = load %struct.String*, %struct.String** %self3, align 8, !dbg !270
  %value = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !270
  %DOT25 = load i8*, i8** %value, align 8, !dbg !270
  %i26 = load i32, i32* %i, align 4, !dbg !270
  %ACCESS = getelementptr i8, i8* %DOT25, i32 %i26, !dbg !270
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !270
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !270
  %i27 = load i32, i32* %i, align 4, !dbg !270
  %ADD28 = add i32 %i27, 1, !dbg !270
  store i32 %ADD28, i32* %i, align 4, !dbg !270
  br label %while.start, !dbg !270

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !270
  %res29 = load %struct.String, %struct.String* %res, align 8, !dbg !270
  ret %struct.String %res29, !dbg !270
}

define %struct.String @String.upper(%struct.String* %self) !dbg !271 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !272
  %res = alloca %struct.String, align 8, !dbg !272
  %i = alloca i32, align 4, !dbg !272
  %c = alloca i8, align 1, !dbg !272
  %v = alloca i32, align 4, !dbg !272
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !272
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !272
  %0 = call %struct.String @String.new(), !dbg !273
  %tmp_struct = alloca %struct.String, align 8, !dbg !273
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !273
  %tmp_op = alloca %struct.String, align 8, !dbg !273
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !273
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !273
  store i32 0, i32* %i, align 4, !dbg !272
  store i32 0, i32* %i, align 4, !dbg !272
  br label %while.start, !dbg !272

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !272
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !272
  %i3 = load i32, i32* %i, align 4, !dbg !272
  %DOT = load i32, i32* %count, align 4, !dbg !272
  %LT = icmp slt i32 %i3, %DOT, !dbg !272
  br i1 %LT, label %while.then, label %while.end, !dbg !272

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !272
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !274
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !274
  %DOT5 = load i8*, i8** %value, align 8, !dbg !274
  %i6 = load i32, i32* %i, align 4, !dbg !274
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !274
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !274
  store i8 %ACC, i8* %c, align 1, !dbg !274
  br label %if.start, !dbg !274

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !275
  %res14 = load %struct.String, %struct.String* %res, align 8, !dbg !275
  ret %struct.String %res14, !dbg !275

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !274
  %GE = icmp sge i8 %c7, 97, !dbg !274
  %c8 = load i8, i8* %c, align 1, !dbg !274
  %LE = icmp sle i8 %c8, 122, !dbg !274
  %AND = and i1 %GE, %LE, !dbg !274
  br i1 %AND, label %if.then, label %if.else, !dbg !274

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !275
  %ADD = add i32 %i13, 1, !dbg !275
  store i32 %ADD, i32* %i, align 4, !dbg !275
  br label %while.start, !dbg !275

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !272
  %c9 = load i8, i8* %c, align 1, !dbg !272
  %as = sext i8 %c9 to i32, !dbg !272
  %SUB = sub i32 %as, 32, !dbg !272
  store i32 %SUB, i32* %v, align 4, !dbg !272
  %v10 = load i32, i32* %v, align 4, !dbg !276
  %as11 = trunc i32 %v10 to i8, !dbg !276
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !276
  br label %if.end, !dbg !276

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !275
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !275
  br label %if.end, !dbg !275
}

define %struct.String @String.lower(%struct.String* %self) !dbg !277 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !278
  %res = alloca %struct.String, align 8, !dbg !278
  %i = alloca i32, align 4, !dbg !278
  %c = alloca i8, align 1, !dbg !278
  %v = alloca i32, align 4, !dbg !278
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !278
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !278
  %0 = call %struct.String @String.new(), !dbg !279
  %tmp_struct = alloca %struct.String, align 8, !dbg !279
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !279
  %tmp_op = alloca %struct.String, align 8, !dbg !279
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !279
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !279
  store i32 0, i32* %i, align 4, !dbg !278
  store i32 0, i32* %i, align 4, !dbg !278
  br label %while.start, !dbg !278

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !278
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !278
  %i3 = load i32, i32* %i, align 4, !dbg !278
  %DOT = load i32, i32* %count, align 4, !dbg !278
  %LT = icmp slt i32 %i3, %DOT, !dbg !278
  br i1 %LT, label %while.then, label %while.end, !dbg !278

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !278
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !280
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !280
  %DOT5 = load i8*, i8** %value, align 8, !dbg !280
  %i6 = load i32, i32* %i, align 4, !dbg !280
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !280
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !280
  store i8 %ACC, i8* %c, align 1, !dbg !280
  br label %if.start, !dbg !280

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !281
  %res15 = load %struct.String, %struct.String* %res, align 8, !dbg !281
  ret %struct.String %res15, !dbg !281

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !280
  %GE = icmp sge i8 %c7, 65, !dbg !280
  %c8 = load i8, i8* %c, align 1, !dbg !280
  %LE = icmp sle i8 %c8, 90, !dbg !280
  %AND = and i1 %GE, %LE, !dbg !280
  br i1 %AND, label %if.then, label %if.else, !dbg !280

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !281
  %ADD14 = add i32 %i13, 1, !dbg !281
  store i32 %ADD14, i32* %i, align 4, !dbg !281
  br label %while.start, !dbg !281

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !278
  %c9 = load i8, i8* %c, align 1, !dbg !278
  %as = sext i8 %c9 to i32, !dbg !278
  %ADD = add i32 %as, 32, !dbg !278
  store i32 %ADD, i32* %v, align 4, !dbg !278
  %v10 = load i32, i32* %v, align 4, !dbg !282
  %as11 = trunc i32 %v10 to i8, !dbg !282
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !282
  br label %if.end, !dbg !282

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !281
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !281
  br label %if.end, !dbg !281
}

define %struct.String @String.trim(%struct.String* %self) !dbg !283 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !284
  %start = alloca i32, align 4, !dbg !284
  %c = alloca i8, align 1, !dbg !284
  %end = alloca i32, align 4, !dbg !284
  %c25 = alloca i8, align 1, !dbg !284
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !284
  store i32 0, i32* %start, align 4, !dbg !284
  store i32 0, i32* %start, align 4, !dbg !284
  br label %while.start, !dbg !284

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !284
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !284
  %start3 = load i32, i32* %start, align 4, !dbg !284
  %DOT = load i32, i32* %count, align 4, !dbg !284
  %LT = icmp slt i32 %start3, %DOT, !dbg !284
  br i1 %LT, label %while.then, label %while.end, !dbg !284

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !284
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !285
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !285
  %DOT5 = load i8*, i8** %value, align 8, !dbg !285
  %start6 = load i32, i32* %start, align 4, !dbg !285
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %start6, !dbg !285
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !285
  store i8 %ACC, i8* %c, align 1, !dbg !285
  br label %if.start, !dbg !285

while.end:                                        ; preds = %if.else, %while.start
  store i32 0, i32* %end, align 4, !dbg !284
  %self17 = load %struct.String*, %struct.String** %self1, align 8, !dbg !284
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !284
  %DOT19 = load i32, i32* %count18, align 4, !dbg !284
  store i32 %DOT19, i32* %end, align 4, !dbg !284
  br label %while.start20, !dbg !284

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !285
  %EQ = icmp eq i8 %c7, 32, !dbg !285
  %c8 = load i8, i8* %c, align 1, !dbg !285
  %EQ9 = icmp eq i8 %c8, 9, !dbg !285
  %OR = or i1 %EQ, %EQ9, !dbg !285
  %c10 = load i8, i8* %c, align 1, !dbg !285
  %EQ11 = icmp eq i8 %c10, 10, !dbg !285
  %OR12 = or i1 %OR, %EQ11, !dbg !285
  %c13 = load i8, i8* %c, align 1, !dbg !285
  %EQ14 = icmp eq i8 %c13, 13, !dbg !285
  %OR15 = or i1 %OR12, %EQ14, !dbg !285
  br i1 %OR15, label %if.then, label %if.else, !dbg !285

if.end:                                           ; preds = %if.then
  br label %while.start, !dbg !285

if.then:                                          ; preds = %if.start
  %start16 = load i32, i32* %start, align 4, !dbg !285
  %ADD = add i32 %start16, 1, !dbg !285
  store i32 %ADD, i32* %start, align 4, !dbg !285
  br label %if.end, !dbg !285

if.else:                                          ; preds = %if.start
  br label %while.end, !dbg !285

while.start20:                                    ; preds = %if.end33, %while.end
  %end23 = load i32, i32* %end, align 4, !dbg !284
  %start24 = load i32, i32* %start, align 4, !dbg !284
  %GT = icmp sgt i32 %end23, %start24, !dbg !284
  br i1 %GT, label %while.then21, label %while.end22, !dbg !284

while.then21:                                     ; preds = %while.start20
  store i8 0, i8* %c25, align 1, !dbg !284
  %self26 = load %struct.String*, %struct.String** %self1, align 8, !dbg !286
  %value27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 0, !dbg !286
  %end28 = load i32, i32* %end, align 4, !dbg !286
  %SUB = sub i32 %end28, 1, !dbg !286
  %DOT29 = load i8*, i8** %value27, align 8, !dbg !286
  %ACCESS30 = getelementptr i8, i8* %DOT29, i32 %SUB, !dbg !286
  %ACC31 = load i8, i8* %ACCESS30, align 1, !dbg !286
  store i8 %ACC31, i8* %c25, align 1, !dbg !286
  br label %if.start32, !dbg !286

while.end22:                                      ; preds = %if.else35, %while.start20
  %start49 = load i32, i32* %start, align 4, !dbg !287
  %end50 = load i32, i32* %end, align 4, !dbg !287
  %start51 = load i32, i32* %start, align 4, !dbg !287
  %SUB52 = sub i32 %end50, %start51, !dbg !287
  %ref_arg = load %struct.String*, %struct.String** %self1, align 8, !dbg !287
  %String.substr = call %struct.String @String.substr(i32 %start49, i32 %SUB52, %struct.String* %ref_arg), !dbg !287
  %tmp_struct = alloca %struct.String, align 8, !dbg !287
  store %struct.String %String.substr, %struct.String* %tmp_struct, align 8, !dbg !287
  ret %struct.String %String.substr, !dbg !287

if.start32:                                       ; preds = %while.then21
  %c36 = load i8, i8* %c25, align 1, !dbg !286
  %EQ37 = icmp eq i8 %c36, 32, !dbg !286
  %c38 = load i8, i8* %c25, align 1, !dbg !286
  %EQ39 = icmp eq i8 %c38, 9, !dbg !286
  %OR40 = or i1 %EQ37, %EQ39, !dbg !286
  %c41 = load i8, i8* %c25, align 1, !dbg !286
  %EQ42 = icmp eq i8 %c41, 10, !dbg !286
  %OR43 = or i1 %OR40, %EQ42, !dbg !286
  %c44 = load i8, i8* %c25, align 1, !dbg !286
  %EQ45 = icmp eq i8 %c44, 13, !dbg !286
  %OR46 = or i1 %OR43, %EQ45, !dbg !286
  br i1 %OR46, label %if.then34, label %if.else35, !dbg !286

if.end33:                                         ; preds = %if.then34
  br label %while.start20, !dbg !286

if.then34:                                        ; preds = %if.start32
  %end47 = load i32, i32* %end, align 4, !dbg !286
  %SUB48 = sub i32 %end47, 1, !dbg !286
  store i32 %SUB48, i32* %end, align 4, !dbg !286
  br label %if.end33, !dbg !286

if.else35:                                        ; preds = %if.start32
  br label %while.end22, !dbg !286
}

define %struct.String @String.replace(i8* %old, i8* %replacement, %struct.String* %self) !dbg !288 {
entry:
  %old1 = alloca i8*, align 8, !dbg !289
  %replacement2 = alloca i8*, align 8, !dbg !289
  %self3 = alloca %struct.String*, align 8, !dbg !289
  %res = alloca %struct.String, align 8, !dbg !289
  %olen = alloca i32, align 4, !dbg !289
  %i = alloca i32, align 4, !dbg !289
  store i8* %old, i8** %old1, align 8, !dbg !289
  store i8* %replacement, i8** %replacement2, align 8, !dbg !289
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !289
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !289
  %0 = call %struct.String @String.new(), !dbg !290
  %tmp_struct = alloca %struct.String, align 8, !dbg !290
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !290
  %tmp_op = alloca %struct.String, align 8, !dbg !290
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !290
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !290
  store i32 0, i32* %olen, align 4, !dbg !289
  %old4 = load i8*, i8** %old1, align 8, !dbg !291
  %strlen = call i32 @strlen(i8* %old4), !dbg !291
  store i32 %strlen, i32* %olen, align 4, !dbg !291
  br label %if.start, !dbg !291

if.start:                                         ; preds = %entry
  %olen5 = load i32, i32* %olen, align 4, !dbg !291
  %EQ = icmp eq i32 %olen5, 0, !dbg !291
  br i1 %EQ, label %if.then, label %if.end, !dbg !291

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !289
  store i32 0, i32* %i, align 4, !dbg !289
  br label %while.start, !dbg !289

if.then:                                          ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self3, align 8, !dbg !292
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !292
  %DOT = load i8*, i8** %value, align 8, !dbg !292
  call void @String.assign(i8* %DOT, %struct.String* %res), !dbg !292
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !292
  ret %struct.String %res7, !dbg !292

while.start:                                      ; preds = %if.end12, %if.end
  %self8 = load %struct.String*, %struct.String** %self3, align 8, !dbg !289
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !289
  %i9 = load i32, i32* %i, align 4, !dbg !289
  %DOT10 = load i32, i32* %count, align 4, !dbg !289
  %LT = icmp slt i32 %i9, %DOT10, !dbg !289
  br i1 %LT, label %while.then, label %while.end, !dbg !289

while.then:                                       ; preds = %while.start
  br label %if.start11, !dbg !289

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !293
  %res36 = load %struct.String, %struct.String* %res, align 8, !dbg !293
  ret %struct.String %res36, !dbg !293

if.start11:                                       ; preds = %while.then
  %self14 = load %struct.String*, %struct.String** %self3, align 8, !dbg !289
  %count15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 1, !dbg !289
  %DOT16 = load i32, i32* %count15, align 4, !dbg !289
  %olen17 = load i32, i32* %olen, align 4, !dbg !289
  %SUB = sub i32 %DOT16, %olen17, !dbg !289
  %i18 = load i32, i32* %i, align 4, !dbg !289
  %LE = icmp sle i32 %i18, %SUB, !dbg !289
  %self19 = load %struct.String*, %struct.String** %self3, align 8, !dbg !294
  %value20 = getelementptr %struct.String, %struct.String* %self19, i32 0, i32 0, !dbg !294
  %DOT21 = load i8*, i8** %value20, align 8, !dbg !294
  %i22 = load i32, i32* %i, align 4, !dbg !294
  %idx = sext i32 %i22 to i64, !dbg !294
  %ADD = getelementptr i8, i8* %DOT21, i64 %idx, !dbg !294
  %old23 = load i8*, i8** %old1, align 8, !dbg !294
  %olen24 = load i32, i32* %olen, align 4, !dbg !294
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %old23, i32 %olen24), !dbg !294
  %EQ25 = icmp eq i32 %strncmp, 0, !dbg !294
  %AND = and i1 %LE, %EQ25, !dbg !294
  br i1 %AND, label %if.then13, label %if.else, !dbg !294

if.end12:                                         ; preds = %if.else, %if.then13
  br label %while.start, !dbg !293

if.then13:                                        ; preds = %if.start11
  %replacement26 = load i8*, i8** %replacement2, align 8, !dbg !295
  call void @String.join(i8* %replacement26, %struct.String* %res), !dbg !295
  %i27 = load i32, i32* %i, align 4, !dbg !295
  %olen28 = load i32, i32* %olen, align 4, !dbg !295
  %ADD29 = add i32 %i27, %olen28, !dbg !295
  store i32 %ADD29, i32* %i, align 4, !dbg !295
  br label %if.end12, !dbg !295

if.else:                                          ; preds = %if.start11
  %self30 = load %struct.String*, %struct.String** %self3, align 8, !dbg !293
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !293
  %DOT32 = load i8*, i8** %value31, align 8, !dbg !293
  %i33 = load i32, i32* %i, align 4, !dbg !293
  %ACCESS = getelementptr i8, i8* %DOT32, i32 %i33, !dbg !293
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !293
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !293
  %i34 = load i32, i32* %i, align 4, !dbg !293
  %ADD35 = add i32 %i34, 1, !dbg !293
  store i32 %ADD35, i32* %i, align 4, !dbg !293
  br label %if.end12, !dbg !293
}

define %struct.String @String.repeat(i32 %n, %struct.String* %self) !dbg !296 {
entry:
  %n1 = alloca i32, align 4, !dbg !297
  %self2 = alloca %struct.String*, align 8, !dbg !297
  %res = alloca %struct.String, align 8, !dbg !297
  %i = alloca i32, align 4, !dbg !297
  store i32 %n, i32* %n1, align 4, !dbg !297
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !297
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !297
  %0 = call %struct.String @String.new(), !dbg !298
  %tmp_struct = alloca %struct.String, align 8, !dbg !298
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !298
  %tmp_op = alloca %struct.String, align 8, !dbg !298
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !298
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !298
  store i32 0, i32* %i, align 4, !dbg !297
  store i32 0, i32* %i, align 4, !dbg !297
  br label %while.start, !dbg !297

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !297
  %n4 = load i32, i32* %n1, align 4, !dbg !297
  %LT = icmp slt i32 %i3, %n4, !dbg !297
  br i1 %LT, label %while.then, label %while.end, !dbg !297

while.then:                                       ; preds = %while.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !299
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !299
  %DOT = load i8*, i8** %value, align 8, !dbg !299
  call void @String.join(i8* %DOT, %struct.String* %res), !dbg !299
  %i6 = load i32, i32* %i, align 4, !dbg !299
  %ADD = add i32 %i6, 1, !dbg !299
  store i32 %ADD, i32* %i, align 4, !dbg !299
  br label %while.start, !dbg !299

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !299
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !299
  ret %struct.String %res7, !dbg !299
}

define %struct.String @String.reverse(%struct.String* %self) !dbg !300 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !301
  %res = alloca %struct.String, align 8, !dbg !301
  %i = alloca i32, align 4, !dbg !301
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !301
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !301
  %0 = call %struct.String @String.new(), !dbg !302
  %tmp_struct = alloca %struct.String, align 8, !dbg !302
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !302
  %tmp_op = alloca %struct.String, align 8, !dbg !302
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !302
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !302
  store i32 0, i32* %i, align 4, !dbg !301
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !301
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !301
  %DOT = load i32, i32* %count, align 4, !dbg !301
  %SUB = sub i32 %DOT, 1, !dbg !301
  store i32 %SUB, i32* %i, align 4, !dbg !301
  br label %while.start, !dbg !301

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !301
  %GE = icmp sge i32 %i3, 0, !dbg !301
  br i1 %GE, label %while.then, label %while.end, !dbg !301

while.then:                                       ; preds = %while.start
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !303
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !303
  %DOT5 = load i8*, i8** %value, align 8, !dbg !303
  %i6 = load i32, i32* %i, align 4, !dbg !303
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !303
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !303
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !303
  %i7 = load i32, i32* %i, align 4, !dbg !303
  %SUB8 = sub i32 %i7, 1, !dbg !303
  store i32 %SUB8, i32* %i, align 4, !dbg !303
  br label %while.start, !dbg !303

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !303
  %res9 = load %struct.String, %struct.String* %res, align 8, !dbg !303
  ret %struct.String %res9, !dbg !303
}

define i32 @String.compare(%struct.String* %other, %struct.String* %self) !dbg !304 {
entry:
  %other1 = alloca %struct.String*, align 8, !dbg !305
  %self2 = alloca %struct.String*, align 8, !dbg !305
  store %struct.String* %other, %struct.String** %other1, align 8, !dbg !305
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !305
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !306
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !306
  %DOT = load i8*, i8** %value, align 8, !dbg !306
  %other4 = load %struct.String*, %struct.String** %other1, align 8, !dbg !306
  %value5 = getelementptr %struct.String, %struct.String* %other4, i32 0, i32 0, !dbg !306
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !306
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !306
  ret i32 %strcmp, !dbg !306
}

define i32 @String.to_int(%struct.String* %self) !dbg !307 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !308
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !308
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !309
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !309
  %DOT = load i8*, i8** %value, align 8, !dbg !309
  %atoi = call i32 @atoi(i8* %DOT), !dbg !309
  ret i32 %atoi, !dbg !309
}

define void @String.operator.ASSIGN.CHARS(i8* %str, %struct.String* %self) !dbg !310 {
entry:
  %str1 = alloca i8*, align 8, !dbg !311
  %self2 = alloca %struct.String*, align 8, !dbg !311
  store i8* %str, i8** %str1, align 8, !dbg !311
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !311
  %str3 = load i8*, i8** %str1, align 8, !dbg !312
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !312
  call void @String.assign(i8* %str3, %struct.String* %ref_arg), !dbg !312
  ret void, !dbg !312
}

define void @String.operator.ASSIGN.String(%struct.String* %v, %struct.String* %self) !dbg !313 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !314
  %self2 = alloca %struct.String*, align 8, !dbg !314
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !314
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !314
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !315
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !315
  %DOT = load i8*, i8** %value, align 8, !dbg !315
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !315
  call void @String.assign(i8* %DOT, %struct.String* %ref_arg), !dbg !315
  ret void, !dbg !315
}

define %struct.String @String.operator.ADD.String(%struct.String* %v, %struct.String* %self) !dbg !316 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !317
  %self2 = alloca %struct.String*, align 8, !dbg !317
  %res = alloca %struct.String, align 8, !dbg !317
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !317
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !317
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !317
  %0 = call %struct.String @String.new(), !dbg !318
  %tmp_struct = alloca %struct.String, align 8, !dbg !318
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !318
  %tmp_op = alloca %struct.String, align 8, !dbg !318
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !318
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !318
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !319
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !319
  %DOT = load i8*, i8** %value, align 8, !dbg !319
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !319
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !319
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !319
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %DOT6), !dbg !319
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !319
  call void @String.delete(%struct.String* %tmp_struct), !dbg !319
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !319
  ret %struct.String %res7, !dbg !319
}

define %struct.String @String.operator.ADD.CHARS(i8* %str, %struct.String* %self) !dbg !320 {
entry:
  %str1 = alloca i8*, align 8, !dbg !321
  %self2 = alloca %struct.String*, align 8, !dbg !321
  %res = alloca %struct.String, align 8, !dbg !321
  store i8* %str, i8** %str1, align 8, !dbg !321
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !321
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !321
  %0 = call %struct.String @String.new(), !dbg !322
  %tmp_struct = alloca %struct.String, align 8, !dbg !322
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !322
  %tmp_op = alloca %struct.String, align 8, !dbg !322
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !322
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !322
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !323
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !323
  %DOT = load i8*, i8** %value, align 8, !dbg !323
  %str4 = load i8*, i8** %str1, align 8, !dbg !323
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str4), !dbg !323
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !323
  call void @String.delete(%struct.String* %tmp_struct), !dbg !323
  %res5 = load %struct.String, %struct.String* %res, align 8, !dbg !323
  ret %struct.String %res5, !dbg !323
}

define void @String.operator.ADD_ASS.String(%struct.String* %v, %struct.String* %self) !dbg !324 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !325
  %self2 = alloca %struct.String*, align 8, !dbg !325
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !325
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !325
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !326
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !326
  %DOT = load i8*, i8** %value, align 8, !dbg !326
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !326
  call void @String.join(i8* %DOT, %struct.String* %ref_arg), !dbg !326
  ret void, !dbg !326
}

define void @String.operator.ADD_ASS.CHARS(i8* %str, %struct.String* %self) !dbg !327 {
entry:
  %str1 = alloca i8*, align 8, !dbg !328
  %self2 = alloca %struct.String*, align 8, !dbg !328
  store i8* %str, i8** %str1, align 8, !dbg !328
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !328
  %str3 = load i8*, i8** %str1, align 8, !dbg !329
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !329
  call void @String.join(i8* %str3, %struct.String* %ref_arg), !dbg !329
  ret void, !dbg !329
}

define i1 @String.operator.EQ.String(%struct.String* %v, %struct.String* %self) !dbg !330 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !331
  %self2 = alloca %struct.String*, align 8, !dbg !331
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !331
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !331
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !332
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !332
  %DOT = load i8*, i8** %value, align 8, !dbg !332
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !332
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !332
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !332
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !332
  %EQ = icmp eq i32 %strcmp, 0, !dbg !332
  ret i1 %EQ, !dbg !332
}

define i1 @String.operator.EQ.CHARS(i8* %str, %struct.String* %self) !dbg !333 {
entry:
  %str1 = alloca i8*, align 8, !dbg !334
  %self2 = alloca %struct.String*, align 8, !dbg !334
  store i8* %str, i8** %str1, align 8, !dbg !334
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !334
  br label %if.start, !dbg !334

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !334
  %EQ = icmp eq i8* %str3, null, !dbg !334
  br i1 %EQ, label %if.then, label %if.end, !dbg !334

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !335
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !335
  %DOT7 = load i8*, i8** %value, align 8, !dbg !335
  %str8 = load i8*, i8** %str1, align 8, !dbg !335
  %strcmp = call i32 @strcmp(i8* %DOT7, i8* %str8), !dbg !335
  %EQ9 = icmp eq i32 %strcmp, 0, !dbg !335
  ret i1 %EQ9, !dbg !335

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !334
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !334
  %DOT = load i32, i32* %count, align 4, !dbg !334
  %EQ5 = icmp eq i32 %DOT, 0, !dbg !334
  ret i1 %EQ5, !dbg !334
}

define i1 @String.operator.NEQ.String(%struct.String* %v, %struct.String* %self) !dbg !336 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !337
  %self2 = alloca %struct.String*, align 8, !dbg !337
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !337
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !337
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !338
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !338
  %DOT = load i8*, i8** %value, align 8, !dbg !338
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !338
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !338
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !338
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !338
  %NEQ = icmp ne i32 %strcmp, 0, !dbg !338
  ret i1 %NEQ, !dbg !338
}

define i1 @String.operator.NEQ.CHARS(i8* %str, %struct.String* %self) !dbg !339 {
entry:
  %str1 = alloca i8*, align 8, !dbg !340
  %self2 = alloca %struct.String*, align 8, !dbg !340
  store i8* %str, i8** %str1, align 8, !dbg !340
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !340
  br label %if.start, !dbg !340

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !340
  %EQ = icmp eq i8* %str3, null, !dbg !340
  br i1 %EQ, label %if.then, label %if.end, !dbg !340

if.end:                                           ; preds = %if.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !341
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !341
  %DOT6 = load i8*, i8** %value, align 8, !dbg !341
  %str7 = load i8*, i8** %str1, align 8, !dbg !341
  %strcmp = call i32 @strcmp(i8* %DOT6, i8* %str7), !dbg !341
  %NEQ8 = icmp ne i32 %strcmp, 0, !dbg !341
  ret i1 %NEQ8, !dbg !341

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !340
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !340
  %DOT = load i32, i32* %count, align 4, !dbg !340
  %NEQ = icmp ne i32 %DOT, 0, !dbg !340
  ret i1 %NEQ, !dbg !340
}

define i1 @String.operator.LT.String(%struct.String* %v, %struct.String* %self) !dbg !342 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !343
  %self2 = alloca %struct.String*, align 8, !dbg !343
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !343
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !343
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !344
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !344
  %DOT = load i8*, i8** %value, align 8, !dbg !344
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !344
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !344
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !344
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !344
  %LT = icmp slt i32 %strcmp, 0, !dbg !344
  ret i1 %LT, !dbg !344
}

define i1 @String.operator.GT.String(%struct.String* %v, %struct.String* %self) !dbg !345 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !346
  %self2 = alloca %struct.String*, align 8, !dbg !346
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !346
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !346
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !347
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !347
  %DOT = load i8*, i8** %value, align 8, !dbg !347
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !347
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !347
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !347
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !347
  %GT = icmp sgt i32 %strcmp, 0, !dbg !347
  ret i1 %GT, !dbg !347
}

define i8* @String.operator.output(%struct.String* %self) !dbg !348 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !349
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !349
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !349
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !349
  %DOT = load i8*, i8** %value, align 8, !dbg !349
  ret i8* %DOT, !dbg !349
}

define void @Color.delete(%struct.Color* %0) {
entry:
  ret void
}

define void @Piece.delete(%struct.Piece* %0) {
entry:
  ret void
}

define void @Game.delete(%struct.Game* %0) {
entry:
  %current = getelementptr %struct.Game, %struct.Game* %0, i32 0, i32 2
  call void @Piece.delete(%struct.Piece* %current)
  %next = getelementptr %struct.Game, %struct.Game* %0, i32 0, i32 3
  call void @Piece.delete(%struct.Piece* %next)
  ret void
}

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nounwind willreturn }

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "tetris.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/examples")
!4 = distinct !DISubprogram(name: "strjoin", linkageName: "strjoin", scope: null, file: !3, line: 58, type: !5, scopeLine: 58, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 58, scope: !4)
!8 = !DILocation(line: 61, scope: !4)
!9 = !DILocation(line: 63, scope: !4)
!10 = !DILocation(line: 62, scope: !4)
!11 = !DILocation(line: 66, scope: !4)
!12 = !DILocation(line: 70, scope: !4)
!13 = distinct !DISubprogram(name: "setColor", linkageName: "setColor", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 9, scope: !13)
!15 = distinct !DISubprogram(name: "init_shapes", linkageName: "init_shapes", scope: null, file: !3, line: 41, type: !5, scopeLine: 41, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!16 = !DILocation(line: 41, scope: !15)
!17 = !DILocation(line: 43, scope: !15)
!18 = !DILocation(line: 44, scope: !15)
!19 = !DILocation(line: 45, scope: !15)
!20 = !DILocation(line: 46, scope: !15)
!21 = !DILocation(line: 48, scope: !15)
!22 = !DILocation(line: 49, scope: !15)
!23 = !DILocation(line: 50, scope: !15)
!24 = !DILocation(line: 51, scope: !15)
!25 = !DILocation(line: 53, scope: !15)
!26 = !DILocation(line: 54, scope: !15)
!27 = !DILocation(line: 55, scope: !15)
!28 = !DILocation(line: 56, scope: !15)
!29 = !DILocation(line: 58, scope: !15)
!30 = !DILocation(line: 59, scope: !15)
!31 = !DILocation(line: 60, scope: !15)
!32 = !DILocation(line: 61, scope: !15)
!33 = !DILocation(line: 63, scope: !15)
!34 = !DILocation(line: 64, scope: !15)
!35 = !DILocation(line: 65, scope: !15)
!36 = !DILocation(line: 66, scope: !15)
!37 = !DILocation(line: 68, scope: !15)
!38 = !DILocation(line: 69, scope: !15)
!39 = !DILocation(line: 70, scope: !15)
!40 = !DILocation(line: 71, scope: !15)
!41 = !DILocation(line: 73, scope: !15)
!42 = !DILocation(line: 74, scope: !15)
!43 = !DILocation(line: 75, scope: !15)
!44 = !DILocation(line: 76, scope: !15)
!45 = distinct !DISubprogram(name: "cell_occupied", linkageName: "cell_occupied", scope: null, file: !3, line: 78, type: !5, scopeLine: 78, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!46 = !DILocation(line: 78, scope: !45)
!47 = !DILocation(line: 79, scope: !45)
!48 = distinct !DISubprogram(name: "piece_color", linkageName: "piece_color", scope: null, file: !3, line: 83, type: !5, scopeLine: 83, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!49 = !DILocation(line: 83, scope: !48)
!50 = !DILocation(line: 84, scope: !48)
!51 = !DILocation(line: 85, scope: !48)
!52 = !DILocation(line: 86, scope: !48)
!53 = !DILocation(line: 87, scope: !48)
!54 = !DILocation(line: 88, scope: !48)
!55 = !DILocation(line: 90, scope: !48)
!56 = !DILocation(line: 89, scope: !48)
!57 = distinct !DISubprogram(name: "spawn_piece", linkageName: "spawn_piece", scope: null, file: !3, line: 93, type: !5, scopeLine: 93, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!58 = !DILocation(line: 93, scope: !57)
!59 = distinct !DISubprogram(name: "random_piece", linkageName: "random_piece", scope: null, file: !3, line: 101, type: !5, scopeLine: 101, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!60 = !DILocation(line: 102, scope: !59)
!61 = distinct !DISubprogram(name: "tick_interval", linkageName: "tick_interval", scope: null, file: !3, line: 104, type: !5, scopeLine: 104, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!62 = !DILocation(line: 104, scope: !61)
!63 = distinct !DISubprogram(name: "is_valid", linkageName: "is_valid", scope: null, file: !3, line: 110, type: !5, scopeLine: 110, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!64 = !DILocation(line: 110, scope: !63)
!65 = !DILocation(line: 120, scope: !63)
!66 = !DILocation(line: 115, scope: !63)
!67 = distinct !DISubprogram(name: "clear_lines", linkageName: "clear_lines", scope: null, file: !3, line: 126, type: !5, scopeLine: 126, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!68 = !DILocation(line: 126, scope: !67)
!69 = !DILocation(line: 147, scope: !67)
!70 = !DILocation(line: 133, scope: !67)
!71 = !DILocation(line: 142, scope: !67)
!72 = distinct !DISubprogram(name: "update_score", linkageName: "update_score", scope: null, file: !3, line: 154, type: !5, scopeLine: 154, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!73 = !DILocation(line: 154, scope: !72)
!74 = !DILocation(line: 158, scope: !72)
!75 = !DILocation(line: 161, scope: !72)
!76 = distinct !DISubprogram(name: "lock_piece", linkageName: "lock_piece", scope: null, file: !3, line: 164, type: !5, scopeLine: 164, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!77 = !DILocation(line: 164, scope: !76)
!78 = !DILocation(line: 178, scope: !76)
!79 = !DILocation(line: 175, scope: !76)
!80 = !DILocation(line: 169, scope: !76)
!81 = !DILocation(line: 179, scope: !76)
!82 = !DILocation(line: 181, scope: !76)
!83 = !DILocation(line: 183, scope: !76)
!84 = distinct !DISubprogram(name: "move_left", linkageName: "move_left", scope: null, file: !3, line: 187, type: !5, scopeLine: 187, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!85 = !DILocation(line: 187, scope: !84)
!86 = !DILocation(line: 190, scope: !84)
!87 = distinct !DISubprogram(name: "move_right", linkageName: "move_right", scope: null, file: !3, line: 194, type: !5, scopeLine: 194, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!88 = !DILocation(line: 194, scope: !87)
!89 = !DILocation(line: 197, scope: !87)
!90 = distinct !DISubprogram(name: "move_down", linkageName: "move_down", scope: null, file: !3, line: 201, type: !5, scopeLine: 201, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!91 = !DILocation(line: 201, scope: !90)
!92 = !DILocation(line: 204, scope: !90)
!93 = distinct !DISubprogram(name: "rotate_cw", linkageName: "rotate_cw", scope: null, file: !3, line: 208, type: !5, scopeLine: 208, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!94 = !DILocation(line: 208, scope: !93)
!95 = !DILocation(line: 211, scope: !93)
!96 = !DILocation(line: 215, scope: !93)
!97 = !DILocation(line: 219, scope: !93)
!98 = !DILocation(line: 223, scope: !93)
!99 = distinct !DISubprogram(name: "hard_drop", linkageName: "hard_drop", scope: null, file: !3, line: 226, type: !5, scopeLine: 226, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!100 = !DILocation(line: 226, scope: !99)
!101 = !DILocation(line: 228, scope: !99)
!102 = !DILocation(line: 231, scope: !99)
!103 = distinct !DISubprogram(name: "ghost_row", linkageName: "ghost_row", scope: null, file: !3, line: 233, type: !5, scopeLine: 233, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!104 = !DILocation(line: 233, scope: !103)
!105 = !DILocation(line: 238, scope: !103)
!106 = distinct !DISubprogram(name: "handle_input", linkageName: "handle_input", scope: null, file: !3, line: 243, type: !5, scopeLine: 243, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!107 = !DILocation(line: 243, scope: !106)
!108 = !DILocation(line: 245, scope: !106)
!109 = !DILocation(line: 249, scope: !106)
!110 = !DILocation(line: 250, scope: !106)
!111 = !DILocation(line: 251, scope: !106)
!112 = !DILocation(line: 252, scope: !106)
!113 = !DILocation(line: 253, scope: !106)
!114 = distinct !DISubprogram(name: "init_game", linkageName: "init_game", scope: null, file: !3, line: 256, type: !5, scopeLine: 256, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!115 = !DILocation(line: 256, scope: !114)
!116 = !DILocation(line: 261, scope: !114)
!117 = !DILocation(line: 268, scope: !114)
!118 = !DILocation(line: 271, scope: !114)
!119 = !DILocation(line: 272, scope: !114)
!120 = distinct !DISubprogram(name: "update_game", linkageName: "update_game", scope: null, file: !3, line: 275, type: !5, scopeLine: 275, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!121 = !DILocation(line: 275, scope: !120)
!122 = !DILocation(line: 280, scope: !120)
!123 = !DILocation(line: 279, scope: !120)
!124 = distinct !DISubprogram(name: "draw_cell", linkageName: "draw_cell", scope: null, file: !3, line: 283, type: !5, scopeLine: 283, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!125 = !DILocation(line: 283, scope: !124)
!126 = !DILocation(line: 286, scope: !124)
!127 = !DILocation(line: 287, scope: !124)
!128 = distinct !DISubprogram(name: "draw_piece_at", linkageName: "draw_piece_at", scope: null, file: !3, line: 289, type: !5, scopeLine: 289, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!129 = !DILocation(line: 289, scope: !128)
!130 = !DILocation(line: 295, scope: !128)
!131 = !DILocation(line: 294, scope: !128)
!132 = distinct !DISubprogram(name: "draw_board", linkageName: "draw_board", scope: null, file: !3, line: 299, type: !5, scopeLine: 299, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!133 = !DILocation(line: 299, scope: !132)
!134 = !DILocation(line: 300, scope: !132)
!135 = !DILocation(line: 303, scope: !132)
!136 = !DILocation(line: 307, scope: !132)
!137 = !DILocation(line: 317, scope: !132)
!138 = !DILocation(line: 314, scope: !132)
!139 = !DILocation(line: 313, scope: !132)
!140 = distinct !DISubprogram(name: "draw_ghost", linkageName: "draw_ghost", scope: null, file: !3, line: 319, type: !5, scopeLine: 319, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!141 = !DILocation(line: 319, scope: !140)
!142 = !DILocation(line: 320, scope: !140)
!143 = !DILocation(line: 322, scope: !140)
!144 = !DILocation(line: 324, scope: !140)
!145 = distinct !DISubprogram(name: "draw_current", linkageName: "draw_current", scope: null, file: !3, line: 326, type: !5, scopeLine: 326, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!146 = !DILocation(line: 326, scope: !145)
!147 = !DILocation(line: 327, scope: !145)
!148 = distinct !DISubprogram(name: "draw_hud", linkageName: "draw_hud", scope: null, file: !3, line: 329, type: !5, scopeLine: 329, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!149 = !DILocation(line: 329, scope: !148)
!150 = !DILocation(line: 332, scope: !148)
!151 = !DILocation(line: 333, scope: !148)
!152 = !DILocation(line: 334, scope: !148)
!153 = !DILocation(line: 335, scope: !148)
!154 = !DILocation(line: 336, scope: !148)
!155 = !DILocation(line: 337, scope: !148)
!156 = !DILocation(line: 338, scope: !148)
!157 = !DILocation(line: 339, scope: !148)
!158 = !DILocation(line: 343, scope: !148)
!159 = !DILocation(line: 344, scope: !148)
!160 = !DILocation(line: 357, scope: !148)
!161 = !DILocation(line: 358, scope: !148)
!162 = !DILocation(line: 359, scope: !148)
!163 = !DILocation(line: 360, scope: !148)
!164 = !DILocation(line: 354, scope: !148)
!165 = !DILocation(line: 349, scope: !148)
!166 = !DILocation(line: 352, scope: !148)
!167 = !DILocation(line: 353, scope: !148)
!168 = distinct !DISubprogram(name: "draw_game_over", linkageName: "draw_game_over", scope: null, file: !3, line: 362, type: !5, scopeLine: 362, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!169 = !DILocation(line: 362, scope: !168)
!170 = !DILocation(line: 363, scope: !168)
!171 = !DILocation(line: 367, scope: !168)
!172 = !DILocation(line: 368, scope: !168)
!173 = !DILocation(line: 369, scope: !168)
!174 = !DILocation(line: 371, scope: !168)
!175 = distinct !DISubprogram(name: "draw_pause", linkageName: "draw_pause", scope: null, file: !3, line: 373, type: !5, scopeLine: 373, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!176 = !DILocation(line: 374, scope: !175)
!177 = !DILocation(line: 378, scope: !175)
!178 = distinct !DISubprogram(name: "render", linkageName: "render", scope: null, file: !3, line: 380, type: !5, scopeLine: 380, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!179 = !DILocation(line: 380, scope: !178)
!180 = !DILocation(line: 381, scope: !178)
!181 = !DILocation(line: 382, scope: !178)
!182 = !DILocation(line: 386, scope: !178)
!183 = !DILocation(line: 384, scope: !178)
!184 = !DILocation(line: 385, scope: !178)
!185 = !DILocation(line: 387, scope: !178)
!186 = !DILocation(line: 388, scope: !178)
!187 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 391, type: !5, scopeLine: 391, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!188 = !DILocation(line: 392, scope: !187)
!189 = !DILocation(line: 393, scope: !187)
!190 = !DILocation(line: 394, scope: !187)
!191 = !DILocation(line: 399, scope: !187)
!192 = !DILocation(line: 400, scope: !187)
!193 = !DILocation(line: 402, scope: !187)
!194 = !DILocation(line: 403, scope: !187)
!195 = !DILocation(line: 404, scope: !187)
!196 = !DILocation(line: 413, scope: !187)
!197 = !DILocation(line: 405, scope: !187)
!198 = !DILocation(line: 406, scope: !187)
!199 = !DILocation(line: 409, scope: !187)
!200 = !DILocation(line: 410, scope: !187)
!201 = !DILocation(line: 411, scope: !187)
!202 = !DILocation(line: 408, scope: !187)
!203 = distinct !DISubprogram(name: "String.delete", linkageName: "String.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!204 = !DILocation(line: 0, scope: !203)
!205 = !DILocation(line: 82, scope: !203)
!206 = distinct !DISubprogram(name: "String.assign", linkageName: "String.assign", scope: null, file: !3, line: 86, type: !5, scopeLine: 86, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!207 = !DILocation(line: 86, scope: !206)
!208 = !DILocation(line: 88, scope: !206)
!209 = !DILocation(line: 95, scope: !206)
!210 = !DILocation(line: 96, scope: !206)
!211 = !DILocation(line: 94, scope: !206)
!212 = distinct !DISubprogram(name: "String.join", linkageName: "String.join", scope: null, file: !3, line: 99, type: !5, scopeLine: 99, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!213 = !DILocation(line: 99, scope: !212)
!214 = !DILocation(line: 101, scope: !212)
!215 = !DILocation(line: 102, scope: !212)
!216 = !DILocation(line: 103, scope: !212)
!217 = distinct !DISubprogram(name: "String.clear", linkageName: "String.clear", scope: null, file: !3, line: 105, type: !5, scopeLine: 105, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!218 = !DILocation(line: 105, scope: !217)
!219 = !DILocation(line: 106, scope: !217)
!220 = distinct !DISubprogram(name: "String.push", linkageName: "String.push", scope: null, file: !3, line: 109, type: !5, scopeLine: 109, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!221 = !DILocation(line: 109, scope: !220)
!222 = !DILocation(line: 116, scope: !220)
!223 = !DILocation(line: 118, scope: !220)
!224 = !DILocation(line: 115, scope: !220)
!225 = distinct !DISubprogram(name: "String.pop", linkageName: "String.pop", scope: null, file: !3, line: 120, type: !5, scopeLine: 120, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!226 = !DILocation(line: 120, scope: !225)
!227 = !DILocation(line: 124, scope: !225)
!228 = !DILocation(line: 125, scope: !225)
!229 = distinct !DISubprogram(name: "String.new", linkageName: "String.new", scope: null, file: !3, line: 130, type: !5, scopeLine: 130, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!230 = !DILocation(line: 130, scope: !229)
!231 = !DILocation(line: 133, scope: !229)
!232 = distinct !DISubprogram(name: "String.from", linkageName: "String.from", scope: null, file: !3, line: 136, type: !5, scopeLine: 136, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!233 = !DILocation(line: 136, scope: !232)
!234 = !DILocation(line: 137, scope: !232)
!235 = !DILocation(line: 138, scope: !232)
!236 = distinct !DISubprogram(name: "String.from_int", linkageName: "String.from_int", scope: null, file: !3, line: 141, type: !5, scopeLine: 141, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!237 = !DILocation(line: 141, scope: !236)
!238 = !DILocation(line: 142, scope: !236)
!239 = !DILocation(line: 144, scope: !236)
!240 = !DILocation(line: 154, scope: !236)
!241 = !DILocation(line: 158, scope: !236)
!242 = !DILocation(line: 161, scope: !236)
!243 = distinct !DISubprogram(name: "String.len", linkageName: "String.len", scope: null, file: !3, line: 167, type: !5, scopeLine: 167, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!244 = !DILocation(line: 167, scope: !243)
!245 = distinct !DISubprogram(name: "String.empty", linkageName: "String.empty", scope: null, file: !3, line: 170, type: !5, scopeLine: 170, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!246 = !DILocation(line: 170, scope: !245)
!247 = distinct !DISubprogram(name: "String.c_str", linkageName: "String.c_str", scope: null, file: !3, line: 173, type: !5, scopeLine: 173, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!248 = !DILocation(line: 173, scope: !247)
!249 = distinct !DISubprogram(name: "String.at", linkageName: "String.at", scope: null, file: !3, line: 176, type: !5, scopeLine: 176, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!250 = !DILocation(line: 176, scope: !249)
!251 = !DILocation(line: 179, scope: !249)
!252 = distinct !DISubprogram(name: "String.find", linkageName: "String.find", scope: null, file: !3, line: 183, type: !5, scopeLine: 183, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!253 = !DILocation(line: 183, scope: !252)
!254 = !DILocation(line: 186, scope: !252)
!255 = !DILocation(line: 189, scope: !252)
!256 = distinct !DISubprogram(name: "String.contains", linkageName: "String.contains", scope: null, file: !3, line: 194, type: !5, scopeLine: 194, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!257 = !DILocation(line: 194, scope: !256)
!258 = !DILocation(line: 195, scope: !256)
!259 = distinct !DISubprogram(name: "String.starts_with", linkageName: "String.starts_with", scope: null, file: !3, line: 197, type: !5, scopeLine: 197, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!260 = !DILocation(line: 197, scope: !259)
!261 = !DILocation(line: 199, scope: !259)
!262 = !DILocation(line: 201, scope: !259)
!263 = distinct !DISubprogram(name: "String.ends_with", linkageName: "String.ends_with", scope: null, file: !3, line: 203, type: !5, scopeLine: 203, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!264 = !DILocation(line: 203, scope: !263)
!265 = !DILocation(line: 205, scope: !263)
!266 = !DILocation(line: 208, scope: !263)
!267 = distinct !DISubprogram(name: "String.substr", linkageName: "String.substr", scope: null, file: !3, line: 212, type: !5, scopeLine: 212, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!268 = !DILocation(line: 212, scope: !267)
!269 = !DILocation(line: 213, scope: !267)
!270 = !DILocation(line: 221, scope: !267)
!271 = distinct !DISubprogram(name: "String.upper", linkageName: "String.upper", scope: null, file: !3, line: 225, type: !5, scopeLine: 225, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!272 = !DILocation(line: 225, scope: !271)
!273 = !DILocation(line: 226, scope: !271)
!274 = !DILocation(line: 229, scope: !271)
!275 = !DILocation(line: 234, scope: !271)
!276 = !DILocation(line: 232, scope: !271)
!277 = distinct !DISubprogram(name: "String.lower", linkageName: "String.lower", scope: null, file: !3, line: 238, type: !5, scopeLine: 238, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!278 = !DILocation(line: 238, scope: !277)
!279 = !DILocation(line: 239, scope: !277)
!280 = !DILocation(line: 242, scope: !277)
!281 = !DILocation(line: 247, scope: !277)
!282 = !DILocation(line: 245, scope: !277)
!283 = distinct !DISubprogram(name: "String.trim", linkageName: "String.trim", scope: null, file: !3, line: 251, type: !5, scopeLine: 251, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!284 = !DILocation(line: 251, scope: !283)
!285 = !DILocation(line: 254, scope: !283)
!286 = !DILocation(line: 261, scope: !283)
!287 = !DILocation(line: 266, scope: !283)
!288 = distinct !DISubprogram(name: "String.replace", linkageName: "String.replace", scope: null, file: !3, line: 268, type: !5, scopeLine: 268, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!289 = !DILocation(line: 268, scope: !288)
!290 = !DILocation(line: 269, scope: !288)
!291 = !DILocation(line: 270, scope: !288)
!292 = !DILocation(line: 272, scope: !288)
!293 = !DILocation(line: 280, scope: !288)
!294 = !DILocation(line: 276, scope: !288)
!295 = !DILocation(line: 277, scope: !288)
!296 = distinct !DISubprogram(name: "String.repeat", linkageName: "String.repeat", scope: null, file: !3, line: 284, type: !5, scopeLine: 284, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!297 = !DILocation(line: 284, scope: !296)
!298 = !DILocation(line: 285, scope: !296)
!299 = !DILocation(line: 288, scope: !296)
!300 = distinct !DISubprogram(name: "String.reverse", linkageName: "String.reverse", scope: null, file: !3, line: 292, type: !5, scopeLine: 292, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!301 = !DILocation(line: 292, scope: !300)
!302 = !DILocation(line: 293, scope: !300)
!303 = !DILocation(line: 296, scope: !300)
!304 = distinct !DISubprogram(name: "String.compare", linkageName: "String.compare", scope: null, file: !3, line: 302, type: !5, scopeLine: 302, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!305 = !DILocation(line: 302, scope: !304)
!306 = !DILocation(line: 303, scope: !304)
!307 = distinct !DISubprogram(name: "String.to_int", linkageName: "String.to_int", scope: null, file: !3, line: 307, type: !5, scopeLine: 307, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!308 = !DILocation(line: 307, scope: !307)
!309 = !DILocation(line: 308, scope: !307)
!310 = distinct !DISubprogram(name: "String.operator.ASSIGN.CHARS", linkageName: "String.operator.ASSIGN.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!311 = !DILocation(line: 0, scope: !310)
!312 = !DILocation(line: 313, scope: !310)
!313 = distinct !DISubprogram(name: "String.operator.ASSIGN.String", linkageName: "String.operator.ASSIGN.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!314 = !DILocation(line: 0, scope: !313)
!315 = !DILocation(line: 316, scope: !313)
!316 = distinct !DISubprogram(name: "String.operator.ADD.String", linkageName: "String.operator.ADD.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!317 = !DILocation(line: 0, scope: !316)
!318 = !DILocation(line: 321, scope: !316)
!319 = !DILocation(line: 322, scope: !316)
!320 = distinct !DISubprogram(name: "String.operator.ADD.CHARS", linkageName: "String.operator.ADD.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!321 = !DILocation(line: 0, scope: !320)
!322 = !DILocation(line: 326, scope: !320)
!323 = !DILocation(line: 327, scope: !320)
!324 = distinct !DISubprogram(name: "String.operator.ADD_ASS.String", linkageName: "String.operator.ADD_ASS.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!325 = !DILocation(line: 0, scope: !324)
!326 = !DILocation(line: 331, scope: !324)
!327 = distinct !DISubprogram(name: "String.operator.ADD_ASS.CHARS", linkageName: "String.operator.ADD_ASS.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!328 = !DILocation(line: 0, scope: !327)
!329 = !DILocation(line: 334, scope: !327)
!330 = distinct !DISubprogram(name: "String.operator.EQ.String", linkageName: "String.operator.EQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!331 = !DILocation(line: 0, scope: !330)
!332 = !DILocation(line: 339, scope: !330)
!333 = distinct !DISubprogram(name: "String.operator.EQ.CHARS", linkageName: "String.operator.EQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!334 = !DILocation(line: 0, scope: !333)
!335 = !DILocation(line: 343, scope: !333)
!336 = distinct !DISubprogram(name: "String.operator.NEQ.String", linkageName: "String.operator.NEQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!337 = !DILocation(line: 0, scope: !336)
!338 = !DILocation(line: 346, scope: !336)
!339 = distinct !DISubprogram(name: "String.operator.NEQ.CHARS", linkageName: "String.operator.NEQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!340 = !DILocation(line: 0, scope: !339)
!341 = !DILocation(line: 350, scope: !339)
!342 = distinct !DISubprogram(name: "String.operator.LT.String", linkageName: "String.operator.LT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!343 = !DILocation(line: 0, scope: !342)
!344 = !DILocation(line: 353, scope: !342)
!345 = distinct !DISubprogram(name: "String.operator.GT.String", linkageName: "String.operator.GT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!346 = !DILocation(line: 0, scope: !345)
!347 = !DILocation(line: 356, scope: !345)
!348 = distinct !DISubprogram(name: "String.operator.output", linkageName: "String.operator.output", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!349 = !DILocation(line: 0, scope: !348)
