; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abc\00", align 1

define i32 @strlen(ptr %str) {
entry:
  %str1 = alloca ptr, align 8
  store ptr %str, ptr %str1, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.start

while.start:                                      ; preds = %while.then, %entry
  %str2 = load ptr, ptr %str1, align 8
  %i3 = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %str2, i32 %i3
  %str4 = load i8, ptr %ACCESS, align 1
  %NEQ = icmp ne i8 %str4, 0
  br i1 %NEQ, label %while.then, label %while.end

while.then:                                       ; preds = %while.start
  %current = load i32, ptr %i, align 4
  %ADD = add i32 %current, 1
  store i32 %ADD, ptr %i, align 4
  br label %while.start

while.end:                                        ; preds = %while.start
  %i5 = load i32, ptr %i, align 4
  ret i32 %i5
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strlen = call i32 @strlen(ptr @STR0)
  store i32 %strlen, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
