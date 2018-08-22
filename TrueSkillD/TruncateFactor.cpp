#include "stdafx.h"
#include "TruncateFactor.h"
#include "TrueSkillD.h"
#include <iostream>
#include "Math.h"
TruncateFactor::TruncateFactor(std::vector<Variable*> pVars, int match_result, double draw_margin) : Factor(pVars)
{
	this->match_result = match_result;
	this->draw_margin = draw_margin;
}
TruncateFactor::TruncateFactor()
{
}
TruncateFactor::~TruncateFactor()
{
}

double TruncateFactor::v_win(double diff, double draw_margin)
{
	double x = diff - draw_margin;
	double denom = math.math_cdf(x);
	if (denom) {
		return math.math_pdf(x) / denom;
	}
	else {
		return -x;
	}
}

double TruncateFactor::v_draw(double diff, double draw_margin)
{
	double a = draw_margin - abs(diff);
	double b = -draw_margin - abs(diff);

	double denom = math.math_cdf(a) - math.math_cdf(b);
	double numer = math.math_pdf(b) - math.math_pdf(a);
	double x, y;
	if (denom) {
		return (numer / denom);
	}
	else {
		return a;
	}
	if (diff < 0) {
		y = -1;
	}
	else {
		y = 1;
	}
	return x * y;
}

double TruncateFactor::w_win(double diff, double draw_margin)
{
	double x = diff - draw_margin;
	double v = v_win(diff, draw_margin);
	double w = v * (v + x);
	if (w > 0 && w < 1) {
		return w;
	}
	else {
		std::cout << "ERROR";
		return -1;
	}
}

double TruncateFactor::w_draw(double diff, double draw_margin)
{
	double a = draw_margin - abs(diff);
	double b = -draw_margin - abs(diff);
	double denom = math.math_cdf(a) - math.math_cdf(b);
	if (!denom) {
		std::cout << "ERROR" << std::endl;
	}
	double v = v_draw(abs(diff), draw_margin);
	return (v*v) + (a * math.math_pdf(a) - b * math.math_pdf(b)) / denom;
}

double TruncateFactor::up() {
	Variable* val = this->pVars[0];
	RatingValue div;
	if (this->pVars[0]->m_Message.find(this) != this->pVars[0]->m_Message.end()) {
		RatingValue msg = this->pVars[0]->m_Message.find(this)->second;
		div = GetVariableValues(Variable(val->get_pi() - msg.pi, val->get_tau() - msg.tau));
	}
	else {
		div = GetVariableValues(Variable(val->get_pi(), val->get_tau()));
	}
	double sqrt_pi = sqrt(div.pi);
	double v, w;
	if (match_result == 0) { // draw
		v = v_draw(div.tau / sqrt_pi, draw_margin * sqrt_pi);
		w = w_draw(div.tau / sqrt_pi, draw_margin * sqrt_pi);
	}
	else { // win
		v = v_win(div.tau / sqrt_pi, draw_margin * sqrt_pi);
		w = w_win(div.tau / sqrt_pi, draw_margin * sqrt_pi);
	}
	double pi = div.pi / (1 - w);
	double tau = (div.tau + sqrt_pi * v) / (1 - w);
	double ret = (val->update_value(this, pi, tau));
	return ret;
}