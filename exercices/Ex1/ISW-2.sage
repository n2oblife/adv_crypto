#SHARES 2
#IN x y
#RANDOMS r0
#OUT z

# ISW-2: 2-share ISW multiplication gadget
# z0 = x0 * y0 + r0
# z1 = x1 * y1 + r0 + x0 * y1 + x1 * y0

# First share: z0 = x0 * y0 + r0
z0 = x0 * y0
z0 = z0 + r0

# Second share: z1 = x1 * y1 + r0 + x0 * y1 + x1 * y0
z1 = x1 * y1
z1 = z1 + r0
tmp1 = x0 * y1
z1 = z1 + tmp1
tmp2 = x1 * y0
z1 = z1 + tmp2