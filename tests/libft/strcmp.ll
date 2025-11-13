; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@str_literal = private constant [2 x i8] c"b\00"
@str_literal.1 = private constant [2 x i8] c"a\00"

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

define i32 @strcmp(ptr %left, ptr %right) {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while

while:                                            ; preds = %while_bloc, %entry
  %i1 = load i32, ptr %i, align 4
  %access = getelementptr i8, ptr %left, i32 %i1
  %access_val = load i8, ptr %access, align 1
  %i2 = load i32, ptr %i, align 4
  %access3 = getelementptr i8, ptr %right, i32 %i2
  %access_val4 = load i8, ptr %access3, align 1
  %EQUAL = icmp eq i8 %access_val, %access_val4
  %i5 = load i32, ptr %i, align 4
  %access6 = getelementptr i8, ptr %left, i32 %i5
  %access_val7 = load i8, ptr %access6, align 1
  %NOT_EQ = icmp ne i8 %access_val7, 0
  %AND = and i1 %EQUAL, %NOT_EQ
  br i1 %AND, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i8 = load i32, ptr %i, align 4
  %ADD = add i32 %i8, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %i9 = load i32, ptr %i, align 4
  %access10 = getelementptr i8, ptr %left, i32 %i9
  %access_val11 = load i8, ptr %access10, align 1
  %to_ascii = call i32 @to_ascii(i8 %access_val11)
  %i12 = load i32, ptr %i, align 4
  %access13 = getelementptr i8, ptr %right, i32 %i12
  %access_val14 = load i8, ptr %access13, align 1
  %to_ascii15 = call i32 @to_ascii(i8 %access_val14)
  %SUB = sub i32 %to_ascii, %to_ascii15
  ret i32 %SUB
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strcmp = call i32 @strcmp(ptr @str_literal, ptr @str_literal.1)
  store i32 %strcmp, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
