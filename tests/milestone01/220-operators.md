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
    output("atk + def:       ", atk + def,       "\n")
    // var - var
    output("atk - def:       ", atk - def,       "\n")
    // var * var
    output("kills * 50:      ", kills * 50,      "\n")
    // var / var
    output("total / count:   ", total / count,   "\n")
    // var % var
    output("atk % xp_cap:    ", atk % xp_cap,    "\n")
    // val + val
    output("10 + 5:          ", 10 + 5,           "\n")
    // fn + var
    output("base_atk + atk:  ", base_atk() + atk,"\n")
    // chained with precedence
    output("2 + 3 * 4:       ", 2 + 3 * 4,       "\n")
    output("(2 + 3) * 4:     ", (2 + 3) * 4,     "\n")

    a f32 = 1.5
    b f32 = 0.3

    output("add:  ", a + b, "\n")  // 1.8
    output("sub:  ", a - b, "\n")  // 1.2
    output("mul:  ", a * b, "\n")  // 0.45
    output("div:  ", a / b, "\n")  // 5.0

    output("lt:   ", a < b,  "\n")  // 0 (false)
    output("gt:   ", a > b,  "\n")  // 1 (true)
    output("lte:  ", a <= b, "\n")  // 0
    output("gte:  ", a >= b, "\n")  // 1

    t f32 = 0.8 - 0.28
    output("clamp(0.52): ", clamp(t), "\n")  // 0.52
    output("clamp(0.01): ", clamp(0.01), "\n")  // 0.05
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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

define i32 @base_atk() {
entry:
  ret i32 10
}

define float @clamp(float %0) {
entry:
  %t = alloca float, align 4
  store float %0, float* %t, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$t" = load float, float* %t, align 4
  %LT = fcmp olt float %"$t", 0x3FA99999A0000000
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret float 0x3FA99999A0000000

endif:                                            ; preds = %if0
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
  %a = alloca float, align 4
  %b = alloca float, align 4
  %bool.text = alloca i8*, align 8
  %bool.text39 = alloca i8*, align 8
  %bool.text44 = alloca i8*, align 8
  %bool.text49 = alloca i8*, align 8
  %t = alloca float, align 4
  store i32 25, i32* %atk, align 4
  store i32 8, i32* %def, align 4
  store i32 3, i32* %kills, align 4
  store i32 100, i32* %total, align 4
  store i32 4, i32* %count, align 4
  store i32 70, i32* %xp_cap, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %"$def" = load i32, i32* %def, align 4
  %ADD = add i32 %"$atk", %"$def"
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %ADD)
  %"$atk4" = load i32, i32* %atk, align 4
  %"$def5" = load i32, i32* %def, align 4
  %SUB = sub i32 %"$atk4", %"$def5"
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  %"$kills" = load i32, i32* %kills, align 4
  %MUL = mul i32 %"$kills", 50
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  %"$total" = load i32, i32* %total, align 4
  %"$count" = load i32, i32* %count, align 4
  %DIV = sdiv i32 %"$total", %"$count"
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.3, i32 0, i32 0), i32 %DIV)
  %"$atk9" = load i32, i32* %atk, align 4
  %"$xp_cap" = load i32, i32* %xp_cap, align 4
  %MOD = srem i32 %"$atk9", %"$xp_cap"
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %MOD)
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.5, i32 0, i32 0), i32 15)
  %call.base_atk = call i32 @base_atk()
  %"$atk12" = load i32, i32* %atk, align 4
  %ADD13 = add i32 %call.base_atk, %"$atk12"
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.6, i32 0, i32 0), i32 %ADD13)
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.7, i32 0, i32 0), i32 14)
  %call.dprintf16 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.8, i32 0, i32 0), i32 20)
  store float 1.500000e+00, float* %a, align 4
  store float 0x3FD3333340000000, float* %b, align 4
  %"$a" = load float, float* %a, align 4
  %"$b" = load float, float* %b, align 4
  %ADD17 = fadd float %"$a", %"$b"
  %AS = fpext float %ADD17 to double
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.9, i32 0, i32 0), double %AS)
  %"$a19" = load float, float* %a, align 4
  %"$b20" = load float, float* %b, align 4
  %SUB21 = fsub float %"$a19", %"$b20"
  %AS22 = fpext float %SUB21 to double
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.10, i32 0, i32 0), double %AS22)
  %"$a24" = load float, float* %a, align 4
  %"$b25" = load float, float* %b, align 4
  %MUL26 = fmul float %"$a24", %"$b25"
  %AS27 = fpext float %MUL26 to double
  %call.dprintf28 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.11, i32 0, i32 0), double %AS27)
  %"$a29" = load float, float* %a, align 4
  %"$b30" = load float, float* %b, align 4
  %DIV31 = fdiv float %"$a29", %"$b30"
  %AS32 = fpext float %DIV31 to double
  %call.dprintf33 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), double %AS32)
  %"$a34" = load float, float* %a, align 4
  %"$b35" = load float, float* %b, align 4
  %LT = fcmp olt float %"$a34", %"$b35"
  store i8* null, i8** %bool.text, align 8
  br label %if1

