# milestone01

## index

- 220 — operator: all arithmetic ops in dungeon combat context
- 221 — operator: arithmetic with precedence (2*3 + 20/4 - 1)
- 222 — operator: reassign an existing variable (2*10 + 3)
- 223 — operator: modulo
- 224 — operator: all comparison ops on hero/enemy stats
- 225 — operator: comparisons produce bools (with precedence)
- 226 — operator: compound assignment += -= *= /= %=
- 227 — operator: logical ops: &&, ||, !, and, or, not
- 228 — operator: and / or with comparisons and precedence
- 229 — operator: type mismatch between operands
- 230 — operator: a comparison result is a bool, not an i32
- 231 — variable: hero stats: declare all common types
- 232 — variable: reassignment: hp loss, xp gain, mp drain
- 233 — variable: all primitive types in one place
- 234 — variable: a use before a shadowing declaration binds to the outer one
- 235 — variable: redeclaring a variable in the same scope
- 236 — variable: using a name that was never declared
- 237 — variable: assigning to a name that was never declared
- 238 — variable: a variable does not outlive the block that declared it
- 239 — variable: redeclaring a global
- 240 — variable: a bool takes only True or False
- 241 — variable: assigning a string or a struct to an integer
- 242 — proto: a string passed to a fn with an i8[] param
- 243 — variables: unary minus on a variable and a parameter
- 244 — global scope: a top-level statement runs before main
- 245 — function: early bare return in a void function
- 246 — bool: i1 and bool are the same type
- 247 — tokenizer: a block comment over two lines, /*/ does not close it
- 248 — calls: arguments over several lines
- 249 — variables: a number literal takes the type of its i8 target
- 250 — variables: a variable first set inside an if still starts at zero
- 251 — variables: a variable with the name of a parameter
- 252 — unsigned: u8 wraps, u32 divides and compares unsigned, u64, casts
- 253 — unsigned: a number that doesn't fit in u8
- 254 — variables: the type written before the name
- 255 — bool: not, with and / or and in if / elif
- 256 — bool: not on an i32
- 257 — float: 2.5f is an f32
- 258 — float: a negative f64 literal into an f32

---

## 220 — operator: all arithmetic ops in dungeon combat context

```ura
// stdout: atk + def:       33
// stdout: atk - def:       17
// stdout: kills * 50:      150
// stdout: total / count:   25
// stdout: atk % xp_cap:    25
// stdout: 10 + 5:          15
// stdout: base_atk + atk:  35
// stdout: 2 + 3 * 4:       14
// stdout: (2 + 3) * 4:     20
// stdout: add:  1.800000
// stdout: sub:  1.200000
// stdout: mul:  0.450000
// stdout: div:  5.000000
// stdout: lt:   False
// stdout: gt:   True
// stdout: lte:  False
// stdout: gte:  True
// stdout: clamp(0.52): 0.520000
// stdout: clamp(0.01): 0.050000
// expect: 0

fn base_atk() i32:
    return 10

fn clamp(t f32) f32:
    if t < 0.05:
        return 0.05
    return t

fn main() i32:
    atk     i32 = 25
    def     i32 = 8
    kills   i32 = 3
    total   i32 = 100
    count   i32 = 4
    xp_cap  i32 = 70

    // var + var
    io::out($"atk + def:       {atk + def}\n")
    // var - var
    io::out($"atk - def:       {atk - def}\n")
    // var * var
    io::out($"kills * 50:      {kills * 50}\n")
    // var / var
    io::out($"total / count:   {total / count}\n")
    // var % var
    io::out($"atk % xp_cap:    {atk % xp_cap}\n")
    // val + val
    io::out($"10 + 5:          {10 + 5}\n")
    // fn + var
    io::out($"base_atk + atk:  {base_atk() + atk}\n")
    // chained with precedence
    io::out($"2 + 3 * 4:       {2 + 3 * 4}\n")
    io::out($"(2 + 3) * 4:     {(2 + 3) * 4}\n")

    a f32 = 1.5
    b f32 = 0.3

    io::out($"add:  {a + b}\n")  // 1.8
    io::out($"sub:  {a - b}\n")  // 1.2
    io::out($"mul:  {a * b}\n")  // 0.45
    io::out($"div:  {a / b}\n")  // 5.0

    io::out($"lt:   {a < b}\n")  // 0 (false)
    io::out($"gt:   {a > b}\n")  // 1 (true)
    io::out($"lte:  {a <= b}\n")  // 0
    io::out($"gte:  {a >= b}\n")  // 1

    t f32 = 0.8 - 0.28
    io::out($"clamp(0.52): {clamp(t)}\n")  // 0.52
    io::out($"clamp(0.01): {clamp(0.01)}\n")  // 0.05
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [21 x i8] c"atk + def:       %d\0A\00", align 1
@str.1 = private unnamed_addr constant [21 x i8] c"atk - def:       %d\0A\00", align 1
@str.2 = private unnamed_addr constant [21 x i8] c"kills * 50:      %d\0A\00", align 1
@str.3 = private unnamed_addr constant [21 x i8] c"total / count:   %d\0A\00", align 1
@str.4 = private unnamed_addr constant [22 x i8] c"atk %% xp_cap:    %d\0A\00", align 1
@str.5 = private unnamed_addr constant [21 x i8] c"10 + 5:          %d\0A\00", align 1
@str.6 = private unnamed_addr constant [21 x i8] c"base_atk + atk:  %d\0A\00", align 1
@str.7 = private unnamed_addr constant [21 x i8] c"2 + 3 * 4:       %d\0A\00", align 1
@str.8 = private unnamed_addr constant [21 x i8] c"(2 + 3) * 4:     %d\0A\00", align 1
@str.9 = private unnamed_addr constant [10 x i8] c"add:  %f\0A\00", align 1
@str.10 = private unnamed_addr constant [10 x i8] c"sub:  %f\0A\00", align 1
@str.11 = private unnamed_addr constant [10 x i8] c"mul:  %f\0A\00", align 1
@str.12 = private unnamed_addr constant [10 x i8] c"div:  %f\0A\00", align 1
@str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.15 = private unnamed_addr constant [10 x i8] c"lt:   %s\0A\00", align 1
@str.16 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.18 = private unnamed_addr constant [10 x i8] c"gt:   %s\0A\00", align 1
@str.19 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.20 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.21 = private unnamed_addr constant [10 x i8] c"lte:  %s\0A\00", align 1
@str.22 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.23 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.24 = private unnamed_addr constant [10 x i8] c"gte:  %s\0A\00", align 1
@str.25 = private unnamed_addr constant [17 x i8] c"clamp(0.52): %f\0A\00", align 1
@str.26 = private unnamed_addr constant [17 x i8] c"clamp(0.01): %f\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @base_atk() {
entry:
  ret i32 10
}

define float @clamp(float %0) {
entry:
  %t = alloca float, align 4
  store float %0, float* %t, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$t" = load float, float* %t, align 4
  %LT = fcmp olt float %"$t", 0x3FA99999A0000000
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  ret float 0x3FA99999A0000000

endif:                                            ; preds = %if6
  %"$t1" = load float, float* %t, align 4
  ret float %"$t1"
}

define i32 @main() {
entry:
  %atk = alloca i32, align 4
  %def = alloca i32, align 4
  %kills = alloca i32, align 4
  %total = alloca i32, align 4
  %count = alloca i32, align 4
  %xp_cap = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size12 = alloca i32, align 4
  %format.text17 = alloca i8*, align 8
  %format.size25 = alloca i32, align 4
  %format.text30 = alloca i8*, align 8
  %format.size38 = alloca i32, align 4
  %format.text43 = alloca i8*, align 8
  %format.size52 = alloca i32, align 4
  %format.text57 = alloca i8*, align 8
  %format.size65 = alloca i32, align 4
  %format.text70 = alloca i8*, align 8
  %format.size80 = alloca i32, align 4
  %format.text85 = alloca i8*, align 8
  %format.size93 = alloca i32, align 4
  %format.text98 = alloca i8*, align 8
  %format.size106 = alloca i32, align 4
  %format.text111 = alloca i8*, align 8
  %a = alloca float, align 4
  %b = alloca float, align 4
  %format.size120 = alloca i32, align 4
  %format.text126 = alloca i8*, align 8
  %format.size137 = alloca i32, align 4
  %format.text143 = alloca i8*, align 8
  %format.size154 = alloca i32, align 4
  %format.text160 = alloca i8*, align 8
  %format.size171 = alloca i32, align 4
  %format.text177 = alloca i8*, align 8
  %format.size187 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text192 = alloca i8*, align 8
  %format.size203 = alloca i32, align 4
  %bool.text204 = alloca i8*, align 8
  %format.text210 = alloca i8*, align 8
  %format.size221 = alloca i32, align 4
  %bool.text222 = alloca i8*, align 8
  %format.text228 = alloca i8*, align 8
  %format.size239 = alloca i32, align 4
  %bool.text240 = alloca i8*, align 8
  %format.text246 = alloca i8*, align 8
  %t = alloca float, align 4
  %format.size255 = alloca i32, align 4
  %format.text261 = alloca i8*, align 8
  %format.size270 = alloca i32, align 4
  %format.text276 = alloca i8*, align 8
  store i32 25, i32* %atk, align 4
  store i32 8, i32* %def, align 4
  store i32 3, i32* %kills, align 4
  store i32 100, i32* %total, align 4
  store i32 4, i32* %count, align 4
  store i32 70, i32* %xp_cap, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %"$def" = load i32, i32* %def, align 4
  %ADD = add i32 %"$atk", %"$def"
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %ADD)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD4 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %ADD)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  %"$atk10" = load i32, i32* %atk, align 4
  %"$def11" = load i32, i32* %def, align 4
  %SUB = sub i32 %"$atk10", %"$def11"
  %call.snprintf13 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  store i32 %call.snprintf13, i32* %format.size12, align 4
  %"$format.size14" = load i32, i32* %format.size12, align 4
  %ADD15 = add i32 %"$format.size14", 1
  %AS16 = sext i32 %ADD15 to i64
  %call.calloc18 = call i8* @calloc(i64 %AS16, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc18, i8** %format.text17, align 8
  %"$format.text19" = load i8*, i8** %format.text17, align 8
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text19", i64 %AS16, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  %"$format.size21" = load i32, i32* %format.size12, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %"$format.text23" = load i8*, i8** %format.text17, align 8
  %"$format.size24" = load i32, i32* %format.size12, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text23", 0
  %4 = insertvalue %String %3, i32 %"$format.size24", 1
  %5 = insertvalue %String %4, i32 %ADD22, 2
  call void @"io::out"(%String %5)
  %"$kills" = load i32, i32* %kills, align 4
  %MUL = mul i32 %"$kills", 50
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  store i32 %call.snprintf26, i32* %format.size25, align 4
  %"$format.size27" = load i32, i32* %format.size25, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %AS29 = sext i32 %ADD28 to i64
  %call.calloc31 = call i8* @calloc(i64 %AS29, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc31, i8** %format.text30, align 8
  %"$format.text32" = load i8*, i8** %format.text30, align 8
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text32", i64 %AS29, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  %"$format.size34" = load i32, i32* %format.size25, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %"$format.text36" = load i8*, i8** %format.text30, align 8
  %"$format.size37" = load i32, i32* %format.size25, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text36", 0
  %7 = insertvalue %String %6, i32 %"$format.size37", 1
  %8 = insertvalue %String %7, i32 %ADD35, 2
  call void @"io::out"(%String %8)
  %"$total" = load i32, i32* %total, align 4
  %"$count" = load i32, i32* %count, align 4
  %DIV = sdiv i32 %"$total", %"$count"
  %call.snprintf39 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.3, i32 0, i32 0), i32 %DIV)
  store i32 %call.snprintf39, i32* %format.size38, align 4
  %"$format.size40" = load i32, i32* %format.size38, align 4
  %ADD41 = add i32 %"$format.size40", 1
  %AS42 = sext i32 %ADD41 to i64
  %call.calloc44 = call i8* @calloc(i64 %AS42, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc44, i8** %format.text43, align 8
  %"$format.text45" = load i8*, i8** %format.text43, align 8
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text45", i64 %AS42, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.3, i32 0, i32 0), i32 %DIV)
  %"$format.size47" = load i32, i32* %format.size38, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %"$format.text49" = load i8*, i8** %format.text43, align 8
  %"$format.size50" = load i32, i32* %format.size38, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text49", 0
  %10 = insertvalue %String %9, i32 %"$format.size50", 1
  %11 = insertvalue %String %10, i32 %ADD48, 2
  call void @"io::out"(%String %11)
  %"$atk51" = load i32, i32* %atk, align 4
  %"$xp_cap" = load i32, i32* %xp_cap, align 4
  %MOD = srem i32 %"$atk51", %"$xp_cap"
  %call.snprintf53 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %MOD)
  store i32 %call.snprintf53, i32* %format.size52, align 4
  %"$format.size54" = load i32, i32* %format.size52, align 4
  %ADD55 = add i32 %"$format.size54", 1
  %AS56 = sext i32 %ADD55 to i64
  %call.calloc58 = call i8* @calloc(i64 %AS56, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc58, i8** %format.text57, align 8
  %"$format.text59" = load i8*, i8** %format.text57, align 8
  %call.snprintf60 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text59", i64 %AS56, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %MOD)
  %"$format.size61" = load i32, i32* %format.size52, align 4
  %ADD62 = add i32 %"$format.size61", 1
  %"$format.text63" = load i8*, i8** %format.text57, align 8
  %"$format.size64" = load i32, i32* %format.size52, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text63", 0
  %13 = insertvalue %String %12, i32 %"$format.size64", 1
  %14 = insertvalue %String %13, i32 %ADD62, 2
  call void @"io::out"(%String %14)
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.5, i32 0, i32 0), i32 15)
  store i32 %call.snprintf66, i32* %format.size65, align 4
  %"$format.size67" = load i32, i32* %format.size65, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %call.snprintf73 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.5, i32 0, i32 0), i32 15)
  %"$format.size74" = load i32, i32* %format.size65, align 4
  %ADD75 = add i32 %"$format.size74", 1
  %"$format.text76" = load i8*, i8** %format.text70, align 8
  %"$format.size77" = load i32, i32* %format.size65, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text76", 0
  %16 = insertvalue %String %15, i32 %"$format.size77", 1
  %17 = insertvalue %String %16, i32 %ADD75, 2
  call void @"io::out"(%String %17)
  %call.base_atk = call i32 @base_atk()
  %"$atk78" = load i32, i32* %atk, align 4
  %ADD79 = add i32 %call.base_atk, %"$atk78"
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.6, i32 0, i32 0), i32 %ADD79)
  store i32 %call.snprintf81, i32* %format.size80, align 4
  %"$format.size82" = load i32, i32* %format.size80, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.6, i32 0, i32 0), i32 %ADD79)
  %"$format.size89" = load i32, i32* %format.size80, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %"$format.text91" = load i8*, i8** %format.text85, align 8
  %"$format.size92" = load i32, i32* %format.size80, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text91", 0
  %19 = insertvalue %String %18, i32 %"$format.size92", 1
  %20 = insertvalue %String %19, i32 %ADD90, 2
  call void @"io::out"(%String %20)
  %call.snprintf94 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.7, i32 0, i32 0), i32 14)
  store i32 %call.snprintf94, i32* %format.size93, align 4
  %"$format.size95" = load i32, i32* %format.size93, align 4
  %ADD96 = add i32 %"$format.size95", 1
  %AS97 = sext i32 %ADD96 to i64
  %call.calloc99 = call i8* @calloc(i64 %AS97, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc99, i8** %format.text98, align 8
  %"$format.text100" = load i8*, i8** %format.text98, align 8
  %call.snprintf101 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text100", i64 %AS97, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.7, i32 0, i32 0), i32 14)
  %"$format.size102" = load i32, i32* %format.size93, align 4
  %ADD103 = add i32 %"$format.size102", 1
  %"$format.text104" = load i8*, i8** %format.text98, align 8
  %"$format.size105" = load i32, i32* %format.size93, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text104", 0
  %22 = insertvalue %String %21, i32 %"$format.size105", 1
  %23 = insertvalue %String %22, i32 %ADD103, 2
  call void @"io::out"(%String %23)
  %call.snprintf107 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.8, i32 0, i32 0), i32 20)
  store i32 %call.snprintf107, i32* %format.size106, align 4
  %"$format.size108" = load i32, i32* %format.size106, align 4
  %ADD109 = add i32 %"$format.size108", 1
  %AS110 = sext i32 %ADD109 to i64
  %call.calloc112 = call i8* @calloc(i64 %AS110, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc112, i8** %format.text111, align 8
  %"$format.text113" = load i8*, i8** %format.text111, align 8
  %call.snprintf114 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text113", i64 %AS110, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.8, i32 0, i32 0), i32 20)
  %"$format.size115" = load i32, i32* %format.size106, align 4
  %ADD116 = add i32 %"$format.size115", 1
  %"$format.text117" = load i8*, i8** %format.text111, align 8
  %"$format.size118" = load i32, i32* %format.size106, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text117", 0
  %25 = insertvalue %String %24, i32 %"$format.size118", 1
  %26 = insertvalue %String %25, i32 %ADD116, 2
  call void @"io::out"(%String %26)
  store float 1.500000e+00, float* %a, align 4
  store float 0x3FD3333340000000, float* %b, align 4
  %"$a" = load float, float* %a, align 4
  %"$b" = load float, float* %b, align 4
  %ADD119 = fadd float %"$a", %"$b"
  %AS121 = fpext float %ADD119 to double
  %call.snprintf122 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.9, i32 0, i32 0), double %AS121)
  store i32 %call.snprintf122, i32* %format.size120, align 4
  %"$format.size123" = load i32, i32* %format.size120, align 4
  %ADD124 = add i32 %"$format.size123", 1
  %AS125 = sext i32 %ADD124 to i64
  %call.calloc127 = call i8* @calloc(i64 %AS125, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc127, i8** %format.text126, align 8
  %"$format.text128" = load i8*, i8** %format.text126, align 8
  %call.snprintf129 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text128", i64 %AS125, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.9, i32 0, i32 0), double %AS121)
  %"$format.size130" = load i32, i32* %format.size120, align 4
  %ADD131 = add i32 %"$format.size130", 1
  %"$format.text132" = load i8*, i8** %format.text126, align 8
  %"$format.size133" = load i32, i32* %format.size120, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text132", 0
  %28 = insertvalue %String %27, i32 %"$format.size133", 1
  %29 = insertvalue %String %28, i32 %ADD131, 2
  call void @"io::out"(%String %29)
  %"$a134" = load float, float* %a, align 4
  %"$b135" = load float, float* %b, align 4
  %SUB136 = fsub float %"$a134", %"$b135"
  %AS138 = fpext float %SUB136 to double
  %call.snprintf139 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.10, i32 0, i32 0), double %AS138)
  store i32 %call.snprintf139, i32* %format.size137, align 4
  %"$format.size140" = load i32, i32* %format.size137, align 4
  %ADD141 = add i32 %"$format.size140", 1
  %AS142 = sext i32 %ADD141 to i64
  %call.calloc144 = call i8* @calloc(i64 %AS142, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc144, i8** %format.text143, align 8
  %"$format.text145" = load i8*, i8** %format.text143, align 8
  %call.snprintf146 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text145", i64 %AS142, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.10, i32 0, i32 0), double %AS138)
  %"$format.size147" = load i32, i32* %format.size137, align 4
  %ADD148 = add i32 %"$format.size147", 1
  %"$format.text149" = load i8*, i8** %format.text143, align 8
  %"$format.size150" = load i32, i32* %format.size137, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text149", 0
  %31 = insertvalue %String %30, i32 %"$format.size150", 1
  %32 = insertvalue %String %31, i32 %ADD148, 2
  call void @"io::out"(%String %32)
  %"$a151" = load float, float* %a, align 4
  %"$b152" = load float, float* %b, align 4
  %MUL153 = fmul float %"$a151", %"$b152"
  %AS155 = fpext float %MUL153 to double
  %call.snprintf156 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.11, i32 0, i32 0), double %AS155)
  store i32 %call.snprintf156, i32* %format.size154, align 4
  %"$format.size157" = load i32, i32* %format.size154, align 4
  %ADD158 = add i32 %"$format.size157", 1
  %AS159 = sext i32 %ADD158 to i64
  %call.calloc161 = call i8* @calloc(i64 %AS159, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc161, i8** %format.text160, align 8
  %"$format.text162" = load i8*, i8** %format.text160, align 8
  %call.snprintf163 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text162", i64 %AS159, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.11, i32 0, i32 0), double %AS155)
  %"$format.size164" = load i32, i32* %format.size154, align 4
  %ADD165 = add i32 %"$format.size164", 1
  %"$format.text166" = load i8*, i8** %format.text160, align 8
  %"$format.size167" = load i32, i32* %format.size154, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text166", 0
  %34 = insertvalue %String %33, i32 %"$format.size167", 1
  %35 = insertvalue %String %34, i32 %ADD165, 2
  call void @"io::out"(%String %35)
  %"$a168" = load float, float* %a, align 4
  %"$b169" = load float, float* %b, align 4
  %DIV170 = fdiv float %"$a168", %"$b169"
  %AS172 = fpext float %DIV170 to double
  %call.snprintf173 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), double %AS172)
  store i32 %call.snprintf173, i32* %format.size171, align 4
  %"$format.size174" = load i32, i32* %format.size171, align 4
  %ADD175 = add i32 %"$format.size174", 1
  %AS176 = sext i32 %ADD175 to i64
  %call.calloc178 = call i8* @calloc(i64 %AS176, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc178, i8** %format.text177, align 8
  %"$format.text179" = load i8*, i8** %format.text177, align 8
  %call.snprintf180 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text179", i64 %AS176, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), double %AS172)
  %"$format.size181" = load i32, i32* %format.size171, align 4
  %ADD182 = add i32 %"$format.size181", 1
  %"$format.text183" = load i8*, i8** %format.text177, align 8
  %"$format.size184" = load i32, i32* %format.size171, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text183", 0
  %37 = insertvalue %String %36, i32 %"$format.size184", 1
  %38 = insertvalue %String %37, i32 %ADD182, 2
  call void @"io::out"(%String %38)
  %"$a185" = load float, float* %a, align 4
  %"$b186" = load float, float* %b, align 4
  %LT = fcmp olt float %"$a185", %"$b186"
  store i32 0, i32* %format.size187, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if7

if7:                                              ; preds = %entry
  br i1 %LT, label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.13, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else8:                                            ; preds = %if7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else8, %if7.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf188 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf188, i32* %format.size187, align 4
  %"$format.size189" = load i32, i32* %format.size187, align 4
  %ADD190 = add i32 %"$format.size189", 1
  %AS191 = sext i32 %ADD190 to i64
  %call.calloc193 = call i8* @calloc(i64 %AS191, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc193, i8** %format.text192, align 8
  %"$format.text194" = load i8*, i8** %format.text192, align 8
  %"$bool.text195" = load i8*, i8** %bool.text, align 8
  %call.snprintf196 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text194", i64 %AS191, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text195")
  %"$format.size197" = load i32, i32* %format.size187, align 4
  %ADD198 = add i32 %"$format.size197", 1
  %"$format.text199" = load i8*, i8** %format.text192, align 8
  %"$format.size200" = load i32, i32* %format.size187, align 4
  %39 = insertvalue %String zeroinitializer, i8* %"$format.text199", 0
  %40 = insertvalue %String %39, i32 %"$format.size200", 1
  %41 = insertvalue %String %40, i32 %ADD198, 2
  call void @"io::out"(%String %41)
  %"$a201" = load float, float* %a, align 4
  %"$b202" = load float, float* %b, align 4
  %GT = fcmp ogt float %"$a201", %"$b202"
  store i32 0, i32* %format.size203, align 4
  store i8* null, i8** %bool.text204, align 8
  br label %if9

if9:                                              ; preds = %endif
  br i1 %GT, label %if9.then, label %else10

if9.then:                                         ; preds = %if9
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text204, align 8
  br label %endif1

else10:                                           ; preds = %if9
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text204, align 8
  br label %endif1

endif1:                                           ; preds = %else10, %if9.then
  %"$bool.text205" = load i8*, i8** %bool.text204, align 8
  %call.snprintf206 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text205")
  store i32 %call.snprintf206, i32* %format.size203, align 4
  %"$format.size207" = load i32, i32* %format.size203, align 4
  %ADD208 = add i32 %"$format.size207", 1
  %AS209 = sext i32 %ADD208 to i64
  %call.calloc211 = call i8* @calloc(i64 %AS209, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc211, i8** %format.text210, align 8
  %"$format.text212" = load i8*, i8** %format.text210, align 8
  %"$bool.text213" = load i8*, i8** %bool.text204, align 8
  %call.snprintf214 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text212", i64 %AS209, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text213")
  %"$format.size215" = load i32, i32* %format.size203, align 4
  %ADD216 = add i32 %"$format.size215", 1
  %"$format.text217" = load i8*, i8** %format.text210, align 8
  %"$format.size218" = load i32, i32* %format.size203, align 4
  %42 = insertvalue %String zeroinitializer, i8* %"$format.text217", 0
  %43 = insertvalue %String %42, i32 %"$format.size218", 1
  %44 = insertvalue %String %43, i32 %ADD216, 2
  call void @"io::out"(%String %44)
  %"$a219" = load float, float* %a, align 4
  %"$b220" = load float, float* %b, align 4
  %LE = fcmp ole float %"$a219", %"$b220"
  store i32 0, i32* %format.size221, align 4
  store i8* null, i8** %bool.text222, align 8
  br label %if11

if11:                                             ; preds = %endif1
  br i1 %LE, label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.19, i32 0, i32 0), i8** %bool.text222, align 8
  br label %endif2

else12:                                           ; preds = %if11
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.20, i32 0, i32 0), i8** %bool.text222, align 8
  br label %endif2

endif2:                                           ; preds = %else12, %if11.then
  %"$bool.text223" = load i8*, i8** %bool.text222, align 8
  %call.snprintf224 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0), i8* %"$bool.text223")
  store i32 %call.snprintf224, i32* %format.size221, align 4
  %"$format.size225" = load i32, i32* %format.size221, align 4
  %ADD226 = add i32 %"$format.size225", 1
  %AS227 = sext i32 %ADD226 to i64
  %call.calloc229 = call i8* @calloc(i64 %AS227, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc229, i8** %format.text228, align 8
  %"$format.text230" = load i8*, i8** %format.text228, align 8
  %"$bool.text231" = load i8*, i8** %bool.text222, align 8
  %call.snprintf232 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text230", i64 %AS227, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0), i8* %"$bool.text231")
  %"$format.size233" = load i32, i32* %format.size221, align 4
  %ADD234 = add i32 %"$format.size233", 1
  %"$format.text235" = load i8*, i8** %format.text228, align 8
  %"$format.size236" = load i32, i32* %format.size221, align 4
  %45 = insertvalue %String zeroinitializer, i8* %"$format.text235", 0
  %46 = insertvalue %String %45, i32 %"$format.size236", 1
  %47 = insertvalue %String %46, i32 %ADD234, 2
  call void @"io::out"(%String %47)
  %"$a237" = load float, float* %a, align 4
  %"$b238" = load float, float* %b, align 4
  %GE = fcmp oge float %"$a237", %"$b238"
  store i32 0, i32* %format.size239, align 4
  store i8* null, i8** %bool.text240, align 8
  br label %if13

if13:                                             ; preds = %endif2
  br i1 %GE, label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.22, i32 0, i32 0), i8** %bool.text240, align 8
  br label %endif3

else14:                                           ; preds = %if13
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.23, i32 0, i32 0), i8** %bool.text240, align 8
  br label %endif3

endif3:                                           ; preds = %else14, %if13.then
  %"$bool.text241" = load i8*, i8** %bool.text240, align 8
  %call.snprintf242 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0), i8* %"$bool.text241")
  store i32 %call.snprintf242, i32* %format.size239, align 4
  %"$format.size243" = load i32, i32* %format.size239, align 4
  %ADD244 = add i32 %"$format.size243", 1
  %AS245 = sext i32 %ADD244 to i64
  %call.calloc247 = call i8* @calloc(i64 %AS245, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc247, i8** %format.text246, align 8
  %"$format.text248" = load i8*, i8** %format.text246, align 8
  %"$bool.text249" = load i8*, i8** %bool.text240, align 8
  %call.snprintf250 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text248", i64 %AS245, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0), i8* %"$bool.text249")
  %"$format.size251" = load i32, i32* %format.size239, align 4
  %ADD252 = add i32 %"$format.size251", 1
  %"$format.text253" = load i8*, i8** %format.text246, align 8
  %"$format.size254" = load i32, i32* %format.size239, align 4
  %48 = insertvalue %String zeroinitializer, i8* %"$format.text253", 0
  %49 = insertvalue %String %48, i32 %"$format.size254", 1
  %50 = insertvalue %String %49, i32 %ADD252, 2
  call void @"io::out"(%String %50)
  store float 0x3FE0A3D700000000, float* %t, align 4
  %"$t" = load float, float* %t, align 4
  %call.clamp = call float @clamp(float %"$t")
  %AS256 = fpext float %call.clamp to double
  %call.snprintf257 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.25, i32 0, i32 0), double %AS256)
  store i32 %call.snprintf257, i32* %format.size255, align 4
  %"$format.size258" = load i32, i32* %format.size255, align 4
  %ADD259 = add i32 %"$format.size258", 1
  %AS260 = sext i32 %ADD259 to i64
  %call.calloc262 = call i8* @calloc(i64 %AS260, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc262, i8** %format.text261, align 8
  %"$format.text263" = load i8*, i8** %format.text261, align 8
  %call.snprintf264 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text263", i64 %AS260, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.25, i32 0, i32 0), double %AS256)
  %"$format.size265" = load i32, i32* %format.size255, align 4
  %ADD266 = add i32 %"$format.size265", 1
  %"$format.text267" = load i8*, i8** %format.text261, align 8
  %"$format.size268" = load i32, i32* %format.size255, align 4
  %51 = insertvalue %String zeroinitializer, i8* %"$format.text267", 0
  %52 = insertvalue %String %51, i32 %"$format.size268", 1
  %53 = insertvalue %String %52, i32 %ADD266, 2
  call void @"io::out"(%String %53)
  %call.clamp269 = call float @clamp(float 0x3F847AE140000000)
  %AS271 = fpext float %call.clamp269 to double
  %call.snprintf272 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.26, i32 0, i32 0), double %AS271)
  store i32 %call.snprintf272, i32* %format.size270, align 4
  %"$format.size273" = load i32, i32* %format.size270, align 4
  %ADD274 = add i32 %"$format.size273", 1
  %AS275 = sext i32 %ADD274 to i64
  %call.calloc277 = call i8* @calloc(i64 %AS275, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc277, i8** %format.text276, align 8
  %"$format.text278" = load i8*, i8** %format.text276, align 8
  %call.snprintf279 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text278", i64 %AS275, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.26, i32 0, i32 0), double %AS271)
  %"$format.size280" = load i32, i32* %format.size270, align 4
  %ADD281 = add i32 %"$format.size280", 1
  %"$format.text282" = load i8*, i8** %format.text276, align 8
  %"$format.size283" = load i32, i32* %format.size270, align 4
  %54 = insertvalue %String zeroinitializer, i8* %"$format.text282", 0
  %55 = insertvalue %String %54, i32 %"$format.size283", 1
  %56 = insertvalue %String %55, i32 %ADD281, 2
  call void @"io::out"(%String %56)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 221 — operator: arithmetic with precedence (2*3 + 20/4 - 1)

```ura
// expect: 10

fn main() i32:
    return 2 * 3 + 20 / 4 - 1
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  ret i32 10
}
```

---

## 222 — operator: reassign an existing variable (2*10 + 3)

```ura
// expect: 23

fn main() i32:
    a i32 = 2
    a = a * 10 + 3
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 10
  %ADD = add i32 %MUL, 3
  store i32 %ADD, i32* %a, align 4
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

---

## 223 — operator: modulo

```ura
// expect: 2

fn main() i32:
    return 17 % 5
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  ret i32 2
}
```

---

## 224 — operator: all comparison ops on hero/enemy stats

```ura
// stdout: hero_hp == min_hp: True
// stdout: hero_hp != orc_hp: True
// stdout: orc_hp < hero_hp:  True
// stdout: orc_hp > hero_hp:  False
// stdout: orc_hp <= 60:      True
// stdout: hero_hp >= 100:    True
// stdout: strong enemy
// stdout: hero is at full health and winning
// expect: 0

fn enemy_atk() i32:
    return 18

fn main() i32:
    hero_hp  i32 = 100
    orc_hp   i32 = 60
    min_hp   i32 = 100

    io::out($"hero_hp == min_hp: {hero_hp == min_hp}\n")   // True
    io::out($"hero_hp != orc_hp: {hero_hp != orc_hp}\n")   // True
    io::out($"orc_hp < hero_hp:  {orc_hp < hero_hp}\n")   // True
    io::out($"orc_hp > hero_hp:  {orc_hp > hero_hp}\n")   // False
    io::out($"orc_hp <= 60:      {orc_hp <= 60}\n")   // True
    io::out($"hero_hp >= 100:    {hero_hp >= 100}\n")   // True

    // comparison with fn call
    if enemy_atk() > 15:
        io::out("strong enemy\n")
    else:
        io::out("weak enemy\n")

    // chained via if
    if hero_hp == 100 and orc_hp < hero_hp:
        io::out("hero is at full health and winning\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [23 x i8] c"hero_hp == min_hp: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [23 x i8] c"hero_hp != orc_hp: %s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [23 x i8] c"orc_hp < hero_hp:  %s\0A\00", align 1
@str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [23 x i8] c"orc_hp > hero_hp:  %s\0A\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [23 x i8] c"orc_hp <= 60:      %s\0A\00", align 1
@str.15 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.16 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.17 = private unnamed_addr constant [23 x i8] c"hero_hp >= 100:    %s\0A\00", align 1
@str.18 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@str.19 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@str.20 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @enemy_atk() {
entry:
  ret i32 18
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  %orc_hp = alloca i32, align 4
  %min_hp = alloca i32, align 4
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %format.size33 = alloca i32, align 4
  %bool.text34 = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %format.size51 = alloca i32, align 4
  %bool.text52 = alloca i8*, align 8
  %format.text58 = alloca i8*, align 8
  %format.size68 = alloca i32, align 4
  %bool.text69 = alloca i8*, align 8
  %format.text75 = alloca i8*, align 8
  %format.size85 = alloca i32, align 4
  %bool.text86 = alloca i8*, align 8
  %format.text92 = alloca i8*, align 8
  store i32 100, i32* %hero_hp, align 4
  store i32 60, i32* %orc_hp, align 4
  store i32 100, i32* %min_hp, align 4
  %"$hero_hp" = load i32, i32* %hero_hp, align 4
  %"$min_hp" = load i32, i32* %min_hp, align 4
  %EQ = icmp eq i32 %"$hero_hp", %"$min_hp"
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %EQ, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text8" = load i8*, i8** %bool.text, align 8
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text8")
  %"$format.size10" = load i32, i32* %format.size, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %"$format.text12" = load i8*, i8** %format.text, align 8
  %"$format.size13" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text12", 0
  %1 = insertvalue %String %0, i32 %"$format.size13", 1
  %2 = insertvalue %String %1, i32 %ADD11, 2
  call void @"io::out"(%String %2)
  %"$hero_hp14" = load i32, i32* %hero_hp, align 4
  %"$orc_hp" = load i32, i32* %orc_hp, align 4
  %NQ = icmp ne i32 %"$hero_hp14", %"$orc_hp"
  store i32 0, i32* %format.size15, align 4
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %NQ, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  %"$orc_hp31" = load i32, i32* %orc_hp, align 4
  %"$hero_hp32" = load i32, i32* %hero_hp, align 4
  %LT = icmp slt i32 %"$orc_hp31", %"$hero_hp32"
  store i32 0, i32* %format.size33, align 4
  store i8* null, i8** %bool.text34, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %LT, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text35" = load i8*, i8** %bool.text34, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text35")
  store i32 %call.snprintf36, i32* %format.size33, align 4
  %"$format.size37" = load i32, i32* %format.size33, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text34, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text43")
  %"$format.size45" = load i32, i32* %format.size33, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text40, align 8
  %"$format.size48" = load i32, i32* %format.size33, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %7 = insertvalue %String %6, i32 %"$format.size48", 1
  %8 = insertvalue %String %7, i32 %ADD46, 2
  call void @"io::out"(%String %8)
  %"$orc_hp49" = load i32, i32* %orc_hp, align 4
  %"$hero_hp50" = load i32, i32* %hero_hp, align 4
  %GT = icmp sgt i32 %"$orc_hp49", %"$hero_hp50"
  store i32 0, i32* %format.size51, align 4
  store i8* null, i8** %bool.text52, align 8
  br label %if12

if12:                                             ; preds = %endif2
  br i1 %GT, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif3

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.10, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif3

endif3:                                           ; preds = %else13, %if12.then
  %"$bool.text53" = load i8*, i8** %bool.text52, align 8
  %call.snprintf54 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text53")
  store i32 %call.snprintf54, i32* %format.size51, align 4
  %"$format.size55" = load i32, i32* %format.size51, align 4
  %ADD56 = add i32 %"$format.size55", 1
  %AS57 = sext i32 %ADD56 to i64
  %call.calloc59 = call i8* @calloc(i64 %AS57, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc59, i8** %format.text58, align 8
  %"$format.text60" = load i8*, i8** %format.text58, align 8
  %"$bool.text61" = load i8*, i8** %bool.text52, align 8
  %call.snprintf62 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text60", i64 %AS57, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text61")
  %"$format.size63" = load i32, i32* %format.size51, align 4
  %ADD64 = add i32 %"$format.size63", 1
  %"$format.text65" = load i8*, i8** %format.text58, align 8
  %"$format.size66" = load i32, i32* %format.size51, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text65", 0
  %10 = insertvalue %String %9, i32 %"$format.size66", 1
  %11 = insertvalue %String %10, i32 %ADD64, 2
  call void @"io::out"(%String %11)
  %"$orc_hp67" = load i32, i32* %orc_hp, align 4
  %LE = icmp sle i32 %"$orc_hp67", 60
  store i32 0, i32* %format.size68, align 4
  store i8* null, i8** %bool.text69, align 8
  br label %if14

if14:                                             ; preds = %endif3
  br i1 %LE, label %if14.then, label %else15

if14.then:                                        ; preds = %if14
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text69, align 8
  br label %endif4

else15:                                           ; preds = %if14
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text69, align 8
  br label %endif4

endif4:                                           ; preds = %else15, %if14.then
  %"$bool.text70" = load i8*, i8** %bool.text69, align 8
  %call.snprintf71 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text70")
  store i32 %call.snprintf71, i32* %format.size68, align 4
  %"$format.size72" = load i32, i32* %format.size68, align 4
  %ADD73 = add i32 %"$format.size72", 1
  %AS74 = sext i32 %ADD73 to i64
  %call.calloc76 = call i8* @calloc(i64 %AS74, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc76, i8** %format.text75, align 8
  %"$format.text77" = load i8*, i8** %format.text75, align 8
  %"$bool.text78" = load i8*, i8** %bool.text69, align 8
  %call.snprintf79 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text77", i64 %AS74, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text78")
  %"$format.size80" = load i32, i32* %format.size68, align 4
  %ADD81 = add i32 %"$format.size80", 1
  %"$format.text82" = load i8*, i8** %format.text75, align 8
  %"$format.size83" = load i32, i32* %format.size68, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text82", 0
  %13 = insertvalue %String %12, i32 %"$format.size83", 1
  %14 = insertvalue %String %13, i32 %ADD81, 2
  call void @"io::out"(%String %14)
  %"$hero_hp84" = load i32, i32* %hero_hp, align 4
  %GE = icmp sge i32 %"$hero_hp84", 100
  store i32 0, i32* %format.size85, align 4
  store i8* null, i8** %bool.text86, align 8
  br label %if16

if16:                                             ; preds = %endif4
  br i1 %GE, label %if16.then, label %else17

if16.then:                                        ; preds = %if16
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.15, i32 0, i32 0), i8** %bool.text86, align 8
  br label %endif5

else17:                                           ; preds = %if16
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.16, i32 0, i32 0), i8** %bool.text86, align 8
  br label %endif5

endif5:                                           ; preds = %else17, %if16.then
  %"$bool.text87" = load i8*, i8** %bool.text86, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.17, i32 0, i32 0), i8* %"$bool.text87")
  store i32 %call.snprintf88, i32* %format.size85, align 4
  %"$format.size89" = load i32, i32* %format.size85, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %AS91 = sext i32 %ADD90 to i64
  %call.calloc93 = call i8* @calloc(i64 %AS91, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc93, i8** %format.text92, align 8
  %"$format.text94" = load i8*, i8** %format.text92, align 8
  %"$bool.text95" = load i8*, i8** %bool.text86, align 8
  %call.snprintf96 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text94", i64 %AS91, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.17, i32 0, i32 0), i8* %"$bool.text95")
  %"$format.size97" = load i32, i32* %format.size85, align 4
  %ADD98 = add i32 %"$format.size97", 1
  %"$format.text99" = load i8*, i8** %format.text92, align 8
  %"$format.size100" = load i32, i32* %format.size85, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text99", 0
  %16 = insertvalue %String %15, i32 %"$format.size100", 1
  %17 = insertvalue %String %16, i32 %ADD98, 2
  call void @"io::out"(%String %17)
  br label %if18

if18:                                             ; preds = %endif5
  %call.enemy_atk = call i32 @enemy_atk()
  %GT101 = icmp sgt i32 %call.enemy_atk, 15
  br i1 %GT101, label %if18.then, label %else19

if18.then:                                        ; preds = %if18
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.18, i32 0, i32 0), i32 13, i32 0 })
  br label %endif6

else19:                                           ; preds = %if18
  call void @"io::out"(%String { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.19, i32 0, i32 0), i32 11, i32 0 })
  br label %endif6

endif6:                                           ; preds = %else19, %if18.then
  br label %if20

if20:                                             ; preds = %endif6
  %"$hero_hp102" = load i32, i32* %hero_hp, align 4
  %EQ103 = icmp eq i32 %"$hero_hp102", 100
  %"$orc_hp104" = load i32, i32* %orc_hp, align 4
  %"$hero_hp105" = load i32, i32* %hero_hp, align 4
  %LT106 = icmp slt i32 %"$orc_hp104", %"$hero_hp105"
  %AND = and i1 %EQ103, %LT106
  br i1 %AND, label %if20.then, label %endif7

if20.then:                                        ; preds = %if20
  call void @"io::out"(%String { i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.20, i32 0, i32 0), i32 35, i32 0 })
  br label %endif7

endif7:                                           ; preds = %if20.then, %if20
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 225 — operator: comparisons produce bools (with precedence)

```ura
// expect: 3

fn main() i32:
    a i32 = 5
    b i32 = 9
    t bool = a < b
    u bool = a == 5
    v bool = b != a
    w bool = b >= 9
    x bool = a <= 4
    y bool = a > b
    z bool = a is 5
    m bool = a + 1 < b * 2
    return 3
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %t = alloca i1, align 1
  %u = alloca i1, align 1
  %v = alloca i1, align 1
  %w = alloca i1, align 1
  %x = alloca i1, align 1
  %y = alloca i1, align 1
  %z = alloca i1, align 1
  %m = alloca i1, align 1
  store i32 5, i32* %a, align 4
  store i32 9, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %LT = icmp slt i32 %"$a", %"$b"
  store i1 %LT, i1* %t, align 1
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 5
  store i1 %EQ, i1* %u, align 1
  %"$b2" = load i32, i32* %b, align 4
  %"$a3" = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %"$b2", %"$a3"
  store i1 %NQ, i1* %v, align 1
  %"$b4" = load i32, i32* %b, align 4
  %GE = icmp sge i32 %"$b4", 9
  store i1 %GE, i1* %w, align 1
  %"$a5" = load i32, i32* %a, align 4
  %LE = icmp sle i32 %"$a5", 4
  store i1 %LE, i1* %x, align 1
  %"$a6" = load i32, i32* %a, align 4
  %"$b7" = load i32, i32* %b, align 4
  %GT = icmp sgt i32 %"$a6", %"$b7"
  store i1 %GT, i1* %y, align 1
  %"$a8" = load i32, i32* %a, align 4
  %EQ9 = icmp eq i32 %"$a8", 5
  store i1 %EQ9, i1* %z, align 1
  %"$a10" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a10", 1
  %"$b11" = load i32, i32* %b, align 4
  %MUL = mul i32 %"$b11", 2
  %LT12 = icmp slt i32 %ADD, %MUL
  store i1 %LT12, i1* %m, align 1
  ret i32 3
}
```

---

## 226 — operator: compound assignment += -= *= /= %=

```ura
// stdout: 15
// stdout: 12
// stdout: 48
// stdout: 8
// stdout: 3
// expect: 0

fn main() i32:
    x i32 = 10
    x += 5
    io::out($"{x}\n")
    x -= 3
    io::out($"{x}\n")
    x *= 4
    io::out($"{x}\n")
    x /= 6
    io::out($"{x}\n")
    x %= 5
    io::out($"{x}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size9 = alloca i32, align 4
  %format.text15 = alloca i8*, align 8
  %format.size24 = alloca i32, align 4
  %format.text30 = alloca i8*, align 8
  %format.size39 = alloca i32, align 4
  %format.text45 = alloca i8*, align 8
  %format.size54 = alloca i32, align 4
  %format.text60 = alloca i8*, align 8
  store i32 10, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x", 5
  store i32 %ADD, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x1")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD2 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD2 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x1")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  call void @"io::out"(%String %2)
  %"$x8" = load i32, i32* %x, align 4
  %SUB = sub i32 %"$x8", 3
  store i32 %SUB, i32* %x, align 4
  %"$x10" = load i32, i32* %x, align 4
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x10")
  store i32 %call.snprintf11, i32* %format.size9, align 4
  %"$format.size12" = load i32, i32* %format.size9, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x10")
  %"$format.size19" = load i32, i32* %format.size9, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size9, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %4 = insertvalue %String %3, i32 %"$format.size22", 1
  %5 = insertvalue %String %4, i32 %ADD20, 2
  call void @"io::out"(%String %5)
  %"$x23" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x23", 4
  store i32 %MUL, i32* %x, align 4
  %"$x25" = load i32, i32* %x, align 4
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$x25")
  store i32 %call.snprintf26, i32* %format.size24, align 4
  %"$format.size27" = load i32, i32* %format.size24, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %AS29 = sext i32 %ADD28 to i64
  %call.calloc31 = call i8* @calloc(i64 %AS29, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc31, i8** %format.text30, align 8
  %"$format.text32" = load i8*, i8** %format.text30, align 8
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text32", i64 %AS29, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$x25")
  %"$format.size34" = load i32, i32* %format.size24, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %"$format.text36" = load i8*, i8** %format.text30, align 8
  %"$format.size37" = load i32, i32* %format.size24, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text36", 0
  %7 = insertvalue %String %6, i32 %"$format.size37", 1
  %8 = insertvalue %String %7, i32 %ADD35, 2
  call void @"io::out"(%String %8)
  %"$x38" = load i32, i32* %x, align 4
  %DIV = sdiv i32 %"$x38", 6
  store i32 %DIV, i32* %x, align 4
  %"$x40" = load i32, i32* %x, align 4
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$x40")
  store i32 %call.snprintf41, i32* %format.size39, align 4
  %"$format.size42" = load i32, i32* %format.size39, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %AS44 = sext i32 %ADD43 to i64
  %call.calloc46 = call i8* @calloc(i64 %AS44, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc46, i8** %format.text45, align 8
  %"$format.text47" = load i8*, i8** %format.text45, align 8
  %call.snprintf48 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text47", i64 %AS44, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$x40")
  %"$format.size49" = load i32, i32* %format.size39, align 4
  %ADD50 = add i32 %"$format.size49", 1
  %"$format.text51" = load i8*, i8** %format.text45, align 8
  %"$format.size52" = load i32, i32* %format.size39, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text51", 0
  %10 = insertvalue %String %9, i32 %"$format.size52", 1
  %11 = insertvalue %String %10, i32 %ADD50, 2
  call void @"io::out"(%String %11)
  %"$x53" = load i32, i32* %x, align 4
  %MOD = srem i32 %"$x53", 5
  store i32 %MOD, i32* %x, align 4
  %"$x55" = load i32, i32* %x, align 4
  %call.snprintf56 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$x55")
  store i32 %call.snprintf56, i32* %format.size54, align 4
  %"$format.size57" = load i32, i32* %format.size54, align 4
  %ADD58 = add i32 %"$format.size57", 1
  %AS59 = sext i32 %ADD58 to i64
  %call.calloc61 = call i8* @calloc(i64 %AS59, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc61, i8** %format.text60, align 8
  %"$format.text62" = load i8*, i8** %format.text60, align 8
  %call.snprintf63 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text62", i64 %AS59, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$x55")
  %"$format.size64" = load i32, i32* %format.size54, align 4
  %ADD65 = add i32 %"$format.size64", 1
  %"$format.text66" = load i8*, i8** %format.text60, align 8
  %"$format.size67" = load i32, i32* %format.size54, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text66", 0
  %13 = insertvalue %String %12, i32 %"$format.size67", 1
  %14 = insertvalue %String %13, i32 %ADD65, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 227 — operator: logical ops: &&, ||, !, and, or, not

```ura
// stdout: can_proceed: True
// stdout: escape:      False
// stdout: blocked:     False
// stdout: hero fights
// stdout: hero advances
// stdout: !!alive: True
// expect: 0

fn is_alive(hp i32) bool:
    return hp > 0

fn has_key(keys i32) bool:
    return keys > 0

fn main() i32:
    hp       i32  = 60
    keys     i32  = 1
    exhausted bool = False

    // &&
    can_proceed bool = is_alive(hp) && has_key(keys) && !exhausted
    io::out($"can_proceed: {can_proceed}\n")   // True

    // ||
    escape bool = hp < 10 || keys == 0
    io::out($"escape:      {escape}\n")   // False

    // !
    blocked bool = !can_proceed
    io::out($"blocked:     {blocked}\n")   // False

    // and / or / not keywords
    if is_alive(hp) and not exhausted:
        io::out("hero fights\n")

    if hp < 10 or keys == 0:
        io::out("hero retreats\n")
    else:
        io::out("hero advances\n")

    // f64 negation
    alive bool = is_alive(hp)
    io::out($"!!alive: {!(!alive)}\n")   // True
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [17 x i8] c"can_proceed: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [17 x i8] c"escape:      %s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [17 x i8] c"blocked:     %s\0A\00", align 1
@str.9 = private unnamed_addr constant [13 x i8] c"hero fights\0A\00", align 1
@str.10 = private unnamed_addr constant [15 x i8] c"hero retreats\0A\00", align 1
@str.11 = private unnamed_addr constant [15 x i8] c"hero advances\0A\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [13 x i8] c"!!alive: %s\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i1 @is_alive(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %GT = icmp sgt i32 %"$hp", 0
  ret i1 %GT
}

define i1 @has_key(i32 %0) {
entry:
  %keys = alloca i32, align 4
  store i32 %0, i32* %keys, align 4
  %"$keys" = load i32, i32* %keys, align 4
  %GT = icmp sgt i32 %"$keys", 0
  ret i1 %GT
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %keys = alloca i32, align 4
  %exhausted = alloca i1, align 1
  %can_proceed = alloca i1, align 1
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %escape = alloca i1, align 1
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %blocked = alloca i1, align 1
  %format.size33 = alloca i32, align 4
  %bool.text34 = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %alive = alloca i1, align 1
  %format.size63 = alloca i32, align 4
  %bool.text64 = alloca i8*, align 8
  %format.text70 = alloca i8*, align 8
  store i32 60, i32* %hp, align 4
  store i32 1, i32* %keys, align 4
  store i1 false, i1* %exhausted, align 1
  %"$hp" = load i32, i32* %hp, align 4
  %call.is_alive = call i1 @is_alive(i32 %"$hp")
  %"$keys" = load i32, i32* %keys, align 4
  %call.has_key = call i1 @has_key(i32 %"$keys")
  %AND = and i1 %call.is_alive, %call.has_key
  %"$exhausted" = load i1, i1* %exhausted, align 1
  %NOT = xor i1 %"$exhausted", true
  %AND6 = and i1 %AND, %NOT
  store i1 %AND6, i1* %can_proceed, align 1
  store i32 0, i32* %format.size, align 4
  %"$can_proceed" = load i1, i1* %can_proceed, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$can_proceed", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text7" = load i8*, i8** %bool.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text7")
  %"$format.size9" = load i32, i32* %format.size, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %"$format.text11" = load i8*, i8** %format.text, align 8
  %"$format.size12" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text11", 0
  %1 = insertvalue %String %0, i32 %"$format.size12", 1
  %2 = insertvalue %String %1, i32 %ADD10, 2
  call void @"io::out"(%String %2)
  %"$hp13" = load i32, i32* %hp, align 4
  %LT = icmp slt i32 %"$hp13", 10
  %"$keys14" = load i32, i32* %keys, align 4
  %EQ = icmp eq i32 %"$keys14", 0
  %OR = or i1 %LT, %EQ
  store i1 %OR, i1* %escape, align 1
  store i32 0, i32* %format.size15, align 4
  %"$escape" = load i1, i1* %escape, align 1
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"$escape", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  %"$can_proceed31" = load i1, i1* %can_proceed, align 1
  %NOT32 = xor i1 %"$can_proceed31", true
  store i1 %NOT32, i1* %blocked, align 1
  store i32 0, i32* %format.size33, align 4
  %"$blocked" = load i1, i1* %blocked, align 1
  store i8* null, i8** %bool.text34, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %"$blocked", label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text35" = load i8*, i8** %bool.text34, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text35")
  store i32 %call.snprintf36, i32* %format.size33, align 4
  %"$format.size37" = load i32, i32* %format.size33, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text34, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text43")
  %"$format.size45" = load i32, i32* %format.size33, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text40, align 8
  %"$format.size48" = load i32, i32* %format.size33, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %7 = insertvalue %String %6, i32 %"$format.size48", 1
  %8 = insertvalue %String %7, i32 %ADD46, 2
  call void @"io::out"(%String %8)
  br label %if12

if12:                                             ; preds = %endif2
  %"$hp49" = load i32, i32* %hp, align 4
  %call.is_alive50 = call i1 @is_alive(i32 %"$hp49")
  %"$exhausted51" = load i1, i1* %exhausted, align 1
  %NOT52 = xor i1 %"$exhausted51", true
  %AND53 = and i1 %call.is_alive50, %NOT52
  br i1 %AND53, label %if12.then, label %endif3

if12.then:                                        ; preds = %if12
  call void @"io::out"(%String { i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.9, i32 0, i32 0), i32 12, i32 0 })
  br label %endif3

endif3:                                           ; preds = %if12.then, %if12
  br label %if13

if13:                                             ; preds = %endif3
  %"$hp54" = load i32, i32* %hp, align 4
  %LT55 = icmp slt i32 %"$hp54", 10
  %"$keys56" = load i32, i32* %keys, align 4
  %EQ57 = icmp eq i32 %"$keys56", 0
  %OR58 = or i1 %LT55, %EQ57
  br i1 %OR58, label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.10, i32 0, i32 0), i32 14, i32 0 })
  br label %endif4

else14:                                           ; preds = %if13
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.11, i32 0, i32 0), i32 14, i32 0 })
  br label %endif4

endif4:                                           ; preds = %else14, %if13.then
  %"$hp59" = load i32, i32* %hp, align 4
  %call.is_alive60 = call i1 @is_alive(i32 %"$hp59")
  store i1 %call.is_alive60, i1* %alive, align 1
  %"$alive" = load i1, i1* %alive, align 1
  %NOT61 = xor i1 %"$alive", true
  %NOT62 = xor i1 %NOT61, true
  store i32 0, i32* %format.size63, align 4
  store i8* null, i8** %bool.text64, align 8
  br label %if15

if15:                                             ; preds = %endif4
  br i1 %NOT62, label %if15.then, label %else16

if15.then:                                        ; preds = %if15
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text64, align 8
  br label %endif5

else16:                                           ; preds = %if15
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text64, align 8
  br label %endif5

endif5:                                           ; preds = %else16, %if15.then
  %"$bool.text65" = load i8*, i8** %bool.text64, align 8
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text65")
  store i32 %call.snprintf66, i32* %format.size63, align 4
  %"$format.size67" = load i32, i32* %format.size63, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %"$bool.text73" = load i8*, i8** %bool.text64, align 8
  %call.snprintf74 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text73")
  %"$format.size75" = load i32, i32* %format.size63, align 4
  %ADD76 = add i32 %"$format.size75", 1
  %"$format.text77" = load i8*, i8** %format.text70, align 8
  %"$format.size78" = load i32, i32* %format.size63, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text77", 0
  %10 = insertvalue %String %9, i32 %"$format.size78", 1
  %11 = insertvalue %String %10, i32 %ADD76, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 228 — operator: and / or with comparisons and precedence

```ura
// stdout: True
// stdout: True
// stdout: False
// stdout: True
// expect: 0

fn main() i32:
    a i32 = 5
    b i32 = 9
    io::out($"{a < b and b == 9}\n")
    io::out($"{a > b or b == 9}\n")
    io::out($"{a < b and a > b}\n")
    io::out($"{a < b or a > b}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %format.size38 = alloca i32, align 4
  %bool.text39 = alloca i8*, align 8
  %format.text45 = alloca i8*, align 8
  %format.size61 = alloca i32, align 4
  %bool.text62 = alloca i8*, align 8
  %format.text68 = alloca i8*, align 8
  store i32 5, i32* %a, align 4
  store i32 9, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %LT = icmp slt i32 %"$a", %"$b"
  %"$b4" = load i32, i32* %b, align 4
  %EQ = icmp eq i32 %"$b4", 9
  %AND = and i1 %LT, %EQ
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %AND, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text5" = load i8*, i8** %bool.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text5")
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  %"$a11" = load i32, i32* %a, align 4
  %"$b12" = load i32, i32* %b, align 4
  %GT = icmp sgt i32 %"$a11", %"$b12"
  %"$b13" = load i32, i32* %b, align 4
  %EQ14 = icmp eq i32 %"$b13", 9
  %OR = or i1 %GT, %EQ14
  store i32 0, i32* %format.size15, align 4
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %OR, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  %"$a31" = load i32, i32* %a, align 4
  %"$b32" = load i32, i32* %b, align 4
  %LT33 = icmp slt i32 %"$a31", %"$b32"
  %"$a34" = load i32, i32* %a, align 4
  %"$b35" = load i32, i32* %b, align 4
  %GT36 = icmp sgt i32 %"$a34", %"$b35"
  %AND37 = and i1 %LT33, %GT36
  store i32 0, i32* %format.size38, align 4
  store i8* null, i8** %bool.text39, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %AND37, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text40" = load i8*, i8** %bool.text39, align 8
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text40")
  store i32 %call.snprintf41, i32* %format.size38, align 4
  %"$format.size42" = load i32, i32* %format.size38, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %AS44 = sext i32 %ADD43 to i64
  %call.calloc46 = call i8* @calloc(i64 %AS44, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc46, i8** %format.text45, align 8
  %"$format.text47" = load i8*, i8** %format.text45, align 8
  %"$bool.text48" = load i8*, i8** %bool.text39, align 8
  %call.snprintf49 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text47", i64 %AS44, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text48")
  %"$format.size50" = load i32, i32* %format.size38, align 4
  %ADD51 = add i32 %"$format.size50", 1
  %"$format.text52" = load i8*, i8** %format.text45, align 8
  %"$format.size53" = load i32, i32* %format.size38, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text52", 0
  %7 = insertvalue %String %6, i32 %"$format.size53", 1
  %8 = insertvalue %String %7, i32 %ADD51, 2
  call void @"io::out"(%String %8)
  %"$a54" = load i32, i32* %a, align 4
  %"$b55" = load i32, i32* %b, align 4
  %LT56 = icmp slt i32 %"$a54", %"$b55"
  %"$a57" = load i32, i32* %a, align 4
  %"$b58" = load i32, i32* %b, align 4
  %GT59 = icmp sgt i32 %"$a57", %"$b58"
  %OR60 = or i1 %LT56, %GT59
  store i32 0, i32* %format.size61, align 4
  store i8* null, i8** %bool.text62, align 8
  br label %if12

if12:                                             ; preds = %endif2
  br i1 %OR60, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i8** %bool.text62, align 8
  br label %endif3

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.10, i32 0, i32 0), i8** %bool.text62, align 8
  br label %endif3

endif3:                                           ; preds = %else13, %if12.then
  %"$bool.text63" = load i8*, i8** %bool.text62, align 8
  %call.snprintf64 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text63")
  store i32 %call.snprintf64, i32* %format.size61, align 4
  %"$format.size65" = load i32, i32* %format.size61, align 4
  %ADD66 = add i32 %"$format.size65", 1
  %AS67 = sext i32 %ADD66 to i64
  %call.calloc69 = call i8* @calloc(i64 %AS67, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc69, i8** %format.text68, align 8
  %"$format.text70" = load i8*, i8** %format.text68, align 8
  %"$bool.text71" = load i8*, i8** %bool.text62, align 8
  %call.snprintf72 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text70", i64 %AS67, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text71")
  %"$format.size73" = load i32, i32* %format.size61, align 4
  %ADD74 = add i32 %"$format.size73", 1
  %"$format.text75" = load i8*, i8** %format.text68, align 8
  %"$format.size76" = load i32, i32* %format.size61, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text75", 0
  %10 = insertvalue %String %9, i32 %"$format.size76", 1
  %11 = insertvalue %String %10, i32 %ADD74, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 229 — operator: type mismatch between operands

```ura
// error: Cannot use '+' with fn and i32

fn twice(n i32) i32:
    return n + n
fn main() i32:
    return twice + 1
```

### stderr

```
error: test.ura:6:12 'twice' is a function, not a value
  |
6 |     return twice + 1
  |            ^^^^^
help: call it: twice(...)
error: aborting due to 1 error
```

### status

```
1
```

---

## 230 — operator: a comparison result is a bool, not an i32

```ura
// error: Cannot assign bool to i32

fn main() i32:
    a i32 = 5
    n i32 = a < 9
    return 0
```

### stderr

```
error: test.ura:5:15 expected 'i32', found 'bool'
  |
5 |     n i32 = a < 9
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 231 — variable: hero stats: declare all common types

```ura
// stdout: === Aldric enters the dungeon ===
// stdout: HP: 100 | MP: 50 | Speed: 1.500000
// stdout: Alive: True
// expect: 0

fn main() i32:
    name  char[] = "Aldric"
    hp    i32   = 100
    mp    i32   = 50
    alive bool  = True
    speed f32 = 1.5
    io::out($"=== {name} enters the dungeon ===\n")
    io::out($"HP: {hp} | MP: {mp} | Speed: {speed}\n")
    io::out($"Alive: {alive}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [31 x i8] c"=== %s enters the dungeon ===\0A\00", align 1
@str.1 = private unnamed_addr constant [29 x i8] c"HP: %d | MP: %d | Speed: %f\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  %hp = alloca i32, align 4
  %mp = alloca i32, align 4
  %alive = alloca i1, align 1
  %speed = alloca float, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text12 = alloca i8*, align 8
  %format.size20 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text25 = alloca i8*, align 8
  store [7 x i8] c"Aldric\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 100, i32* %hp, align 4
  store i32 50, i32* %mp, align 4
  store i1 true, i1* %alive, align 1
  store float 1.500000e+00, float* %speed, align 4
  %"$name" = load i8*, i8** %name, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i8* %"$name")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  %"$hp" = load i32, i32* %hp, align 4
  %"$mp" = load i32, i32* %mp, align 4
  %"$speed" = load float, float* %speed, align 4
  %AS7 = fpext float %"$speed" to double
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @str.1, i32 0, i32 0), i32 %"$hp", i32 %"$mp", double %AS7)
  store i32 %call.snprintf8, i32* %format.size6, align 4
  %"$format.size9" = load i32, i32* %format.size6, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @str.1, i32 0, i32 0), i32 %"$hp", i32 %"$mp", double %AS7)
  %"$format.size16" = load i32, i32* %format.size6, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text12, align 8
  %"$format.size19" = load i32, i32* %format.size6, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %5 = insertvalue %String %4, i32 %"$format.size19", 1
  %6 = insertvalue %String %5, i32 %ADD17, 2
  call void @"io::out"(%String %6)
  store i32 0, i32* %format.size20, align 4
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$alive", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf21, i32* %format.size20, align 4
  %"$format.size22" = load i32, i32* %format.size20, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %AS24 = sext i32 %ADD23 to i64
  %call.calloc26 = call i8* @calloc(i64 %AS24, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc26, i8** %format.text25, align 8
  %"$format.text27" = load i8*, i8** %format.text25, align 8
  %"$bool.text28" = load i8*, i8** %bool.text, align 8
  %call.snprintf29 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text27", i64 %AS24, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text28")
  %"$format.size30" = load i32, i32* %format.size20, align 4
  %ADD31 = add i32 %"$format.size30", 1
  %"$format.text32" = load i8*, i8** %format.text25, align 8
  %"$format.size33" = load i32, i32* %format.size20, align 4
  %7 = insertvalue %String zeroinitializer, i8* %"$format.text32", 0
  %8 = insertvalue %String %7, i32 %"$format.size33", 1
  %9 = insertvalue %String %8, i32 %ADD31, 2
  call void @"io::out"(%String %9)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 232 — variable: reassignment: hp loss, xp gain, mp drain

```ura
// stdout: After combat:
// stdout: HP: 88
// stdout: MP: 40
// stdout: XP: 50
// expect: 0

fn main() i32:
    hp i32 = 100
    mp i32 = 50
    xp i32 = 0

    hp = hp - 12
    mp -= 10
    xp += 50

    io::out("After combat:\n")
    io::out($"HP: {hp}\n")
    io::out($"MP: {mp}\n")
    io::out($"XP: {xp}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [15 x i8] c"After combat:\0A\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"HP: %d\0A\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"MP: %d\0A\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"XP: %d\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %mp = alloca i32, align 4
  %xp = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size9 = alloca i32, align 4
  %format.text15 = alloca i8*, align 8
  %format.size23 = alloca i32, align 4
  %format.text29 = alloca i8*, align 8
  store i32 100, i32* %hp, align 4
  store i32 50, i32* %mp, align 4
  store i32 0, i32* %xp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %SUB = sub i32 %"$hp", 12
  store i32 %SUB, i32* %hp, align 4
  %"$mp" = load i32, i32* %mp, align 4
  %SUB1 = sub i32 %"$mp", 10
  store i32 %SUB1, i32* %mp, align 4
  %"$xp" = load i32, i32* %xp, align 4
  %ADD = add i32 %"$xp", 50
  store i32 %ADD, i32* %xp, align 4
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 14, i32 0 })
  %"$hp2" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$hp2")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD3 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$hp2")
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %1 = insertvalue %String %0, i32 %"$format.size8", 1
  %2 = insertvalue %String %1, i32 %ADD6, 2
  call void @"io::out"(%String %2)
  %"$mp10" = load i32, i32* %mp, align 4
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %"$mp10")
  store i32 %call.snprintf11, i32* %format.size9, align 4
  %"$format.size12" = load i32, i32* %format.size9, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %"$mp10")
  %"$format.size19" = load i32, i32* %format.size9, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size9, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %4 = insertvalue %String %3, i32 %"$format.size22", 1
  %5 = insertvalue %String %4, i32 %ADD20, 2
  call void @"io::out"(%String %5)
  %"$xp24" = load i32, i32* %xp, align 4
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"$xp24")
  store i32 %call.snprintf25, i32* %format.size23, align 4
  %"$format.size26" = load i32, i32* %format.size23, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %AS28 = sext i32 %ADD27 to i64
  %call.calloc30 = call i8* @calloc(i64 %AS28, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc30, i8** %format.text29, align 8
  %"$format.text31" = load i8*, i8** %format.text29, align 8
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text31", i64 %AS28, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"$xp24")
  %"$format.size33" = load i32, i32* %format.size23, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %"$format.text35" = load i8*, i8** %format.text29, align 8
  %"$format.size36" = load i32, i32* %format.size23, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text35", 0
  %7 = insertvalue %String %6, i32 %"$format.size36", 1
  %8 = insertvalue %String %7, i32 %ADD34, 2
  call void @"io::out"(%String %8)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 233 — variable: all primitive types in one place

```ura
// stdout: name:  Aldric
// stdout: hp:    100
// stdout: xp:    999999
// stdout: level: 12
// stdout: grade: S
// stdout: alive: True
// stdout: ratio: 0.850000
// expect: 0

fn main() i32:
    name   char[] = "Aldric"
    hp     i32   = 100
    xp     i64  = 999999 as i64
    level  i16 = 12 as i16
    grade  char  = 'S'
    alive  bool  = True
    ratio  f32 = 0.85

    io::out($"name:  {name}\n")
    io::out($"hp:    {hp}\n")
    io::out($"xp:    {xp}\n")
    io::out($"level: {level}\n")
    io::out($"grade: {grade}\n")
    io::out($"alive: {alive}\n")
    io::out($"ratio: {ratio}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [11 x i8] c"name:  %s\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"hp:    %d\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"xp:    %lld\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"level: %d\0A\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"grade: %c\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [11 x i8] c"alive: %s\0A\00", align 1
@str.8 = private unnamed_addr constant [11 x i8] c"ratio: %f\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  %hp = alloca i32, align 4
  %xp = alloca i64, align 8
  %level = alloca i16, align 2
  %grade = alloca i8, align 1
  %alive = alloca i1, align 1
  %ratio = alloca float, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %format.size19 = alloca i32, align 4
  %format.text24 = alloca i8*, align 8
  %format.size32 = alloca i32, align 4
  %format.text38 = alloca i8*, align 8
  %format.size46 = alloca i32, align 4
  %format.text52 = alloca i8*, align 8
  %format.size60 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text65 = alloca i8*, align 8
  %format.size74 = alloca i32, align 4
  %format.text80 = alloca i8*, align 8
  store [7 x i8] c"Aldric\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 100, i32* %hp, align 4
  store i64 999999, i64* %xp, align 8
  store i16 12, i16* %level, align 2
  store i8 83, i8* %grade, align 1
  store i1 true, i1* %alive, align 1
  store float 0x3FEB333340000000, float* %ratio, align 4
  %"$name" = load i8*, i8** %name, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i8* %"$name")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$hp")
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %5 = insertvalue %String %4, i32 %"$format.size18", 1
  %6 = insertvalue %String %5, i32 %ADD16, 2
  call void @"io::out"(%String %6)
  %"$xp" = load i64, i64* %xp, align 8
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i64 %"$xp")
  store i32 %call.snprintf20, i32* %format.size19, align 4
  %"$format.size21" = load i32, i32* %format.size19, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %AS23 = sext i32 %ADD22 to i64
  %call.calloc25 = call i8* @calloc(i64 %AS23, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc25, i8** %format.text24, align 8
  %"$format.text26" = load i8*, i8** %format.text24, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text26", i64 %AS23, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i64 %"$xp")
  %"$format.size28" = load i32, i32* %format.size19, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text24, align 8
  %"$format.size31" = load i32, i32* %format.size19, align 4
  %7 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %8 = insertvalue %String %7, i32 %"$format.size31", 1
  %9 = insertvalue %String %8, i32 %ADD29, 2
  call void @"io::out"(%String %9)
  %"$level" = load i16, i16* %level, align 2
  %AS33 = sext i16 %"$level" to i32
  %call.snprintf34 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %AS33)
  store i32 %call.snprintf34, i32* %format.size32, align 4
  %"$format.size35" = load i32, i32* %format.size32, align 4
  %ADD36 = add i32 %"$format.size35", 1
  %AS37 = sext i32 %ADD36 to i64
  %call.calloc39 = call i8* @calloc(i64 %AS37, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc39, i8** %format.text38, align 8
  %"$format.text40" = load i8*, i8** %format.text38, align 8
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text40", i64 %AS37, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %AS33)
  %"$format.size42" = load i32, i32* %format.size32, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %"$format.text44" = load i8*, i8** %format.text38, align 8
  %"$format.size45" = load i32, i32* %format.size32, align 4
  %10 = insertvalue %String zeroinitializer, i8* %"$format.text44", 0
  %11 = insertvalue %String %10, i32 %"$format.size45", 1
  %12 = insertvalue %String %11, i32 %ADD43, 2
  call void @"io::out"(%String %12)
  %"$grade" = load i8, i8* %grade, align 1
  %AS47 = sext i8 %"$grade" to i32
  %call.snprintf48 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i32 %AS47)
  store i32 %call.snprintf48, i32* %format.size46, align 4
  %"$format.size49" = load i32, i32* %format.size46, align 4
  %ADD50 = add i32 %"$format.size49", 1
  %AS51 = sext i32 %ADD50 to i64
  %call.calloc53 = call i8* @calloc(i64 %AS51, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc53, i8** %format.text52, align 8
  %"$format.text54" = load i8*, i8** %format.text52, align 8
  %call.snprintf55 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text54", i64 %AS51, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i32 %AS47)
  %"$format.size56" = load i32, i32* %format.size46, align 4
  %ADD57 = add i32 %"$format.size56", 1
  %"$format.text58" = load i8*, i8** %format.text52, align 8
  %"$format.size59" = load i32, i32* %format.size46, align 4
  %13 = insertvalue %String zeroinitializer, i8* %"$format.text58", 0
  %14 = insertvalue %String %13, i32 %"$format.size59", 1
  %15 = insertvalue %String %14, i32 %ADD57, 2
  call void @"io::out"(%String %15)
  store i32 0, i32* %format.size60, align 4
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$alive", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf61 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf61, i32* %format.size60, align 4
  %"$format.size62" = load i32, i32* %format.size60, align 4
  %ADD63 = add i32 %"$format.size62", 1
  %AS64 = sext i32 %ADD63 to i64
  %call.calloc66 = call i8* @calloc(i64 %AS64, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc66, i8** %format.text65, align 8
  %"$format.text67" = load i8*, i8** %format.text65, align 8
  %"$bool.text68" = load i8*, i8** %bool.text, align 8
  %call.snprintf69 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text67", i64 %AS64, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text68")
  %"$format.size70" = load i32, i32* %format.size60, align 4
  %ADD71 = add i32 %"$format.size70", 1
  %"$format.text72" = load i8*, i8** %format.text65, align 8
  %"$format.size73" = load i32, i32* %format.size60, align 4
  %16 = insertvalue %String zeroinitializer, i8* %"$format.text72", 0
  %17 = insertvalue %String %16, i32 %"$format.size73", 1
  %18 = insertvalue %String %17, i32 %ADD71, 2
  call void @"io::out"(%String %18)
  %"$ratio" = load float, float* %ratio, align 4
  %AS75 = fpext float %"$ratio" to double
  %call.snprintf76 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.8, i32 0, i32 0), double %AS75)
  store i32 %call.snprintf76, i32* %format.size74, align 4
  %"$format.size77" = load i32, i32* %format.size74, align 4
  %ADD78 = add i32 %"$format.size77", 1
  %AS79 = sext i32 %ADD78 to i64
  %call.calloc81 = call i8* @calloc(i64 %AS79, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc81, i8** %format.text80, align 8
  %"$format.text82" = load i8*, i8** %format.text80, align 8
  %call.snprintf83 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text82", i64 %AS79, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.8, i32 0, i32 0), double %AS75)
  %"$format.size84" = load i32, i32* %format.size74, align 4
  %ADD85 = add i32 %"$format.size84", 1
  %"$format.text86" = load i8*, i8** %format.text80, align 8
  %"$format.size87" = load i32, i32* %format.size74, align 4
  %19 = insertvalue %String zeroinitializer, i8* %"$format.text86", 0
  %20 = insertvalue %String %19, i32 %"$format.size87", 1
  %21 = insertvalue %String %20, i32 %ADD85, 2
  call void @"io::out"(%String %21)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 234 — variable: a use before a shadowing declaration binds to the outer one

```ura
// stdout: outer 1
// stdout: inner 2
// stdout: after 1
// stdout: loop y 6
// stdout: shadow y 0
// expect: 0

fn pick(n i32) i32:
    return n * 2

fn main() i32:
    x i32 = 1
    if 1 < 2:
        // resolves to the OUTER x, even though an inner x follows
        io::out($"outer {x}\n")
        x i32 = 2
        io::out($"inner {x}\n")
    io::out($"after {x}\n")

    y i32 = pick(3)
    while y > 0:
        io::out($"loop y {y}\n")
        y i32 = 0
        io::out($"shadow y {y}\n")
        break
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"outer %d\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"inner %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after %d\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"loop y %d\0A\00", align 1
@str.4 = private unnamed_addr constant [13 x i8] c"shadow y %d\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @pick(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %x6 = alloca i32, align 4
  %format.size7 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %format.size21 = alloca i32, align 4
  %format.text27 = alloca i8*, align 8
  %y = alloca i32, align 4
  %format.size35 = alloca i32, align 4
  %format.text41 = alloca i8*, align 8
  %y49 = alloca i32, align 4
  %format.size50 = alloca i32, align 4
  %format.text56 = alloca i8*, align 8
  store i32 1, i32* %x, align 4
  br label %if6

if6:                                              ; preds = %entry
  br i1 true, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"$x" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$x")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  store i32 2, i32* %x6, align 4
  %"$x8" = load i32, i32* %x6, align 4
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$x8")
  store i32 %call.snprintf9, i32* %format.size7, align 4
  %"$format.size10" = load i32, i32* %format.size7, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$x8")
  %"$format.size17" = load i32, i32* %format.size7, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"$x22" = load i32, i32* %x, align 4
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"$x22")
  store i32 %call.snprintf23, i32* %format.size21, align 4
  %"$format.size24" = load i32, i32* %format.size21, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %AS26 = sext i32 %ADD25 to i64
  %call.calloc28 = call i8* @calloc(i64 %AS26, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc28, i8** %format.text27, align 8
  %"$format.text29" = load i8*, i8** %format.text27, align 8
  %call.snprintf30 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text29", i64 %AS26, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"$x22")
  %"$format.size31" = load i32, i32* %format.size21, align 4
  %ADD32 = add i32 %"$format.size31", 1
  %"$format.text33" = load i8*, i8** %format.text27, align 8
  %"$format.size34" = load i32, i32* %format.size21, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text33", 0
  %7 = insertvalue %String %6, i32 %"$format.size34", 1
  %8 = insertvalue %String %7, i32 %ADD32, 2
  call void @"io::out"(%String %8)
  %call.pick = call i32 @pick(i32 3)
  store i32 %call.pick, i32* %y, align 4
  br label %while7

while7:                                           ; preds = %endif
  %"$y" = load i32, i32* %y, align 4
  %GT = icmp sgt i32 %"$y", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$y36" = load i32, i32* %y, align 4
  %call.snprintf37 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %"$y36")
  store i32 %call.snprintf37, i32* %format.size35, align 4
  %"$format.size38" = load i32, i32* %format.size35, align 4
  %ADD39 = add i32 %"$format.size38", 1
  %AS40 = sext i32 %ADD39 to i64
  %call.calloc42 = call i8* @calloc(i64 %AS40, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc42, i8** %format.text41, align 8
  %"$format.text43" = load i8*, i8** %format.text41, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text43", i64 %AS40, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %"$y36")
  %"$format.size45" = load i32, i32* %format.size35, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text41, align 8
  %"$format.size48" = load i32, i32* %format.size35, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %10 = insertvalue %String %9, i32 %"$format.size48", 1
  %11 = insertvalue %String %10, i32 %ADD46, 2
  call void @"io::out"(%String %11)
  store i32 0, i32* %y49, align 4
  %"$y51" = load i32, i32* %y49, align 4
  %call.snprintf52 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.4, i32 0, i32 0), i32 %"$y51")
  store i32 %call.snprintf52, i32* %format.size50, align 4
  %"$format.size53" = load i32, i32* %format.size50, align 4
  %ADD54 = add i32 %"$format.size53", 1
  %AS55 = sext i32 %ADD54 to i64
  %call.calloc57 = call i8* @calloc(i64 %AS55, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc57, i8** %format.text56, align 8
  %"$format.text58" = load i8*, i8** %format.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text58", i64 %AS55, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.4, i32 0, i32 0), i32 %"$y51")
  %"$format.size60" = load i32, i32* %format.size50, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %"$format.text62" = load i8*, i8** %format.text56, align 8
  %"$format.size63" = load i32, i32* %format.size50, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text62", 0
  %13 = insertvalue %String %12, i32 %"$format.size63", 1
  %14 = insertvalue %String %13, i32 %ADD61, 2
  call void @"io::out"(%String %14)
  br label %endloop

endloop:                                          ; preds = %while7.then, %while7
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 235 — variable: redeclaring a variable in the same scope

```ura
// error: Redeclaration of variable 'a'

fn main() i32:
    a i32 = 1
    a i32 = 2
    return a
```

### stderr

```
error: test.ura:5:5 'a' is already declared, on line 4
  |
5 |     a i32 = 2
  |     ^
help: use a = 2
error: aborting due to 1 error
```

### status

```
1
```

---

## 236 — variable: using a name that was never declared

```ura
// error: Undeclared variable 'y'

fn main() i32:
    x i32 = 5
    io::out($"{y}\n")
    return 0
```

### stderr

```
error: test.ura:5:16 undeclared variable 'y'
  |
5 |     io::out($"{y}\n")
  |                ^
help: did you mean 'x'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 237 — variable: assigning to a name that was never declared

```ura
// error: Undeclared variable 'score'

fn main() i32:
    score = 100
    io::out($"{score}\n")
    return 0
```

### stderr

```
error: test.ura:4:5 undeclared variable 'score'
  |
4 |     score = 100
  |     ^^^^^
help: declare it first, like: score i32 = 0
error: test.ura:5:16 undeclared variable 'score'
  |
5 |     io::out($"{score}\n")
  |                ^^^^^
help: declare it first, like: score i32 = 0
error: aborting due to 2 errors
```

### status

```
1
```

---

## 238 — variable: a variable does not outlive the block that declared it

```ura
// error: Undeclared variable 'y'

fn main() i32:
    x i32 = 1
    if x > 0:
        y i32 = 5
    io::out($"{y}\n")
    return 0
```

### stderr

```
error: test.ura:7:16 undeclared variable 'y'
  |
7 |     io::out($"{y}\n")
  |                ^
help: did you mean 'x'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 239 — variable: redeclaring a global

```ura
// error: Redeclaration of variable 'score'

score i32 = 0
score i32 = 10

fn main() i32:
    io::out($"{score}\n")
    return 0
```

### stderr

```
error: test.ura:4:1 'score' is already declared, on line 3
  |
4 | score i32 = 10
  | ^^^^^
help: use score = 10
error: aborting due to 1 error
```

### status

```
1
```

---

## 240 — variable: a bool takes only True or False

```ura
// error: Cannot assign i32 to bool

fn main() i32:
    b bool = 5
    return 0
```

### stderr

```
error: test.ura:4:14 expected 'bool', found 'i32'
  |
4 |     b bool = 5
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 241 — variable: assigning a string or a struct to an integer

```ura
// error: Cannot assign array to i32

struct Point:
    x i32

fn main() i32:
    a i32 = "hello"
    p Point
    b i32 = p
    io::out($"{a}{b}\n")
    return 0
```

### stderr

```
error: test.ura:7:13 expected 'i32', found 'String'
  |
7 |     a i32 = "hello"
  |             ^^^^^^^
error: test.ura:9:13 expected 'i32', found 'Point'
  |
9 |     b i32 = p
  |             ^
error: aborting due to 2 errors
```

### status

```
1
```

---

## 242 — proto: a string passed to a fn with an i8[] param

```ura
// stdout: hi ura
// expect: 0
proto printf(fmt i8[], ...) i32

fn greet(name i8[]):
    printf("hi %s\n", name)

fn main() i32:
    greet("ura")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"hi %s\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"ura\00", align 1

declare i32 @printf(i8*, ...)

define void @greet(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %"$name" = load i8*, i8** %name, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8* %"$name")
  ret void
}

define i32 @main() {
entry:
  call void @greet(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}
```

---

## 243 — variables: unary minus on a variable and a parameter

```ura
// expect: 12   (neg(-5) + 7)
fn neg(n i32) i32:
    return -n

fn main() i32:
    a i32 = 5
    b i32 = -a
    return neg(b) + -(-7)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @neg(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %NEG = sub i32 0, %"$n"
  ret i32 %NEG
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %NEG = sub i32 0, %"$a"
  store i32 %NEG, i32* %b, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.neg = call i32 @neg(i32 %"$b")
  %ADD = add i32 %call.neg, 7
  ret i32 %ADD
}
```

---

## 244 — global scope: a top-level statement runs before main

```ura
// expect: 15
count i32 = 10
count += 5

fn main() i32:
    return count
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@count = global i32 0

define i32 @main() {
entry:
  store i32 10, i32* @count, align 4
  %"$count" = load i32, i32* @count, align 4
  %ADD = add i32 %"$count", 5
  store i32 %ADD, i32* @count, align 4
  %"$count1" = load i32, i32* @count, align 4
  ret i32 %"$count1"
}
```

---

## 245 — function: early bare return in a void function

```ura
// expect: 3
total i32 = 0

fn add_small(n i32):
    if n > 2:
        return
    total += n

fn main() i32:
    add_small(1)
    add_small(5)
    add_small(2)
    return total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@total = global i32 0

define void @add_small(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 2
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %"$total" = load i32, i32* @total, align 4
  %ADD = add i32 %"$total", %"$n1"
  store i32 %ADD, i32* @total, align 4
  ret void
}

define i32 @main() {
entry:
  store i32 0, i32* @total, align 4
  call void @add_small(i32 1)
  call void @add_small(i32 5)
  call void @add_small(i32 2)
  %"$total" = load i32, i32* @total, align 4
  ret i32 %"$total"
}
```

---

## 246 — bool: i1 and bool are the same type

```ura
// expect: 2
fn main() i32:
    a i1 = True
    b bool = a
    if b:
        return 2
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  store i1 true, i1* %a, align 1
  %"$a" = load i1, i1* %a, align 1
  store i1 %"$a", i1* %b, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i1, i1* %b, align 1
  br i1 %"$b", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 2

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 247 — tokenizer: a block comment over two lines, /*/ does not close it

