#include<iostream>

#include "GameMap.h" 
#include "Population.h"
#include "GameRule.h" 

extern vector<Individual> nowpopulation;
extern vector<Individual> midpopulation;
//extern vector<vector<Unit>> unit1_2dmap;//单位二维容器
//extern vector<Unit> unit1;//我方单位容器
//extern vector<Unit> unit2;//敌方单位容器

int main()
{
	int i = 0;
	int j = 0;
	int k = 0;
	
	Initialize();
	Cacula_Fitness();
	Cacula_FitnessProba();
	nowpopulation.at(0).printout(nowpopulation.begin(), nowpopulation.end());
	double best = GetBestFitness();
	double sum = GetSumFitness();
	cout << 0 << " sum:" << left << setw(5) << sum << " best:" << left << setw(5) << best << endl;
	
	for (i = 0; i < 200; i++) {
		plan_learning();
		Cacula_Fitness();
		Cacula_FitnessProba();
		double best = GetBestFitness();
		double sum = GetSumFitness();
		Save_HQ_Indivi();
		cout << i << " Finish save HQ individuals." << endl;
	}
	Save_HQ_Unit();
	
	cout << "Finish learning." << endl;
	Save_HQ_Indivi();
	Update_HQ_Lib();
	cout << " Finish update HQ Lib." << endl;

	for (i = 0; i < 200; i++) {
		Save_HQ_Indivi();
		cout << "Finish save HQ individuals." << endl;
		Update_HQ_Lib();
		cout << " Finish update HQ Lib." << endl;
		
		select();
		Cacula_Fitness();
		Cacula_FitnessProba();
		double best = GetBestFitness();
		double sum = GetSumFitness();
		cout << i << " sum:" << left << setw(5) << sum << " best:" << left << setw(5) << best << endl;
	}

	pp();
	nowpopulation.at(0).printout(nowpopulation.begin(), nowpopulation.end());
}

