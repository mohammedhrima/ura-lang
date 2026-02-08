; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %while.start

while.start:                                      ; preds = %if.end, %entry
  %a1 = load i32, ptr %a, align 4
  %LESS = icmp slt i32 %a1, 10
  br i1 %LESS, label %while.then, label %while.end

while.then:                                       ; preds = %while.start
  %a2 = load i32, ptr %a, align 4
  %ADD = add i32 %a2, 1
  store i32 %ADD, ptr %a, align 4
  br label %if.start

while.end:                                        ; preds = %if.then, %while.start
  %a4 = load i32, ptr %a, align 4
  ret i32 %a4

if.start:                                         ; preds = %while.then
  %a3 = load i32, ptr %a, align 4
  %EQUAL = icmp eq i32 %a3, 2
  br i1 %EQUAL, label %if.then, label %if.end

if.end:                                           ; preds = %if.start
  br label %while.start

if.then:                                          ; preds = %if.start
  br label %while.end
}