if1:                                              ; preds = %entry
  br i1 %LT, label %if1.then, label %else2

if1.then:                                         ; preds = %if1
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.13, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else2:                                            ; preds = %if1
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else2, %if1.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf36 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text")
  %"$a37" = load float, float* %a, align 4
  %"$b38" = load float, float* %b, align 4
  %GT = fcmp ogt float %"$a37", %"$b38"
  store i8* null, i8** %bool.text39, align 8
  br label %if3

if3:                                              ; preds = %endif
  br i1 %GT, label %if3.then, label %else4

if3.then:                                         ; preds = %if3
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif1

else4:                                            ; preds = %if3
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif1

endif1:                                           ; preds = %else4, %if3.then
  %"$bool.text40" = load i8*, i8** %bool.text39, align 8
  %call.dprintf41 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text40")
  %"$a42" = load float, float* %a, align 4
  %"$b43" = load float, float* %b, align 4
  %LE = fcmp ole float %"$a42", %"$b43"
  store i8* null, i8** %bool.text44, align 8
  br label %if5

if5:                                              ; preds = %endif1
  br i1 %LE, label %if5.then, label %else6

if5.then:                                         ; preds = %if5
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.19, i32 0, i32 0), i8** %bool.text44, align 8
  br label %endif2

else6:                                            ; preds = %if5
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.20, i32 0, i32 0), i8** %bool.text44, align 8
  br label %endif2

endif2:                                           ; preds = %else6, %if5.then
  %"$bool.text45" = load i8*, i8** %bool.text44, align 8
  %call.dprintf46 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0), i8* %"$bool.text45")
  %"$a47" = load float, float* %a, align 4
  %"$b48" = load float, float* %b, align 4
  %GE = fcmp oge float %"$a47", %"$b48"
  store i8* null, i8** %bool.text49, align 8
  br label %if7

if7:                                              ; preds = %endif2
  br i1 %GE, label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.22, i32 0, i32 0), i8** %bool.text49, align 8
  br label %endif3

else8:                                            ; preds = %if7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.23, i32 0, i32 0), i8** %bool.text49, align 8
  br label %endif3

