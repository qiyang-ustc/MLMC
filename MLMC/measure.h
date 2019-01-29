#include <Eigen/Dense>
#ifndef __MY_MEAS__
#define __MY_MEAS__
extern void Cal_Energy();
extern void measure();
extern void Cal_Obs_Comp();
extern int sgn(double );
extern void random_sgn(Eigen::VectorXi &, Eigen::VectorXd &);
extern double Energy_Terms(int);
#endif