#ifndef MOUZ_RUNTIME_H
#define MOUZ_RUNTIME_H
#ifdef __cplusplus
extern "C" {
#endif
void mouz_runtime_start(void);
void mouz_runtime_stop(void);
const char* mouz_runtime_version(void);
#ifdef __cplusplus
}
#endif
#endif
