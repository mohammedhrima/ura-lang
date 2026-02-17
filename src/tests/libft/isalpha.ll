; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i1 @isalpha(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %c2 = load i8, ptr %c1, align 1
  %GE = icmp sge i8 %c2, 97
  %c3 = load i8, ptr %c1, align 1
  %LE = icmp sle i8 %c3, 122
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i32 @main() {
entry:
  br label %if.start

if.start:                                         ; preds = %entry
  %isalpha = call i1 @isalpha(i8 49)
  br i1 %isalpha, label %if.then, label %if.else

if.end:                                           ; No predecessors!
  ret i32 0

if.then:                                          ; preds = %if.start
  ret i32 11

if.else:                                          ; preds = %if.start
  ret i32 12
}
