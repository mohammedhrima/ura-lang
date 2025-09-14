; ModuleID = '/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn'
source_filename = "/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %user = alloca %User, align 8
  %a = getelementptr inbounds nuw %User, ptr %user, i32 0, i32 0
  store i32 10, ptr %a, align 4
  %a1 = getelementptr inbounds nuw %User, ptr %user, i32 0, i32 0
  %a2 = load i32, ptr %a1, align 4
  ret i32 %a2
}
