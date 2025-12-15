; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

define i1 @isalpha(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %c2 = load i8, ptr %c1, align 1
  %MO_EQUAL = icmp sge i8 %c2, 97
  %c3 = load i8, ptr %c1, align 1
  %LE_EQUAL = icmp sle i8 %c3, 122
  %AND = and i1 %MO_EQUAL, %LE_EQUAL
  ret i1 %AND
}

define i1 @isdigit(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %c2 = load i8, ptr %c1, align 1
  %MO_EQUAL = icmp sge i8 %c2, 49
  %c3 = load i8, ptr %c1, align 1
  %LE_EQUAL = icmp sle i8 %c3, 57
  %AND = and i1 %MO_EQUAL, %LE_EQUAL
  ret i1 %AND
}

define i1 @islanum(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %c2 = load i8, ptr %c1, align 1
  %isalpha = call i1 @isalpha(i8 %c2)
  %c3 = load i8, ptr %c1, align 1
  %isdigit = call i1 @isdigit(i8 %c3)
  %OR = or i1 %isalpha, %isdigit
  ret i1 %OR
}

define i32 @main() {
entry:
  %islanum = call i1 @islanum(i8 49)
  br i1 %islanum, label %if, label %else

if:                                               ; preds = %entry
  ret i32 11
  br label %end_if

else:                                             ; preds = %entry
  ret i32 12
  br label %end_if

end_if:                                           ; preds = %else, %if
  ret i32 0
}
