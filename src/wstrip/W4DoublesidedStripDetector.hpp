 /**
   @file    W4DoublesidedStripDetector.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef W4DoublesidedStripDetector_hpp
#define W4DoublesidedStripDetector_hpp

#include <P4PVConstruct.hpp>

#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class W4DoublesidedStripDetector
    : public P4PVConstruct
{

public:
    W4DoublesidedStripDetector();
    virtual ~W4DoublesidedStripDetector();
    
    virtual void Construct(G4LogicalVolume*) override;

    void PrepareMaterials();
    W4DoublesidedStripDetector* AddDetectorLayer
    (const std::string& material, double thickness);
    W4DoublesidedStripDetector* AddDetectorLayer
    (int user_detector_id, const std::string& material, double thickness);
    
private:

    struct layer_info
    {
	/* ID */
	int detector_id;
	int user_detector_id;		
	
	/* physical info */
	G4String material;       
	double xlength, ylength;
	double thickness;
	G4ThreeVector position;
	G4ThreeVector normal;

	/* pixel info */
	int xpixels, ypixels, zpixels;
	bool merge_same_pixel;
	bool merge_adjacent_pixel;	
    };
    std::vector<layer_info> layers_info;
    
    struct parameters_list
    {
	int verbose_level;
	
	// std::string world_material;
	// double world_size;
	// G4ThreeVector world_center;

	double detector_size;
	double detector_gap;
	double gap_of_abso_and_scat;
	int detid_max_of_scatterer;
	
	G4ThreeVector detector_center;
	G4ThreeVector detector_normal;

	bool is_enabled_merge_same_pixel;
	bool is_enabled_merge_adjacent_pixel;
	
	void ApplyParameters(sim4py::ParameterGene* module);
	bool is_already_applied;

	bool is_enabled_board;
	std::string board_material;
	double board_size;
	double board_thickness;

	int npixels_xside;
	int npixels_yside;
	
    } parameter;
    
};
#endif

