/**
   @date 2020/06/25
**/

#ifndef P4GeneralParticleSource_hpp
#define P4GeneralParticleSource_hpp

#include <G4GeneralParticleSource.hh>
#include <ParameterGene.hpp>

class P4GeneralParticleSource
    : public sim4py::ParameterGene<G4GeneralParticleSource>
{
    
public:
    
    P4GeneralParticleSource();
    virtual ~P4GeneralParticleSource();

    void GeneratePrimaryVertex(G4Event* event) override;

private:

    bool is_applied_parameters;
    int verbose_level;

    void apply_parameters();
    void ion_command();
    void ion_level_command();
    
};

#endif
