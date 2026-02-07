; ModuleID = 'moduleName'
source_filename = "moduleName"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [9 x i8] c"is digit\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"is not digit\00", align 1

declare i32 @puts(ptr)

define i1 @isdigit(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %c2 = load i8, ptr %c1, align 1
  %MO_EQUAL = icmp sge i8 %c2, 48
  %c3 = load i8, ptr %c1, align 1
  %LE_EQUAL = icmp sle i8 %c3, 57
  %AND = and i1 %MO_EQUAL, %LE_EQUAL
  ret i1 %AND
}

define i32 @main() {
entry:
  %isdigit = call i1 @isdigit(i8 49)
  br i1 %isdigit, label %if, label %else

if:                                               ; preds = %entry
  %puts = call i32 @puts(ptr @STR0)
  br label %end_if

else:                                             ; preds = %entry
  %puts1 = call i32 @puts(ptr @STR1)
  br label %end_if

end_if:                                           ; preds = %else, %if
  ret i32 0
}
