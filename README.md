# DPSO

DPSO project focuses on improving the performance of Particle Swarm Optimization (PSO) for covering array generation.

A covering array is a mathematical object that systematically covers the interactions among a fixed number of factors. It can be used for software testing purposes, i.e. combinatorial testing.

To enhance conventional PSO algorithm for covering array generation, DPSO utilizes the set and probability theories to represent particle's velocity, and refines related evolution operators. DPSO further applies two auxiliary strategies to enhance the effectiveness of search. The details of DPSO are in the following paper:

> H. Wu, C. Nie, F.C. Kuo, h. Leung and C.J. Colbourn, "A Discrete Particle Swarm Optimization for Covering Array Generation," IEEE Transactions on Evolutionary Computation, 19(4): 575-591, 2015

This project implements both conventional version (CPSO) and discrete version (DPSO) of PSO for covering array generation. It also extends four famous PSO variants to their discrete versions.

* *TVAC*: Time-Varying Acceleration Coefficients [1]

* *CLPSO*: Comprehensive Learning PSO [2]

* *DMS-PSO*: Multi-Swarm PSO [3]

* *APSO*: Adaptive PSO [4]

Moverover, as evolutionary algorithms are highly impacted by their parameter settings, we have conducted parameter tunings for both CPSO and DPSO. The recommended parameter settings for covering array generation are as follows:

|                       | CPSO | DPSO |
|:----------------------|:----:|:----:|
| population size       | 80   | 80   |
| maximum iteration     | 250  | 250  |
| inertia weight        | 0.9  | 0.5  |
| acceleration constant | 1.3  | 1.3  |
| p1                    |      | 0.5  |
| p2                    |      | 0.3  |
| p3                    |      | 0.7  |


## Build & Run

DPSO is tested under Windows 10 with GCC 5.3.0 (MinGW-64), and Mac OS X 10.11 with Apple LLVM 6.1.0

The generator firstly reads the `filename.model` file of SUT, and then invokes one of the PSO algorithms to construct a covering array. The result is written to `filename.ca` file.

1. Build.

	```
	make
	```

2. Run.

	```
	gen [algorithm] [filename]							# for example: gen dpso example
	```

	All supported algorithms:

	```
	[algorihtm] :=  cpso  |  dpso  |					  # CPSO & DPSO
			        ctvac | cclpso | cdmspso | capso |	  # variants of CPSO
			        dtvac | dclpso | ddmspso | dapso	  # variants of DPSO
	```

	The `filename.model` should be the following format, which represents CA(N;2,3^7):

	```
	parameter 7
	value 3 3 3 3 3 3 3
	tway 2
	```

## Reference

[1] A. Ratnaweera, S. Halgamuge, and H. Watson, "Self-organizing hierarchical particle swarm optimizer with time-varying acceleration coefficients," IEEE Transactions on Evolutionary Computation, 8(3), 2004

[2] J. Liang, A. Qin, P. Suganthan, and S. Baskar, "Comprehensive learning particle swarm optimizer for global optimization of multimodal functions," IEEE Transactions on Evolutionary Computation, 10(3), 2006

[3] J. Liang and P. Suganthan, "Dynamic multi-swarm particle swarm optimizer," in Swarm Intelligence Symposium, proceedings of Swarm Intelligence Symposium, pp.124-129, 2005

[4] Z. hui Zhan, J. Zhang, Y. Li, and H.-H. Chung, "Adaptive particle swarm optimization," IEEE Transactions on Systems, Man, and Cybernetics, Part B: Cybernetics, 39(6) 2009
