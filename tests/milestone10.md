# milestone10

## index

- 001 — loop: loop (infinite) with break + continue
- 002 — loop: for over Range::create in both directions
- 003 — loop: array-for iterates elements by value
- 004 — loop: change array elements through an index
- 005 — loop: for without a loop variable
- 006 — loop: for over a non-iterable
- 008 — loop: for over struct fields: an Array of rows and an Array of structs
- 009 — loop: Range::create with a step
- 012 — loop: break and continue inside a for
- 013 — loop: continue: skip wall tiles '#', count open spaces

---

## 001 — loop: loop (infinite) with break + continue

```ura
// stdout: 1 2 4 5
// expect: 0

fn main() i32:
    i i32 = 0
    while True:
        i = i + 1
        if i == 3:
            continue
        if i == 6:
            break
        output(i, " ")
    output("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif1, %if1.then, %entry
  br i1 true, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i", 1
  store i32 %ADD, i32* %i, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i2" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i2", 3
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %while0

endif:                                            ; preds = %if1
  br label %if2

if2:                                              ; preds = %endif
  %"$i3" = load i32, i32* %i, align 4
  %EQ4 = icmp eq i32 %"$i3", 6
  br i1 %EQ4, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  br label %endloop

endif1:                                           ; preds = %if2
  %"$i5" = load i32, i32* %i, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$i5")
  br label %while0

endloop:                                          ; preds = %if2.then, %while0
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 002 — loop: for over Range::create in both directions

```ura
// stdout: 0 1 2 3 4 | 5 4 3 2 1
// expect: 0

fn main() i32:
    for i in Range::create(0, 5):
        output(i, " ")
    output("| ")
    for i in Range::create(5, 0):
        output(i, " ")
    output("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Range = type { i32, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"| \00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define %Range @Range.create(i32 %0, i32 %1) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 1, i32* %r.step, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*self" = load %Range*, %Range** %self, align 8
  %"*self.start" = getelementptr inbounds %Range, %Range* %"*self", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*self1" = load %Range*, %Range** %self, align 8
  %"*self.step" = getelementptr inbounds %Range, %Range* %"*self1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.step" = load i32, i32* %"*self.step", align 4
  %MUL = mul i32 %"$*i", %"$*self.step"
  %"$*self.start" = load i32, i32* %"*self.start", align 4
  %ADD = add i32 %"$*self.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*self2" = load %Range*, %Range** %self, align 8
  %"*self.step3" = getelementptr inbounds %Range, %Range* %"*self2", i32 0, i32 2
  %"$*self.step4" = load i32, i32* %"*self.step3", align 4
  %GT = icmp sgt i32 %"$*self.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*self6" = load %Range*, %Range** %self, align 8
  %"*self.stop" = getelementptr inbounds %Range, %Range* %"*self6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*self.stop" = load i32, i32* %"*self.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*self.stop"
  %AND = and i1 %GT, %GE
  %"*self7" = load %Range*, %Range** %self, align 8
  %"*self.step8" = getelementptr inbounds %Range, %Range* %"*self7", i32 0, i32 2
  %"$*self.step9" = load i32, i32* %"*self.step8", align 4
  %LT = icmp slt i32 %"$*self.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*self11" = load %Range*, %Range** %self, align 8
  %"*self.stop12" = getelementptr inbounds %Range, %Range* %"*self11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*self.stop14" = load i32, i32* %"*self.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*self.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if1

if1:                                              ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i1 false

endif:                                            ; preds = %if1
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
}

define i32 @main() {
entry:
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i2 = alloca i32, align 4
  %tmp5 = alloca %Range, align 8
  %i6 = alloca i32, align 4
  %i7 = alloca i32, align 4
  %call.Range.create = call %Range @Range.create(i32 0, i32 5)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i2, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i2)
  br i1 %call.Range.in, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"$i" = load i32, i32* %i2, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$i")
  br label %while2

endloop:                                          ; preds = %while2
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  %call.Range.create4 = call %Range @Range.create(i32 5, i32 0)
  store %Range %call.Range.create4, %Range* %tmp5, align 4
  store i32 0, i32* %i6, align 4
  store i32 0, i32* %i7, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %endloop
  %call.Range.in8 = call i1 @Range.in(%Range* %tmp5, i32* %i6, i32* %i7)
  br i1 %call.Range.in8, label %while3.then, label %endloop1

while3.then:                                      ; preds = %while3
  %"$i9" = load i32, i32* %i7, align 4
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$i9")
  br label %while3

endloop1:                                         ; preds = %while3
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 003 — loop: array-for iterates elements by value

```ura
// stdout: sum = 60
// expect: 0

fn main() i32:
    nums Array<i32> = [10, 20, 30]
    total i32 = 0
    for x in nums:
        total = total + x
    output("sum = ", total, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i32>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"sum = %d\0A\00", align 1

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"copy<i32>"(i32* %0, i32* %1, i32 %2) {
entry:
  %dst = alloca i32*, align 8
  %src = alloca i32*, align 8
  %count = alloca i32, align 4
  store i32* %0, i32** %dst, align 8
  store i32* %1, i32** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i32*, i32** %dst, align 8
  %AS = bitcast i32* %"$dst" to i8*
  %"$src" = load i32*, i32** %src, align 8
  %AS1 = bitcast i32* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %"$cap8")
  store i32* %"call.new<i32>", i32** %bigger, align 8
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i32>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i32>.own"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define i1 @"Array<i32>.in"(%"Array<i32>"* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*i" = load i32*, i32** %i, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$*i", %"$*self.count"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  ret i1 false

endif:                                            ; preds = %if6
  %"*x" = load i32*, i32** %x, align 8
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"*i2" = load i32*, i32** %i, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*i3" = load i32, i32* %"*i2", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %"*x", align 4
  %"*i4" = load i32*, i32** %i, align 8
  %"$*i5" = load i32, i32* %"*i4", align 4
  %ADD = add i32 %"$*i5", 1
  store i32 %ADD, i32* %"*i4", align 4
  ret i1 true
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if7

if7:                                              ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  ret void
}

define i32 @main() {
entry:
  %nums = alloca %"Array<i32>", align 8
  %arr = alloca [3 x i32], align 4
  %array = alloca %"Array<i32>", align 8
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  %x = alloca i32, align 4
  store [3 x i32] [i32 10, i32 20, i32 30], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  %1 = insertvalue %"Array<i32>" zeroinitializer, i32* %0, 0
  %2 = insertvalue %"Array<i32>" %1, i32 3, 1
  %3 = insertvalue %"Array<i32>" %2, i32 0, 2
  store %"Array<i32>" %3, %"Array<i32>"* %array, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %array)
  %"$array" = load %"Array<i32>", %"Array<i32>"* %array, align 8
  store %"Array<i32>" %"$array", %"Array<i32>"* %nums, align 8
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %x, align 4
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"call.Array<i32>.in" = call i1 @"Array<i32>.in"(%"Array<i32>"* %nums, i32* %i, i32* %x)
  br i1 %"call.Array<i32>.in", label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"$total" = load i32, i32* %total, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$total", %"$x"
  store i32 %ADD, i32* %total, align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"$total1" = load i32, i32* %total, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$total1")
  call void @"Array<i32>.drop"(%"Array<i32>"* %nums)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 004 — loop: change array elements through an index

```ura
// stdout: 2 4 6 8
// expect: 0

