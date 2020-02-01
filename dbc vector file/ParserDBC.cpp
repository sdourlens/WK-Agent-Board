#include <string.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>    
#include <stdio.h>
#include <list>
#include <stddef.h>
#include "ParserDBC.h"

using namespace std;

/*********************************************************
*                                                        *
*                REGISTERING VARIABLES                   *
*                                                        *
**********************************************************/
std::list<int> lstFRAME_ID;                                //List to register all the FRAMES IDs
std::list<std::string> lstFRAME_NAME;                      //List to register all FRAMES names
int indList=0;                                             //Index to search param in lists
std::list<int> lstFRAME_DLC;                               //List to register all the FRAMES DLCs
std::list<std::string> lstFRAME_TRANSMITTER;               //List to register all the FRAMES transmitters

std::list<std::string> lstSG_RECEIVERS;                    //List to register all the SIGNALS receivers
std::list<std::string> lstSG_UNIT;                         //List to register all the SIGNALS units
std::list<float> lstSG_MIN;                                //List to register all the SIGNALS minimum values
std::list<float> lstSG_MAX;                                //List to register all the SIGNALS maximum values
std::list<float> lstSG_FACTOR;                             //List to register all the SIGNALS Factors
std::list<float> lstSG_OFFSET;                             //List to register all the SIGNALS Offsets
std::list<int> lstSG_BYTEORDER;                            //List to register all the SIGNALS byte orders (INTEL/MOTOROLA)
std::list<int> lstSG_TYPE;                                 //List to register all the SIGNALS data types (signed/unsigned)
std::list<int> lstSG_POSITION;                             //List to register all the SIGNALS position of start bit
std::list<int> lstSG_LENGTH;                               //List to register all the SIGNALS data length
std::list<std::string> lstSG_NAME;                         //List to register all the SIGNALS names
std::list<int> lstSG_ID;                                   //List to register all the SIGNALS IDs

std::list<int>::iterator IT_int;                           //Iterator to scan the lists of INTEGERS
std::list<std::string>::iterator IT_str;                   //Iterator to scan the lists of STRING
std::list<float>::iterator IT_float;                       //Iterator to scan the lists of FLOAT


/*********************************************************
*                                                        *
*  VARIABLES USED TO PARSE THE MESSAGES IN THE DBC FILE  *
*                                                        *
**********************************************************/
std::string strBO ("BO_");                                 //Reference string used to find CAN messages in DBC file

bool SIGNAL_CHECKING=0;                                    //Status to check CAN messages parsing
int INDEX_FRAME=0;                                         //Number of CAN messages in DBC file

std::string strID="";                                      //string to scan CAN messages IDs in dbc file
int iID=0;                                                 //integer to scan CAN messages IDs in dbc file
std::string strNAME="";                                    //string to scan CAN messages names in dbc file
std::string strDLC="";                                     //string to scan CAN messages DLCs in dbc file
int iDLC=0;                                                //integer to scan CAN messages IDs in dbc file
std::string strSG_Transmitter="";                          //string to scan CAN messages transmitters in dbc file


/*********************************************************
*                                                        *
*  VARIABLES USED TO PARSE THE MESSAGES IN THE DBC FILE  *
*                                                        *
**********************************************************/
std::string strSG ("SG_");                                 //Reference string used to find SIGNALS in dbc file

bool MESSAGE_CHECKING=0;                                   //Status to check SIGNALS parsing
int NB_RECEIVERS=0;                                        //number of receivers of a signal (separated by ',')

std::string strSG_NAME = "";                               //string to scan SIGNALS names in dbc file
std::string strSG_Position = "";                           //string to scan SIGNALS position in dbc file
int iSG_Position;                                          //integer to scan SIGNALS position in dbc file
std::string strSG_Length="";                               //string to scan SIGNALS length in dbc file
int iSG_Length=0;                                          //integer to scan SIGNALS length in dbc file
int iByteOrder=0;                                          //integer to scan SIGNALS byte orders in dbc file
int iType=1;                                               //integer to scan SIGNALS data types in dbc file
std::string strSG_Factor="";                               //string to scan SIGNALS factors in dbc file
float fSG_Factor=0;                                        //float to scan SIGNALS factors in dbc file
std::string strSG_Offset="";                               //string to scan SIGNALS offsets in dbc file
float fSG_Offset=0;                                        //float to scan SIGNALS offsets in dbc file
std::string strSG_Min="";                                  //string to scan SIGNALS minimum values in dbc file
float fSG_Min=0;                                           //float to scan SIGNALS minimum values in dbc file
std::string strSG_Max="";                                  //string to scan SIGNALS maximum values in dbc file
float fSG_Max=0;                                           //float to scan SIGNALS maximum values in dbc file
std::string strSG_Unit="";                                 //string to scan SIGNALS units in dbc file
std::string strSG_Receivers="";                            //string to scan SIGNALS receivers in dbc file
std::string cCAR="";                                       //used for string convertion


/*********************************************************
*                                                        *
*                 ALL USING VARIABLES                    *
*                                                        *
**********************************************************/
const char* FilePath = "C:\\Users\\Florian CURTIL\\Desktop\\Parser\\Test Valence 440h.dbc";

std::string strPonct(":");
std::string ligne;                                        //used to read a line of the dbc file
std::size_t foundFrame;                                   //return positions in the lines of the found caracters
std::size_t foundSignal;                                  //return positions in the lines of the found caracters
std::size_t foundPonct;                                   //return positions in the lines of the found caracters
int i = 0;
int j = 0;
int indcar=0;


