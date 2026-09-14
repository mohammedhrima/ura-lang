; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %b = alloca i32, align 4
  store i32 1, i32* %b, align 4
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  %c = alloca i32, align 4
  %a1 = load i32, i32* %a, align 4
  %ADD = add i32 %a1, 1
  store i32 %ADD, i32* %c, align 4
  %a2 = load i32, i32* %a, align 4
  ret i32 %a2
}
