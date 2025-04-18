# Chapter 5 Practice Problems

## Question 5.5
**A**. There are $2n$ additions and $2n$ multiplications performed.

**B**. The total cycles are $5n+3$, then CPE is $5$.

![](plots/poly.svg)

## Question 5.6
**A**. $2n$ additions and $n$ multiplications are performed;

**B**. The total cycles are $8n$, then CPE is $8$.

![](plots/polyh.svg)

**C**. The one in 5.6 has smaller CPE via critical path analysis.

## Question 5.7
Please see [poly.c](poly.c).

## Question 5.8
As the data-flow graph shown, we get the following CPEs for each reassociation.
* `r = ((r * x) * y) * z`: $5$ cycles
* `r = (r * (x * y)) * z`: $\frac{10}{3}$ cycles
* `r = r * ((x * y) * z)`: $\frac{5}{3}$ cycles
* `r = r * (x * (y * z))`: $\frac{5}{3}$ cycles
* `r = (r * x) * (y * z)`: $\frac{10}{3}$ cycles
![](plots/aprod.svg)

## Question 5.9
Please see [merge.c](merge.c)

## Question 5.10
**A**. The first 1000 elements are shifted right by one position.

**B**. The elements at $[1, 1000)$ are set to `a[0]`.

**C**. During copying `a+1` to `a`, the load address never mathces any address in store buffer set by `s_addr`. Thus, the cirtical path is just `add`. Copying `a` to `a+1` leads to the address match, which causes the dependency between `s_data` and `load`. Therefore, the cirtical path is `load` and `s_data`.
![](plots/copy_array.svg)

**D.** The expect CPE is around 1 cycle, same to copying `a+1` to `a`. Different iteration handles different address, so the address would never match.