/******************* END VARIABLES*************************/   


/******************************************************************************
* FUNCTION: init_values
*
* Description: Erase and Reset all the variables used to scan
*              the DBC file
*            
* Arguments: /
*
* return: /
*          
*******************************************************************************/
void init_Values()
{
   SIGNAL_CHECKING=0;
   MESSAGE_CHECKING=0;
   NB_RECEIVERS=0;
   strID="";
   strNAME="";
   strDLC="";
   strSG_Transmitter="";
      
   strSG_NAME = "";
   strSG_Position="";
   strSG_Length="";
   strSG_Factor="";
   strSG_Offset="";
   strSG_Min="";
   strSG_Max="";
   strSG_Unit="";
   strSG_Receivers="";

}


/******************************************************************************
* FUNCTION: GetAndShow_Signal_Receivers
*
* Description: Search and save in a list all the parameters "receivers" of 
*              the signals
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Signal_Receivers()
{
    indcar++;
    cCAR= ligne[indcar];
 
    while (((cCAR.compare(" ") == 0)||(cCAR.compare("\t")==0))&&(indcar<ligne.length()))
    {
       indcar++;
       cCAR= ligne[indcar];
    }

    while ((indcar < ligne.length())&&(cCAR.compare(" ") != 0)&&(cCAR.compare("\t")!=0))
    {
       
       strSG_Receivers.push_back(ligne[indcar]);

       if (cCAR.compare(",")==0)
       {
        NB_RECEIVERS++;
       }                        
       indcar++;
       cCAR= ligne[indcar];  
    }
    if (strSG_Receivers!="")
    {
       NB_RECEIVERS++;       
    }
    lstSG_RECEIVERS.push_back(strSG_Receivers.c_str());
}


/******************************************************************************
* FUNCTION: GetAndShow_Signal_Unit
*
* Description: Search and save in a list all the parameters "unit" of 
*              the signals
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Signal_Unit()
{
 cCAR= ligne[indcar];
 
    while (cCAR.compare("\"") != 0)
    {
       indcar++;
       cCAR= ligne[indcar];
    }
    indcar++;
    cCAR= ligne[indcar];
    while (cCAR.compare("\"") != 0)
    {
       strSG_Unit.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];  
    }
    if(strSG_Unit.compare("") == 0)
    {
        strSG_Unit="No_Unit";
    }
    lstSG_UNIT.push_back(strSG_Unit.c_str());
}

/******************************************************************************
* FUNCTION: GetAndShow_Signal_MinMax_Values
*
* Description: Search and save in a list all the parameters "min" and "max" of 
*              the signals
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Signal_MinMax_Values()
{
 cCAR= ligne[indcar];
 
    while (cCAR.compare("[") != 0)
    {
       indcar++;
       cCAR= ligne[indcar];
    }
    indcar++;
    cCAR= ligne[indcar];
    while (cCAR.compare("|") != 0)
    {
       strSG_Min.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];  
    }
    fSG_Min = atof (strSG_Min.c_str());

    lstSG_MIN.push_back(fSG_Min);
    indcar++;
    cCAR= ligne[indcar];
    
     while (cCAR.compare("]") != 0)
    {
       strSG_Max.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];
    }
    fSG_Max = atof (strSG_Max.c_str());
    lstSG_MAX.push_back(fSG_Max);
}

/******************************************************************************
* FUNCTION: GetAndShow_Signal_Conversion
*
* Description: Search and save in a list all the parameters "offset" and  
*              "factor" of the signals
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Signal_Conversion()
{
 cCAR= ligne[indcar];
 
    while (cCAR.compare("(") != 0)
    {
       indcar++;
       cCAR= ligne[indcar];
    }
    indcar++;
    cCAR= ligne[indcar];
    while (cCAR.compare(",") != 0)
    {
       strSG_Factor.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];  
    }
    fSG_Factor = atof (strSG_Factor.c_str());
    lstSG_FACTOR.push_back(fSG_Factor);
    
    indcar++;
    cCAR= ligne[indcar];
    
     while (cCAR.compare(")") != 0)
    {
       strSG_Offset.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];
    }
    fSG_Offset = atof (strSG_Offset.c_str());
    lstSG_OFFSET.push_back(fSG_Offset);
}

/******************************************************************************
* FUNCTION: GetAndShow_Signal_ByteOrder_And_Type
*
* Description: Search and save in a list all the parameters "byte order" and  
*              "data type" of the signals
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Signal_ByteOrder_And_Type()
{
    indcar++;
    cCAR= ligne[indcar];  
    if ((cCAR.compare("0") == 0)||(cCAR.compare("\t")==0))
    {
       iByteOrder=0;//MOTOROLA                        
    }
    else
    {
        iByteOrder=1;//INTEL  
    }
    lstSG_BYTEORDER.push_back(iByteOrder);
    indcar++;
    cCAR= ligne[indcar];
    
    if (cCAR.compare("+") == 0)
    {
       iType=1;       //unsigned                  
    }
    else
    {
       iType=-1;      //signed
    }
    lstSG_TYPE.push_back(iType);
    
}

/******************************************************************************
* FUNCTION: GetAndShow_Signal_Position_And_Lenght
*
* Description: Search and save in a list all the parameters "position" and  
*              "length" of the signals
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void  GetAndShow_Signal_Position_And_Lenght()
{
   while (cCAR.compare(":") != 0)
    {
       indcar++;
       cCAR= ligne[indcar]; 
    }
    indcar++;
    cCAR= ligne[indcar];
    
    while ((cCAR.compare(" ") == 0)||(cCAR.compare("\t")==0))
    {
       indcar++;
       cCAR= ligne[indcar];
    }

    while (cCAR.compare("|") != 0)
    {
       strSG_Position.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];  
    }
    iSG_Position = atoi (strSG_Position.c_str());
    lstSG_POSITION.push_back(iSG_Position);
    indcar++;
    cCAR= ligne[indcar];
    
    while (cCAR.compare("@") != 0)
    {
       strSG_Length.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];  
    }
    iSG_Length = atoi (strSG_Length.c_str());
    lstSG_LENGTH.push_back(iSG_Length);
}

/******************************************************************************
* FUNCTION: GetAndShow_Signal_NAME
*
* Description: Search and save in a list all the names of the signals  
*              
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Signal_NAME()
{
    indcar=foundSignal+3;
    cCAR= ligne[indcar];
 
    while ((cCAR.compare(" ") == 0)||(cCAR.compare("\t")==0))
    {
       indcar++;
       cCAR= ligne[indcar];
       
    }

    while ((cCAR.compare(" ") != 0)&&(cCAR.compare("\t")!=0)&&(cCAR.compare(":") != 0))
    {
       strSG_NAME.push_back(ligne[indcar]);                        
       indcar++;
       cCAR= ligne[indcar];  
    }
    lstSG_NAME.push_back(strSG_NAME.c_str());
}

/******************************************************************************
* FUNCTION: Associate_IDFrame_To_Signal()
*
* Description: Create a list to associate the Signals with the CAN frames ID
*              
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void Associate_IDFrame_To_Signal()
{
 lstSG_ID.push_back(iID);
}

/******************************************************************************
* FUNCTION: GetAndShow_Message_Transmitter
*
* Description: Search and save in a list all the parameters "transmitter" 
*              of the CAN FRAMES
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Message_Transmitter()
{
 
    while (((cCAR.compare(" ") == 0)||(cCAR.compare("\t")==0))&&(indcar<ligne.length()))
    {
       indcar++;
       cCAR= ligne[indcar];
    }
    while ((indcar < ligne.length())&&(cCAR.compare(" ") != 0)&&(cCAR.compare("\t")!=0))
    {   
       strSG_Transmitter.push_back(ligne[indcar]);                      
       indcar++;
       cCAR= ligne[indcar];  
    }
    lstFRAME_TRANSMITTER.push_back(strSG_Transmitter.c_str());
}

/******************************************************************************
* FUNCTION: GetAndShow_Message_DLC
*
* Description: Search and save in a list all the parameters "DLC" 
*              of the CAN FRAMES
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Message_DLC()
{
    while ((cCAR.compare(" ") == 0)||(cCAR.compare("\t")==0))
    {
       indcar++;
       cCAR= ligne[indcar];
    }

    while ((cCAR.compare(" ") != 0)&&(cCAR.compare("\t")!=0))
    {
       strDLC.push_back(ligne[indcar]);                           
       indcar++;
       cCAR= ligne[indcar];
          
    }
    iDLC = atoi (strDLC.c_str()); 
    lstFRAME_DLC.push_back(iDLC);   
}

/******************************************************************************
* FUNCTION: GetAndShow_Message_NAME
*
* Description: Search and save in a list all the NAMES
*              of the CAN FRAMES
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Message_Name()
{
     while ((cCAR.compare(" ") == 0)||(cCAR.compare("\t")==0))
    {
       indcar++;
       cCAR= ligne[indcar];
    }
    
     while (cCAR.compare(":") != 0)
    {
       strNAME.push_back(ligne[indcar]); 
       indcar++;  
       cCAR= ligne[indcar];                         
    }
    indcar++;
    lstFRAME_NAME.push_back(strNAME.c_str());
}

/******************************************************************************
* FUNCTION: GetAndShow_Message_Transmitter
*
* Description: Search and save in a list all CAN FRAMES IDS 
*              of the CAN FRAMES
*            
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void GetAndShow_Message_ID()
{
    indcar=foundFrame+3;
    cCAR= ligne[indcar];

    while ((cCAR.compare(" ") == 0)||(cCAR.compare("\t")==0))
    {
       indcar++;
       cCAR= ligne[indcar];
    }

    while ((cCAR.compare(" ") != 0)&&(cCAR.compare("\t")!=0))
    {
       strID.push_back(ligne[indcar]);                           
       indcar++;
       cCAR= ligne[indcar];    
    }
    iID = atoi (strID.c_str());
    lstFRAME_ID.push_back(iID);     

}


/******************************************************************************
* FUNCTION: ReadOneSignal
*
* Description: Read all the parameters of ONE frame and convert the hexadecimal
*               value (in frame) to  physical value
*            
* Arguments: Structure NEWFRAME (composed of all the parameters and data)
*
* Return: the Structure NEWFRAME modified
*          
*******************************************************************************/
NEWFRAME ReadOneSignal(NEWFRAME f)
{
//Get Structure variables
int startbit=f.Signal.POSITION;
int length=f.Signal.LENGTH;
int byteorder=f.Signal.BYTEORDER;
float factor=f.Signal.FACTOR;
float offset=f.Signal.OFFSET;
int type=f.Signal.TYPE;

//
unsigned int SIGNALtemp             = 0;
unsigned int SIGNAL_HEX             = 0;
unsigned int MASK                   = 0;
int          k                      = 0;
int          last_bit_position      = 0;
int          bit_position           = 0;
float        SIGNAL_PHYS            = 0;

//--------------DONNEES RECUES PAR CAN----------------///

unsigned int TRAME_LOW =0x00;                         //4 first bytes of the frame (0 to 3)                       
unsigned int TRAME_HIGH=0x00;                         //4 last bytes of the frames (4 to 7)  

for(k=0; k<8; k++)                                    //Get all the bytes of the frame and put them into 2 long int LOW and HIGH
{
  if(k<4)         
  {
      TRAME_HIGH=TRAME_HIGH<<8;
      TRAME_HIGH=(TRAME_HIGH|f.FRAMEBYTE[7-k]);
  }
  else
  {
      TRAME_LOW=TRAME_LOW<<8;
      TRAME_LOW=(TRAME_LOW|f.FRAMEBYTE[7-k]);      
  }
}        

    switch(byteorder)
    {
     case(0):                                               //MOTOROLA CASE: convert bit by bit
       last_bit_position=-1;
       bit_position=startbit;                               //save the previous bit position
      
       for(k=0; k<length; k++)
       {
            if(last_bit_position==0){bit_position=15;}      //jump when the data is on several bytes
            if(last_bit_position==8){bit_position=23;}
            if(last_bit_position==16){bit_position=31;}
            if(last_bit_position==24){bit_position=39;}
            if(last_bit_position==32){bit_position=47;}
            if(last_bit_position==40){bit_position=55;}
            if(last_bit_position==48){bit_position=63;}
            
           if(bit_position<32)                              //if the taken bit is on 4 first bytes of the frame(0 to 3)
           {
            SIGNALtemp=SIGNALtemp<<1;        
            SIGNALtemp=SIGNALtemp|(TRAME_LOW>>(bit_position))&(0x01);
           }
           else                                            //if the taken bit is on 4 last bytes of the frame(4 to 7)
           {
            SIGNALtemp=SIGNALtemp<<1;        
            SIGNALtemp=SIGNALtemp|(TRAME_HIGH>>(bit_position-32))&(0x01);
           }
           last_bit_position=bit_position;
           bit_position--;                                 //next bit
           
       }
       SIGNAL_HEX=SIGNAL_HEX|SIGNALtemp;                   //build the signal value (hexadecimal)
        
     break;
                                                           //INTEL CASE: TAKE ALL THE DATA IN ONE TIME
     case(1):
      if ((startbit+length)<32)                            //if the taken bytes are on 4 first bytes of the frame(0 to 3)
      {
        SIGNALtemp=SIGNALtemp| TRAME_LOW;
        SIGNAL_HEX=(SIGNALtemp>>(startbit));             
      }

      if (startbit>31)                                     //if the taken bytes are on 4 last bytes of the frame(4 to 7)                            
      {
        SIGNALtemp=SIGNALtemp| TRAME_HIGH;
        SIGNAL_HEX=SIGNALtemp>>(startbit-32);                          
      }
      
      if((startbit<32)&&(startbit+length>31))              //if the taken bytes are on first and last bytes
      {
        SIGNALtemp=SIGNALtemp| TRAME_LOW;
        SIGNAL_HEX=SIGNALtemp>> startbit;  
        SIGNALtemp= 0x00;
        SIGNALtemp=SIGNALtemp|TRAME_HIGH;
        SIGNALtemp=SIGNALtemp<<(length-(length-(32-startbit)));
        SIGNAL_HEX=SIGNAL_HEX|SIGNALtemp;
      }
      
     break;              
    }
    
    for(k=0;k<length; k++)                                     //CUT THE SIGNAL HEX VALUE ACCORDING TO ITS LENGTH
      {                                                        
        MASK=MASK<<1;
        MASK=MASK|0x01;
      } 
      SIGNAL_HEX=SIGNAL_HEX&MASK;
      
    if (type==-1)                                              //CONVERT INTO SIGNED OR UNSIGNED ACCORDING TO ITS BYTE ORDER
    {
         switch(length)
         {
         case (8): SIGNAL_HEX=(char)SIGNAL_HEX;  
         break;
         case(16): SIGNAL_PHYS =(short)SIGNAL_HEX;
         break;
         default: SIGNAL_PHYS =(signed int)SIGNAL_HEX;  
         break;
          
         }
    }
    else
    {
        SIGNAL_PHYS =(unsigned int)SIGNAL_HEX;
    }

    SIGNAL_PHYS=(SIGNAL_PHYS*factor)+offset;                   //CONVERT TO PHYSICAL VALUE ACCORDING TO FACTOR AND OFFSET PARAMETERS
    f.Signal.VALUE=SIGNAL_PHYS;
    
    return (f);
}

