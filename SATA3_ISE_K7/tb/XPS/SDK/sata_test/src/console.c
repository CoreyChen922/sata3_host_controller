/*
 * console.cpp
 *
 */

/****************************************************************************
     Function: RunConsole
       Output: none
  Description: execute Console
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "console.h"
#include "global.h"

#include "platform.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xparameters.h"
#include "xio.h"
#include "xil_types.h"


#define MAX_INPUT_LINE              (4096)         // maximum length of input line
#define CONSOLE_COMMAND_PROMPT      "SATA>"        // string for command prompt




#define SECTOR_COUNT 128

void PrintMessage(TyMessageType tyMessageType, const char * pszToFormat, ...);
int GetCommand(char * pszCommand, unsigned short usMAxLength);
void FilterInputString(char *pszInputString);
void DoConsoleCommand(const char *pszCommandLine);
void StrToUpr(char *pszString);
void ReadRegisters(char *pszString);
void WriteRegisters(char *pszString);
unsigned int reverse(register unsigned int );
void TTest(void);
void WriteData(void);
void ReadData(char *pszParams);
void Test(void);

void RunConsole(void)
{
   char szCommandLine[MAX_INPUT_LINE];
   unsigned char bFinishConsole;

   // show first message
   //PrintMessage(INFO_MESSAGE, MSG_CONSOLE_INFO);
   xil_printf("\r\n%s\r\n", MSG_CONSOLE_INFO);

   // main console loop
   bFinishConsole = FALSE;
   while(!bFinishConsole)
   {


      // when getting commands from command file
      // command is already in pszCommandLine

	  // show command prompt (we cannot use PrintMessage because we need stay on the same line)
	   xil_printf("\n\r%s", CONSOLE_COMMAND_PROMPT);

	  // read user input
	  if(GetCommand(szCommandLine, MAX_INPUT_LINE) == 0)
	  {
	      // check for errors
		  //bFinishConsole = TRUE;
		  strcpy(szCommandLine, "");
		  continue;
      }


      // filter and convert command line
      FilterInputString(szCommandLine);
      //xil_printf("\n\r command = %s", szCommandLine);
      // pass command line to parser
      DoConsoleCommand(szCommandLine);
   }
}


/****************************************************************************
     Function: DoConsoleCommand
     Engineer: Vitezslav Hola
        Input: const char *pszCommandLine : input string with command
               TyCmdFiles *ptyCmdFiles    : pointer to structure with command files
               unsigned char *pbFinish    : pointer to finish variable
       Output: none
  Description: get command line, parse it and call particular function
               for special commands (quit and cfile), fill passed structure and variable
               to signal request for reading file or to exit
Date           Initials    Description
18-Jan-2007    VH          Initial
****************************************************************************/
void DoConsoleCommand(const char *pszCommandLine)
{
    char *pszParsedCommandLine = NULL;
    char *pszCommandParams;

    // split command line, find first space
    pszParsedCommandLine = strdup(pszCommandLine);              // duplicate string (free must be called at the end)
    if(pszParsedCommandLine == NULL)
    {
        // show critical error
    	xil_printf("\n\rInvalid Command");
        return;
    }
    pszCommandParams = strstr(pszParsedCommandLine, " ");
    if(pszCommandParams == NULL)
        {       // no parameters (no space), set pointer to empty string (terminating char)
        pszCommandParams = &(pszParsedCommandLine[strlen(pszParsedCommandLine)]);
        }
    else
        {       // split string
        *pszCommandParams = '\0';           // cut pszParsedCommandLine
        pszCommandParams++;                 // move after space
        }

    // convert command string to uppercase
    StrToUpr(pszParsedCommandLine);

    // start to parse commands

    if(!strcmp(pszParsedCommandLine,"READ"))
    {
    	ReadRegisters(pszCommandParams);
    }
    else if (!strcmp(pszParsedCommandLine,"WRITE"))
    {
    	WriteRegisters(pszCommandParams);
    }
    else if (!strcmp(pszParsedCommandLine,"TTEST"))
    {
       	TTest();
    }
    else if (!strcmp(pszParsedCommandLine,"WRITEDATA"))
    {
    	WriteData();
    }
    else if (!strcmp(pszParsedCommandLine,"READDATA"))
    {
        	ReadData(pszCommandParams);
    }
    else if (!strcmp(pszParsedCommandLine,"TEST"))
    {
    	Test();
    }
    else if (!strcmp(pszParsedCommandLine,"HELP"))
    {
    	xil_printf("\n\r READ        : read from read_address");
		xil_printf("\n\r               Parameters <read_address>");
		xil_printf("\n\r WRITE       : write write_data to write address");
		xil_printf("\n\r               Parameters <write_address> <write_data>");
		xil_printf("\n\r TTEST       : Throughput test");
		xil_printf("\n\r               Parameters NIL");
		xil_printf("\n\r WRITEDATA   : Writes data in to 0x80001xxx memory");
	    xil_printf("\n\r               Parameters NIL");
	    xil_printf("\n\r READDATA    : Reads data from 0x80002xxx memory");
	    xil_printf("\n\r               Parameters [print] [compare]");
	    xil_printf("\n\r               print = 1 for print data compare = 1 for compare");
		xil_printf("\n\r HELP        : Diplay this help command");
		xil_printf("\n\r               Parameters NIL");
    }
    else
    {
    	xil_printf("\n\rInvalid command");
    }

    // free memory after strdup
    if(pszParsedCommandLine != NULL)
        free(pszParsedCommandLine);
}

