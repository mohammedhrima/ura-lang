main():
    int b
    ref int a = b REF_HOLD_ID   (left don't have reference, right is ID)
    int c = 2     ID_VAL
    a = c         REF_ID        (left has reference, right is ID)
    a = 4         REF_VAL       (left has reference, right is register or value)
    ref int d = a REF_HOLD_REF  (left don't have reference, right is a reference)
    ref int e = c
            e = d REF_REF       (left has reference, right is reference)
    int f = c     ID_ID
    int g = c     ID_REF        
