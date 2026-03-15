; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/no-methods/st02.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/no-methods/st02.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Model = type { i8*, i32 }
%struct.Company = type { i8* }
%struct.Car = type { %struct.Model, %struct.Company }
%struct.User = type { i8*, i32, %struct.Car }

@STR0 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"car\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define void @Model.init(%struct.Model* %0) {
entry:
  store %struct.Model zeroinitializer, %struct.Model* %0, align 8
  ret void
}

define void @Model.clean(%struct.Model* %0) {
entry:
  ret void
}

define void @Company.init(%struct.Company* %0) {
entry:
  store %struct.Company zeroinitializer, %struct.Company* %0, align 8
  ret void
}

define void @Company.clean(%struct.Company* %0) {
entry:
  ret void
}

define void @Car.init(%struct.Car* %0) {
entry:
  store %struct.Car zeroinitializer, %struct.Car* %0, align 8
  %model = getelementptr %struct.Car, %struct.Car* %0, i32 0, i32 0
  call void @Model.init(%struct.Model* %model)
  %company = getelementptr %struct.Car, %struct.Car* %0, i32 0, i32 1
  call void @Company.init(%struct.Company* %company)
  ret void
}

define void @Car.clean(%struct.Car* %0) {
entry:
  %model = getelementptr %struct.Car, %struct.Car* %0, i32 0, i32 0
  call void @Model.clean(%struct.Model* %model)
  %company = getelementptr %struct.Car, %struct.Car* %0, i32 0, i32 1
  call void @Company.clean(%struct.Company* %company)
  ret void
}

define void @User.init(%struct.User* %0) {
entry:
  store %struct.User zeroinitializer, %struct.User* %0, align 8
  %car = getelementptr %struct.User, %struct.User* %0, i32 0, i32 2
  call void @Car.init(%struct.Car* %car)
  ret void
}

define void @User.clean(%struct.User* %0) {
entry:
  %car = getelementptr %struct.User, %struct.User* %0, i32 0, i32 2
  call void @Car.clean(%struct.Car* %car)
  ret void
}

define i32 @main() !dbg !4 {
entry:
  %user = alloca %struct.User, align 8, !dbg !7
  store %struct.User zeroinitializer, %struct.User* %user, align 8, !dbg !7
  call void @User.init(%struct.User* %user), !dbg !7
  %name = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  %age = getelementptr %struct.User, %struct.User* %user, i32 0, i32 1, !dbg !7
  store i32 123, i32* %age, align 4, !dbg !7
  %car = getelementptr %struct.User, %struct.User* %user, i32 0, i32 2, !dbg !7
  %model = getelementptr %struct.Car, %struct.Car* %car, i32 0, i32 0, !dbg !7
  %name1 = getelementptr %struct.Model, %struct.Model* %model, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR1, i32 0, i32 0), i8** %name1, align 8, !dbg !7
  %car2 = getelementptr %struct.User, %struct.User* %user, i32 0, i32 2, !dbg !7
  %model3 = getelementptr %struct.Car, %struct.Car* %car2, i32 0, i32 0, !dbg !7
  %name4 = getelementptr %struct.Model, %struct.Model* %model3, i32 0, i32 0, !dbg !7
  %0 = load i8*, i8** %name4, align 8, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %0), !dbg !7
  call void @User.clean(%struct.User* %user), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "st02.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/no-methods")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 20, type: !5, scopeLine: 20, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 20, scope: !4)
