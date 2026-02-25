; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abd\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"abc\00", align 1

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
  br label %if.start

if.start:                                         ; preds = %entry
  %n4 = load i32, ptr %n3, align 4
  %EQ = icmp eq i32 %n4, 0
  br i1 %EQ, label %if.then, label %if.end

if.end:                                           ; preds = %if.start
  br label %while.start

if.then:                                          ; preds = %if.start
  ret i32 0

while.start:                                      ; preds = %while.then, %if.end
  %i5 = load i32, ptr %i, align 4
  %n6 = load i32, ptr %n3, align 4
  %LT = icmp slt i32 %i5, %n6
  %left7 = load ptr, ptr %left1, align 8
  %i8 = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %left7, i32 %i8
  %right9 = load ptr, ptr %right2, align 8
  %i10 = load i32, ptr %i, align 4
  %ACCESS11 = getelementptr i8, ptr %right9, i32 %i10
  %left12 = load i8, ptr %ACCESS, align 1
  %right13 = load i8, ptr %ACCESS11, align 1
  %EQ14 = icmp eq i8 %left12, %right13
  %AND = and i1 %LT, %EQ14
  %left15 = load ptr, ptr %left1, align 8
  %i16 = load i32, ptr %i, align 4
  %ACCESS17 = getelementptr i8, ptr %left15, i32 %i16
  %left18 = load i8, ptr %ACCESS17, align 1
  %NEQ = icmp ne i8 %left18, 0
  %AND19 = and i1 %AND, %NEQ
  br i1 %AND19, label %while.then, label %while.end

while.then:                                       ; preds = %while.start
  %current = load i32, ptr %i, align 4
  %ADD = add i32 %current, 1
  store i32 %ADD, ptr %i, align 4
  br label %while.start

while.end:                                        ; preds = %while.start
  br label %if.start20

if.start20:                                       ; preds = %while.end
  %i23 = load i32, ptr %i, align 4
  %n24 = load i32, ptr %n3, align 4
  %EQ25 = icmp eq i32 %i23, %n24
  br i1 %EQ25, label %if.then22, label %if.end21

if.end21:                                         ; preds = %if.start20
  %left26 = load ptr, ptr %left1, align 8
  %i27 = load i32, ptr %i, align 4
  %ACCESS28 = getelementptr i8, ptr %left26, i32 %i27
  %left29 = load i8, ptr %ACCESS28, align 1
  %as = sext i8 %left29 to i32
  %right30 = load ptr, ptr %right2, align 8
  %i31 = load i32, ptr %i, align 4
  %ACCESS32 = getelementptr i8, ptr %right30, i32 %i31
  %right33 = load i8, ptr %ACCESS32, align 1
  %as34 = sext i8 %right33 to i32
  %SUB = sub i32 %as, %as34
  ret i32 %SUB

if.then22:                                        ; preds = %if.start20
  ret i32 0
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strncmp = call i32 @strncmp(ptr @STR0, ptr @STR1, i32 4)
  store i32 %strncmp, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