fn main() i32:
    nums Array<i32> = [1, 2, 3, 4]
    for i in Range::create(0, nums.len()):
        nums[i] = nums[i] * 2
    for x in nums:
        output(x, " ")
    output("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Range = type { i32, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define %Range @Range.create(i32 %0, i32 %1) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 1, i32* %r.step, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*self" = load %Range*, %Range** %self, align 8
  %"*self.start" = getelementptr inbounds %Range, %Range* %"*self", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*self1" = load %Range*, %Range** %self, align 8
  %"*self.step" = getelementptr inbounds %Range, %Range* %"*self1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.step" = load i32, i32* %"*self.step", align 4
  %MUL = mul i32 %"$*i", %"$*self.step"
  %"$*self.start" = load i32, i32* %"*self.start", align 4
  %ADD = add i32 %"$*self.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*self2" = load %Range*, %Range** %self, align 8
  %"*self.step3" = getelementptr inbounds %Range, %Range* %"*self2", i32 0, i32 2
  %"$*self.step4" = load i32, i32* %"*self.step3", align 4
  %GT = icmp sgt i32 %"$*self.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*self6" = load %Range*, %Range** %self, align 8
  %"*self.stop" = getelementptr inbounds %Range, %Range* %"*self6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*self.stop" = load i32, i32* %"*self.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*self.stop"
  %AND = and i1 %GT, %GE
  %"*self7" = load %Range*, %Range** %self, align 8
  %"*self.step8" = getelementptr inbounds %Range, %Range* %"*self7", i32 0, i32 2
  %"$*self.step9" = load i32, i32* %"*self.step8", align 4
  %LT = icmp slt i32 %"$*self.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*self11" = load %Range*, %Range** %self, align 8
  %"*self.stop12" = getelementptr inbounds %Range, %Range* %"*self11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*self.stop14" = load i32, i32* %"*self.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*self.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if1

if1:                                              ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i1 false

endif:                                            ; preds = %if1
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
}

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"copy<i32>"(i32* %0, i32* %1, i32 %2) {
entry:
  %dst = alloca i32*, align 8
  %src = alloca i32*, align 8
  %count = alloca i32, align 4
  store i32* %0, i32** %dst, align 8
  store i32* %1, i32** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i32*, i32** %dst, align 8
  %AS = bitcast i32* %"$dst" to i8*
  %"$src" = load i32*, i32** %src, align 8
  %AS1 = bitcast i32* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if2

if2:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  ret void

endif1:                                           ; preds = %if3
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %"$cap8")
  store i32* %"call.new<i32>", i32** %bigger, align 8
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i32>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if4

if4:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if4.then, %if4
  %"*self19" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i32>.own"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if5

if5:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %while7

while7:                                           ; preds = %while7.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define i32 @"Array<i32>.len"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32* @"Array<i32>.[]"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  ret i32* %"*self.data[]"
}

define i1 @"Array<i32>.in"(%"Array<i32>"* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"*i" = load i32*, i32** %i, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$*i", %"$*self.count"
  br i1 %GE, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  ret i1 false

endif:                                            ; preds = %if8
  %"*x" = load i32*, i32** %x, align 8
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"*i2" = load i32*, i32** %i, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*i3" = load i32, i32* %"*i2", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %"*x", align 4
  %"*i4" = load i32*, i32** %i, align 8
  %"$*i5" = load i32, i32* %"*i4", align 4
  %ADD = add i32 %"$*i5", 1
  store i32 %ADD, i32* %"*i4", align 4
  ret i1 true
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if9

if9:                                              ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  ret void
}

define i32 @main() {
entry:
  %nums = alloca %"Array<i32>", align 8
  %arr = alloca [4 x i32], align 4
  %array = alloca %"Array<i32>", align 8
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i2 = alloca i32, align 4
  %ref = alloca i32*, align 8
  %ref5 = alloca i32*, align 8
  %i7 = alloca i32, align 4
  %x = alloca i32, align 4
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  %1 = insertvalue %"Array<i32>" zeroinitializer, i32* %0, 0
  %2 = insertvalue %"Array<i32>" %1, i32 4, 1
  %3 = insertvalue %"Array<i32>" %2, i32 0, 2
  store %"Array<i32>" %3, %"Array<i32>"* %array, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %array)
  %"$array" = load %"Array<i32>", %"Array<i32>"* %array, align 8
  store %"Array<i32>" %"$array", %"Array<i32>"* %nums, align 8
  %"call.Array<i32>.len" = call i32 @"Array<i32>.len"(%"Array<i32>"* %nums)
  %call.Range.create = call %Range @Range.create(i32 0, i32 %"call.Array<i32>.len")
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i2, align 4
  br label %while10

while10:                                          ; preds = %while10.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i2)
  br i1 %call.Range.in, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"$i" = load i32, i32* %i2, align 4
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %nums, i32 %"$i")
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  %"$i3" = load i32, i32* %i2, align 4
  %"call.Array<i32>.[]4" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %nums, i32 %"$i3")
  store i32* %"call.Array<i32>.[]4", i32** %ref5, align 8
  %"*ref6" = load i32*, i32** %ref5, align 8
  %"$*ref" = load i32, i32* %"*ref6", align 4
  %MUL = mul i32 %"$*ref", 2
  store i32 %MUL, i32* %"*ref", align 4
  br label %while10

endloop:                                          ; preds = %while10
  store i32 0, i32* %i7, align 4
  store i32 0, i32* %x, align 4
  br label %while11

while11:                                          ; preds = %while11.then, %endloop
  %"call.Array<i32>.in" = call i1 @"Array<i32>.in"(%"Array<i32>"* %nums, i32* %i7, i32* %x)
  br i1 %"call.Array<i32>.in", label %while11.then, label %endloop1

while11.then:                                     ; preds = %while11
  %"$x" = load i32, i32* %x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x")
  br label %while11

