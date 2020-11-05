/**
   @date 2020/08/06
**/

#ifndef UserVolume_hpp
#define UserVolume_hpp

#include <P4PVConstruct.hpp>

class UserVolume
    : public P4PVConstruct
{

public:
    
    UserVolume();
    virtual ~UserVolume(){}    
    virtual void Construct(G4LogicalVolume*) override;

private:

    int verbose_level;
    
};

#endif