void StrToUpr(char *pszString)
{

   while (*pszString)
      {
      if ((*pszString >= 'a') && (*pszString <= 'z'))
         {
         *pszString += ('A' - 'a');
         }
      pszString++;
      }
}

/****************************************************************************
     Function: GetCommand
     Engineer: Shameerudheen P T
        Input:
               char * pszCommand - pointer to command string
       Output: int -number of characters
  Description: reading command from serial port
Date           Initials    Description
30-Jul-2013    SPT          Initial
*****************************************************************************/
int GetCommand(char * pszCommand, unsigned short usMAxLength)
{
	char ctemp;
	int i;
	for (i = 1; i < usMAxLength; i++)
	{
		ctemp = XUartLite_RecvByte(XPAR_RS232_UART_1_BASEADDR);
		if (ctemp == '\r')
		{
			*(pszCommand) = '\0';
			return (i-1);
		}
		else
		{
			*(pszCommand++) = ctemp;
		}
	}
	*(pszCommand) = '\0';
    return (i-1);
}

void TTest(void)
{
	u32 status_reg,iteration_count_reg,timer_status = 0;
	u32 write_throughput_reg_low,write_throughput_reg_high;
	u32 read_throughput_reg_low,read_throughput_reg_high;
	u64 write_throughput_reg, read_throughput_reg;
	u32 expected_data_reg,read_data_reg;


	const u32 SECTORS   = 128; //sectors per read/write
	u32 cmd_complete;
	u32 i;
	u32 * initial_value = (u32 *)0x8010001C;
	u32 * ptotal_iteration_count = (u32 *) 0x80100000;

	xil_printf("\n\r Throughput Test");

	xil_printf("\n\r Iteration count %d",(*ptotal_iteration_count));
	xil_printf("\n\r Initial value %d",(*initial_value));


	//reseting test_reset_reg (GTPRESET_IN)
	//Xil_Out32(SATA_BASEADDR, 0);
	//Xil_Out32(SATA_BASEADDR, 1);
	//Xil_Out32(SATA_BASEADDR, 0);

	//for (i=0; i<1000; i++); //if delay requires

	Xil_Out32(SATA_BASEADDR + (0x100004) ,0x1); //writing command reg (invoking "ttest" command)

	//DMA write and read

	Xil_Out32(XPS_TIMER_BASEADDR + (0),0x4c0); //0x4c0 // To enable timer and interrupt
	do
	{
	    status_reg = Xil_In32(SATA_BASEADDR + (0x100008));
		timer_status = Xil_In32(XPS_TIMER_BASEADDR + (0));
	}while(((status_reg & 0x1) == 0x1) && ((timer_status & 0x100) == 0)); //checking WR_complete

	if((timer_status & 0x100) == 0x100)
	{
		Xil_Out32(XPS_TIMER_BASEADDR + (0),0x120); // To disable timer and interrupt
		Xil_Out32(XPS_TIMER_BASEADDR + (4),0);     // Load value 0
		Xil_Out32(XPS_TIMER_BASEADDR + (0),0x4c0); //0x4c0 // To enable timer and interrupt
		do
		{
			status_reg = Xil_In32(SATA_BASEADDR + (0x100008));
			timer_status = Xil_In32(XPS_TIMER_BASEADDR + (0));
		}while(((status_reg & 0x1) == 0x1) && ((timer_status & 0x100) == 0)); //checking WR_complete

		if((timer_status & 0x100) == 0x100)
		{
			Xil_Out32(XPS_TIMER_BASEADDR + (0),0x120); // To disable timer and interrupt
			Xil_Out32(XPS_TIMER_BASEADDR + (4),0);     // Load value 0
			Xil_Out32(XPS_TIMER_BASEADDR + (0),0x4c0); //0x4c0 // To enable timer and interrupt
			do
			{
				status_reg = Xil_In32(SATA_BASEADDR + (0x100008));
				timer_status = Xil_In32(XPS_TIMER_BASEADDR + (0));
			}while(((status_reg & 0x1) == 0x1) && ((timer_status & 0x100) == 0)); //checking WR_complete

			if((timer_status & 0x100) == 0x100)
			{
				if ((*ptotal_iteration_count) <= 40000)
				{
					xil_printf("\n\r time out (iteration count <= 40000) while write/read. status reg.: 0x%x",status_reg);
					return;
				}
				else
				{
					Xil_Out32(XPS_TIMER_BASEADDR + (0),0x120); // To disable timer and interrupt
					Xil_Out32(XPS_TIMER_BASEADDR + (4),0);     // Load value 0
					Xil_Out32(XPS_TIMER_BASEADDR + (0),0x4c0); //0x4c0 // To enable timer and interrupt
					do
					{
						status_reg = Xil_In32(SATA_BASEADDR + (0x100008));
						timer_status = Xil_In32(XPS_TIMER_BASEADDR + (0));
					}while(((status_reg & 0x1) == 0x1) && ((timer_status & 0x100) == 0)); //checking WR_complete
					if((timer_status & 0x100) == 0x100)
					{
						if ((*ptotal_iteration_count) <= 60000)
						{
							xil_printf("\n\r time out (iteration count <= 60000) while write/read. status reg.: 0x%x",status_reg);
							return;
						}
						else
						{
							Xil_Out32(XPS_TIMER_BASEADDR + (0),0x120); // To disable timer and interrupt
							Xil_Out32(XPS_TIMER_BASEADDR + (4),0);     // Load value 0
							Xil_Out32(XPS_TIMER_BASEADDR + (0),0x4c0); //0x4c0 // To enable timer and interrupt
							do
							{
								status_reg = Xil_In32(SATA_BASEADDR + (0x100008));
								timer_status = Xil_In32(XPS_TIMER_BASEADDR + (0));
							}while(((status_reg & 0x1) == 0x1) && ((timer_status & 0x100) == 0)); //checking WR_complete
							if((timer_status & 0x100) == 0x100)
							{
								if ((*ptotal_iteration_count) <= 80000)
								{
									xil_printf("\n\r time out (iteration count <= 80000) while write/read. status reg.: 0x%x",status_reg);
									return;
								}
								else
								{
									Xil_Out32(XPS_TIMER_BASEADDR + (0),0x120); // To disable timer and interrupt
									Xil_Out32(XPS_TIMER_BASEADDR + (4),0);     // Load value 0
									Xil_Out32(XPS_TIMER_BASEADDR + (0),0x4c0); //0x4c0 // To enable timer and interrupt
									do
									{
										status_reg = Xil_In32(SATA_BASEADDR + (0x100008));
										timer_status = Xil_In32(XPS_TIMER_BASEADDR + (0));
									}while(((status_reg & 0x1) == 0x1) && ((timer_status & 0x100) == 0)); //checking WR_complete
									if((timer_status & 0x100) == 0x100)
									{
										if ((*ptotal_iteration_count) <= 100000)
										{
											xil_printf("\n\r time out (iteration count <=100000) while write/read. status reg.: 0x%x",status_reg);
											return;
										}
										else
										{
											Xil_Out32(XPS_TIMER_BASEADDR + (0),0x120); // To disable timer and interrupt
											Xil_Out32(XPS_TIMER_BASEADDR + (4),0);     // Load value 0
											do
											{
												status_reg = Xil_In32(SATA_BASEADDR + (0x100008));
												timer_status = Xil_In32(XPS_TIMER_BASEADDR + (0));
											}while(((status_reg & 0x1) == 0x1)); //checking WR_complete
										}
									}
								}
						   }
						}
					}
				}
			}
		}
	}

	Xil_Out32(XPS_TIMER_BASEADDR + (0),0x120); // To disable timer and interrupt
	Xil_Out32(XPS_TIMER_BASEADDR + (4),0);     // Load value 0
	xil_printf("\n\r write complete");

	if ((status_reg & 0x1) != 0x1)
	{
		write_throughput_reg_low = Xil_In32(SATA_BASEADDR + (0x10000C));
		write_throughput_reg_high = Xil_In32(SATA_BASEADDR + (0x100010));
		iteration_count_reg = Xil_In32(SATA_BASEADDR + (0x100014));
		write_throughput_reg = write_throughput_reg_high;
		write_throughput_reg = ((write_throughput_reg << 32)| write_throughput_reg_low);
		//xil_printf("\n\r WRITE Throughput count Low %d%d, in hex: %x%08x", write_throughput_reg_high,
		//		                                                       write_throughput_reg_low,
		//		                                                       write_throughput_reg_high,
		//		                                                       write_throughput_reg_low);
		xil_printf("\n\r WRITE Throughput count in hex: 0x%x%08x", write_throughput_reg_high, write_throughput_reg_low);
		//xil_printf("\n\r WRITE Throughput count High %d, in hex: %x",write_throughput_reg_high, write_throughput_reg_high);
//		write_throughput_reg = 0x10;
//		xil_printf("\n\r WRITE Throughput count %llx",write_throughput_reg);
		xil_printf("\n\r WRITE Throughput data: (%d x %d x %d) Bytes, time: (0x%x%08x x %d)ns \n\r",
				iteration_count_reg , SECTORS , 512, write_throughput_reg_high, write_throughput_reg_low, 5);


	}



	if ((status_reg & 0x2) == 0x2)
	{
		xil_printf("\n\r Comparison Error");
		iteration_count_reg = Xil_In32(SATA_BASEADDR + (0x100014));
		xil_printf("\n\r Iteration count: %d ", iteration_count_reg);
		expected_data_reg = Xil_In32(SATA_BASEADDR + (0x100028));
		xil_printf("\n\r Expected Data: %d ", expected_data_reg);
		read_data_reg = Xil_In32(SATA_BASEADDR + (0x10002C));
		xil_printf("\n\r Actual Read Data: %d ", read_data_reg);
		return;
	}

	cmd_complete = Xil_In32(SATA_BASEADDR + (0x100018));

	if ((cmd_complete & 0x1) == 0x1)
	{
		read_throughput_reg_low = Xil_In32(SATA_BASEADDR + (0x100020));
		read_throughput_reg_high = Xil_In32(SATA_BASEADDR + (0x100024));
		iteration_count_reg = Xil_In32(SATA_BASEADDR + (0x100014));
		read_throughput_reg = read_throughput_reg_high;
		read_throughput_reg = ((read_throughput_reg << 32)| read_throughput_reg_low);
		//xil_printf("\n\r Read Throughput count %d, in hex: %x %08x", read_throughput_reg_high,
		//		                                                    read_throughput_reg_low,
		//		                                                    read_throughput_reg_high,
		//		                                                    read_throughput_reg_low);
		xil_printf("\n\r Read Throughput count in hex: 0x%x%08x", read_throughput_reg_high, read_throughput_reg_low);
		//xil_printf("\n\r Read Throughput count %d, in hex: %x",read_throughput_reg_high, read_throughput_reg_high);
	    //xil_printf("\n\r Read Throughput count %d",read_throughput_reg);
		xil_printf("\n\r Read Throughput data: (%d x %d x %d) Bytes, time: (0x%x%08x x %d)ns",
				iteration_count_reg, SECTORS, 512, read_throughput_reg_high, read_throughput_reg_low, 5);

	}
}

