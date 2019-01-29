#include <math.h>
#include "my_vrbls.h"
#include <iostream>
#include <stdio.h>
#include "Markov.h"
#include "measure.h"
//----this "find(indicator)"is highly project-dependent
//---- PROJECT - DEPENDENT---------------------------------------------
//WARNING: This model should be changed with int* find(indicator)!!
int* Obj_find(int indicator)
{
	static int x;
	static int y;

	if (indicator<N)
	{
		return &(Obj_B1(indicator));
	}
	else
	{
		indicator -= N;
	}

	if (indicator<N)
	{
		return &(Obj_B2(indicator));
	}
	else
	{
		indicator -= N;
	}

	if (indicator<N)
	{
		return &(Obj_B3(indicator));
	}
	else
	{
		indicator -= N;
	}

	if (indicator<N*N)
	{
		x = indicator / N;
		y = indicator%N;
		return &(Obj_A1(x, y));
	}
	else
	{
		indicator -= N*N;
	}

	if (indicator<N*N)
	{
		x = indicator / N;
		y = indicator%N;
		return &(Obj_A2(x, y));
	}
	else
	{
		indicator -= N*N;
		x = indicator / N;
		y = indicator%N;
		return &(Obj_A3(x, y));
	}
}
int* find(int indicator)
{
	static int x;
	static int y;

	if (indicator<N)
	{
		return &(B1(indicator));
	}
	else
	{
		indicator -= N;
	}

	if (indicator<N)
	{
		return &(B2(indicator));
	}
	else
	{
		indicator -= N;
	}

	if (indicator<N)
	{
		return &(B3(indicator));
	}
	else
	{
		indicator -= N;
	}

	if (indicator<N*N)
	{
		x = indicator / N;
		y = indicator%N;
		return &(A1(x, y));
	}
	else
	{
		indicator -= N*N;
	}

	if (indicator<N*N)
	{
		x = indicator / N;
		y = indicator%N;
		return &(A2(x, y));
	}
	else
	{
		indicator -= N*N;
		x = indicator / N;
		y = indicator%N;
		return &(A3(x, y));
	}
}
//============= = Test and Print ======================================
//!!THIS IS PROJECT - DEPENDENT
void tst_and_prt()
{
//--Test and Print---------------------------------------------- -
	if ((NBlck>MxBlck)||(NBlck<MnBlck))
	{
		std::cout << "MnBlck <= NBlck <=MxBlck?" << std::endl;
		exit(1);
	}

	if ((NBlck>200) && (NBlck != MxBlck))
	{
		std::cout << "NBlck>200 is supposed for extensive simulation. NBlck=MxBlk is suggested!" << std::endl;
		exit(1);
	}

	printf(" AT on N=%d model\n",N);
	printf(" coupling = %g\n", Jcp);
	printf(" Will simulate \t%d steps\n", NSample*NBlck);
	printf(" Blocks        \t%d\n", NBlck);
	printf(" Throw away    \t%d steps\n", NToss);
}

//!!THIS IS PROJECT - INDEPENDENT!!!
void Initialize_Objective_Function()
{
	
	for (int i = 0;i < Vol;i++)
	{
		if (rn() > 0.5)
		{
			*Obj_find(i) = 1;
		}
		else
		{
			*Obj_find(i) = -1;
		}
	}
}

void Initialize_Configuration()
{
	for (int i = 0;i < Vol;i++)
	{
		if (rn() > 0.5)
		{
			*find(i) = 1;
		}
		else
		{
			*find(i) = -1;
		}
	}
}

void Initialize_Random_Layers() 
{
	//initialize C1
	for (int i = 0;i < N; i++)
	{
		if (rn() > 0.5)
		{
			C1(i) = +0.5;
		}
		else
		{
			C1(i) = -0.5;
		}
	}
	//initialize C2
	for (int i = 0;i <N;i++)
	{
		if (rn() > 0.5)
		{
			C2(i) = +0.5;
		}
		else
		{
			C2(i) = -0.5;
		}
	}
	//initialize C3
	for (int i = 0;i <N;i++)
	{
		if (rn() > 0.5)
		{
			C3(i) = +0.5;
		}
		else
		{
			C3(i) = -0.5;
		}
	}

}

void Initialize_Quantities()
{
	Nor_Jcp = Jcp / NV;
	Exp_Minus_Nor_Jcp = exp(-Nor_Jcp);
}

void Print_Result(int num)
{
	int sum=0;
	std::cout << "(" << num + 1 << ")\t";
	for (int j = 0;j < N;j++)
	{
		sum += Result(j);
		std::cout << Result(j) << "\t";
	}
	std::cout << "Output Unbias = " << fabs(1.0*sum / N / NV) << std::endl;
}
void Print_Outputs()
{
	int temp;
	int sum = 0;
	std::cout<<std::endl;

	std::cout << "Obj_A1=\n" << Obj_A1<<std::endl;

	std::cout << "Obj_A2=\n" << Obj_A2 << std::endl;

	std::cout << "Obj_A3=\n" << Obj_A3 << std::endl;

	std::cout << "Obj_B1=\n" << Obj_B1 << std::endl;

	std::cout << "Obj_B2=\n" << Obj_B2 << std::endl;

	std::cout << "Obj_B3=\n" << Obj_B3 << std::endl;

	std::cout << "C1=\n" << C1 << std::endl;

	std::cout << "C2=\n" << C2 << std::endl;

	std::cout << "C3=\n" << C3 << std::endl;
	for(int num=0;num<NV;num++)
	{
		temp = num;
		for (int j = 0;j < N;j++)
		{
			Input(j) = 2 * (temp % 2) - 1;
			temp = temp / 2;
		}
		Result = Obj_A1*Input + Obj_B1;
		random_sgn(Result, C1);		//layer 1
		
		Result = Obj_A2*Result + Obj_B2;
		random_sgn(Result, C2);		//layer 2
		
		Result = Obj_A3*Result + Obj_B3;
		random_sgn(Result, C3);		//layer 3
		
		std::cout << "(" << num + 1 << ")\t";
		for (int j = 0;j < N;j++)
		{
			sum += Result(j);
			std::cout << Result(j) << "\t";
		}
		std::cout << std::endl;
	}
	bias = 1.0*sum / N / NV;
	std::cout << "Output Unbias = " << bias << std::endl;

}
//============= = Initialization ======================================
void initialize()
{
	tst_and_prt();
	Set_RNG();
	Initialize_Configuration();							//->
	Initialize_Objective_Function();					/*These two function is project-independent.
														But you should change the two int* find() function*/
	Initialize_Random_Layers();
	Initialize_Quantities();
	Print_Outputs();
	//Initialize_Result();
	//Initialize_Energy();
}