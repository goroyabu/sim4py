/**
   @date 2020/07/06
**/

#ifndef W4EmStandardPhysicsSwitchingDoppler_hpp
#define W4EmStandardPhysicsSwitchingDoppler_hpp

#include <ParameterGene.hpp>

#include <G4VPhysicsConstructor.hh>
#include <G4EmParticleList.hh>
#include <globals.hh>

template<bool is_with_doppler>
class W4EmStandardPhysicsSwitchingDoppler
    : public sim4py::ParameterGene, public G4VPhysicsConstructor
{
    
public:
    
    explicit W4EmStandardPhysicsSwitchingDoppler(G4int ver=0, const G4String& name="");
    virtual ~W4EmStandardPhysicsSwitchingDoppler();
    
    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
    
private:
    G4int  verbose;
    G4EmParticleList partList;    
   
};

using W4EmStandardPhysicsWithDoppler = W4EmStandardPhysicsSwitchingDoppler<true>;
using W4EmStandardPhysicsNoDoppler = W4EmStandardPhysicsSwitchingDoppler<false>;

#include "W4EmStandardPhysicsSwitchingDoppler.icc"

#endif