void ReadRegisters(char *pszParams)
{
	unsigned long uladdress;
	u32 ureadvalue;

	//xil_printf("\n\r Read function Called");

	//xil_printf("\n\r Read parameter%s", pszParams );

	if(!strcmp(pszParams, ""))
	{
		xil_printf("\n\r Read address not given");
		return;
	}

	if ((pszParams[0] == '0') && ((pszParams[1] == 'x') || (pszParams[1] == 'X')))
	{
		sscanf(pszParams,"%lx",&uladdress);
	}
	else
	{
		uladdress = (unsigned long) atol(pszParams);
	}
	//xil_printf("\n\r Read parameter %d (%x)", uladdress, uladdress );

	ureadvalue = Xil_In32(SATA_BASEADDR + (uladdress & 0xFFFFFF));
	xil_printf("\n\r Read value = %8d (0x%08x)", ureadvalue, ureadvalue);
    return;
}


void WriteRegisters(char *pszParams)
{
	u32 ulAddress, ureadvalue;
	u32 ulData;
	char * pszNextParam;
	//xil_printf("\n\r Write function Called");
	if(!strcmp(pszParams, ""))
	{
		xil_printf("\n\r Write address not given");
		return;
	}
	//xil_printf("\n\r Write address string %s", pszParams );
	if ((pszParams[0] == '0') && ((pszParams[1] == 'x') || (pszParams[1] == 'X'))) //support for hexa decimal nos.
	{
		sscanf(pszParams,"%lx",&ulAddress);
	}
	else
	{
		ulAddress = (unsigned long) atol(pszParams);
	}

	//xil_printf("\n\r Write addr parameter %d (%x)", ulAddress, ulAddress );

    //xil_printf("\n\r Write address = ", ulAddress); putnum (ulAddress);

    pszNextParam = strstr(pszParams, " ");
	    if(pszNextParam == NULL)
	    {       // no parameters (no space), set pointer to empty string (terminating char)
	    	pszNextParam = &(pszParams[strlen(pszParams)]);
	    }
	    else
	    {       // split string
	        *pszNextParam = '\0';           // cut pszParsedCommandLine
	        pszNextParam++;                 // move after space
	    }
	if(!strcmp(pszNextParam, ""))
	{
		xil_printf("\n\r Write data not given");
		return;
	}


	if ((pszNextParam[0] == '0') && ((pszNextParam[1] == 'x') || (pszNextParam[1] == 'X')))
	{
		sscanf(pszNextParam,"%lx",&ulData);
	}
	else
	{
		ulData = (unsigned long) atol(pszNextParam);
	}
	//xil_printf("\n\r Write data parameter %d (%x)", ulData, ulData )

	Xil_Out32(SATA_BASEADDR + (ulAddress & 0xFFFFFF),ulData);
	//Xil_Out32(SATA_BASEADDR+4,ulData);
	return;
}

