#include "stdafx.h"
#include "Layers.h"
#include "TrueSkillD.h"
#include "Math.h"
Layers::Layers(size_t RatingFactorListSize, size_t PerfFactorListSize, size_t TeamPerfFactorListSize, size_t TeamDiffFactorListSize, size_t TruncFactorListSize)
{
	this->RatingFactorList.resize(RatingFactorListSize);
	this->PerfFactorList.resize(PerfFactorListSize);
	this->TeamPerfFactorList.resize(TeamPerfFactorListSize);
	this->TeamDiffFactorList.resize(TeamDiffFactorListSize);
	this->TruncFactorList.resize(TruncFactorListSize);
}

Layers::~Layers()
{
}


int Layers::build_rating_layer(std::vector<Variable*> pRating_vars, std::vector<RatingValue> & flatten_ratings) {
	for (size_t i = 0; i < pRating_vars.size(); i++)
	{
		std::vector<Variable*> pVars;
		pVars.push_back(pRating_vars[i]);
		this->RatingFactorList[i] = PriorFactor(pVars, flatten_ratings[i], TAU);// class TrueSkill's self.tau
		this->RatingFactorList[i].ConstructMessageMap(pVars);
	}
	return 0;
}

int Layers::build_perf_layer(std::vector<Variable*> pRating_vars, std::vector<Variable*> pPerf_vars) {
	for (size_t i = 0; i < pRating_vars.size(); i++) // rating_vars.size() == perf_vars.size()
	{
		std::vector<Variable*> pVars;
		pVars.push_back(pRating_vars[i]);
		pVars.push_back(pPerf_vars[i]);
		this->PerfFactorList[i] = LikelihoodFactor(pRating_vars[i], pPerf_vars[i], BETA * BETA, pVars);
		this->PerfFactorList[i].ConstructMessageMap(pVars);
	}
	return 0;
}

int Layers::build_team_perf_layer(std::vector<Variable*> pPerf_vars, std::vector<Variable*> pTeam_perf_vars, std::vector<double> flatten_weights, std::vector<int> team_sizes) {
	std::vector<Variable*> pVars;
	for (size_t i = 0; i < pTeam_perf_vars.size(); i++)
	{
		int start;
		if (i > 0) {
			start = team_sizes[i - 1];
		}
		else {
			start = 0;
		}
		int end = team_sizes[i];
		std::vector<Variable*> pChild_perf_vars;
		std::vector<double> coeffs;
		for (int j = start; j < end; j++) {
			pChild_perf_vars.push_back(pPerf_vars[j]);
			coeffs.push_back(flatten_weights[j]);
		}
		pVars = pChild_perf_vars;
		pVars.push_back(pTeam_perf_vars[i]); // all variables connected with this factor
		this->TeamPerfFactorList[i] = SumFactor(pTeam_perf_vars[i], pChild_perf_vars, pVars, coeffs);
		this->TeamPerfFactorList[i].ConstructMessageMap(pVars);
	}

	return 0;
}

int Layers::build_team_diff_layer(std::vector<Variable*> pTeam_diff_vars, std::vector<Variable*> pTeam_perf_vars) {
	for (size_t team = 0; team < pTeam_diff_vars.size(); team++)
	{
		std::vector<Variable*> pSub_team_perf_vars;
		pSub_team_perf_vars.push_back(pTeam_perf_vars[team]);
		pSub_team_perf_vars.push_back(pTeam_perf_vars[team + 1]);
		std::vector<double> coeffs;/////////////////////////////////////////////////////
		coeffs.push_back(1);
		coeffs.push_back(-1);
		std::vector<Variable*> pVars = pSub_team_perf_vars;
		pVars.push_back(pTeam_diff_vars[team]); // all variables connected with this factor
		this->TeamDiffFactorList[team] = SumFactor(pTeam_diff_vars[team], pSub_team_perf_vars, pVars, coeffs);
		this->TeamDiffFactorList[team].ConstructMessageMap(pVars);
	}
	return 0;
}


double Layers::calc_draw_probability(double draw_margin, int size) {
	return (2 * math.math_cdf(draw_margin / (sqrt(size) * BETA)) - 1);
}
double Layers::calc_draw_margin(double draw_probability, int size)
{
	return this->math.math_ppf((draw_probability + 1) / 2) * sqrt(size) * BETA;
}

int Layers::build_trunc_layer(std::vector<Variable*> pTeam_diff_vars, std::vector<RatingGroups> & rating_groups) {
	for (size_t i = 0; i < pTeam_diff_vars.size(); i++) {
		//modify here to use a dynamic draw probability function
		double draw_probability = DRAW_PROBABILITY; //static draw probability

		int size = rating_groups[i].vars.size();
		size += rating_groups[i + 1].vars.size();

		double draw_margin = calc_draw_margin(draw_probability, size);

		int match_result; // draw = 0; win = 1;
		if (rating_groups[i].rank == rating_groups[i + 1].rank) {// draw
			match_result = 0;
		}
		else { // win
			match_result = 1;
		}
		std::vector<Variable*> pVars;
		pVars.push_back(pTeam_diff_vars[i]);
		this->TruncFactorList[i] = TruncateFactor(pVars, match_result, draw_margin);
		this->TruncFactorList[i].ConstructMessageMap(pVars);
	}
	return 0;
}

size_t Layers::get_RatingFactorListSize() {
	return this->RatingFactorList.size();
}
size_t Layers::get_PerfFactorListSize() {
	return this->PerfFactorList.size();
}
size_t Layers::get_TeamPerfFactorListSize() {
	return this->TeamPerfFactorList.size();
}
size_t Layers::get_TeamDiffFactorListSize() {
	return this->TeamDiffFactorList.size();
}
size_t Layers::get_TruncFactorListSize() {
	return this->TruncFactorList.size();
}