// TrueSkillD.cpp: 定义控制台应用程序的入口点。
//

#include "Math.h"
#include "stdafx.h"
#include <cmath>
#include"Variable.h"
#include"Factor.h"
#include "PriorFactor.h"
#include"LikelihoodFactor.h"
#include "SumFactor.h"
#include "TruncateFactor.h"
#include "Layers.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "TrueSkillD.h"

RatingValue GetVariableValues(const Variable & variable) {
	RatingValue ret;
	ret.mu = variable.get_mu();
	ret.pi = variable.get_pi();
	ret.sigma = variable.get_sigma();
	ret.tau = variable.get_tau();
	return ret;
}

int factor_graph_builder(Layers *layers, std::vector<RatingGroups> & rating_groups, std::vector<RatingValue> flatten_ratings, std::vector<double> flatten_weights, Variable* Rating_vars, Variable* Perf_vars, Variable* TeamPerf_vars, Variable* TeamDiff_vars)
{

	size_t groupsCount = rating_groups.size();

	size_t size = flatten_ratings.size();

	std::vector<Variable*> pRating_vars;
	std::vector<Variable*> pPerf_vars;
	std::vector<Variable*> pTeamPerf_vars;
	std::vector<Variable*> pTeamDiff_vars;


	for (size_t i = 0; i < size; i++) {
		pRating_vars.push_back(&Rating_vars[i]);
		pPerf_vars.push_back(&Perf_vars[i]);
	}

	for (size_t i = 0; i < groupsCount; i++) {
		pTeamPerf_vars.push_back(&TeamPerf_vars[i]);
	}

	for (size_t i = 0; i < groupsCount - 1; i++) {
		pTeamDiff_vars.push_back(&TeamDiff_vars[i]);
	}

	//team_sizes = _team_sizes(rating_groups); // [3,6]
	std::vector<int> team_sizes;
	for (size_t i = 0; i < groupsCount; i++) {
		if (i == 0) {
			team_sizes.push_back(rating_groups[i].vars.size());
		}
		else {
			team_sizes.push_back(team_sizes[i - 1] + rating_groups[i].vars.size());
		}
	}

	layers->build_rating_layer(pRating_vars, flatten_ratings);
	layers->build_perf_layer(pRating_vars, pPerf_vars);
	layers->build_team_perf_layer(pPerf_vars, pTeamPerf_vars, flatten_weights, team_sizes);
	layers->build_team_diff_layer(pTeamDiff_vars, pTeamPerf_vars);
	layers->build_trunc_layer(pTeamDiff_vars, rating_groups);
	return 0;
}

int run_schedule(Layers* layers, double min_delta, Variable* Rating_vars, Variable* Perf_vars, Variable* TeamPerf_vars, Variable* TeamDiff_vars) {
	if (min_delta <= 0) {
		std::cout << "min_delta must be greater than 0";
	}
	//# gray arrows
	for (size_t i = 0; i < layers->get_RatingFactorListSize(); i++) {
		layers->RatingFactorList[i].down();
	}
	for (size_t i = 0; i < layers->get_PerfFactorListSize(); i++) {
		layers->PerfFactorList[i].down();
	}
	for (size_t i = 0; i < layers->get_TeamPerfFactorListSize(); i++) {
		layers->TeamPerfFactorList[i].down();
	}

	//# arrow #1, #2, #3
	for (int x = 0; x < 10; x++) {
		double delta;
		if (layers->get_TeamDiffFactorListSize() == 1) {
			// only two teams
			layers->TeamDiffFactorList[0].down();
			delta = layers->TruncFactorList[0].up();
		}
		else {
			// multiple teams
			delta = 0;

			for (size_t y = 0; y < layers->get_TeamDiffFactorListSize() - 1; y++) {
				layers->TeamDiffFactorList[y].down();
				delta = std::max(delta, layers->TruncFactorList[y].up());
				layers->TeamDiffFactorList[y].up(1);// up to right variable
			}

			for (size_t y = layers->TeamDiffFactorList.size() - 1; y > 0; y--) {
				layers->TeamDiffFactorList[y].down();
				delta = std::max(delta, layers->TruncFactorList[y].up());
				layers->TeamDiffFactorList[y].up(0);// up to left variable
			}
		}
		//# repeat until update is small enough
		if (delta <= min_delta) {
			break;
		}
	}

	// up both ends
	layers->TeamDiffFactorList[0].up(0);
	layers->TeamDiffFactorList[layers->TeamDiffFactorList.size() - 1].up(1);
	// up the remainder of the black arrows
	for (size_t i = 0; i < layers->get_TeamPerfFactorListSize(); i++)
	{
		for (size_t j = 0; j < layers->TeamPerfFactorList[i].pVars.size() - 1; j++)
		{
			layers->TeamPerfFactorList[i].up(j);
		}
	}
	for (size_t i = 0; i < layers->get_PerfFactorListSize(); i++)
	{
		layers->PerfFactorList[i].up();
	}
	return 0;
}

