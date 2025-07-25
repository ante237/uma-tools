#ifndef UMA
#define UMA
#include <cctype>
#include <array>

using namespace std;

struct umaStruct {
        int mood;
        int style;
        int spd;
        int sta;
        int pow;
        int gut;
        int wit;
        char turfApt;
        char dirtApt;
        char sprintApt;
        char mileApt;
        char mediumApt;
        char longApt;
        char frontApt;
        char paceApt;
        char lateApt;
        char endApt;
    };

class Uma
{
public:
    Uma(int mood, int style, int spd, int sta, int pow, int gut, int wit,
    char turfApt, char dirtApt,
    char sprintApt, char mileApt, char mediumApt, char longApt, 
    char frontApt, char paceApt, char lateApt, char endApt)
    :
    mood(mood), style(style), spd(spd), sta(sta), pow(pow), gut(gut), wit(wit),
    turfApt(turfApt), dirtApt(dirtApt),
    sprintApt(sprintApt), mileApt(mileApt), mediumApt(mediumApt), longApt(longApt),
    frontApt(frontApt), paceApt(paceApt), lateApt(lateApt), endApt(endApt)
    {}

    Uma(umaStruct& umaS)
    :
    mood(umaS.mood), style(umaS.style), 
    spd(umaS.spd), sta(umaS.sta), pow(umaS.pow), gut(umaS.gut), wit(umaS.wit),
    turfApt(umaS.turfApt), dirtApt(umaS.dirtApt),
    sprintApt(umaS.sprintApt), mileApt(umaS.mileApt), mediumApt(umaS.mediumApt), longApt(umaS.longApt),
    frontApt(umaS.frontApt), paceApt(umaS.paceApt), lateApt(umaS.lateApt), endApt(umaS.endApt)
    {}

    ~Uma() = default;

    void setStats(int spd = -1, int sta = -1, int pow = -1, int gut = -1, int wit = -1)
    {
        if(spd != -1) this->spd = spd;
        if(sta != -1) this->sta = sta;
        if(pow != -1) this->pow = pow;
        if(gut != -1) this->gut = gut;
        if(wit != -1) this->wit = wit;
    }

    /**
     * @brief Get the Stats of the uma, modified by mood
     * 
     * @return int* with indexation
     * 0: spd,
     * 1: sta,
     * 2: pow,
     * 3: gut,
     * 4: wit
     */
    array<int, 5> getStats()
    {
        double mod = getMoodMult();
        return {(int)(this->spd * mod), 
            (int)(this->sta * mod), 
            (int)(this->pow * mod), 
            (int)(this->gut * mod), 
            (int)(this->wit * mod)};
    }

    /**
     * @brief Set the Mood of the uma
     * 
     * @param mood number between 0-4, the lower the better the mood
     */
    void setMood(int mood)
    {
        if(mood >= 0 && mood <= 4) this->mood = mood;
    }

    int getMood()
    {
        return this->mood;
    }

    double getMoodMult()
    {
        return 0.96 + this->mood * 0.02;
    }

    void setStyle(int style)
    {
        if(style >= 0 && style <= 3) this->style = style;
    }

    int getStyle()
    {
        return this->style;
    }

    void setSurfaceApt(char turf, char dirt)
    {
        if(checkValidApt(turf)) this->turfApt = turf;
        if(checkValidApt(dirt)) this->dirtApt = dirt;
    }
    
    array<char, 2> getSurfaceApt()
    {
        return {this->turfApt, this->dirtApt};
    }

    void setDistApt(char sprint, char mile, char medium, char lng)
    {
        if(checkValidApt(sprint)) this->sprintApt = sprint;
        if(checkValidApt(mile)) this->mileApt = mile;
        if(checkValidApt(medium)) this->mediumApt = medium;
        if(checkValidApt(lng)) this->longApt = lng;
        
    }
    
    array<char, 4> getDistApt()
    {
        return {this->sprintApt, this->mileApt, this->mediumApt, this->longApt};
    }

    void setStyleApt(char front, char pace, char late, char end)
    {
        if(checkValidApt(front)) this->frontApt = front;
        if(checkValidApt(pace)) this->paceApt = pace;
        if(checkValidApt(late)) this->lateApt = late;
        if(checkValidApt(end)) this->endApt = end;
    }
    
    array<char, 4> getStyleApt()
    {
        return {this->frontApt, this->paceApt, this->lateApt, this->endApt};
    }

    char getCurrentStyleApt()
    {
        switch (this->style)
        {
        case 0:
            return this->frontApt;
        case 1:
            return this->paceApt;
        case 2:
            return this->lateApt;
        case 3:
            return this->endApt;
        }
        return ' ';
    }

    /*The umas mood
    great: 4,
    good: 3,
    normal: 2,
    bad: 1,
    awful: 0 */
    int mood;
    /*The running style of the uma
    front runner: 0,
    pace chaser: 1,
    late surger: 2,
    end closer: 3*/
    int style;
    int spd;
    int sta;
    int pow;
    int gut;
    int wit;
    char turfApt;
    char dirtApt;
    char sprintApt;
    char mileApt;
    char mediumApt;
    char longApt;
    char frontApt;
    char paceApt;
    char lateApt;
    char endApt;

    private:
    bool checkValidApt(char c)
    {
        c = tolower(c);
        switch (c)
        {
        case 's':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
            return true;
        default:
            return false;
        }
    }

};

#endif