/******************************************************************************
* FUNCTION: Build_Frame
*
* Description: Read all the parameters of ONE frame, convert the physical
*               value to hexadecimal value and build the new LAYOUT of the frame
*               
*  THE PREVIOUS FRAME IS PRESERVED, ONLY THE BYTES OF THE SIGNAL ARE MODIFIED
*            
* Arguments: Structure NEWFRAME (composed of all the parameters and data)
*
* Return: the Structure NEWFRAME modified
*          
*******************************************************************************/
NEWFRAME Build_Frame(NEWFRAME newFrame)
{   
    long int     val_hex          = 0x00;
    int          k                = 0;    
    int          last_bit_position= 0;
    int          bit_position     = 0;
    unsigned int Mask_For_Length  = 0x00;
    unsigned int mask             = 0x00;
    int          l                = 0;

//GET STRUCTURE VARIABLES
    int startbit=newFrame.Signal.POSITION;
    int length=newFrame.Signal.LENGTH;
    int byteorder=newFrame.Signal.BYTEORDER;
    float factor=newFrame.Signal.FACTOR;
    float offset=newFrame.Signal.OFFSET;
    int Frame_DLC=newFrame.DLC; 
    float val_phys = newFrame.Signal.VALUE;

  
    ///////DONNEES RECUES PAR CAN///////////
    unsigned int TRAME_LOW =0x00;                           //4 first bytes of the frame (0 to 3)                    
    unsigned int TRAME_HIGH=0x00;                           //4 last bytes of the frames (4 to 7)     
    
    for(k=0; k<8; k++)                                      //Get all the bytes of the frame and put them into 2 long int LOW and HIGH
    {
      if(k<4)
      {
          TRAME_HIGH=TRAME_HIGH<<8;
          TRAME_HIGH=(TRAME_HIGH|newFrame.FRAMEBYTE[7-k]);
      }
      else
      {
          TRAME_LOW=TRAME_LOW<<8;
          TRAME_LOW=(TRAME_LOW|newFrame.FRAMEBYTE[7-k]);      
      }
    }   
    
    unsigned int TRAME_LOWtemp   = 0x00;
    unsigned int TRAME_HIGHtemp  = 0x00;
    
    
    val_hex = int((val_phys-offset)/factor);                //convert HEX value to Physical value
    for(k=0; k<length; k++)
    {
     Mask_For_Length = Mask_For_Length << 1;                //Cut the value according to its length
     Mask_For_Length = Mask_For_Length | 0x01;     
    }
    val_hex = val_hex & Mask_For_Length;
    
    switch(byteorder)
    {
     case(0):                                               //MOTOROLA CASE:  get bit by bit
       last_bit_position=-1;
       bit_position=startbit;
       
       for(k=0; k<length; k++)
       {
            TRAME_LOWtemp=0x00;
            TRAME_HIGHtemp=0x00;
            if(last_bit_position==0){bit_position=15;}      //jump when a signal is on several bytes
            if(last_bit_position==8){bit_position=23;}
            if(last_bit_position==16){bit_position=31;}
            if(last_bit_position==24){bit_position=39;}
            if(last_bit_position==32){bit_position=47;}
            if(last_bit_position==40){bit_position=55;}
            if(last_bit_position==48){bit_position=63;}
            
           if(bit_position<32)
           {
            TRAME_LOWtemp=TRAME_LOWtemp|(val_hex & (0x01<<((length-1)-k)));
            TRAME_LOWtemp=TRAME_LOWtemp>>((length-1)-k);
            TRAME_LOWtemp=TRAME_LOWtemp<<bit_position;
            
            mask=0xFFFFFFFF;                                 //this mask allow to preserve the other bytes of the frame
            mask=mask&(~(1u << (bit_position)));             //only the wanted bits are deleted
            TRAME_LOW=TRAME_LOW&mask;
            TRAME_LOW=TRAME_LOW|TRAME_LOWtemp;
            
            
           }
           else
           {
            TRAME_HIGHtemp=TRAME_HIGHtemp|(val_hex & (0x01<<((length-1)-k)));
            TRAME_HIGHtemp=TRAME_HIGHtemp>>((length-1)-k);
            TRAME_HIGHtemp=TRAME_HIGHtemp<<(bit_position-32);

            mask=0xFFFFFFFF;                                 //this mask allow to preserve the other bytes of the frame
            mask=mask&(~(1u << (bit_position-32)));          //only the wanted bits are deleted
            TRAME_HIGH=TRAME_HIGH&mask;
            TRAME_HIGH=TRAME_HIGH|TRAME_HIGHtemp;
            
           }
           last_bit_position=bit_position;
           bit_position--;
       }
       
     break;
     
     case(1):                                                //INTEL CASE
             
        mask=0xFFFFFFFF;                                     //this mask allow to preserve the other bytes of the frame
        for(l=0; l<length; l++)
        {                                                    //only the wanted bits are deleted
             mask=mask&(~(1u << (startbit+l)));  
        }
        TRAME_LOW=TRAME_LOW&mask;
        
        mask=0xFFFFFFFF;
        for(l=0; l<length; l++)
        {
             mask=mask&(~(1u << ((startbit-32)+l)));  
        }
        TRAME_HIGH=TRAME_HIGH&mask;
            
      if ((startbit+length)<32)                              //data on first 4 bytes (0 to 3)
      {
        TRAME_LOWtemp=TRAME_LOWtemp| val_hex;
        TRAME_LOWtemp=TRAME_LOWtemp<<startbit;    
        TRAME_LOW=TRAME_LOW|TRAME_LOWtemp;                
      }
      
      if (startbit>31)                                    //data on last 4 bytes (4 to 7)                                  
      {
        TRAME_HIGHtemp=TRAME_HIGHtemp| val_hex;
        TRAME_HIGHtemp=TRAME_HIGHtemp<<(startbit-32);    
        TRAME_HIGH=TRAME_HIGH|TRAME_HIGHtemp;           
      }
      
      if((startbit<32)&&(startbit+length>31))           //data on first AND last bytes
      {
        TRAME_LOWtemp=TRAME_LOWtemp| val_hex;
        TRAME_LOWtemp=TRAME_LOWtemp<<startbit; 
        TRAME_HIGHtemp=TRAME_HIGHtemp| val_hex;
        TRAME_HIGHtemp=TRAME_HIGHtemp>>(length-(length-(32-startbit))); 
        TRAME_LOW=TRAME_LOW|TRAME_LOWtemp; 
        TRAME_HIGH=TRAME_HIGH|TRAME_HIGHtemp;  
      }
     break;                
    }


    for (k=0; k<Frame_DLC; k++)                         //CUT THE 2 long int varaiables to create the 8 bytes of the frame
    {

        if (k<4)
        {
         newFrame.FRAMEBYTE[k]= (TRAME_LOW & ((0x000000FF)<<(8*k))) >>(8*k);                         
        }
        else
        {
        newFrame.FRAMEBYTE[k]= (TRAME_HIGH & ((0x000000FF)<<(8*(k-4)))) >>(8*(k-4)); 
        }        
    } 
    
    return newFrame;   
}


