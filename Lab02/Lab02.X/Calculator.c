// Standard libraries
#include <stdio.h>
#include <math.h>
#include <xc.h>

//User libraries:
#include "BOARD.h"


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double Avg(double num1, double num2);
double AbsV(double operand);
double FtoC(double operand);
double Tang(double operand);
double CtoF(double operand1);


//add more prototypes here

void CalculatorRun(void)
{
    printf("\n\nWelcome to Vince calculator program! Compiled at %s %s\n", __DATE__, __TIME__);

    // Your code here

    char c;
    int num1;
    int num2;
    printf("\nPlease enter a mathematical operation:");
    scanf("%c", &c);
    printf("\nPlease enter a number:");
    scanf("%d", &num1);
    if (c != '*') {
        if (c != '/') {
            if (c != '-') {
                if (c != 'm') {
                    if (c != '+') {
                        if (c != 'a') {
                            if (c != 'c') {
                                if (c != 'f') {
                                    if (c != 't') {

                                    } else {
                                        double answer = 0;
                                        answer = Tang(num1);
                                        printf("%f", answer);
                                    }

                                } else {
                                    double answer = 0;
                                    answer = CtoF(num1);
                                    printf("%f", answer);
                                }
                            } else {
                                double answer = 0;
                                answer = FtoC(num1);
                                printf("%f", answer);
                            }
                        } else {
                            double answer = 0;
                            answer = AbsV(num1);
                            printf("%f", answer);
                        }
                    } else {
                        double answer = 0;
                        printf("Please enter the second number:");
                        scanf("%d", &num2);
                        answer = Add(num1, num2);
                        printf("%f", answer);
                    }
                } else {
                    double answer = 0;
                    printf("Please enter the second number:");
                    scanf("%d", &num2);
                    answer = Avg(num1, num2);
                    printf("%f", answer);
                }


            } else {
                double answer = 0;
                printf("Please enter the second number:");
                scanf("%d", &num2);
                double Subtract(double num1, double num2);
                answer = Subtract(num1, num2);
                printf("%f", answer);
            }
        } else {
            double answer = 0;
            printf("Please enter the second number:");
            scanf("%d", &num2);
            double Divide(double num1, double num2);
            answer = Divide(num1, num2);
            printf("%f", answer);
        }
    } else {
        double answer = 0;
        printf("Please enter the second number:");
        scanf("%d", &num2);
        answer = Multiply(num1, num2);
        printf("%f", answer);

    }

    while (1);
}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2)
{
    double answer = operand1 + operand2;
    return answer;

}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2)
{
    double answer = operand1 - operand2;
    return answer;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2)
{
    double answer = operand1 * operand2;
    return answer;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2)
{
    double answer = operand1 / operand2;
    return answer;
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsV(double operand1)
{
    double answer = abs(operand1);
    return answer;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FtoC(double operand1)
{
    double answer = ((operand1 - 32)*(5.0 / 9.0));
    return answer;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CtoF(double operand1)
{
    double answer = ((operand1 * (9.0 / 5.0) + 32));
    return answer;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Avg(double operand1, double operand2)
{
    double answer = ((operand1 + operand2) / 2);
    return answer;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tang(double operand1)
{
    double answer = tan(operand1);
    return answer;
}
/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/