endloop1:                                         ; preds = %while11
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  call void @"Array<i32>.drop"(%"Array<i32>"* %nums)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 005 — loop: for without a loop variable

```ura
// error: expected a name after 'for'
fn main() i32:
    for in Range::create(0, 5):
        output(1)
    return 0
```

### stderr

```
error: test.ura:3:9 expected a name after 'for'
  |
3 |     for in Range::create(0, 5):
  |         ^^
help: use for x in Range::create(0, 5):
error: test.ura:3:9 expected 'in' after the loop name
  |
3 |     for in Range::create(0, 5):
  |         ^^
help: use for in in Range::create(0, 5):
error: test.ura:3:9 unexpected 'in'
  |
3 |     for in Range::create(0, 5):
  |         ^^
error: test.ura:3:12 expected ':' after the for line
  |
3 |     for in Range::create(0, 5):
  |            ^^^^^
help: use for in: Range::create(0, 5):
error: test.ura:3:31 unexpected ':'
  |
3 |     for in Range::create(0, 5):
  |                               ^
error: aborting due to 5 errors
```

### status

```
1
```

---

## 006 — loop: for over a non-iterable

```ura
// error: can't loop over 'i32'
fn main() i32:
    x i32 = 5
    for i in x:
        output(i)
    return 0
```

### stderr

```
error: test.ura:4:14 can't loop over 'i32'
  |
4 |     for i in x:
  |              ^
help: give it operator in(i &i32, x &T) bool
error: aborting due to 1 error
```

### status

```
1
```

---

## 008 — loop: for over struct fields: an Array of rows and an Array of structs

