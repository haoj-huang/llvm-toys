
int SCRAM();
int turnReactorOn();

void test_loop(int wrongTemperature, int restart) {
	turnReactorOn();
	if (wrongTemperature)
		SCRAM();
	if (restart)
		SCRAM();
	turnReactorOn();
	SCRAM();
}

