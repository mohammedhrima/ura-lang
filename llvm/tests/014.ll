; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [5 x i8] c"user\00", align 1
@STR1 = private unnamed_addr constant [24 x i8] c"user (%s) has %d years\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @main() {
entry:
  %user = alloca { ptr, i32 }, align 8
  %name = getelementptr { ptr, i32 }, ptr %user, i32 0, i32 0
  store ptr @STR0, ptr %name, align 8
  %age = getelementptr { ptr, i32 }, ptr %user, i32 0, i32 1
  store i32 30, ptr %age, align 4
  %name1 = getelementptr { ptr, i32 }, ptr %user, i32 0, i32 0
  %name2 = load ptr, ptr %name1, align 8
  %age3 = getelementptr { ptr, i32 }, ptr %user, i32 0, i32 1
  %age4 = load i32, ptr %age3, align 4
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 2, ptr %name2, i32 %age4)
  %age5 = getelementptr { ptr, i32 }, ptr %user, i32 0, i32 1
  %age6 = load i32, ptr %age5, align 4
  ret i32 %age6
}
