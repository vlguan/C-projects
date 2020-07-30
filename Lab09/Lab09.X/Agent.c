/*
 * File:   Agent.c
 * Author: vince
 *
 * Created on March 8, 2020, 9:55 PM
 */


#include "xc.h"
#include "Agent.h"
#include "Field.h"
#include "Negotiation.h"
#include "FieldOled.h"
#include <stdio.h>
#include "Oled.h"
#include "OledDriver.h"
//end states

enum {
    END_CHEAT = 1,
    END_DEFEAT,
    END_VICTORY,
};
//variables
static AgentState State;
//security key a
static NegotiationData A;
//security key b
static NegotiationData B;
static NegotiationData Table;
static int turn = 0;
static Field FieldA; 
static Field FieldB;
static int End;
#define ALLBIT 0xFFFF
//helper functions

void victory(void)
{
    OledClear(OLED_COLOR_BLACK);
    OledDrawString("Victory is yours!");
    OledUpdate();
}

void defeat(void)
{
    OledClear(OLED_COLOR_BLACK);
    OledDrawString("Defeat try again");
    OledUpdate();
}

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *      Initializing both fields
 *      Placing boats on your field
 *      Setting the start state of the Agent SM
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * The agent can assume that stdlib's rand() function has been seeded properly, so it is not needed
 * to call srand() inside AgentInit().
 *  */
void AgentInit(void)
{
    OledInit();
    State = AGENT_STATE_START;
    FieldInit(&FieldA, &FieldB);
    FieldAIPlaceAllBoats(&FieldA);
    A = 0; //
    B = 0; //
    Table = 0;
    turn = 0; //turn count
    End = 0;



    OledDrawString("This is BattleBoats!\n Press BTN4 to\nchallenge, or wait\nfor opponent.");
    OledUpdate();
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event)
{
    static GuessData guessB;
    GuessData guessTmp;
    Message msg;
    msg.type = MESSAGE_NONE;
    //    buttoncheck(event);
    if (event.type == BB_EVENT_ERROR) {
        //printf ("error");
        msg.type = MESSAGE_ERROR;
        OledDrawString("Error Encountered!\n");
        OledUpdate();
        return msg;
    }
    if (event.type == BB_EVENT_RESET_BUTTON) {
        AgentInit();
        return msg;
    }
    if (End == END_DEFEAT) {
        defeat();
    }
    if (End == END_VICTORY) {
        victory();
    }

    switch (State) {
        //start state
    case AGENT_STATE_START:
        
        if (event.type == BB_EVENT_START_BUTTON) {
            //printf("count1\n");
            State = AGENT_STATE_CHALLENGING;
            msg.type = MESSAGE_CHA;
            //randomizes 
            A = rand() & ALLBIT;
            Table = NegotiationHash(A);
            msg.param0 = Table;
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_NONE, turn);
            return msg;

        } else if (event.type == BB_EVENT_CHA_RECEIVED) {
            //printf("count2\n");
            State = AGENT_STATE_ACCEPTING;
            msg.type = MESSAGE_ACC;
            B = rand() & ALLBIT;
            Table = event.param0;

            msg.param0 = B;
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_NONE, turn);
            return msg;

        } else {
            break;
        }
        return msg;
        
        //challenge state
    case AGENT_STATE_CHALLENGING:
        //reads the challenge event
        if (event.type == BB_EVENT_ACC_RECEIVED) {
            //printf("count3\n");
            B = event.param0;
            msg.type = MESSAGE_REV;

            msg.param0 = A;
            //decides who goes first
            if (NegotiateCoinFlip(A, B) == HEADS) {
                State = AGENT_STATE_WAITING_TO_SEND;
            } else {
                State = AGENT_STATE_DEFENDING;
            }

        }
        FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_MINE, turn);
        return msg;
        
        //accepting state
    case AGENT_STATE_ACCEPTING:
        //decides 
        if (event.type == BB_EVENT_REV_RECEIVED) {
            //printf("count4\n");
            A = event.param0;
            //double checks the negotiation
            if (!NegotiationVerify(A, Table)) {
                //printf("count5\n");
                State = AGENT_STATE_END_SCREEN;
                End = END_CHEAT;
                break;
                //checks who defends first
            } else if (NegotiateCoinFlip(A, B) == HEADS) {
                //printf("count6\n");
                State = AGENT_STATE_DEFENDING;
            } else {
                //printf("count7\n");
                msg.type = MESSAGE_SHO;
                guessB = FieldAIDecideGuess(&FieldB);
                msg.param0 = guessB.row;
                msg.param1 = guessB.col;
                State = AGENT_STATE_ATTACKING;
            }
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_THEIRS, turn);
            return msg;
        } else {
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_THEIRS, turn);
            return msg;
        }
        break;
        
        //waiting state
    case AGENT_STATE_WAITING_TO_SEND:
        if (event.type == BB_EVENT_MESSAGE_SENT) {
            //printf("msg sent\n");
            if (turn < FIELD_COLS * FIELD_ROWS) {
                //printf("turnmove\n");
                turn++;
            }
            guessTmp = FieldAIDecideGuess(&FieldB);
            guessB.row = guessTmp.row;
            guessB.col = guessTmp.col;


            msg.param0 = guessB.row;
            msg.param1 = guessB.col;

            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_MINE, turn);
            msg.type = MESSAGE_SHO;
            State = AGENT_STATE_ATTACKING;
        }
        return msg;
        
        //defending state
    case AGENT_STATE_DEFENDING:
        if (event.type == BB_EVENT_SHO_RECEIVED) {
            //printf("count9\n");
            guessB.row = event.param0;
            
            guessB.col = event.param1;
            FieldRegisterEnemyAttack(&FieldA, &guessB);
            if (FieldGetBoatStates(&FieldA) == 0) {
                State = AGENT_STATE_END_SCREEN;
                End = END_DEFEAT;
                defeat();
            }
            msg.param0 = guessB.row;
            msg.param1 = guessB.col;
            msg.param2 = guessB.result;


            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_THEIRS, turn);
            msg.type = MESSAGE_RES;
            State = AGENT_STATE_WAITING_TO_SEND;
            return msg;
        }
        
        //attack state
    case AGENT_STATE_ATTACKING:
        if (event.type == BB_EVENT_RES_RECEIVED && End != END_DEFEAT) {
            //printf("count10\n");
            guessB.row = event.param0;
            guessB.col = event.param1;
            guessB.result = event.param2;
            
            FieldUpdateKnowledge(&FieldB, &guessB);
            if (FieldGetBoatStates(&FieldB) == 0) {
                //sends to end screen
                //printf("end\n");
                End = END_VICTORY;
                victory();
                break;
            }
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_MINE, turn);
            State = AGENT_STATE_DEFENDING;
            break;
        }

        break;
        
        //end state
    case AGENT_STATE_END_SCREEN:
        //checks for cheater cheater lemon eater
        if (End == END_CHEAT) {
            OledClear(OLED_COLOR_BLACK);
            OledDrawString("You are lame! Stop cheating!");
            OledUpdate();
            break;
        }
    default:
        return msg;
    }
    return msg;
}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void)
{
    return State;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState)
{
    State = newState;

}

