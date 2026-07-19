# projects / ura-libft

## index

- isalnum
- isalpha
- isdigit

## isalnum

```ura
// libft/isalnum.ura
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'
fn isdigit(c char) bool: return c >= '1' and c <= '9'
fn isalnum(c char) bool: return isalpha(c) or isdigit(c)

main():
    if isalnum('1'):
        output("is alnum\n")
    else:
        output("is not alnum\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn isalpha(c : char) : bool
└─ return
   └─ and : bool
      ├─ >= : bool
      │  ├─ c : char
      │  └─ char 'a'
      └─ <= : bool
         ├─ c : char
         └─ char 'z'

fn isdigit(c : char) : bool
└─ return
   └─ and : bool
      ├─ >= : bool
      │  ├─ c : char
      │  └─ char '1'
      └─ <= : bool
         ├─ c : char
         └─ char '9'

fn isalnum(c : char) : bool
└─ return
   └─ or : bool
      ├─ call isalpha : bool
      │  └─ c : char
      └─ call isdigit : bool
         └─ c : char

fn main() : i32
└─ if
   ├─ condition call isalnum : bool
   │  └─ char '1'
   ├─ output : void
   │  └─ chars "is alnum\n"
   └─ else
      └─ output : void
         └─ chars "is not alnum\n"
```

```out
is alnum
```

```err
```

```ll

@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i1 @isalpha(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c1, 97
  %c2 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c2, 122
  %and = and i1 %ge, %le
  ret i1 %and
}

define i1 @isdigit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c1, 49
  %c2 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c2, 57
  %and = and i1 %ge, %le
  ret i1 %and
}

define i1 @isalnum(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %call = call i1 @isalpha(i8 %c1)
  %c2 = load i8, i8* %c, align 1
  %call3 = call i1 @isdigit(i8 %c2)
  %or = or i1 %call, %call3
  ret i1 %or
}

define i32 @main() {
entry:
  %call = call i1 @isalnum(i8 49)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## isalpha

```ura
// libft/isalpha.ura
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'

main():
    if isalpha('1'):
        output("is alpha\n")
    else:
        output("is not alpha\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn isalpha(c : char) : bool
└─ return
   └─ and : bool
      ├─ >= : bool
      │  ├─ c : char
      │  └─ char 'a'
      └─ <= : bool
         ├─ c : char
         └─ char 'z'

fn main() : i32
└─ if
   ├─ condition call isalpha : bool
   │  └─ char '1'
   ├─ output : void
   │  └─ chars "is alpha\n"
   └─ else
      └─ output : void
         └─ chars "is not alpha\n"
```

```out
is not alpha
```

```err
```

```ll

@str = private unnamed_addr constant [10 x i8] c"is alpha\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alpha\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i1 @isalpha(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c1, 97
  %c2 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c2, 122
  %and = and i1 %ge, %le
  ret i1 %and
}

define i32 @main() {
entry:
  %call = call i1 @isalpha(i8 49)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## isdigit

```ura
// libft/isdigit.ura
fn isdigit(c char) bool: return c >= '1' and c <= '9'

main():
    if isdigit('1'):
        output("is alnum\n")
    else:
        output("is not alnum\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn isdigit(c : char) : bool
└─ return
   └─ and : bool
      ├─ >= : bool
      │  ├─ c : char
      │  └─ char '1'
      └─ <= : bool
         ├─ c : char
         └─ char '9'

fn main() : i32
└─ if
   ├─ condition call isdigit : bool
   │  └─ char '1'
   ├─ output : void
   │  └─ chars "is alnum\n"
   └─ else
      └─ output : void
         └─ chars "is not alnum\n"
```

```out
is alnum
```

```err
```

```ll

@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i1 @isdigit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c1, 49
  %c2 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c2, 57
  %and = and i1 %ge, %le
  ret i1 %and
}

define i32 @main() {
entry:
  %call = call i1 @isdigit(i8 49)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```
