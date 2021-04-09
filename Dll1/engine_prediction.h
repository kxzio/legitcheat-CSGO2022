#pragma once
enum stage_t
{
	start,
	work,
	finish
};

class c_engine_prediction
{
private:
	struct prediction_data_t
	{
		float curtime, frametime;
		int stage;
		int* prediction_random_seed = nullptr;
		int* prediction_player = nullptr;
	};
public:
	prediction_data_t prediction_data;

	void init();
	void start();
	void finish();
};

extern c_engine_prediction* engine_prediction;