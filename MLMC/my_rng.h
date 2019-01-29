/*These are codes about random-number generator:
Most of it are copied from my_rng.f90.
If something makes wrong, you can check the original version
of this random-number generator.
Translated by Qiyang 2017.1.28
*/
#include <time.h>
#ifndef _MY_RNG_
#define _MY_RNG_
#ifndef _INC_MATH
#include <math.h>
#endif
#ifndef _STDIO_H_
#include <stdio.h>
#endif
//RNG - setting
extern void Set_RNG();
// for possible time-seed settings
extern int seed;
/*
rn() will return a double type random-number in U[0,1]
*/
double rn();
/*
rn_log() will return a double type random-number following normal possion distribution
*/
double rn_log();
/*
rn_i() will return an integer type random-number in [- 2147483647, 2147483647];
*/
int rn_i();
//-----------check elapsed time------------
extern clock_t t_prev;
extern clock_t t_now;
extern clock_t t_start;
extern clock_t t_end;
extern double t_past;

//----------Parameters used in time-monitor----
//---------This is project independent------
extern double t_simu;					// time used for simulation
extern double t_meas;					// time used for measure
extern double t_full;					// time used for full program

void set_elapse_time();
void elapse_time();

/**/
#endif
