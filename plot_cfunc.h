#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include "TNtuple.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"

#ifndef PLOT_CFUNC_H
#define PLOT_CFUNC_H

const Int_t nFiles = 115;
const Int_t nTimes = 64;

Double_t StandardError(const Int_t nPoints, double mean, double point[])
{
    Double_t err = 0;
    for (Int_t i = 0; i < nPoints; i++)
    {
        err += TMath::Power(mean - point[i], 2);
    }

    return TMath::Sqrt(err / (64 - 1)) / 8;
}

Double_t StandardMean(const Int_t nBins, const TH1F* hist)
{
    Double_t mean = 0;
    for (Int_t bin = 1; bin <= nBins; bin++)
    {
        mean += hist->GetBinContent(bin);
    }
    
    return (mean / (Double_t)nBins);
}


#endif