```ura
// stdout: 0 0 0
// stdout: 0 0 9
// stdout: 0 1 2
// stdout: 0 10 20
// expect: 0

struct Room:
    floor i32

struct World:
    grid  Array<Array<i32>>
    rooms Array<Room>

    pub fn create() World:
        w World
        w.grid.append([0, 0, 0])
        w.grid.append([0, 0, 9])
        for i in Range::create(0, 3):
            r Room
            r.floor = i
            w.rooms.append(r)
        return w

    // nested for over a 2D field
    fn show_grid():
        for row in self.grid:
            for v in row:
                output(v, " ")
            output("\n")

    // read the rooms, change them through an index, read them again
    fn bump():
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")
        for i in Range::create(0, self.rooms.len()):
            self.rooms[i].floor = self.rooms[i].floor * 10
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")

fn main() i32:
    w World = World::create()
    w.show_grid()
    w.bump()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Range = type { i32, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }
%"Array<Array<i32>>" = type { %"Array<i32>"*, i32, i32 }
%Room = type { i32 }
%"Array<Room>" = type { %Room*, i32, i32 }
%World = type { %"Array<Array<i32>>", %"Array<Room>" }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define %Range @Range.create(i32 %0, i32 %1) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 1, i32* %r.step, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*self" = load %Range*, %Range** %self, align 8
  %"*self.start" = getelementptr inbounds %Range, %Range* %"*self", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*self1" = load %Range*, %Range** %self, align 8
  %"*self.step" = getelementptr inbounds %Range, %Range* %"*self1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.step" = load i32, i32* %"*self.step", align 4
  %MUL = mul i32 %"$*i", %"$*self.step"
  %"$*self.start" = load i32, i32* %"*self.start", align 4
  %ADD = add i32 %"$*self.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*self2" = load %Range*, %Range** %self, align 8
  %"*self.step3" = getelementptr inbounds %Range, %Range* %"*self2", i32 0, i32 2
  %"$*self.step4" = load i32, i32* %"*self.step3", align 4
  %GT = icmp sgt i32 %"$*self.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*self6" = load %Range*, %Range** %self, align 8
  %"*self.stop" = getelementptr inbounds %Range, %Range* %"*self6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*self.stop" = load i32, i32* %"*self.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*self.stop"
  %AND = and i1 %GT, %GE
  %"*self7" = load %Range*, %Range** %self, align 8
  %"*self.step8" = getelementptr inbounds %Range, %Range* %"*self7", i32 0, i32 2
  %"$*self.step9" = load i32, i32* %"*self.step8", align 4
  %LT = icmp slt i32 %"$*self.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*self11" = load %Range*, %Range** %self, align 8
  %"*self.stop12" = getelementptr inbounds %Range, %Range* %"*self11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*self.stop14" = load i32, i32* %"*self.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*self.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if1

if1:                                              ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i1 false

endif:                                            ; preds = %if1
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
}

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"copy<i32>"(i32* %0, i32* %1, i32 %2) {
entry:
  %dst = alloca i32*, align 8
  %src = alloca i32*, align 8
  %count = alloca i32, align 4
  store i32* %0, i32** %dst, align 8
  store i32* %1, i32** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i32*, i32** %dst, align 8
  %AS = bitcast i32* %"$dst" to i8*
  %"$src" = load i32*, i32** %src, align 8
  %AS1 = bitcast i32* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if2

if2:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  ret void

endif1:                                           ; preds = %if3
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %"$cap8")
  store i32* %"call.new<i32>", i32** %bigger, align 8
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i32>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if4

if4:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if4.then, %if4
  %"*self19" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i32>.own"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if5

if5:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %while7

while7:                                           ; preds = %while7.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define void @"Array<i32>.="(%"Array<i32>"* %0, %"Array<i32>"* %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %other = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %other, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*other" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"*other2" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i6"
  %"*other7" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other7", i32 0, i32 0
  %"$*other.data" = load i32*, i32** %"*other.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i32, i32* %"$*other.data", i32 %"$i8"
  %"$*other.data[]" = load i32, i32* %"*other.data[]", align 4
  store i32 %"$*other.data[]", i32* %"*self.data[]", align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"*other11" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define i1 @"Array<i32>.in"(%"Array<i32>"* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  br label %if9

if9:                                              ; preds = %entry
  %"*i" = load i32*, i32** %i, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$*i", %"$*self.count"
  br i1 %GE, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  ret i1 false

endif:                                            ; preds = %if9
  %"*x" = load i32*, i32** %x, align 8
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"*i2" = load i32*, i32** %i, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*i3" = load i32, i32* %"*i2", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %"*x", align 4
  %"*i4" = load i32*, i32** %i, align 8
  %"$*i5" = load i32, i32* %"*i4", align 4
  %ADD = add i32 %"$*i5", 1
  store i32 %ADD, i32* %"*i4", align 4
  ret i1 true
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if10

if10:                                             ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  ret void
}

define %"Array<i32>"* @"new<Array<i32>>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%"Array<i32>"* getelementptr (%"Array<i32>", %"Array<i32>"* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %"Array<i32>"*
  ret %"Array<i32>"* %AS1
}

define void @"copy<Array<i32>>"(%"Array<i32>"* %0, %"Array<i32>"* %1, i32 %2) {
entry:
  %dst = alloca %"Array<i32>"*, align 8
  %src = alloca %"Array<i32>"*, align 8
  %count = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %dst, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %"Array<i32>"*, %"Array<i32>"** %dst, align 8
  %AS = bitcast %"Array<i32>"* %"$dst" to i8*
  %"$src" = load %"Array<i32>"*, %"Array<i32>"** %src, align 8
  %AS1 = bitcast %"Array<i32>"* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%"Array<i32>"* getelementptr (%"Array<i32>", %"Array<i32>"* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Array<i32>>"(%"Array<i32>"* %0) {
entry:
  %data = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %data, align 8
  %"$data" = load %"Array<i32>"*, %"Array<i32>"** %data, align 8
  %AS = bitcast %"Array<i32>"* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %"Array<i32>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  br label %if12

if12:                                             ; preds = %endif
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if12.then, label %endif1

if12.then:                                        ; preds = %if12
  ret void

endif1:                                           ; preds = %if12
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Array<i32>>" = call %"Array<i32>"* @"new<Array<i32>>"(i32 %"$cap8")
  store %"Array<i32>"* %"call.new<Array<i32>>", %"Array<i32>"** %bigger, align 8
  %"*self9" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %"Array<i32>"*, %"Array<i32>"** %bigger, align 8
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Array<i32>>"(%"Array<i32>"* %"$bigger", %"Array<i32>"* %"$*self.data", i32 %"$*self.count12")
  br label %if13

if13:                                             ; preds = %endif1
  %"*self13" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if13.then, label %endif2

if13.then:                                        ; preds = %if13
  %"*self16" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data17", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if13.then, %if13
  %"*self19" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %"Array<i32>"*, %"Array<i32>"** %bigger, align 8
  store %"Array<i32>"* %"$bigger21", %"Array<i32>"** %"*self.data20", align 8
  %"*self22" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  br label %while15

while15:                                          ; preds = %while15.then, %endif
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self4" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count10"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]", align 8
  br label %while15

endloop:                                          ; preds = %while15
  ret void
}

define void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %0, %"Array<i32>" %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %value = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store %"Array<i32>" %1, %"Array<i32>"* %value, align 8
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count9"
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %value)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" %"$copy", %"Array<i32>"* %"*self.data[]", align 8
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  call void @"Array<i32>.drop"(%"Array<i32>"* %value)
  ret void
}

define i1 @"Array<Array<i32>>.in"(%"Array<Array<i32>>"* %0, i32* %1, %"Array<i32>"* %2) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %i = alloca i32*, align 8
  %x = alloca %"Array<i32>"*, align 8
  %copy = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32* %1, i32** %i, align 8
  store %"Array<i32>"* %2, %"Array<i32>"** %x, align 8
  br label %if17

if17:                                             ; preds = %entry
  %"*i" = load i32*, i32** %i, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$*i", %"$*self.count"
  br i1 %GE, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  ret i1 false

endif:                                            ; preds = %if17
  %"*x" = load %"Array<i32>"*, %"Array<i32>"** %x, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 0
  %"*i2" = load i32*, i32** %i, align 8
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*i3" = load i32, i32* %"*i2", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*i3"
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %"*self.data[]")
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*x")
  store %"Array<i32>" %"$copy", %"Array<i32>"* %"*x", align 8
  %"*i4" = load i32*, i32** %i, align 8
  %"$*i5" = load i32, i32* %"*i4", align 4
  %ADD = add i32 %"$*i5", 1
  store i32 %ADD, i32* %"*i4", align 4
  ret i1 true
}

define void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %"*self")
  br label %if18

if18:                                             ; preds = %entry
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if18.then, %if18
  ret void
}

define %Room* @"new<Room>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Room*
  ret %Room* %AS1
}

define void @"copy<Room>"(%Room* %0, %Room* %1, i32 %2) {
entry:
  %dst = alloca %Room*, align 8
  %src = alloca %Room*, align 8
  %count = alloca i32, align 4
  store %Room* %0, %Room** %dst, align 8
  store %Room* %1, %Room** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Room*, %Room** %dst, align 8
  %AS = bitcast %Room* %"$dst" to i8*
  %"$src" = load %Room*, %Room** %src, align 8
  %AS1 = bitcast %Room* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Room>"(%Room* %0) {
entry:
  %data = alloca %Room*, align 8
  store %Room* %0, %Room** %data, align 8
  %"$data" = load %Room*, %Room** %data, align 8
  %AS = bitcast %Room* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Room>.reserve"(%"Array<Room>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Room*, align 8
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self3" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  br label %if20

if20:                                             ; preds = %endif
  %"*self6" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if20.then, label %endif1

if20.then:                                        ; preds = %if20
  ret void

endif1:                                           ; preds = %if20
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Room>" = call %Room* @"new<Room>"(i32 %"$cap8")
  store %Room* %"call.new<Room>", %Room** %bigger, align 8
  %"*self9" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Room*, %Room** %bigger, align 8
  %"$*self.data" = load %Room*, %Room** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Room>"(%Room* %"$bigger", %Room* %"$*self.data", i32 %"$*self.count12")
  br label %if21

if21:                                             ; preds = %endif1
  %"*self13" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if21.then, label %endif2

if21.then:                                        ; preds = %if21
  %"*self16" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Room*, %Room** %"*self.data17", align 8
  call void @"free<Room>"(%Room* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if21.then, %if21
  %"*self19" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Room*, %Room** %bigger, align 8
  store %Room* %"$bigger21", %Room** %"*self.data20", align 8
  %"*self22" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Room>.own"(%"Array<Room>"* %0) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  br label %if22

if22:                                             ; preds = %entry
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"*self2" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self3" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Room>.reserve"(%"Array<Room>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if22.then, %if22
  ret void
}

define void @"Array<Room>.clear"(%"Array<Room>"* %0) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self1" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  br label %while24

while24:                                          ; preds = %while24.then, %endif
  %"*self2" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while24.then, label %endloop

while24.then:                                     ; preds = %while24
  %"*self4" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Room*, %Room** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Room, %Room* %"$*self.data", i32 %"$*self.count10"
  store %Room zeroinitializer, %Room* %"*self.data[]", align 4
  br label %while24

endloop:                                          ; preds = %while24
  ret void
}

define void @"Array<Room>.append"(%"Array<Room>"* %0, %Room %1) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  %value = alloca %Room, align 8
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  store %Room %1, %Room* %value, align 4
  br label %if25

if25:                                             ; preds = %entry
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if25.then, label %endif

if25.then:                                        ; preds = %if25
  %"*self2" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self3" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Room>.reserve"(%"Array<Room>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if25.then, %if25
  %"*self6" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Room*, %Room** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Room, %Room* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load %Room, %Room* %value, align 4
  store %Room %"$value", %Room* %"*self.data[]", align 4
  %"*self10" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<Room>.len"(%"Array<Room>"* %0) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Room* @"Array<Room>.[]"(%"Array<Room>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  call void @"Array<Room>.own"(%"Array<Room>"* %"*self")
  %"*self1" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load %Room*, %Room** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds %Room, %Room* %"$*self.data", i32 %"$index"
  ret %Room* %"*self.data[]"
}

define i1 @"Array<Room>.in"(%"Array<Room>"* %0, i32* %1, %Room* %2) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  %i = alloca i32*, align 8
  %x = alloca %Room*, align 8
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  store i32* %1, i32** %i, align 8
  store %Room* %2, %Room** %x, align 8
  br label %if26

if26:                                             ; preds = %entry
  %"*i" = load i32*, i32** %i, align 8
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self", i32 0, i32 1
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$*i", %"$*self.count"
  br i1 %GE, label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  ret i1 false

endif:                                            ; preds = %if26
  %"*x" = load %Room*, %Room** %x, align 8
  %"*self1" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self1", i32 0, i32 0
  %"*i2" = load i32*, i32** %i, align 8
  %"$*self.data" = load %Room*, %Room** %"*self.data", align 8
  %"$*i3" = load i32, i32* %"*i2", align 4
  %"*self.data[]" = getelementptr inbounds %Room, %Room* %"$*self.data", i32 %"$*i3"
  %"$*self.data[]" = load %Room, %Room* %"*self.data[]", align 4
  store %Room %"$*self.data[]", %Room* %"*x", align 4
  %"*i4" = load i32*, i32** %i, align 8
  %"$*i5" = load i32, i32* %"*i4", align 4
  %ADD = add i32 %"$*i5", 1
  store i32 %ADD, i32* %"*i4", align 4
  ret i1 true
}

define void @"Array<Room>.drop"(%"Array<Room>"* %0) {
entry:
  %self = alloca %"Array<Room>"*, align 8
  store %"Array<Room>"* %0, %"Array<Room>"** %self, align 8
  %"*self" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  call void @"Array<Room>.clear"(%"Array<Room>"* %"*self")
  br label %if27

if27:                                             ; preds = %entry
  %"*self1" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if27.then, label %endif

if27.then:                                        ; preds = %if27
  %"*self2" = load %"Array<Room>"*, %"Array<Room>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Room>", %"Array<Room>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load %Room*, %Room** %"*self.data", align 8
  call void @"free<Room>"(%Room* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if27.then, %if27
  ret void
}

define %World @World.create() {
entry:
  %w = alloca %World, align 8
  %arr = alloca [3 x i32], align 4
  %array = alloca %"Array<i32>", align 8
  %arr2 = alloca [3 x i32], align 4
  %array3 = alloca %"Array<i32>", align 8
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i5 = alloca i32, align 4
  %r = alloca %Room, align 8
  store %World zeroinitializer, %World* %w, align 8
  %w.grid = getelementptr inbounds %World, %World* %w, i32 0, i32 0
  store [3 x i32] zeroinitializer, [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  %1 = insertvalue %"Array<i32>" zeroinitializer, i32* %0, 0
  %2 = insertvalue %"Array<i32>" %1, i32 3, 1
  %3 = insertvalue %"Array<i32>" %2, i32 0, 2
  store %"Array<i32>" %3, %"Array<i32>"* %array, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %array)
  %"$array" = load %"Array<i32>", %"Array<i32>"* %array, align 8
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %w.grid, %"Array<i32>" %"$array")
  %w.grid1 = getelementptr inbounds %World, %World* %w, i32 0, i32 0
  store [3 x i32] [i32 0, i32 0, i32 9], [3 x i32]* %arr2, align 4
  %4 = getelementptr inbounds [3 x i32], [3 x i32]* %arr2, i32 0, i32 0
  %5 = insertvalue %"Array<i32>" zeroinitializer, i32* %4, 0
  %6 = insertvalue %"Array<i32>" %5, i32 3, 1
  %7 = insertvalue %"Array<i32>" %6, i32 0, 2
  store %"Array<i32>" %7, %"Array<i32>"* %array3, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %array3)
  %"$array4" = load %"Array<i32>", %"Array<i32>"* %array3, align 8
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %w.grid1, %"Array<i32>" %"$array4")
  %call.Range.create = call %Range @Range.create(i32 0, i32 3)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i5, align 4
  br label %while28

while28:                                          ; preds = %while28.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i5)
  br i1 %call.Range.in, label %while28.then, label %endloop

while28.then:                                     ; preds = %while28
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$i" = load i32, i32* %i5, align 4
  store i32 %"$i", i32* %r.floor, align 4
  %w.rooms = getelementptr inbounds %World, %World* %w, i32 0, i32 1
  %"$r" = load %Room, %Room* %r, align 4
  call void @"Array<Room>.append"(%"Array<Room>"* %w.rooms, %Room %"$r")
  br label %while28

endloop:                                          ; preds = %while28
  %"$w" = load %World, %World* %w, align 8
  ret %World %"$w"
}

define void @World.show_grid(%World* %0) {
entry:
  %self = alloca %World*, align 8
  %i = alloca i32, align 4
  %row = alloca %"Array<i32>", align 8
  %i2 = alloca i32, align 4
  %v = alloca i32, align 4
  store %World* %0, %World** %self, align 8
  %"*self" = load %World*, %World** %self, align 8
  %"*self.grid" = getelementptr inbounds %World, %World* %"*self", i32 0, i32 0
  store i32 0, i32* %i, align 4
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %row, align 8
  br label %while29

while29:                                          ; preds = %endloop, %entry
  %"call.Array<Array<i32>>.in" = call i1 @"Array<Array<i32>>.in"(%"Array<Array<i32>>"* %"*self.grid", i32* %i, %"Array<i32>"* %row)
  br i1 %"call.Array<Array<i32>>.in", label %while29.then, label %endloop1

while29.then:                                     ; preds = %while29
  store i32 0, i32* %i2, align 4
  store i32 0, i32* %v, align 4
  br label %while30

while30:                                          ; preds = %while30.then, %while29.then
  %"call.Array<i32>.in" = call i1 @"Array<i32>.in"(%"Array<i32>"* %row, i32* %i2, i32* %v)
  br i1 %"call.Array<i32>.in", label %while30.then, label %endloop

while30.then:                                     ; preds = %while30
  %"$v" = load i32, i32* %v, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$v")
  br label %while30

endloop:                                          ; preds = %while30
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %while29

endloop1:                                         ; preds = %while29
  call void @"Array<i32>.drop"(%"Array<i32>"* %row)
  ret void
}

define void @World.bump(%World* %0) {
entry:
  %self = alloca %World*, align 8
  %i = alloca i32, align 4
  %r = alloca %Room, align 8
  %tmp = alloca %Range, align 8
  %i6 = alloca i32, align 4
  %i7 = alloca i32, align 4
  %ref = alloca %Room*, align 8
  %ref14 = alloca %Room*, align 8
  %i19 = alloca i32, align 4
  %r20 = alloca %Room, align 8
  store %World* %0, %World** %self, align 8
  %"*self" = load %World*, %World** %self, align 8
  %"*self.rooms" = getelementptr inbounds %World, %World* %"*self", i32 0, i32 1
  store i32 0, i32* %i, align 4
  store %Room zeroinitializer, %Room* %r, align 4
  br label %while31

while31:                                          ; preds = %while31.then, %entry
  %"call.Array<Room>.in" = call i1 @"Array<Room>.in"(%"Array<Room>"* %"*self.rooms", i32* %i, %Room* %r)
  br i1 %"call.Array<Room>.in", label %while31.then, label %endloop

while31.then:                                     ; preds = %while31
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.floor" = load i32, i32* %r.floor, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$r.floor")
  br label %while31

endloop:                                          ; preds = %while31
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %"*self4" = load %World*, %World** %self, align 8
  %"*self.rooms5" = getelementptr inbounds %World, %World* %"*self4", i32 0, i32 1
  %"call.Array<Room>.len" = call i32 @"Array<Room>.len"(%"Array<Room>"* %"*self.rooms5")
  %call.Range.create = call %Range @Range.create(i32 0, i32 %"call.Array<Room>.len")
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i6, align 4
  store i32 0, i32* %i7, align 4
  br label %while32

while32:                                          ; preds = %while32.then, %endloop
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i6, i32* %i7)
  br i1 %call.Range.in, label %while32.then, label %endloop1

while32.then:                                     ; preds = %while32
  %"*self8" = load %World*, %World** %self, align 8
  %"*self.rooms9" = getelementptr inbounds %World, %World* %"*self8", i32 0, i32 1
  %"$i" = load i32, i32* %i7, align 4
  %"call.Array<Room>.[]" = call %Room* @"Array<Room>.[]"(%"Array<Room>"* %"*self.rooms9", i32 %"$i")
  store %Room* %"call.Array<Room>.[]", %Room** %ref, align 8
  %"*ref" = load %Room*, %Room** %ref, align 8
  %"*ref.floor" = getelementptr inbounds %Room, %Room* %"*ref", i32 0, i32 0
  %"*self10" = load %World*, %World** %self, align 8
  %"*self.rooms11" = getelementptr inbounds %World, %World* %"*self10", i32 0, i32 1
  %"$i12" = load i32, i32* %i7, align 4
  %"call.Array<Room>.[]13" = call %Room* @"Array<Room>.[]"(%"Array<Room>"* %"*self.rooms11", i32 %"$i12")
  store %Room* %"call.Array<Room>.[]13", %Room** %ref14, align 8
  %"*ref15" = load %Room*, %Room** %ref14, align 8
  %"*ref.floor16" = getelementptr inbounds %Room, %Room* %"*ref15", i32 0, i32 0
  %"$*ref.floor" = load i32, i32* %"*ref.floor16", align 4
  %MUL = mul i32 %"$*ref.floor", 10
  store i32 %MUL, i32* %"*ref.floor", align 4
  br label %while32

endloop1:                                         ; preds = %while32
  %"*self17" = load %World*, %World** %self, align 8
  %"*self.rooms18" = getelementptr inbounds %World, %World* %"*self17", i32 0, i32 1
  store i32 0, i32* %i19, align 4
  store %Room zeroinitializer, %Room* %r20, align 4
  br label %while33

while33:                                          ; preds = %while33.then, %endloop1
  %"call.Array<Room>.in21" = call i1 @"Array<Room>.in"(%"Array<Room>"* %"*self.rooms18", i32* %i19, %Room* %r20)
  br i1 %"call.Array<Room>.in21", label %while33.then, label %endloop2

while33.then:                                     ; preds = %while33
  %r.floor22 = getelementptr inbounds %Room, %Room* %r20, i32 0, i32 0
  %"$r.floor23" = load i32, i32* %r.floor22, align 4
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$r.floor23")
  br label %while33

endloop2:                                         ; preds = %while33
  %call.dprintf25 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %w = alloca %World, align 8
  %call.World.create = call %World @World.create()
  store %World %call.World.create, %World* %w, align 8
  call void @World.show_grid(%World* %w)
  call void @World.bump(%World* %w)
  %w.rooms = getelementptr inbounds %World, %World* %w, i32 0, i32 1
  call void @"Array<Room>.drop"(%"Array<Room>"* %w.rooms)
  %w.grid = getelementptr inbounds %World, %World* %w, i32 0, i32 0
  call void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %w.grid)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 009 — loop: Range::create with a step

```ura
// stdout: 0 3 6 9
// stdout: 10 7 4 1
// stdout: 0 4 8 12 16
// stdout: 0
// expect: 0

