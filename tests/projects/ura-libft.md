# projects / ura-libft

## index

- isalnum
- isalpha
- isdigit
- lstnew — new nodes, add_back, add_front and size
- lstlast — returns a reference to the last node
- lstiter — apply a function to each node's content in place
- lstdelone — unlink a node by index and clear the list

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
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

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
   │  └─ char[] "is alnum\n"
   └─ else
      └─ output : void
         └─ char[] "is not alnum\n"
```

```out
is alnum
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i1 @isalnum(i8 49)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
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
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

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
   │  └─ char[] "is alpha\n"
   └─ else
      └─ output : void
         └─ char[] "is not alpha\n"
```

```out
is not alpha
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"is alpha\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alpha\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i1 @isalpha(i8 49)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
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
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

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
   │  └─ char[] "is alnum\n"
   └─ else
      └─ output : void
         └─ char[] "is not alnum\n"
```

```out
is alnum
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i1 @isdigit(i8 49)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## lstnew — new nodes, add_back, add_front and size

```ura
struct Node:
    content char[]
    ref? next Node

struct Lst:
    ref? head Node

fn ft_lstnew(content char[]) ref Node:
    n Node[] = new Node[1]
    n[0].content = content
    return ref n[0]

fn ft_lstadd_front(ref lst Lst, ref node Node) void:
    node.next = ref lst.head
    lst.head  = ref node

fn ft_lstadd_back(ref lst Lst, ref node Node) void:
    if lst.head == null:
        lst.head = ref node
        return
    ref? cur Node = ref lst.head
    while cur.next != null:
        cur = ref cur.next
    cur.next = ref node

fn ft_lstsize(ref lst Lst) i32:
    ref? cur Node = ref lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = ref cur.next
    return count

main():
    lst Lst
    ft_lstadd_back(ref lst, ft_lstnew("a"))
    ft_lstadd_back(ref lst, ft_lstnew("b"))
    ft_lstadd_front(ref lst, ft_lstnew("z"))
    ref? cur Node = ref lst.head
    while cur != null:
        output(cur.content, " ")
        cur = ref cur.next
    output("\n")
    output("size=", ft_lstsize(ref lst), "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ content : char[]
└─ next : STRUCT_CALL

struct Lst
└─ head : STRUCT_CALL

fn ft_lstnew(content : array) : STRUCT_CALL
├─ = : array
│  ├─ n : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ = : array
│  ├─ .content : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ n : STRUCT_CALL[]
│  │     └─ int 0
│  └─ content : char[]
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ n : STRUCT_CALL[]
         └─ int 0

fn ft_lstadd_front(lst : STRUCT_CALL, node : STRUCT_CALL) : void
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ node : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
└─ = : STRUCT_CALL
   ├─ .head : STRUCT_CALL
   │  └─ lst : STRUCT_CALL
   └─ ref : STRUCT_CALL
      └─ node : STRUCT_CALL

fn ft_lstadd_back(lst : STRUCT_CALL, node : STRUCT_CALL) : void
├─ if
│  ├─ condition == : bool
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ node : STRUCT_CALL
│  └─ return
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ .next : STRUCT_CALL
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ = : STRUCT_CALL
   ├─ .next : STRUCT_CALL
   │  └─ cur : STRUCT_CALL
   └─ ref : STRUCT_CALL
      └─ node : STRUCT_CALL

fn ft_lstsize(lst : STRUCT_CALL) : i32
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ = : i32
│  ├─ count : i32
│  └─ int 0
├─ while
│  ├─ condition != : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ += : i32
│  │  ├─ count : i32
│  │  └─ int 1
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ return
   └─ count : i32

fn main() : i32
├─ lst : STRUCT_CALL
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "a"
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "b"
├─ call ft_lstadd_front : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "z"
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ output : void
│  │  ├─ .content : char[]
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ char[] " "
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
├─ output : void
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "size="
   ├─ call ft_lstsize : i32
   │  └─ ref : STRUCT_CALL
   │     └─ lst : STRUCT_CALL
   └─ char[] "\n"
```

