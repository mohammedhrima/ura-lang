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
  br label %if.start

if.start:                                         ; preds = %entry
  %n2 = load i32, ptr %n1, align 4
  %LT = icmp slt i32 %n2, 0
  br i1 %LT, label %if.then, label %if.end

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start4

if.then:                                          ; preds = %if.start
  %putchar = call i32 @putchar(i8 45)
  %n3 = load i32, ptr %n1, align 4
  %MUL = mul i32 %n3, -1
  store i32 %MUL, ptr %n1, align 4
  br label %if.end

if.start4:                                        ; preds = %if.end
  %n7 = load i32, ptr %n1, align 4
  %EQ = icmp eq i32 %n7, 0
  br i1 %EQ, label %if.then6, label %if.end5

if.end5:                                          ; preds = %if.start4
  %digits = alloca ptr, align 8
  store ptr @STR0, ptr %digits, align 8
  %temp = alloca i32, align 4
  %n9 = load i32, ptr %n1, align 4
  store i32 %n9, ptr %temp, align 4
  %digit_count = alloca i32, align 4
  store i32 0, ptr %digit_count, align 4
  br label %while.start

if.then6:                                         ; preds = %if.start4
  %putchar8 = call i32 @putchar(i8 48)
  ret void

while.start:                                      ; preds = %while.then, %if.end5
  %temp10 = load i32, ptr %temp, align 4
  %GT = icmp sgt i32 %temp10, 0
  br i1 %GT, label %while.then, label %while.end

while.then:                                       ; preds = %while.start
  %current = load i32, ptr %digit_count, align 4
  %ADD = add i32 %current, 1
  store i32 %ADD, ptr %digit_count, align 4
  %temp11 = load i32, ptr %temp, align 4
  %DIV = sdiv i32 %temp11, 10
  store i32 %DIV, ptr %temp, align 4
  br label %while.start

while.end:                                        ; preds = %while.start
  %buffer = alloca ptr, align 8
  %digit_count12 = load i32, ptr %digit_count, align 4
  %ADD13 = add i32 %digit_count12, 1
  %calloc = call ptr @calloc(i32 %ADD13, i32 1)
  store ptr %calloc, ptr %buffer, align 8
  %i = alloca i32, align 4
  %digit_count14 = load i32, ptr %digit_count, align 4
  %SUB = sub i32 %digit_count14, 1
  store i32 %SUB, ptr %i, align 4
  br label %while.start15

while.start15:                                    ; preds = %while.then16, %while.end
  %i18 = load i32, ptr %i, align 4
  %GE = icmp sge i32 %i18, 0
  br i1 %GE, label %while.then16, label %while.end17

while.then16:                                     ; preds = %while.start15
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
  %current28 = load i32, ptr %i, align 4
  %SUB29 = sub i32 %current28, 1
  store i32 %SUB29, ptr %i, align 4
  br label %while.start15

while.end17:                                      ; preds = %while.start15
  store i32 0, ptr %i, align 4
  br label %while.start30

while.start30:                                    ; preds = %while.then31, %while.end17
  %i33 = load i32, ptr %i, align 4
  %digit_count34 = load i32, ptr %digit_count, align 4
  %LT35 = icmp slt i32 %i33, %digit_count34
  br i1 %LT35, label %while.then31, label %while.end32

while.then31:                                     ; preds = %while.start30
  %buffer36 = load ptr, ptr %buffer, align 8
  %i37 = load i32, ptr %i, align 4
  %ACCESS38 = getelementptr i8, ptr %buffer36, i32 %i37
  %buffer39 = load i8, ptr %ACCESS38, align 1
  %putchar40 = call i32 @putchar(i8 %buffer39)
  %current41 = load i32, ptr %i, align 4
  %ADD42 = add i32 %current41, 1
  store i32 %ADD42, ptr %i, align 4
  br label %while.start30

while.end32:                                      ; preds = %while.start30
  %buffer43 = load ptr, ptr %buffer, align 8
  %free = call i32 @free(ptr %buffer43)
  ret void
}

define i32 @main() {
entry:
  call void @putnbr(i32 123)
  %putchar = call i32 @putchar(i8 10)
  call void @putnbr(i32 -456)
  %putchar1 = call i32 @putchar(i8 10)
  call void @putnbr(i32 0)
  %putchar2 = call i32 @putchar(i8 10)
  ret i32 0
}
