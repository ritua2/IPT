This directory contains the serial code for four applications:
=============================================================
1. Molecular dynamics simulation

2. Heat diffusion equation

3. Circuit satisfiability

4. Matrix multiplication



Goal: 
=====
Transform the aforementioned serial code to OpenMP code either manually or using IPT.



Steps to compile and run the serial code:
========================================
1. Molecular dynamics simulation
Compile: icpc -o md md.c
Run: md 2 500 500 0.01

2. Heat diffusion
Compile: icpc -o heat heat_serial.c calc_up.c
Run: heat

3. Circuit satisfiability
Compile: icpc -o circuit circuit.c
Run: circuit

4. Matrix multiplication
Compile: icpc -o matrix_mul matrix_mul.cc
Run: matrix_mul 10 10 10
 


Before running IPT, set the environment by running the following command from the parent directory named trainingIPT:
=====================================================================================================================
source runBeforeIPT.sh




*PLEASE NOTE*: Do not run code on login nodes. You may want to type the "idev" command to get access to a compute node and run IPT or other code from there.
============

             : Load the Intel compiler if you see an error message like the following:
               icpc: command not found


               Command used: ml intel