```out
z a b 
size=3
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { { i8*, i64 }, %Node* }
%Lst = type { %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"a\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"z\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"size=\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @ft_lstnew({ i8*, i64 } %0) {
entry:
  %content = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %content, align 8
  %n = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 24)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %n, align 8
  %n1 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data = extractvalue { %Node*, i64 } %n1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %content2 = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %content3 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  store { i8*, i64 } %content3, { i8*, i64 }* %content2, align 8
  %n4 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data5 = extractvalue { %Node*, i64 } %n4, 0
  %arr.at6 = getelementptr %Node, %Node* %arr.data5, i32 0
  ret %Node* %arr.at6
}

declare i8* @calloc(i64, i64)

define void @ft_lstadd_front(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %node = alloca %Node*, align 8
  store %Node* %1, %Node** %node, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  %ref2 = load %Node*, %Node** %node, align 8
  %next = getelementptr %Node, %Node* %ref2, i32 0, i32 1
  store %Node* %ref1, %Node** %next, align 8
  %ref3 = load %Node*, %Node** %node, align 8
  %ref4 = load %Lst*, %Lst** %lst, align 8
  %head5 = getelementptr %Lst, %Lst* %ref4, i32 0, i32 0
  store %Node* %ref3, %Node** %head5, align 8
  ret void
}

define void @ft_lstadd_back(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %node = alloca %Node*, align 8
  store %Node* %1, %Node** %node, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  %refcmp = icmp eq %Node* %ref1, null
  br i1 %refcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %cur = alloca %Node*, align 8
  %ref5 = load %Lst*, %Lst** %lst, align 8
  %head6 = getelementptr %Lst, %Lst* %ref5, i32 0, i32 0
  %ref7 = load %Node*, %Node** %head6, align 8
  store %Node* %ref7, %Node** %cur, align 8
  br label %while.cond

then:                                             ; preds = %entry
  %ref2 = load %Node*, %Node** %node, align 8
  %ref3 = load %Lst*, %Lst** %lst, align 8
  %head4 = getelementptr %Lst, %Lst* %ref3, i32 0, i32 0
  store %Node* %ref2, %Node** %head4, align 8
  ret void

while.cond:                                       ; preds = %while.body, %endif
  %ref8 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref8, i32 0, i32 1
  %ref9 = load %Node*, %Node** %next, align 8
  %refcmp10 = icmp ne %Node* %ref9, null
  br i1 %refcmp10, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %Node*, %Node** %cur, align 8
  %next12 = getelementptr %Node, %Node* %ref11, i32 0, i32 1
  %ref13 = load %Node*, %Node** %next12, align 8
  store %Node* %ref13, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %ref14 = load %Node*, %Node** %node, align 8
  %ref15 = load %Node*, %Node** %cur, align 8
  %next16 = getelementptr %Node, %Node* %ref15, i32 0, i32 1
  store %Node* %ref14, %Node** %next16, align 8
  ret void
}

define i32 @ft_lstsize(%Lst* %0) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %cur = alloca %Node*, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  store %Node* %ref1, %Node** %cur, align 8
  %count = alloca i32, align 4
  store i32 0, i32* %count, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %ref2 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref2, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cur3 = load i32, i32* %count, align 4
  %add = add i32 %cur3, 1
  store i32 %add, i32* %count, align 4
  %ref4 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref4, i32 0, i32 1
  %ref5 = load %Node*, %Node** %next, align 8
  store %Node* %ref5, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %count6 = load i32, i32* %count, align 4
  ret i32 %count6
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %lst = alloca %Lst, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %call = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call)
  %call1 = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call1)
  %call2 = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_front(%Lst* %lst, %Node* %call2)
  %cur = alloca %Node*, align 8
  %head = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  %ref = load %Node*, %Node** %head, align 8
  store %Node* %ref, %Node** %cur, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %ref3 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref3, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref4 = load %Node*, %Node** %cur, align 8
  %content = getelementptr %Node, %Node* %ref4, i32 0, i32 0
  %content5 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  %str.len = extractvalue { i8*, i64 } %content5, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %content5, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %ref6 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref6, i32 0, i32 1
  %ref7 = load %Node*, %Node** %next, align 8
  store %Node* %ref7, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %call8 = call i32 @ft_lstsize(%Lst* %lst)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.8, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i32 %call8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## lstlast — returns a reference to the last node

```ura
struct Node:
    content char[]
    ref? next Node

struct Lst:
    ref? head Node

fn ft_lstnew(content char[]) ref Node:
    n Node[] = new Node[1]
    n[0].content = content
    return ref n[0]

fn ft_lstadd_back(ref lst Lst, ref node Node) void:
    if lst.head == null:
        lst.head = ref node
        return
    ref? cur Node = ref lst.head
    while cur.next != null:
        cur = ref cur.next
    cur.next = ref node

fn ft_lstlast(ref lst Lst) ref? Node:
    ref? cur Node = ref lst.head
    if cur == null:
        return null
    while cur.next != null:
        cur = ref cur.next
    return ref cur