fn main() i32:
    for i in Range::create(0, 10, 3):
        output(i, " ")
    output("\n")
    for i in Range::create(10, 0, -3):
        output(i, " ")
    output("\n")
    n i32 = 4
    for i in Range::create(0, 20, n):
        output(i, " ")
    output("\n")
    // a step wider than the span yields the start only
    for i in Range::create(0, 3, 10):
        output(i, " ")
    output("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Range = type { i32, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define %Range @Range.create(i32 %0, i32 %1, i32 %2) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %step = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store i32 %2, i32* %step, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  %"$step" = load i32, i32* %step, align 4
  store i32 %"$step", i32* %r.step, align 4
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*self" = load %Range*, %Range** %self, align 8
  %"*self.start" = getelementptr inbounds %Range, %Range* %"*self", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*self1" = load %Range*, %Range** %self, align 8
  %"*self.step" = getelementptr inbounds %Range, %Range* %"*self1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.step" = load i32, i32* %"*self.step", align 4
  %MUL = mul i32 %"$*i", %"$*self.step"
  %"$*self.start" = load i32, i32* %"*self.start", align 4
  %ADD = add i32 %"$*self.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*self2" = load %Range*, %Range** %self, align 8
  %"*self.step3" = getelementptr inbounds %Range, %Range* %"*self2", i32 0, i32 2
  %"$*self.step4" = load i32, i32* %"*self.step3", align 4
  %GT = icmp sgt i32 %"$*self.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*self6" = load %Range*, %Range** %self, align 8
  %"*self.stop" = getelementptr inbounds %Range, %Range* %"*self6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*self.stop" = load i32, i32* %"*self.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*self.stop"
  %AND = and i1 %GT, %GE
  %"*self7" = load %Range*, %Range** %self, align 8
  %"*self.step8" = getelementptr inbounds %Range, %Range* %"*self7", i32 0, i32 2
  %"$*self.step9" = load i32, i32* %"*self.step8", align 4
  %LT = icmp slt i32 %"$*self.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*self11" = load %Range*, %Range** %self, align 8
  %"*self.stop12" = getelementptr inbounds %Range, %Range* %"*self11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*self.stop14" = load i32, i32* %"*self.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*self.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 false

endif:                                            ; preds = %if0
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
}

