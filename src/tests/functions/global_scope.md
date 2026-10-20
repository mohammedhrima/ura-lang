# global_scope

## index

- 001 — no main: the file is the program

---

## 001 — no main: the file is the program

```ura
// expect: 40   (23 at 4 * 10, 4 at 0, 99 missing: 0)
struct Finder:
    data i32[]
    size i32

    fn index_of(v i32) i32:
        i i32 = 0
        while True:
            if i == self.size: break
            if self.data[i] == v: return i
            i += 1
        return -1

f Finder
f.data = [4, 8, 15, 16, 23, 42]
f.size = 6
return f.index_of(23) * 10 + f.index_of(4) + (f.index_of(99) + 1) * 100
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Finder = type { i32*, i32 }

@f = global %Finder zeroinitializer

define i32 @Finder.index_of(%Finder* %0, i32 %1) {
entry:
  %self = alloca %Finder*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  store %Finder* %0, %Finder** %self, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif2, %entry
  br i1 true, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i = load i32, i32* %i, align 4
  %dref.self = load %Finder*, %Finder** %self, align 8
  %load.dref.self.size = getelementptr inbounds %Finder, %Finder* %dref.self, i32 0, i32 1
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %EQ = icmp eq i32 %load.i, %load.load.dref.self.size
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then1, %cond
  ret i32 -1

then1:                                            ; preds = %then
  br label %endwhile

endif:                                            ; preds = %then
  %dref.self4 = load %Finder*, %Finder** %self, align 8
  %load.dref.self.data = getelementptr inbounds %Finder, %Finder* %dref.self4, i32 0, i32 0
  %load.load.dref.self.data = load i32*, i32** %load.dref.self.data, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.load.dref.self.data[load.i]" = getelementptr inbounds i32, i32* %load.load.dref.self.data, i32 %load.i5
  %"load.load.load.dref.self.data[load.i]" = load i32, i32* %"load.load.dref.self.data[load.i]", align 4
  %load.v = load i32, i32* %v, align 4
  %EQ6 = icmp eq i32 %"load.load.load.dref.self.data[load.i]", %load.v
  br i1 %EQ6, label %then3, label %endif2

then3:                                            ; preds = %endif
  %load.i7 = load i32, i32* %i, align 4
  ret i32 %load.i7

endif2:                                           ; preds = %endif
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %array = alloca [6 x i32], align 4
  store %Finder zeroinitializer, %Finder* @f, align 8
  %e = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 0
  store i32 4, i32* %e, align 4
  %e1 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 1
  store i32 8, i32* %e1, align 4
  %e2 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 2
  store i32 15, i32* %e2, align 4
  %e3 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 3
  store i32 16, i32* %e3, align 4
  %e4 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 4
  store i32 23, i32* %e4, align 4
  %e5 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 5
  store i32 42, i32* %e5, align 4
  %array6 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 0
  store i32* %array6, i32** getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 0), align 8
  store i32 6, i32* getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 1), align 4
  %"index_of(&load.f,23)" = call i32 @Finder.index_of(%Finder* @f, i32 23)
  %MUL = mul i32 %"index_of(&load.f,23)", 10
  %"index_of(&load.f,4)" = call i32 @Finder.index_of(%Finder* @f, i32 4)
  %ADD = add i32 %MUL, %"index_of(&load.f,4)"
  %"index_of(&load.f,99)" = call i32 @Finder.index_of(%Finder* @f, i32 99)
  %ADD7 = add i32 %"index_of(&load.f,99)", 1
  %MUL8 = mul i32 %ADD7, 100
  %ADD9 = add i32 %ADD, %MUL8
  ret i32 %ADD9
}
```

