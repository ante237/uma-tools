#ifndef CALC
#define CALC

#include <math.h>
#include <stdexcept>
#include <vector>

#include "uma.hpp"

using namespace std;

int calcStamNeeded(Uma& uma, int trackLen, vector<double> recovery);
int calcEffectiveStamina(Uma& uma, int trackLen, vector<double> recovery);
double calcEHP(Uma& uma, double hp, vector<double> recovery);
double calcHP(Uma& uma, int trackLen);
double calcSkillProb(Uma& uma);
double getStyleCoeff(int style);

#endif