/******************************************************************************
* FUNCTION: GetSignal
*
* Description: Search all the parameters and data of a SIGNAL in the lists
*               and save it temporary in the NEWFRAME structure
*            
* Arguments: int frame_ID
*            string SG_NAME  = Name of the wanted Signal
*            Structure NEWFRAME (composed of all the parameters and data)
*
* Return: the Structure NEWFRAME modified
*          
*******************************************************************************/
NEWFRAME GetSignal(int frame_ID, std::string SG_NAME, NEWFRAME newFrame)
{
    int indFrame=-1; 
    int indlisttemp=0; 
    int indSignal=0;           
  
    newFrame.Signal.ID=frame_ID;
    
    newFrame.Signal.STATUS=-1;
    indList=0;
    while((indSignal==0)&&(IT_int!=lstSG_ID.end()))
    {
        for(IT_int=lstSG_ID.begin();IT_int!=lstSG_ID.end();IT_int++)     //SEARCH ID OF THE FRAME CONTAINING THE WANTED SIGNAL
        {
           if (*IT_int==frame_ID)                                  
           {
               newFrame.Signal.STATUS=0;
               
               for(IT_str=lstSG_NAME.begin();IT_str!=lstSG_NAME.end();IT_str++)   //SEARCH NAME OF THE WANTED SIGNAL
               {                                                               
                  if((indlisttemp==indList) && (SG_NAME==*IT_str))
                  {
                      indSignal=indList; 
                      newFrame.Signal.NAME=*IT_str;
                      newFrame.Signal.STATUS=0;
                  }
                  indlisttemp++;
               }
           }
            indlisttemp=0;
            indList++;   
        }        
    }
////////////////////////// SEARCH BYTE ORDER ///////////////////////////////
    newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_int=lstSG_BYTEORDER.begin();IT_int!=lstSG_BYTEORDER.end();IT_int++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.BYTEORDER=*IT_int;}
      indList++;
    }

 /////////////////////////////// SEARCH FACTOR ///////////////////////////////////////// 
    newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_float=lstSG_FACTOR.begin();IT_float!=lstSG_FACTOR.end();IT_float++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.FACTOR=*IT_float;}
      indList++;
    }
 ////////////////////////////// SEARCH OFFSET //////////////////////////////////////////////// 
    newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_float=lstSG_OFFSET.begin();IT_float!=lstSG_OFFSET.end();IT_float++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.OFFSET=*IT_float;}
      indList++;
    }
 ////////////////////////////// SEARCH LENGTH //////////////////////////////////////////////// 
    newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_int=lstSG_LENGTH.begin();IT_int!=lstSG_LENGTH.end();IT_int++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.LENGTH=*IT_int;}
      indList++;
    }
 ////////////////////////////// SEARCH POSITION //////////////////////////////////////////////// 
    newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_int=lstSG_POSITION.begin();IT_int!=lstSG_POSITION.end();IT_int++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.POSITION=*IT_int;}
      indList++;
    }
 ///////////////////////////////SEARCH MIN /////////////////////////////////////////////// 
    newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_float=lstSG_MIN.begin();IT_float!=lstSG_MIN.end();IT_float++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.MIN=*IT_float;}
      indList++;
    }
 //////////////////////////////SEARCH MAX //////////////////////////////////////////////// 
     newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_float=lstSG_MAX.begin();IT_float!=lstSG_MAX.end();IT_float++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.MAX=*IT_float;}
      indList++;
    }
 /////////////////////////////// SEARCH TYPE /////////////////////////////////////////////// 
    newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_int=lstSG_TYPE.begin();IT_int!=lstSG_TYPE.end();IT_int++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.TYPE=*IT_int;}
      indList++;
    }
 //////////////////////////////// SEARCH UNIT ////////////////////////////////////////////// 
     newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_str=lstSG_UNIT.begin();IT_str!=lstSG_UNIT.end();IT_str++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.UNIT=*IT_str;}
      indList++;
    }
 /////////////////////////////// SEARCH RECEIVERS /////////////////////////////////////////////// 
     newFrame.Signal.STATUS=-1;
    indList=0;
    for(IT_str=lstSG_RECEIVERS.begin();IT_str!=lstSG_RECEIVERS.end();IT_str++)
    {
      if(indList==indSignal){newFrame.Signal.STATUS=0;newFrame.Signal.RECEIVERS=*IT_str;}
      indList++;
    }
 return newFrame; 
}


