# Chapter 5 Homework

## Question 5.13
**A & B**. The lower bound of CPE is the floating-point addition latency i.e. 3 cycles.

![](plots/inner1.svg)

**C**. The critical path does not change, so the CPE lower bound is integer addition latency i.e. 1 cycle.

**D**. As the plot shown, multiplication is not on the critical path.

## Question 5.14
**A**. The CPE is bounded by throughput.

**B**. kx1 unrolling cannot breakthorugh the latency bound.

## Question 5.15
The thoughput bound limits the performance to a CPE of 1.

## Question 5.16
Please see `inner4` in [inner.c](inner.c).

## Question 5.17
Please see [memset.c](memset.c).