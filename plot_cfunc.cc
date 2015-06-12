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
    TFile * f = new TFile("outFile.root");
    TTree * t = (TTree*)f->Get("tree");
    Float_t time, cfunc;
    t->SetBranchAddress("time", &time);
    t->SetBranchAddress("cfunc", &cfunc);

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

    gROOT->ForceStyle();
}
