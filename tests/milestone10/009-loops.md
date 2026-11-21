# milestone10

## index

- 009 — loop: Range::create with a step
- 012 — loop: break and continue inside a for
- 013 — loop: continue: skip wall tiles '#', count open spaces

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

