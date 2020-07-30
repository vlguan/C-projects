/*
 * File:   newmainXC161.c
 * Author: vince
 *
 * Created on March 12, 2020, 9:07 PM
 */


#include "xc.h"
#include "Agent.h"
#include "Field.h"
#include "Negotiation.h"
#include "FieldOled.h"
#include <stdio.h>
#include "Oled.h"
#include "OledDriver.h"

enum {
    END_STATE_CHEAT = 1,
    END_STATE_DEFEAT,
    END_STATE_VICTORY,
};

// Module Variables
static AgentState State;
static Field FieldA;
static Field FieldB;
static NegotiationData SecA;
static NegotiationData SecB;
static NegotiationData Hash;
static int TurnCount = 0;
static int EndState;

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
    SecA = 0;
    SecB = 0;
    Hash = 0;
    TurnCount = 0;
    EndState = 0;
    OledDrawString("This is BattleBoats!\nPress BTN4 to\nchallenge, or wait\nfor opponent.");
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
    GuessData guessTemp;
    Message message;
    message.type = MESSAGE_NONE;

    if (event.type == BB_EVENT_ERROR) {
        message.type = MESSAGE_ERROR;
        OledDrawString("Error Encountered!\n");
        OledUpdate();
        return message;
    }
    if (event.type == BB_EVENT_RESET_BUTTON) {
        AgentInit();
        return message;
    }
    if (EndState == END_STATE_DEFEAT) {
        OledClear(OLED_COLOR_BLACK);
        OledDrawString("Game Over: Defeat");
        OledUpdate();
    }
    if (EndState == END_STATE_VICTORY) {
        OledClear(OLED_COLOR_BLACK);
        OledDrawString("Game Over: Victory");
        OledUpdate();
    }

    switch (State) {
    case AGENT_STATE_START:
        if (event.type == BB_EVENT_START_BUTTON) {
            State = AGENT_STATE_CHALLENGING;
            message.type = MESSAGE_CHA;
            SecA = rand() & 0xFFFF;
            Hash = NegotiationHash(SecA);
            message.param0 = Hash;
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_NONE, TurnCount);
            return message;
        } else if (event.type == BB_EVENT_CHA_RECEIVED) {
            State = AGENT_STATE_ACCEPTING;
            message.type = MESSAGE_ACC;
            SecB = rand() & 0xFFFF;
            Hash = event.param0;
            message.param0 = SecB;
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_NONE, TurnCount);
            return message;
        } else {
            break;
        }
        return message;
    case AGENT_STATE_CHALLENGING:
        if (event.type == BB_EVENT_ACC_RECEIVED) {
            SecB = event.param0;
            message.type = MESSAGE_REV;
            message.param0 = SecA;
            if (NegotiateCoinFlip(SecA, SecB) == HEADS) {
                State = AGENT_STATE_WAITING_TO_SEND;
            } else {
                State = AGENT_STATE_DEFENDING;
            }
        }
        FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_MINE, TurnCount);
        return message;
    case AGENT_STATE_ACCEPTING:
        printf("%d\n", event.type);
        if (event.type == BB_EVENT_REV_RECEIVED) {
            SecA = event.param0;
            if (!NegotiationVerify(SecA, Hash)) {
                State = AGENT_STATE_END_SCREEN;
                EndState = END_STATE_CHEAT;
                break;
            } else if (NegotiateCoinFlip(SecA, SecB) == HEADS) {
                State = AGENT_STATE_DEFENDING;
            } else { // Tails
                message.type = MESSAGE_SHO;
                guessB = FieldAIDecideGuess(&FieldB);
                message.param0 = guessB.row;
                message.param1 = guessB.col;
                State = AGENT_STATE_ATTACKING;
            }
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_THEIRS, TurnCount);
            return message;
        } else {
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_THEIRS, TurnCount);
            return message;
        }
        break;
    case AGENT_STATE_WAITING_TO_SEND:
        if (event.type == BB_EVENT_MESSAGE_SENT) {
            if (TurnCount < FIELD_COLS * FIELD_ROWS) { // Corner Case
                TurnCount++;
            }
            guessTemp = FieldAIDecideGuess(&FieldB);
            guessB.row = guessTemp.row;
            guessB.col = guessTemp.col;
            message.param0 = guessB.row;
            message.param1 = guessB.col;
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_MINE, TurnCount);
            message.type = MESSAGE_SHO;
            State = AGENT_STATE_ATTACKING;
        }
        return message;
    case AGENT_STATE_DEFENDING:
        if (event.type == BB_EVENT_SHO_RECEIVED) {
            guessB.row = event.param0; // Update local agent vars
            guessB.col = event.param1;
            FieldRegisterEnemyAttack(&FieldA, &guessB);
            if (FieldGetBoatStates(&FieldA) == 0) {
                State = AGENT_STATE_END_SCREEN;
                EndState = END_STATE_DEFEAT;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString("Game Over: Defeat");
                OledUpdate();
            }
            message.param0 = guessB.row;
            message.param1 = guessB.col;
            message.param2 = guessB.result;
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_THEIRS, TurnCount);
            message.type = MESSAGE_RES;
            State = AGENT_STATE_WAITING_TO_SEND;
            return message;
        }
        break;
    case AGENT_STATE_ATTACKING:
        if (event.type == BB_EVENT_RES_RECEIVED && EndState != END_STATE_DEFEAT) {
            guessB.row = event.param0;
            guessB.col = event.param1;
            guessB.result = event.param2;
            FieldUpdateKnowledge(&FieldB, &guessB);
            if (FieldGetBoatStates(&FieldB) == 0) {
                //                State = AGENT_STATE_END_SCREEN;
                EndState = END_STATE_VICTORY;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString("Game Over: Victory");
                OledUpdate();
                break;
            }
            FieldOledDrawScreen(&FieldA, &FieldB, FIELD_OLED_TURN_MINE, TurnCount);
            State = AGENT_STATE_DEFENDING;
            break;
        }
        break;
    case AGENT_STATE_END_SCREEN:
        if (EndState == END_STATE_CHEAT) {
            OledClear(OLED_COLOR_BLACK);
            OledDrawString("Challenger Caught Cheating!");
        } else if (EndState == END_STATE_DEFEAT) {
            //            OledDrawString("Game Over: Defeat");
        } else if (EndState == EndState) {
            //            OledDrawString("Game Over: Victory");
        }
        OledUpdate();
        break;
    default:
        return message;
    }
    return message;
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