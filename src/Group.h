#include "systemc.h"
#include "Particle.h"

static double calcF(double x, double y, double z);
static bool compare(double newValue, double oldValue);
static void calcV(double random, double pBest[3], double X[3], double gBest[3], double* V);
static void calcX(double* X, double V[3]);

SC_MODULE(group) {
	const int n = 10, numOfIterations = 10;
	particle particles[10];
	double gBestValue, gBestPosition[3];

	void prc_group() {
		double newBest, random;
		bool newBestLower;
		cout << "number of particles = " << n << endl;
		cout << "number of iterations = " << numOfIterations << endl;
		for (int i = 0; i < numOfIterations; i++) {
			cout << "group best " << gBestValue << endl;
			for (int j = 0; j < n; j++) {
				if (compare(particles[j].pBestValue, gBestValue)) {
					gBestPosition[0] = particles[j].pBestPosition[0];
					gBestPosition[1] = particles[j].pBestPosition[1];
					gBestPosition[2] = particles[j].pBestPosition[2];
					gBestValue = particles[j].pBestValue;
				}
			}
			random = (double)(rand() % 100) / 100;
			for (int j = 0; j < n; j++) {
				calcV(random, particles[j].pBestPosition, particles[j].X, gBestPosition, particles[j].V);
				calcX(particles[j].X, particles[j].V);
				newBest = calcF(particles[j].X[0], particles[j].X[1], particles[j].X[2]);
				newBestLower = compare(newBest, particles[j].pBestValue);
				if (newBestLower) {
					particles[j].pBestPosition[0] = particles[j].X[0];
					particles[j].pBestPosition[1] = particles[j].X[1];
					particles[j].pBestPosition[2] = particles[j].X[2];
					particles[j].pBestValue = newBest;
				}
			}
		}
	}

	SC_CTOR(group) {
		for (int i = 0; i < n; i++) {
			particles[i].init();
		}
		gBestValue = particles[0].pBestValue;
		gBestPosition[0] = particles[0].pBestPosition[0];
		gBestPosition[1] = particles[0].pBestPosition[1];
		gBestPosition[2] = particles[0].pBestPosition[2];
		SC_THREAD(prc_group);
	}
};
