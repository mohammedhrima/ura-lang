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
```c
func int sum(int count, ...):
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

main():
   int result = sum(3, 10, 20, 30)
   return result
```

+ pointers: 008.c
```c
main():
   int a = 1
   ref int b = a
   b = 2
   return a  // returns 2
```

+ casting: 009.c
```c
main():
   int a = 1
   char b = a as char
   b = 'e'
   return 0
```

+ stack allocation: 010.c
```c
main():
   chars a = stack(10) as chars
   return 0
```

+ try/catch: 011.c
```c
main():
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
```

+ array bounds checking: 012.c
```c
protoFunc int printf(chars, ...)

main():
   printf("Before allocation\n")
   chars u = stack(2) as chars
   printf("After allocation\n")
   u[-10] = 'a'  // Runtime error: segmentation fault (array bounds)
   printf("This should not print\n")
   return 0
```

Output:
```
Before allocation
After allocation

line 6: segmentation fault (array bounds)
error in accessing u[-10]
                   ^^^^
```

### Features

- **Type System**: `int`, `char`, `chars` (string), `bool`, `va_list`
- **Control Flow**: `if`/`else`, `while` loops
- **Functions**: Function declarations with parameters, variadic functions
- **Memory**: Stack allocation with `stack()`, references with `ref`
- **Type Casting**: Explicit type conversion with `as`
- **Exception Handling**: `try`/`catch`/`throw` for error handling
- **Array Access**: Bracket notation `[]` with runtime bounds checking
- **Safety**: Automatic bounds checking for array access with detailed error messages