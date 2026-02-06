; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@STR0 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"a\00", align 1

define i32 @to_ascii(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %n = alloca i32, align 4
  %c2 = load i8, ptr %c1, align 1
  %cast = sext i8 %c2 to i32
  store i32 %cast, ptr %n, align 4
  %n3 = load i32, ptr %n, align 4
  %LESS = icmp slt i32 %n3, 0
  %n4 = load i32, ptr %n, align 4
  %MORE = icmp sgt i32 %n4, 127
  %OR = or i1 %LESS, %MORE
  br i1 %OR, label %if, label %end_if

if:                                               ; preds = %entry
  ret i32 -1
  br label %end_if

end_if:                                           ; preds = %if, %entry
  %n5 = load i32, ptr %n, align 4
  %MO_EQUAL = icmp sge i32 %n5, 97
  %n6 = load i32, ptr %n, align 4
  %LE_EQUAL = icmp sle i32 %n6, 122
  %AND = and i1 %MO_EQUAL, %LE_EQUAL
  %n7 = load i32, ptr %n, align 4
  %MO_EQUAL8 = icmp sge i32 %n7, 65
  %n9 = load i32, ptr %n, align 4
  %LE_EQUAL10 = icmp sle i32 %n9, 90
  %AND11 = and i1 %MO_EQUAL8, %LE_EQUAL10
  %OR12 = or i1 %AND, %AND11
  %n13 = load i32, ptr %n, align 4
  %MO_EQUAL14 = icmp sge i32 %n13, 48
  %n15 = load i32, ptr %n, align 4
  %LE_EQUAL16 = icmp sle i32 %n15, 57
  %AND17 = and i1 %MO_EQUAL14, %LE_EQUAL16
  %OR18 = or i1 %OR12, %AND17
  %n19 = load i32, ptr %n, align 4
  %EQUAL = icmp eq i32 %n19, 32
  %OR20 = or i1 %OR18, %EQUAL
  %n21 = load i32, ptr %n, align 4
  %EQUAL22 = icmp eq i32 %n21, 0
  %OR23 = or i1 %OR20, %EQUAL22
  br i1 %OR23, label %if24, label %end_if25

if24:                                             ; preds = %end_if
  %n26 = load i32, ptr %n, align 4
  ret i32 %n26
  br label %end_if25

end_if25:                                         ; preds = %if24, %end_if
  ret i32 -1
}

define i32 @strcmp(ptr %left, ptr %right) {
entry:
  %left1 = alloca ptr, align 8
  store ptr %left, ptr %left1, align 8
  %right2 = alloca ptr, align 8
  store ptr %right, ptr %right2, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while

while:                                            ; preds = %while_bloc, %entry
  %left3 = load ptr, ptr %left1, align 8
  %idx = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %left3, i32 %idx
  %right4 = load ptr, ptr %right2, align 8
  %idx5 = load i32, ptr %i, align 4
  %ACCESS6 = getelementptr i8, ptr %right4, i32 %idx5
  %left7 = load i8, ptr %ACCESS, align 1
  %right8 = load i8, ptr %ACCESS6, align 1
  %EQUAL = icmp eq i8 %left7, %right8
  %left9 = load ptr, ptr %left1, align 8
  %idx10 = load i32, ptr %i, align 4
  %ACCESS11 = getelementptr i8, ptr %left9, i32 %idx10
  %left12 = load i8, ptr %ACCESS11, align 1
  %NOT_EQUAL = icmp ne i8 %left12, 0
  %AND = and i1 %EQUAL, %NOT_EQUAL
  br i1 %AND, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i13 = load i32, ptr %i, align 4
  %ADD = add i32 %i13, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %left14 = load ptr, ptr %left1, align 8
  %idx15 = load i32, ptr %i, align 4
  %ACCESS16 = getelementptr i8, ptr %left14, i32 %idx15
  %left17 = load i8, ptr %ACCESS16, align 1
  %to_ascii = call i32 @to_ascii(i8 %left17)
  %right18 = load ptr, ptr %right2, align 8
  %idx19 = load i32, ptr %i, align 4
  %ACCESS20 = getelementptr i8, ptr %right18, i32 %idx19
  %right21 = load i8, ptr %ACCESS20, align 1
  %to_ascii22 = call i32 @to_ascii(i8 %right21)
  %SUB = sub i32 %to_ascii, %to_ascii22
  ret i32 %SUB
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strcmp = call i32 @strcmp(ptr @STR0, ptr @STR1)
  store i32 %strcmp, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
