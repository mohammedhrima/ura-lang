# builtins / c-funcs

## index

- 003 — puts: simple messages, char comparison on hero grade
- 004 — proto printf: hello world with a format string

## 003 — puts: simple messages, char comparison on hero grade

```ura
// builtins/c-funcs/003.ura - puts: simple messages, char comparison on hero grade

proto fn puts(str chars) int

fn is_s_rank(grade char) bool:
    return grade == 'S'

main():
    grade char = 'S'
    if is_s_rank(grade):
        puts("S-rank hero — unstoppable")
    else:
        puts("keep training")
```

```out
S-rank hero — unstoppable
```

```err
```

```ll

@str = private unnamed_addr constant [28 x i8] c"S-rank hero \E2\80\94 unstoppable\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"keep training\00", align 1

declare i32 @puts(i8*)

define i1 @is_s_rank(i8 %0) {
entry:
  %grade = alloca i8, align 1
  store i8 %0, i8* %grade, align 1
  %grade1 = load i8, i8* %grade, align 1
  %eq = icmp eq i8 %grade1, 83
  ret i1 %eq
}

define i32 @main() {
entry:
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  %grade1 = load i8, i8* %grade, align 1
  %call = call i1 @is_s_rank(i8 %grade1)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %call2 = call i32 @puts(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %call3 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}
```

## 004 — proto printf: hello world with a format string

```ura
// builtins/c-funcs/004.ura - proto printf: hello world with a format string

proto fn printf(fmt chars, ...) int

main():
    s chars = "ura"
    printf("hello %s %d\n", s, 42)
    return 0
```

```out
hello ura 42
```

```err
```

```ll

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"hello %s %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %s, align 8
  %s1 = load i8*, i8** %s, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i8* %s1, i32 42)
  ret i32 0
}
```
