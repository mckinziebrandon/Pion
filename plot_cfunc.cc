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

    TFile * f       = new TFile("outFile.root");
    TFile * outFile = new TFile("plot_cfunc.root", "RECREATE");
    outFile->cd();

    //TCanvas * MyC = new TCanvas("MyC", "Canvas", 900, 500);
    TCanvas * MyC = new TCanvas();
//    MyC->Divide(3, 1);

    Float_t time, cfunc;
    TTree * tree = (TTree*)f->Get("tree");
    tree->SetBranchAddress("time", &time);
    tree->SetBranchAddress("cfunc", &cfunc);

    Double_t CFunc_Time_File[nTimes][nFiles];   // for easy access to all data points
    Double_t C_j[nTimes][nFiles];               // jacknife subsamples
    Double_t mEff_j[nTimes-1][nFiles];          // jacknife subsamples
    Double_t mEff[nTimes-1];                  // jacknife averaged
    Double_t mean[nTimes];
    Double_t entry[nFiles];
    Double_t error[nTimes];

    Int_t nEntries   = (Int_t)tree->GetEntries();
    Int_t file_count = 0;
    TString filename;
    filename  = "file";
    filename += file_count;
    
    TH1F * CFunction[nFiles];
    CFunction[0] = new TH1F(filename.Data(), filename.Data(), nTimes, -0.5, 63.5);
   
    TH1F * h_mEff = new TH1F("h_mEff", "Pion Effective Mass; time; m_EFF (t)", (nTimes/2 - 2), 1.5, 28.5);

    // --------------- BEGIN CFunction-fill loop ---------------
    for (Int_t i = 0; i < nEntries; i++) 
    {
        tree->GetEntry(i);

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
    } // --------------- END CFunction-fill loop ---------------

    /*********************************************************************************************************
                                   PART 1: "STANDARD" STATISTICAL METHOD
    *********************************************************************************************************/
    // --------------- BEGIN statistics loop ---------------
    for (Int_t t = 0; t < nTimes; t++) 
    {
        mean[t] = 0;
        for (Int_t c = 0; c < nFiles; c++)
        {
            CFunc_Time_File[t][c] = CFunction[c]->GetBinContent(t+1);
            entry[c] = CFunction[c]->GetBinContent(t+1);
            mean[t] += entry[c];
            if (c == nFiles-1)
            {
                mean[t]  = mean[t] / (Double_t)nTimes;
                error[t] = StandardError(nFiles, mean[t], entry);
            }
        }
    //    cout << "mean[" << t << "]: " << mean[t];
    //    cout << "\terr["  << t << "]: " << error[t] << endl;
    } // --------------- END statistics loop ---------------

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
    //g->Draw("AP");
    g->Write();

    /*********************************************************************************************************
                                   PART 2: "JACKNIFE" STATISTICAL METHOD
    *********************************************************************************************************/

    // Initialize 2-dimensional set {C_j}
    for (int time = 0; time < nTimes; time++)
    {
        for (int j = 0; j < nFiles; j++)
        {
            C_j[time][j] = 0;
            for (int file = 0; file < nFiles; file++)
            {
                if (file != j)
                {
                    C_j[time][j] += CFunc_Time_File[time][file];
                }
            }
            C_j[time][j] = C_j[time][j] / Double_t(nFiles - 1);
            if (time > (nTimes/2))
            {
                C_j[2*(nTimes/2)-time][j] += C_j[time][j];
                C_j[2*(nTimes/2)-time][j] = C_j[2*(nTimes/2)-time][j] / 2;

                C_j[time][j] = 0;
            }
        }
    }

    // Calculate mEff(t) from set {mEff(t)_j}
    Double_t jack_err[(nTimes/2)-2];
    for (int time = 2; time < nTimes/2-4 ; time++)
    {
        mEff[time]      = 0;
        jack_err[time]  = 0;

        for (int j = 0; j < nFiles; j++) 
        {
            mEff_j[time][j] = TMath::Log(C_j[time-1][j] / C_j[time][j]);
            mEff[time] += mEff_j[time][j];
        }
        mEff[time] = mEff[time] / Double_t(nFiles);

        for (int j = 0; j < nFiles; j++)
        {
            jack_err[time] += TMath::Power(mEff_j[time][j]-mEff[time], 2);
        }

        jack_err[time] = jack_err[time] * Double_t(nFiles - 1)/nFiles;
        cout << "err: " << jack_err[time] << endl;

        h_mEff->SetBinContent(time-1, mEff[time]);
        h_mEff->SetBinError(time-1, jack_err[time]);
    }

    MyC->cd();
    h_mEff->GetYaxis()->SetRangeUser(0.12, 0.22);
    h_mEff->SetMarkerStyle(26);
    h_mEff->Draw("E1");



//    gROOT->ForceStyle();
//    outFile->Close();
}
