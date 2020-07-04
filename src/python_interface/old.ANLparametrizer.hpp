/**
   @date 2020/04/02
   @date 2020/06/23 switch_flag, fix_parameters, unit
**/
#ifndef VANL_Parameters_hpp
#define VANL_Parameters_hpp

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <tuple>

#include <pybind11/pybind11.h>

// #include "ANLStatus.hpp"
#include "UnitDefinition.hpp"

namespace anl
{
    class UnitDefinition;
    using unit = UnitDefinition;
    template<typename... Ts> class VANLparametersList;
    
    using ANLparamsInt         = VANLparametersList<int>;
    using ANLparamsString      = VANLparametersList<std::string>;    
    using ANLparamsDouble      = VANLparametersList<double>;
    using ANLparamsDoubleUnit  = VANLparametersList<double, unit>;
    using ANLparamsVector3     = VANLparametersList<double, double, double>;
    using ANLparamsVector3Unit = VANLparametersList<double, double, double, unit>;
    using ANLparamsBool        = VANLparametersList<bool>;
    using ANLflags             = ANLparamsBool;

    template<typename Tuple,size_t N=0> std::ostream& stream_tuple(std::ostream& os, const Tuple& tup)
    {
	if constexpr ( std::tuple_size<Tuple>::value==1 && N==0 ) {
		os << std::get<N>(tup);
		return os;
	    }
	if constexpr ( N < std::tuple_size<Tuple>::value ) {
		if constexpr ( N==0 ) os << "(";
		os << std::get<N>(tup) << ", ";
		stream_tuple<Tuple, N+1>(os, tup);
	    }
	else os << ")";
	return os;
    }
    template<size_t N=0, typename ...Ts> std::ostream& operator<< (std::ostream& os, const std::tuple<Ts...>& tup)
    {
        return stream_tuple(os, tup);
    }
    
    template<typename... Ts>
    class VANLparametersList
    {
	
    private:
	
	int                            nparams;
	std::vector<std::string>       keys;
	std::vector<std::tuple<Ts...>> values;
	std::vector<bool>              is_modified_s;
	std::vector<bool>              is_fixed_s;

    public:

	VANLparametersList() : nparams(0) {}

	VANLparametersList(const VANLparametersList& other)
	{
	    nparams       = other.nparams;
	    keys          = other.keys;
	    values        = other.values;
	    is_modified_s = other.is_modified_s;
	    is_fixed_s    = other.is_fixed_s;
	    //std::copy( other.keys.begin(), other.keys.end(), std::back_inserter(keys) );
	    //std::copy( other.values.begin(), other.values.end(), std::back_inserter(values) );
	}
	
	~VANLparametersList()
	{}
	
	VANLparametersList& operator=(const VANLparametersList& other)
	{
	    if ( this == &other ) return *this;
	    nparams       = other.nparams;
	    keys          = other.keys;
	    values        = other.values;
	    is_modified_s = other.is_modified_s;
	    is_fixed_s    = other.is_fixed_s;
	    return *this;
	}
	
	int index(const std::string& key) const
	{
	    auto it = std::find( keys.begin(), keys.end(), key );
	    return (int)std::distance( keys.begin(), it );    
	}
	
	bool exist(const std::string& key) const
	{
	    auto id = index(key);
	    if ( id == (int)keys.size() ) return false;
	    return true;	    
	}

	int define(const std::string& key, Ts ...defvalue)
	{
	    if ( exist(key)==true ) {
		// std::cerr << "*Warning* : A parameter " << key  << " is already defined" << std::endl;
		return -1;
	    }

	    keys.emplace_back(key);
	    //values.emplace_back( std::make_tuple<Ts...>(defvalue...));
	    values.emplace_back( defvalue... );
	    is_modified_s.emplace_back( false );
	    is_fixed_s.emplace_back( false );

	    ++nparams;
	    return 0;
	}
	
	void set(const std::string& key, Ts ...value)
	{
	    if ( exist(key)==true && is_fixed(key)==false ) {
		auto newvalue = std::tuple<Ts...>(value...);
		if ( values[ index(key) ] != newvalue ) is_modified_s[ index(key) ] = true;
		values[ index(key) ] = newvalue;
	    }
	    // else
	    // std::cerr << "*Warning* : A parameter " << key << " is not found" << std::endl; 
	}

	bool is_modified(const std::string& key) const
	{
	    if ( exist(key)==true )
		return is_modified_s[ index(key) ];
	    // else
	    // std::cerr << "*Warning* : A parameter " << key << " is not found" << std::endl;
	    return false;
	}

