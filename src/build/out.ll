; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 11, i32* %b, align 4
  %c = alloca i1, align 1
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %NQ = icmp ne i32 %a1, %b2
  store i1 %NQ, i1* %c, align 1
  %c3 = load i1, i1* %c, align 1
  ret i1 %c3
}
