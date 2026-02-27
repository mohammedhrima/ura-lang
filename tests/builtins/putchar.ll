; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

declare i32 @write(i32, ptr, i32) local_unnamed_addr

define noundef i32 @putchar(i8 %c) local_unnamed_addr {
entry:
  %stack = alloca [2 x i8], align 1
  store i8 %c, ptr %stack, align 1
  %write = call i32 @write(i32 1, ptr nonnull %stack, i32 1)
  ret i32 0
}

define noundef i32 @main() local_unnamed_addr {
entry:
  %stack.i2 = alloca [2 x i8], align 1
  %stack.i = alloca [2 x i8], align 1
  call void @llvm.lifetime.start.p0(i64 2, ptr nonnull %stack.i)
  store i8 97, ptr %stack.i, align 1
  %write.i = call i32 @write(i32 1, ptr nonnull %stack.i, i32 1)
  call void @llvm.lifetime.end.p0(i64 2, ptr nonnull %stack.i)
  call void @llvm.lifetime.start.p0(i64 2, ptr nonnull %stack.i2)
  store i8 10, ptr %stack.i2, align 1
  %write.i3 = call i32 @write(i32 1, ptr nonnull %stack.i2, i32 1)
  call void @llvm.lifetime.end.p0(i64 2, ptr nonnull %stack.i2)
  ret i32 0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #0

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
