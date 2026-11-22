#include "mouz_runtime.h"
static int runtime_active = 0;
void mouz_runtime_start(void) { runtime_active = 1; }
void mouz_runtime_stop(void) { runtime_active = 0; }
const char* mouz_runtime_version(void) { return runtime_active ? "Mouz Runtime 0.1.0 (active)" : "Mouz Runtime 0.1.0"; }
