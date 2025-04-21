#include "src/fear/arena.c"
#include "src/fear/array.c"
#include "src/fear/logger.c"
#include "src/fear/mem.c"
#include "src/fear/heap.c"
#ifdef FEAR_STL_ENABLED
#include "src/fear/stl_helpers.c"
#endif
#include "src/fear/string.c"
#include "src/fear/math.c"

struct FearContext fear_context;