; ModuleID = '/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn'
source_filename = "/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn"

define i1 @isalpha(i8 %c) {
entry:
  %MO_EQ = icmp sge i8 %c, 97
  %LE_EQ = icmp sle i8 %c, 122
  %and = and i1 %MO_EQ, %LE_EQ
  ret i1 %and
}

define i1 @isdigit(i8 %c) {
entry:
  %MO_EQ = icmp sge i8 %c, 49
  %LE_EQ = icmp sle i8 %c, 57
  %and = and i1 %MO_EQ, %LE_EQ
  ret i1 %and
}

define i1 @islanum(i8 %c) {
entry:
  %isalpha = call i1 @isalpha(i8 %c)
  %isdigit = call i1 @isdigit(i8 %c)
  %or = or i1 %isalpha, %isdigit
  ret i1 %or
}

define i32 @main() {
entry:
  %isdigit = call i1 @isdigit(i8 49)
  br i1 %isdigit, label %if, label %else

if:                                               ; preds = %entry
  ret i32 11
  br label %end_if

else:                                             ; preds = %entry
  ret i32 12
  br label %end_if

end_if:                                           ; preds = %else, %if
  ret i32 0
}
