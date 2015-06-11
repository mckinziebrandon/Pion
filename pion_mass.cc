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
#include "TFile.h"

std::fstream& GotoLine(std::fstream&, int);

void pion_mass()
{
    using namespace std;
    int fileNumber(1000), time(0);
    double cfunc(2);
    string line;

    TFile* outFile  = new TFile("outFile.root", "RECREATE");
    TNtuple* t      = new TNtuple("t", "t", "time:cfunc");
    TString fName   = "../ID32_mu0.0042_tsrc0/nuc3pt.dat.";
    fName  += fileNumber;

    fstream inFile;
    cout << "\n\nOpening " << fName << " . . . \n";
    inFile.open(fName.Data());

    // organize data from files into TNtuples
    if (inFile.is_open())
    {
        GotoLine(inFile, 112);  // start of pion data
        while (time < 63)       // 63 timeslices total
        {
            inFile >> time >> cfunc;
            t->Fill(time, cfunc);
            cout << "time: " << time << "\t";
            cout << "cfunc: " << cfunc << endl;
            GotoLine(inFile, time + 113);
        }
    }
    else
    {
        cout << "\nError: unable to open file\n";
    }

    cout << "\n\n";
    t->Draw("cfunc:time");
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

