; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i1 @check() {
entry:
  ret i1 true
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %if.start.0

if.start.0:                                       ; preds = %entry
  %check = call i1 @check()
  br i1 %check, label %if.then.2, label %elif.start.3

if.end.1:                                         ; No predecessors!
  ret i32 0

if.then.2:                                        ; preds = %if.start.0
  ret i32 1

elif.start.3:                                     ; preds = %if.start.0
  %a1 = load i32, ptr %a, align 4
  %EQUAL = icmp eq i32 %a1, 2
  br i1 %EQUAL, label %elif.then.4, label %if.else.5

elif.then.4:                                      ; preds = %elif.start.3
  ret i32 2

if.else.5:                                        ; preds = %elif.start.3
  ret i32 3
}
