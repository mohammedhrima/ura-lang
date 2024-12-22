/* 
+ Example Program in wolf-c
+ Demonstrates function definitions, conditionals, and loops
*/

func int factorial(int n):
   if n <= 1: return 1
   else: return n * factorial(n - 1)

main():
   int num = 5
   output("Calculating factorial of ", num)
   int result = factorial(num)

   if result < 0:
      output("Error: Factorial overflow!")
   else:
      output("Factorial of ", num, " is ", result)

   // Loop demonstration
   output("Counting down:")
   int i = num
   while i > 0:
      output(i)
      i -= 1
   output("Done!")

