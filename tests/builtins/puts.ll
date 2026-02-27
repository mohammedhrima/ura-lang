; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

@STR0 = private unnamed_addr constant [9 x i8] c"is digit\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"is not digit\00", align 1

declare i32 @puts(ptr)

define i1 @isdigit(i8 %c) {
entry:
  %0 = add i8 %c, -48
  %AND = icmp ult i8 %0, 10
  ret i1 %AND
}

define i32 @main() {
entry:
  %isdigit = call i1 @isdigit(i8 49)
  br i1 %isdigit, label %if.then, label %if.else

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0

if.then:                                          ; preds = %entry
  %puts = call i32 @puts(ptr noundef nonnull dereferenceable(1) @STR0)
  br label %if.end

if.else:                                          ; preds = %entry
  %puts1 = call i32 @puts(ptr noundef nonnull dereferenceable(1) @STR1)
  br label %if.end
}
