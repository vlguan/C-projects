/*
 * File:   newmainXC16.c
 * Author: vince
 *
 * Created on March 12, 2020, 8:55 PM
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

int main(void)
{
    BOARD_Init();
    AgentInit();

    int chaHeads = 0, chaTails = 0, accHeads = 0, accTails = 0;
    int i;
    BB_Event event;
    //    Message message;
    NegotiationData A;
    NegotiationData Hash;
    printf("Agent.c test harness\n");
    printf("Testing AgentInit():\n");
    AgentInit();
    if (AgentGetState() == AGENT_STATE_START) {
        printf("AgentInit() passed 1/1 basic test. Further testing will be done in unison with ");
        printf("AgentRun():\n");
    }

    printf("Testing for uniform paths: 100 cases (May take some time)\n");
    for (i = 0; i < 100; i++) {
        AgentInit();
        if ((rand() & 1) == 1) { // Cha
            event.type = BB_EVENT_START_BUTTON;
            AgentRun(event);
            event.type = BB_EVENT_ACC_RECEIVED;
            event.param0 = rand() & 0xFFFF;
            AgentRun(event);
            if (AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
                chaHeads++;
            } else if (AgentGetState() == AGENT_STATE_DEFENDING) {
                chaTails++;
            } else {
                printf("Error in AgentTest.c, cha\n");
            }
        } else { // Acc
            A = rand() & 0xFFFF;
            Hash = NegotiationHash(A);
            event.type = BB_EVENT_CHA_RECEIVED;
            event.param0 = Hash;
            AgentRun(event);
            event.type = BB_EVENT_REV_RECEIVED;
            event.param0 = A;
            AgentRun(event);
            if (AgentGetState() == AGENT_STATE_DEFENDING) {
                accHeads++;
            } else if (AgentGetState() == AGENT_STATE_ATTACKING) {
                accTails++;
            } else {
                printf("Error in AgentTest.c, acc %d\n", AgentGetState());
                accTails++;
            }
        }
    }
    printf("Challenger Heads: %d\nChallenger Tails: %d\nAccepting Heads: %d\nAccepting Tails: %d\n",
            chaHeads, chaTails, accHeads, accTails);
    printf("Should be around 25 each, give or take ~5\n");
    printf("Testing AgentRun() mismatching secret keys:\n");
    A = rand() & 0xFFFF;
    Hash = NegotiationHash(A) + 1;
    event.type = BB_EVENT_CHA_RECEIVED;
    event.param0 = Hash;
    AgentRun(event);
    event.type = BB_EVENT_REV_RECEIVED;
    event.param0 = A;
    AgentRun(event);
    if (AgentGetState() == AGENT_STATE_DEFENDING) {
        printf("Went to defending state, fail test\n");
    } else if (AgentGetState() == AGENT_STATE_ATTACKING) {
        printf("Went to attacking state, fail test\n");
    } else {
        printf("Caught cheating, pass test\n");
    }
    while (1);
}