; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

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
  %EQUAL = icmp eq i32 %n4, 0
  br i1 %EQUAL, label %if5, label %end_if6

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
  %MO_EQ = icmp sge i32 %i18, 0
  br i1 %MO_EQ, label %while_bloc16, label %end_while17

while_bloc16:                                     ; preds = %while15
  %digit = alloca i32, align 4
  %n19 = load i32, ptr %n1, align 4
  %MOD = srem i32 %n19, 10
  store i32 %MOD, ptr %digit, align 4
  %buffer20 = load ptr, ptr %buffer, align 8
  %i21 = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %buffer20, i32 %i21
  %digits22 = load ptr, ptr %digits, align 8
  %digit23 = load i32, ptr %digit, align 4
  %ACCESS24 = getelementptr i8, ptr %digits22, i32 %digit23
  %digits25 = load i8, ptr %ACCESS24, align 1
  store i8 %digits25, ptr %ACCESS, align 1
  %n26 = load i32, ptr %n1, align 4
  %DIV27 = sdiv i32 %n26, 10
  store i32 %DIV27, ptr %n1, align 4
  %i28 = load i32, ptr %i, align 4
  %SUB29 = sub i32 %i28, 1
  store i32 %SUB29, ptr %i, align 4
  br label %while15

end_while17:                                      ; preds = %while15
  store i32 0, ptr %i, align 4
  br label %while30

while30:                                          ; preds = %while_bloc31, %end_while17
  %i33 = load i32, ptr %i, align 4
  %digit_count34 = load i32, ptr %digit_count, align 4
  %LESS35 = icmp slt i32 %i33, %digit_count34
  br i1 %LESS35, label %while_bloc31, label %end_while32

while_bloc31:                                     ; preds = %while30
  %buffer36 = load ptr, ptr %buffer, align 8
  %i37 = load i32, ptr %i, align 4
  %ACCESS38 = getelementptr i8, ptr %buffer36, i32 %i37
  %buffer39 = load i8, ptr %ACCESS38, align 1
  %putchar40 = call i32 @putchar(i8 %buffer39)
  %i41 = load i32, ptr %i, align 4
  %ADD42 = add i32 %i41, 1
  store i32 %ADD42, ptr %i, align 4
  br label %while30

end_while32:                                      ; preds = %while30
  %buffer43 = load ptr, ptr %buffer, align 8
  %free = call i32 @free(ptr %buffer43)
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
