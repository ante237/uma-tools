
all:
	g++ -g -w -O0 -oout main.cpp calc.cpp uma.hpp track.hpp modifiers.cpp -std=c++14

wasm:
	emcc calclib.cpp calc.cpp modifiers.cpp -o calclib.js \
		-sEXPORTED_FUNCTIONS="['_reqStamina','_malloc','_free']" \
		-sEXPORTED_RUNTIME_METHODS="['ccall','cwrap','setValue','getValue', 'HEAP32', 'HEAPU8', 'HEAPF64']" \
		-sMODULARIZE=1 \
		-sEXPORT_ES6=1 \
		-sALLOW_MEMORY_GROWTH=1