	bool is_fixed(const std::string& key) const 
	{
	    if ( exist(key)==true )
		return is_fixed_s[ index(key) ];
	    return false;
	}

	void fix(const std::string& key)
	{
	    if ( exist(key)==true )
		is_fixed_s[ index(key) ] = true;
	}
	
	void fix_all()
	{
	    for ( auto&& flag : is_fixed_s ) flag = true;
	}
	
	std::tuple<Ts...> get(const std::string& key) const
	{
	    if ( exist(key)==false ) {
		// std::cerr << "***Error*** : A parameter " << key << " is not found" << std::endl;
		return std::tuple<Ts...>();
	    }
	    return values[ index(key) ];
	    
	}
	
	// std::vector<std::tuple<std::string, std::tuple<Ts...>>> parameter_info(const std::string& key)
	// {
	//     std::vector<std::tuple<std::string, Ts...>> out;
	//     for ( int i=0; i<nparams; ++i ) {
	// 	out.emplace_back( std::make_tuple(keys[i], values[i]) );
	//     }
	//     return out;
	// }
	
	std::tuple<std::string, std::tuple<Ts...>> parameter_info(const std::string& key)
	{
	    if ( exist(key)==false ) return std::tuple<std::string, std::tuple<Ts...>>();
	    return std::make_tuple( keys[ index(key) ], values[ index(key) ] );	    
	}
		
    };

    template<class... Parents>
    class ANLparametrizer : public Parents...
    {
	
    public:

	ANLparametrizer()
	    : Parents()...
	    {	    
		mANLparamsInt             = new ANLparamsInt;            
		mANLparamsString          = new ANLparamsString;         
		mANLparamsDouble          = new ANLparamsDouble;         
		mANLparamsDoubleUnit      = new ANLparamsDoubleUnit;     
		mANLparamsVector3         = new ANLparamsVector3;        
		mANLparamsVector3Unit     = new ANLparamsVector3Unit;    
		mANLparamsBool            = new ANLparamsBool;
		define_parameter<std::string>("class_name", "ANLparametrizer");
	    }	
	
	ANLparametrizer(Parents... parents)
	    : Parents(parents)...
	    {	    
		mANLparamsInt             = new ANLparamsInt;            
		mANLparamsString          = new ANLparamsString;         
		mANLparamsDouble          = new ANLparamsDouble;         
		mANLparamsDoubleUnit      = new ANLparamsDoubleUnit;     
		mANLparamsVector3         = new ANLparamsVector3;        
		mANLparamsVector3Unit     = new ANLparamsVector3Unit;    
		mANLparamsBool            = new ANLparamsBool;
		define_parameter<std::string>("class_name", "ANLparametrizer");
	    }	

	
	template<typename... Ts> anl::ANLparametrizer<Parents...>* define_parameter(const std::string& key, Ts... defvalue)
        {
	    if ( this->get_params_list<Ts...>()==nullptr ) {
		std::cerr << "***Error*** in ANLparametrizer : Type of parameters must be int, string, double(w/ & wo/ unit), vector(3 value of double, w/ & wo/ unit) or bool" << std::endl;
		return this;
	    }	    
	    if ( exist(key)==true ) {
		std::cerr << "*Warning* in ANLparametrizer : A parameter " << key << " is already defined. Ignored this method call." << std::endl;
		return this;
	    }
            this->get_params_list<Ts...>()->define(key, defvalue...);
	    
	    m_list_of_keys.emplace_back( key );	    
	    auto [ k, v ] = this->get_params_list<Ts...>()->parameter_info(key);
	    std::stringstream ss;
	    ss << v;
	    m_dict_of_params.emplace_back( ss.str() );
	    
            return this;
        }
        template<typename... Ts> anl::ANLparametrizer<Parents...>* set_parameter(const std::string& key, Ts... value)
        {
	    if ( this->get_params_list<Ts...>()==nullptr ) {
		std::cerr << "***Error*** in ANLparametrizer : Type of parameters must be int, string, double(w/ & wo/ unit), vector(3 value of double, w/ & wo/ unit) or bool" << std::endl;
		return this;
	    } 
	    if ( exist(key)==false ) {
		std::cerr << "*Warning* in ANLparametrizer : A parameter " << key << " is not found." << std::endl;
		return this;
	    }
	    if ( this->get_params_list<Ts...>()->exist(key)==false ) {
		std::cerr << "*Warning* in ANLparametrizer : Type Error of parameter " << key << ". Ignored this method call." << std::endl;
		return this;
	    }
	    if ( this->get_params_list<Ts...>()->is_fixed(key)==true ) {
		std::cerr << "*Warning* in ANLparametrizer : A parameter " << key << " is already fixed." << std::endl;
		return this;
	    }
            this->get_params_list<Ts...>()->set(key, value...);
            return this;
        }
        template<typename... Ts> std::tuple<Ts...> get_parameter(const std::string& key)
        {
	    if ( this->get_params_list<Ts...>()==nullptr ) {
		std::cerr << "***Error*** in ANLparametrizer : Type of parameters must be int, string, double(w/ & wo/ unit), vector(3 value of double, w/ & wo/ unit) or bool" << std::endl;
		return std::tuple<Ts...>();
	    } 
	    if ( exist(key)==false ) {
		std::cerr << "*Warning* in ANLparametrizer : A parameter " << key << " is not found." << std::endl;
		return std::tuple<Ts...>();
	    }
	    if ( this->get_params_list<Ts...>()->exist(key)==false ) {
		std::cerr << "*Warning* in ANLparametrizer : Type Error of parameter " << key << ". Ignored this method call." << std::endl;
		return std::tuple<Ts...>();
	    }
            return this->get_params_list<Ts...>()->get(key);
        }

