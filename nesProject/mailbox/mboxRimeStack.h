#ifndef NESPROJECT_MBOXRIMESTACK_H
#define NESPROJECT_MBOXRIMESTACK_H

void initMboxRimeStack();
void sendFromMboxToCentralUnit(unsigned char *cmd, int bytes);

#endif //NESPROJECT_MBOXRIMESTACK_H
