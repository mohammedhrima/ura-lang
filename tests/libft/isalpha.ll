; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

define i1 @isalpha(i8 %c) {
entry:
  %MO_EQ = icmp sge i8 %c, 97
  %LE_EQ = icmp sle i8 %c, 122
  %AND = and i1 %MO_EQ, %LE_EQ
  ret i1 %AND
}

define i32 @main() {
entry:
  %isalpha = call i1 @isalpha(i8 49)
  br i1 %isalpha, label %if, label %else

if:                                               ; preds = %entry
  ret i32 11
  br label %end_if

else:                                             ; preds = %entry
  ret i32 12
  br label %end_if

end_if:                                           ; preds = %else, %if
  ret i32 0
}
