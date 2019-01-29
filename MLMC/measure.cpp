#include <math.h>
#include "my_vrbls.h"
#include <Eigen/Dense>
//============= = Measurement ======================================== =
//THIS IS PROJECT - DEPENDENT
int sgn(double x)
{
	if (x > 0)
	{
		return 1;
	}
	else if (x < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
void random_sgn(Eigen::VectorXi &temp,Eigen::VectorXd &c)
{
	for (int i = 0;i < N;i++)
	{
		temp(i) = sgn(temp(i) + c(i));
	}
}
double Energy_Terms(int num)
{
/*	1.Generate Input
	2.Calculate Result Output.
	3.Calculate Inner Product.*/

	//---1---
	int temp=num;
	for (int j = 0;j < N;j++)
	{
		Input(j) = 2 * (temp % 2) - 1;
		temp = temp / 2;
	}
	//---2---

	
	Result = Obj_A1*Input + Obj_B1;
	random_sgn(Result, C1);		//layer 1

	Result = Obj_A2*Result + Obj_B2;
	random_sgn(Result, C2);		//layer 2

	Result = Obj_A3*Result + Obj_B3;
	random_sgn(Result, C3);		//layer 3

								/* generate <Result> first
									Because we need to change <Input> later */
	Input = A1*Input + B1;
	random_sgn(Input, C1);		//layer 1					

	Input = A2*Input + B2;
	random_sgn(Input, C2);		//layer 2

	Input = A3*Input + B3;
	random_sgn(Input, C3);		//layer 3
	
	//---3----

	Input = Input - Result;
	return Input.dot(Input)/4.0/N;
}


//Energy = (f-g)^2/(2^N*4N)
void Cal_Energy()
{
	double sum;
	double temp;
	sum = 0;
	for (int i = 0;i < NV;i++)
	{
		temp = Energy_Terms(i);
		sum = sum + temp;
	}
	Energy = sum*VNor;
}

void measure()
{
Cal_Energy();
Quan(0) = Energy;
Quan(1) = Energy*Energy;
Quan(2) = A1(1, 1);
Quan(3) = A2(1, 1);
Quan(4) = A3(1, 1);
}

void cal_Q(int jb, int b1, int b2, int epo)
{
	int k;
	double tmp;

	//--Average----------------------------------------------------
	tmp = pow(Ave[b1], epo);   if (fabs(tmp) > eps) tmp = Ave[b2] / tmp;
	Ave[jb] = tmp;

	//--Obs(j, k) series--------------------------------------------
	for (k = 0;k < NBlck;k++) 
	{
		tmp = pow(Obs(b1,k), epo);   if (fabs(tmp) > eps) tmp = Obs(b2,k) / tmp;
		Obs(jb,k) = tmp;
	}
}
//===================================================================== =

//============= = Calculate Binder ratio 2================================
//Q = <(o - <o>) ^ 4> / <(o - <o>) ^ 2> ^ 4
//THIS IS PROJECT - DEPENDENT
void Cal_QE(int jb,int b1,int b2,int b3,int b4)
{
	int k;
	double a1, a2, a3, a4, q0, q1;
	//--Average----------------------------------------------------
	a1 = Ave(b1);  a2 = Ave(b2);  a3 = Ave(b3);   a4 = Ave(b4);
	q0 = a4 - 4 * a3*a1 + 6 * a2*a1*a1 - 3 * pow(a1, 4.0);
	q1 = a2 - a1*a1;   q1 = q1*q1;
	Ave(jb) = 2.0;   if (q1 > eps) Ave(jb) = q0 / q1;

	//--Obs(j, k) series--------------------------------------------
	for(k = 0;k < NBlck;k++)
	{
		a1 = Obs(b1,k);  a2 = Obs(b2,k);  a3 = Obs(b3,k);   a4 = Obs(b4,k);
		q0 = a4 - 4 * a3*a1 + 6 * a2*a1*a1 - 3 * pow(a1, 4.0);
		q1 = a2 - a1*a1;   q1 = q1*q1;
		Obs(jb,k) = 2.0;   
		if (q1 > eps) Obs(jb,k) = q0 / q1;
	}
}
//======================================================================

void Cal_Obs_Comp()
{
	//int jb, b2, b3, b1;
	//--calculate the average----------------------------------------
	//jb = NObs_b + 1;   call cal_QE(jb, 1, 2, 3, 4)           //Qe = <E ^ 4> / <E ^ 2> ^ 2
}//===================================================================
