class particle {
public:
	double X[3];
	double V[3];
	double pBestValue, pBestPosition[3];

	void init() {
		X[0] = (rand() % 61) - 30;
		X[1] = (rand() % 61) - 30;
		X[2] = (rand() % 61) - 30;
		V[0] = (double)((rand() % 10) + 1) / 10;
		V[1] = (double)((rand() % 10) + 1) / 10;
		V[2] = (double)((rand() % 10) + 1) / 10;
		pBestPosition[0] = X[0];
		pBestPosition[1] = X[1];
		pBestPosition[2] = X[2];
		pBestValue = X[0] * X[0] + X[1] * X[1] + X[2] * X[2];
	}
};
