/**
   @date 2020/07/02
**/

#ifndef P4ParticleGun_hpp
#define P4ParticleGun_hpp

#include <ParameterGene.hpp>

#include <G4ParticleGun.hh>

class P4ParticleGun
    : public sim4py::ParameterGene<G4ParticleGun>
{

public:

    P4ParticleGun();
    virtual ~P4ParticleGun();
    
    void GeneratePrimaryVertex(G4Event* event) override;

protected:

    bool is_applied_parameters;
    int verbose_level;

    void apply_parameters();
    void ion_command();
    void ion_level_command();
    
};

#endif
