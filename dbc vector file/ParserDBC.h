

#ifndef ParserDBC_h
#define ParserDBC_h

//-----------      STRUCTURES      ------------//
typedef struct signal              
{
    int ID;                        //ID of the frame containing the signal
    std::string NAME;              //Name of the signal
    int POSITION;                  //Start bit of the signal in the frame
    int LENGTH;                    //length in bits of the signal
    int BYTEORDER;                 //byteorder of the signal (INTEL or MOTOROLA)
    int TYPE;                      //type of the signal (SIGNED or UNSIGNED)
    float FACTOR;                  //factor to convert the signal to physical value
    float OFFSET;                  //offset to convert the signal to physical value
    float MIN;                     //minimum physical value of the signal
    float MAX;                     //maximum physical value of the signal
    std::string UNIT;              //unit of the signal
    std::string RECEIVERS;         //all the receivers of the signal
    int STATUS;                    //unused    
    float VALUE;                   //physical value of teh signal
} SIGNAL;

typedef struct newframe
{
        SIGNAL Signal;             //the signal structure  is contained in the frame strcuturez
        int ID;                    //frame ID
        std::string NAME;          //Name of the frame
        int DLC;                   //DLC of the frame (numberf of bytes)
        std::string TRANSMITTER;   //name of the frame transmitter
        int STATUS;                //status of the frame
        int FRAMEBYTE[8];          //data bytes of the frame

} NEWFRAME;    
//-----------     END STRUCTURES   ------------//

void init_Values();

void save_Frames_Parameters();
void save_Signals_Parameters();

void Associate_IDFrame_To_Signal();
void GetAndShow_Signal_Unit();
void GetAndShow_Signal_MinMax_Values();
void GetAndShow_Signal_Conversion();
void GetAndShow_Signal_ByteOrder_And_Type();
void GetAndShow_Signal_Position_And_Lenght();
void GetAndShow_Signal_NAME();

void GetAndShow_Message_DLC();
void GetAndShow_Message_NAME();
void GetAndShow_Message_ID();
void GetAndShow_Message_Transmitter();

NEWFRAME Build_Frame(NEWFRAME newFrame);
NEWFRAME ReadOneSignal(NEWFRAME sg);
NEWFRAME GetSignal(int frame_ID, std::string SG_NAME, NEWFRAME newFrame);
NEWFRAME GetFrame(int frame_ID, NEWFRAME newFrame);

///////HIGH LEVEL FUNCTIONS///////////////
void Parse_DBC_File();
NEWFRAME convert_ONE_Signal_to_Physical_Value(int frame_ID, std::string Signal_Name, NEWFRAME newFrame);
NEWFRAME modify_ONE_Signal_Value_And_Build_layout(int frame_ID, std::string Signal_Name, float Physical_Value, NEWFRAME newFrame);
//////////////////////////////////////////
int main();

#endif
