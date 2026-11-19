# milestone12

## index

- 001 — nullable: comparing a never-null value to null is rejected
- 002 — nullable: null needs a target type
- 003 — try: a throw is caught in the same function
- 004 — try: a throw propagates across calls to a caller's catch
- 005 — try: a rethrow inside a catch propagates outward
- 006 — try: a nested try: the inner catch handles it
- 007 — try: return inside a try returns from the function
- 008 — try: break inside a try exits the enclosing loop
- 009 — try: an uncaught throw prints its message and exits
- 010 — try: any type can be thrown; a catch takes the type it names
- 011 — try: catch without a matching try is a parse error
- 012 — try: code after a throw is unreachable and dropped

---

## 001 — nullable: comparing a never-null value to null is rejected

```ura
// error: 's1' is never null, so this comparison is always the same

fn main() i32:
    s1 char[] = "hello"
    if s1 != null:
        output("never\n")
    return 0
```

---

## 002 — nullable: null needs a target type

```ura
// error: 'x' can never be null; declare it optional to assign null to it

fn main() i32:
    x char[] = null
    return 0
```

---

## 003 — try: a throw is caught in the same function

```ura
// stdout: caught: boom
// expect: 0
use "@/error"

fn main() i32:
    try:
        throw Error::create("boom")
    catch e Error:
        output("caught: ", e.message, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error = global [1 x i64] zeroinitializer
@ucg.error_type = global i32 0

define %Error @Error.create(i8* %0) {
entry:
  %msg = alloca i8*, align 8
  %e = alloca %Error, align 8
  store i8* %0, i8** %msg, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$msg" = load i8*, i8** %msg, align 8
  store i8* %"$msg", i8** %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  ret %Error %"$e"
}

define i32 @main() {
entry:
  %arr = alloca [5 x i8], align 1
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at8 = alloca %Error*, align 8
  %arr10 = alloca [9 x i8], align 1
  %arr11 = alloca [2 x i8], align 1
  %arr12 = alloca [12 x i8], align 1
  %error.at18 = alloca %Error*, align 8
  %arr20 = alloca [3 x i8], align 1
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif4

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  br i1 true, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  store [5 x i8] c"boom\00", [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  %call.Error.create = call %Error @Error.create(i8* %0)
  br label %if2

if2:                                              ; preds = %if1.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  br label %endif1

endif:                                            ; preds = %if2
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 true, i1* @ucg.thrown, align 1
  br label %endif1

endif1:                                           ; preds = %endif, %if2.then, %if1
  br label %if3

if3:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown7" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown7", %EQ
  br i1 %AND, label %if3.then, label %endif2

if3.then:                                         ; preds = %if3
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at8, align 8
  %"*error.at9" = load %Error*, %Error** %error.at8, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at9", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  store [9 x i8] c"caught: \00", [9 x i8]* %arr10, align 1
  %1 = getelementptr inbounds [9 x i8], [9 x i8]* %arr10, i32 0, i32 0
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr11, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr11, i32 0, i32 0
  %"$e.message" = load i8*, i8** %e.message, align 8
  store [12 x i8] c"caught: %s\0A\00", [12 x i8]* %arr12, align 1
  %3 = getelementptr inbounds [12 x i8], [12 x i8]* %arr12, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i8* %"$e.message")
  br label %endif2

endif2:                                           ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif2
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %if4.then, label %endif3

if4.then:                                         ; preds = %if4
  br label %endif4

endif3:                                           ; preds = %if4
  ret i32 0

endif4:                                           ; preds = %if4.then, %if0
  br label %if5

if5:                                              ; preds = %endif4
  %"$ucg.error_type14" = load i32, i32* @ucg.error_type, align 4
  %EQ15 = icmp eq i32 %"$ucg.error_type14", 1
  %"$ucg.thrown16" = load i1, i1* @ucg.thrown, align 1
  %AND17 = and i1 %"$ucg.thrown16", %EQ15
  br i1 %AND17, label %if5.then, label %endif5

if5.then:                                         ; preds = %if5
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at18, align 8
  %"*error.at19" = load %Error*, %Error** %error.at18, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at19", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr20, align 1
  %4 = getelementptr inbounds [3 x i8], [3 x i8]* %arr20, i32 0, i32 0
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %4, i8* %"$*error.at.message")
  ret i32 1

endif5:                                           ; preds = %if5
  br label %if6

if6:                                              ; preds = %endif5
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %if6.then, label %endif6

if6.then:                                         ; preds = %if6
  ret i32 1

endif6:                                           ; preds = %if6
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 004 — try: a throw propagates across calls to a caller's catch

```ura
// stdout: caught: from deep
// stdout: after
// expect: 0
use "@/error"

