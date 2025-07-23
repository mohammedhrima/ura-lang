; ModuleID = 'file.w'
source_filename = "file.w"

define i32 @call() {
entry:
  ret i32 1
}

define i32 @main() {
entry:
  %0 = call i32 @call()
  ret i32 0
}
