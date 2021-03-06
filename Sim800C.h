/*
 *	SOFTWARE SERIAL NOTES:
 *
 *		PINOUT
 *		The library has the following known limitations:
 *		1. If using multiple software serial ports, only one can receive data at a time.
 *		2. Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be used for RX: 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).
 *		3. Not all pins on the Leonardo and Micro support change interrupts, so only the following can be used for RX: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 *		4. On Arduino or Genuino 101 the current maximum RX speed is 57600bps
 *		5. On Arduino or Genuino 101 RX doesn't work on Pin 13
 *
 *		BAUD RATE
 *		Supported baud rates are 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, and 115200.
*/

#ifndef Sim800C_h
#define Sim800C_h
#include <SoftwareSerial.h>
#include "Arduino.h"

#define SwSerial  SoftwareSerial
//#define HwSerial  Serial

#define DEFAULT_RX_PIN      10
#define DEFAULT_TX_PIN 		11
#define DEFAULT_POWER_PIN 	2		// pin to the reset pin Sim800C


#define BUFFER_RESERVE_MEMORY	255
#define DEFAULT_BAUD_RATE		9600
#define TIME_OUT_READ_SERIAL	5000

#define ERROR   0
#define OK      1

#define RESPON_OK		"OK\r\n"

#define RESPON_ERROR	"ERROR\r\n"

#define ctrlz 26 //Ascii character for ctr+z. End of a SMS.
#define cr    13 //Ascii character for carriage return.
#define lf    10 //Ascii character for line feed.

#define network_registered1	"0,1" //"+CREG: 0,1"

#define network_registered2	"0,5" //"+CREG: 0,5"

#define No_data               0
#define Calling_with_number   1
#define Sms_received          2
#define NOT_Recog_Data        4
#define RING                  5
#define CUSD				  6

#define NoSMS                 255

enum registration_ret_val_enum
{
	REG_NOT_REGISTERED = 0,
	REG_REGISTERED     = 1,
	REG_NO_RESPONSE    = 2,
	REG_COMM_LINE_BUSY = 3,
	
	REG_LAST_ITEM
};

enum call_status
{
	NO_ANSWER	=7,
	NO_DIALTONE =8,
	BUSY        =9,
	NO_CARRIER  =10,
	CONNECT     =11,
	MO_RING     =12,
	MO_CONNECTED=13
};

enum WhiteList
{
	Disable	=0,
	Enable_only_call =1,
	Enable_only_SMS =2,
	Enable_call_and_SMS =3
};

enum getsms_ret_val_enum
{
	GETSMS_NO_SMS       = 2,
	GETSMS_UNREAD_SMS   = 3,
	GETSMS_READ_SMS     = 4,
	GETSMS_OTHER_SMS    = 5,
	GETSMS_NOT_AUTH_SMS = 6,
	GETSMS_AUTH_SMS     = 7,

	GETSMS_LAST_ITEM
};

class Sim800C
{
private:

    uint32_t _baud;
    int _timeout;
    bool _sleepMode;
    uint8_t _functionalityMode;

    String _readSerial();
    String _readSerial(uint32_t timeout);

    bool send_cmd_wait_reply(String aCmd,const char*aResponExit,uint32_t aTimeoutMax);
    bool send_cmd_wait_reply(const __FlashStringHelper *aCmd,const char*aResponExit,uint32_t aTimeoutMax);

public:

    uint8_t sms_index=NoSMS;
    String SimBuffer;

    Sim800C(void);

    void begin();					//Default baud 9600
    void begin(uint32_t baud);
    void PowerOn();
    void PowerOff();
    void reset();

    uint8_t Setup(void);

    uint8_t is_network_registered();

    bool setSleepMode(bool state);
    bool getSleepMode();
    bool setFunctionalityMode(uint8_t fun);
    uint8_t getFunctionalityMode();

    bool setPIN(String pin);
    String getProductInfo();

    String getOperatorsList();
    String getOperator();

    bool answerCall();
    bool callNumber(String number);
    bool hangoffCall();
    uint8_t getCallStatus();

    bool sendSms(char* number,char* text);
    uint8_t readSms(uint8_t index, char * phone_number, char * SMS_text);
    bool deleteSMS(uint8_t position);
    bool delAllSms();

    bool AddToWhiteList(uint8_t Command,uint8_t index,char * PhoneNumber); //index=1-30
    uint8_t whiteListStatus(char * PhoneNumbers);
    bool miss_call(String aSenderNumber,uint8_t NumOfTry);

    uint8_t check_receive_command(void);

    String signalQuality();
    void setPhoneFunctionality();

    void RTCtime(int *day,int *month, int *year,int *hour,int *minute, int *second);
    String dateNet();

};

#endif