fn deep():
    throw Error::create("from deep")

fn middle():
    deep()

fn main() i32:
    try:
        middle()
    catch e Error:
        output("caught: ", e.message, "\n")
    output("after\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error = global [1 x i64] zeroinitializer
@ucg.error_type = global i32 0

define %Error @Error.create(i8* %0) {
entry:
  %msg = alloca i8*, align 8
  %e = alloca %Error, align 8
  store i8* %0, i8** %msg, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$msg" = load i8*, i8** %msg, align 8
  store i8* %"$msg", i8** %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  ret %Error %"$e"
}

define void @deep() {
entry:
  %arr = alloca [10 x i8], align 1
  %error.at = alloca %Error*, align 8
  store [10 x i8] c"from deep\00", [10 x i8]* %arr, align 1
  %0 = getelementptr inbounds [10 x i8], [10 x i8]* %arr, i32 0, i32 0
  %call.Error.create = call %Error @Error.create(i8* %0)
  br label %if0

if0:                                              ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 true, i1* @ucg.thrown, align 1
  ret void
}

define void @middle() {
entry:
  call void @deep()
  br label %if1

if1:                                              ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret void

endif:                                            ; preds = %if1
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %arr = alloca [9 x i8], align 1
  %arr8 = alloca [2 x i8], align 1
  %arr9 = alloca [12 x i8], align 1
  %arr11 = alloca [7 x i8], align 1
  %arr12 = alloca [7 x i8], align 1
  %error.at18 = alloca %Error*, align 8
  %arr20 = alloca [3 x i8], align 1
  br label %if2

if2:                                              ; preds = %entry
  br i1 true, label %if2.then, label %endif4

if2.then:                                         ; preds = %if2
  br label %if3

if3:                                              ; preds = %if2.then
  br i1 true, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  call void @middle()
  br label %if4

if4:                                              ; preds = %if3.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  br label %endif1

endif:                                            ; preds = %if4
  br label %endif1

endif1:                                           ; preds = %endif, %if4.then, %if3
  br label %if5

if5:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown7" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown7", %EQ
  br i1 %AND, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  store [9 x i8] c"caught: \00", [9 x i8]* %arr, align 1
  %0 = getelementptr inbounds [9 x i8], [9 x i8]* %arr, i32 0, i32 0
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr8, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr8, i32 0, i32 0
  %"$e.message" = load i8*, i8** %e.message, align 8
  store [12 x i8] c"caught: %s\0A\00", [12 x i8]* %arr9, align 1
  %2 = getelementptr inbounds [12 x i8], [12 x i8]* %arr9, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i8* %"$e.message")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  br label %if6

if6:                                              ; preds = %endif2
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %if6.then, label %endif3

if6.then:                                         ; preds = %if6
  br label %endif4

endif3:                                           ; preds = %if6
  store [7 x i8] c"after\0A\00", [7 x i8]* %arr11, align 1
  %3 = getelementptr inbounds [7 x i8], [7 x i8]* %arr11, i32 0, i32 0
  store [7 x i8] c"after\0A\00", [7 x i8]* %arr12, align 1
  %4 = getelementptr inbounds [7 x i8], [7 x i8]* %arr12, i32 0, i32 0
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4)
  ret i32 0

endif4:                                           ; preds = %if6.then, %if2
  br label %if7

if7:                                              ; preds = %endif4
  %"$ucg.error_type14" = load i32, i32* @ucg.error_type, align 4
  %EQ15 = icmp eq i32 %"$ucg.error_type14", 1
  %"$ucg.thrown16" = load i1, i1* @ucg.thrown, align 1
  %AND17 = and i1 %"$ucg.thrown16", %EQ15
  br i1 %AND17, label %if7.then, label %endif5

if7.then:                                         ; preds = %if7
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at18, align 8
  %"*error.at19" = load %Error*, %Error** %error.at18, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at19", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr20, align 1
  %5 = getelementptr inbounds [3 x i8], [3 x i8]* %arr20, i32 0, i32 0
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %5, i8* %"$*error.at.message")
  ret i32 1

