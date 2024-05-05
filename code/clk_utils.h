#ifndef CLK_UTILS_H
#define CLK_UTILS_H
#include "headers.h"
#define SHKEY 300

void initClk(void);
void destroyClk(bool terminateAll);
int getClk(void);

#endif //