main():
    lst Lst
    ft_lstadd_back(ref lst, ft_lstnew("a"))
    ft_lstadd_back(ref lst, ft_lstnew("b"))
    ft_lstadd_back(ref lst, ft_lstnew("c"))
    ref? L Node = ft_lstlast(ref lst)
    output(L.content, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ content : char[]
└─ next : STRUCT_CALL

struct Lst
└─ head : STRUCT_CALL

fn ft_lstnew(content : array) : STRUCT_CALL
├─ = : array
│  ├─ n : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ = : array
│  ├─ .content : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ n : STRUCT_CALL[]
│  │     └─ int 0
│  └─ content : char[]
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ n : STRUCT_CALL[]
         └─ int 0

fn ft_lstadd_back(lst : STRUCT_CALL, node : STRUCT_CALL) : void
├─ if
│  ├─ condition == : bool
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ node : STRUCT_CALL
│  └─ return
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ .next : STRUCT_CALL
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ = : STRUCT_CALL
   ├─ .next : STRUCT_CALL
   │  └─ cur : STRUCT_CALL
   └─ ref : STRUCT_CALL
      └─ node : STRUCT_CALL

fn ft_lstlast(lst : STRUCT_CALL) : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ if
│  ├─ condition == : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ return
│     └─ NULL_LIT : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ .next : STRUCT_CALL
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ return
   └─ ref : STRUCT_CALL
      └─ cur : STRUCT_CALL

fn main() : i32
├─ lst : STRUCT_CALL
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "a"
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "b"
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "c"
├─ = : STRUCT_CALL
│  ├─ L : STRUCT_CALL
│  └─ call ft_lstlast : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ lst : STRUCT_CALL
└─ output : void
   ├─ .content : char[]
   │  └─ L : STRUCT_CALL
   └─ char[] "\n"
```

```out
c
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { { i8*, i64 }, %Node* }
%Lst = type { %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"a\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"c\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @ft_lstnew({ i8*, i64 } %0) {
entry:
  %content = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %content, align 8
  %n = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 24)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %n, align 8
  %n1 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data = extractvalue { %Node*, i64 } %n1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %content2 = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %content3 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  store { i8*, i64 } %content3, { i8*, i64 }* %content2, align 8
  %n4 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data5 = extractvalue { %Node*, i64 } %n4, 0
  %arr.at6 = getelementptr %Node, %Node* %arr.data5, i32 0
  ret %Node* %arr.at6
}

declare i8* @calloc(i64, i64)

define void @ft_lstadd_back(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %node = alloca %Node*, align 8
  store %Node* %1, %Node** %node, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  %refcmp = icmp eq %Node* %ref1, null
  br i1 %refcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %cur = alloca %Node*, align 8
  %ref5 = load %Lst*, %Lst** %lst, align 8
  %head6 = getelementptr %Lst, %Lst* %ref5, i32 0, i32 0
  %ref7 = load %Node*, %Node** %head6, align 8
  store %Node* %ref7, %Node** %cur, align 8
  br label %while.cond

then:                                             ; preds = %entry
  %ref2 = load %Node*, %Node** %node, align 8
  %ref3 = load %Lst*, %Lst** %lst, align 8
  %head4 = getelementptr %Lst, %Lst* %ref3, i32 0, i32 0
  store %Node* %ref2, %Node** %head4, align 8
  ret void

while.cond:                                       ; preds = %while.body, %endif
  %ref8 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref8, i32 0, i32 1
  %ref9 = load %Node*, %Node** %next, align 8
  %refcmp10 = icmp ne %Node* %ref9, null
  br i1 %refcmp10, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %Node*, %Node** %cur, align 8
  %next12 = getelementptr %Node, %Node* %ref11, i32 0, i32 1
  %ref13 = load %Node*, %Node** %next12, align 8
  store %Node* %ref13, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %ref14 = load %Node*, %Node** %node, align 8
  %ref15 = load %Node*, %Node** %cur, align 8
  %next16 = getelementptr %Node, %Node* %ref15, i32 0, i32 1
  store %Node* %ref14, %Node** %next16, align 8
  ret void
}

define %Node* @ft_lstlast(%Lst* %0) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %cur = alloca %Node*, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  store %Node* %ref1, %Node** %cur, align 8
  %ref2 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp eq %Node* %ref2, null
  br i1 %refcmp, label %then, label %endif

endif:                                            ; preds = %entry
  br label %while.cond

then:                                             ; preds = %entry
  ret %Node* null

while.cond:                                       ; preds = %while.body, %endif
  %ref3 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref3, i32 0, i32 1
  %ref4 = load %Node*, %Node** %next, align 8
  %refcmp5 = icmp ne %Node* %ref4, null
  br i1 %refcmp5, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref6 = load %Node*, %Node** %cur, align 8
  %next7 = getelementptr %Node, %Node* %ref6, i32 0, i32 1
  %ref8 = load %Node*, %Node** %next7, align 8
  store %Node* %ref8, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %ref9 = load %Node*, %Node** %cur, align 8
  ret %Node* %ref9
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %lst = alloca %Lst, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %call = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call)
  %call1 = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call1)
  %call2 = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call2)
  %L = alloca %Node*, align 8
  %call3 = call %Node* @ft_lstlast(%Lst* %lst)
  store %Node* %call3, %Node** %L, align 8
  %ref = load %Node*, %Node** %L, align 8
  %content = getelementptr %Node, %Node* %ref, i32 0, i32 0
  %content4 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  %str.len = extractvalue { i8*, i64 } %content4, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %content4, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## lstiter — apply a function to each node's content in place

