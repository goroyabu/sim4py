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

    W4DoublesidedStripDetector* SetCurrentLayerSize
    (double xlength, double ylength);
    W4DoublesidedStripDetector* SetCurrentLayerPosition
    (double x, double y, double z, sim4py::unit length);
    W4DoublesidedStripDetector* SetCurrentLayerRotation
    (double x, double y, double z, sim4py::unit angle);    
    W4DoublesidedStripDetector* SetCurrentLayerPixels
    (int xpixels, int ypixels, int zpixels=-1);
    
private:

    struct layer_info
    {
	layer_info()
	{
	    detector_id = -1;
	    user_detector_id = -1;
	    material = -1;
	    thickness = -1;
	    
	    xlength = -1;
	    ylength = -1;

	    position = G4ThreeVector( 0, 0, 0 );
	    rotation = G4ThreeVector( 0, 0, 0 );
	    is_set_position = false;
	    is_set_rotation = false;

	    xpixels = -1;
	    ypixels = -1;
	    zpixels = -1;
	    merge_same_pixel = false;
	    merge_adjacent_pixel = false;
	}   
	
	/* ID */
	int detector_id;
	int user_detector_id;		
	
	/* physical info */
	G4String material;       
	double xlength, ylength;
	double board_margin;
	double thickness;
	G4ThreeVector position;
	G4ThreeVector rotation;
	bool is_set_position;
	bool is_set_rotation;

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
	//G4ThreeVector detector_normal;
	G4ThreeVector detector_rotation;

	bool is_enabled_merge_same_pixel;
	bool is_enabled_merge_adjacent_pixel;
	
	void ApplyParameters(sim4py::ParameterGene* module);
	bool is_already_applied;

	bool is_enabled_board;
	std::string board_material;
	double board_size;
	double board_thickness;
	double board_margin;

	int npixels_xside;
	int npixels_yside;
	
    } parameter;
    
};
#endif

