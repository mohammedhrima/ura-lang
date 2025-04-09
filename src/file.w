
func void swap(ref int a, ref int b):
   int t = a
   a = b
   b = t

main():
   int a = 1
   int b = 2
   output("before swap: a:", a, " b: ", b, "\n")
   swap(a, b)
   output("after swap: a:", a, " b: ", b , "\n")
   