endif5:                                           ; preds = %if7
  br label %if8

if8:                                              ; preds = %endif5
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %if8.then, label %endif6

if8.then:                                         ; preds = %if8
  ret i32 1

endif6:                                           ; preds = %if8
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 005 — try: a rethrow inside a catch propagates outward

```ura
// stdout: mid: first
// stdout: main: second
// expect: 0
use "@/error"

fn middle():
    try:
        throw Error::create("first")
    catch e Error:
        output("mid: ", e.message, "\n")
        throw Error::create("second")

fn main() i32:
    try:
        middle()
    catch e Error:
        output("main: ", e.message, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error = global [1 x i64] zeroinitializer
@ucg.error_type = global i32 0

define %Error @Error.create(i8* %0) {
entry:
  %msg = alloca i8*, align 8
  %e = alloca %Error, align 8
  store i8* %0, i8** %msg, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$msg" = load i8*, i8** %msg, align 8
  store i8* %"$msg", i8** %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  ret %Error %"$e"
}

define void @middle() {
entry:
  %arr = alloca [6 x i8], align 1
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at6 = alloca %Error*, align 8
  %arr8 = alloca [6 x i8], align 1
  %arr9 = alloca [2 x i8], align 1
  %arr10 = alloca [9 x i8], align 1
  %arr11 = alloca [7 x i8], align 1
  %error.at14 = alloca %Error*, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif1

if0.then:                                         ; preds = %if0
  store [6 x i8] c"first\00", [6 x i8]* %arr, align 1
  %0 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  %call.Error.create = call %Error @Error.create(i8* %0)
  br label %if1

if1:                                              ; preds = %if0.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endif1

endif:                                            ; preds = %if1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 true, i1* @ucg.thrown, align 1
  br label %endif1

endif1:                                           ; preds = %endif, %if1.then, %if0
  br label %if2

if2:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown5" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown5", %EQ
  br i1 %AND, label %if2.then, label %endif3

if2.then:                                         ; preds = %if2
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at6, align 8
  %"*error.at7" = load %Error*, %Error** %error.at6, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at7", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  store [6 x i8] c"mid: \00", [6 x i8]* %arr8, align 1
  %1 = getelementptr inbounds [6 x i8], [6 x i8]* %arr8, i32 0, i32 0
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr9, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr9, i32 0, i32 0
  %"$e.message" = load i8*, i8** %e.message, align 8
  store [9 x i8] c"mid: %s\0A\00", [9 x i8]* %arr10, align 1
  %3 = getelementptr inbounds [9 x i8], [9 x i8]* %arr10, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i8* %"$e.message")
  store [7 x i8] c"second\00", [7 x i8]* %arr11, align 1
  %4 = getelementptr inbounds [7 x i8], [7 x i8]* %arr11, i32 0, i32 0
  %call.Error.create12 = call %Error @Error.create(i8* %4)
  br label %if3

if3:                                              ; preds = %if2.then
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %if3.then, label %endif2

if3.then:                                         ; preds = %if3
  ret void

endif2:                                           ; preds = %if3
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at14, align 8
  %"*error.at15" = load %Error*, %Error** %error.at14, align 8
  store %Error %call.Error.create12, %Error* %"*error.at15", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endif3:                                           ; preds = %if2
  br label %if4

if4:                                              ; preds = %endif3
  %"$ucg.thrown16" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown16", label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  ret void

endif4:                                           ; preds = %if4
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %arr = alloca [7 x i8], align 1
  %arr8 = alloca [2 x i8], align 1
  %arr9 = alloca [10 x i8], align 1
  %error.at15 = alloca %Error*, align 8
  %arr17 = alloca [3 x i8], align 1
  br label %if5

if5:                                              ; preds = %entry
  br i1 true, label %if5.then, label %endif4

if5.then:                                         ; preds = %if5
  br label %if6

if6:                                              ; preds = %if5.then
  br i1 true, label %if6.then, label %endif1

if6.then:                                         ; preds = %if6
  call void @middle()
  br label %if7

if7:                                              ; preds = %if6.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  br label %endif1

endif:                                            ; preds = %if7
  br label %endif1

endif1:                                           ; preds = %endif, %if7.then, %if6
  br label %if8

if8:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown7" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown7", %EQ
  br i1 %AND, label %if8.then, label %endif2

if8.then:                                         ; preds = %if8
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  store [7 x i8] c"main: \00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr8, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr8, i32 0, i32 0
  %"$e.message" = load i8*, i8** %e.message, align 8
  store [10 x i8] c"main: %s\0A\00", [10 x i8]* %arr9, align 1
  %2 = getelementptr inbounds [10 x i8], [10 x i8]* %arr9, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i8* %"$e.message")
  br label %endif2

endif2:                                           ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif2
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %if9.then, label %endif3

if9.then:                                         ; preds = %if9
  br label %endif4

endif3:                                           ; preds = %if9
  ret i32 0

endif4:                                           ; preds = %if9.then, %if5
  br label %if10

if10:                                             ; preds = %endif4
  %"$ucg.error_type11" = load i32, i32* @ucg.error_type, align 4
  %EQ12 = icmp eq i32 %"$ucg.error_type11", 1
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  %AND14 = and i1 %"$ucg.thrown13", %EQ12
  br i1 %AND14, label %if10.then, label %endif5

if10.then:                                        ; preds = %if10
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at15, align 8
  %"*error.at16" = load %Error*, %Error** %error.at15, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at16", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr17, align 1
  %3 = getelementptr inbounds [3 x i8], [3 x i8]* %arr17, i32 0, i32 0
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %3, i8* %"$*error.at.message")
  ret i32 1

endif5:                                           ; preds = %if10
  br label %if11

if11:                                             ; preds = %endif5
  %"$ucg.thrown19" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown19", label %if11.then, label %endif6

if11.then:                                        ; preds = %if11
  ret i32 1

endif6:                                           ; preds = %if11
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 006 — try: a nested try: the inner catch handles it

```ura
// stdout: inner: inner
// expect: 0
use "@/error"

