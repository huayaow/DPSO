# DPSO

DPSO focuses on improving the performance of Particle Swarm Optimization (PSO) for covering array generation.

A covering array is a mathematical object that systematically covers the interactions among a fixed number of factors. It can be used for software testing purposes, i.e. combinatorial testing.

To enhance conventional PSO algorithm for covering array generation, DPSO utilizes the set and probability theories to represent particle's velocity, and refines related evolution operators. DPSO further applies two auxiliary strategies to improve the effectiveness of search process. The details of DPSO are in the following paper:

> H. Wu, C. Nie, F.C. Kuo, h. Leung and C.J. Colbourn, "A Discrete Particle Swarm Optimization for Covering Array Generation," IEEE Transactions on Evolutionary Computation, 19(4): 575-591, 2015

This project implements both conventional version (CPSO) and discrete version (DPSO) of PSO for covering array generation. It also extends four famous PSO variants to their discrete versions.

* *TVAC*: time-varying acceleration coefficients PSO [1]
* *CLPSO*: comprehensive learning PSO [2]
* *APSO*: adaptive PSO [3]
* *DMS-PSO*: multi-swarm PSO [4]

Moverover, as evolutionary algorithms are highly impacted by their parameter settings, we have conducted parameter tunings for both CPSO and DPSO. The recommended parameter settings for covering array generation are as follows:

|                       | CPSO | DPSO |
|:----------------------|:----:|:----:|
| population size       | 80   | 80   |
| maximum iteration     | 250  | 250  |
| inertia weight        | 0.9  | 0.5  |
| acceleration constant | 1.3  | 1.3  |
| pro1 (discrete)       |      | 0.5  |
| pro2 (discrete)       |      | 0.3  |
| pro3 (discrete)       |      | 0.7  |


## Building

Run `make` to build.

## Usage

`DPSO [OPTIONS] [MODEL_FILE]`

For example, run `./DPSO specification.txt` to generate a covering array for the testing model described in **specification.txt**, which is **CA(N; 2, 3^5)**. More options can be found by `./DPSO --help`.

DPSO provides both conventional and discrete versions of standard PSO and its four variants. These algorithms include `CPSO`, `DPSO` (default), `CTVAC`, `DTVAC`, `CCLPSO`, `DCLPSO`, `CAPSO`, `DAPSO`, `CDMSPSO`, `DDMSPSO`.

The model file gives parameters, values and covering strength. It has the following format:

```
[value of covering strength]
[number of parameters]
[number of values for each parameter]
```

The constraint file gives the dependences among parameter values. It has the following format:

```
[number of disjunctive clauses]
[number of terms in this disjunctive clause]
[+ for plain, - for negated] [variable] [more plain or negated variables...]
[more disjunctive clauses...]
```

## Reference

[1] A. Ratnaweera, S. Halgamuge, and H. Watson, "Self-organizing hierarchical particle swarm optimizer with time-varying acceleration coefficients," IEEE Transactions on Evolutionary Computation, 8(3), 2004

[2] J. Liang, A. Qin, P. Suganthan, and S. Baskar, "Comprehensive learning particle swarm optimizer for global optimization of multimodal functions," IEEE Transactions on Evolutionary Computation, 10(3), 2006

[3] Z. hui Zhan, J. Zhang, Y. Li, and H.-H. Chung, "Adaptive particle swarm optimization," IEEE Transactions on Systems, Man, and Cybernetics, Part B: Cybernetics, 39(6), 2009

[4] J. Liang and P. Suganthan, "Dynamic multi-swarm particle swarm optimizer," in proceedings of Swarm Intelligence Symposium, pp.124-129, 2005

