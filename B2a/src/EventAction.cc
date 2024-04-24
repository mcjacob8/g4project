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
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "TrackerHit.hh" // Include the header file for TrackerHit
#include <iostream>  

namespace B2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // get number of stored trajectories

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  std::size_t n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing

  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "
           << hc->GetSize() << " hits stored in this event" << G4endl;
  }

  //B2::TrackerHit hit;
  B2::TrackerHit* hit = nullptr;
  //TrackerHitsCollection* hitsCollection = GetHitsCollection("trackerHitsCollectionName", event);
  G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
  if (hc) {
        // Loop over hits in the collection
        for (size_t i = 0; i < hc->GetSize(); ++i) {
            //TrackerHit* hit = (*hitsCollection)[i];
            //hit = (*hc)[i];
            hit = dynamic_cast<B2::TrackerHit*>(hc->GetHit(i));
            if (hit) {
              G4int eventID = event->GetEventID(); // Get the event ID
              hit->WriteToFile("event_summary.txt", eventID);
              std::cout << "!!!TrackID: " << hit->GetTrackID() << std::endl;

            }
            //G4int trackID = hit->GetTrackID(); // Get the track ID from the hit
            //G4int eventID = hit->GetEventID(); // Get the event ID from the hit
            // Process or use the event-related data as needed
        }
    }

  //B2::TrackerHit hit; // Create a TrackerHit object

  //std::cout << "!!!TrackID: " << hit->GetTrackID() << std::endl;
  //hit->Print();
  //std::cout << "!!!EventID: " << eventID << std::endl;
  //std::cout << "!!!ChamberNb: " << hit->GetChamberNb() << std::endl;
  //hit->WriteToFile("event_summary2.txt"); // Call WriteToFile function with desired filename
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