endif3:                                           ; preds = %else8, %if7.then
  %"$bool.text50" = load i8*, i8** %bool.text49, align 8
  %call.dprintf51 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0), i8* %"$bool.text50")
  store float 0x3FE0A3D700000000, float* %t, align 4
  %"$t" = load float, float* %t, align 4
  %call.clamp = call float @clamp(float %"$t")
  %AS52 = fpext float %call.clamp to double
  %call.dprintf53 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.25, i32 0, i32 0), double %AS52)
  %call.clamp54 = call float @clamp(float 0x3F847AE140000000)
  %AS55 = fpext float %call.clamp54 to double
  %call.dprintf56 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.26, i32 0, i32 0), double %AS55)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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

    output("hero_hp == min_hp: ", hero_hp == min_hp, "\n")   // True
    output("hero_hp != orc_hp: ", hero_hp != orc_hp, "\n")   // True
    output("orc_hp < hero_hp:  ", orc_hp < hero_hp,  "\n")   // True
    output("orc_hp > hero_hp:  ", orc_hp > hero_hp,  "\n")   // False
    output("orc_hp <= 60:      ", orc_hp <= 60,       "\n")   // True
    output("hero_hp >= 100:    ", hero_hp >= 100,      "\n")   // True

    // comparison with fn call
    if enemy_atk() > 15:
        output("strong enemy\n")
    else:
        output("weak enemy\n")

    // chained via if
    if hero_hp == 100 and orc_hp < hero_hp:
        output("hero is at full health and winning\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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

define i32 @enemy_atk() {
entry:
  ret i32 18
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  %orc_hp = alloca i32, align 4
  %min_hp = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text9 = alloca i8*, align 8
  %bool.text14 = alloca i8*, align 8
  %bool.text19 = alloca i8*, align 8
  %bool.text23 = alloca i8*, align 8
  %bool.text27 = alloca i8*, align 8
  store i32 100, i32* %hero_hp, align 4
  store i32 60, i32* %orc_hp, align 4
  store i32 100, i32* %min_hp, align 4
  %"$hero_hp" = load i32, i32* %hero_hp, align 4
  %"$min_hp" = load i32, i32* %min_hp, align 4
  %EQ = icmp eq i32 %"$hero_hp", %"$min_hp"
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %EQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  %"$hero_hp8" = load i32, i32* %hero_hp, align 4
  %"$orc_hp" = load i32, i32* %orc_hp, align 4
  %NQ = icmp ne i32 %"$hero_hp8", %"$orc_hp"
  store i8* null, i8** %bool.text9, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %NQ, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text10" = load i8*, i8** %bool.text9, align 8
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text10")
  %"$orc_hp12" = load i32, i32* %orc_hp, align 4
  %"$hero_hp13" = load i32, i32* %hero_hp, align 4
  %LT = icmp slt i32 %"$orc_hp12", %"$hero_hp13"
  store i8* null, i8** %bool.text14, align 8
  br label %if4

if4:                                              ; preds = %endif1
  br i1 %LT, label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif2

else5:                                            ; preds = %if4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif2

endif2:                                           ; preds = %else5, %if4.then
  %"$bool.text15" = load i8*, i8** %bool.text14, align 8
  %call.dprintf16 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text15")
  %"$orc_hp17" = load i32, i32* %orc_hp, align 4
  %"$hero_hp18" = load i32, i32* %hero_hp, align 4
  %GT = icmp sgt i32 %"$orc_hp17", %"$hero_hp18"
  store i8* null, i8** %bool.text19, align 8
  br label %if6

if6:                                              ; preds = %endif2
  br i1 %GT, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif3

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.10, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif3

endif3:                                           ; preds = %else7, %if6.then
  %"$bool.text20" = load i8*, i8** %bool.text19, align 8
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text20")
  %"$orc_hp22" = load i32, i32* %orc_hp, align 4
  %LE = icmp sle i32 %"$orc_hp22", 60
  store i8* null, i8** %bool.text23, align 8
  br label %if8

if8:                                              ; preds = %endif3
  br i1 %LE, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text23, align 8
  br label %endif4

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text23, align 8
  br label %endif4

endif4:                                           ; preds = %else9, %if8.then
  %"$bool.text24" = load i8*, i8** %bool.text23, align 8
  %call.dprintf25 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text24")
  %"$hero_hp26" = load i32, i32* %hero_hp, align 4
  %GE = icmp sge i32 %"$hero_hp26", 100
  store i8* null, i8** %bool.text27, align 8
  br label %if10

if10:                                             ; preds = %endif4
  br i1 %GE, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.15, i32 0, i32 0), i8** %bool.text27, align 8
  br label %endif5

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.16, i32 0, i32 0), i8** %bool.text27, align 8
  br label %endif5

endif5:                                           ; preds = %else11, %if10.then
  %"$bool.text28" = load i8*, i8** %bool.text27, align 8
  %call.dprintf29 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.17, i32 0, i32 0), i8* %"$bool.text28")
  br label %if12

if12:                                             ; preds = %endif5
  %call.enemy_atk = call i32 @enemy_atk()
  %GT30 = icmp sgt i32 %call.enemy_atk, 15
  br i1 %GT30, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.18, i32 0, i32 0))
  br label %endif6

else13:                                           ; preds = %if12
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.19, i32 0, i32 0))
  br label %endif6

endif6:                                           ; preds = %else13, %if12.then
  br label %if14

