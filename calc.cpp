#include <iostream>

#include "calc.hpp"

#define CAREER_PAD 400
#define FRONT_COEFF 0.95
#define PACE_COEFF 0.89
#define LATE_COEFF 1.0
#define END_COEFF 0.995

using namespace std;


int calcStamNeeded(Uma& uma, int trackLen, vector<double> recovery)
{
    array<int, 5> stats = uma.getStats();
    int sta = stats[1] * uma.getMoodMult();

    double hp = calcHP(uma, trackLen);
    double ehp = calcEHP(uma, hp, recovery);
    double styleCoeff = getStyleCoeff(uma.getStyle());

    
}

/**
 * @brief Caculates the effective stamina of the uma. This is the stamina value that
 * the skill recovery would amount to.
 * 
 * @param uma The uma object
 * @param trackLen The length of the track in meters
 * @param recovery A vector of the recovery skills
 * @return The calculated effective stamina of the uma
 */
int calcEffectiveStamina(Uma& uma, int trackLen, vector<double> recovery)
{
    array<int, 5> stats = uma.getStats();
    int sta = stats[1] * uma.getMoodMult();
    int style = uma.getStyle();

    double stratCoeff = getStyleCoeff(style);
    double hp = calcHP(uma, trackLen);
    double ehp = calcEHP(uma, hp, recovery);
    return sta + round((ehp - hp) / (0.8 * stratCoeff));
}

/**
 * @brief Calculates the effective HP of the uma. This is the hp it would have
 * considering the probabalistic hp recovered by recovery skills
 * 
 * @param uma The uma object
 * @param hp The HP of the uma on the given track
 * @param recovery A vector of the recovery skills
 * @return Total effective HP of the uma
 */
double calcEHP(Uma& uma, double hp, vector<double> recovery)
{
    array<int, 5> stats = uma.getStats();
    int sta = stats[1] * uma.getMoodMult();

    double procProb = calcSkillProb(uma);
    double totalRecovery = 0;

    for(auto rec: recovery)
    {
        totalRecovery += rec * procProb;
    }
    double ehp = hp + hp * totalRecovery;
    cout << "EHP: " << ehp << "\n";
    return  ehp;
}

/**
 * @brief Calculates the HP of the uma based on the track
 * 
 * @param uma The uma object
 * @param trackLen The length of the track in meters
 * @return Total HP of the uma 
 */
double calcHP(Uma& uma, int trackLen)
{
    double stratCoeff = getStyleCoeff(uma.getStyle());
    double hp = (0.8 * stratCoeff * uma.sta * uma.getMoodMult()) + trackLen;
    cout << "HP: " << hp << "\n";
    return hp;
}

/**
 * @brief Calculate the probability of a skill being used
 * 
 * @param uma The uma object
 * @return Percentage of a skill being used in decimal format
 */
double calcSkillProb(Uma& uma)
{
    array<int, 5> stats = uma.getStats();
    double effWit = stats[4] * uma.getMoodMult();
    char styleApt = uma.getCurrentStyleApt();
    switch (styleApt)
    {
    case 'S':
    case 's':
        effWit = effWit * 1.1;
        break;
    case 'B':
    case 'b':
        effWit = effWit * 0.85;
        break;
    case 'C':
    case 'c':
        effWit = effWit * 0.75;
        break;
    case 'D':
    case 'd':
        effWit = effWit * 0.6;
        break;
    case 'E':
    case 'e':
        effWit = effWit * 0.4;
        break;
    case 'F':
    case 'f':
        effWit = effWit * 0.2;
        break;
    case 'G':
    case 'g':
        effWit = effWit * 0.1;
        break;
    default:
        break;
    }
    double prob = 1 - (90 / effWit);
    return max(prob, 0.2);
}

/**
 * @brief Get the Style Coeff
 * 
 * @param style 
 * @return Coefficient of the running style 
 */
double getStyleCoeff(int style)
{
    double styleCoeff = -1.0;
    if(style == 0)
        styleCoeff = FRONT_COEFF;
    else if(style == 1)
        styleCoeff = PACE_COEFF;
    else if(style == 2)
        styleCoeff = LATE_COEFF;
    else if(style == 3)
        styleCoeff = END_COEFF;
    
    if(styleCoeff < 0) throw std::runtime_error("Invalid style argument");
    return styleCoeff;
}