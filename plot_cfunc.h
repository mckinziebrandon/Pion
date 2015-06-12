#include <iostream>
#include "TMath.h"
using namespace std;

double ComputeError(const int nFiles, double mean, double entry[])
{
//    double dev[nFiles];
    double err = 0;
    for (int i = 0; i < nFiles; i++)
    {
        err += TMath::Power(mean - entry[i], 2);
    }
    
    return TMath::Sqrt(err / (64 - 1)) / 8;
}