void WriteData(void)
{
	u32 i;

	for (i = 0x100000; i < (0x100000 + (SECTOR_COUNT * 0x200)); i = i + 4)
	{
		Xil_Out32(SATA_BASEADDR + (i & 0xFFFFFF),i);
	}
	xil_printf("\n\r Write done"); //putnum(ureadvalue);
    return;
}

void ReadData(char *pszParams)
{
	u32 i, ureadvalue, uParam, uParam2;
	char * pszNextParam;

	if(strcmp(pszParams, ""))
	{
		uParam = (unsigned long) atol(pszParams);
	}
	else
	{
		uParam = 0;
	}

	pszNextParam = strstr(pszParams, " ");
	if(pszNextParam == NULL)
	{       // no parameters (no space), set pointer to empty string (terminating char)
		pszNextParam = &(pszParams[strlen(pszParams)]);
	}
	else
	{       // split string
		*pszNextParam = '\0';           // cut pszParsedCommandLine
		pszNextParam++;                 // move after space
	}

	if(strcmp(pszNextParam, ""))
	{
		uParam2 = (unsigned long) atol(pszNextParam);
	}
	else
	{
		uParam2 = 0;
	}

	for (i = 0x200000; i < (0x200000 + (SECTOR_COUNT * 0x200)); i = i + 4)
	{
		ureadvalue = Xil_In32(SATA_BASEADDR + (i & 0xFFFFFF));
		if (uParam == 1)
			xil_printf("\n\r  %8d (0x%08x)", ureadvalue, ureadvalue);

		if (ureadvalue != (i - (0x100000)))
		{
			if (uParam2 == 1)
			{
				xil_printf("\n\r Error, read value: %8d  expected value (%8d)", ureadvalue, (i - (0x10000)));
				return;
			}
		}
	}
	xil_printf("\n\r Read done");
    return;
}

