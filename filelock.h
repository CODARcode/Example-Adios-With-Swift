#ifndef FILELOCK_H
#define FILELOCK_H

#ifdef __cplusplus
extern "C" {
#endif

void lockup(const char* fname);
void lockdown(const char* fname);

#ifdef __cplusplus
}
#endif
#endif
