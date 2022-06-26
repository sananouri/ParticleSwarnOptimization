#include "systemc.h"

SC_MODULE(software) {
	double newValue, oldValue;
	bool comparison_result;
	double Vs[3], Xs[3];
	sc_event compare_start, comparison_result_ready, comparison_result_used;
	sc_event calcX_start, x_ready, x_used;

	void prc_software_isLower() {
		while (true) {
			wait(compare_start);
			comparison_result = newValue < oldValue;
			comparison_result_ready.notify(SC_ZERO_TIME);
			wait(comparison_result_used);
		}
	}

	void prc_software_calcX() {
		while (true) {
			wait(calcX_start);
			Xs[0] = Xs[0] + Vs[0];
			Xs[1] = Xs[1] + Vs[1];
			Xs[2] = Xs[2] + Vs[2];
			x_ready.notify(SC_ZERO_TIME);
			wait(x_used);
		}
	}

	SC_CTOR(software) {
		SC_THREAD(prc_software_isLower);
		SC_THREAD(prc_software_calcX);
	}
};

static software SW("SW");

static bool compare(double newValue, double oldValue) {
	wait(100, SC_NS); //	delay to demonstrate software being slower than hardware
	SW.newValue = newValue;
	SW.oldValue = oldValue;
	SW.compare_start.notify(1,SC_NS);
	wait(SW.comparison_result_ready);
	bool result = SW.comparison_result;
	SW.comparison_result_used.notify(SC_ZERO_TIME);
	return result;
}

static void calcX(double* X, double V[3]) {
	wait(100, SC_NS); // delay to demonstrate software being slower than hardware
	SW.Vs[0] = V[0];
	SW.Vs[1] = V[1];
	SW.Vs[2] = V[2];
	SW.Xs[0] = X[0];
	SW.Xs[1] = X[1];
	SW.Xs[2] = X[2];
	SW.calcX_start.notify(SC_ZERO_TIME);
	wait(SW.x_ready);
	X[0] = SW.Xs[0];
	X[1] = SW.Xs[1];
	X[2] = SW.Xs[2];
	SW.x_used.notify(SC_ZERO_TIME);
}
