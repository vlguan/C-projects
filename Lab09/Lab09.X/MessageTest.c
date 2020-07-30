/*
 * File:   MessageTest.c
 * Author: vince
 *
 * Created on March 8, 2020, 9:54 PM
 */


#include "xc.h"
#include "stdio.h"
#include "Message.h"
#include "BOARD.h"
#include "BattleBoats.h"
#include "string.h"

int main()
{
    BOARD_Init();
    //checksums
    printf("CheckSum test:\n");
    char *correctload = "RES,1,0,3";
    uint8_t sum = Message_CalculateChecksum(correctload);
    printf("%02X", sum);
    char sumstr[MESSAGE_CHECKSUM_LEN];
    sprintf(sumstr, "%02X", sum);
    char *correct = "5A";
    if (strcmp(sumstr, correct) == 0) printf("PASS\n");

    else printf("FAIL\n");

    //PARSE
    printf("Parse test:\n");
    char *cs = "5A";                            //checksum string
    BB_Event event;
    int success = Message_ParseMessage(correctload, cs, &event);    //check if everything matches
    if (success == SUCCESS && event.type == BB_EVENT_RES_RECEIVED && event.param0 == 1 &&
            event.param1 == 0 && event.param2 == 3)
    {
        printf("Passed\n");
    }
    else
    {
        printf("Failed\n");
    }
    
    //test encode
    char storeMessage[MESSAGE_MAX_LEN];
    Message m;
    m.type = MESSAGE_REV;
    m.param0 = 2;
    int length = Message_Encode(storeMessage, m);
    printf("%s", storeMessage);
    if (length == 10)
    {
        printf("Passed\n");
    }
    else
    {
        printf("Failed\n");
    }
    
    //ENCODE
    printf("Encode test:\n");
    char strmsg[MESSAGE_MAX_LEN];
    Message x;
    x.type = MESSAGE_REV;
    x.param0 = 2;
    int len = Message_Encode(strmsg, x);
    printf("%s", strmsg);
    if (len == 10) printf("PASS\n");
    else printf("FAIL\n");
    //DECODE
    printf("Decode test:\n");
    char enigma[MESSAGE_MAX_LEN];
    char *str = "$REV,1*5C\n";
    BB_Event event2;
    enigma[0] = '$';
    enigma[1] = 'R';
    enigma[2] = 'E';
    enigma[3] = 'V';
    enigma[4] = ',';
    enigma[5] = '1';
    enigma[6] = '*';
    enigma[7] = '5';
    enigma[8] = 'C';
    enigma[9] = '\n';
    int i = 0;
    while (enigma[i] != '\0') {
        int solve = Message_Decode(enigma[i], &event2);
        if (solve == SUCCESS) printf("%d passpt1\n", i);
        else printf("%d FAIL\n", i);
        i++;
    }

    if (event2.type == BB_EVENT_REV_RECEIVED && event2.param0 == 1) printf("PASS\n");

    else printf("FAIL\n");

    strcpy(enigma, "");
    printf("msg is %s\n", enigma);
    strcpy(enigma, "$SHO,5,3*52\n");
    printf("decoded string is %s\n", enigma);
    i = 0;
    while (enigma[i] != '\0') {
        int solve = Message_Decode(enigma[i], &event2);
        if (solve == SUCCESS) printf("%d passpt1\n", i);
        else printf("%d FAIL\n", i);
        i++;
    }
    printf("event is %d\n", event2.type);
    return 0;





}