fn main() i32:
    try:
        try:
            throw Error::create("inner")
        catch e Error:
            output("inner: ", e.message, "\n")
    catch e Error:
        output("outer: ", e.message, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error = global [1 x i64] zeroinitializer
@ucg.error_type = global i32 0

define %Error @Error.create(i8* %0) {
entry:
  %msg = alloca i8*, align 8
  %e = alloca %Error, align 8
  store i8* %0, i8** %msg, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$msg" = load i8*, i8** %msg, align 8
  store i8* %"$msg", i8** %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  ret %Error %"$e"
}

define i32 @main() {
entry:
  %arr = alloca [6 x i8], align 1
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at11 = alloca %Error*, align 8
  %arr13 = alloca [8 x i8], align 1
  %arr14 = alloca [2 x i8], align 1
  %arr15 = alloca [11 x i8], align 1
  %e21 = alloca %Error, align 8
  %error.at22 = alloca %Error*, align 8
  %arr25 = alloca [8 x i8], align 1
  %arr27 = alloca [2 x i8], align 1
  %arr29 = alloca [11 x i8], align 1
  %error.at36 = alloca %Error*, align 8
  %arr38 = alloca [3 x i8], align 1
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif7

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  br i1 true, label %if1.then, label %endif4

if1.then:                                         ; preds = %if1
  br label %if2

if2:                                              ; preds = %if1.then
  br i1 true, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  store [6 x i8] c"inner\00", [6 x i8]* %arr, align 1
  %0 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  %call.Error.create = call %Error @Error.create(i8* %0)
  br label %if3

if3:                                              ; preds = %if2.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  br label %endif1

endif:                                            ; preds = %if3
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 true, i1* @ucg.thrown, align 1
  br label %endif1

endif1:                                           ; preds = %endif, %if3.then, %if2
  br label %if4

if4:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown10", %EQ
  br i1 %AND, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at11, align 8
  %"*error.at12" = load %Error*, %Error** %error.at11, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at12", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  store [8 x i8] c"inner: \00", [8 x i8]* %arr13, align 1
  %1 = getelementptr inbounds [8 x i8], [8 x i8]* %arr13, i32 0, i32 0
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr14, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr14, i32 0, i32 0
  %"$e.message" = load i8*, i8** %e.message, align 8
  store [11 x i8] c"inner: %s\0A\00", [11 x i8]* %arr15, align 1
  %3 = getelementptr inbounds [11 x i8], [11 x i8]* %arr15, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i8* %"$e.message")
  br label %endif2

endif2:                                           ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif2
  %"$ucg.thrown16" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown16", label %if5.then, label %endif3

if5.then:                                         ; preds = %if5
  br label %endif4

endif3:                                           ; preds = %if5
  br label %endif4

endif4:                                           ; preds = %endif3, %if5.then, %if1
  br label %if6

if6:                                              ; preds = %endif4
  %"$ucg.error_type17" = load i32, i32* @ucg.error_type, align 4
  %EQ18 = icmp eq i32 %"$ucg.error_type17", 1
  %"$ucg.thrown19" = load i1, i1* @ucg.thrown, align 1
  %AND20 = and i1 %"$ucg.thrown19", %EQ18
  br i1 %AND20, label %if6.then, label %endif5

if6.then:                                         ; preds = %if6
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at22, align 8
  %"*error.at23" = load %Error*, %Error** %error.at22, align 8
  %"$*error.at24" = load %Error, %Error* %"*error.at23", align 8
  store %Error %"$*error.at24", %Error* %e21, align 8
  store [8 x i8] c"outer: \00", [8 x i8]* %arr25, align 1
  %4 = getelementptr inbounds [8 x i8], [8 x i8]* %arr25, i32 0, i32 0
  %e.message26 = getelementptr inbounds %Error, %Error* %e21, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr27, align 1
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %arr27, i32 0, i32 0
  %"$e.message28" = load i8*, i8** %e.message26, align 8
  store [11 x i8] c"outer: %s\0A\00", [11 x i8]* %arr29, align 1
  %6 = getelementptr inbounds [11 x i8], [11 x i8]* %arr29, i32 0, i32 0
  %call.dprintf30 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %6, i8* %"$e.message28")
  br label %endif5

