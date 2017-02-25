# C-Parallel-vs-Serial-Sorting
Project shows how parallelization of the simple bubble sort gains the speedup

<h3>Synopsis </h3>
<p>This project is a school example of showing how parallelization of the simple sorting can gain better results. It uses C library "mpi" which stands for Message Passing Interface. It enables the user to use communication between processes and threads by implementing simple MPI functions. Project first generates desired number of FLOAT numbers and than first sorts it using MPI, and than using simple bubble sort. It calculates times for each sorting and displays the speedup.</p>

<h3>Tests </h3>
<p>Program has been set to generate 300 floating point numbers. That number can be changed by editing constant variable N to desired number.</p>
<p>Because of using MPI, program can not be compiled by ordinary 'gcc' compiler. You should use following syntax in the command line: 'mpicc sort.c -o desired_name'. To run the program, use: './desired_name' from the command line.</p>