```ura
// expect: 5
/*/ this comment opens with slash star slash
   and goes on until here */
fn main() i32:
    return 5
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  ret i32 5
}
```

---

## 248 — calls: arguments over several lines

```ura
// expect: 6
fn add(a i32, b i32, c i32) i32:
    return a + b + c

fn main() i32:
    return add(
        1,
        2,
    3)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @add(i32 %0, i32 %1, i32 %2) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store i32 %2, i32* %c, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  %"$c" = load i32, i32* %c, align 4
  %ADD1 = add i32 %ADD, %"$c"
  ret i32 %ADD1
}

define i32 @main() {
entry:
  %call.add = call i32 @add(i32 1, i32 2, i32 3)
  ret i32 %call.add
}
```

---

## 249 — variables: a number literal takes the type of its i8 target

```ura
// expect: 1
fn main() i32:
    c i8 = 5
    if c == 5:
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 5, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 5
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 250 — variables: a variable first set inside an if still starts at zero

```ura
// expect: 13
fn main() i32:
    a i32 = 10
    b i32
    c i32
    if a > 50:
        c = 7
    if a > 5:
        b = 3
    return a + b + c
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 0, i32* %b, align 4
  store i32 0, i32* %c, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 50
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 7, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a2" = load i32, i32* %a, align 4
  %GT3 = icmp sgt i32 %"$a2", 5
  br i1 %GT3, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  store i32 3, i32* %b, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  %"$a4" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a4", %"$b"
  %"$c" = load i32, i32* %c, align 4
  %ADD5 = add i32 %ADD, %"$c"
  ret i32 %ADD5
}
```

---

## 251 — variables: a variable with the name of a parameter

```ura
fn f(r i32) i32:
    r i32 = 2
    return r

