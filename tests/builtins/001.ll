; ModuleID = '/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn'
source_filename = "/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn"

@str_literal = private constant [9 x i8] c"is digit\00"
@str_literal.1 = private constant [13 x i8] c"is not digit\00"

declare i32 @puts(ptr)

define i1 @isdigit(i8 %c) {
entry:
  %MO_EQ = icmp sge i8 %c, 48
  %LE_EQ = icmp sle i8 %c, 57
  %AND = and i1 %MO_EQ, %LE_EQ
  ret i1 %AND
}

define i32 @main() {
entry:
  %isdigit = call i1 @isdigit(i8 49)
  br i1 %isdigit, label %if, label %else

if:                                               ; preds = %entry
  %puts = call i32 @puts(ptr @str_literal)
  br label %end_if

else:                                             ; preds = %entry
  %puts1 = call i32 @puts(ptr @str_literal.1)
  br label %end_if

end_if:                                           ; preds = %else, %if
  ret i32 0
}
