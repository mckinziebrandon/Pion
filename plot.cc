/*==================================================================
Filename:       plot.cc
Author:         Brandon McKinzie
Date Created:   Jun 12, 2015
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

void plot()
{
    using namespace std;
    TFile * gFile = new TFile("plot_cfunc.root");

}
