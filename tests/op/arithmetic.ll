; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/op/arithmetic.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/op/arithmetic.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"%s%d\0A\00", align 1
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

declare i32 @printf(ptr, i32, ...)

define i32 @foo() !dbg !4 {
entry:
  ret i32 5, !dbg !7
}

define i32 @bar() !dbg !8 {
entry:
  ret i32 10, !dbg !9
}

define void @print_int(ptr %label, i32 %val) !dbg !10 {
entry:
  %label1 = alloca ptr, align 8, !dbg !11
  %val2 = alloca i32, align 4, !dbg !11
  store i32 0, ptr %val2, align 4, !dbg !11
  store ptr null, ptr %label1, align 8, !dbg !11
  store ptr %label, ptr %label1, align 8, !dbg !11
  store i32 %val, ptr %val2, align 4, !dbg !11
  %label3 = load ptr, ptr %label1, align 8, !dbg !12
  %val4 = load i32, ptr %val2, align 4, !dbg !12
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 2, ptr %label3, i32 %val4), !dbg !12
  ret void, !dbg !12
}

define i32 @main() !dbg !13 {
entry:
  %a = alloca i32, align 4, !dbg !14
  %b = alloca i32, align 4, !dbg !14
  store i32 0, ptr %b, align 4, !dbg !14
  store i32 0, ptr %a, align 4, !dbg !14
  store i32 20, ptr %a, align 4, !dbg !14
  store i32 4, ptr %b, align 4, !dbg !14
  %a1 = load i32, ptr %a, align 4, !dbg !15
  %b2 = load i32, ptr %b, align 4, !dbg !15
  %ADD = add i32 %a1, %b2, !dbg !15
  call void @print_int(ptr @STR1, i32 %ADD), !dbg !15
  %a3 = load i32, ptr %a, align 4, !dbg !16
  %ADD4 = add i32 %a3, 15, !dbg !16
  call void @print_int(ptr @STR2, i32 %ADD4), !dbg !16
  %foo = call i32 @foo(), !dbg !17
  %a5 = load i32, ptr %a, align 4, !dbg !17
  %ADD6 = add i32 %a5, %foo, !dbg !17
  call void @print_int(ptr @STR3, i32 %ADD6), !dbg !17
  %b7 = load i32, ptr %b, align 4, !dbg !18
  %ADD8 = add i32 25, %b7, !dbg !18
  call void @print_int(ptr @STR4, i32 %ADD8), !dbg !18
  call void @print_int(ptr @STR5, i32 70), !dbg !19
  %bar = call i32 @bar(), !dbg !20
  %ADD9 = add i32 50, %bar, !dbg !20
  call void @print_int(ptr @STR6, i32 %ADD9), !dbg !20
  %foo10 = call i32 @foo(), !dbg !21
  %a11 = load i32, ptr %a, align 4, !dbg !21
  %ADD12 = add i32 %foo10, %a11, !dbg !21
  call void @print_int(ptr @STR7, i32 %ADD12), !dbg !21
  %bar13 = call i32 @bar(), !dbg !22
  %ADD14 = add i32 %bar13, 100, !dbg !22
  call void @print_int(ptr @STR8, i32 %ADD14), !dbg !22
  %foo15 = call i32 @foo(), !dbg !23
  %bar16 = call i32 @bar(), !dbg !23
  %ADD17 = add i32 %foo15, %bar16, !dbg !23
  call void @print_int(ptr @STR9, i32 %ADD17), !dbg !23
  %a18 = load i32, ptr %a, align 4, !dbg !24
  %b19 = load i32, ptr %b, align 4, !dbg !24
  %SUB = sub i32 %a18, %b19, !dbg !24
  call void @print_int(ptr @STR10, i32 %SUB), !dbg !24
  %a20 = load i32, ptr %a, align 4, !dbg !25
  %SUB21 = sub i32 %a20, 5, !dbg !25
  call void @print_int(ptr @STR11, i32 %SUB21), !dbg !25
  %foo22 = call i32 @foo(), !dbg !26
  %a23 = load i32, ptr %a, align 4, !dbg !26
  %SUB24 = sub i32 %a23, %foo22, !dbg !26
  call void @print_int(ptr @STR12, i32 %SUB24), !dbg !26
  %b25 = load i32, ptr %b, align 4, !dbg !27
  %SUB26 = sub i32 30, %b25, !dbg !27
  call void @print_int(ptr @STR13, i32 %SUB26), !dbg !27
  call void @print_int(ptr @STR14, i32 35), !dbg !28
  %bar27 = call i32 @bar(), !dbg !29
  %SUB28 = sub i32 40, %bar27, !dbg !29
  call void @print_int(ptr @STR15, i32 %SUB28), !dbg !29
  %foo29 = call i32 @foo(), !dbg !30
  %b30 = load i32, ptr %b, align 4, !dbg !30
  %SUB31 = sub i32 %foo29, %b30, !dbg !30
  call void @print_int(ptr @STR16, i32 %SUB31), !dbg !30
  %bar32 = call i32 @bar(), !dbg !31
  %SUB33 = sub i32 %bar32, 3, !dbg !31
  call void @print_int(ptr @STR17, i32 %SUB33), !dbg !31
  %bar34 = call i32 @bar(), !dbg !32
  %foo35 = call i32 @foo(), !dbg !32
  %SUB36 = sub i32 %bar34, %foo35, !dbg !32
  call void @print_int(ptr @STR18, i32 %SUB36), !dbg !32
  %a37 = load i32, ptr %a, align 4, !dbg !33
  %b38 = load i32, ptr %b, align 4, !dbg !33
  %MUL = mul i32 %a37, %b38, !dbg !33
  call void @print_int(ptr @STR19, i32 %MUL), !dbg !33
  %a39 = load i32, ptr %a, align 4, !dbg !34
  %MUL40 = mul i32 %a39, 3, !dbg !34
  call void @print_int(ptr @STR20, i32 %MUL40), !dbg !34
  %foo41 = call i32 @foo(), !dbg !35
  %a42 = load i32, ptr %a, align 4, !dbg !35
  %MUL43 = mul i32 %a42, %foo41, !dbg !35
  call void @print_int(ptr @STR21, i32 %MUL43), !dbg !35
  %b44 = load i32, ptr %b, align 4, !dbg !36
  %MUL45 = mul i32 7, %b44, !dbg !36
  call void @print_int(ptr @STR22, i32 %MUL45), !dbg !36
  call void @print_int(ptr @STR23, i32 72), !dbg !37
  %bar46 = call i32 @bar(), !dbg !38
  %MUL47 = mul i32 2, %bar46, !dbg !38
  call void @print_int(ptr @STR24, i32 %MUL47), !dbg !38
  %foo48 = call i32 @foo(), !dbg !39
  %a49 = load i32, ptr %a, align 4, !dbg !39
  %MUL50 = mul i32 %foo48, %a49, !dbg !39
  call void @print_int(ptr @STR25, i32 %MUL50), !dbg !39
  %bar51 = call i32 @bar(), !dbg !40
  %MUL52 = mul i32 %bar51, 5, !dbg !40
  call void @print_int(ptr @STR26, i32 %MUL52), !dbg !40
  %foo53 = call i32 @foo(), !dbg !41
  %bar54 = call i32 @bar(), !dbg !41
  %MUL55 = mul i32 %foo53, %bar54, !dbg !41
  call void @print_int(ptr @STR27, i32 %MUL55), !dbg !41
  %a56 = load i32, ptr %a, align 4, !dbg !42
  %b57 = load i32, ptr %b, align 4, !dbg !42
  %DIV = sdiv i32 %a56, %b57, !dbg !42
  call void @print_int(ptr @STR28, i32 %DIV), !dbg !42
  %a58 = load i32, ptr %a, align 4, !dbg !43
  %DIV59 = sdiv i32 %a58, 2, !dbg !43
  call void @print_int(ptr @STR29, i32 %DIV59), !dbg !43
  %foo60 = call i32 @foo(), !dbg !44
  %a61 = load i32, ptr %a, align 4, !dbg !44
  %DIV62 = sdiv i32 %a61, %foo60, !dbg !44
  call void @print_int(ptr @STR30, i32 %DIV62), !dbg !44
  %b63 = load i32, ptr %b, align 4, !dbg !45
  %DIV64 = sdiv i32 100, %b63, !dbg !45
  call void @print_int(ptr @STR31, i32 %DIV64), !dbg !45
  call void @print_int(ptr @STR32, i32 5), !dbg !46
  %foo65 = call i32 @foo(), !dbg !47
  %DIV66 = sdiv i32 30, %foo65, !dbg !47
  call void @print_int(ptr @STR33, i32 %DIV66), !dbg !47
  %bar67 = call i32 @bar(), !dbg !48
  %b68 = load i32, ptr %b, align 4, !dbg !48
  %DIV69 = sdiv i32 %bar67, %b68, !dbg !48
  call void @print_int(ptr @STR34, i32 %DIV69), !dbg !48
  %bar70 = call i32 @bar(), !dbg !49
  %DIV71 = sdiv i32 %bar70, 2, !dbg !49
  call void @print_int(ptr @STR35, i32 %DIV71), !dbg !49
  %bar72 = call i32 @bar(), !dbg !50
  %foo73 = call i32 @foo(), !dbg !50
  %DIV74 = sdiv i32 %bar72, %foo73, !dbg !50
  call void @print_int(ptr @STR36, i32 %DIV74), !dbg !50
  %a75 = load i32, ptr %a, align 4, !dbg !51
  %b76 = load i32, ptr %b, align 4, !dbg !51
  %MOD = srem i32 %a75, %b76, !dbg !51
  call void @print_int(ptr @STR37, i32 %MOD), !dbg !51
  %a77 = load i32, ptr %a, align 4, !dbg !52
  %MOD78 = srem i32 %a77, 3, !dbg !52
  call void @print_int(ptr @STR38, i32 %MOD78), !dbg !52
  %foo79 = call i32 @foo(), !dbg !53
  %a80 = load i32, ptr %a, align 4, !dbg !53
  %MOD81 = srem i32 %a80, %foo79, !dbg !53
  call void @print_int(ptr @STR39, i32 %MOD81), !dbg !53
  %b82 = load i32, ptr %b, align 4, !dbg !54
  %MOD83 = srem i32 50, %b82, !dbg !54
  call void @print_int(ptr @STR40, i32 %MOD83), !dbg !54
  call void @print_int(ptr @STR41, i32 5), !dbg !55
  %foo84 = call i32 @foo(), !dbg !56
  %MOD85 = srem i32 33, %foo84, !dbg !56
  call void @print_int(ptr @STR42, i32 %MOD85), !dbg !56
  %bar86 = call i32 @bar(), !dbg !57
  %b87 = load i32, ptr %b, align 4, !dbg !57
  %MOD88 = srem i32 %bar86, %b87, !dbg !57
  call void @print_int(ptr @STR43, i32 %MOD88), !dbg !57
  %bar89 = call i32 @bar(), !dbg !58
  %MOD90 = srem i32 %bar89, 3, !dbg !58
  call void @print_int(ptr @STR44, i32 %MOD90), !dbg !58
  %bar91 = call i32 @bar(), !dbg !59
  %foo92 = call i32 @foo(), !dbg !59
  %MOD93 = srem i32 %bar91, %foo92, !dbg !59
  call void @print_int(ptr @STR45, i32 %MOD93), !dbg !59
  %b94 = load i32, ptr %b, align 4, !dbg !60
  %MUL95 = mul i32 %b94, 2, !dbg !60
  %a96 = load i32, ptr %a, align 4, !dbg !60
  %ADD97 = add i32 %a96, %MUL95, !dbg !60
  call void @print_int(ptr @STR46, i32 %ADD97), !dbg !60
  %a98 = load i32, ptr %a, align 4, !dbg !61
  %b99 = load i32, ptr %b, align 4, !dbg !61
  %ADD100 = add i32 %a98, %b99, !dbg !61
  %MUL101 = mul i32 %ADD100, 2, !dbg !61
  call void @print_int(ptr @STR47, i32 %MUL101), !dbg !61
  %a102 = load i32, ptr %a, align 4, !dbg !62
  %b103 = load i32, ptr %b, align 4, !dbg !62
  %SUB104 = sub i32 %a102, %b103, !dbg !62
  %foo105 = call i32 @foo(), !dbg !62
  %ADD106 = add i32 %SUB104, %foo105, !dbg !62
  call void @print_int(ptr @STR48, i32 %ADD106), !dbg !62
  %foo107 = call i32 @foo(), !dbg !63
  %bar108 = call i32 @bar(), !dbg !63
  %MUL109 = mul i32 %foo107, %bar108, !dbg !63
  %a110 = load i32, ptr %a, align 4, !dbg !63
  %ADD111 = add i32 %MUL109, %a110, !dbg !63
  %b112 = load i32, ptr %b, align 4, !dbg !63
  %SUB113 = sub i32 %ADD111, %b112, !dbg !63
  call void @print_int(ptr @STR49, i32 %SUB113), !dbg !63
  ret i32 0, !dbg !63
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "arithmetic.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/op")
!4 = distinct !DISubprogram(name: "foo", linkageName: "foo", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "bar", linkageName: "bar", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "print_int", linkageName: "print_int", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2)
!11 = !DILocation(line: 9, scope: !10)
!12 = !DILocation(line: 10, scope: !10)
!13 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2)
!14 = !DILocation(line: 12, scope: !13)
!15 = !DILocation(line: 17, scope: !13)
!16 = !DILocation(line: 18, scope: !13)
!17 = !DILocation(line: 19, scope: !13)
!18 = !DILocation(line: 20, scope: !13)
!19 = !DILocation(line: 21, scope: !13)
!20 = !DILocation(line: 22, scope: !13)
!21 = !DILocation(line: 23, scope: !13)
!22 = !DILocation(line: 24, scope: !13)
!23 = !DILocation(line: 25, scope: !13)
!24 = !DILocation(line: 28, scope: !13)
!25 = !DILocation(line: 29, scope: !13)
!26 = !DILocation(line: 30, scope: !13)
!27 = !DILocation(line: 31, scope: !13)
!28 = !DILocation(line: 32, scope: !13)
!29 = !DILocation(line: 33, scope: !13)
!30 = !DILocation(line: 34, scope: !13)
!31 = !DILocation(line: 35, scope: !13)
!32 = !DILocation(line: 36, scope: !13)
!33 = !DILocation(line: 39, scope: !13)
!34 = !DILocation(line: 40, scope: !13)
!35 = !DILocation(line: 41, scope: !13)
!36 = !DILocation(line: 42, scope: !13)
!37 = !DILocation(line: 43, scope: !13)
!38 = !DILocation(line: 44, scope: !13)
!39 = !DILocation(line: 45, scope: !13)
!40 = !DILocation(line: 46, scope: !13)
!41 = !DILocation(line: 47, scope: !13)
!42 = !DILocation(line: 50, scope: !13)
!43 = !DILocation(line: 51, scope: !13)
!44 = !DILocation(line: 52, scope: !13)
!45 = !DILocation(line: 53, scope: !13)
!46 = !DILocation(line: 54, scope: !13)
!47 = !DILocation(line: 55, scope: !13)
!48 = !DILocation(line: 56, scope: !13)
!49 = !DILocation(line: 57, scope: !13)
!50 = !DILocation(line: 58, scope: !13)
!51 = !DILocation(line: 61, scope: !13)
!52 = !DILocation(line: 62, scope: !13)
!53 = !DILocation(line: 63, scope: !13)
!54 = !DILocation(line: 64, scope: !13)
!55 = !DILocation(line: 65, scope: !13)
!56 = !DILocation(line: 66, scope: !13)
!57 = !DILocation(line: 67, scope: !13)
!58 = !DILocation(line: 68, scope: !13)
!59 = !DILocation(line: 69, scope: !13)
!60 = !DILocation(line: 72, scope: !13)
!61 = !DILocation(line: 73, scope: !13)
!62 = !DILocation(line: 74, scope: !13)
!63 = !DILocation(line: 75, scope: !13)
