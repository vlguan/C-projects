/*
 * File:   AgentTest.c
 * Author: vince
 *
 * Created on March 8, 2020, 9:50 PM
 */


#include "xc.h"
#include <stdio.h>
#include "Agent.h"
#include "BOARD.h"
#include "Negotiation.h"
#include "BattleBoats.h"
#include "FieldOled.h"
#include "Oled.h"
#include "OledDriver.h"

//variable
#define ALLBIT 0xFFFF
int i = 0;

int main(void)
{
    BOARD_Init();
    AgentInit();
    int challengehead = 0, challengetail = 0, acceptinghead = 0, acceptingtail = 0;
    int i;
    BB_Event event;
    NegotiationData X;
    NegotiationData table;
    printf("Agent test harness\n");
    printf("Agent init test:\n");
    AgentInit();
    if (AgentGetState() == AGENT_STATE_START) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Agent Run test:\n");
    printf("running 100 cases to see if works\n");
    //iterates 100 cases
    for (i = 0; i < 100; i++) {

        AgentInit();
        if ((rand() & 1) == 1) { 
            //challenge
            event.type = BB_EVENT_START_BUTTON;
            AgentRun(event);
            event.type = BB_EVENT_ACC_RECEIVED;
            event.param0 = rand() & ALLBIT;
            AgentRun(event);
            if (AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
                challengehead++;
            } else if (AgentGetState() == AGENT_STATE_DEFENDING) {
                challengetail++;
            } else {
                printf("check challenge state\n");
            }
        } else { 
            //accepting
            X = rand() & ALLBIT;
            table = NegotiationHash(X);
            event.type = BB_EVENT_CHA_RECEIVED;
            event.param0 = table;
            AgentRun(event);
            event.type = BB_EVENT_REV_RECEIVED;
            event.param0 = X;
            AgentRun(event);
            if (AgentGetState() == AGENT_STATE_DEFENDING) {
                acceptinghead++;
            } else if (AgentGetState() ==  AGENT_STATE_ATTACKING){
                acceptingtail++;
            } else {
                printf("check accepting state\n ");
                acceptingtail++;
            }
        }
        
    }
    //sees if its actually heads or tails
    printf("CHeads:%d\nCTails:%d\nAHeads:%d\nATails:%d\n",challengehead, challengetail, acceptinghead, acceptingtail);
    printf("answer: about 25 each +/- 5\n");
    printf("AgentRun: \n");
    X = rand() & ALLBIT;
    //tests if cheating works
    table = NegotiationHash(X) + 1;
    event.type = BB_EVENT_CHA_RECEIVED;
    event.param0 = table;
    AgentRun(event);
    event.type = BB_EVENT_REV_RECEIVED;
    event.param0 = X;
    AgentRun(event);
    if (AgentGetState() == AGENT_STATE_DEFENDING) {
        printf("fail test, sent to defending\n");
    } else if (AgentGetState() == AGENT_STATE_ATTACKING) {
        printf("fail test, sent to attack\n");
    } else {
        printf("Caught cheating, pass test\n");
    }
    while (1);

}
    