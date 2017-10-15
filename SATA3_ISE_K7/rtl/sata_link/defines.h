 /*
    _______________________________________________________
   |               |         |         |         |         |
   |Primitive Name |  Byte 3 |  Byte 2 |  Byte 1 |  Byte 0 |
   |               | Contents| Contents| Contents| Contents| 
   |_______________|_________|_________|_________|_________|
   |   ALIGNP      |  D27.3  |  D10.2  |  D10.2. |  K28.5  |
   |   CONTP       |  D25.4  |  D25.4  |  D10.5  |  K28.3  | 
   |   DMATP       |  D22.1  |  D22.1  |  D21.5  |  K28.3  | 
   |   EOFP        |  D21.6  |  D21.6  |  D21.5  |  K28.3  |  
   |   HOLDP       |  D21.6  |  D21.6  |  D10.5  |  K28.3  | 
   |   HOLDAP      |  D21.4  |  D21.4  |  D10.5  |  K28.3  |
   |   PMACKP      |  D21.4  |  D21.4  |  D21.4  |  K28.3  |
   |   PMNAKP      |  D21.7  |  D21.7  |  D21.4  |  K28.3  |
   |   PMREQ_PP    |  D23.0  |  D23.0  |  D21.5  |  K28.3  |
   |   PMREQ_SP    |  D21.3  |  D21.3  |  D21.4  |  K28.3  |
   |   PMREQ_SP    |  D21.3  |  D21.3  |  D21.4  |  K28.3  |
   |   R_ERRP      |  D22.2  |  D22.2  |  D21.5  |  K28.3  |
   |   R_IPP       |  D21.2  |  D21.2  |  D21.5  |  K28.3  | 
   |   R_OKP       |  D21.1  |  D21.1  |  D21.5  |  K28.3  | 
   |   R_RDYP      |  D10.2  |  D10.2  |  D21.4  |  K28.3  |
   |   SOFP        |  D23.1  |  D23.1  |  D21.5  |  K28.3  |  
   |   SYNCP       |  D21.5  |  D21.5  |  D21.4  |  K28.3  | 
   |   WTRMP       |  D24.2  |  D24.2  |  D21.5  |  K28.3  | 
   |   X_RDYP      |  D23.2  |  D23.2  |  D21.5  |  K28.3  |
   |_______________|_________|_________|_________|_________|
   
*/   
   
   `define ALIGN       32'h 7B4A4ABC
   `define CONT        32'h 9999AA7C
   `define DMAT        32'h 3636B57C
   `define EOF         32'h D5D5B57C
   `define HOLD        32'h D5D5AA7C
   `define HOLDA       32'h 9595AA7C
   `define PMACK       32'h 9595957C
   `define PMNAK       32'h F5F5957C
   `define PMREQ_P     32'h 1717B57C
   `define PMREQ_S     32'h 7575957C
   `define R_ERR       32'h 5656B57C
   `define R_IP        32'h 5555B57C
   `define R_OK        32'h 3535B57C
   `define R_RDY       32'h 4A4A957C
   `define SOF         32'h 3737B57C
   `define SYNC        32'h B5B5957C
   `define WTRM        32'h 5858B57C
   `define X_RDY       32'h 5757B57C
           
           
   `define L_IDLE         6'h 00
   `define L_SyncEscape   6'h 01
   `define L_NoCommErr    6'h 02
   `define L_NoComm       6'h 03
   `define L_SendAlign    6'h 04
   `define L_RESET        6'h 05
   
   `define HL_SendChkRdy  6'h 06
   `define DL_SendChkRdy  6'h 07
   `define L_SendSOF      6'h 08
   `define L_SendData     6'h 09
   `define L_RcvrHold     6'h 0A
   `define L_SendHold     6'h 0B
   `define L_SendCRC      6'h 0C
   `define L_SendEOF      6'h 0D
   `define L_Wait         6'h 0E
           
   `define L_RcvChkRdy    6'h 0F
   `define L_RcvWaitFifo  6'h 10
   `define L_RcvData      6'h 11
   `define L_Hold         6'h 12
   `define L_RcvHold      6'h 13
   `define L_RcvEOF       6'h 14
   `define L_GoodCRC      6'h 15
   `define L_GoodEnd      6'h 16
   `define L_BadEnd       6'h 17
           
   `define L_TPMPartial   6'h 18
   `define L_TPMSlumber   6'h 19
   `define L_PMOff        6'h 1A
   `define L_PMDeny       6'h 1B
   `define L_ChkPhyRdy    6'h 1C
   `define L_NoCommPower  6'h 1D
   `define L_WakeUp1      6'h 1E
   `define L_WakeUp2      6'h 1F
   `define L_NoPmnak      6'h 20
   
   `define L_InsertAlign1 6'h 21
   `define L_InsertAlign2 6'h 22
   