void Test(void)
{
/*
	u32 i, status_reg;

	for (i=0; i<10; i++)
	{
		Xil_Out32(SATA_BASEADDR + (4), 1);
		do
		{
			status_reg = Xil_In32(SATA_BASEADDR + (12));
		}while((status_reg & 0x7) != 0);
	}
	xil_printf("\n\r test done");
*/
    return;
}

/****************************************************************************
     Function: FilterInputString
     Engineer: Shameerudheen P T
        Input: char *pszInputString : pointer to input string
       Output: none
  Description: remove special characters ('\r', etc.) from the string,
               remove duplicated spaces
Date           Initials    Description
30-Jul-2013    SPT          Initial
****************************************************************************/
void FilterInputString(char *pszInputString)
{
    char *pszString;
    unsigned char bNextSpace;
    unsigned int uiLastChar;


    // check for special characters and duplicate space
    pszString = pszInputString;
    bNextSpace = TRUE;          // remove spaces on the begining
    while(*pszString)
        {
        switch(*pszString)
            {
            case ' '  :
                if(bNextSpace)
                    {
                    // remove (*pszString) from pszInputString
                    memmove((void *)pszString, (void *)(pszString+1), strlen(pszString));
                    // do not move pszString
                    }
                else
                    {
                    bNextSpace = TRUE;
                    pszString++;
                    }
                break;

            case '\r' :
            case '\n' :
            case '\t' :
            case '\b' :
                // remove (*pszString) from pszInputString
				memmove((void *)pszString, (void *)(pszString+1), strlen(pszString));
                // do not move pszString
                break;

            default:
                pszString++;                // next character
                bNextSpace = FALSE;         // correct character, next space is valid
                break;
            }
        }

    // check for space at the end of the string
    uiLastChar = strlen(pszInputString);
    if((uiLastChar > 0) && (pszInputString[uiLastChar - 1] == ' '))
        pszInputString[uiLastChar - 1] = '\0';
}


















