#ifndef CALC
#define CALC

#include <vector>
#include <array>

#include "uma.hpp"
#include "track.hpp"


int calcStamNeeded(Uma& uma, int trackLen, vector<double> recovery);
int calcEffectiveStamina(Uma& uma, int trackLen, vector<double> recovery);
double calcEHP(Uma& uma, double hp, std::vector<double> recovery);
double calcHP(Uma& uma, int trackLen);
array<int, 2> calcReqStamina(Uma& uma, Track& track, vector<double> recovery);
array<double, 2> calcHPDrain(Uma& uma, Track& track);
array<array<double, 2>, 6> calcDurations(Uma& uma, Track& track);
array<array<double, 2>, 6> calcDurations(Uma& uma, Track& track, std::array<double, 5> targetSpeeds, std::array<double, 5> accels);
std::array<double, 5> calcTargetSpeed(Uma& uma, Track& track);
std::array<double, 5> calcAccel(Uma& uma, Track& track);
double calcBaseSpeed(int trackLen);
double calcSkillProb(Uma& uma);

#endif