endif5:                                           ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif5
  %"$ucg.thrown31" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown31", label %if7.then, label %endif6

if7.then:                                         ; preds = %if7
  br label %endif7

endif6:                                           ; preds = %if7
  ret i32 0

endif7:                                           ; preds = %if7.then, %if0
  br label %if8

if8:                                              ; preds = %endif7
  %"$ucg.error_type32" = load i32, i32* @ucg.error_type, align 4
  %EQ33 = icmp eq i32 %"$ucg.error_type32", 1
  %"$ucg.thrown34" = load i1, i1* @ucg.thrown, align 1
  %AND35 = and i1 %"$ucg.thrown34", %EQ33
  br i1 %AND35, label %if8.then, label %endif8

if8.then:                                         ; preds = %if8
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at36, align 8
  %"*error.at37" = load %Error*, %Error** %error.at36, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at37", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr38, align 1
  %7 = getelementptr inbounds [3 x i8], [3 x i8]* %arr38, i32 0, i32 0
  %call.dprintf39 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %7, i8* %"$*error.at.message")
  ret i32 1

endif8:                                           ; preds = %if8
  br label %if9

if9:                                              ; preds = %endif8
  %"$ucg.thrown40" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown40", label %if9.then, label %endif9

if9.then:                                         ; preds = %if9
  ret i32 1

endif9:                                           ; preds = %if9
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 007 — try: return inside a try returns from the function

```ura
// stdout: ret = 5
// expect: 0
use "@/error"

fn pick() i32:
    try:
        return 5
    catch e Error:
        return 0

fn main() i32:
    output("ret = ", pick(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error_type = global i32 0
@ucg.error = global [1 x i64] zeroinitializer

define i32 @pick() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 5

endif:                                            ; preds = %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  ret i32 0

endif1:                                           ; preds = %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  ret i32 0

endif2:                                           ; preds = %if2
  ret i32 0
}

define i32 @main() {
entry:
  %arr = alloca [7 x i8], align 1
  %arr4 = alloca [2 x i8], align 1
  %arr5 = alloca [10 x i8], align 1
  %error.at = alloca %Error*, align 8
  %arr7 = alloca [3 x i8], align 1
  br label %if3

if3:                                              ; preds = %entry
  br i1 true, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  store [7 x i8] c"ret = \00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  %call.pick = call i32 @pick()
  br label %if4

if4:                                              ; preds = %if3.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  br label %endif1

endif:                                            ; preds = %if4
  store [2 x i8] c"\0A\00", [2 x i8]* %arr4, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr4, i32 0, i32 0
  store [10 x i8] c"ret = %d\0A\00", [10 x i8]* %arr5, align 1
  %2 = getelementptr inbounds [10 x i8], [10 x i8]* %arr5, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %call.pick)
  ret i32 0

endif1:                                           ; preds = %if4.then, %if3
  br label %if5

if5:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown6", %EQ
  br i1 %AND, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr7, align 1
  %3 = getelementptr inbounds [3 x i8], [3 x i8]* %arr7, i32 0, i32 0
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %3, i8* %"$*error.at.message")
  ret i32 1

endif2:                                           ; preds = %if5
  br label %if6

if6:                                              ; preds = %endif2
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown9", label %if6.then, label %endif3

if6.then:                                         ; preds = %if6
  ret i32 1

endif3:                                           ; preds = %if6
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 008 — try: break inside a try exits the enclosing loop

```ura
// stdout: i=0
// stdout: done
// expect: 0
use "@/error"