if14:                                             ; preds = %endif6
  %"$hero_hp33" = load i32, i32* %hero_hp, align 4
  %EQ34 = icmp eq i32 %"$hero_hp33", 100
  %"$orc_hp35" = load i32, i32* %orc_hp, align 4
  %"$hero_hp36" = load i32, i32* %hero_hp, align 4
  %LT37 = icmp slt i32 %"$orc_hp35", %"$hero_hp36"
  %AND = and i1 %EQ34, %LT37
  br i1 %AND, label %if14.then, label %endif7

if14.then:                                        ; preds = %if14
  %call.dprintf38 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.20, i32 0, i32 0))
  br label %endif7

endif7:                                           ; preds = %if14.then, %if14
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
target triple = "x86_64-unknown-linux-gnu"

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
    output(x, "\n")
    x -= 3
    output(x, "\n")
    x *= 4
    output(x, "\n")
    x /= 6
    output(x, "\n")
    x %= 5
    output(x, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 10, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x", 5
  store i32 %ADD, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x1")
  %"$x2" = load i32, i32* %x, align 4
  %SUB = sub i32 %"$x2", 3
  store i32 %SUB, i32* %x, align 4
  %"$x3" = load i32, i32* %x, align 4
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x3")
  %"$x5" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x5", 4
  store i32 %MUL, i32* %x, align 4
  %"$x6" = load i32, i32* %x, align 4
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$x6")
  %"$x8" = load i32, i32* %x, align 4
  %DIV = sdiv i32 %"$x8", 6
  store i32 %DIV, i32* %x, align 4
  %"$x9" = load i32, i32* %x, align 4
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$x9")
  %"$x11" = load i32, i32* %x, align 4
  %MOD = srem i32 %"$x11", 5
  store i32 %MOD, i32* %x, align 4
  %"$x12" = load i32, i32* %x, align 4
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$x12")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
    output("can_proceed: ", can_proceed, "\n")   // True

    // ||
    escape bool = hp < 10 || keys == 0
    output("escape:      ", escape,      "\n")   // False

    // !
    blocked bool = !can_proceed
    output("blocked:     ", blocked,     "\n")   // False

    // and / or / not keywords
    if is_alive(hp) and not exhausted:
        output("hero fights\n")

    if hp < 10 or keys == 0:
        output("hero retreats\n")
    else:
        output("hero advances\n")

    // f64 negation
    alive bool = is_alive(hp)
    output("!!alive: ", !(!alive), "\n")   // True
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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
  %bool.text = alloca i8*, align 8
  %escape = alloca i1, align 1
  %bool.text9 = alloca i8*, align 8
  %blocked = alloca i1, align 1
  %bool.text14 = alloca i8*, align 8
  %alive = alloca i1, align 1
  %bool.text34 = alloca i8*, align 8
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
  %"$can_proceed" = load i1, i1* %can_proceed, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$can_proceed", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  %"$hp7" = load i32, i32* %hp, align 4
  %LT = icmp slt i32 %"$hp7", 10
  %"$keys8" = load i32, i32* %keys, align 4
  %EQ = icmp eq i32 %"$keys8", 0
  %OR = or i1 %LT, %EQ
  store i1 %OR, i1* %escape, align 1
  %"$escape" = load i1, i1* %escape, align 1
  store i8* null, i8** %bool.text9, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %"$escape", label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text10" = load i8*, i8** %bool.text9, align 8
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text10")
  %"$can_proceed12" = load i1, i1* %can_proceed, align 1
  %NOT13 = xor i1 %"$can_proceed12", true
  store i1 %NOT13, i1* %blocked, align 1
  %"$blocked" = load i1, i1* %blocked, align 1
  store i8* null, i8** %bool.text14, align 8
  br label %if4

if4:                                              ; preds = %endif1
  br i1 %"$blocked", label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif2

else5:                                            ; preds = %if4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif2

endif2:                                           ; preds = %else5, %if4.then
  %"$bool.text15" = load i8*, i8** %bool.text14, align 8
  %call.dprintf16 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text15")
  br label %if6

if6:                                              ; preds = %endif2
  %"$hp17" = load i32, i32* %hp, align 4
  %call.is_alive18 = call i1 @is_alive(i32 %"$hp17")
  %"$exhausted19" = load i1, i1* %exhausted, align 1
  %NOT20 = xor i1 %"$exhausted19", true
  %AND21 = and i1 %call.is_alive18, %NOT20
  br i1 %AND21, label %if6.then, label %endif3

if6.then:                                         ; preds = %if6
  %call.dprintf22 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.9, i32 0, i32 0))
  br label %endif3