```ura
struct Node:
    content char[]
    ref? next Node

struct Lst:
    ref? head Node

fn ft_lstnew(content char[]) ref Node:
    n Node[] = new Node[1]
    n[0].content = content
    return ref n[0]

fn ft_lstadd_back(ref lst Lst, ref node Node) void:
    if lst.head == null:
        lst.head = ref node
        return
    ref? cur Node = ref lst.head
    while cur.next != null:
        cur = ref cur.next
    cur.next = ref node

fn upcase(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        c char = s[i]
        if c >= 'a' and c <= 'z':
            out[i] = (c as i32 - 32) as char
        else:
            out[i] = c
        i += 1
    return out

fn ft_lstiter(ref lst Lst, f fn(char[]) char[]) void:
    ref? cur Node = ref lst.head
    while cur != null:
        cur.content = f(cur.content)
        cur = ref cur.next

main():
    lst Lst
    ft_lstadd_back(ref lst, ft_lstnew("ab"))
    ft_lstadd_back(ref lst, ft_lstnew("cd"))
    ft_lstiter(ref lst, upcase)
    ref? cur Node = ref lst.head
    while cur != null:
        output(cur.content, " ")
        cur = ref cur.next
    output("\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ content : char[]
└─ next : STRUCT_CALL

struct Lst
└─ head : STRUCT_CALL

fn ft_lstnew(content : array) : STRUCT_CALL
├─ = : array
│  ├─ n : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ = : array
│  ├─ .content : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ n : STRUCT_CALL[]
│  │     └─ int 0
│  └─ content : char[]
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ n : STRUCT_CALL[]
         └─ int 0

fn ft_lstadd_back(lst : STRUCT_CALL, node : STRUCT_CALL) : void
├─ if
│  ├─ condition == : bool
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ node : STRUCT_CALL
│  └─ return
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ .next : STRUCT_CALL
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ = : STRUCT_CALL
   ├─ .next : STRUCT_CALL
   │  └─ cur : STRUCT_CALL
   └─ ref : STRUCT_CALL
      └─ node : STRUCT_CALL

fn upcase(s : array) : char[]
├─ = : i32
│  ├─ n : i32
│  └─ cast : i32
│     └─ .len : u64
│        └─ s : char[]
├─ = : array
│  ├─ out : char[]
│  └─ array : char[]
│     └─ n : i32
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : i32
│  │  └─ n : i32
│  ├─ = : char
│  │  ├─ c : char
│  │  └─ index : char
│  │     ├─ s : char[]
│  │     └─ i : i32
│  ├─ if
│  │  ├─ condition and : bool
│  │  │  ├─ >= : bool
│  │  │  │  ├─ c : char
│  │  │  │  └─ char 'a'
│  │  │  └─ <= : bool
│  │  │     ├─ c : char
│  │  │     └─ char 'z'
│  │  ├─ = : char
│  │  │  ├─ index : char
│  │  │  │  ├─ out : char[]
│  │  │  │  └─ i : i32
│  │  │  └─ cast : char
│  │  │     └─ - : i32
│  │  │        ├─ cast : i32
│  │  │        │  └─ c : char
│  │  │        └─ int 32
│  │  └─ else
│  │     └─ = : char
│  │        ├─ index : char
│  │        │  ├─ out : char[]
│  │        │  └─ i : i32
│  │        └─ c : char
│  └─ += : i32
│     ├─ i : i32
│     └─ int 1
└─ return
   └─ out : char[]

fn ft_lstiter(lst : STRUCT_CALL, f : FN_TYPE) : void
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
└─ while
   ├─ condition != : bool
   │  ├─ cur : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   ├─ = : array
   │  ├─ .content : char[]
   │  │  └─ cur : STRUCT_CALL
   │  └─ call f : char[]
   │     └─ .content : char[]
   │        └─ cur : STRUCT_CALL
   └─ = : STRUCT_CALL
      ├─ cur : STRUCT_CALL
      └─ ref : STRUCT_CALL
         └─ .next : STRUCT_CALL
            └─ cur : STRUCT_CALL

fn main() : i32
├─ lst : STRUCT_CALL
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "ab"
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "cd"
├─ call ft_lstiter : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ FN_TYPE : FN_TYPE
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ output : void
│  │  ├─ .content : char[]
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ char[] " "
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ output : void
   └─ char[] "\n"
```

