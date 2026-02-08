; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %while.start

while.start:                                      ; preds = %while.then, %entry
  %a1 = load i32, ptr %a, align 4
  %LESS = icmp slt i32 %a1, 10
  br i1 %LESS, label %while.then, label %while.end

while.then:                                       ; preds = %while.start
  %a2 = load i32, ptr %a, align 4
  %ADD = add i32 %a2, 1
  store i32 %ADD, ptr %a, align 4
  br label %while.start

while.end:                                        ; preds = %while.start
  %a3 = load i32, ptr %a, align 4
  ret i32 %a3
}