/******************************************************************************
* FUNCTION: GetFrame
*
* Description: Search all the parameters and data of a FRAME and save it temporary
*              in the NEWFRAME structure
*            
* Arguments: int frame_ID
*            Structure NEWFRAME (composed of all the parameters and data)
*
* Return: the Structure NEWFRAME modified
*          
*******************************************************************************/
NEWFRAME GetFrame(int frame_ID, NEWFRAME newFrame)
{
    int indFrame=-1;             

    newFrame.ID=frame_ID;
    
    newFrame.STATUS=-1;
    //SEARCH ID FRAME AND GET INDLIST FOR OTHER PARAMETERS//
    indList=0;
    for(IT_int=lstFRAME_ID.begin();IT_int!=lstFRAME_ID.end();IT_int++)
    {
       if (*IT_int==frame_ID){newFrame.STATUS=0;indFrame=indList;}
        indList++;
    }
    
    newFrame.STATUS=-1;
    /////////////////////// SEARCH NAME FRAME////////////////////////////////////
    indList=0;
    for(IT_str=lstFRAME_NAME.begin();IT_str!=lstFRAME_NAME.end();IT_str++)
    {
      if(indList==indFrame){newFrame.STATUS=0;newFrame.NAME=*IT_str;}
      indList++;
    }
    
    newFrame.STATUS=-1;
    /////////////////////// SEARCH DLC FRAME////////////////////////////////////
    indList=0;
    for(IT_int=lstFRAME_DLC.begin();IT_int!=lstFRAME_DLC.end();IT_int++)
    {
      if(indList==indFrame){newFrame.STATUS=0;newFrame.DLC=*IT_int;}
      indList++;
    }

    newFrame.STATUS=-1;
    /////////////////////// SEARCH TRANSMITTER FRAME////////////////////////////////////
    indList=0;
    for(IT_str=lstFRAME_TRANSMITTER.begin();IT_str!=lstFRAME_TRANSMITTER.end();IT_str++)
    {
      if(indList==indFrame){newFrame.STATUS=0;newFrame.TRANSMITTER =*IT_str;}
      indList++;
    }
    return newFrame;
}


