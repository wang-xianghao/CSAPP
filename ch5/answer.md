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

## Question 5.8
As the data-flow graph shown, we get the following CPEs for each reassociation.
* `r = ((r * x) * y) * z`: $5$ cycles
* `r = (r * (x * y)) * z`: $\frac{10}{3}$ cycles
* `r = r * ((x * y) * z)`: $\frac{5}{3}$ cycles
* `r = r * (x * (y * z))`: $\frac{5}{3}$ cycles
* `r = (r * x) * (y * z)`: $\frac{10}{3}$ cycles
![](plots/aprod.svg)