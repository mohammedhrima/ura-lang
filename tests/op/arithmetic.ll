; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"%s%d\0A\00", align 1
@STR1 = private unnamed_addr constant [16 x i8] c"add var+var:   \00", align 1
@STR2 = private unnamed_addr constant [16 x i8] c"add var+val:   \00", align 1
@STR3 = private unnamed_addr constant [16 x i8] c"add var+fn:    \00", align 1
@STR4 = private unnamed_addr constant [16 x i8] c"add val+var:   \00", align 1
@STR5 = private unnamed_addr constant [16 x i8] c"add val+val:   \00", align 1
@STR6 = private unnamed_addr constant [16 x i8] c"add val+fn:    \00", align 1
@STR7 = private unnamed_addr constant [16 x i8] c"add fn+var:    \00", align 1
@STR8 = private unnamed_addr constant [16 x i8] c"add fn+val:    \00", align 1
@STR9 = private unnamed_addr constant [16 x i8] c"add fn+fn:     \00", align 1
@STR10 = private unnamed_addr constant [16 x i8] c"sub var-var:   \00", align 1
@STR11 = private unnamed_addr constant [16 x i8] c"sub var-val:   \00", align 1
@STR12 = private unnamed_addr constant [16 x i8] c"sub var-fn:    \00", align 1
@STR13 = private unnamed_addr constant [16 x i8] c"sub val-var:   \00", align 1
@STR14 = private unnamed_addr constant [16 x i8] c"sub val-val:   \00", align 1
@STR15 = private unnamed_addr constant [16 x i8] c"sub val-fn:    \00", align 1
@STR16 = private unnamed_addr constant [16 x i8] c"sub fn-var:    \00", align 1
@STR17 = private unnamed_addr constant [16 x i8] c"sub fn-val:    \00", align 1
@STR18 = private unnamed_addr constant [16 x i8] c"sub fn-fn:     \00", align 1
@STR19 = private unnamed_addr constant [16 x i8] c"mul var*var:   \00", align 1
@STR20 = private unnamed_addr constant [16 x i8] c"mul var*val:   \00", align 1
@STR21 = private unnamed_addr constant [16 x i8] c"mul var*fn:    \00", align 1
@STR22 = private unnamed_addr constant [16 x i8] c"mul val*var:   \00", align 1
@STR23 = private unnamed_addr constant [16 x i8] c"mul val*val:   \00", align 1
@STR24 = private unnamed_addr constant [16 x i8] c"mul val*fn:    \00", align 1
@STR25 = private unnamed_addr constant [16 x i8] c"mul fn*var:    \00", align 1
@STR26 = private unnamed_addr constant [16 x i8] c"mul fn*val:    \00", align 1
@STR27 = private unnamed_addr constant [16 x i8] c"mul fn*fn:     \00", align 1
@STR28 = private unnamed_addr constant [16 x i8] c"div var/var:   \00", align 1
@STR29 = private unnamed_addr constant [16 x i8] c"div var/val:   \00", align 1
@STR30 = private unnamed_addr constant [16 x i8] c"div var/fn:    \00", align 1
@STR31 = private unnamed_addr constant [16 x i8] c"div val/var:   \00", align 1
@STR32 = private unnamed_addr constant [16 x i8] c"div val/val:   \00", align 1
@STR33 = private unnamed_addr constant [16 x i8] c"div val/fn:    \00", align 1
@STR34 = private unnamed_addr constant [16 x i8] c"div fn/var:    \00", align 1
@STR35 = private unnamed_addr constant [16 x i8] c"div fn/val:    \00", align 1
@STR36 = private unnamed_addr constant [16 x i8] c"div fn/fn:     \00", align 1
@STR37 = private unnamed_addr constant [16 x i8] c"mod var%var:   \00", align 1
@STR38 = private unnamed_addr constant [16 x i8] c"mod var%val:   \00", align 1
@STR39 = private unnamed_addr constant [16 x i8] c"mod var%fn:    \00", align 1
@STR40 = private unnamed_addr constant [16 x i8] c"mod val%var:   \00", align 1
@STR41 = private unnamed_addr constant [16 x i8] c"mod val%val:   \00", align 1
@STR42 = private unnamed_addr constant [16 x i8] c"mod val%fn:    \00", align 1
@STR43 = private unnamed_addr constant [16 x i8] c"mod fn%var:    \00", align 1
@STR44 = private unnamed_addr constant [16 x i8] c"mod fn%val:    \00", align 1
@STR45 = private unnamed_addr constant [16 x i8] c"mod fn%fn:     \00", align 1
@STR46 = private unnamed_addr constant [16 x i8] c"chain a+b*2:   \00", align 1
@STR47 = private unnamed_addr constant [16 x i8] c"chain (a+b)*2: \00", align 1
@STR48 = private unnamed_addr constant [16 x i8] c"chain a-b+foo: \00", align 1
@STR49 = private unnamed_addr constant [16 x i8] c"chain nested:  \00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @foo() !dbg !4 {
entry:
  ret i32 5, !dbg !7
}

