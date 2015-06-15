/*==================================================================
Filename:       pion_mass.cc
Author:         Brandon McKinzie
Date Created:   Jun 10, 2015
Description:    Obtain pion mass via curve fitting the corresponding
                two-point correlation function.
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include "TNtuple.h"
#include "TTree.h"
#include "TFile.h"

std::fstream& GotoLine(std::fstream&, int);

void pion_mass()
{
    using namespace std;
    const int n_slices  = 63;
    const int n_files   = 165; 

    int time(0); double cfunc(0);

    fstream inFile;
    TFile* outFile  = new TFile("outFile.root", "RECREATE");

    TString fileName;
    TString baseName= "../ID32_mu0.0042_tsrc0/nuc3pt.dat.";
    int fileNumber(1000);
    TNtuple * tree = new TNtuple("tree", "tree", "time:cfunc");
    TString branchName;

    for (int i = 1; i <= n_files; i++)  // --- begin file loop --- 
    {
        // ----- (re)set file associations -----
        fileName    = baseName;
        fileName   += fileNumber;
        branchName  = Form("file%d", fileNumber);
        cout << "\nOpening " << fileName << " . . . \n";
        inFile.open(fileName.Data());

        if (inFile.is_open())
        {
            GotoLine(inFile, 112);  // start of pion data
            for (int j = 0; j <= n_slices; j++)
            {
                (inFile >> time >> cfunc).ignore(200, '\n');
                tree->Fill(time, cfunc); 
            }
        }
        else
        {
            cout << "\nError: unable to open file\n";
        }

        inFile.close();
        fileNumber += 8;
    } // --- end file loop --- 

    cout << "\n\n";
    tree->Draw("cfunc:time");
    cout << "\nEntries: " << tree->GetEntries() << endl;
    outFile->Write();
    
}

std::fstream& GotoLine(std::fstream& file, int num)
{
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i)
    {
            file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