```out
AB CD 
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { { i8*, i64 }, %Node* }
%Lst = type { %Node* }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [204 x i8] c"runtime error: Call to a null function value\0A   lstiter.ura:38:23\0A   |\0A38 |         cur.content = f(cur.content)\0A   |                       ^\0A\00", align 1
@str = private unnamed_addr constant [3 x i8] c"ab\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"cd\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @ft_lstnew({ i8*, i64 } %0) {
entry:
  %content = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %content, align 8
  %n = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 24)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %n, align 8
  %n1 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data = extractvalue { %Node*, i64 } %n1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %content2 = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %content3 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  store { i8*, i64 } %content3, { i8*, i64 }* %content2, align 8
  %n4 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data5 = extractvalue { %Node*, i64 } %n4, 0
  %arr.at6 = getelementptr %Node, %Node* %arr.data5, i32 0
  ret %Node* %arr.at6
}

declare i8* @calloc(i64, i64)

define void @ft_lstadd_back(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %node = alloca %Node*, align 8
  store %Node* %1, %Node** %node, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  %refcmp = icmp eq %Node* %ref1, null
  br i1 %refcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %cur = alloca %Node*, align 8
  %ref5 = load %Lst*, %Lst** %lst, align 8
  %head6 = getelementptr %Lst, %Lst* %ref5, i32 0, i32 0
  %ref7 = load %Node*, %Node** %head6, align 8
  store %Node* %ref7, %Node** %cur, align 8
  br label %while.cond

then:                                             ; preds = %entry
  %ref2 = load %Node*, %Node** %node, align 8
  %ref3 = load %Lst*, %Lst** %lst, align 8
  %head4 = getelementptr %Lst, %Lst* %ref3, i32 0, i32 0
  store %Node* %ref2, %Node** %head4, align 8
  ret void

while.cond:                                       ; preds = %while.body, %endif
  %ref8 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref8, i32 0, i32 1
  %ref9 = load %Node*, %Node** %next, align 8
  %refcmp10 = icmp ne %Node* %ref9, null
  br i1 %refcmp10, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %Node*, %Node** %cur, align 8
  %next12 = getelementptr %Node, %Node* %ref11, i32 0, i32 1
  %ref13 = load %Node*, %Node** %next12, align 8
  store %Node* %ref13, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %ref14 = load %Node*, %Node** %node, align 8
  %ref15 = load %Node*, %Node** %cur, align 8
  %next16 = getelementptr %Node, %Node* %ref15, i32 0, i32 1
  store %Node* %ref14, %Node** %next16, align 8
  ret void
}

define { i8*, i64 } @upcase({ i8*, i64 } %0) {
entry:
  %s = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %s, align 8
  %n = alloca i32, align 4
  %s1 = load { i8*, i64 }, { i8*, i64 }* %s, align 8
  %len = extractvalue { i8*, i64 } %s1, 1
  %cast = trunc i64 %len to i32
  store i32 %cast, i32* %n, align 4
  %out = alloca { i8*, i64 }, align 8
  %n2 = load i32, i32* %n, align 4
  %n3 = sext i32 %n2 to i64
  %heap = call i8* @calloc(i64 %n3, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %n3, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %out, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i4 = load i32, i32* %i, align 4
  %n5 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i4, %n5
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %s6 = load { i8*, i64 }, { i8*, i64 }* %s, align 8
  %arr.data = extractvalue { i8*, i64 } %s6, 0
  %i7 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i7
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c8 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c8, 97
  %c9 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c9, 122
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %out22 = load { i8*, i64 }, { i8*, i64 }* %out, align 8
  ret { i8*, i64 } %out22

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %out10 = load { i8*, i64 }, { i8*, i64 }* %out, align 8
  %arr.data11 = extractvalue { i8*, i64 } %out10, 0
  %i12 = load i32, i32* %i, align 4
  %arr.at13 = getelementptr i8, i8* %arr.data11, i32 %i12
  %c14 = load i8, i8* %c, align 1
  %cast15 = sext i8 %c14 to i32
  %sub = sub i32 %cast15, 32
  %cast16 = trunc i32 %sub to i8
  store i8 %cast16, i8* %arr.at13, align 1
  br label %endif

next:                                             ; preds = %while.body
  %out17 = load { i8*, i64 }, { i8*, i64 }* %out, align 8
  %arr.data18 = extractvalue { i8*, i64 } %out17, 0
  %i19 = load i32, i32* %i, align 4
  %arr.at20 = getelementptr i8, i8* %arr.data18, i32 %i19
  %c21 = load i8, i8* %c, align 1
  store i8 %c21, i8* %arr.at20, align 1
  br label %endif
}

define void @ft_lstiter(%Lst* %0, { i8*, i64 } ({ i8*, i64 })* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %f = alloca { i8*, i64 } ({ i8*, i64 })*, align 8
  store { i8*, i64 } ({ i8*, i64 })* %1, { i8*, i64 } ({ i8*, i64 })** %f, align 8
  %cur = alloca %Node*, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  store %Node* %ref1, %Node** %cur, align 8
  br label %while.cond

while.cond:                                       ; preds = %cont, %entry
  %ref2 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref2, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %Node*, %Node** %cur, align 8
  %content = getelementptr %Node, %Node* %ref3, i32 0, i32 0
  %ref4 = load %Node*, %Node** %cur, align 8
  %content5 = getelementptr %Node, %Node* %ref4, i32 0, i32 0
  %content6 = load { i8*, i64 }, { i8*, i64 }* %content5, align 8
  %fn = load { i8*, i64 } ({ i8*, i64 })*, { i8*, i64 } ({ i8*, i64 })** %f, align 8
  %isnull = icmp eq { i8*, i64 } ({ i8*, i64 })* %fn, null
  br i1 %isnull, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  ret void

trap:                                             ; preds = %while.body
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([204 x i8], [204 x i8]* @trap_msg, i32 0, i32 0), i64 203)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %call = call { i8*, i64 } %fn({ i8*, i64 } %content6)
  store { i8*, i64 } %call, { i8*, i64 }* %content, align 8
  %ref7 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref7, i32 0, i32 1
  %ref8 = load %Node*, %Node** %next, align 8
  store %Node* %ref8, %Node** %cur, align 8
  br label %while.cond
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

define i32 @main(i32 %0, i8** %1) {
entry:
  %lst = alloca %Lst, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %call = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 2 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call)
  %call1 = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i64 2 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call1)
  call void @ft_lstiter(%Lst* %lst, { i8*, i64 } ({ i8*, i64 })* @upcase)
  %cur = alloca %Node*, align 8
  %head = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  %ref = load %Node*, %Node** %head, align 8
  store %Node* %ref, %Node** %cur, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %ref2 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref2, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %Node*, %Node** %cur, align 8
  %content = getelementptr %Node, %Node* %ref3, i32 0, i32 0
  %content4 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  %str.len = extractvalue { i8*, i64 } %content4, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %content4, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %ref5 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref5, i32 0, i32 1
  %ref6 = load %Node*, %Node** %next, align 8
  store %Node* %ref6, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## lstdelone — unlink a node by index and clear the list

```ura
struct Node:
    content char[]
    ref? next Node

