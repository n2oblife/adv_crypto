#SHARES 3
#IN x y
#RANDOMS r0 r1
#OUT z

# EC16-3: 3-share multiplication gadget from [8]
# z0 = x0 * y0 + r0 + x0 * y2 + x2 * y0
# z1 = x1 * y1 + r1 + x0 * y1 + x1 * y0  
# z2 = x2 * y2 + r0 + r1 + x1 * y2 + x2 * y1

# First share: z0 = x0 * y0 + r0 + x0 * y2 + x2 * y0
z0 = x0 * y0
z0 = z0 + r0
tmp1 = x0 * y2
z0 = z0 + tmp1
tmp2 = x2 * y0
z0 = z0 + tmp2

# Second share: z1 = x1 * y1 + r1 + x0 * y1 + x1 * y0
z1 = x1 * y1
z1 = z1 + r1
tmp3 = x0 * y1
z1 = z1 + tmp3
tmp4 = x1 * y0
z1 = z1 + tmp4

# Third share: z2 = x2 * y2 + r0 + r1 + x1 * y2 + x2 * y1
z2 = x2 * y2
z2 = z2 + r0
z2 = z2 + r1
tmp5 = x1 * y2
z2 = z2 + tmp5
tmp6 = x2 * y1
z2 = z2 + tmp6