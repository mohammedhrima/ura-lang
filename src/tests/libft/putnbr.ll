; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [11 x i8] c"0123456789\00", align 1

declare ptr @calloc(i32, i32)

declare i32 @write(i32, ptr, i32)

declare i32 @free(ptr)

define i32 @putchar(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %str = alloca ptr, align 8
  %calloc = call ptr @calloc(i32 2, i32 1)
  store ptr %calloc, ptr %str, align 8
  %str2 = load ptr, ptr %str, align 8
  %ACCESS = getelementptr i8, ptr %str2, i32 0
  %c3 = load i8, ptr %c1, align 1
  store i8 %c3, ptr %ACCESS, align 1
  %str4 = load ptr, ptr %str, align 8
  %write = call i32 @write(i32 1, ptr %str4, i32 1)
  %str5 = load ptr, ptr %str, align 8
  %free = call i32 @free(ptr %str5)
  ret i32 0
}

define void @putnbr(i32 %n) {
entry:
  %n1 = alloca i32, align 4
  store i32 %n, ptr %n1, align 4
  %n2 = load i32, ptr %n1, align 4
  %LESS = icmp slt i32 %n2, 0
  br i1 %LESS, label %if, label %end_if

if:                                               ; preds = %entry
  %putchar = call i32 @putchar(i8 45)
  %n3 = load i32, ptr %n1, align 4
  %MUL = mul i32 %n3, -1
  store i32 %MUL, ptr %n1, align 4
  br label %end_if

end_if:                                           ; preds = %if, %entry
  %n4 = load i32, ptr %n1, align 4
  %EQ = icmp eq i32 %n4, 0
  br i1 %EQ, label %if5, label %end_if6

if5:                                              ; preds = %end_if
  %putchar7 = call i32 @putchar(i8 48)
  ret void
  br label %end_if6

end_if6:                                          ; preds = %if5, %end_if
  %digits = alloca ptr, align 8
  store ptr @STR0, ptr %digits, align 8
  %temp = alloca i32, align 4
  %n8 = load i32, ptr %n1, align 4
  store i32 %n8, ptr %temp, align 4
  %digit_count = alloca i32, align 4
  store i32 0, ptr %digit_count, align 4
  br label %while

while:                                            ; preds = %while_bloc, %end_if6
  %temp9 = load i32, ptr %temp, align 4
  %MORE = icmp sgt i32 %temp9, 0
  br i1 %MORE, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %digit_count10 = load i32, ptr %digit_count, align 4
  %ADD = add i32 %digit_count10, 1
  store i32 %ADD, ptr %digit_count, align 4
  %temp11 = load i32, ptr %temp, align 4
  %DIV = sdiv i32 %temp11, 10
  store i32 %DIV, ptr %temp, align 4
  br label %while

end_while:                                        ; preds = %while
  %buffer = alloca ptr, align 8
  %digit_count12 = load i32, ptr %digit_count, align 4
  %ADD13 = add i32 %digit_count12, 1
  %calloc = call ptr @calloc(i32 %ADD13, i32 1)
  store ptr %calloc, ptr %buffer, align 8
  %i = alloca i32, align 4
  %digit_count14 = load i32, ptr %digit_count, align 4
  %SUB = sub i32 %digit_count14, 1
  store i32 %SUB, ptr %i, align 4
  br label %while15

while15:                                          ; preds = %while_bloc16, %end_while
  %i18 = load i32, ptr %i, align 4
  %GE = icmp sge i32 %i18, 0
  br i1 %GE, label %while_bloc16, label %end_while17

while_bloc16:                                     ; preds = %while15
  %digit = alloca i32, align 4
  %n19 = load i32, ptr %n1, align 4
  %MOD = srem i32 %n19, 10
  store i32 %MOD, ptr %digit, align 4
  %buffer20 = load ptr, ptr %buffer, align 8
  %idx = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %buffer20, i32 %idx
  %digits21 = load ptr, ptr %digits, align 8
  %idx22 = load i32, ptr %digit, align 4
  %ACCESS23 = getelementptr i8, ptr %digits21, i32 %idx22
  %digits24 = load i8, ptr %ACCESS23, align 1
  store i8 %digits24, ptr %ACCESS, align 1
  %n25 = load i32, ptr %n1, align 4
  %DIV26 = sdiv i32 %n25, 10
  store i32 %DIV26, ptr %n1, align 4
  %i27 = load i32, ptr %i, align 4
  %SUB28 = sub i32 %i27, 1
  store i32 %SUB28, ptr %i, align 4
  br label %while15

end_while17:                                      ; preds = %while15
  store i32 0, ptr %i, align 4
  br label %while29

while29:                                          ; preds = %while_bloc30, %end_while17
  %i32 = load i32, ptr %i, align 4
  %digit_count33 = load i32, ptr %digit_count, align 4
  %LESS34 = icmp slt i32 %i32, %digit_count33
  br i1 %LESS34, label %while_bloc30, label %end_while31

while_bloc30:                                     ; preds = %while29
  %buffer35 = load ptr, ptr %buffer, align 8
  %idx36 = load i32, ptr %i, align 4
  %ACCESS37 = getelementptr i8, ptr %buffer35, i32 %idx36
  %buffer38 = load i8, ptr %ACCESS37, align 1
  %putchar39 = call i32 @putchar(i8 %buffer38)
  %i40 = load i32, ptr %i, align 4
  %ADD41 = add i32 %i40, 1
  store i32 %ADD41, ptr %i, align 4
  br label %while29

end_while31:                                      ; preds = %while29
  %buffer42 = load ptr, ptr %buffer, align 8
  %free = call i32 @free(ptr %buffer42)
  ret void
  ret void
}

define i32 @main() {
entry:
  call void @putnbr(i32 123)
  %putchar = call i32 @putchar(i8 10)
  call void @putnbr(i32 -456)
  %putchar1 = call i32 @putchar(i8 10)
  call void @putnbr(i32 0)
  ret i32 0
}