define i32 @main() {
entry:
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i4 = alloca i32, align 4
  %tmp7 = alloca %Range, align 8
  %i8 = alloca i32, align 4
  %i9 = alloca i32, align 4
  %n = alloca i32, align 4
  %tmp15 = alloca %Range, align 8
  %i16 = alloca i32, align 4
  %i17 = alloca i32, align 4
  %tmp23 = alloca %Range, align 8
  %i24 = alloca i32, align 4
  %i25 = alloca i32, align 4
  %call.Range.create = call %Range @Range.create(i32 0, i32 10, i32 3)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i4, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i4)
  br i1 %call.Range.in, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$i" = load i32, i32* %i4, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$i")
  br label %while1

endloop:                                          ; preds = %while1
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call.Range.create6 = call %Range @Range.create(i32 10, i32 0, i32 -3)
  store %Range %call.Range.create6, %Range* %tmp7, align 4
  store i32 0, i32* %i8, align 4
  store i32 0, i32* %i9, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endloop
  %call.Range.in10 = call i1 @Range.in(%Range* %tmp7, i32* %i8, i32* %i9)
  br i1 %call.Range.in10, label %while2.then, label %endloop1

while2.then:                                      ; preds = %while2
  %"$i11" = load i32, i32* %i9, align 4
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$i11")
  br label %while2

