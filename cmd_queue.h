#include <stdio.h>

#ifndef cmd_queue_h
#define cmd_queue_h

void cqInit();
void cqRelease();
void cqPushCommand(char *cmd, void *ref);
void cqExecuteCommand();

void tsWait(uint t);
void tsStart();
void tsEnd();

#endif