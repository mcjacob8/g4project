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
/// \file B2/B2a/src/EventAction.cc
/// \brief Implementation of the B2::EventAction class

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4AnalysisManager.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "TrackerHit.hh" // Include the header file for TrackerHit
#include <iostream>  
#include <map>

namespace B2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // get number of stored trajectories

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  std::size_t n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing

  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    //G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "
           << hc->GetSize() << " hits stored in this event" << G4endl;
  }

  B2::TrackerHit* hit = nullptr;
  G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
  if (hc) {
    G4double sum = 0;
    G4double avx = 0;
    G4double avy = 0;
    G4double num = 0;
    std::map<int, double> xpos_map;
    std::map<int, double> ypos_map;
    std::map<int, double> nums_map;

        // Loop over hits in the collection
        for (size_t i = 0; i < hc->GetSize(); ++i) {
            //TrackerHit* hit = (*hitsCollection)[i];
            //hit = (*hc)[i];
            hit = dynamic_cast<B2::TrackerHit*>(hc->GetHit(i));
            // Loop over hits
            if (hit){
              G4double ene = hit->GetEdep() / CLHEP::keV;
              G4int chm = hit->GetChamberNb();
              G4int tID = hit->GetTrackID();
              G4int pID = hit->GetParticleID();
              //G4int eID = hit->GetEventID();
              G4ThreeVector xyz = hit->GetPos() / CLHEP::cm;
              if (chm == 1){
                sum += ene;
                //G4cout << "Particle ID: " << hit->GetParticleID()  << G4endl;
                if (pID == 11){
                  xpos_map[tID] += xyz.x();
                  ypos_map[tID] += xyz.y();
                  nums_map[tID] += 1;
                }
                if (tID == 1){
                  //G4cout << "Particle ID: " << hit->GetParticleID()  << G4endl;
                  //coordinates here
                  avx += xyz.x();
                  avy += xyz.y();
                  num += 1;
                  
                }
              }
            }
      }
      for (auto it = nums_map.begin(); it != nums_map.end(); ++it) {
       // std::cout << "Integer: " << it->first << ", Sum: " << it->second << std::endl;
       analysisManager->FillH2(1, xpos_map[it->first]/(it->second), ypos_map[it->first]/(it->second));

      }
      //G4cout << "Particle ID: " << hit->GetParticleID()  << G4endl;
      //G4cout << "Particle ID: " << hit->GetParticleID()  << G4endl;
      analysisManager->FillH1(1, avx/num);
      analysisManager->FillH1(0, sum);
      analysisManager->FillH2(0, avx/num, avy/num);
      analysisManager->FillNtupleIColumn(0,sum);
      analysisManager->FillNtupleIColumn(1,avx/num);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

