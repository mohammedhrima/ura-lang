; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %a1, 1
  br i1 %EQ, label %then, label %elif

then:                                             ; preds = %entry
  ret i32 1

elif:                                             ; preds = %entry
  %a3 = load i32, i32* %a, align 4
  %EQ4 = icmp eq i32 %a3, 2
  br i1 %EQ4, label %then2, label %else

then2:                                            ; preds = %elif
  ret i32 2

else:                                             ; preds = %elif
  ret i32 3

endif:                                            ; No predecessors!
  ret i32 0
}
