/**
   @date 2020/07/02
**/

#ifndef W4ActionInitialization_hpp
#define W4ActionInitialization_hpp

#include <P4ActionInitialization.hpp>

class W4RunAction;
class W4EventAction;

class W4ActionInitialization
    : public P4ActionInitialization
{

public:
    
    W4ActionInitialization();
    virtual ~W4ActionInitialization();

    virtual void Build() const override;
};

#endif 