endloop1:                                         ; preds = %while2
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  store i32 4, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.Range.create14 = call %Range @Range.create(i32 0, i32 20, i32 %"$n")
  store %Range %call.Range.create14, %Range* %tmp15, align 4
  store i32 0, i32* %i16, align 4
  store i32 0, i32* %i17, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %endloop1
  %call.Range.in18 = call i1 @Range.in(%Range* %tmp15, i32* %i16, i32* %i17)
  br i1 %call.Range.in18, label %while3.then, label %endloop2

while3.then:                                      ; preds = %while3
  %"$i19" = load i32, i32* %i17, align 4
  %call.dprintf20 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$i19")
  br label %while3

endloop2:                                         ; preds = %while3
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %call.Range.create22 = call %Range @Range.create(i32 0, i32 3, i32 10)
  store %Range %call.Range.create22, %Range* %tmp23, align 4
  store i32 0, i32* %i24, align 4
  store i32 0, i32* %i25, align 4
  br label %while4

while4:                                           ; preds = %while4.then, %endloop2
  %call.Range.in26 = call i1 @Range.in(%Range* %tmp23, i32* %i24, i32* %i25)
  br i1 %call.Range.in26, label %while4.then, label %endloop3

while4.then:                                      ; preds = %while4
  %"$i27" = load i32, i32* %i25, align 4
  %call.dprintf28 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %"$i27")
  br label %while4

endloop3:                                         ; preds = %while4
  %call.dprintf29 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 012 — loop: break and continue inside a for

