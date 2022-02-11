/* 
   This program accepts an input N from the user, validates if there is only 
   one natural number and then does the following operations:
   --The program generates 2*N Random numbers within the interval -1 to 1
     (One for x and y coordinate each)
   --All the points within a unit distance from the origin are identified.
   --The fraction of points lying inside the unit circle will help us identify
     the approximate value of PI

   The purpose of each variable is illustrated as and when declared
   
   The Authors of this program are: ME20B178, EE20B075, EE20B080 (Group 4C).

   This program was created on 02 February, 2021 (Starting and Ending Date).

   There are adequate comments within the program that explain everything. 
*/
   
//Given below are the 5 header files required for the program

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//This function estimates the value of PI by generating Random Numbers
double piestimate(int N)
{
    long long int i, ins = 0; //ins is an iterator for no of points inside circle
    double x, y, ans, dis; //dis is the distance of point from origin
    for(i = 0; i < N; i++)
    {
        x = rand(); //Random values are assigned for both coordinates
        y = rand();
        x = -1 + x * 2 / RAND_MAX; //The interval range is modified over here
        y = -1 + y * 2 / RAND_MAX;
        dis = sqrt(x*x + y*y); //Distance from origin is computed
        if (dis <= 1)
            ins++;
    }
    ans = 4 * (double)ins / N; //PI is approximated
    return ans;
}

int main(int argc, char **argv) 
{   
    //Checking if there are multiple/no arguments
    if(argc != 2) {
        printf("Error: You did not enter exactly one number!");
        return 0;
    }

    int i, j;

    /* Checking if the given input is a whole number or not (48, 57 are ASCII values for 0, 9 respectively) */
    for(i = 0; i < strlen(argv[1]); i++)
    {
        if(argv[1][i] < 48 || argv[1][i] > 57 || strcmp("0",argv[1]) == 0)
        {
            printf("Error! Please enter a natural number only! (In Base 10)");
            return 0;
        }
    }

    srand(time(0)); //A random seed value is generated
    long long int N;
    double piapprox, err, sum = 0; //sum denotes the sum of errors in all iterations
                                             //sump denotes the sum of piapproxs in all iterations
    N = atoi(argv[1]);
    sum = 0;
    for(j = 0; j < 10; j++) //10 different trails are made to average out the estimated error
    {
        piapprox = piestimate(N);
        err = fabs(M_PI - piapprox);
        sum = sum + err;
    }
    err = sum / 10; //Average error is taken
    printf("Estimation of pi is: %lf \nwhen N was %lld \n", M_PI + err, N);
    printf("Error of approximation: %lf", err);
    return 0;
}
