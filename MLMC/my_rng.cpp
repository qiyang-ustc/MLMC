#include <math.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <ctime>
//------------------seed settings-----------
//----------Note:The seed function is system-depened---
//time seed;
//seed = year+70(month+12(day+31(hour+23(min+59*second))))

static time_t tm_temp=time(NULL);
static long tm = (long)tm_temp;
static long year = tm / 31536000;
static long month = (tm % 31536000) / 2592000;
static long day = (tm % 2592000) / 86400;
static long hour = (tm % 86400) / 3600;
static long min = (tm % 3600) / 60;
static long sec = tm % 60;
int seed = year + 70 * (month + 12 * (day + 31 * (hour + 23 * (min + 59 * sec))));

//-------------Random number generator-------


//-------------Random number generator settings---
// static declaration:
const static int MxInt = 2147483647;
const static int mult = 32781;
const static int mod2 = 2796203, mul2 = 125;
const static int len1 = 9689, ifd1 = 471;
const static int len2 = 127, ifd2 = 30;
static int inxt1[len1 + 1], inxt2[len2 + 1];
static int ir1[len1 + 1], ir2[len2 + 1];
static int ipnt1, ipnf1;
static int ipnt2, ipnf2;
const static int mxrn = 10000;
static int irn[mxrn + 1];
static int nrannr; // random-number seed and counter
static int i_r, k_r, k1_r, iseed, l_r;

void Set_RNG()
{
	nrannr = mxrn;
	iseed = abs(seed) + 1;
	k_r = 387420489 + 2 * iseed; // 387 420 489=3^18
	k1_r = 1313131 * iseed;
	k1_r = k1_r - (k1_r / mod2) * mod2;

	for (i_r = 1; i_r <= len1; i_r++) {
		k_r = k_r * mult;
		k1_r = k1_r * mul2;
		k1_r = k1_r - (k1_r / mod2) * mod2;
		ir1[i_r] = k_r + k1_r * 8193;
	}

	for (i_r = 1; i_r <= len2; i_r++) {
		k_r = k_r * mult;
		k1_r = k1_r * mul2;
		k1_r = k1_r - (k1_r / mod2) * mod2;
		ir2[i_r] = k_r + k1_r * 4099;
	}

	for (i_r = 1; i_r <= len1; i_r++) {
		inxt1[i_r] = i_r + 1;
	}
	inxt1[len1] = 1;
	ipnt1 = 1;
	ipnf1 = ifd1 + 1;

	for (i_r = 1; i_r <= len2; i_r++) {
		inxt2[i_r] = i_r + 1;
	}
	inxt2[len2] = 1;
	ipnt2 = 1;
	ipnf2 = ifd2 + 1;
	// Setting end
}

double rn() 
{
  // random-number generator:
  // copy from function rng.f90
  nrannr = nrannr + 1;
  if (nrannr >= mxrn) {
    nrannr = 1;
    for (i_r = 1; i_r <= mxrn; i_r++) {
      l_r = ir1[ipnt1] ^ ir1[ipnf1];
      k_r = ir2[ipnt2] ^ ir2[ipnf2];
      irn[i_r] = k_r ^ l_r;
      ir1[ipnt1] = l_r;
      ipnt1 = inxt1[ipnt1];
      ipnf1 = inxt1[ipnf1];
      ir2[ipnt2] = k_r;
      ipnt2 = inxt2[ipnt2];
      ipnf2 = inxt2[ipnf2];
    }
  }
  return 1.0 * irn[nrannr] / MxInt / 2.0 + 0.5;
}

int rn_i() {
  // static declaration:
  const static int MxInt = 2147483647;
  const static int mult = 32781;
  const static int mod2 = 2796203, mul2 = 125;
  const static int len1 = 9689, ifd1 = 471;
  const static int len2 = 127, ifd2 = 30;
  static int inxt1[len1 + 1], inxt2[len2 + 1];
  static int ir1[len1 + 1], ir2[len2 + 1];
  static int ipnt1, ipnf1;
  static int ipnt2, ipnf2;
  const static int mxrn = 10000;
  static int irn[mxrn + 1];
  static int nrannr; // random-number seed and counter

  // settings:
  static int i_r, k_r, k1_r, iseed, l_r;
  static int rn_initialize = 1;
  if (rn_initialize == 1) 
  {
    nrannr = mxrn;
    iseed = abs(seed) + 1;
    k_r = 387420489 + 2 * iseed; // 387 420 489=3^18
    k1_r = 1313131 * iseed;
    k1_r = k1_r - (k1_r / mod2) * mod2;

    for (i_r = 1; i_r <= len1; i_r++) {
      k_r = k_r * mult;
      k1_r = k1_r * mul2;
      k1_r = k1_r - (k1_r / mod2) * mod2;
      ir1[i_r] = k_r + k1_r * 8193;
    }

    for (i_r = 1; i_r <= len2; i_r++) {
      k_r = k_r * mult;
      k1_r = k1_r * mul2;
      k1_r = k1_r - (k1_r / mod2) * mod2;
      ir2[i_r] = k_r + k1_r * 4099;
    }

    for (i_r = 1; i_r <= len1; i_r++) {
      inxt1[i_r] = i_r + 1;
    }
    inxt1[len1] = 1;
    ipnt1 = 1;
    ipnf1 = ifd1 + 1;

    for (i_r = 1; i_r <= len2; i_r++) {
      inxt2[i_r] = i_r + 1;
    }
    inxt2[len2] = 1;
    ipnt2 = 1;
    ipnf2 = ifd2 + 1;
    // Setting end
    rn_initialize = 0;
  }
  // random-number generator:
  // copy from function rng.f90
  nrannr = nrannr + 1;
  if (nrannr >= mxrn) {
    nrannr = 1;
    for (i_r = 1; i_r <= mxrn; i_r++) {
      l_r = ir1[ipnt1] ^ ir1[ipnf1];
      k_r = ir2[ipnt2] ^ ir2[ipnf2];
      irn[i_r] = k_r ^ l_r;
      ir1[ipnt1] = l_r;
      ipnt1 = inxt1[ipnt1];
      ipnf1 = inxt1[ipnf1];
      ir2[ipnt2] = k_r;
      ipnt2 = inxt2[ipnt2];
      ipnf2 = inxt2[ipnf2];
    }
  }
  return irn[nrannr];
}

double rn_log()
{
	return -log(rn());
}
//-------------trace time-elapse-------
clock_t t_prev;
clock_t t_now;
clock_t t_start;
clock_t t_end;
double t_past;
void set_elapse_time()
{
    t_prev = clock();
	t_now = clock();
	t_start = clock();
}

void elapse_time()
{
	t_now = clock();
	t_past = (double)(t_now - t_prev);
	t_prev = clock();
}
//t_start======t_prev + t_past = t_now ========t_end


//----------Parameters used in time-monitor----
//---------This is project independent------
double t_simu=0;					// time used for simulation
double t_meas=0;					// time used for measuring
double t_full=0;					// time used for full program