```ura
// stdout: break:    0 1 2 3
// stdout: continue: 1 3 5 7
// stdout: nested:   00 01 10 11 20 21
// expect: 0

fn main() i32:
    output("break:    ")
    for i in Range::create(0, 10):
        if i == 4:
            break
        output(i, " ")

    output("\ncontinue: ")
    for i in Range::create(0, 8):
        if i % 2 == 0:
            continue
        output(i, " ")

    // break leaves only the inner for
    output("\nnested:   ")
    for i in Range::create(0, 3):
        for j in Range::create(0, 3):
            if j == 2:
                break
            output(i, j, " ")
    output("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Range = type { i32, i32, i32 }

@str = private unnamed_addr constant [11 x i8] c"break:    \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"\0Acontinue: \00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"\0Anested:   \00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"%d%d \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define %Range @Range.create(i32 %0, i32 %1) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 1, i32* %r.step, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*self" = load %Range*, %Range** %self, align 8
  %"*self.start" = getelementptr inbounds %Range, %Range* %"*self", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*self1" = load %Range*, %Range** %self, align 8
  %"*self.step" = getelementptr inbounds %Range, %Range* %"*self1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.step" = load i32, i32* %"*self.step", align 4
  %MUL = mul i32 %"$*i", %"$*self.step"
  %"$*self.start" = load i32, i32* %"*self.start", align 4
  %ADD = add i32 %"$*self.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*self2" = load %Range*, %Range** %self, align 8
  %"*self.step3" = getelementptr inbounds %Range, %Range* %"*self2", i32 0, i32 2
  %"$*self.step4" = load i32, i32* %"*self.step3", align 4
  %GT = icmp sgt i32 %"$*self.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*self6" = load %Range*, %Range** %self, align 8
  %"*self.stop" = getelementptr inbounds %Range, %Range* %"*self6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*self.stop" = load i32, i32* %"*self.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*self.stop"
  %AND = and i1 %GT, %GE
  %"*self7" = load %Range*, %Range** %self, align 8
  %"*self.step8" = getelementptr inbounds %Range, %Range* %"*self7", i32 0, i32 2
  %"$*self.step9" = load i32, i32* %"*self.step8", align 4
  %LT = icmp slt i32 %"$*self.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*self11" = load %Range*, %Range** %self, align 8
  %"*self.stop12" = getelementptr inbounds %Range, %Range* %"*self11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*self.stop14" = load i32, i32* %"*self.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*self.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if1

if1:                                              ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i1 false

endif:                                            ; preds = %if1
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
}

define i32 @main() {
entry:
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i6 = alloca i32, align 4
  %tmp11 = alloca %Range, align 8
  %i12 = alloca i32, align 4
  %i13 = alloca i32, align 4
  %tmp21 = alloca %Range, align 8
  %i22 = alloca i32, align 4
  %i23 = alloca i32, align 4
  %tmp26 = alloca %Range, align 8
  %i27 = alloca i32, align 4
  %j = alloca i32, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0))
  %call.Range.create = call %Range @Range.create(i32 0, i32 10)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i6, align 4
  br label %while2

while2:                                           ; preds = %endif, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i6)
  br i1 %call.Range.in, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  br label %if3

if3:                                              ; preds = %while2.then
  %"$i" = load i32, i32* %i6, align 4
  %EQ = icmp eq i32 %"$i", 4
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  br label %endloop

endif:                                            ; preds = %if3
  %"$i7" = load i32, i32* %i6, align 4
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$i7")
  br label %while2

endloop:                                          ; preds = %if3.then, %while2
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0))
  %call.Range.create10 = call %Range @Range.create(i32 0, i32 8)
  store %Range %call.Range.create10, %Range* %tmp11, align 4
  store i32 0, i32* %i12, align 4
  store i32 0, i32* %i13, align 4
  br label %while4

while4:                                           ; preds = %endif1, %if5.then, %endloop
  %call.Range.in14 = call i1 @Range.in(%Range* %tmp11, i32* %i12, i32* %i13)
  br i1 %call.Range.in14, label %while4.then, label %endloop2

while4.then:                                      ; preds = %while4
  br label %if5

if5:                                              ; preds = %while4.then
  %"$i15" = load i32, i32* %i13, align 4
  %MOD = srem i32 %"$i15", 2
  %EQ16 = icmp eq i32 %MOD, 0
  br i1 %EQ16, label %if5.then, label %endif1

if5.then:                                         ; preds = %if5
  br label %while4

endif1:                                           ; preds = %if5
  %"$i17" = load i32, i32* %i13, align 4
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$i17")
  br label %while4

endloop2:                                         ; preds = %while4
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0))
  %call.Range.create20 = call %Range @Range.create(i32 0, i32 3)
  store %Range %call.Range.create20, %Range* %tmp21, align 4
  store i32 0, i32* %i22, align 4
  store i32 0, i32* %i23, align 4
  br label %while6

while6:                                           ; preds = %endloop4, %endloop2
  %call.Range.in24 = call i1 @Range.in(%Range* %tmp21, i32* %i22, i32* %i23)
  br i1 %call.Range.in24, label %while6.then, label %endloop5

while6.then:                                      ; preds = %while6
  %call.Range.create25 = call %Range @Range.create(i32 0, i32 3)
  store %Range %call.Range.create25, %Range* %tmp26, align 4
  store i32 0, i32* %i27, align 4
  store i32 0, i32* %j, align 4
  br label %while7

while7:                                           ; preds = %endif3, %while6.then
  %call.Range.in28 = call i1 @Range.in(%Range* %tmp26, i32* %i27, i32* %j)
  br i1 %call.Range.in28, label %while7.then, label %endloop4

while7.then:                                      ; preds = %while7
  br label %if8

if8:                                              ; preds = %while7.then
  %"$j" = load i32, i32* %j, align 4
  %EQ29 = icmp eq i32 %"$j", 2
  br i1 %EQ29, label %if8.then, label %endif3

if8.then:                                         ; preds = %if8
  br label %endloop4

endif3:                                           ; preds = %if8
  %"$i30" = load i32, i32* %i23, align 4
  %"$j31" = load i32, i32* %j, align 4
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i32 %"$i30", i32 %"$j31")
  br label %while7

endloop4:                                         ; preds = %if8.then, %while7
  br label %while6

endloop5:                                         ; preds = %while6
  %call.dprintf33 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 013 — loop: continue: skip wall tiles '#', count open spaces

```ura
// stdout: Open tiles: 6
// expect: 0

fn main() i32:
    tiles Array<char> = ['#', '.', '#', '.', '.',
                         '#', '.', '@', '.', '#']
    open i32 = 0
    i    i32 = 0
    while i < tiles.len():
        if tiles[i] == '#':
            i = i + 1
            continue
        open = open + 1
        i = i + 1
    output("Open tiles: ", open, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i8>" = type { i8*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"Open tiles: %d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.reserve"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i8>.own"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define i32 @"Array<i8>.len"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  call void @"Array<i8>.own"(%"Array<i8>"* %"*self")
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*self")
  br label %if6

if6:                                              ; preds = %entry
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret void
}

define i32 @main() {
entry:
  %tiles = alloca %"Array<i8>", align 8
  %arr = alloca [10 x i8], align 1
  %array = alloca %"Array<i8>", align 8
  %open = alloca i32, align 4
  %i = alloca i32, align 4
  %ref = alloca i8*, align 8
  store [10 x i8] c"#.#..#.@.#", [10 x i8]* %arr, align 1
  %0 = getelementptr inbounds [10 x i8], [10 x i8]* %arr, i32 0, i32 0
  %1 = insertvalue %"Array<i8>" zeroinitializer, i8* %0, 0
  %2 = insertvalue %"Array<i8>" %1, i32 10, 1
  %3 = insertvalue %"Array<i8>" %2, i32 0, 2
  store %"Array<i8>" %3, %"Array<i8>"* %array, align 8
  call void @"Array<i8>.own"(%"Array<i8>"* %array)
  %"$array" = load %"Array<i8>", %"Array<i8>"* %array, align 8
  store %"Array<i8>" %"$array", %"Array<i8>"* %tiles, align 8
  store i32 0, i32* %open, align 4
  store i32 0, i32* %i, align 4
  br label %while7

while7:                                           ; preds = %endif, %if8.then, %entry
  %"call.Array<i8>.len" = call i32 @"Array<i8>.len"(%"Array<i8>"* %tiles)
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %"call.Array<i8>.len"
  br i1 %LT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  br label %if8

if8:                                              ; preds = %while7.then
  %"$i1" = load i32, i32* %i, align 4
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %tiles, i32 %"$i1")
  store i8* %"call.Array<i8>.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  %EQ = icmp eq i8 %"$*ref", 35
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  store i32 %ADD, i32* %i, align 4
  br label %while7

endif:                                            ; preds = %if8
  %"$open" = load i32, i32* %open, align 4
  %ADD3 = add i32 %"$open", 1
  store i32 %ADD3, i32* %open, align 4
  %"$i4" = load i32, i32* %i, align 4
  %ADD5 = add i32 %"$i4", 1
  store i32 %ADD5, i32* %i, align 4
  br label %while7

endloop:                                          ; preds = %while7
  %"$open6" = load i32, i32* %open, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$open6")
  call void @"Array<i8>.drop"(%"Array<i8>"* %tiles)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

