### Examples

+ simple main: 000.c
```bash
def int main():
   int a = 1
   return a
end
```

+ while loop: 001.c
```bash
def int main():
   int i = 0
   while i < 5:
      i = i + 1
   end
   return i
end
```

+ while loop: 002.c
```bash
def int main():
   int i = 0
   while i < 5:
      i += 1
   end
   return i
end
```

+ builtin and access []: 003.c
```bash
protoFunc int putchar(char a)

def int main():
   chars str = "abcd"
   char c = str[1]
   putchar(c)
   return 0
end
```

+ function with params: 004.c
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

+ built-in function with variadic arguments: 005.c
```bash
protoFunc int printf(chars fmt, ...)

def int main():
   int result = 10
   printf("func1 returned: %d\n", result)
   return 0
end
```

+ function with-in function: 006.c
```bash
def int main():
   def bool isdigit(char c):
      return (c >= '0' && c <= '9')
   end
   char c = 'a'
   if isdigit(c):
      return 11
   end
   return 12
end
```

+ variadic function: 007.c
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

+ reference: 008.c
```bash
def int main():
   int a = 1
   ref int b = a
   b = 2
   return a
end
```

+ casting: 009.c
```bash
def int main():
   int a = 1
   char b = a as char
   b = 'e'
   return 0
end
```

+ stack allocation: 010.c
```bash
def int main():
   chars a = stack(10) as chars
   return 0
end
```

+ try/catch: 011.c
```bash
def int main():
   try:
      int x = 10
      int y = 0
      if y == 0:
         throw 1
      end
      int result = x / y
   catch int error:
      return error
   end
   return 0
end
```

+ array bounds checking: 012.c
```bash
protoFunc int printf(chars, ...)
protoFunc void exit(int)

def int main():
   printf("Before allocation\n")
   chars u = stack(2) as chars
   printf("After allocation\n")
   u[-10] = 'a'
   printf("This should not print\n")
   return 0
end
```

Output:
```
Before allocation
After allocation

line 6: segmentation fault (array bounds)
error in accessing u[-10]
                   ^^^^
```