define i32 @bar() !dbg !8 {
entry:
  ret i32 10, !dbg !9
}

define void @print_int(i8* %label, i32 %val) !dbg !10 {
entry:
  %label1 = alloca i8*, align 8, !dbg !11
  %val2 = alloca i32, align 4, !dbg !11
  store i8* %label, i8** %label1, align 8, !dbg !11
  store i32 %val, i32* %val2, align 4, !dbg !11
  %label3 = load i8*, i8** %label1, align 8, !dbg !11
  %val4 = load i32, i32* %val2, align 4, !dbg !11
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %label3, i32 %val4), !dbg !11
  ret void, !dbg !11
}

define i32 @main() !dbg !12 {
entry:
  %a = alloca i32, align 4, !dbg !13
  %b = alloca i32, align 4, !dbg !13
  store i32 0, i32* %a, align 4, !dbg !13
  store i32 20, i32* %a, align 4, !dbg !13
  store i32 0, i32* %b, align 4, !dbg !13
  store i32 4, i32* %b, align 4, !dbg !13
  %a1 = load i32, i32* %a, align 4, !dbg !14
  %b2 = load i32, i32* %b, align 4, !dbg !14
  %ADD = add i32 %a1, %b2, !dbg !14
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR1, i32 0, i32 0), i32 %ADD), !dbg !14
  %a3 = load i32, i32* %a, align 4, !dbg !15
  %ADD4 = add i32 %a3, 15, !dbg !15
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR2, i32 0, i32 0), i32 %ADD4), !dbg !15
  %foo = call i32 @foo(), !dbg !16
  %a5 = load i32, i32* %a, align 4, !dbg !16
  %ADD6 = add i32 %a5, %foo, !dbg !16
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR3, i32 0, i32 0), i32 %ADD6), !dbg !16
  %b7 = load i32, i32* %b, align 4, !dbg !17
  %ADD8 = add i32 25, %b7, !dbg !17
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR4, i32 0, i32 0), i32 %ADD8), !dbg !17
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR5, i32 0, i32 0), i32 70), !dbg !18
  %bar = call i32 @bar(), !dbg !19
  %ADD9 = add i32 50, %bar, !dbg !19
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR6, i32 0, i32 0), i32 %ADD9), !dbg !19
  %foo10 = call i32 @foo(), !dbg !20
  %a11 = load i32, i32* %a, align 4, !dbg !20
  %ADD12 = add i32 %foo10, %a11, !dbg !20
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR7, i32 0, i32 0), i32 %ADD12), !dbg !20
  %bar13 = call i32 @bar(), !dbg !21
  %ADD14 = add i32 %bar13, 100, !dbg !21
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR8, i32 0, i32 0), i32 %ADD14), !dbg !21
  %foo15 = call i32 @foo(), !dbg !22
  %bar16 = call i32 @bar(), !dbg !22
  %ADD17 = add i32 %foo15, %bar16, !dbg !22
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR9, i32 0, i32 0), i32 %ADD17), !dbg !22
  %a18 = load i32, i32* %a, align 4, !dbg !23
  %b19 = load i32, i32* %b, align 4, !dbg !23
  %SUB = sub i32 %a18, %b19, !dbg !23
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR10, i32 0, i32 0), i32 %SUB), !dbg !23
  %a20 = load i32, i32* %a, align 4, !dbg !24
  %SUB21 = sub i32 %a20, 5, !dbg !24
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR11, i32 0, i32 0), i32 %SUB21), !dbg !24
  %foo22 = call i32 @foo(), !dbg !25
  %a23 = load i32, i32* %a, align 4, !dbg !25
  %SUB24 = sub i32 %a23, %foo22, !dbg !25
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR12, i32 0, i32 0), i32 %SUB24), !dbg !25
  %b25 = load i32, i32* %b, align 4, !dbg !26
  %SUB26 = sub i32 30, %b25, !dbg !26
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR13, i32 0, i32 0), i32 %SUB26), !dbg !26
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR14, i32 0, i32 0), i32 35), !dbg !27
  %bar27 = call i32 @bar(), !dbg !28
  %SUB28 = sub i32 40, %bar27, !dbg !28
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR15, i32 0, i32 0), i32 %SUB28), !dbg !28
  %foo29 = call i32 @foo(), !dbg !29
  %b30 = load i32, i32* %b, align 4, !dbg !29
  %SUB31 = sub i32 %foo29, %b30, !dbg !29
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR16, i32 0, i32 0), i32 %SUB31), !dbg !29
  %bar32 = call i32 @bar(), !dbg !30
  %SUB33 = sub i32 %bar32, 3, !dbg !30
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR17, i32 0, i32 0), i32 %SUB33), !dbg !30
  %bar34 = call i32 @bar(), !dbg !31
  %foo35 = call i32 @foo(), !dbg !31
  %SUB36 = sub i32 %bar34, %foo35, !dbg !31
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR18, i32 0, i32 0), i32 %SUB36), !dbg !31
  %a37 = load i32, i32* %a, align 4, !dbg !32
  %b38 = load i32, i32* %b, align 4, !dbg !32
  %MUL = mul i32 %a37, %b38, !dbg !32
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR19, i32 0, i32 0), i32 %MUL), !dbg !32
  %a39 = load i32, i32* %a, align 4, !dbg !33
  %MUL40 = mul i32 %a39, 3, !dbg !33
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR20, i32 0, i32 0), i32 %MUL40), !dbg !33
  %foo41 = call i32 @foo(), !dbg !34
  %a42 = load i32, i32* %a, align 4, !dbg !34
  %MUL43 = mul i32 %a42, %foo41, !dbg !34
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR21, i32 0, i32 0), i32 %MUL43), !dbg !34
  %b44 = load i32, i32* %b, align 4, !dbg !35
  %MUL45 = mul i32 7, %b44, !dbg !35
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR22, i32 0, i32 0), i32 %MUL45), !dbg !35
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR23, i32 0, i32 0), i32 72), !dbg !36
  %bar46 = call i32 @bar(), !dbg !37
  %MUL47 = mul i32 2, %bar46, !dbg !37
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR24, i32 0, i32 0), i32 %MUL47), !dbg !37
  %foo48 = call i32 @foo(), !dbg !38
  %a49 = load i32, i32* %a, align 4, !dbg !38
  %MUL50 = mul i32 %foo48, %a49, !dbg !38
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR25, i32 0, i32 0), i32 %MUL50), !dbg !38
  %bar51 = call i32 @bar(), !dbg !39
  %MUL52 = mul i32 %bar51, 5, !dbg !39
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR26, i32 0, i32 0), i32 %MUL52), !dbg !39
  %foo53 = call i32 @foo(), !dbg !40
  %bar54 = call i32 @bar(), !dbg !40
  %MUL55 = mul i32 %foo53, %bar54, !dbg !40
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR27, i32 0, i32 0), i32 %MUL55), !dbg !40
  %a56 = load i32, i32* %a, align 4, !dbg !41
  %b57 = load i32, i32* %b, align 4, !dbg !41
  %DIV = sdiv i32 %a56, %b57, !dbg !41
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR28, i32 0, i32 0), i32 %DIV), !dbg !41
  %a58 = load i32, i32* %a, align 4, !dbg !42
  %DIV59 = sdiv i32 %a58, 2, !dbg !42
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR29, i32 0, i32 0), i32 %DIV59), !dbg !42
  %foo60 = call i32 @foo(), !dbg !43
  %a61 = load i32, i32* %a, align 4, !dbg !43
  %DIV62 = sdiv i32 %a61, %foo60, !dbg !43
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR30, i32 0, i32 0), i32 %DIV62), !dbg !43
  %b63 = load i32, i32* %b, align 4, !dbg !44
  %DIV64 = sdiv i32 100, %b63, !dbg !44
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR31, i32 0, i32 0), i32 %DIV64), !dbg !44
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR32, i32 0, i32 0), i32 5), !dbg !45
  %foo65 = call i32 @foo(), !dbg !46
  %DIV66 = sdiv i32 30, %foo65, !dbg !46
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR33, i32 0, i32 0), i32 %DIV66), !dbg !46
  %bar67 = call i32 @bar(), !dbg !47
  %b68 = load i32, i32* %b, align 4, !dbg !47
  %DIV69 = sdiv i32 %bar67, %b68, !dbg !47
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR34, i32 0, i32 0), i32 %DIV69), !dbg !47
  %bar70 = call i32 @bar(), !dbg !48
  %DIV71 = sdiv i32 %bar70, 2, !dbg !48
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR35, i32 0, i32 0), i32 %DIV71), !dbg !48
  %bar72 = call i32 @bar(), !dbg !49
  %foo73 = call i32 @foo(), !dbg !49
  %DIV74 = sdiv i32 %bar72, %foo73, !dbg !49
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR36, i32 0, i32 0), i32 %DIV74), !dbg !49
  %a75 = load i32, i32* %a, align 4, !dbg !50
  %b76 = load i32, i32* %b, align 4, !dbg !50
  %MOD = srem i32 %a75, %b76, !dbg !50
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR37, i32 0, i32 0), i32 %MOD), !dbg !50
  %a77 = load i32, i32* %a, align 4, !dbg !51
  %MOD78 = srem i32 %a77, 3, !dbg !51
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR38, i32 0, i32 0), i32 %MOD78), !dbg !51
  %foo79 = call i32 @foo(), !dbg !52
  %a80 = load i32, i32* %a, align 4, !dbg !52
  %MOD81 = srem i32 %a80, %foo79, !dbg !52
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR39, i32 0, i32 0), i32 %MOD81), !dbg !52
  %b82 = load i32, i32* %b, align 4, !dbg !53
  %MOD83 = srem i32 50, %b82, !dbg !53
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR40, i32 0, i32 0), i32 %MOD83), !dbg !53
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR41, i32 0, i32 0), i32 5), !dbg !54
  %foo84 = call i32 @foo(), !dbg !55
  %MOD85 = srem i32 33, %foo84, !dbg !55
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR42, i32 0, i32 0), i32 %MOD85), !dbg !55
  %bar86 = call i32 @bar(), !dbg !56
  %b87 = load i32, i32* %b, align 4, !dbg !56
  %MOD88 = srem i32 %bar86, %b87, !dbg !56
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR43, i32 0, i32 0), i32 %MOD88), !dbg !56
  %bar89 = call i32 @bar(), !dbg !57
  %MOD90 = srem i32 %bar89, 3, !dbg !57
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR44, i32 0, i32 0), i32 %MOD90), !dbg !57
  %bar91 = call i32 @bar(), !dbg !58
  %foo92 = call i32 @foo(), !dbg !58
  %MOD93 = srem i32 %bar91, %foo92, !dbg !58
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR45, i32 0, i32 0), i32 %MOD93), !dbg !58
  %b94 = load i32, i32* %b, align 4, !dbg !59
  %MUL95 = mul i32 %b94, 2, !dbg !59
  %a96 = load i32, i32* %a, align 4, !dbg !59
  %ADD97 = add i32 %a96, %MUL95, !dbg !59
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR46, i32 0, i32 0), i32 %ADD97), !dbg !59
  %a98 = load i32, i32* %a, align 4, !dbg !60
  %b99 = load i32, i32* %b, align 4, !dbg !60
  %ADD100 = add i32 %a98, %b99, !dbg !60
  %MUL101 = mul i32 %ADD100, 2, !dbg !60
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR47, i32 0, i32 0), i32 %MUL101), !dbg !60
  %a102 = load i32, i32* %a, align 4, !dbg !61
  %b103 = load i32, i32* %b, align 4, !dbg !61
  %SUB104 = sub i32 %a102, %b103, !dbg !61
  %foo105 = call i32 @foo(), !dbg !61
  %ADD106 = add i32 %SUB104, %foo105, !dbg !61
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR48, i32 0, i32 0), i32 %ADD106), !dbg !61
  %foo107 = call i32 @foo(), !dbg !62
  %bar108 = call i32 @bar(), !dbg !62
  %MUL109 = mul i32 %foo107, %bar108, !dbg !62
  %a110 = load i32, i32* %a, align 4, !dbg !62
  %ADD111 = add i32 %MUL109, %a110, !dbg !62
  %b112 = load i32, i32* %b, align 4, !dbg !62
  %SUB113 = sub i32 %ADD111, %b112, !dbg !62
  call void @print_int(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR49, i32 0, i32 0), i32 %SUB113), !dbg !62
  ret i32 0, !dbg !62
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "foo", linkageName: "foo", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = distinct !DISubprogram(name: "bar", linkageName: "bar", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 7, scope: !8)
!10 = distinct !DISubprogram(name: "print_int", linkageName: "print_int", scope: null, file: !3, line: 10, type: !5, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 10, scope: !10)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 13, scope: !12)
!14 = !DILocation(line: 18, scope: !12)
!15 = !DILocation(line: 19, scope: !12)
!16 = !DILocation(line: 20, scope: !12)
!17 = !DILocation(line: 21, scope: !12)
!18 = !DILocation(line: 22, scope: !12)
!19 = !DILocation(line: 23, scope: !12)
!20 = !DILocation(line: 24, scope: !12)
!21 = !DILocation(line: 25, scope: !12)
!22 = !DILocation(line: 26, scope: !12)
!23 = !DILocation(line: 29, scope: !12)
!24 = !DILocation(line: 30, scope: !12)
!25 = !DILocation(line: 31, scope: !12)
!26 = !DILocation(line: 32, scope: !12)
!27 = !DILocation(line: 33, scope: !12)
!28 = !DILocation(line: 34, scope: !12)
!29 = !DILocation(line: 35, scope: !12)
!30 = !DILocation(line: 36, scope: !12)
!31 = !DILocation(line: 37, scope: !12)
!32 = !DILocation(line: 40, scope: !12)
!33 = !DILocation(line: 41, scope: !12)
!34 = !DILocation(line: 42, scope: !12)
!35 = !DILocation(line: 43, scope: !12)
!36 = !DILocation(line: 44, scope: !12)
!37 = !DILocation(line: 45, scope: !12)
!38 = !DILocation(line: 46, scope: !12)
!39 = !DILocation(line: 47, scope: !12)
!40 = !DILocation(line: 48, scope: !12)
!41 = !DILocation(line: 51, scope: !12)
!42 = !DILocation(line: 52, scope: !12)
!43 = !DILocation(line: 53, scope: !12)
!44 = !DILocation(line: 54, scope: !12)
!45 = !DILocation(line: 55, scope: !12)
!46 = !DILocation(line: 56, scope: !12)
!47 = !DILocation(line: 57, scope: !12)
!48 = !DILocation(line: 58, scope: !12)
!49 = !DILocation(line: 59, scope: !12)
!50 = !DILocation(line: 62, scope: !12)
!51 = !DILocation(line: 63, scope: !12)
!52 = !DILocation(line: 64, scope: !12)
!53 = !DILocation(line: 65, scope: !12)
!54 = !DILocation(line: 66, scope: !12)
!55 = !DILocation(line: 67, scope: !12)
!56 = !DILocation(line: 68, scope: !12)
!57 = !DILocation(line: 69, scope: !12)
!58 = !DILocation(line: 70, scope: !12)
!59 = !DILocation(line: 73, scope: !12)
!60 = !DILocation(line: 74, scope: !12)
!61 = !DILocation(line: 75, scope: !12)
!62 = !DILocation(line: 76, scope: !12)