fn main() i32:
    i i32 = 0
    while i < 3:
        try:
            if i == 1:
                break
            output("i=", i, "\n")
        catch e Error:
            output("x\n")
        i += 1
    output("done\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error_type = global i32 0
@ucg.error = global [1 x i64] zeroinitializer

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %arr = alloca [3 x i8], align 1
  %arr8 = alloca [2 x i8], align 1
  %arr10 = alloca [6 x i8], align 1
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %arr12 = alloca [3 x i8], align 1
  %arr13 = alloca [3 x i8], align 1
  %arr17 = alloca [6 x i8], align 1
  %arr18 = alloca [6 x i8], align 1
  %error.at24 = alloca %Error*, align 8
  %arr26 = alloca [3 x i8], align 1
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif4

if0.then:                                         ; preds = %if0
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif3, %if0.then
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  br i1 true, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  br label %if3

if3:                                              ; preds = %if2.then
  %"$i7" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i7", 1
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  br label %endloop

endif:                                            ; preds = %if3
  store [3 x i8] c"i=\00", [3 x i8]* %arr, align 1
  %0 = getelementptr inbounds [3 x i8], [3 x i8]* %arr, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr8, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr8, i32 0, i32 0
  %"$i9" = load i32, i32* %i, align 4
  store [6 x i8] c"i=%d\0A\00", [6 x i8]* %arr10, align 1
  %2 = getelementptr inbounds [6 x i8], [6 x i8]* %arr10, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$i9")
  br label %endif1

endif1:                                           ; preds = %endif, %if2
  br label %if4

if4:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ11 = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ11
  br i1 %AND, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  store [3 x i8] c"x\0A\00", [3 x i8]* %arr12, align 1
  %3 = getelementptr inbounds [3 x i8], [3 x i8]* %arr12, i32 0, i32 0
  store [3 x i8] c"x\0A\00", [3 x i8]* %arr13, align 1
  %4 = getelementptr inbounds [3 x i8], [3 x i8]* %arr13, i32 0, i32 0
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4)
  br label %endif2

endif2:                                           ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif2
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %if5.then, label %endif3

if5.then:                                         ; preds = %if5
  br label %endif4

endif3:                                           ; preds = %if5
  %"$i16" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i16", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %if3.then, %while1
  store [6 x i8] c"done\0A\00", [6 x i8]* %arr17, align 1
  %5 = getelementptr inbounds [6 x i8], [6 x i8]* %arr17, i32 0, i32 0
  store [6 x i8] c"done\0A\00", [6 x i8]* %arr18, align 1
  %6 = getelementptr inbounds [6 x i8], [6 x i8]* %arr18, i32 0, i32 0
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %6)
  ret i32 0

endif4:                                           ; preds = %if5.then, %if0
  br label %if6

if6:                                              ; preds = %endif4
  %"$ucg.error_type20" = load i32, i32* @ucg.error_type, align 4
  %EQ21 = icmp eq i32 %"$ucg.error_type20", 1
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  %AND23 = and i1 %"$ucg.thrown22", %EQ21
  br i1 %AND23, label %if6.then, label %endif5

if6.then:                                         ; preds = %if6
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at24, align 8
  %"*error.at25" = load %Error*, %Error** %error.at24, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at25", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr26, align 1
  %7 = getelementptr inbounds [3 x i8], [3 x i8]* %arr26, i32 0, i32 0
  %call.dprintf27 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %7, i8* %"$*error.at.message")
  ret i32 1

