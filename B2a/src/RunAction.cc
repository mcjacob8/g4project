//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B2/B2a/src/RunAction.cc
/// \brief Implementation of the B2::RunAction class

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "TrackerHit.hh"
#include <iostream> 

namespace B2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
  auto analysisManager = G4AnalysisManager::Instance();

  // set up our root files
  analysisManager->SetDefaultFileType("root");
	G4cout << "Using " << analysisManager->GetType() << G4endl;
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFileName("Histograms");

  // TH1 Histograms
  analysisManager->CreateH1("Energy Deposition", "Energy", 100, 0.,200);
  analysisManager->SetH1XAxisTitle(0, "Energy (keV)");  // Set x-axis title
  analysisManager->SetH1YAxisTitle(0, "Number of events");  // Set y-axis title

  analysisManager->CreateH1("Position x", "Position x", 50,-20,20);

  // TH2 Histograms
  analysisManager->CreateH2("x position", "y position", 100, -100, 100, 100, -100, 100);
  analysisManager->SetH2XAxisTitle(0, "x pos (cm)");  // Set x-axis title
  analysisManager->SetH2YAxisTitle(0, "y pos (cm)");  // Set y-axis title

  analysisManager->CreateH2("x position 2", "y position 2", 100, -100, 100, 100, -100, 100);
  analysisManager->SetH2XAxisTitle(1, "x pos (cm)");  // Set x-axis title
  analysisManager->SetH2YAxisTitle(1, "y pos (cm)");  // Set y-axis title


  analysisManager->SetVerboseLevel(1);

  // Unclear if this is necessary or not but it doesn't seem to hurt
  analysisManager->CreateNtuple("tutorial", "hits");
  analysisManager->CreateNtupleDColumn("energy");
  analysisManager->CreateNtupleDColumn("position");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* )
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

