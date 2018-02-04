#ifndef NESPROJECT_CURIMESTACK_H
#define NESPROJECT_CURIMESTACK_H

void initCURimeStack();
void sendDoorNode(unsigned char* c, int bytes);
void sendGateNode(unsigned char* c, int bytes);
void sendRoomLightNodes(unsigned char* c, int bytes);

#endif //NESPROJECT_CURIMESTACK_H