endif3:                                           ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif3
  %"$hp23" = load i32, i32* %hp, align 4
  %LT24 = icmp slt i32 %"$hp23", 10
  %"$keys25" = load i32, i32* %keys, align 4
  %EQ26 = icmp eq i32 %"$keys25", 0
  %OR27 = or i1 %LT24, %EQ26
  br i1 %OR27, label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  %call.dprintf28 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.10, i32 0, i32 0))
  br label %endif4

else8:                                            ; preds = %if7
  %call.dprintf29 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.11, i32 0, i32 0))
  br label %endif4

endif4:                                           ; preds = %else8, %if7.then
  %"$hp30" = load i32, i32* %hp, align 4
  %call.is_alive31 = call i1 @is_alive(i32 %"$hp30")
  store i1 %call.is_alive31, i1* %alive, align 1
  %"$alive" = load i1, i1* %alive, align 1
  %NOT32 = xor i1 %"$alive", true
  %NOT33 = xor i1 %NOT32, true
  store i8* null, i8** %bool.text34, align 8
  br label %if9

if9:                                              ; preds = %endif4
  br i1 %NOT33, label %if9.then, label %else10

if9.then:                                         ; preds = %if9
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif5

else10:                                           ; preds = %if9
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif5

endif5:                                           ; preds = %else10, %if9.then
  %"$bool.text35" = load i8*, i8** %bool.text34, align 8
  %call.dprintf36 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text35")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
    output(a < b and b == 9, "\n")
    output(a > b or b == 9, "\n")
    output(a < b and a > b, "\n")
    output(a < b or a > b, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text9 = alloca i8*, align 8
  %bool.text19 = alloca i8*, align 8
  %bool.text29 = alloca i8*, align 8
  store i32 5, i32* %a, align 4
  store i32 9, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %LT = icmp slt i32 %"$a", %"$b"
  %"$b4" = load i32, i32* %b, align 4
  %EQ = icmp eq i32 %"$b4", 9
  %AND = and i1 %LT, %EQ
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %AND, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  %"$a5" = load i32, i32* %a, align 4
  %"$b6" = load i32, i32* %b, align 4
  %GT = icmp sgt i32 %"$a5", %"$b6"
  %"$b7" = load i32, i32* %b, align 4
  %EQ8 = icmp eq i32 %"$b7", 9
  %OR = or i1 %GT, %EQ8
  store i8* null, i8** %bool.text9, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %OR, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text10" = load i8*, i8** %bool.text9, align 8
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text10")
  %"$a12" = load i32, i32* %a, align 4
  %"$b13" = load i32, i32* %b, align 4
  %LT14 = icmp slt i32 %"$a12", %"$b13"
  %"$a15" = load i32, i32* %a, align 4
  %"$b16" = load i32, i32* %b, align 4
  %GT17 = icmp sgt i32 %"$a15", %"$b16"
  %AND18 = and i1 %LT14, %GT17
  store i8* null, i8** %bool.text19, align 8
  br label %if4

if4:                                              ; preds = %endif1
  br i1 %AND18, label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif2

else5:                                            ; preds = %if4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif2

endif2:                                           ; preds = %else5, %if4.then
  %"$bool.text20" = load i8*, i8** %bool.text19, align 8
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text20")
  %"$a22" = load i32, i32* %a, align 4
  %"$b23" = load i32, i32* %b, align 4
  %LT24 = icmp slt i32 %"$a22", %"$b23"
  %"$a25" = load i32, i32* %a, align 4
  %"$b26" = load i32, i32* %b, align 4
  %GT27 = icmp sgt i32 %"$a25", %"$b26"
  %OR28 = or i1 %LT24, %GT27
  store i8* null, i8** %bool.text29, align 8
  br label %if6

if6:                                              ; preds = %endif2
  br i1 %OR28, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i8** %bool.text29, align 8
  br label %endif3

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.10, i32 0, i32 0), i8** %bool.text29, align 8
  br label %endif3

