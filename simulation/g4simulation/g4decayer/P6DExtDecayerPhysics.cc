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
// $Id: P6DExtDecayerPhysics.cc,v 1.3 2015/01/05 23:49:39 mccumber Exp $
//
/// \file eventgenerator/pythia/decayer6/src/P6DExtDecayerPhysics.cc
/// \brief Implementation of the P6DExtDecayerPhysics class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "P6DExtDecayerPhysics.hh"
#include "G4Pythia6Decayer.hh"

#include <Geant4/G4VPhysicsConstructor.hh>
#include <Geant4/G4ParticleDefinition.hh>
#include <Geant4/G4ProcessManager.hh>
#include <Geant4/G4Decay.hh>

#include <iostream>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

P6DExtDecayerPhysics::P6DExtDecayerPhysics(const G4String& name)
  : G4VPhysicsConstructor(name),
    _active_force_decay(false),
    _force_decay_type(kAll)
{
/// Standard constructor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

P6DExtDecayerPhysics::~P6DExtDecayerPhysics() 
{
/// Destructor
}

//
// protected methods
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void P6DExtDecayerPhysics::ConstructParticle()
{
/// Nothing to be done here
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void P6DExtDecayerPhysics::ConstructProcess()
{
/// Loop over all particles instantiated and add external decayer
/// to all decay processes if External decayer is set

  // Create Geant4 external decayer
  G4Pythia6Decayer* extDecayer = new G4Pythia6Decayer();
  extDecayer->SetVerboseLevel(0); 
 
  aParticleIterator->reset();
  int decayer_used = 0;
  while ((*aParticleIterator)())
  {    
    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    
    if ( verboseLevel > 1 ) {
      G4cout << "Setting ext decayer for: " 
             <<  aParticleIterator->value()->GetParticleName() 
             << G4endl;
    } 
    
    G4ProcessVector* processVector = pmanager->GetProcessList();
    for (G4int i=0; i<processVector->length(); i++) {
    
      G4Decay* decay = dynamic_cast<G4Decay*>((*processVector)[i]);
      if ( decay )
	{
          // The extDecayer will be deleted in G4Decay destructor
	  // increment counter in case we want to print out stats
	  // for whatever reason (non null means it is used and
	  // must not be deleted)
          decay->SetExtDecayer(extDecayer);
          decayer_used++;
	}
    }              
  }

  if (_active_force_decay) {
    extDecayer->ForceDecayType(_force_decay_type);
  }

  // If the extDecayer isn't used for this particle we need to delete it here
  cout << "decayer used: " << decayer_used << endl;
  if (! decayer_used)
    {
      cout << "deleting decayer" << endl;
      delete  extDecayer;
    }
  if ( verboseLevel > 0 ) {
    G4cout << "External decayer physics constructed." << G4endl;
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
