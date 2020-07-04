/**
   @file VANLmodule.hpp
   @author Goro Yabu
   @date 2018/11/23
   @date 2020/06/23 VANLmodule
   @version 1.0
   @brief This file is modified from VANL_Module.hh
**/
#ifndef VANLmodule_hpp
#define VANLmodule_hpp

//#include <map>
#include <string>
//#include <iostream>

//#include "ANLStatus.hpp"
#include "VANLparametersList.hpp"

namespace anl
{
    
    template<class... Parents>
    class VANLmodule : public Parents...
    {
	
    public:

	VANLmodule()
	    : Parents()...
	    {
		mANLparamListStacker = new ANLparamListStacker();
		define_parameter<int>("module_id", 1);
		define_parameter<std::string>("module_name", "template");
	    }
	VANLmodule(Parents... parents)
	    : Parents(parents)...
	    {
		mANLparamListStacker = new ANLparamListStacker();
	    }
	
	template<typename... Ts> anl::VANLmodule<Parents...>* define_parameter(const std::string& key, Ts... defvalue)
	{
	    this->get_params_list<Ts...>()->define(key, defvalue...);
	    return this;
	}
	template<typename... Ts> anl::VANLmodule<Parents...>* set_parameter(const std::string& key, Ts... value)
	{
	    this->get_params_list<Ts...>()->set(key, value...);
	    return this;
	}
	template<typename... Ts> std::tuple<Ts...> get_parameter(const std::string& key)
	{
	    return this->get_params_list<Ts...>()->get(key);
	}
	void show_parameters()
	{
	}

    private:
	
	// ANLparamsInt* mANLparamsInt;
	ANLparamListStacker* mANLparamListStacker;

	template<typename... Ts> VANLparametersList<Ts...>* get_params_list()
	{
	    return mANLparamListStacker->get_params_list<Ts...>();
	}
	// template<> VANLparametersList<int> get_params_list() { return mANLparamsInt; }
	// template<> ANLparamsInt* get_params_list() { return mANLparamsInt; }
	
    };


}
#endif
    