struct Lst:
    ref? head Node

fn ft_lstnew(content char[]) ref Node:
    n Node[] = new Node[1]
    n[0].content = content
    return ref n[0]

fn ft_lstadd_back(ref lst Lst, ref node Node) void:
    if lst.head == null:
        lst.head = ref node
        return
    ref? cur Node = ref lst.head
    while cur.next != null:
        cur = ref cur.next
    cur.next = ref node

fn ft_lstsize(ref lst Lst) i32:
    ref? cur Node = ref lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = ref cur.next
    return count

fn ft_lstdelone(ref lst Lst, idx i32) void:
    if lst.head == null:
        return
    if idx <= 0:
        lst.head = ref lst.head.next
        return
    ref? prev Node = ref lst.head
    i i32 = 1
    while i < idx and prev.next != null:
        prev = ref prev.next
        i += 1
    if prev.next != null:
        prev.next = ref prev.next.next

fn ft_lstclear(ref lst Lst) void:
    lst.head = null

fn show(ref lst Lst) void:
    ref? cur Node = ref lst.head
    while cur != null:
        output(cur.content, " ")
        cur = ref cur.next
    output("\n")

main():
    lst Lst
    ft_lstadd_back(ref lst, ft_lstnew("a"))
    ft_lstadd_back(ref lst, ft_lstnew("b"))
    ft_lstadd_back(ref lst, ft_lstnew("c"))
    ft_lstdelone(ref lst, 1)
    show(ref lst)
    ft_lstdelone(ref lst, 0)
    show(ref lst)
    ft_lstclear(ref lst)
    output("size=", ft_lstsize(ref lst), "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ content : char[]
└─ next : STRUCT_CALL

struct Lst
└─ head : STRUCT_CALL

fn ft_lstnew(content : array) : STRUCT_CALL
├─ = : array
│  ├─ n : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ = : array
│  ├─ .content : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ n : STRUCT_CALL[]
│  │     └─ int 0
│  └─ content : char[]
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ n : STRUCT_CALL[]
         └─ int 0

fn ft_lstadd_back(lst : STRUCT_CALL, node : STRUCT_CALL) : void
├─ if
│  ├─ condition == : bool
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ node : STRUCT_CALL
│  └─ return
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ .next : STRUCT_CALL
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ = : STRUCT_CALL
   ├─ .next : STRUCT_CALL
   │  └─ cur : STRUCT_CALL
   └─ ref : STRUCT_CALL
      └─ node : STRUCT_CALL

fn ft_lstsize(lst : STRUCT_CALL) : i32
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ = : i32
│  ├─ count : i32
│  └─ int 0
├─ while
│  ├─ condition != : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ += : i32
│  │  ├─ count : i32
│  │  └─ int 1
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ return
   └─ count : i32

fn ft_lstdelone(lst : STRUCT_CALL, idx : i32) : void
├─ if
│  ├─ condition == : bool
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ return
├─ if
│  ├─ condition <= : bool
│  │  ├─ idx : i32
│  │  └─ int 0
│  ├─ = : STRUCT_CALL
│  │  ├─ .head : STRUCT_CALL
│  │  │  └─ lst : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ .next : STRUCT_CALL
│  │        └─ .head : STRUCT_CALL
│  │           └─ lst : STRUCT_CALL
│  └─ return
├─ = : STRUCT_CALL
│  ├─ prev : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ = : i32
│  ├─ i : i32
│  └─ int 1
├─ while
│  ├─ condition and : bool
│  │  ├─ < : bool
│  │  │  ├─ i : i32
│  │  │  └─ idx : i32
│  │  └─ != : bool
│  │     ├─ .next : STRUCT_CALL
│  │     │  └─ prev : STRUCT_CALL
│  │     └─ NULL_LIT : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ prev : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ .next : STRUCT_CALL
│  │        └─ prev : STRUCT_CALL
│  └─ += : i32
│     ├─ i : i32
│     └─ int 1
└─ if
   ├─ condition != : bool
   │  ├─ .next : STRUCT_CALL
   │  │  └─ prev : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   └─ = : STRUCT_CALL
      ├─ .next : STRUCT_CALL
      │  └─ prev : STRUCT_CALL
      └─ ref : STRUCT_CALL
         └─ .next : STRUCT_CALL
            └─ .next : STRUCT_CALL
               └─ prev : STRUCT_CALL

fn ft_lstclear(lst : STRUCT_CALL) : void
└─ = : STRUCT_CALL
   ├─ .head : STRUCT_CALL
   │  └─ lst : STRUCT_CALL
   └─ NULL_LIT : STRUCT_CALL

fn show(lst : STRUCT_CALL) : void
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ output : void
│  │  ├─ .content : char[]
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ char[] " "
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ output : void
   └─ char[] "\n"

fn main() : i32
├─ lst : STRUCT_CALL
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "a"
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "b"
├─ call ft_lstadd_back : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call ft_lstnew : STRUCT_CALL
│     └─ char[] "c"
├─ call ft_lstdelone : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ int 1
├─ call show : void
│  └─ ref : STRUCT_CALL
│     └─ lst : STRUCT_CALL
├─ call ft_lstdelone : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ int 0
├─ call show : void
│  └─ ref : STRUCT_CALL
│     └─ lst : STRUCT_CALL
├─ call ft_lstclear : void
│  └─ ref : STRUCT_CALL
│     └─ lst : STRUCT_CALL
└─ output : void
   ├─ char[] "size="
   ├─ call ft_lstsize : i32
   │  └─ ref : STRUCT_CALL
   │     └─ lst : STRUCT_CALL
   └─ char[] "\n"
```

```out
a c 
c 
size=0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { { i8*, i64 }, %Node* }
%Lst = type { %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"c\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"size=\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @ft_lstnew({ i8*, i64 } %0) {
entry:
  %content = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %content, align 8
  %n = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 24)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %n, align 8
  %n1 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data = extractvalue { %Node*, i64 } %n1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %content2 = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %content3 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  store { i8*, i64 } %content3, { i8*, i64 }* %content2, align 8
  %n4 = load { %Node*, i64 }, { %Node*, i64 }* %n, align 8
  %arr.data5 = extractvalue { %Node*, i64 } %n4, 0
  %arr.at6 = getelementptr %Node, %Node* %arr.data5, i32 0
  ret %Node* %arr.at6
}

declare i8* @calloc(i64, i64)

define void @ft_lstadd_back(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %node = alloca %Node*, align 8
  store %Node* %1, %Node** %node, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  %refcmp = icmp eq %Node* %ref1, null
  br i1 %refcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %cur = alloca %Node*, align 8
  %ref5 = load %Lst*, %Lst** %lst, align 8
  %head6 = getelementptr %Lst, %Lst* %ref5, i32 0, i32 0
  %ref7 = load %Node*, %Node** %head6, align 8
  store %Node* %ref7, %Node** %cur, align 8
  br label %while.cond

then:                                             ; preds = %entry
  %ref2 = load %Node*, %Node** %node, align 8
  %ref3 = load %Lst*, %Lst** %lst, align 8
  %head4 = getelementptr %Lst, %Lst* %ref3, i32 0, i32 0
  store %Node* %ref2, %Node** %head4, align 8
  ret void

while.cond:                                       ; preds = %while.body, %endif
  %ref8 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref8, i32 0, i32 1
  %ref9 = load %Node*, %Node** %next, align 8
  %refcmp10 = icmp ne %Node* %ref9, null
  br i1 %refcmp10, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %Node*, %Node** %cur, align 8
  %next12 = getelementptr %Node, %Node* %ref11, i32 0, i32 1
  %ref13 = load %Node*, %Node** %next12, align 8
  store %Node* %ref13, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %ref14 = load %Node*, %Node** %node, align 8
  %ref15 = load %Node*, %Node** %cur, align 8
  %next16 = getelementptr %Node, %Node* %ref15, i32 0, i32 1
  store %Node* %ref14, %Node** %next16, align 8
  ret void
}

define i32 @ft_lstsize(%Lst* %0) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %cur = alloca %Node*, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  store %Node* %ref1, %Node** %cur, align 8
  %count = alloca i32, align 4
  store i32 0, i32* %count, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %ref2 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref2, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cur3 = load i32, i32* %count, align 4
  %add = add i32 %cur3, 1
  store i32 %add, i32* %count, align 4
  %ref4 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref4, i32 0, i32 1
  %ref5 = load %Node*, %Node** %next, align 8
  store %Node* %ref5, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %count6 = load i32, i32* %count, align 4
  ret i32 %count6
}

