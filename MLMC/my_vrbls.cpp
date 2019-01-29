#include <math.h>
#include <vector>
#include "my_rng.h"
#include <fstream>
#include <string>
#include <Eigen/Dense>
int Ipow(int a, int b)
{
	int sum=1;
	for (int i = 0;i < b;i++)
	{
		sum *= a;
	}
	return sum;
}


//-------Parameters used in Output----
//-------Project-depended---
char	      ident[] = "ML-NNNN-Int";	    //Project name
char		  data_file[] = "ML.dat";		//File for proper data
char		  cor_data_file[] = "cor.dat";	//File for big correlation data
bool		  prt;							//indicate whether we which file we need to out put to


//-------Parameters used in Science----
//-------Project-independed---
int		      Mxint = 2147483647;			// maximum int
int		      Mnint = -2147483647;			// minimum int
double		   tm32 = 1.0 / Ipow(2, 32);
double		   eps = 1.0E-14;				//very small number
double		   tol = 0.20;					//tolerance of correlation
double		   Pi= 4 * atan(1.0);

//-------Parameters used in Monte Carlo simulation----
//-------Project-independed---
int NBlck;
int NSample;
int TotalSample;
int NToss;
int TossThrown;
int MxBlck = 1024;
int MnBlck = 2;
//-------Parameters used in Markov Cahin---
//-------Project-depended//----
double Jcp;
double Nor_Jcp;					//  Nor_Jcp = Jcp/NV;
double Exp_Minus_Nor_Jcp;		//  Exp_Minus_Nor_Jcp = exp(-Nor_Jcp)
int sweep;
//-------Dynamical Parameters in Markov Chain--
double Energy=1;				//Max Energy in this system;


//-------Prameters used in Simulation----
//-------Project-depended//----
int			  N = 3;						//dimension of input
Eigen::MatrixXi	  Obj_A1(N, N);				//The architecture of the neural network.
Eigen::MatrixXi	  Obj_A2(N, N);
Eigen::MatrixXi	  Obj_A3(N, N);
Eigen::MatrixXi	  A1(N, N);
Eigen::MatrixXi	  A2(N, N);
Eigen::MatrixXi	  A3(N, N);

Eigen::VectorXi	  Obj_B1(N);
Eigen::VectorXi	  Obj_B2(N);
Eigen::VectorXi	  Obj_B3(N);
Eigen::VectorXi	  B1(N);
Eigen::VectorXi	  B2(N);
Eigen::VectorXi	  B3(N);

Eigen::VectorXd	  C1(N);
Eigen::VectorXd	  C2(N);
Eigen::VectorXd	  C3(N);

Eigen::VectorXi	  Input(N);							//Generate Input every times
Eigen::VectorXi   Result(N);						//The answer for 2^n vectors


double		VNor = 1.0 / Ipow(2, N);
int			NV = Ipow(2, N);
int			Vol = 3 * N*N + 3 * N;					//Number of spins
long test_receive = 0;
long test_refuse = 0;
long jumped_steps = 0;
double bias;
//-----------------------------------------------------------------


//--Statistics and measurement---------------------------------------
//THIS IS PROJECT - DEPENDENT
int NObs_b		 = 5;								//#basic     observables
int NObs_c		 = 0;								//#composite observables
int NObs		 = NObs_b + NObs_c;					//#total     observables
Eigen::ArrayXd  Quan(NObs_b);						//Measured quantities
Eigen::ArrayXXd Obs(NObs,MxBlck);					//1st--#quan.  2nd--#block
Eigen::ArrayXd  Ave(NObs), Dev(NObs), Cor(NObs);	//average, error bars, and correlation of observables
double BSC_p;										//probability for binary symmetric channel C_{ BCS }(p)
//-----------------------------------------------------------------
