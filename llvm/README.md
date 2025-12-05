### Examples

+ simple main: 001.c
```c
main():
   chars str = "Hello World"
   return 0
```

+ while loop: 002.c
```c
main():
   int i = 0
   while i < 5:
      i = i + 1
   return i
```

+ builtin and access []: 003.c
```c
protoFunc int putchar(char c)
main():
   char* str = "abcd"
   char c = str[0]
   putchar(c)
   return 0
```

+ function with params: 004.c
```c
func int add_multiply(int a, int b, int c):
   int sum = a + b
   int result = sum * c
   return result

main():
   int x = add_multiply(5, 3, 2)
   return x
```

+ built-in function with variadic arguments: 005.c
```c
protoFunc int printf(chars fmt, ...)
main():
   int result = 10
   printf("func1 returned: %d\n", result)
   return 0
```

+ function with-in function: 006.c
```c
main():
   char c = 'a'
   func bool isdigit(): return (c >= '0' && c <= '9')
   if isdigit(): return 11
   else: return 12
```
the above code will turn to: (in compile time)
```c
func bool isdigit(char* ptr): return (*ptr >= '0' && *ptr <= '9')
main():
   char c = 'a'
   if isdigit(&c): return 11
   else: return 12
```

+ variadic function: 007.c
+ pointers: 008.c
```c
main():
   int a = 1
   ref int b = a
   b = 2
   return b
```