endif5:                                           ; preds = %if6
  br label %if7

if7:                                              ; preds = %endif5
  %"$ucg.thrown28" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown28", label %if7.then, label %endif6

if7.then:                                         ; preds = %if7
  ret i32 1

endif6:                                           ; preds = %if7
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 009 — try: an uncaught throw prints its message and exits

```ura
// stderr: uncaught
// expect: 1
use "@/error"

fn main() i32:
    throw Error::create("uncaught\n")
    output("unreachable\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error = global [1 x i64] zeroinitializer
@ucg.error_type = global i32 0

define %Error @Error.create(i8* %0) {
entry:
  %msg = alloca i8*, align 8
  %e = alloca %Error, align 8
  store i8* %0, i8** %msg, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$msg" = load i8*, i8** %msg, align 8
  store i8* %"$msg", i8** %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  ret %Error %"$e"
}

define i32 @main() {
entry:
  %arr = alloca [10 x i8], align 1
  %error.at = alloca %Error*, align 8
  %error.at5 = alloca %Error*, align 8
  %arr7 = alloca [3 x i8], align 1
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif1

if0.then:                                         ; preds = %if0
  store [10 x i8] c"uncaught\0A\00", [10 x i8]* %arr, align 1
  %0 = getelementptr inbounds [10 x i8], [10 x i8]* %arr, i32 0, i32 0
  %call.Error.create = call %Error @Error.create(i8* %0)
  br label %if1

if1:                                              ; preds = %if0.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endif1

endif:                                            ; preds = %if1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 true, i1* @ucg.thrown, align 1
  br label %endif1

endif1:                                           ; preds = %endif, %if1.then, %if0
  br label %if2

if2:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at5, align 8
  %"*error.at6" = load %Error*, %Error** %error.at5, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at6", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr7, align 1
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %arr7, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %1, i8* %"$*error.at.message")
  ret i32 1

endif2:                                           ; preds = %if2
  br label %if3

if3:                                              ; preds = %endif2
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown8", label %if3.then, label %endif3

if3.then:                                         ; preds = %if3
  ret i32 1

endif3:                                           ; preds = %if3
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 010 — try: any type can be thrown; a catch takes the type it names

```ura
// stdout: code 42
// stdout: after
// expect: 0
use "@/error"

fn fail():
    throw 42

fn main() i32:
    try:
        fail()
    catch e Error:
        output("error ", e.message, "\n")
    catch code i32:
        output("code ", code, "\n")
    output("after\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Error = type { i8* }

@ucg.thrown = global i1 false
@ucg.error = global [1 x i64] zeroinitializer
@ucg.error_type = global i32 0

define void @fail() {
entry:
  %error.at = alloca i32*, align 8
  store i32* bitcast ([1 x i64]* @ucg.error to i32*), i32** %error.at, align 8
  %"*error.at" = load i32*, i32** %error.at, align 8
  store i32 42, i32* %"*error.at", align 4
  store i32 1, i32* @ucg.error_type, align 4
  store i1 true, i1* @ucg.thrown, align 1
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %arr = alloca [7 x i8], align 1
  %arr8 = alloca [2 x i8], align 1
  %arr9 = alloca [10 x i8], align 1
  %code = alloca i32, align 4
  %error.at14 = alloca i32*, align 8
  %arr17 = alloca [6 x i8], align 1
  %arr18 = alloca [2 x i8], align 1
  %arr19 = alloca [9 x i8], align 1
  %arr22 = alloca [7 x i8], align 1
  %arr23 = alloca [7 x i8], align 1
  %error.at29 = alloca %Error*, align 8
  %arr31 = alloca [3 x i8], align 1
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif4

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  br i1 true, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  call void @fail()
  br label %if2

if2:                                              ; preds = %if1.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  br label %endif1

endif:                                            ; preds = %if2
  br label %endif1

endif1:                                           ; preds = %endif, %if2.then, %if1
  br label %if3

if3:                                              ; preds = %endif1
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 2
  %"$ucg.thrown7" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown7", %EQ
  br i1 %AND, label %if3.then, label %elif4

if3.then:                                         ; preds = %if3
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  store [7 x i8] c"error \00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr8, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr8, i32 0, i32 0
  %"$e.message" = load i8*, i8** %e.message, align 8
  store [10 x i8] c"error %s\0A\00", [10 x i8]* %arr9, align 1
  %2 = getelementptr inbounds [10 x i8], [10 x i8]* %arr9, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i8* %"$e.message")
  br label %endif2

elif4:                                            ; preds = %if3
  %"$ucg.error_type10" = load i32, i32* @ucg.error_type, align 4
  %EQ11 = icmp eq i32 %"$ucg.error_type10", 1
  %"$ucg.thrown12" = load i1, i1* @ucg.thrown, align 1
  %AND13 = and i1 %"$ucg.thrown12", %EQ11
  br i1 %AND13, label %elif4.then, label %endif2

elif4.then:                                       ; preds = %elif4
  store i1 false, i1* @ucg.thrown, align 1
  store i32* bitcast ([1 x i64]* @ucg.error to i32*), i32** %error.at14, align 8
  %"*error.at15" = load i32*, i32** %error.at14, align 8
  %"$*error.at16" = load i32, i32* %"*error.at15", align 4
  store i32 %"$*error.at16", i32* %code, align 4
  store [6 x i8] c"code \00", [6 x i8]* %arr17, align 1
  %3 = getelementptr inbounds [6 x i8], [6 x i8]* %arr17, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr18, align 1
  %4 = getelementptr inbounds [2 x i8], [2 x i8]* %arr18, i32 0, i32 0
  %"$code" = load i32, i32* %code, align 4
  store [9 x i8] c"code %d\0A\00", [9 x i8]* %arr19, align 1
  %5 = getelementptr inbounds [9 x i8], [9 x i8]* %arr19, i32 0, i32 0
  %call.dprintf20 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %5, i32 %"$code")
  br label %endif2