define void @ft_lstdelone(%Lst* %0, i32 %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %idx = alloca i32, align 4
  store i32 %1, i32* %idx, align 4
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  %refcmp = icmp eq %Node* %ref1, null
  br i1 %refcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %idx4 = load i32, i32* %idx, align 4
  %le = icmp sle i32 %idx4, 0
  br i1 %le, label %then3, label %endif2

then:                                             ; preds = %entry
  ret void

endif2:                                           ; preds = %endif
  %prev = alloca %Node*, align 8
  %ref11 = load %Lst*, %Lst** %lst, align 8
  %head12 = getelementptr %Lst, %Lst* %ref11, i32 0, i32 0
  %ref13 = load %Node*, %Node** %head12, align 8
  store %Node* %ref13, %Node** %prev, align 8
  %i = alloca i32, align 4
  store i32 1, i32* %i, align 4
  br label %while.cond

then3:                                            ; preds = %endif
  %ref5 = load %Lst*, %Lst** %lst, align 8
  %head6 = getelementptr %Lst, %Lst* %ref5, i32 0, i32 0
  %ref7 = load %Node*, %Node** %head6, align 8
  %next = getelementptr %Node, %Node* %ref7, i32 0, i32 1
  %ref8 = load %Node*, %Node** %next, align 8
  %ref9 = load %Lst*, %Lst** %lst, align 8
  %head10 = getelementptr %Lst, %Lst* %ref9, i32 0, i32 0
  store %Node* %ref8, %Node** %head10, align 8
  ret void

while.cond:                                       ; preds = %while.body, %endif2
  %i14 = load i32, i32* %i, align 4
  %idx15 = load i32, i32* %idx, align 4
  %lt = icmp slt i32 %i14, %idx15
  %ref16 = load %Node*, %Node** %prev, align 8
  %next17 = getelementptr %Node, %Node* %ref16, i32 0, i32 1
  %ref18 = load %Node*, %Node** %next17, align 8
  %refcmp19 = icmp ne %Node* %ref18, null
  %and = and i1 %lt, %refcmp19
  br i1 %and, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref20 = load %Node*, %Node** %prev, align 8
  %next21 = getelementptr %Node, %Node* %ref20, i32 0, i32 1
  %ref22 = load %Node*, %Node** %next21, align 8
  store %Node* %ref22, %Node** %prev, align 8
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %ref25 = load %Node*, %Node** %prev, align 8
  %next26 = getelementptr %Node, %Node* %ref25, i32 0, i32 1
  %ref27 = load %Node*, %Node** %next26, align 8
  %refcmp28 = icmp ne %Node* %ref27, null
  br i1 %refcmp28, label %then24, label %endif23

endif23:                                          ; preds = %then24, %while.end
  ret void

then24:                                           ; preds = %while.end
  %ref29 = load %Node*, %Node** %prev, align 8
  %next30 = getelementptr %Node, %Node* %ref29, i32 0, i32 1
  %ref31 = load %Node*, %Node** %next30, align 8
  %next32 = getelementptr %Node, %Node* %ref31, i32 0, i32 1
  %ref33 = load %Node*, %Node** %next32, align 8
  %ref34 = load %Node*, %Node** %prev, align 8
  %next35 = getelementptr %Node, %Node* %ref34, i32 0, i32 1
  store %Node* %ref33, %Node** %next35, align 8
  br label %endif23
}

