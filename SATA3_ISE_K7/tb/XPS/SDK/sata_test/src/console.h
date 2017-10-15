/*
 * console.h
 *
 *  Created on: Jul 30, 2013
 *      Author: Shameerudheen P T
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#define MSG_CONSOLE_INFO       "Please enter your command."

#define MAX_OUTPUT_LINE                      4096

typedef enum {
   INFO_MESSAGE      = 0,
   ERROR_MESSAGE     = 1,
   HELP_MESSAGE      = 2,
   DEBUG_MESSAGE     = 3,
   LOG_MESSAGE       = 4
} TyMessageType;

void RunConsole(void);

#endif /* CONSOLE_H_ */
