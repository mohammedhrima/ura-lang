; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abd\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"abc\00", align 1

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
  %GE = icmp sge i32 %n5, 97
  %n6 = load i32, ptr %n, align 4
  %LE = icmp sle i32 %n6, 122
  %AND = and i1 %GE, %LE
  %n7 = load i32, ptr %n, align 4
  %GE8 = icmp sge i32 %n7, 65
  %n9 = load i32, ptr %n, align 4
  %LE10 = icmp sle i32 %n9, 90
  %AND11 = and i1 %GE8, %LE10
  %OR12 = or i1 %AND, %AND11
  %n13 = load i32, ptr %n, align 4
  %GE14 = icmp sge i32 %n13, 48
  %n15 = load i32, ptr %n, align 4
  %LE16 = icmp sle i32 %n15, 57
  %AND17 = and i1 %GE14, %LE16
  %OR18 = or i1 %OR12, %AND17
  %n19 = load i32, ptr %n, align 4
  %EQ = icmp eq i32 %n19, 32
  %OR20 = or i1 %OR18, %EQ
  %n21 = load i32, ptr %n, align 4
  %EQ22 = icmp eq i32 %n21, 0
  %OR23 = or i1 %OR20, %EQ22
  br i1 %OR23, label %if24, label %end_if25

if24:                                             ; preds = %end_if
  %n26 = load i32, ptr %n, align 4
  ret i32 %n26
  br label %end_if25

end_if25:                                         ; preds = %if24, %end_if
  ret i32 -1
}

define i32 @strncmp(ptr %left, ptr %right, i32 %n) {
entry:
  %left1 = alloca ptr, align 8
  store ptr %left, ptr %left1, align 8
  %right2 = alloca ptr, align 8
  store ptr %right, ptr %right2, align 8
  %n3 = alloca i32, align 4
  store i32 %n, ptr %n3, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  %n4 = load i32, ptr %n3, align 4
  %EQ = icmp eq i32 %n4, 0
  br i1 %EQ, label %if, label %end_if

if:                                               ; preds = %entry
  ret i32 0
  br label %end_if

end_if:                                           ; preds = %if, %entry
  br label %while

while:                                            ; preds = %while_bloc, %end_if
  %i5 = load i32, ptr %i, align 4
  %n6 = load i32, ptr %n3, align 4
  %LESS = icmp slt i32 %i5, %n6
  %left7 = load ptr, ptr %left1, align 8
  %idx = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %left7, i32 %idx
  %right8 = load ptr, ptr %right2, align 8
  %idx9 = load i32, ptr %i, align 4
  %ACCESS10 = getelementptr i8, ptr %right8, i32 %idx9
  %left11 = load i8, ptr %ACCESS, align 1
  %right12 = load i8, ptr %ACCESS10, align 1
  %EQ13 = icmp eq i8 %left11, %right12
  %AND = and i1 %LESS, %EQ13
  %left14 = load ptr, ptr %left1, align 8
  %idx15 = load i32, ptr %i, align 4
  %ACCESS16 = getelementptr i8, ptr %left14, i32 %idx15
  %left17 = load i8, ptr %ACCESS16, align 1
  %NOT_EQUAL = icmp ne i8 %left17, 0
  %AND18 = and i1 %AND, %NOT_EQUAL
  br i1 %AND18, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i19 = load i32, ptr %i, align 4
  %ADD = add i32 %i19, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %i20 = load i32, ptr %i, align 4
  %n21 = load i32, ptr %n3, align 4
  %EQ22 = icmp eq i32 %i20, %n21
  br i1 %EQ22, label %if23, label %end_if24

if23:                                             ; preds = %end_while
  ret i32 0
  br label %end_if24

end_if24:                                         ; preds = %if23, %end_while
  %left_val = alloca i32, align 4
  %left25 = load ptr, ptr %left1, align 8
  %idx26 = load i32, ptr %i, align 4
  %ACCESS27 = getelementptr i8, ptr %left25, i32 %idx26
  %left28 = load i8, ptr %ACCESS27, align 1
  %to_ascii = call i32 @to_ascii(i8 %left28)
  store i32 %to_ascii, ptr %left_val, align 4
  %right_val = alloca i32, align 4
  %right29 = load ptr, ptr %right2, align 8
  %idx30 = load i32, ptr %i, align 4
  %ACCESS31 = getelementptr i8, ptr %right29, i32 %idx30
  %right32 = load i8, ptr %ACCESS31, align 1
  %to_ascii33 = call i32 @to_ascii(i8 %right32)
  store i32 %to_ascii33, ptr %right_val, align 4
  %left_val34 = load i32, ptr %left_val, align 4
  %right_val35 = load i32, ptr %right_val, align 4
  %SUB = sub i32 %left_val34, %right_val35
  ret i32 %SUB
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strncmp = call i32 @strncmp(ptr @STR0, ptr @STR1, i32 4)
  store i32 %strncmp, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
