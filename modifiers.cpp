#include <stdexcept>
#include <array>
#include <cctype>
#include "modifiers.hpp"

using namespace std;

/**
 * @brief Get the Style Coeff
 * 
 * @param style 
 * @return Coefficient of the running style 
 */
double getHPDrainCoeff(int style)
{
    if(style < 0 || style >= 4)
        throw out_of_range("Invalid style argument");

    static const array<double, 4> coeffs{
        {FRONT_HPDRAIN_COEFF, PACE_HPDRAIN_COEFF, LATE_HPDRAIN_COEFF, END_HPDRAIN_COEFF}
    };
    return coeffs[style];
}

double getStyleSpeedCoeff(int style, int phase)
{
    if(style < 0 || style >= 4)
        throw out_of_range("Invalid style argument");
    if(phase < 0 || phase >= 3)
        throw out_of_range("Invalid phase argument");

    static const array<array<double, 3>, 4> coeffs {{
        { FRONT_SPEED_EARLY_COEFF, FRONT_SPEED_MID_COEFF, FRONT_SPEED_LATE_COEFF },
        { PACE_SPEED_EARLY_COEFF,  PACE_SPEED_MID_COEFF,  PACE_SPEED_LATE_COEFF  },
        { LATE_SPEED_EARLY_COEFF,  LATE_SPEED_MID_COEFF,  LATE_SPEED_LATE_COEFF  },
        { END_SPEED_EARLY_COEFF,   END_SPEED_MID_COEFF,   END_SPEED_LATE_COEFF   }
    }};
    return coeffs[style][phase];
}


double getStyleAcccelCoeff(int style, int phase)
{
    if(style < 0 || style >= 4)
        throw out_of_range("Invalid style argument");
    if(phase < 0 || phase >= 3)
        throw out_of_range("Invalid phase argument");

    static const array<array<double, 3>, 4> coeffs {{
        { FRONT_ACCEL_EARLY_COEFF, FRONT_ACCEL_MID_COEFF, FRONT_ACCEL_LATE_COEFF },
        { PACE_ACCEL_EARLY_COEFF,  PACE_ACCEL_MID_COEFF,  PACE_ACCEL_LATE_COEFF  },
        { LATE_ACCEL_EARLY_COEFF,  LATE_ACCEL_MID_COEFF,  LATE_ACCEL_LATE_COEFF  },
        { END_ACCEL_EARLY_COEFF,   END_ACCEL_MID_COEFF,   END_ACCEL_LATE_COEFF   }
    }};
    return coeffs[style][phase];
}

double getStyleCoeff(char c)
{
    int cTrans = aptTranslation(c);
    if(cTrans < 0)
        throw out_of_range("Invalid aptitude argument");

    static const array<double, 8> coeffs{
        {STYLE_S_APT,STYLE_A_APT,STYLE_B_APT,STYLE_C_APT,
         STYLE_D_APT,STYLE_E_APT,STYLE_F_APT,STYLE_G_APT}
    };
    return coeffs[cTrans];
}


double getDistSpeedCoeff(char c)
{
    int cTrans = aptTranslation(c);
    if(cTrans < 0)
        throw out_of_range("Invalid aptitude argument");

    static const array<double, 8> coeffs{
        {DIST_S_APT_SPEED,DIST_A_APT_SPEED,DIST_B_APT_SPEED,DIST_C_APT_SPEED,
         DIST_D_APT_SPEED,DIST_E_APT_SPEED,DIST_F_APT_SPEED,DIST_G_APT_SPEED}
    };
    return coeffs[cTrans];
}

double getDistAcccelCoeff(char c)
{
    int cTrans = aptTranslation(c);
    if(cTrans < 0)
        throw out_of_range("Invalid aptitude argument");

    static const array<double, 8> coeffs{
        {DIST_S_APT_ACCEL,DIST_A_APT_ACCEL,DIST_B_APT_ACCEL,DIST_C_APT_ACCEL,
         DIST_D_APT_ACCEL,DIST_E_APT_ACCEL,DIST_F_APT_ACCEL,DIST_G_APT_ACCEL}
    };
    return coeffs[cTrans];
}

double getTurfCoeff(char c)
{
    int cTrans = aptTranslation(c);
    if(cTrans < 0)
        throw out_of_range("Invalid aptitude argument");

    static const array<double, 8> coeffs{
        {TURF_S_APT,TURF_A_APT,TURF_B_APT,TURF_C_APT,
         TURF_D_APT,TURF_E_APT,TURF_F_APT,TURF_G_APT}
    };
    return coeffs[cTrans];
}

int getSurfaceSpeedMod(int surface, int condition)
{
    if(surface < 0 || surface > 1)
        throw out_of_range("Invalid surface argument");
    if(condition < 0 || surface > 3)
        throw out_of_range("Invalid condition argument");
    
    static const array<array<int, 4>, 2> coeffs{{
        {TURF_FIRM_SPEED_MOD, TURF_GOOD_SPEED_MOD, TURF_SOFT_SPEED_MOD, TURF_HEAVY_SPEED_MOD},
        {DIRT_FIRM_SPEED_MOD, DIRT_GOOD_SPEED_MOD, DIRT_SOFT_SPEED_MOD, DIRT_HEAVY_SPEED_MOD}
    }};
    return coeffs[surface][condition];
}

int getSurfacePowerMod(int surface, int condition)
{
    if(surface < 0 || surface > 1)
        throw out_of_range("Invalid surface argument");
    if(condition < 0 || surface > 3)
        throw out_of_range("Invalid condition argument");
    
    static const array<array<int, 4>, 2> coeffs{{
        {TURF_FIRM_POWER_MOD, TURF_GOOD_POWER_MOD, TURF_SOFT_POWER_MOD, TURF_HEAVY_POWER_MOD},
        {DIRT_FIRM_POWER_MOD, DIRT_GOOD_POWER_MOD, DIRT_SOFT_POWER_MOD, DIRT_HEAVY_POWER_MOD}
    }};
    return coeffs[surface][condition];
}

double getSurfaceHPDrainMod(int surface, int condition)
{
    if(surface < 0 || surface > 1)
        throw out_of_range("Invalid surface argument");
    if(condition < 0 || surface > 3)
        throw out_of_range("Invalid condition argument");
    
    static const array<array<double, 4>, 2> coeffs{{
        {TURF_FIRM_HPDRAIN_MOD, TURF_GOOD_HPDRAIN_MOD, TURF_SOFT_HPDRAIN_MOD, TURF_HEAVY_HPDRAIN_MOD},
        {DIRT_FIRM_HPDRAIN_MOD, DIRT_GOOD_HPDRAIN_MOD, DIRT_SOFT_HPDRAIN_MOD, DIRT_HEAVY_HPDRAIN_MOD}
    }};
    return coeffs[surface][condition];
}

int getLenClass(int trackLen)
{
    if(trackLen >= LONG_LEN)
        return 3;
    if(trackLen >= MEDUIM_LEN)
        return 2;
    if(trackLen >= MILE_LEN)
        return 1;
    return 0;
}

int aptTranslation(char c)
{
    c = tolower(c);
    switch (c)
        {
        case 's':
            return 0;
        case 'a':
            return 1;
        case 'b':
            return 2;
        case 'c':
            return 3;
        case 'd':
            return 4;
        case 'e':
            return 5;
        case 'f':
            return 6;
        case 'g':
            return 7;
        default:
            return -1;
        }
}