  .text
  .align 2

.globl __divdi3; .type __divdi3,@function; __divdi3:
  bltz a0, .L10
  bltz a1, .L11


.globl __udivdi3; .type __udivdi3,@function; __udivdi3:
  mv a2, a1
  mv a1, a0
  li a0, -1
  beqz a2, .L5
  li a3, 1
  bgeu a2, a1, .L2
.L1:
  blez a2, .L2
  slli a2, a2, 1
  slli a3, a3, 1
  bgtu a1, a2, .L1
.L2:
  li a0, 0
.L3:
  bltu a1, a2, .L4
  sub a1, a1, a2
  or a0, a0, a3
.L4:
  srli a3, a3, 1
  srli a2, a2, 1
  bnez a3, .L3
.L5:
  ret
.size __udivdi3,.-__udivdi3
.globl __hidden___udivdi3; __hidden___udivdi3 = __udivdi3; .hidden __hidden___udivdi3

.globl __umoddi3; .type __umoddi3,@function; __umoddi3:

  move t0, ra
  jal __hidden___udivdi3
  move a0, a1
  jr t0
.size __umoddi3,.-__umoddi3


.L10:
  neg a0, a0

  bgtz a1, .L12

  neg a1, a1
  j __hidden___udivdi3
.L11:
  neg a1, a1
.L12:
  move t0, ra
  jal __hidden___udivdi3
  neg a0, a0
  jr t0
.size __divdi3,.-__divdi3

.globl __moddi3; .type __moddi3,@function; __moddi3:
  move t0, ra
  bltz a1, .L31
  bltz a0, .L32
.L30:
  jal __hidden___udivdi3
  move a0, a1
  jr t0
.L31:
  neg a1, a1
  bgez a0, .L30
.L32:
  neg a0, a0
  jal __hidden___udivdi3
  neg a0, a1
  jr t0
.size __moddi3,.-__moddi3