/******************************************************************************
* FUNCTION: save_Signals_Parameters
*
* Description: Save all the parameters of a signal in Lists (one list by parameter).
               Each Signal is associated
*              to the ID of the frame
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void save_Signals_Parameters()
{
  Associate_IDFrame_To_Signal();
  GetAndShow_Signal_NAME();
  GetAndShow_Signal_Position_And_Lenght();
  GetAndShow_Signal_ByteOrder_And_Type();
  GetAndShow_Signal_Conversion();
  GetAndShow_Signal_MinMax_Values();
  GetAndShow_Signal_Unit();
  GetAndShow_Signal_Receivers();     
}


/******************************************************************************
* FUNCTION: save_Frames_Parameters
*
* Description: Save all the parameters of a FRAME in Lists(one list by parameter) 

* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void save_Frames_Parameters()
{
    GetAndShow_Message_ID();
    GetAndShow_Message_Name();
    GetAndShow_Message_DLC();
    GetAndShow_Message_Transmitter();
}


/******************************************************************************
* FUNCTION: Parse_DBC_File
*
* Description: Open a dbc file, scan it and save all the frames and signals 
*               parameters in lists 
*
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
void Parse_DBC_File()
{
    std::ifstream strDBC_FILE(FilePath);                                   // Open dbc file
   
    if(strDBC_FILE )                                                       // if dbc file open
    {
        i=0;
    
        while( std::getline( strDBC_FILE, ligne ))                         // get line by line
        {
             init_Values();                                                // init the storage variables
             foundFrame = ligne.find(strBO);                               //search frame in line 
             foundSignal = ligne.find(strSG);                              //search signal in line
             foundPonct = ligne.find(strPonct);                            // search ':' in line
             
             if ((foundFrame!=std::string::npos)&&(foundPonct!=std::string::npos))   //If frame found 
             {
                   for(j=foundFrame-1; j>=0; j--)
                   {
                         cCAR=ligne[j];
                           
                         if((cCAR.compare(" ")!=0)&&(cCAR.compare("\t")!=0))         //check if it's really a frame
                         {                                  
                           MESSAGE_CHECKING=1;
                         }
                   }
                             
                   if(MESSAGE_CHECKING!=1)                                 //save the frame parameters
                   {                     
                     save_Frames_Parameters();
                     INDEX_FRAME++;                                                        
                   }                
             } 
               
             if ((foundSignal!=std::string::npos)&&(foundPonct!=std::string::npos)) //Signal found
              {       
                   for(j=foundSignal-1; j>=0; j--)                                  //check if it's really a signal
                   {
                         cCAR=ligne[j];
                         if((cCAR.compare(" ")!=0)&&(cCAR.compare(" ")!=0))
                         {                                  
                           SIGNAL_CHECKING=1;
                         }
                   }
                      
                   if (SIGNAL_CHECKING!=1)
                   {          
                         save_Signals_Parameters();                                 //save the signal parameters                                          
                   }
              }
            i++;
       }               
    }                     
}


/******************************************************************************
* FUNCTION: convert_ONE_Signal_to_Physical_Value
*
* Description: for only ONE Signal, convert the hexadecimal value contained 
*              in Frame to its physical value. 
*
* Arguments: int frame_ID
*            string Signal_Name (name of the signal to convert)
*            NEWFRAME newFrame (structure composed of all th eframe and signals parameters)
*
* Return:  the structure NEWFRAME modified
*          
*******************************************************************************/
NEWFRAME convert_ONE_Signal_to_Physical_Value(int frame_ID, std::string Signal_Name, NEWFRAME newFrame)
{
     newFrame = GetFrame(frame_ID, newFrame);
     newFrame = GetSignal(frame_ID, Signal_Name, newFrame);
     newFrame = ReadOneSignal(newFrame); 
     return newFrame;
}


