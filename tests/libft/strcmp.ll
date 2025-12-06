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
  %MO_EQ = icmp sge i32 %n5, 97
  %n6 = load i32, ptr %n, align 4
  %LE_EQ = icmp sle i32 %n6, 122
  %AND = and i1 %MO_EQ, %LE_EQ
  %n7 = load i32, ptr %n, align 4
  %MO_EQ8 = icmp sge i32 %n7, 65
  %n9 = load i32, ptr %n, align 4
  %LE_EQ10 = icmp sle i32 %n9, 90
  %AND11 = and i1 %MO_EQ8, %LE_EQ10
  %OR12 = or i1 %AND, %AND11
  %n13 = load i32, ptr %n, align 4
  %MO_EQ14 = icmp sge i32 %n13, 48
  %n15 = load i32, ptr %n, align 4
  %LE_EQ16 = icmp sle i32 %n15, 57
  %AND17 = and i1 %MO_EQ14, %LE_EQ16
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
  %i4 = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %left3, i32 %i4
  %right5 = load ptr, ptr %right2, align 8
  %i6 = load i32, ptr %i, align 4
  %ACCESS7 = getelementptr i8, ptr %right5, i32 %i6
  %left8 = load i8, ptr %ACCESS, align 1
  %right9 = load i8, ptr %ACCESS7, align 1
  %EQUAL = icmp eq i8 %left8, %right9
  %left10 = load ptr, ptr %left1, align 8
  %i11 = load i32, ptr %i, align 4
  %ACCESS12 = getelementptr i8, ptr %left10, i32 %i11
  %left13 = load i8, ptr %ACCESS12, align 1
  %NOT_EQ = icmp ne i8 %left13, 0
  %AND = and i1 %EQUAL, %NOT_EQ
  br i1 %AND, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i14 = load i32, ptr %i, align 4
  %ADD = add i32 %i14, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %left15 = load ptr, ptr %left1, align 8
  %i16 = load i32, ptr %i, align 4
  %ACCESS17 = getelementptr i8, ptr %left15, i32 %i16
  %left18 = load i8, ptr %ACCESS17, align 1
  %to_ascii = call i32 @to_ascii(i8 %left18)
  %right19 = load ptr, ptr %right2, align 8
  %i20 = load i32, ptr %i, align 4
  %ACCESS21 = getelementptr i8, ptr %right19, i32 %i20
  %right22 = load i8, ptr %ACCESS21, align 1
  %to_ascii23 = call i32 @to_ascii(i8 %right22)
  %SUB = sub i32 %to_ascii, %to_ascii23
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
