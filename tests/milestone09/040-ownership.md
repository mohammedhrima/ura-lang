# milestone09

## index

- 040 — ownership: nested arrays, an Array attribute, reassignment, drop(x), returning an attribute copies
- 041 — ownership: operator = taking the other side by value

---

## 040 — ownership: nested arrays, an Array attribute, reassignment, drop(x), returning an attribute copies

```ura
struct Bag:
    items Array<i32>
    label i32

    fn first_items() Array<i32>:
        return self.items

fn make(n i32) Array<i32>:
    a Array<i32>
    i i32 = 0
    while i < n:
        a.append(i)
        i += 1
    return a

fn total(a Array<i32>) i32:
    sum i32 = 0
    i i32 = 0
    while i < a.size():
        sum += a[i]
        i += 1
    return sum

fn main() i32:
    grid Array<Array<i32>>
    row Array<i32> = make(3)
    grid.append(row)
    grid.append(make(2))
    grid[0].append(9)
    copy Array<i32> = grid[0]
    copy.append(5)
    output(grid.size(), " ", grid[0].size(), " ", grid[1].size(), " ", copy.size(), " ", row.size(), "\n")
    bag Bag
    bag.items = make(4)
    bag.items = make(2)
    items Array<i32> = bag.first_items()
    items.append(7)
    output(bag.items.size(), " ", items.size(), " ", total(items), " ", total(grid[0]), "\n")
    popped Array<i32> = grid.pop()
    drop(row)
    output(popped.size(), " ", row.size(), " ", grid.size(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i32>" = type { i32*, i32, i32 }
%Bag = type { %"Array<i32>", i32 }
%"Array<Array<i32>>" = type { %"Array<i32>"*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"%d %d %d %d %d\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"%d %d %d %d\0A\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

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

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %"*self.data[]", align 4
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
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
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %"*other2" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
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
  br label %while7

endloop:                                          ; preds = %while7
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"*other11" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if8

if8:                                              ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  ret void
}

define %"Array<i32>" @Bag.first_items(%Bag* %0) {
entry:
  %self = alloca %Bag*, align 8
  %copy = alloca %"Array<i32>", align 8
  store %Bag* %0, %Bag** %self, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  %"*self" = load %Bag*, %Bag** %self, align 8
  %"*self.items" = getelementptr inbounds %Bag, %Bag* %"*self", i32 0, i32 0
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %"*self.items")
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  ret %"Array<i32>" %"$copy"
}

define %"Array<i32>" @make(i32 %0) {
entry:
  %n = alloca i32, align 4
  %a = alloca %"Array<i32>", align 8
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %a, align 8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  %"$i1" = load i32, i32* %i, align 4
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 %"$i1")
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  store i32 %ADD, i32* %i, align 4
  br label %while9

endloop:                                          ; preds = %while9
  %"$a" = load %"Array<i32>", %"Array<i32>"* %a, align 8
  ret %"Array<i32>" %"$a"
}

define i32 @total(%"Array<i32>" %0) {
entry:
  %a = alloca %"Array<i32>", align 8
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %ref = alloca i32*, align 8
  store %"Array<i32>" %0, %"Array<i32>"* %a, align 8
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %while10

while10:                                          ; preds = %while10.then, %entry
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %"call.Array<i32>.size"
  br i1 %LT, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"$i1" = load i32, i32* %i, align 4
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 %"$i1")
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  %"$*ref" = load i32, i32* %"*ref", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*ref"
  store i32 %ADD, i32* %sum, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while10

endloop:                                          ; preds = %while10
  %"$sum4" = load i32, i32* %sum, align 4
  call void @"Array<i32>.drop"(%"Array<i32>"* %a)
  ret i32 %"$sum4"
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

define void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  ret void
}

define void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if15

if15:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if15.then, %if15
  br label %while16

while16:                                          ; preds = %while16.then, %endif
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
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
  br label %while16

endloop:                                          ; preds = %while16
  ret void
}

define void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %0, %"Array<i32>" %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %value = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store %"Array<i32>" %1, %"Array<i32>"* %value, align 8
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if17.then, %if17
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

define %"Array<i32>" @"Array<Array<i32>>.pop"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %index = alloca i32, align 4
  %value = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %i = alloca i32, align 4
  %copy15 = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %value, align 8
  br label %if18

if18:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"$value" = load %"Array<i32>", %"Array<i32>"* %value, align 8
  ret %"Array<i32>" %"$value"

endif:                                            ; preds = %if18
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %"*self2")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$index4"
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %"*self.data[]")
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %value)
  store %"Array<i32>" %"$copy", %"Array<i32>"* %value, align 8
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while19

while19:                                          ; preds = %while19.then, %endif
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 0
  %"$*self.data12" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data12", i32 %"$i13"
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy15, align 8
  %"*self16" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self16", i32 0, i32 0
  %"$i18" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i18", 1
  %"$*self.data19" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data17", align 8
  %"*self.data[]20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data19", i32 %ADD
  call void @"Array<i32>.="(%"Array<i32>"* %copy15, %"Array<i32>"* %"*self.data[]20")
  %"$copy21" = load %"Array<i32>", %"Array<i32>"* %copy15, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]14")
  store %"Array<i32>" %"$copy21", %"Array<i32>"* %"*self.data[]14", align 8
  %"$i22" = load i32, i32* %i, align 4
  %ADD23 = add i32 %"$i22", 1
  store i32 %ADD23, i32* %i, align 4
  br label %while19

endloop:                                          ; preds = %while19
  %"*self24" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count25" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self24", i32 0, i32 1
  %"$*self.count26" = load i32, i32* %"*self.count25", align 4
  %SUB27 = sub i32 %"$*self.count26", 1
  store i32 %SUB27, i32* %"*self.count25", align 4
  %"*self28" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data29" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self28", i32 0, i32 0
  %"*self30" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count31" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self30", i32 0, i32 1
  %"$*self.data32" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data29", align 8
  %"$*self.count33" = load i32, i32* %"*self.count31", align 4
  %"*self.data[]34" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data32", i32 %"$*self.count33"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]34")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]34", align 8
  %"$value35" = load %"Array<i32>", %"Array<i32>"* %value, align 8
  ret %"Array<i32>" %"$value35"
}

define %"Array<i32>" @"Array<Array<i32>>.pop.1"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"call.Array<Array<i32>>.pop" = call %"Array<i32>" @"Array<Array<i32>>.pop"(%"Array<Array<i32>>"* %"*self", i32 %SUB)
  ret %"Array<i32>" %"call.Array<Array<i32>>.pop"
}

define i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %"*self")
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$index"
  ret %"Array<i32>"* %"*self.data[]"
}

define void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %"*self")
  br label %if20

if20:                                             ; preds = %entry
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  ret void
}

define i32 @main() {
entry:
  %grid = alloca %"Array<Array<i32>>", align 8
  %row = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %ref = alloca %"Array<i32>"*, align 8
  %copy2 = alloca %"Array<i32>", align 8
  %copy3 = alloca %"Array<i32>", align 8
  %ref5 = alloca %"Array<i32>"*, align 8
  %ref9 = alloca %"Array<i32>"*, align 8
  %ref12 = alloca %"Array<i32>"*, align 8
  %bag = alloca %Bag, align 8
  %items = alloca %"Array<i32>", align 8
  %copy23 = alloca %"Array<i32>", align 8
  %copy25 = alloca %"Array<i32>", align 8
  %ref27 = alloca %"Array<i32>"*, align 8
  %popped = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>" zeroinitializer, %"Array<Array<i32>>"* %grid, align 8
  %call.make = call %"Array<i32>" @make(i32 3)
  store %"Array<i32>" %call.make, %"Array<i32>"* %row, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %row)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %grid, %"Array<i32>" %"$copy")
  %call.make1 = call %"Array<i32>" @make(i32 2)
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %grid, %"Array<i32>" %call.make1)
  %"call.Array<Array<i32>>.[]" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]", %"Array<i32>"** %ref, align 8
  %"*ref" = load %"Array<i32>"*, %"Array<i32>"** %ref, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %"*ref", i32 9)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy3, align 8
  %"call.Array<Array<i32>>.[]4" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]4", %"Array<i32>"** %ref5, align 8
  %"*ref6" = load %"Array<i32>"*, %"Array<i32>"** %ref5, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy3, %"Array<i32>"* %"*ref6")
  %"$copy7" = load %"Array<i32>", %"Array<i32>"* %copy3, align 8
  store %"Array<i32>" %"$copy7", %"Array<i32>"* %copy2, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %copy2, i32 5)
  %"call.Array<Array<i32>>.size" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %"call.Array<Array<i32>>.[]8" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]8", %"Array<i32>"** %ref9, align 8
  %"*ref10" = load %"Array<i32>"*, %"Array<i32>"** %ref9, align 8
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %"*ref10")
  %"call.Array<Array<i32>>.[]11" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 1)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]11", %"Array<i32>"** %ref12, align 8
  %"*ref13" = load %"Array<i32>"*, %"Array<i32>"** %ref12, align 8
  %"call.Array<i32>.size14" = call i32 @"Array<i32>.size"(%"Array<i32>"* %"*ref13")
  %"call.Array<i32>.size15" = call i32 @"Array<i32>.size"(%"Array<i32>"* %copy2)
  %"call.Array<i32>.size16" = call i32 @"Array<i32>.size"(%"Array<i32>"* %row)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"call.Array<Array<i32>>.size", i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"call.Array<i32>.size15", i32 %"call.Array<i32>.size16")
  store %Bag zeroinitializer, %Bag* %bag, align 8
  %bag.items = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %call.make17 = call %"Array<i32>" @make(i32 4)
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items)
  store %"Array<i32>" %call.make17, %"Array<i32>"* %bag.items, align 8
  %bag.items18 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %call.make19 = call %"Array<i32>" @make(i32 2)
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items18)
  store %"Array<i32>" %call.make19, %"Array<i32>"* %bag.items18, align 8
  %call.Bag.first_items = call %"Array<i32>" @Bag.first_items(%Bag* %bag)
  store %"Array<i32>" %call.Bag.first_items, %"Array<i32>"* %items, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %items, i32 7)
  %bag.items20 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %"call.Array<i32>.size21" = call i32 @"Array<i32>.size"(%"Array<i32>"* %bag.items20)
  %"call.Array<i32>.size22" = call i32 @"Array<i32>.size"(%"Array<i32>"* %items)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy23, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy23, %"Array<i32>"* %items)
  %"$copy24" = load %"Array<i32>", %"Array<i32>"* %copy23, align 8
  %call.total = call i32 @total(%"Array<i32>" %"$copy24")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy25, align 8
  %"call.Array<Array<i32>>.[]26" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]26", %"Array<i32>"** %ref27, align 8
  %"*ref28" = load %"Array<i32>"*, %"Array<i32>"** %ref27, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy25, %"Array<i32>"* %"*ref28")
  %"$copy29" = load %"Array<i32>", %"Array<i32>"* %copy25, align 8
  %call.total30 = call i32 @total(%"Array<i32>" %"$copy29")
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size21", i32 %"call.Array<i32>.size22", i32 %call.total, i32 %call.total30)
  %"call.Array<Array<i32>>.pop" = call %"Array<i32>" @"Array<Array<i32>>.pop.1"(%"Array<Array<i32>>"* %grid)
  store %"Array<i32>" %"call.Array<Array<i32>>.pop", %"Array<i32>"* %popped, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %row)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %row, align 8
  %"call.Array<i32>.size32" = call i32 @"Array<i32>.size"(%"Array<i32>"* %popped)
  %"call.Array<i32>.size33" = call i32 @"Array<i32>.size"(%"Array<i32>"* %row)
  %"call.Array<Array<i32>>.size34" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %call.dprintf35 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 %"call.Array<i32>.size32", i32 %"call.Array<i32>.size33", i32 %"call.Array<Array<i32>>.size34")
  call void @"Array<i32>.drop"(%"Array<i32>"* %popped)
  call void @"Array<i32>.drop"(%"Array<i32>"* %items)
  %bag.items36 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items36)
  call void @"Array<i32>.drop"(%"Array<i32>"* %copy2)
  call void @"Array<i32>.drop"(%"Array<i32>"* %row)
  call void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %grid)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 041 — ownership: operator = taking the other side by value

```ura
struct Vec:
    x i32

    operator =(other Vec):
        self.x = other.x

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:4:5 operator = takes the other side by reference, a copy would need itself
  |
4 |     operator =(other Vec):
  |     ^^^^^^^^
help: use operator =(other &Vec)
error: aborting due to 1 error
```

### status

```
1
```

