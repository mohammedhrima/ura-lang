; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %while.start.0

while.start.0:                                    ; preds = %while.then.1, %entry
  %a1 = load i32, ptr %a, align 4
  %LESS = icmp slt i32 %a1, 10
  br i1 %LESS, label %while.then.1, label %while.end.2

while.then.1:                                     ; preds = %while.start.0
  %a2 = load i32, ptr %a, align 4
  %ADD = add i32 %a2, 1
  store i32 %ADD, ptr %a, align 4
  br label %while.start.0

while.end.2:                                      ; preds = %while.start.0
  %a3 = load i32, ptr %a, align 4
  ret i32 %a3
}