endif2:                                           ; preds = %elif4.then, %elif4, %if3.then
  br label %if5

if5:                                              ; preds = %endif2
  %"$ucg.thrown21" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown21", label %if5.then, label %endif3

if5.then:                                         ; preds = %if5
  br label %endif4

endif3:                                           ; preds = %if5
  store [7 x i8] c"after\0A\00", [7 x i8]* %arr22, align 1
  %6 = getelementptr inbounds [7 x i8], [7 x i8]* %arr22, i32 0, i32 0
  store [7 x i8] c"after\0A\00", [7 x i8]* %arr23, align 1
  %7 = getelementptr inbounds [7 x i8], [7 x i8]* %arr23, i32 0, i32 0
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %7)
  ret i32 0

endif4:                                           ; preds = %if5.then, %if0
  br label %if6

if6:                                              ; preds = %endif4
  %"$ucg.error_type25" = load i32, i32* @ucg.error_type, align 4
  %EQ26 = icmp eq i32 %"$ucg.error_type25", 2
  %"$ucg.thrown27" = load i1, i1* @ucg.thrown, align 1
  %AND28 = and i1 %"$ucg.thrown27", %EQ26
  br i1 %AND28, label %if6.then, label %endif5

if6.then:                                         ; preds = %if6
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([1 x i64]* @ucg.error to %Error*), %Error** %error.at29, align 8
  %"*error.at30" = load %Error*, %Error** %error.at29, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at30", i32 0, i32 0
  %"$*error.at.message" = load i8*, i8** %"*error.at.message", align 8
  store [3 x i8] c"%s\00", [3 x i8]* %arr31, align 1
  %8 = getelementptr inbounds [3 x i8], [3 x i8]* %arr31, i32 0, i32 0
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %8, i8* %"$*error.at.message")
  ret i32 1

endif5:                                           ; preds = %if6
  br label %if7

if7:                                              ; preds = %endif5
  %"$ucg.thrown33" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown33", label %if7.then, label %endif6

if7.then:                                         ; preds = %if7
  ret i32 1

endif6:                                           ; preds = %if7
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 011 — try: catch without a matching try is a parse error

```ura
// error: 'catch' without a matching 'try'
fn main() i32:
    catch e Error:
        output("x\n")
    return 0
```

### stderr

```
error: test.ura:3:5 'catch' without a matching 'try'
  |
3 |     catch e Error:
  |     ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 012 — try: code after a throw is unreachable and dropped

```ura
// stdout: caught: x
// expect: 0
use "@/error"

fn boom():
    throw Error::create("x")
    output("dead\n")

fn main() i32:
    try:
        boom()
    catch e Error:
        output("caught: ", e.message, "\n")
    return 0
```

