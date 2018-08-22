#ifndef  LAYERS_INC
#define  LAYERS_INC

#include "Variable.h"
#include"Factor.h"
#include "PriorFactor.h"
#include"LikelihoodFactor.h"
#include "SumFactor.h"
#include "TruncateFactor.h"
#include <vector>
#include "Math.h"

class Layers
{
public:
	Layers(size_t RatingFactorListSize, size_t PerfFactorListSize, size_t TeamPerfFactorListSize, size_t TeamDiffFactorListSize, size_t TruncFactorListSize);
	~Layers();

	int build_rating_layer(std::vector<Variable*> rating_vars, std::vector<RatingValue> & flatten_ratings);
	int build_perf_layer(std::vector<Variable*> rating_vars, std::vector<Variable*> perf_vars);
	int build_team_perf_layer(std::vector<Variable*> perf_vars, std::vector<Variable*> team_perf_vars, std::vector<double> flatten_weights, std::vector<int> team_sizes);
	int build_team_diff_layer(std::vector<Variable*> team_diff_vars, std::vector<Variable*> team_perf_vars);
	int build_trunc_layer(std::vector<Variable*> team_diff_vars, std::vector<RatingGroups> & rating_groups);

	size_t get_RatingFactorListSize();
	size_t get_PerfFactorListSize();
	size_t get_TeamPerfFactorListSize();
	size_t get_TeamDiffFactorListSize();
	size_t get_TruncFactorListSize();

	double calc_draw_probability(double draw_margin, int size);
	double calc_draw_margin(double draw_probability, int size);

	std::vector<PriorFactor> RatingFactorList;
	std::vector<LikelihoodFactor> PerfFactorList;
	std::vector<SumFactor> TeamPerfFactorList;
	std::vector<SumFactor> TeamDiffFactorList;
	std::vector<TruncateFactor> TruncFactorList;

	Math math = Math();
private:

};
#endif
