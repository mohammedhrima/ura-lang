# builtins / c-funcs

## index

- 004 — proto printf: hello world with a format string

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