define void @ft_lstclear(%Lst* %0) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  store %Node* null, %Node** %head, align 8
  ret void
}

define void @show(%Lst* %0) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %cur = alloca %Node*, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  store %Node* %ref1, %Node** %cur, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %ref2 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref2, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %Node*, %Node** %cur, align 8
  %content = getelementptr %Node, %Node* %ref3, i32 0, i32 0
  %content4 = load { i8*, i64 }, { i8*, i64 }* %content, align 8
  %str.len = extractvalue { i8*, i64 } %content4, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %content4, 0
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref5 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref5, i32 0, i32 1
  %ref6 = load %Node*, %Node** %next, align 8
  store %Node* %ref6, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %lst = alloca %Lst, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %call = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call)
  %call1 = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call1)
  %call2 = call %Node* @ft_lstnew({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0), i64 1 })
  call void @ft_lstadd_back(%Lst* %lst, %Node* %call2)
  call void @ft_lstdelone(%Lst* %lst, i32 1)
  call void @show(%Lst* %lst)
  call void @ft_lstdelone(%Lst* %lst, i32 0)
  call void @show(%Lst* %lst)
  call void @ft_lstclear(%Lst* %lst)
  %call3 = call i32 @ft_lstsize(%Lst* %lst)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.8, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i32 %call3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}
```
