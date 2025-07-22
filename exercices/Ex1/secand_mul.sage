#SHARES 2
#IN a b
#RANDOMS r0
#OUT c

# SecAnd multiplication gadget for d=2 shares
# Input: a = (a0, a1), b = (b0, b1) 
# Output: c = (c0, c1) where c = a * b (Boolean multiplication/AND)

# First output share: c0 = a0*b0 + r0
c0 = a0 * b0
c0 = c0 + r0

# Second output share: c1 = a1*b1 + a0*b1 + a1*b0 + r0
c1 = a1 * b1
tmp1 = a0 * b1
c1 = c1 + tmp1
tmp2 = a1 * b0
c1 = c1 + tmp2
c1 = c1 + r0    #should finish wth the masking