/****************************************************************************
     Function: PrintMessage
     Engineer: Shameerudheen P T
        Input: TyMessageType tyMessageType - type of message
               char * pszToFormat - format of message with variable number of parameters (as printf)
       Output: none
  Description: prints out messages to console, debug window, etc...
Date           Initials    Description
30-Jul-2013    SPT          Initial
*****************************************************************************/
/*
void PrintMessage(TyMessageType tyMessageType, const char * pszToFormat, ...)
{
   char     szBuffer[MAX_OUTPUT_LINE];
   char     szDisplay[MAX_OUTPUT_LINE];
   va_list  marker; // = NULL; commented for builing in linux

//lint -save -e*
   va_start(marker, pszToFormat);
   vsprintf(szBuffer, pszToFormat, marker);
   va_end  (marker);
//lint -restore

   switch (tyMessageType)
      {
      case HELP_MESSAGE:
         strcpy(szDisplay, szBuffer);
         break;
      case INFO_MESSAGE:
         sprintf(szDisplay, "%s\n\r", szBuffer);
         break;
      case ERROR_MESSAGE:
         sprintf(szDisplay, "Error: %s\n\r", szBuffer);
         break;
      case DEBUG_MESSAGE:
         sprintf(szDisplay, "Debug: %s\n\r", szBuffer);
         break;
      case LOG_MESSAGE:
//         LogMessage(szBuffer, "");
         return;
      default:
         return;
      }

   printf("%s",szDisplay);
}
*/
