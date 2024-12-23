# Experiment Codes for Game-Theoretically Secure Distributed Protocols for Fair Allocation in Coalitional Games

Please refer to the experiment section of this paper.

## exp1_synthetic.cpp

This is the code for testing the number of P-samples under different $n$, $C$ and $\epsilon$ for the synthetic game. Prepare the config file containing the following five parameters (e.g., let the config file be `config1_synthetic.in`):

```
C epsilon delta n n'
```

where `n` is the number of players, `n'` is the number of corrupted players.

To compile and run the code, use the following commands:

```
g++ exp1_synthetic.cpp -o exp1_synthetic
./exp1_synthetic config1_synthetic.in
```

## exp2_synthetic.cpp

This is the code for computing the Empirical Cumulative Distribution Function over 1000 simulations for the synthetic game. Prepare the config file containing the following six parameters (e.g., let the config file be `config2_synthetic.in`):

```
T C epsilon delta n n'
```

where `T` is the number of P-samples per simulation, `n` is the number of players, and `n'` is the number of corrupted players.

To compile and run the code, use the following commands:

```
g++ exp2_synthetic.cpp -o exp2_synthetic
./exp2_synthetic config2_synthetic.in
```

## exp1_edgesyn.cpp

This is the code for testing the number of P-samples under different $n$, $C$ and $\epsilon$ for the edge synergy game. Prepare the config file in the following format (e.g., let the config file be `config1_edgesyn.in`):

```
C epsilon delta n1 n2
<the number of papers m>
num1 person_1 person_2 ... person_num1
num2 person_1 person_2 ... person_num2
...
numm person_1 person_2 ... person_numm
```

where `n1` is the number of non-collaborated players, and `n2` is the number of collaborated players.

To compile and run the code, use the following commands:

```
g++ exp1_edgesyn.cpp -o exp1_edgesyn
./exp1_edgesyn dblp_2021.in config_21.in
```

## exp2_edgesyn.cpp

This is the code for getting the Empirical Cumulative Distribution Function over 1000 simulations for the edge synergy game. Prepare the config file in the following format (e.g., let the config file be `config2_edgesyn.in`): 

```
T C epsilon delta n1 n2
<the number of papers m>
num1 person_1 person_2 ... person_num1
num2 person_1 person_2 ... person_num2
...
numm person_1 person_2 ... person_numm
```

where `T` is the number of P-samples per simulation, `n1` is the number of non-collaborated players, and `n2` is the number of collaborated players.

To compile and run the code, use the following commands:

```
g++ exp2_edgesyn.cpp -o exp2_edgesyn
./exp2_edgesyn config2_edgesyn.in
```
