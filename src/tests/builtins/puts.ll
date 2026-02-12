; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [9 x i8] c"is digit\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"is not digit\00", align 1

declare i32 @puts(ptr)

define i1 @isdigit(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %c2 = load i8, ptr %c1, align 1
  %GE = icmp sge i8 %c2, 48
  %c3 = load i8, ptr %c1, align 1
  %LE = icmp sle i8 %c3, 57
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i32 @main() {
entry:
  br label %if.start

if.start:                                         ; preds = %entry
  %isdigit = call i1 @isdigit(i8 49)
  br i1 %isdigit, label %if.then, label %if.else

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0

if.then:                                          ; preds = %if.start
  %puts = call i32 @puts(ptr @STR0)
  br label %if.end

if.else:                                          ; preds = %if.start
  %puts1 = call i32 @puts(ptr @STR1)
  br label %if.end
}
