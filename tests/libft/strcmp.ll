; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"a\00", align 1

define i32 @strcmp(ptr %left, ptr %right) {
entry:
  %left1 = alloca ptr, align 8
  store ptr %left, ptr %left1, align 8
  %right2 = alloca ptr, align 8
  store ptr %right, ptr %right2, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.start

while.start:                                      ; preds = %while.then, %entry
  %left3 = load ptr, ptr %left1, align 8
  %i4 = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %left3, i32 %i4
  %right5 = load ptr, ptr %right2, align 8
  %i6 = load i32, ptr %i, align 4
  %ACCESS7 = getelementptr i8, ptr %right5, i32 %i6
  %left8 = load i8, ptr %ACCESS, align 1
  %right9 = load i8, ptr %ACCESS7, align 1
  %EQ = icmp eq i8 %left8, %right9
  %left10 = load ptr, ptr %left1, align 8
  %i11 = load i32, ptr %i, align 4
  %ACCESS12 = getelementptr i8, ptr %left10, i32 %i11
  %left13 = load i8, ptr %ACCESS12, align 1
  %NEQ = icmp ne i8 %left13, 0
  %AND = and i1 %EQ, %NEQ
  br i1 %AND, label %while.then, label %while.end

while.then:                                       ; preds = %while.start
  %current = load i32, ptr %i, align 4
  %ADD = add i32 %current, 1
  store i32 %ADD, ptr %i, align 4
  br label %while.start

while.end:                                        ; preds = %while.start
  %left14 = load ptr, ptr %left1, align 8
  %i15 = load i32, ptr %i, align 4
  %ACCESS16 = getelementptr i8, ptr %left14, i32 %i15
  %left17 = load i8, ptr %ACCESS16, align 1
  %as = sext i8 %left17 to i32
  %right18 = load ptr, ptr %right2, align 8
  %i19 = load i32, ptr %i, align 4
  %ACCESS20 = getelementptr i8, ptr %right18, i32 %i19
  %right21 = load i8, ptr %ACCESS20, align 1
  %as22 = sext i8 %right21 to i32
  %SUB = sub i32 %as, %as22
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
