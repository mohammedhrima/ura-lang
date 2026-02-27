; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i32 @square(i32 %n) local_unnamed_addr #0 {
entry:
  %MUL = mul i32 %n, %n
  ret i32 %MUL
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i32 @main() local_unnamed_addr #0 {
entry:
  ret i32 25
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