fn main() i32:
    return f(1)
```

### stderr

```
error: test.ura:2:5 'r' is already declared, on line 1
  |
2 |     r i32 = 2
  |     ^
help: use r = 2
error: aborting due to 1 error
```

### status

```
1
```

---

## 252 — unsigned: u8 wraps, u32 divides and compares unsigned, u64, casts

```ura
fn main() i32:
    small u8 = 250
    small += 10
    big u32 = 4000000000
    half u32 = big / 2
    wide u64 = 18000000000
    io::out($"{typeof(small)} {small} {big} {half} {big > 100} {wide}\n")
    io::out($"{sizeof(u16)} {big as f64} {-1 as u8}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@TYPEOF = private unnamed_addr constant [3 x i8] c"u8\00", align 1
@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [21 x i8] c"%s %u %u %u %s %llu\0A\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"%d %f %u\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %small = alloca i8, align 1
  %big = alloca i32, align 4
  %half = alloca i32, align 4
  %wide = alloca i64, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size14 = alloca i32, align 4
  %format.text19 = alloca i8*, align 8
  store i8 -6, i8* %small, align 1
  %"$small" = load i8, i8* %small, align 1
  %ADD = add i8 %"$small", 10
  store i8 %ADD, i8* %small, align 1
  store i32 -294967296, i32* %big, align 4
  %"$big" = load i32, i32* %big, align 4
  %DIV = udiv i32 %"$big", 2
  store i32 %DIV, i32* %half, align 4
  store i64 18000000000, i64* %wide, align 8
  %"$big1" = load i32, i32* %big, align 4
  %GT = icmp ugt i32 %"$big1", 100
  store i32 0, i32* %format.size, align 4
  %"$small2" = load i8, i8* %small, align 1
  %AS = zext i8 %"$small2" to i32
  %"$big3" = load i32, i32* %big, align 4
  %"$half" = load i32, i32* %half, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %GT, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$wide" = load i64, i64* %wide, align 8
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @TYPEOF, i32 0, i32 0), i32 %AS, i32 %"$big3", i32 %"$half", i8* %"$bool.text", i64 %"$wide")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size", 1
  %AS5 = sext i32 %ADD4 to i64
  %call.calloc = call i8* @calloc(i64 %AS5, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS5, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @TYPEOF, i32 0, i32 0), i32 %AS, i32 %"$big3", i32 %"$half", i8* %"$bool.text6", i64 %"$wide")
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %1 = insertvalue %String %0, i32 %"$format.size11", 1
  %2 = insertvalue %String %1, i32 %ADD9, 2
  call void @"io::out"(%String %2)
  %"$big12" = load i32, i32* %big, align 4
  %AS13 = uitofp i32 %"$big12" to double
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 ptrtoint (i16* getelementptr (i16, i16* null, i32 1) to i32), double %AS13, i32 255)
  store i32 %call.snprintf15, i32* %format.size14, align 4
  %"$format.size16" = load i32, i32* %format.size14, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %AS18 = sext i32 %ADD17 to i64
  %call.calloc20 = call i8* @calloc(i64 %AS18, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc20, i8** %format.text19, align 8
  %"$format.text21" = load i8*, i8** %format.text19, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text21", i64 %AS18, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 ptrtoint (i16* getelementptr (i16, i16* null, i32 1) to i32), double %AS13, i32 255)
  %"$format.size23" = load i32, i32* %format.size14, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text19, align 8
  %"$format.size26" = load i32, i32* %format.size14, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %4 = insertvalue %String %3, i32 %"$format.size26", 1
  %5 = insertvalue %String %4, i32 %ADD24, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 253 — unsigned: a number that doesn't fit in u8

```ura
fn main() i32:
    x u8 = 256
    return 0
```

### stderr

```
error: test.ura:2:12 256 doesn't fit in u8
  |
2 |     x u8 = 256
  |            ^^^
help: u8 goes from 0 to 255
error: aborting due to 1 error
```

### status

```
1
```

---

## 254 — variables: the type written before the name

```ura
struct Point:
    x i32

fn main() i32:
    i32 x = 1
    i8[] s = "ab"
    Point p
    p.x = x
    return p.x
```

### stderr

```
error: test.ura:5:5 the type goes after the name
  |
5 |     i32 x = 1
  |     ^^^
help: use x i32 = 1
error: test.ura:6:5 the type goes after the name
  |
6 |     i8[] s = "ab"
  |     ^^^^
help: use s i8[] = "ab"
error: test.ura:7:5 the type goes after the name
  |
7 |     Point p
  |     ^^^^^
help: use p Point
error: aborting due to 3 errors
```

### status

```
1
```

---

## 255 — bool: not, with and / or and in if / elif

```ura
fn main() i32:
    x i32 = 5
    big bool = not (x < 3)
    if not big or x == 0:
        io::out("no\n")
    elif not (x > 10) and big:
        io::out($"yes {not big}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"no\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"yes %s\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %big = alloca i1, align 1
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  store i32 5, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %LT = icmp slt i32 %"$x", 3
  %NOT = xor i1 %LT, true
  store i1 %NOT, i1* %big, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"$big" = load i1, i1* %big, align 1
  %NOT2 = xor i1 %"$big", true
  %"$x3" = load i32, i32* %x, align 4
  %EQ = icmp eq i32 %"$x3", 0
  %OR = or i1 %NOT2, %EQ
  br i1 %OR, label %if6.then, label %elif7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  br label %endif1

elif7:                                            ; preds = %if6
  %"$x4" = load i32, i32* %x, align 4
  %GT = icmp sgt i32 %"$x4", 10
  %NOT5 = xor i1 %GT, true
  %"$big6" = load i1, i1* %big, align 1
  %AND = and i1 %NOT5, %"$big6"
  br i1 %AND, label %elif7.then, label %endif1

elif7.then:                                       ; preds = %elif7
  %"$big7" = load i1, i1* %big, align 1
  %NOT8 = xor i1 %"$big7", true
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if8

if8:                                              ; preds = %elif7.then
  br i1 %NOT8, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text9" = load i8*, i8** %bool.text, align 8
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text9")
  %"$format.size11" = load i32, i32* %format.size, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %"$format.text13" = load i8*, i8** %format.text, align 8
  %"$format.size14" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text13", 0
  %1 = insertvalue %String %0, i32 %"$format.size14", 1
  %2 = insertvalue %String %1, i32 %ADD12, 2
  call void @"io::out"(%String %2)
  br label %endif1

endif1:                                           ; preds = %endif, %elif7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 256 — bool: not on an i32

```ura
fn main() i32:
    x i32 = 5
    return not x
```

### stderr

```
error: test.ura:3:16 expected 'bool', found 'i32'
  |
3 |     return not x
  |                ^
error: test.ura:3:12 expected 'i32', found 'bool'
  |
3 |     return not x
  |            ^^^
error: aborting due to 2 errors
```

### status

```
1
```

---

## 257 — float: 2.5f is an f32

```ura
// stdout: -2.500000 -1.000000 0.500000
fn main() i32:
    a f32 = -2.5f
    b f32 = 1.5f + a
    c f32 = 0.5
    io::out($"{a} {b} {c}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"%f %f %f\0A\00", align 1

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

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
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
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %a = alloca float, align 4
  %b = alloca float, align 4
  %c = alloca float, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store float -2.500000e+00, float* %a, align 4
  %"$a" = load float, float* %a, align 4
  %ADD = fadd float 1.500000e+00, %"$a"
  store float %ADD, float* %b, align 4
  store float 5.000000e-01, float* %c, align 4
  %"$a1" = load float, float* %a, align 4
  %AS = fpext float %"$a1" to double
  %"$b" = load float, float* %b, align 4
  %AS2 = fpext float %"$b" to double
  %"$c" = load float, float* %c, align 4
  %AS3 = fpext float %"$c" to double
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), double %AS, double %AS2, double %AS3)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size", 1
  %AS5 = sext i32 %ADD4 to i64
  %call.calloc = call i8* @calloc(i64 %AS5, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS5, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), double %AS, double %AS2, double %AS3)
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 258 — float: a negative f64 literal into an f32

```ura
// error: expected 'f32', found 'f64'
fn main() i32:
    a f32 = -2.5
    return 0
```

### stderr

```
error: test.ura:3:13 expected 'f32', found 'f64'
  |
3 |     a f32 = -2.5
  |             ^
help: write -2.5f for an f32
error: aborting due to 1 error
```

### status

```
1
```

