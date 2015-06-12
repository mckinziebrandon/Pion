/*==================================================================
Filename:       plot_cfunc.cc
Author:         Brandon McKinzie
Date Created:   Jun 11, 2015
Description:    
                
==================================================================*/
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
#include "plot_cfunc.h"

void plot_cfunc()
{
    using namespace std;
    const int nFiles = 115;
    const int nTimes = 64;
    TFile * f = new TFile("outFile.root");
    TTree * t = (TTree*)f->Get("tree");

    Float_t time, cfunc;
    t->SetBranchAddress("time", &time);
    t->SetBranchAddress("cfunc", &cfunc);
    int nEntries = (int)t->GetEntries();
    Float_t x[64], y[64];
    
    // make array of tf1 containing each files thing.
    // can get C(t) for each t then. 
    TFile * outFile = new TFile("plot_cfunc.root", "RECREATE");
    outFile->cd();
    TH1F * CFunction[nFiles];
    int file_count = 0;
    TString filename;
    filename = "file";
    filename += file_count;
    CFunction[0] = new TH1F(filename.Data(), filename.Data(), 64, -0.5, 63.5);
    for (int i = 0; i < nEntries; i++)
    {
        t->GetEntry(i);

        CFunction[file_count]->SetBinContent((i%64)+1, cfunc);

        if ((i%64) + 1 == 64)
        {
            if (file_count == nFiles) break;
            CFunction[file_count]->Write();
            file_count++;

            filename = "file";
            filename += file_count;
            CFunction[file_count] = new TH1F(filename.Data(), filename.Data(), 64, -0.5, 63.5);
        }
    }

    double mean[nTimes];
    double entry[nFiles];
    double error[nTimes];
    for (int t = 0; t < nTimes; t++)
    {
        mean[t] = 0;
        for (int c = 0; c < nFiles; c++)
        {
            entry[c] = CFunction[c]->GetBinContent(t+1);
            mean[t] += entry[c];
            if (c == nFiles-1)
            {
                mean[t]  = mean[t] / (double)nTimes;
                error[t] = ComputeError(nFiles, mean[t], entry);
            }
        }
        cout << "mean[" << t << "]: " << mean[t];
        cout << "\terr["  << t << "]: " << error[t] << endl;
    }

    Float_t xx[64], yy[64], e[64];
    for (int i = 0; i < nTimes; i ++)
    {
        xx[i] = i;
        yy[i] = (Float_t)mean[i];
        e[i]  = error[i];
    }

    TGraph * g = new TGraphErrors(nTimes, xx, yy, NULL, e);
    g->SetTitle("Pion Correlation Function; time; c(t)");
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();
    g->Draw("AP");


    /*
    TCanvas * temp = new TCanvas();
    temp->cd();
    t->Draw("cfunc:time>>graph");
    temp->Close();
    TH2F * g = (TH2F*)gDirectory->Get("graph");
    g->SetTitle("Pion Two-Point Correlation Function; x; y");
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();
    //g->Fit();
    g->Draw("col");
    */

    gROOT->ForceStyle();
}
