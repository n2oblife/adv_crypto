#SHARES 3
#IN x y
#RANDOMS r0 r1 r2
#OUT z

# ISW-3: 3-share ISW multiplication gadget
# z0 = x0 * y0 + r0 + r1
# z1 = x1 * y0 + (x0 * y1 + r0) + x1 * y1 + r2
# z2 = x2 * y0 + (x0 * y2 + r1) + (x2 * y1 + (x1 * y2 + r2)) + x2 * y2

# First share: z0 = x0 * y0 + r0 + r1
z0 = x0 * y0
z0 = z0 + r0
z0 = z0 + r1

# Second share: z1 = x1 * y0 + (x0 * y1 + r0) + x1 * y1 + r2
z1 = x1 * y0
tmp1 = x0 * y1
tmp1 = tmp1 + r0
z1 = z1 + tmp1
tmp2 = x1 * y1
z1 = z1 + tmp2
z1 = z1 + r2

# Third share: z2 = x2 * y0 + (x0 * y2 + r1) + (x2 * y1 + (x1 * y2 + r2)) + x2 * y2
z2 = x2 * y0
tmp3 = x0 * y2
tmp3 = tmp3 + r1
z2 = z2 + tmp3
tmp4 = x1 * y2
tmp4 = tmp4 + r2
tmp5 = x2 * y1
tmp5 = tmp5 + tmp4
z2 = z2 + tmp5
tmp6 = x2 * y2
z2 = z2 + tmp6