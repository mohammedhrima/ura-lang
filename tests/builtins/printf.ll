; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

@STR1 = private unnamed_addr constant [6 x i8] c"alice\00", align 1
@STR2 = private unnamed_addr constant [10 x i8] c"name: %s\0A\00", align 1
@STR3 = private unnamed_addr constant [9 x i8] c"age: %d\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"grade: %c\0A\00", align 1
@STR5 = private unnamed_addr constant [18 x i8] c"name: %s age: %d\0A\00", align 1
@STR6 = private unnamed_addr constant [12 x i8] c"x + y = %d\0A\00", align 1
@STR7 = private unnamed_addr constant [14 x i8] c"negative: %d\0A\00", align 1
@STR8 = private unnamed_addr constant [10 x i8] c"zero: %d\0A\00", align 1
@STR9 = private unnamed_addr constant [5 x i8] c"john\00", align 1
@STR10 = private unnamed_addr constant [4 x i8] c"doe\00", align 1
@STR11 = private unnamed_addr constant [18 x i8] c"full name: %s %s\0A\00", align 1
@str = private unnamed_addr constant [12 x i8] c"hello world\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, i32 noundef, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define noundef i32 @main() local_unnamed_addr #0 {
entry:
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %printf2 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR2, i32 1, ptr nonnull @STR1)
  %printf4 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR3, i32 1, i32 30)
  %printf6 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR4, i32 1, i8 65)
  %printf9 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR5, i32 2, ptr nonnull @STR1, i32 30)
  %printf12 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR6, i32 1, i32 30)
  %printf14 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR7, i32 1, i32 -5)
  %printf15 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR8, i32 1, i32 0)
  %printf18 = tail call i32 (ptr, i32, ...) @printf(ptr nonnull dereferenceable(1) @STR11, i32 2, ptr nonnull @STR9, ptr nonnull @STR10)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