/******************************************************************************
* FUNCTION: modify_ONE_Signal_Value_And_Build_layout
*
* Description: for only ONE Signal, convert the physical value to hexadecimal, 
*               and build the new layout value contained 
*
* Arguments: int frame_ID
*            string Signal_Name (name of the signal to convert)
*            float Physical_Value (value to convert to hexadecimal value)
*            NEWFRAME newFrame (structure composed of all th eframe and signals parameters)
*
* Return:  the structure NEWFRAME modified
*          
*******************************************************************************/
NEWFRAME modify_ONE_Signal_Value_And_Build_layout(int frame_ID, std::string Signal_Name, float Physical_Value, NEWFRAME newFrame)
{
     newFrame = GetFrame(frame_ID, newFrame);
     newFrame = GetSignal(frame_ID, Signal_Name, newFrame);
     newFrame.Signal.VALUE=Physical_Value;
     newFrame =  Build_Frame(newFrame);
     return newFrame;    
}


/******************************************************************************
* FUNCTION: main
*
* Description: this main tests the read and write functions.
*               1. INIT: parse DBC file
*               2. Convert a physical value to hexadecimal and build the frame
*               3. Read this frame and convert the hexadecimal value to physical
*               4. display the results
* Arguments: /
*
* Return: /
*          
*******************************************************************************/
int main()
{
     
     NEWFRAME frame;
     
     /////// INIT/ ENREGISTREMENT DES DONNES CAN//////
     Parse_DBC_File();
     
     
    /////DONNEES QUE L'ON SOUHAITE CONVERTIR ET LIRE///
    int frame_ID=196;
    std::string SG_Name = "U_CEL_MIN_BMS";
    //// CE QUE L'ON SOUHAITE ECRIRE DANS LA TRAME ////
    float Physical_Value=18000;

     
    /////////SIMULE UNE LECTURE TRAME CAN (FONCTION LECTURE TRAME A CREER)////////////
      frame.FRAMEBYTE[0]=0x00;
      frame.FRAMEBYTE[1]=0x00;
      frame.FRAMEBYTE[2]=0x00;
      frame.FRAMEBYTE[3]=0x00;
      frame.FRAMEBYTE[4]=0x00;
      frame.FRAMEBYTE[5]=0x00;
      frame.FRAMEBYTE[6]=0x00;
      frame.FRAMEBYTE[7]=0x00;

     
     ////// MODIFIER VALEUR SIGNAL ET CONSTRUIRE LE LAYOUT ///////////
     printf("\n\n\n BUILD LAYOUT ");
     frame = modify_ONE_Signal_Value_And_Build_layout(frame_ID, SG_Name, Physical_Value, frame);
     
      ////// AFFICHAGE DU LYOUT DE LA TRAME //////
     for(i=0; i<frame.DLC; i++)
     {
      printf("\n FRAME BYTE %i    = 0x%X",i,frame.FRAMEBYTE[i]);
     }
     
          //// CONVERSION DU SIGNAL EN VALEUR PHYSIQUE///
     frame = convert_ONE_Signal_to_Physical_Value(frame_ID, SG_Name, frame);
     
     //////// AFFICHAGE DES PARAMETRES DE LA TRAME ///////////////////
     printf("\n\nREAD SIGNAL ");
     printf("\n SIGNAL NAME     = %s", frame.Signal.NAME.c_str());
     printf("\n SIGNAL FACTOR   = %.2f", frame.Signal.FACTOR);
     printf("\n SIGNAL OFFSET   = %.2f", frame.Signal.OFFSET);
     printf("\n SIGNAL POSITION = %i", frame.Signal.POSITION);
     printf("\n SIGNAL LENGTH   = %i", frame.Signal.LENGTH);
     printf("\n SIGNAL ID       = %i", frame.Signal.ID);
     printf("\n SIGNAL DLC      = %i", frame.DLC);
     printf("\n SIGNAL TYPE     = %i", frame.Signal.TYPE);
     printf("\n SIGNAL UNIT     = %s", frame.Signal.UNIT.c_str());
     printf("\n SIGNAL BYTEORDER= %i", frame.Signal.BYTEORDER);
     printf("\n SIGNAL MAX      = %.2f", frame.Signal.MAX);
     printf("\n SIGNAL MIN      = %.2f", frame.Signal.MIN);
     printf("\n SIGNAL RECEIVERS= %s", frame.Signal.RECEIVERS.c_str());
     printf("\n SIGNAL VALUE    = %.2f", frame.Signal.VALUE);
     printf("\n");
   
}

