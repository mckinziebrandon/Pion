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
#include "TDirectory.h"
#include "TCanvas.h"
#include "TF1.h"

void plot_cfunc()
{
    using namespace std;
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
    TGraph * CFunction[165];
    int file_count = 0;
    bool need_new_file = true;
    TString filename;
    for (int i = 0; i < nEntries; i++)
    {
        t->GetEntry(i);

        x[i%64] = i%64;
        y[i%64] = cfunc;

        if (i%63 == 0 && i!=0)
        {
            filename = "file";
            filename += file_count;
            CFunction[file_count] = new TGraph(64, x, y);
            CFunction[file_count]->Write();
            cout << "\n file #" << file_count << " has "
                 << CFunction[file_count]->GetN() << " entries.";
            file_count++;
        }
    }

    cout << "Total graphs made: " << file_count << endl;
    cout << "\n nEntries / 64 = " << nEntries / 64 << endl;

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
