### Examples

+ simple main: 000
```bash
def int main():
    int a = 1
    return a
end
```

+ while loop with compound assignment: 001
```bash
def int main():
    int i = 0
    while i < 5:
        i += 1
    end
   return i
end
```

+ if/elif/else conditionals: 002
```bash
def int main():
   int i = 0
   if i < 1:
      i += 2
   end
   elif i < 3:
      i += 4
   end
   else:
      i += 3
   end
   return i
end
```

+ builtin and array access: 003
```bash
protoFunc int putchar(char a)

def int main():
   chars str = "abcd"
   char c = str[1]
   putchar(c)
   return 0
end
```

+ function with multiple parameters: 004
```bash
def int add_multiply(int a, int b, int c):
   int sum = a + b
   int result = sum * c
   return result
end

def int main():
   int x = add_multiply(5, 3, 2)
   return x
end
```

+ nested function with isdigit: 005
```bash
def bool isdigit(char c):
   return (c >= '0' && c <= '9')
end

def int main():
   def bool isdigit(char c):
      return (c >= '0' && c <= '9')
   end
   char c = 'a'
   isdigit(c)
   if isdigit(c):
      return 11
   end
   return 12
end
```

+ variadic function: 006
```bash
def int sum(int count, ...):
   va_list args
   va_start(args, count)
   int total = 0
   int i = 0
   while (i < count):
      total = total + va_arg(args, int)
      i = i + 1
   end
   va_end(args)
   return total
end

def int main():
   int result = sum(3, 10, 20, 30)
   return result
end
```

+ reference: 007
```bash
def int main():
   int a = 1
   ref int b = a
   b = 2
   return a
end
```

+ reference parameter: 008
```bash
def void func(ref int c):
   c = 2
end

def int main():
   int a = 12
   ref int c = a
   func(c)
   return a
end
```

+ casting: 008
```bash

def int main():
   int a = 'a'
   int b = a as int
   return b
end
```

+ stack allocation: 010
```bash
def int main():
   chars a = stack(10) as chars
   return 0
end
```

+ try/catch with function call: 011
```bash
def int func():
   throw 12
   return 0
end

def int main():
   try:
      func()
      int x = 10
      int y = 0
      if y == 0:
         throw 1
      int result = x / y
   end
   catch int error:
      return error
   end
   return 0
end
```

+ array bounds checking with function: 012
```bash
protoFunc int printf(chars, ...)
protoFunc void exit(int)

def void bounds_check(int index, int size, int line, int col, chars varname):
    if index < 0 || index >= size:
        printf("\nline %d: segmentation fault (array bounds)\n", line)
        printf("error in accessing %s[%d]\n", varname, index)
        printf("                   ^^^^\n")
        exit(1)
    end
    return
end

def int main():
   printf("Before allocation\n")
   chars u = stack(2) as chars
   printf("After allocation\n")
   u[-10] = 'a'
   printf("This should not print\n")
   return 0
end
```
