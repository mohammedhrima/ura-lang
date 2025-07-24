; ModuleID = '/home/ironman/Desktop/Wolf-Compiler/src/code/file.w'
source_filename = "/home/ironman/Desktop/Wolf-Compiler/src/code/file.w"

define i32 @m() {
entry:
  ret i32 12
}

define i32 @main() {
entry:
  %0 = call i32 @m()
  ret i32 0
}
