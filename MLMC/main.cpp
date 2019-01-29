#include <iostream>
#include <fstream>
#include "my_rng.h"
#include "my_vrbls.h"
#include <stdlib.h>
#include "initialize.h"
#include "Markov.h"
#include "measure.h"
#include "statistics.h"
int main()
{
	int itoss, isamp, iblck;
	//Parameters setting
	NToss = 50000;NSample = 1000; sweep = 20;Jcp = 5;seed = 22237; NBlck = 1024;
	TotalSample = NSample*NBlck;

	//---Initialization----------------------------------------------
	set_elapse_time();
	initialize();
	//elapse_time();
	//printf("        set up time:%lf s\n",t_past/CLOCKS_PER_SEC);


	//---Thermolization
	for (itoss = 0;itoss < NToss;itoss++)
	{
		Markov();
	}
	//elapse_time();
	//printf("thermalization time:%lf s\n", t_past / CLOCKS_PER_SEC);

	//-------Simulation
	for (iblck = 0;iblck < NBlck;iblck++)
	{
		for (isamp = 0; isamp < NSample; isamp++)
		{	
				//elapse_time();
			Markov();
				//elapse_time();
				//t_simu += t_past;
			measure();
				//elapse_time();
				//t_meas += t_past;
			coll_data(iblck);
			//std::cout << Energy<<std::endl;
		}
		norm_Nsamp(iblck);
	}
	//elapse_time();
	//printf("    Simulation time:%lf s\n", t_past / CLOCKS_PER_SEC);
		
	stat_analy();
	write2file();
	system("pause");
}