	int show_parameters()
        {
	    auto nparams = (int)m_list_of_keys.size();
	    if ( nparams==0 ) return -1;
	    std::cout << std::endl;
	    std::cout << " Parameters of " << m_dict_of_params[0] << " (N=" << nparams << ")" << std::endl;
	    std::cout << "==================================================" << std::endl;
	    std::cout << " Key                      : Value                 " << std::endl;
	    std::cout << "--------------------------------------------------" << std::endl;
	    
	    for ( int i=0; i<nparams; ++i ) {
		auto key = m_list_of_keys[i];
		auto info = m_dict_of_params[i];		
		std::cout << std::setw(25) << key << " = " << info << std::endl;
	    }
	    
	    std::cout << "--------------------------------------------------" << std::endl;
	    return 0;
        }
	
	template<class T> static pybind11::class_<T>& define_common_method(pybind11::class_<T>& pyclass)
	{
	    pyclass.def(pybind11::init<>())
		.def("set_parameter",   &T::template set_parameter<int>)
		.def("set_parameter",   &T::template set_parameter<double>)
		.def("set_parameter",   &T::template set_parameter<std::string>)
		.def("set_parameter",   &T::template set_parameter<double,unit>)
		.def("set_parameter",   &T::template set_parameter<double,double,double>)
		.def("set_parameter",   &T::template set_parameter<double,double,double,unit>)
		.def("switch_flag",     &T::template set_parameter<bool>)
		.def("show_parameters", &T::show_parameters);
	    return pyclass;
	}
	
	
    private:

	std::vector<std::string> m_list_of_keys;
	std::vector<std::string> m_dict_of_params;
	
	ANLparamsInt*            mANLparamsInt;           
	ANLparamsString*         mANLparamsString;        
	ANLparamsDouble*         mANLparamsDouble;        
	ANLparamsDoubleUnit*     mANLparamsDoubleUnit;    
	ANLparamsVector3*        mANLparamsVector3;       
	ANLparamsVector3Unit*    mANLparamsVector3Unit;   
	ANLparamsBool*           mANLparamsBool;
	
	int index(const std::string& key) const
	{
	    auto it = std::find( m_list_of_keys.begin(), m_list_of_keys.end(), key );
	    return (int)std::distance( m_list_of_keys.begin(), it );    
	}	

	bool exist(const std::string& key) const
	{
	    auto id = index(key);
	    if ( id == (int)m_list_of_keys.size() ) return false;
	    return true;	    
	}

	template<typename... Ts> VANLparametersList<Ts...>* get_params_list() { return nullptr; }
	template<> ANLparamsInt*            get_params_list() { return mANLparamsInt; }
	template<> ANLparamsString*         get_params_list() { return mANLparamsString; }
	template<> ANLparamsDouble*         get_params_list() { return mANLparamsDouble; }
	template<> ANLparamsDoubleUnit*     get_params_list() { return mANLparamsDoubleUnit; }
	template<> ANLparamsVector3*        get_params_list() { return mANLparamsVector3; }
	template<> ANLparamsVector3Unit*    get_params_list() { return mANLparamsVector3Unit; }
	template<> ANLparamsBool*           get_params_list() { return mANLparamsBool; }
	
    };
    
}

#endif
