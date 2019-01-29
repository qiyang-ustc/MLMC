#include "my_vrbls.h"
#include "measure.h"
#include <math.h>
#include <stdio.h>
#include <ctime>
//-- - PROJECT - INDEPENDENT------------------------------------------ -
//============= = Collect data ========================================
//THIS IS PROJECT - INDEPENDENT
void coll_data(int iblck)
{
	for (int j = 0;j < NObs_b;j++)
	{
		Obs(j,iblck) = Obs(j,iblck) + Quan(j);
	}
}
//============= = Normalize by Nsamp ==================================
//THIS IS PROJECT - INDEPENDENT
void norm_Nsamp(int iblck)
{
	int j;
	double nor;
	nor = 1.0 / NSample;
	for (j = 0;j < NObs_b;j++)
	{
		Obs(j, iblck) = Obs(j, iblck) * nor;
	}
}
//===================================================================

//============= = Statistics ==========================================
//THIS IS PROJECT - INDEPENDENT
void stat_analy()
{
	int j, k, k0;
	double devn, devp, nor;

	//--calculate average------------------------------------------ -
	nor = 1.0 / NBlck;
	for (j = 0;j < NObs_b;j++)
	{
		Ave(j) = 0;
		for (k = 0;k < NBlck;k++)
		{
			Ave(j) += Obs(j,k);
		}
		Ave(j) = nor*Ave(j);
	}


	while (1)
	{
		//--calculate error and t = 1 correlation for basics obs.--------
		prt = 1;
		for (j = 0;j < NObs_b;j++)
		{
			devp = 0;
			Cor(j) = 0;
			Dev(j) = 0;
			for (k = 0;k < NBlck;k++)
			{
				devn = Obs(j,k) - Ave(j);
				Dev(j) = Dev(j) + devn*devn;
				Cor(j) = Cor(j) + devn*devp;
				devp = devn;
			}

			Dev(j) = Dev(j) * nor;        Cor(j) = Cor(j) * nor;

			if (Dev(j) > eps) Cor(j) = Cor(j) / Dev(j);
			Dev(j) = sqrt(Dev(j) / (NBlck - 1.0));
			if (fabs(Cor(j)) > tol) prt = 0;
		}

		if (prt)break;
		if (NBlck <= 64) { prt = 0; break; }

		//--coarsen blocking------------------------------------------
		NBlck = NBlck / 2;
		nor = nor * 2;
		for (j = 0;j < NObs_b;j++)
		{
			k0 = 0;
			for (k = 0;k < NBlck;k++)
			{
				Obs(j,k) = (Obs(j,k0) + Obs(j,k0 + 1))*0.5;
				k0 = k0 + 2;
			}
		}
	}

	//--define auxillary variables and average of composite obs.---- -
	Cal_Obs_Comp();

	//--calculate error and t = 1 correlation for composite obs.---- -
	for (j = NObs_b;j < NObs;j++)
	{
		devp = 0;  
		Cor(j) = 0;
		Dev(j) = 0;
		for (k = 0;k < NBlck;k++)
		{
			devn = Obs(j,k) - Ave(j);
			Dev(j) = Dev(j) + devn*devn;
			Cor(j) = Cor(j) + devn*devp;
			devp = devn;
		}
		Dev(j) = Dev(j) * nor;
		Cor(j) = Cor(j) * nor;
		if (Dev(j) > eps) Cor(j) = Cor(j) / Dev(j);

		Dev(j) = sqrt(Dev(j) / (NBlck - 1.0));
	}
}

//===================================================================

//============= = Write to files ==================================== =
						//THIS IS PROJECT - DEPENDENT
void write2file()
{
	FILE *fp = NULL;
	int j, k, Nwri;
	errno_t err;
	//--open file----------------------------------------------------
	if (prt)
	{
		err = fopen_s(&fp,data_file, "a");
	}
	else
	{
		err = fopen_s(&fp,cor_data_file, "a");
	}
	if (err != 0)
	{
		std::cout << "Fail to open files" << std::endl;
		system("pause");
		exit(1);
	}
	std::cout << std::endl;
	fprintf(fp, "\n");

	//--write to data file--------------------------------------------
	fprintf(fp, "%s\t%d\t%d\t%d\t%d\t%g\t%d\t%d\n", ident, N, NToss, NSample, sweep, Jcp, seed, NBlck); //--------------This line depends on project
	for (j = 0;j < NObs;j++)
	{
		std::cout << j << "	" << Ave(j) << "	" << Dev(j) << "	" << Cor(j) << std::endl;;
		fprintf(fp, "%d\t%12.9lf\t%12.9lf\t%8f\n", j, Ave(j), Dev(j), Cor(j));
	}
	//ReceiveProbability
	fprintf(fp, "Receive-Probability= %lf\n", 1.0*test_receive / (test_receive + test_refuse));
	fprintf(fp, "Avergae - Steps	= %lf\n", 1.0*jumped_steps / sweep / NBlck / NSample);
	fprintf(fp, "Bias				= %lf\n", bias);
	//--write to output file if #block is too small------------------ -
	if (NBlck <= 64)
	{
		std::cout << std::endl;
		Nwri = NObs_b;       if (Nwri > 5) Nwri = 5;
		for (k = 0;k < NBlck;k++)
		{
			std::cout << k << '\t';
			for (j = 0;j < Nwri;j++)
			{
				std::cout << Obs(j,k) << '\t';
			}
			std::cout << std::endl;
		}
	}

	t_end = clock();
	printf("simulation  time:%lf s\n", (double)t_simu / CLOCKS_PER_SEC);
	printf("measure     time:%lf s\n", (double)t_meas / CLOCKS_PER_SEC);
	printf("total CPU   time:%lf s\n", (double)(t_end-t_start) / CLOCKS_PER_SEC);
	
	fclose(fp);
}
//===================================================================