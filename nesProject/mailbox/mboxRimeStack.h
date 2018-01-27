//
// Created by Raff on 31/10/2017.
//

#ifndef NESPROJECT_MBOXRIMESTACK_H
#define NESPROJECT_MBOXRIMESTACK_H

void initMboxRimeStack();
void sendFromMboxToCentralUnit(unsigned char *cmd, int bytes);

#endif //NESPROJECT_MBOXRIMESTACK_H
