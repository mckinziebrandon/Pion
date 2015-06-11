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
    const int n_slices  = 63;
    const int n_files   = 1; 

    int time(0);
    double cfunc(2);
    string line;

    TFile* outFile  = new TFile("outFile.root", "RECREATE");
    TNtuple* t      = new TNtuple("t", "t", "time:cfunc");
    TString baseName= "../ID32_mu0.0042_tsrc0/nuc3pt.dat.";

    fstream inFile;
    TString fileName;
    int fileNumber(1000);
    for (int i = 1; i <= n_files; i++)  // begin file loop
    {
        fileName  = baseName;
        fileName += fileNumber;
        cout << "\n\nOpening " << fileName << " . . . \n";
        inFile.open(fileName.Data());
        if (inFile.is_open())
        {
            GotoLine(inFile, 112);  // start of pion data
            while (time < n_slices)
            {
                (inFile >> time >> cfunc).ignore(100, '\n');
                t->Fill(time, cfunc);

                cout << "time: " << time << "\t";
                cout << "cfunc: " << cfunc << endl;
            }
        }
        else
        {
            cout << "\nError: unable to open file\n";
        }

        fileNumber += 8;
    } // end file loop

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

