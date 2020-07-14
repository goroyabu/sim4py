/**
   @date 2020/07/02
**/

#include "W4ActionInitialization.hpp"
#include "W4RunAction.hpp"
#include "W4EventAction.hpp"

W4ActionInitialization::W4ActionInitialization()
    : P4ActionInitialization()
{
    SetParameter<std::string>("class_name", "W4ActionInitialization");

    // std::string file_name = "sim_%Y%m%d_%H%M%S.root";
    // DefineParameter<std::string>("file_name", file_name);

    RegisterUserAction( new W4RunAction );
    RegisterUserAction( new W4EventAction );    
}

W4ActionInitialization::~W4ActionInitialization()
{
}

void W4ActionInitialization::Build() const
{
    P4ActionInitialization::Build();
}
