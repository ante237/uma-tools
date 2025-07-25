#include <array>

#include "calc.hpp"
#include "uma.hpp"
#include "track.hpp"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

extern "C" {  
    
    
    int* reqStamina(umaStruct* uma, trackStruct* track, double* recArr, int arrLen)
    {
        Uma curUma(*uma);
        Track curTrack(*track);

        std::vector<double> vec(recArr, recArr + arrLen);

        static array<int, 2> arr = calcReqStamina(curUma, curTrack, vec);

        return arr.data();
    }

    
}
#endif
