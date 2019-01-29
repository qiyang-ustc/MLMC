#include <math.h>
#include "my_vrbls.h"
#include "measure.h"
#include "initialize.h"
void Metropolis(int* p)
{
	double Prev_Energy;
	
	Prev_Energy = Energy;
	*p = -*p;
	Cal_Energy();
	if (Energy<Prev_Energy || rn() < exp(Jcp*(Prev_Energy - Energy)))//Satisfying and updating
	{
		test_receive++;
	}
	else
	{
		test_refuse++;
		*p = -*p;
		Energy = Prev_Energy;
	}
}
bool Clock_Refuse(int* p,int iclock)
{
	double prev_energy, delta_energy;
	prev_energy = Energy_Terms(iclock);
	*p = -*p;
	delta_energy = (Energy_Terms(iclock) - prev_energy);
	*p = -*p;
	if (delta_energy< 0|| rn() > (1-exp(-Nor_Jcp*delta_energy)) / (1-Exp_Minus_Nor_Jcp)  )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void Clock_Metropolis(int* p)
{
	int iclock = 0;
	while (1)
	{
		iclock = iclock + (int)(-log(rn())/ Nor_Jcp);
		jumped_steps++;
		if (iclock >= NV || iclock < 0)
		{
			*p = -*p;
			test_receive++;
			break;
		}
		else
		{
			if (Clock_Refuse(p,iclock))//The state refuse the updating
			{
				test_refuse++;
				break;
			}
			iclock++;
		}
	}
}

void Markov()
{
    /*1.Select a spin to transverse
      2.Decide whether to transverse this spin
      3.return back.*/

    //----1------
    static int indicator;
    static int* point;
    
    for(int i=0;i<sweep;i++)
    {
		indicator = abs(rn_i() % Vol);
        point = find(indicator);

       //Metropolis(point);
        Clock_Metropolis(point);
		//std::cout << *point << std::endl;
    }

}