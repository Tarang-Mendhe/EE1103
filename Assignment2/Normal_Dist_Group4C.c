/**************************************************************************************

Date: 09th February, 2021

Authors: ME20B178, EE20B075, EE20B080 (Group 4C)

Purpose: To create 2 different Gaussian Noise Distributions using the Box-Mueller 
        Transform, adding this noise to the sin wave function array x[n], extracting 
        the noise (z[n]) back again from the final y[n] obtained and finally 
        comparing the percentage of points that lie within N Standard Deviations
        of the mean of the distribution

Inputs: A (Amplitude of sin wave), f (frequency of wave), M (No of cycles of the wave),
        P (No of points taken per cycle). The inputs are accepted in the same order
        
Outputs: The mean, standard deviation, no of points which fall within 2sigma, 4sigma,
        and 6sigma for both the distibutions and analysation of the difference 
        between the outputs obtained

***************************************************************************************/

//4 Libraries are required for this program
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

float x[1000], yf[1000], ys[1000], zf[1000], zs[1000];
int M, P, N;
float A, f;
float mu, stdev;
int sig2, sig4, sig6;

void singen() //Stores values of sin(x) in array x[n]
{
    int i;
    for(i=0;i<N;i++)
    {
        x[i] = A * sin(2 * M_PI * f * i);
    }
}

void boxmul() //Creates the gaussian noise for 2 different distributions
{
    float U1, U2; //U1, U2 reprsents the uniform distributions
    int i;
    for(i = 0; i < N; i++)
    {
        U1 = rand() / (float)RAND_MAX;
        U2 = rand() / (float)RAND_MAX;
        zf[i] = 0.1 * 2*A * sqrt(-2 * log (U1)) * cos(2 * M_PI * U2); //z represents the noise
        U1 = rand() / (float)RAND_MAX;
        U2 = rand() / (float)RAND_MAX;
        zs[i] = 0.1 * 2*A + 0.2 * 2*A * sqrt(-2 * log (U1)) * cos(2 * M_PI * U2);
        yf[i] = x[i] + zf[i]; //y represents the combined sin wave and noise
        ys[i] = x[i] + zs[i];
    }   
}

void getdetail(float z[]) //Gives the details of any one distribution
{
    float ms, sume = 0, sumr = 0;
    sig2 = sig4 = sig6 = 0;
    int i;
    for(i = 0; i < N; i++)
    {
        sume += z[i];
        sumr += z[i]*z[i];
    }
    mu = sume / N; //Average is calculated
    ms = sumr / N; //Mean square is calculated
    stdev = sqrt(ms - mu*mu); //Standard deviation is calculated
    for(i = 0; i < N; i++) //No of points within different standard deviations is calculated
    {
        if(fabs(z[i] - mu) <= stdev)
            sig2++;
        else if(fabs(z[i] - mu) <= 2*stdev)
            sig4++;
        else if(fabs(z[i] - mu) <= 3*stdev)
            sig6++;
    }
    sig4 = sig4 + sig2;
    sig6 = sig6 + sig4;
}

int main(int argc, char **argv)
{
    //Checking if there are multiple/no arguments
    if(argc != 5)
    {
        printf("Please enter only A, f, M, P in order! Any text will be ignored/interpreted as 0");
        return 0;
    }

    int sig2s, sig4s, sig6s; //Most of the statements are self-explanatory

    A = atof(argv[1]);
    f = atof(argv[2]);
    M = atoi(argv[3]);
    P = atoi(argv[4]);
    N = M * P;
    singen(); //Sine function is used to store values in x[n]
    srand(time(0));
    boxmul(); //Box Mueller Transform is applied to get 2 distributions
    
    printf("(Note: %c stands for standard deviation)\n\n", (char)236); //All details are printed below
    
    printf("Guassian Noise 1:\n");
    getdetail(zf);
    printf("No of points taken: %d\n", N);
    printf("The mean of the distribution is: %f\n", mu);
    printf("The standard deviation of the distribution is: %f\n", stdev);
    printf("No of points lying within 2%c: %d (About %f%% of the points)\n", (char)236, sig2, sig2 / (float)N);
    printf("No of points lying within 4%c: %d (About %f%% of the points)\n", (char)236, sig4, sig4 / (float)N);
    printf("No of points lying within 6%c: %d (About %f%% of the points)\n\n", (char)236, sig6, sig6 / (float)N);

    sig2s = sig2; //sig2s, sig4s, sig6s store the values of the previous distribution
    sig4s = sig4;
    sig6s = sig6;

    printf("Guassian Noise 2:\n");
    getdetail(zs);
    printf("No of points taken: %d\n", N);
    printf("The mean of the distribution is: %f\n", mu);
    printf("The standard deviation of the distribution is: %f\n", stdev);
    printf("No of points lying within 2%c: %d (About %f%% of the points)\n", (char)236, sig2, sig2 / (float)N);
    printf("No of points lying within 4%c: %d (About %f%% of the points)\n", (char)236, sig4, sig4 / (float)N);
    printf("No of points lying within 6%c: %d (About %f%% of the points)\n\n", (char)236, sig6, sig6 / (float)N);

    printf("Difference of points lying within 2%c: %d (About %f%% of the points)\n", (char)236, sig2-sig2s, (sig2-sig2s) / (float)N);
    printf("Difference of points lying within 4%c: %d (About %f%% of the points)\n", (char)236, sig4-sig4s, (sig4-sig4s) / (float)N);
    printf("Difference of points lying within 6%c: %d (About %f%% of the points)\n\n", (char)236, sig6-sig6s, (sig6-sig6s) / (float)N);
    printf("Therefore we see that the percentage of points lying within N standard deviations\n");
    printf("is approximately the same, independent of mean and standard deviation!");
    return 0;
}