endif3:                                           ; preds = %else7, %if6.then
  %"$bool.text30" = load i8*, i8** %bool.text29, align 8
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text30")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
    output("=== ", name, " enters the dungeon ===\n")
    output("HP: ", hp, " | MP: ", mp, " | Speed: ", speed, "\n")
    output("Alive: ", alive, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [31 x i8] c"=== %s enters the dungeon ===\0A\00", align 1
@str.1 = private unnamed_addr constant [29 x i8] c"HP: %d | MP: %d | Speed: %f\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  %hp = alloca i32, align 4
  %mp = alloca i32, align 4
  %alive = alloca i1, align 1
  %speed = alloca float, align 4
  %bool.text = alloca i8*, align 8
  store [7 x i8] c"Aldric\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 100, i32* %hp, align 4
  store i32 50, i32* %mp, align 4
  store i1 true, i1* %alive, align 1
  store float 1.500000e+00, float* %speed, align 4
  %"$name" = load i8*, i8** %name, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$hp" = load i32, i32* %hp, align 4
  %"$mp" = load i32, i32* %mp, align 4
  %"$speed" = load float, float* %speed, align 4
  %AS = fpext float %"$speed" to double
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @str.1, i32 0, i32 0), i32 %"$hp", i32 %"$mp", double %AS)
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$alive", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

    output("After combat:\n")
    output("HP: ", hp, "\n")
    output("MP: ", mp, "\n")
    output("XP: ", xp, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [15 x i8] c"After combat:\0A\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"HP: %d\0A\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"MP: %d\0A\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"XP: %d\0A\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %mp = alloca i32, align 4
  %xp = alloca i32, align 4
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
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0))
  %"$hp2" = load i32, i32* %hp, align 4
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$hp2")
  %"$mp4" = load i32, i32* %mp, align 4
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %"$mp4")
  %"$xp6" = load i32, i32* %xp, align 4
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"$xp6")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

    output("name:  ", name,  "\n")
    output("hp:    ", hp,    "\n")
    output("xp:    ", xp,    "\n")
    output("level: ", level, "\n")
    output("grade: ", grade, "\n")
    output("alive: ", alive, "\n")
    output("ratio: ", ratio, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [11 x i8] c"name:  %s\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"hp:    %d\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"xp:    %lld\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"level: %d\0A\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"grade: %c\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [11 x i8] c"alive: %s\0A\00", align 1
@str.8 = private unnamed_addr constant [11 x i8] c"ratio: %f\0A\00", align 1

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
  %bool.text = alloca i8*, align 8
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
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$hp" = load i32, i32* %hp, align 4
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$hp")
  %"$xp" = load i64, i64* %xp, align 8
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i64 %"$xp")
  %"$level" = load i16, i16* %level, align 2
  %AS = sext i16 %"$level" to i32
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %AS)
  %"$grade" = load i8, i8* %grade, align 1
  %AS4 = sext i8 %"$grade" to i32
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i32 %AS4)
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$alive", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text")
  %"$ratio" = load float, float* %ratio, align 4
  %AS7 = fpext float %"$ratio" to double
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.8, i32 0, i32 0), double %AS7)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
        output("outer ", x, "\n")
        x i32 = 2
        output("inner ", x, "\n")
    output("after ", x, "\n")

    y i32 = pick(3)
    while y > 0:
        output("loop y ", y, "\n")
        y i32 = 0
        output("shadow y ", y, "\n")
        break
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"outer %d\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"inner %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after %d\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"loop y %d\0A\00", align 1
@str.4 = private unnamed_addr constant [13 x i8] c"shadow y %d\0A\00", align 1

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
  %x1 = alloca i32, align 4
  %y = alloca i32, align 4
  %y8 = alloca i32, align 4
  store i32 1, i32* %x, align 4
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$x" = load i32, i32* %x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32 2, i32* %x1, align 4
  %"$x2" = load i32, i32* %x1, align 4
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$x2")
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$x4" = load i32, i32* %x, align 4
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"$x4")
  %call.pick = call i32 @pick(i32 3)
  store i32 %call.pick, i32* %y, align 4
  br label %while1

