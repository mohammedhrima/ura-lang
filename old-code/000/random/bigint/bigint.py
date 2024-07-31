import ctypes
class PyLongObject(ctypes.Structure):
    _fields_ = [("ob_refcnt", ctypes.c_long),
                ("ob_type", ctypes.c_void_p),
                ("ob_size", ctypes.c_ulong),
                ("ob_digit", ctypes.c_uint * 3)]

SHIFT = 30  # number of bits for each 'digit'
MASK = (2 ** SHIFT)

def split(bignum):§
    t = abs(bignum)
    num_list = []
    while t != 0:
        # Get remainder from division
        small_int = t % MASK  # more efficient bitwise analogue: (t & (MASK-1))
        num_list.append(small_int)
        # Get integral part of the division (floor division)
        t = t // MASK  # more efficient bitwise analogue: t >>= SHIFT

    return num_list

def merge(num_list):
    bignum = 0
    for i, n in enumerate(num_list):
        bignum += n * (2 ** (SHIFT * i))
    return bignum


a = 18446744073709551615
b = split(a)
c = merge(b)

print("a : ", a)
print("b : ", b)
print("c : ", c)

for d in PyLongObject.from_address(id(a)).ob_digit:
    print(d)

def add(a, b):
    z = []
    if len(a) < len(b):
        # Ensure a is the larger of the two
        a, b = b, a
    carry = 0
    for i in range(0, len(b)):
        carry += a[i] + b[i]
        z.append(carry % MASK)
        carry = carry // MASK
    for i in range(i + 1, len(a)):
        carry += a[i]
        z.append(carry % MASK)
        carry = carry // MASK
    z.append(carry)
    # remove trailing zeros
    i = len(z)
    while i > 0 and z[i-1] == 0:
        i -= 1
    z = z[0:i]
    return z


d = 8223372036854775807
e = 8223372036854775807
f = merge(add(split(d), split(e)))

print("d : ", d)
print("e : ", e)
print("f : ", f)