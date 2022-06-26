#include "systemc.h"

SC_MODULE(hardware) {
	double x, y, z, f, w;
	const int c1 = 2, c2 = 2;
	double Xh[3], Vh[3], random, pBest[3], gBest[3];
	sc_event calcF_start, f_ready, f_used;
	sc_event calcV_start, v_ready, v_used;

	void prc_hardware_calcF() {
		while (true) {
			wait(calcF_start);
			f = x * x + y * y + z * z;
			f_ready.notify(SC_ZERO_TIME);
			wait(f_used);
		}
	}

	void prc_harware_calcV() {
		while (true) {
			wait(calcV_start);
			w = ((rand() % 101) - 50) / 100;
			Vh[0] = w * Vh[0] + c1 * random * (pBest[0] - Xh[0]) +
				c2 * random * (gBest[0] - Xh[0]);
			Vh[1] = w * Vh[1] + c1 * random * (pBest[1] - Xh[1]) +
				c2 * random * (gBest[1] - Xh[1]);
			Vh[2] = w * Vh[2] + c1 * random * (pBest[2] - Xh[2]) +
				c2 * random * (gBest[2] - Xh[2]);
			v_ready.notify(SC_ZERO_TIME);
			wait(v_used);
		}
	}

	SC_CTOR(hardware) {
		SC_THREAD(prc_hardware_calcF);
		SC_THREAD(prc_harware_calcV);
	}
};

static hardware HW("hw");

static void calcV(double random, double pBest[3], double X[3], double gBest[3], double* V) {
	HW.random = random;
	HW.pBest[0] = pBest[0];
	HW.pBest[1] = pBest[1];
	HW.pBest[2] = pBest[2];
	HW.gBest[0] = gBest[0];
	HW.gBest[1] = gBest[1];
	HW.gBest[2] = gBest[2];
	HW.Xh[0] = X[0];
	HW.Xh[1] = X[1];
	HW.Xh[2] = X[2];
	HW.Vh[0] = V[0];
	HW.Vh[1] = V[1];
	HW.Vh[2] = V[2];
	HW.calcV_start.notify(SC_ZERO_TIME);
	wait(HW.v_ready);
	V[0] = HW.Vh[0];
	V[1] = HW.Vh[1];
	V[2] = HW.Vh[2];
	HW.v_used.notify(SC_ZERO_TIME);
}

static double calcF(double x, double y, double z) {
	HW.x = x;
	HW.y = y;
	HW.z = z;
	HW.calcF_start.notify(SC_ZERO_TIME);
	wait(HW.f_ready);
	double result = HW.f;
	HW.f_used.notify(SC_ZERO_TIME);
	return result;
}