/**
   @date 2020/06/25
**/

#ifndef P4SphericalSource_hpp
#define P4SphericalSource_hpp

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ThreeVector.hh>
#include <ParameterGene.hpp>

class P4SphericalSource
    : public sim4py::ParameterGene, public G4VUserPrimaryGeneratorAction
{

public:

    P4SphericalSource();
    virtual ~P4SphericalSource();

    void GeneratePrimaryVertex(G4Event* event) override;

private:

    bool is_applied_parameters;
    int verbose_level;
    //
    // void apply_parameters();
    // void ion_command();
    // void ion_level_command();
    //
    // bool match_candidates
    // (const std::string& key, const std::string& input, std::vector<std::string>& cand_list);

};

#endif
