#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TLegend.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include <TH1F.h>
#include <TFile.h>
#include <TF1.h>
#include <TAxis.h>
#include <TLine.h>

using namespace std;

TCanvas *c1 = new TCanvas("c1","c1",1200,600);
TH1F * hist1 = new TH1F("Energy Deposition","Energy Deposition+",100,0,250);


int main() {
    //  Vectors to store initial data in
    vector<double> eventnum;
    vector<double> engs;
    vector<int> det;

    // Create a matrix using a vector of vectors of variant
    

    int nevents = 3;
    vector<vector<double>> events;
    vector<vector<double>> toten;



    // Open the input file
    std::ifstream inputFile("/Users/jacob/compphys2/g4project/build/event_summary.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string token;

        // Read TrackID from the line
        while (iss >> token) {
            if (token == "TrackID:") {
                int TrackID;
                if (iss >> TrackID) {
                    if (TrackID != 1) {
                        //Skip processing EventID if TrackID is not 1
                        break;
                    }
                    //std::cout << "TrackID: " << TrackID << std::endl;
                }
            } else if (token == "EventID:") {
                int EventID;
                if (iss >> EventID) {
                    //std::cout << "EventID: " << EventID << std::endl;
                    // You can store or process EventID here
                    eventnum.push_back(EventID);
                }
            } else if (token == "Edep:") {
                double Edep;
                if (iss >> Edep) {
                    //std::cout << "Edep: " << Edep << std::endl;
                    engs.push_back(Edep);
                    // You can store or process Edep here
                }
            }else if (token == "ChamberNb:") {
                int ChamberNb;
                if (iss >> ChamberNb) {
                    //std::cout << "ChamberNb: " << ChamberNb << std::endl;
                    det.push_back(ChamberNb);
                    // You can store or process ChamberNb here
                }
             }
         }
    }

    // Close the file
    inputFile.close();

    // Store our data as vector of vectors
    for (int n = 0; n < nevents-1; ++n) {
        vector<double> row1;
        vector<double> row2;
        for (int i = 0; i < eventnum.size(); ++i) {
            if (eventnum[i]==n) {
                row1.push_back(det[i]);   // Detector number
                row2.push_back(engs[i]);  // Energy deposited
            }
        }
        events.push_back(row1);
        events.push_back(row2);
    }


    // Store energy deposition of each event on each detector
    int ndet = 5;
    for (int e = 0; e < nevents*2-1; e+=2) {
        vector<double> row;
        for (int n = 0; n < ndet; n++) {
            double sum = 0;
            for (int i = 0; i < events[e].size(); ++i) {
                if (events[e][i]==n) {
                    sum += events[e+1][i];
                }
            } 
            cout << "sum = " << sum << endl;
            row.push_back(sum);
        }
        //cout << "test = " << row[0] << endl;
        toten.push_back(row);
    }
    //cout << "test = " << toten[0][0] << endl;

    hist1 ->Fill(toten[0][0]);
    hist1 ->Fill(toten[1][0]);

    //draw graph
	c1->cd();
	hist1->Draw();
    c1->SaveAs("histogram.png");


    return 0;
}