while1:                                           ; preds = %endif
  %"$y" = load i32, i32* %y, align 4
  %GT = icmp sgt i32 %"$y", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$y6" = load i32, i32* %y, align 4
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %"$y6")
  store i32 0, i32* %y8, align 4
  %"$y9" = load i32, i32* %y8, align 4
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.4, i32 0, i32 0), i32 %"$y9")
  br label %endloop

endloop:                                          ; preds = %while1.then, %while1
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
    output(y, "\n")
    return 0
```

### stderr

```
error: test.ura:5:12 undeclared variable 'y'
  |
5 |     output(y, "\n")
  |            ^
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
    output(score, "\n")
    return 0
```

### stderr

```
error: test.ura:4:5 undeclared variable 'score'
  |
4 |     score = 100
  |     ^^^^^
help: declare it first, like: score i32 = 0
error: test.ura:5:12 undeclared variable 'score'
  |
5 |     output(score, "\n")
  |            ^^^^^
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
    output(y, "\n")
    return 0
```

### stderr

```
error: test.ura:7:12 undeclared variable 'y'
  |
7 |     output(y, "\n")
  |            ^
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
    output(score, "\n")
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
    output(a, b, "\n")
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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
target triple = "x86_64-unknown-linux-gnu"

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
    output(typeof(small), " ", small, " ", big, " ", half, " ", big > 100, " ", wide, "\n")
    output(sizeof(u16), " ", big as f64, " ", -1 as u8, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@TYPEOF = private unnamed_addr constant [3 x i8] c"u8\00", align 1
@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [21 x i8] c"%s %u %u %u %s %llu\0A\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"%d %f %u\0A\00", align 1

define i32 @main() {
entry:
  %small = alloca i8, align 1
  %big = alloca i32, align 4
  %half = alloca i32, align 4
  %wide = alloca i64, align 8
  %bool.text = alloca i8*, align 8
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
  %"$small2" = load i8, i8* %small, align 1
  %AS = zext i8 %"$small2" to i32
  %"$big3" = load i32, i32* %big, align 4
  %"$half" = load i32, i32* %half, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %GT, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$wide" = load i64, i64* %wide, align 8
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @TYPEOF, i32 0, i32 0), i32 %AS, i32 %"$big3", i32 %"$half", i8* %"$bool.text", i64 %"$wide")
  %"$big4" = load i32, i32* %big, align 4
  %AS5 = uitofp i32 %"$big4" to double
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 ptrtoint (i16* getelementptr (i16, i16* null, i32 1) to i32), double %AS5, i32 255)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
        output("no\n")
    elif not (x > 10) and big:
        output("yes ", not big, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"no\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"yes %s\0A\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %big = alloca i1, align 1
  %bool.text = alloca i8*, align 8
  store i32 5, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %LT = icmp slt i32 %"$x", 3
  %NOT = xor i1 %LT, true
  store i1 %NOT, i1* %big, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$big" = load i1, i1* %big, align 1
  %NOT2 = xor i1 %"$big", true
  %"$x3" = load i32, i32* %x, align 4
  %EQ = icmp eq i32 %"$x3", 0
  %OR = or i1 %NOT2, %EQ
  br i1 %OR, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  br label %endif1

elif1:                                            ; preds = %if0
  %"$x4" = load i32, i32* %x, align 4
  %GT = icmp sgt i32 %"$x4", 10
  %NOT5 = xor i1 %GT, true
  %"$big6" = load i1, i1* %big, align 1
  %AND = and i1 %NOT5, %"$big6"
  br i1 %AND, label %elif1.then, label %endif1

elif1.then:                                       ; preds = %elif1
  %"$big7" = load i1, i1* %big, align 1
  %NOT8 = xor i1 %"$big7", true
  store i8* null, i8** %bool.text, align 8
  br label %if2

if2:                                              ; preds = %elif1.then
  br i1 %NOT8, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else3, %if2.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  br label %endif1

endif1:                                           ; preds = %endif, %elif1, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
    output(a, " ", b, " ", c, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"%f %f %f\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca float, align 4
  %b = alloca float, align 4
  %c = alloca float, align 4
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
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), double %AS, double %AS2, double %AS3)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

