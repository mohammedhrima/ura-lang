; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %while.start.0

while.start.0:                                    ; preds = %if.end.4, %entry
  %a1 = load i32, ptr %a, align 4
  %LESS = icmp slt i32 %a1, 10
  br i1 %LESS, label %while.then.1, label %while.end.2

while.then.1:                                     ; preds = %while.start.0
  %a2 = load i32, ptr %a, align 4
  %ADD = add i32 %a2, 1
  store i32 %ADD, ptr %a, align 4
  br label %if.start.3

while.end.2:                                      ; preds = %if.then.5, %while.start.0
  %a4 = load i32, ptr %a, align 4
  ret i32 %a4

if.start.3:                                       ; preds = %while.then.1
  %a3 = load i32, ptr %a, align 4
  %EQUAL = icmp eq i32 %a3, 2
  br i1 %EQUAL, label %if.then.5, label %if.end.4

if.end.4:                                         ; preds = %if.start.3
  br label %while.start.0

if.then.5:                                        ; preds = %if.start.3
  br label %while.end.2
}
