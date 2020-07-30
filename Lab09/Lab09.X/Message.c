/*
 * File:   Message.c
 * Author: vince
 *
 * Created on March 8, 2020, 9:55 PM
 */


#include "xc.h"
#include "Message.h"
#include "BOARD.h"
#include "string.h"
#include "stdio.h"

typedef enum {
    WAITING,
    PAYLOAD,
    CHECKSUM
} State;
static State x = WAITING;
static char checkSumstr[MESSAGE_CHECKSUM_LEN];
static char payloadStr[MESSAGE_MAX_PAYLOAD_LEN];


//helper functions

/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */
uint8_t Message_CalculateChecksum(const char* payload)
{
    uint8_t num = payload[0];
    //calculates the checksum
    int i;
    //increments to the length of the string
    for (i = 1; i < strlen(payload); i++) {
        //xor char in string
        num = num ^ payload[i];
    }
    return num;
}

/**
 * Given a payload and its checksum, verify that the payload matches the checksum
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //An event corresponding to the parsed message.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event)
{
    //sends the AGENT the msgs
    message_event->type = BB_EVENT_NO_EVENT;
    //standard errors
    if (strlen(checksum_string) != 2 || strlen(payload) > MESSAGE_MAX_PAYLOAD_LEN)
    {
        return STANDARD_ERROR;              
    }
    
    //gets checksum
    uint8_t checkSum = Message_CalculateChecksum(payload);      
    char checkSumStr[MESSAGE_CHECKSUM_LEN];
    sprintf(checkSumStr, "%02X", checkSum);                 
    if (strcmp(checkSumStr, checksum_string) != 0)   return STANDARD_ERROR;                                  
    
    char TMP[MESSAGE_MAX_PAYLOAD_LEN];
    strcpy(TMP, payload);                           
    char *messageType = strtok(TMP, ",");           

    //check if the token is equal to the event
    if (strcmp(messageType, "CHA") == 0)
    {
        //tokenize data in payload string
        //printf("CHA\n");
        char *data0 = strtok(NULL, ",");                    
        message_event->param0 = atoi(data0);        
        //turn in to unsigned int        
        message_event->type = BB_EVENT_CHA_RECEIVED;                 
    }
    else if (strcmp(messageType, "ACC") == 0)                
    {
        // do it again 
        //printf("ACC\n");
        char *data0 = strtok(NULL, ",");
        message_event->param0 = atoi(data0);
        message_event->type = BB_EVENT_ACC_RECEIVED;
    }
    else if (strcmp(messageType, "REV") == 0)
    {
        //printf("REV\n");
        char *data0 = strtok(NULL, ",");
        message_event->param0 = atoi(data0);
        message_event->type = BB_EVENT_REV_RECEIVED;
    }
    else if (strcmp(messageType, "SHO") == 0)
    {
        //printf("SHO\n");
        char *data0 = strtok(NULL, ",");             
        char *data1 = strtok(NULL, ",");
        message_event->param0 = atoi(data0);
        message_event->param1 = atoi(data1);
        message_event->type = BB_EVENT_SHO_RECEIVED;
    }
    else if (strcmp(messageType, "RES") == 0)
    {
        //printf("RESSS\n");
        char *data0 = strtok(NULL, ",");                        
        char *data1 = strtok(NULL, ",");                        
        char *data2 = strtok(NULL, ",");        
        //payload data
        message_event->param0 = atoi(data0);
        message_event->param1 = atoi(data1);
        message_event->param2 = atoi(data2);
        message_event->type = BB_EVENT_RES_RECEIVED;
    }  
    else
    {  //no matches
        message_event->type = BB_EVENT_NO_EVENT;                
        return STANDARD_ERROR;
    }
    return SUCCESS;
}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 */
int Message_Encode(char *message_string, Message message_to_encode){
    if (message_to_encode.type == MESSAGE_NONE) return 0;  //aha ha no messages 
    char temp[MESSAGE_MAX_PAYLOAD_LEN];
    if (message_to_encode.type == MESSAGE_ACC) sprintf(temp, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
    //checks acc
    if (message_to_encode.type == MESSAGE_CHA) sprintf(temp, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
// check cha
    if (message_to_encode.type == MESSAGE_RES) sprintf(temp, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1,
            message_to_encode.param2);
//checks res
    if (message_to_encode.type == MESSAGE_SHO) sprintf(temp, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
//checks sho
    if (message_to_encode.type == MESSAGE_REV) sprintf(temp, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
    //checks rev
uint8_t checkSum = Message_CalculateChecksum(temp);
sprintf(message_string, MESSAGE_TEMPLATE, temp, checkSum);
int length = strlen(message_string);
return length;
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                              otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event *decoded_message_event)
{
    switch (x) {
    case WAITING:
        memset(payloadStr, NULL, sizeof (payloadStr));
        memset(checkSumstr, NULL, sizeof (checkSumstr));
        if (char_in == '$') x = PAYLOAD;
        break;
    case PAYLOAD:
        if (char_in == '$' || char_in == '\n') {
            x = WAITING;
            return STANDARD_ERROR;
        }
        if (strlen(payloadStr) > MESSAGE_MAX_PAYLOAD_LEN) {
            x = WAITING;
            return STANDARD_ERROR;
        }
        if (char_in == '*') {
            x = CHECKSUM;
        } else sprintf(payloadStr, "%s%c", payloadStr, char_in);
        break;


    case CHECKSUM:
        if (char_in == '\n') {
            int load = Message_ParseMessage(payloadStr, checkSumstr, decoded_message_event);
            if (load == SUCCESS) {
                x = WAITING;
                return SUCCESS;
            } else {
                x = WAITING;
                return STANDARD_ERROR;
            }
        }
        if (strlen(checkSumstr) > MESSAGE_CHECKSUM_LEN) {
            x = WAITING;
            return STANDARD_ERROR;
        }
        //could be problem change later
        switch (char_in) {
            //checks the characters
        case '0':case '1':case '2':case '3':case '4':case '5':
        case '6':case '7':case '8':case '9':case 'A':case 'B':
        case 'C':case 'D':case 'E':case 'F':
            sprintf(checkSumstr, "%s%c", checkSumstr, char_in);
            break;
        default:
            x = WAITING;
            return STANDARD_ERROR;
            break;

        }

    }
    return SUCCESS;
}