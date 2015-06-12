/*==================================================================
Filename:       plot_cfunc.cc
Author:         Brandon McKinzie
Date Created:   Jun 11, 2015
Description:    
                
==================================================================*/
#include <iostream>
#include "plot_cfunc.h"

void plot_cfunc()
{
    using namespace std;

    TFile * f = new TFile("outFile.root");
    TFile * outFile = new TFile("plot_cfunc.root", "RECREATE");
    outFile->cd();

    Float_t time, cfunc;
    TTree * tree = (TTree*)f->Get("tree");
    tree->SetBranchAddress("time", &time);
    tree->SetBranchAddress("cfunc", &cfunc);

    Double_t mean[nTimes];
    Double_t entry[nFiles];
    Double_t error[nTimes];

    Int_t nEntries = (Int_t)tree->GetEntries();
    Int_t file_count = 0;
    TString filename;
    filename  = "file";
    filename += file_count;
    
    TH1F * CFunction[nFiles];
    CFunction[0] = new TH1F(filename.Data(), filename.Data(), nTimes, -0.5, 63.5);
   

    // --------------- BEGIN CFunction-fill loop ---------------
 //   Float_t CFunc_Time_File[nTimes][nFiles]; // for easy access to all data points
  //  Int_t count(0);
    for (Int_t i = 0; i < nEntries; i++) 
    {
        tree->GetEntry(i);
//        CFunc_Time_File[time][count] = cfunc;

        CFunction[file_count]->SetBinContent((i%nTimes)+1, cfunc);

        if ((i%nTimes) + 1 == nTimes)
        {
            if (file_count == nFiles)
            {
                break;
            }

            CFunction[file_count]->Write();

            file_count++;
            filename  = "file";
            filename += file_count;
            CFunction[file_count] = new TH1F(filename.Data(), filename.Data(), nTimes, -0.5, 63.5);
        }
    } 
    // --------------- END CFunction-fill loop ---------------

    /*********************************************************************************************************
                                   PART 1: "STANDARD" STATISTICAL METHOD
    *********************************************************************************************************/
    // --------------- BEGIN statistics loop ---------------
    for (Int_t t = 0; t < nTimes; t++) 
    {
        mean[t] = 0;
        for (Int_t c = 0; c < nFiles; c++)
        {
            entry[c] = CFunction[c]->GetBinContent(t+1);
            mean[t] += entry[c];
            if (c == nFiles-1)
            {
                mean[t]  = mean[t] / (Double_t)nTimes;
                error[t] = StandardError(nFiles, mean[t], entry);
            }
        }
        cout << "mean[" << t << "]: " << mean[t];
        cout << "\terr["  << t << "]: " << error[t] << endl;
    } 
    // --------------- END statistics loop ---------------

    Float_t x[nTimes], y[nTimes], err[nTimes];
    for (Int_t i = 0; i < nTimes; i ++)
    {
        x[i] = i;
        y[i] = mean[i];
        err[i] = error[i];
    }

    TGraph * g = new TGraphErrors(nTimes, x, y, NULL, err);
    g->SetTitle("Pion Correlation Function; time; c(t)");
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();
    g->Write();

    /*********************************************************************************************************
                                   PART 2: "JACKNIFE" STATISTICAL METHOD
    *********************************************************************************************************/



    gROOT->ForceStyle();
    outFile->Close();
}
