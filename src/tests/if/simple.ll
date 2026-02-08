; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %if.start.0

if.start.0:                                       ; preds = %entry
  %a1 = load i32, ptr %a, align 4
  %EQUAL = icmp eq i32 %a1, 1
  br i1 %EQUAL, label %if.then.2, label %if.end.1

if.end.1:                                         ; preds = %if.start.0
  ret i32 0

if.then.2:                                        ; preds = %if.start.0
  ret i32 5
}
