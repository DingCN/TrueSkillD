#ifndef  TRUESKILLD_INC
#define  TRUESKILLD_INC
#include <vector>
#include "Variable.h"

// Default initial mean of ratings.
const double MU = 25.0;
// Default initial standard deviation of ratings.
const double SIGMA = MU / 3;
// Default distance that guarantees about 76% chance of winning.
const double BETA = SIGMA / 2;
// Default dynamic factor.
const double TAU = SIGMA / 100;
// Default draw probability of the game.
const double DRAW_PROBABILITY = 0.10;
// A basis to check reliability of the result.
const double DELTA = 0.000001;

class Variable;

struct RatingGroups {
	std::vector<Variable> vars; // players in the group
	std::vector<double> weights; // weight for 'partial play'
	int rank;
	int pos; // original position of a group before sorted by rank 
};
struct RatingValue {
	double mu;
	double pi;
	double sigma;
	double tau;
};

struct sort_group
{
	inline bool operator() (const RatingGroups& group1, const RatingGroups& group2)
	{
		return (group1.rank < group2.rank);
	}
};

struct unsort_group
{
	inline bool operator() (const RatingGroups& group1, const RatingGroups& group2)
	{
		return (group1.pos < group2.pos);
	}
};

RatingValue GetVariableValues(const Variable & variable);


#endif