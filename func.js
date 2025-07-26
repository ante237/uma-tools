import * as CalclibModule from './calclib.js';
const sizeof_umaStruct = 40; //Padding to align to 4 byte
const sizeof_trackStruct = 12;
const sizeof_double = 8;

export async function calculateStamina(uma, track, recovery)
{
    const Module = await CalclibModule.default();
    console.log("WASM module loaded");
    const reqStamina = Module.cwrap('reqStamina', 'number', ['number', 'number', 'number', 'number']);
    const umaPtr = Module._malloc(sizeof_umaStruct);
    let offset = 0;
    
    Module.HEAP32[(umaPtr + offset) >> 2] = uma.mood;  // mood
    offset += 4;
    Module.HEAP32[(umaPtr + offset) >> 2] = uma.style;  // style
    offset += 4;
    Module.HEAP32[(umaPtr + offset) >> 2] = uma.spd; // spd
    offset += 4;
    Module.HEAP32[(umaPtr + offset) >> 2] = uma.sta; // sta
    offset += 4;
    Module.HEAP32[(umaPtr + offset) >> 2] = uma.pow;  // pow
    offset += 4;
    Module.HEAP32[(umaPtr + offset) >> 2] = uma.gut;  // gut
    offset += 4;
    Module.HEAP32[(umaPtr + offset) >> 2] = uma.wit;  // wit
    offset += 4;
    
    const aptitudes = [uma.turf, uma.dirt,
        uma.front, uma.pace, uma.late, uma.end,
        uma.sprint, uma.mile, uma.medium, uma.long];
    for (let i = 0; i < 10; i++) 
    {
        Module.HEAPU8[umaPtr + offset + i] = aptitudes[i].charCodeAt(0);
    };
        
    offset = 0;
    const trackPtr = Module._malloc(sizeof_trackStruct);
    Module.HEAP32[(trackPtr + offset) >> 2] = track.surface;  // surface
    offset += 4;
    Module.HEAP32[(trackPtr + offset) >> 2] = track.condition;  // condition
    offset += 4;
    Module.HEAP32[(trackPtr + offset) >> 2] = track.length; // length
    offset += 4;
    
    const rec = recovery;
    const recLen = rec.length;
    const recPtr = Module._malloc(sizeof_double*recLen);
    for(let i = 0; i < recLen; i++)
    {
        Module.HEAPF64[(recPtr >> 3) + i] = rec[i];
    }
    
    const res = reqStamina(umaPtr, trackPtr, recPtr, recLen);
    
    Module._free(umaPtr);
    Module._free(trackPtr);
    Module._free(recPtr);
    
    const staminaOptimal = Module.HEAP32[(res >> 2) + 1];
    const staminaWorst = Module.HEAP32[res >> 2];

    console.log(staminaOptimal);
    
    return {staminaWorst, staminaOptimal}
}

export function distClassification(length)
{
    if(length < 1500) return 0;
    else if(length < 2000) return 1;
    else if(length < 2500) return 2;
    else return 3;
}

export function cmpCond(cond, value)
{
    const trgValue = cond.value;
    const op = cond.operator;
    switch(op)
    {
        case "==":
            return value == trgValue;
        case "<":
            return value < trgValue;
        case ">":
            return value > trgValue;
        case "<=":
            return value <= trgValue;
        case ">=":
            return value >= trgValue;
        case "!=":
            return value != trgValue;
        default:
            return false;
    }
}