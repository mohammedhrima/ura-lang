; ModuleID = 'moduleName'
source_filename = "moduleName"

@STR = private constant [4 x i8] c"abd\00"
@STR.1 = private constant [4 x i8] c"abc\00"

define i32 @to_ascii(i8 %c) {
entry:
  %n = alloca i32, align 4
  store i8 %c, ptr %n, align 1
  %n1 = load i32, ptr %n, align 4
  %LESS = icmp slt i32 %n1, 0
  %n2 = load i32, ptr %n, align 4
  %MORE = icmp sgt i32 %n2, 127
  %OR = or i1 %LESS, %MORE
  br i1 %OR, label %if, label %end_if

if:                                               ; preds = %entry
  ret i32 -1
  br label %end_if

end_if:                                           ; preds = %if, %entry
  %n3 = load i32, ptr %n, align 4
  %MO_EQ = icmp sge i32 %n3, 97
  %n4 = load i32, ptr %n, align 4
  %LE_EQ = icmp sle i32 %n4, 122
  %AND = and i1 %MO_EQ, %LE_EQ
  %n5 = load i32, ptr %n, align 4
  %MO_EQ6 = icmp sge i32 %n5, 65
  %n7 = load i32, ptr %n, align 4
  %LE_EQ8 = icmp sle i32 %n7, 90
  %AND9 = and i1 %MO_EQ6, %LE_EQ8
  %OR10 = or i1 %AND, %AND9
  %n11 = load i32, ptr %n, align 4
  %MO_EQ12 = icmp sge i32 %n11, 48
  %n13 = load i32, ptr %n, align 4
  %LE_EQ14 = icmp sle i32 %n13, 57
  %AND15 = and i1 %MO_EQ12, %LE_EQ14
  %OR16 = or i1 %OR10, %AND15
  %n17 = load i32, ptr %n, align 4
  %EQUAL = icmp eq i32 %n17, 32
  %OR18 = or i1 %OR16, %EQUAL
  %n19 = load i32, ptr %n, align 4
  %EQUAL20 = icmp eq i32 %n19, 0
  %OR21 = or i1 %OR18, %EQUAL20
  br i1 %OR21, label %if22, label %end_if23

if22:                                             ; preds = %end_if
  %n24 = load i32, ptr %n, align 4
  ret i32 %n24
  br label %end_if23

end_if23:                                         ; preds = %if22, %end_if
  ret i32 -1
}

define i32 @strncmp(ptr %left, ptr %right, i32 %n) {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  %EQUAL = icmp eq i32 %n, 0
  br i1 %EQUAL, label %if, label %end_if

if:                                               ; preds = %entry
  ret i32 0
  br label %end_if

end_if:                                           ; preds = %if, %entry
  br label %while

while:                                            ; preds = %while_bloc, %end_if
  %i1 = load i32, ptr %i, align 4
  %LESS = icmp slt i32 %i1, %n
  %i2 = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %left, i32 %i2
  %i3 = load i32, ptr %i, align 4
  %ACCESS4 = getelementptr i8, ptr %right, i32 %i3
  %left5 = load i8, ptr %ACCESS, align 1
  %right6 = load i8, ptr %ACCESS4, align 1
  %EQUAL7 = icmp eq i8 %left5, %right6
  %AND = and i1 %LESS, %EQUAL7
  %i8 = load i32, ptr %i, align 4
  %ACCESS9 = getelementptr i8, ptr %left, i32 %i8
  %left10 = load i8, ptr %ACCESS9, align 1
  %NOT_EQ = icmp ne i8 %left10, 0
  %AND11 = and i1 %AND, %NOT_EQ
  br i1 %AND11, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i12 = load i32, ptr %i, align 4
  %ADD = add i32 %i12, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %i13 = load i32, ptr %i, align 4
  %EQUAL14 = icmp eq i32 %i13, %n
  br i1 %EQUAL14, label %if15, label %end_if16

if15:                                             ; preds = %end_while
  ret i32 0
  br label %end_if16

end_if16:                                         ; preds = %if15, %end_while
  %left_val = alloca i32, align 4
  %i17 = load i32, ptr %i, align 4
  %ACCESS18 = getelementptr i8, ptr %left, i32 %i17
  %left19 = load i8, ptr %ACCESS18, align 1
  %to_ascii = call i32 @to_ascii(i8 %left19)
  store i32 %to_ascii, ptr %left_val, align 4
  %right_val = alloca i32, align 4
  %i20 = load i32, ptr %i, align 4
  %ACCESS21 = getelementptr i8, ptr %right, i32 %i20
  %right22 = load i8, ptr %ACCESS21, align 1
  %to_ascii23 = call i32 @to_ascii(i8 %right22)
  store i32 %to_ascii23, ptr %right_val, align 4
  %left_val24 = load i32, ptr %left_val, align 4
  %right_val25 = load i32, ptr %right_val, align 4
  %SUB = sub i32 %left_val24, %right_val25
  ret i32 %SUB
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strncmp = call i32 @strncmp(ptr @STR, ptr @STR.1, i32 2)
  store i32 %strncmp, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