std::vector<RatingGroups> rate(std::vector<RatingGroups> & rating_groups, double min_delta = DELTA) {

	if (rating_groups.size() <= 1) {
		std::cout << "need at least two groups to rate";
	}

	std::sort(rating_groups.begin(), rating_groups.end(), sort_group());

	//weights should be at least greater than 0
	for (size_t i = 0; i < rating_groups.size(); i++) {
		for (size_t j = 0; j < rating_groups[i].weights.size(); j++)
		{
			if (min_delta > rating_groups[i].weights[j])
				rating_groups[i].weights[j] = min_delta;
		}
	}

	std::vector<RatingValue> flatten_ratings;
	std::vector<double> flatten_weights;
	if (flatten_weights.size() != flatten_ratings.size()) {
		std::cout << "every player must be assigned one and only one weight." << std::endl;
	}

	size_t groupsCount = rating_groups.size();
	for (size_t i = 0; i < groupsCount; i++) {
		for (size_t j = 0; j < rating_groups[i].vars.size(); j++) {
			RatingValue val = GetVariableValues(rating_groups[i].vars[j]);
			flatten_ratings.push_back(val);
			double weights = rating_groups[i].weights[j];
			flatten_weights.push_back(weights);
		}
	}
	if (flatten_ratings.size() != flatten_weights.size()) {
		std::cout << "ERROR" << std::endl;
	}

	size_t size = flatten_ratings.size();


	Variable* Rating_vars = new Variable[size];
	Variable* Perf_vars = new Variable[size];
	Variable* TeamPerf_vars = new Variable[groupsCount];
	Variable* TeamDiff_vars = new Variable[groupsCount - 1];
	Layers *layers = new Layers(size, size, groupsCount, groupsCount - 1, groupsCount - 1);

	// build factor graph
	factor_graph_builder(layers, rating_groups, flatten_ratings, flatten_weights, Rating_vars, Perf_vars, TeamPerf_vars, TeamDiff_vars);

	run_schedule(layers, min_delta, Rating_vars, Perf_vars, TeamPerf_vars, TeamDiff_vars);

	//moving results from layers to rating_groups
	int index = 0;
	for (size_t i = 0; i < rating_groups.size(); i++) {
		for (size_t j = 0; j < rating_groups[i].vars.size(); j++) {
			rating_groups[i].vars[j] = Rating_vars[index];
			index++;
		}
	}
	//restoring groups to original order
	std::sort(rating_groups.begin(), rating_groups.end(), unsort_group());

	delete[] Rating_vars;
	delete[] Perf_vars;
	delete[] TeamPerf_vars;
	delete[] TeamDiff_vars;
	delete layers;
	return rating_groups;
}

Variable Rating(double mu = MU, double sigma = SIGMA)
{
	double pi = 1 / (sigma*sigma);
	double tau = pi * mu;
	return Variable(pi, tau);
}

int main()
{

	Variable r1 = Rating(26); // player 1
	Variable r2 = Rating(27); // player 2
	Variable r3 = Rating(30); // player 3

	Variable r4 = Rating(24); // player 4
	Variable r5 = Rating(23); // player 5
	Variable r6 = Rating(22); // player 6

	std::vector<double> weights; // 玩家的权重设置, used for 'partial play'
	weights.push_back(1);
	weights.push_back(1);
	weights.push_back(1);
	RatingGroups a;
	a.vars.push_back(r1);
	a.vars.push_back(r2);
	a.vars.push_back(r3);
	a.weights = weights;
	a.pos = 0;
	a.rank = 0;
	RatingGroups b;
	b.vars.push_back(r4);
	b.vars.push_back(r5);
	b.vars.push_back(r6);
	b.weights = weights;
	b.pos = 1;
	b.rank = 1;

	std::vector<RatingGroups> rating_groups;
	rating_groups.push_back(a);
	rating_groups.push_back(b);

	//std::vector<double> weights;
	//weights.push_back(1);
	//weights.push_back(1);
	//weights.push_back(1);
	//std::vector<double> partial_weights;
	//partial_weights.push_back(0.1);
	//partial_weights.push_back(0.5);
	//partial_weights.push_back(0.4);
	//RatingGroups a;
	//a.vars.push_back(r1);
	//a.weights = weights;
	//a.pos = 0;
	//a.rank = 0;
	//RatingGroups b;
	//b.vars.push_back(r2);
	//b.vars.push_back(r3);
	//b.weights = weights;
	//b.pos = 1;
	//b.rank = 1;
	//RatingGroups c;
	//c.vars.push_back(r4);
	//c.vars.push_back(r5);
	//c.vars.push_back(r6);
	//c.weights = weights;
	//c.pos = 2;
	//c.rank = 2;

	//std::vector<RatingGroups> rating_groups;
	//rating_groups.push_back(a);
	//rating_groups.push_back(b);
	//rating_groups.push_back(c);

	size_t groupsCount = rating_groups.size();
	for (size_t i = 0; i < groupsCount; i++) {
		for (size_t j = 0; j < rating_groups[i].vars.size(); j++) {
			RatingValue val = GetVariableValues(rating_groups[i].vars[j]);
			std::cout << val.mu << ',' << val.sigma << ',' << val.pi << ',' << val.tau << std::endl;
		}
	}
	std::cout << '\n';

	std::vector<RatingGroups> result = rate(rating_groups);

	groupsCount = result.size();
	for (size_t i = 0; i < groupsCount; i++) {
		for (size_t j = 0; j < rating_groups[i].vars.size(); j++) {
			RatingValue val = GetVariableValues(rating_groups[i].vars[j]);
			std::cout << val.mu << ',' << val.sigma << ',' << val.pi << ',' << val.tau << std::endl;
		}
	}
	return 0;
}