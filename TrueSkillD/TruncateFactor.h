#ifndef  TRUNCATEFACTOR_INC
#define  TRUNCATEFACTOR_INC

#include "Factor.h"
#include <vector>
#include "Math.h"
class TruncateFactor : public Factor
{
public:
	TruncateFactor(std::vector<Variable*> pVars, int match_result, double draw_margin);
	TruncateFactor();
	~TruncateFactor();
	double down() { return 0.0; };
	double up();
	Math math = Math();

	double v_win(double diff, double draw_margin);
	double v_draw(double diff, double draw_margin);
	double w_win(double diff, double draw_margin);
	double w_draw(double diff, double draw_margin);

private:
	int match_result;
	double draw_margin;
};
#endif
