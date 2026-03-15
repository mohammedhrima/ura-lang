; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/test.c'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/test.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

%struct.Color = type { i8, i8, i8, i8 }

@.str = private unnamed_addr constant [11 x i8] c"Ura Raylib\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.Color, align 1
  %3 = alloca i64, align 8
  %4 = alloca %struct.Color, align 1
  %5 = alloca i64, align 8
  store i32 0, ptr %1, align 4
  call void @InitWindow(i32 noundef 800, i32 noundef 600, ptr noundef @.str)
  call void @SetTargetFPS(i32 noundef 60)
  br label %6

6:                                                ; preds = %9, %0
  %7 = call zeroext i1 @WindowShouldClose()
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %20

9:                                                ; preds = %6
  call void @BeginDrawing()
  %10 = getelementptr inbounds nuw %struct.Color, ptr %2, i32 0, i32 0
  store i8 30, ptr %10, align 1
  %11 = getelementptr inbounds nuw %struct.Color, ptr %2, i32 0, i32 1
  store i8 30, ptr %11, align 1
  %12 = getelementptr inbounds nuw %struct.Color, ptr %2, i32 0, i32 2
  store i8 -106, ptr %12, align 1
  %13 = getelementptr inbounds nuw %struct.Color, ptr %2, i32 0, i32 3
  store i8 -1, ptr %13, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %3, ptr align 1 %2, i64 4, i1 false)
  %14 = load i64, ptr %3, align 8
  call void @ClearBackground(i64 %14)
  %15 = getelementptr inbounds nuw %struct.Color, ptr %4, i32 0, i32 0
  store i8 -1, ptr %15, align 1
  %16 = getelementptr inbounds nuw %struct.Color, ptr %4, i32 0, i32 1
  store i8 -36, ptr %16, align 1
  %17 = getelementptr inbounds nuw %struct.Color, ptr %4, i32 0, i32 2
  store i8 0, ptr %17, align 1
  %18 = getelementptr inbounds nuw %struct.Color, ptr %4, i32 0, i32 3
  store i8 -1, ptr %18, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %5, ptr align 1 %4, i64 4, i1 false)
  %19 = load i64, ptr %5, align 8
  call void @DrawRectangle(i32 noundef 350, i32 noundef 250, i32 noundef 100, i32 noundef 100, i64 %19)
  call void @EndDrawing()
  br label %6, !llvm.loop !6

20:                                               ; preds = %6
  call void @CloseWindow()
  ret i32 0
}

declare void @InitWindow(i32 noundef, i32 noundef, ptr noundef) #1

declare void @SetTargetFPS(i32 noundef) #1

declare zeroext i1 @WindowShouldClose() #1

declare void @BeginDrawing() #1

declare void @ClearBackground(i64) #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

declare void @DrawRectangle(i32 noundef, i32 noundef, i32 noundef, i32 noundef, i64) #1

declare void @EndDrawing() #1

declare void @CloseWindow() #1

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 26, i32 2]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 20.1.8"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
