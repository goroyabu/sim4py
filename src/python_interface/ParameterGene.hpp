/**
   @date 2020/04/02
   @date 2020/06/23 switch_flag, fix_parameters, unit
**/
#ifndef ParameterGene_hpp
#define ParameterGene_hpp

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <tuple>
#include <map>

#include <pybind11/pybind11.h>

#include "UnitDefinition.hpp"

namespace sim4py
{
    class UnitDefinition;
    using unit = UnitDefinition;
    template<typename... Ts> class VUIparametersList;
    
    using UIparamsInt         = VUIparametersList<int>;
    using UIparamsString      = VUIparametersList<std::string>;    
    using UIparamsDouble      = VUIparametersList<double>;
    using UIparamsDoubleUnit  = VUIparametersList<double, unit>;
    using UIparamsVector3     = VUIparametersList<double, double, double>;
    using UIparamsVector3Unit = VUIparametersList<double, double, double, unit>;
    using UIparamsBool        = VUIparametersList<bool>;
    using ANLflags            = UIparamsBool;

    template<typename Tuple,size_t N=0> std::ostream& stream_tuple(std::ostream& os, const Tuple& tup)
    {
	if constexpr ( std::tuple_size<Tuple>::value==1 && N==0 ) {
		os << std::get<N>(tup);
		return os;
	    }
	if constexpr ( N < std::tuple_size<Tuple>::value ) {
		if constexpr ( N==0 ) os << "(";
		os << std::get<N>(tup);
		if constexpr ( N < std::tuple_size<Tuple>::value-1 ) os << ", ";
		stream_tuple<Tuple, N+1>(os, tup);
	    }
	else os << ")";
	return os;
    }
    
    template<size_t N=0, typename ...Ts> std::ostream& operator<< (std::ostream& os, const std::tuple<Ts...>& tup)
    {
        return stream_tuple<std::tuple<Ts...>,N>(os, tup);
    }

    // template<class T> pybind11::class_<T>& define_common_method(pybind11::class_<T>& pyclass)
    // {
    // 	pyclass.def(pybind11::init<>())
    // 	    .def("SetParameter",   &T::template SetParameter<int>)
    // 	    .def("SetParameter",   &T::template SetParameter<double>)
    // 	    .def("SetParameter",   &T::template SetParameter<std::string>)
    // 	    .def("SetParameter",   &T::template SetParameter<double,unit>)
    // 	    .def("SetParameter",   &T::template SetParameter<double,double,double>)
    // 	    .def("SetParameter",   &T::template SetParameter<double,double,double,unit>)
    // 	    .def("SwitchFlag",     &T::template SetParameter<bool>)
    // 	    .def("ShowParameters", &T::ShowParameters);
    // 	return pyclass;
    // }

    template<class Child, class...Ancestors> pybind11::class_<Child, Ancestors...>& define_common_method(pybind11::class_<Child, Ancestors...>& pyclass)
    {
	pyclass
	    .def("SetParameter",   &Child::template SetParameter<int>)
	    .def("SetParameter",   &Child::template SetParameter<double>)
	    .def("SetParameter",   &Child::template SetParameter<std::string>)
	    .def("SetParameter",   &Child::template SetParameter<double,unit>)
	    .def("SetParameter",   &Child::template SetParameter<double,double,double>)
	    .def("SetParameter",   &Child::template SetParameter<double,double,double,unit>)
	    .def("SwitchFlag",     &Child::template SetParameter<bool>)
	    .def("ShowParameters", &Child::ShowParameters);
	return pyclass;
    }

    template<class Child, class...Ancestors> pybind11::class_<Child, Ancestors...>& define_as_singleton(pybind11::class_<Child, Ancestors...>& pyclass)
    {
	static Child* instance = nullptr;	
	pyclass.def_static("Instance", [](){ if ( instance==nullptr ) instance = new Child(); return instance; },
			   pybind11::return_value_policy::reference);
	// pyclass.def(pybind11::init( [](){ if ( instance==nullptr ) instance = new Child(); return instance; }),
	// 		   pybind11::return_value_policy::reference);	
	return pyclass;
    }

    
    template<typename... Ts>
    class VUIparametersList
    {
	
    private:
	
	int                            nparams;
	std::vector<std::string>       keys;
	std::vector<std::tuple<Ts...>> values;
	std::vector<bool>              is_modified_s;
	std::vector<bool>              is_fixed_s;

    public:

	VUIparametersList() : nparams(0) {}

	VUIparametersList(const VUIparametersList& other)
	{
	    nparams       = other.nparams;
	    keys          = other.keys;
	    values        = other.values;
	    is_modified_s = other.is_modified_s;
	    is_fixed_s    = other.is_fixed_s;
	}
	
	virtual ~VUIparametersList()
	{}
	
	VUIparametersList& operator=(const VUIparametersList& other)
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
    
    template<class ClassIDtype>
    class TParameterGene
    {
	
    public:

	// using Parents::Parents...;
	
	TParameterGene(const ClassIDtype& class_name)
	{
	    // InitializeListOfParameters();
	    // mUIparamsInt             = std::make_unique<UIparamsInt>();            
	    // mUIparamsString          = std::make_unique<UIparamsString>();         
	    // mUIparamsDouble          = std::make_unique<UIparamsDouble>();         
	    // mUIparamsDoubleUnit      = std::make_unique<UIparamsDoubleUnit>();     
	    // mUIparamsVector3         = std::make_unique<UIparamsVector3>();        
	    // mUIparamsVector3Unit     = std::make_unique<UIparamsVector3Unit>();    
	    // mUIparamsBool            = std::make_unique<UIparamsBool>();	    
	    mUIparamsInt             = new UIparamsInt;            
	    mUIparamsString          = new UIparamsString;         
	    mUIparamsDouble          = new UIparamsDouble;         
	    mUIparamsDoubleUnit      = new UIparamsDoubleUnit;     
	    mUIparamsVector3         = new UIparamsVector3;        
	    mUIparamsVector3Unit     = new UIparamsVector3Unit;    
	    mUIparamsBool            = new UIparamsBool;
	    DefineParameter<std::string>("class_name", static_cast<std::string>(class_name) );
	}	
	
	TParameterGene(const TParameterGene<ClassIDtype>& other)
	{
	    auto [ class_name ] = other.GetParameter<std::string>("class_name");
	    
	    mUIparamsInt             = new UIparamsInt( *(other.mUIparamsInt) );            
	    mUIparamsString          = new UIparamsString( *(other.mUIparamsString) );         
	    mUIparamsDouble          = new UIparamsDouble( *(other.mUIparamsDouble) );         
	    mUIparamsDoubleUnit      = new UIparamsDoubleUnit( *(other.mUIparamsDoubleUnit) );     
	    mUIparamsVector3         = new UIparamsVector3( *(other.mUIparamsVector3) );        
	    mUIparamsVector3Unit     = new UIparamsVector3Unit( *(other.mUIparamsVector3Unit) );    
	    mUIparamsBool            = new UIparamsBool( *(other.mUIparamsBool) );

	    // mUIparamsInt             = std::make_unique<UIparamsInt>( *(other.mUIparamsInt) );            
	    // mUIparamsString          = std::make_unique<UIparamsString>( *(other.mUIparamsString) );         
	    // mUIparamsDouble          = std::make_unique<UIparamsDouble>( *(other.mUIparamsDouble) );         
	    // mUIparamsDoubleUnit      = std::make_unique<UIparamsDoubleUnit>( *(other.mUIparamsDoubleUnit) );     
	    // mUIparamsVector3         = std::make_unique<UIparamsVector3>( *(other.mUIparamsVector3) );        
	    // mUIparamsVector3Unit     = std::make_unique<UIparamsVector3Unit>( *(other.mUIparamsVector3Unit) );    
	    // mUIparamsBool            = std::make_unique<UIparamsBool>( *(other.mUIparamsBool) );	    

	    // mUIparamsInt             = std::move( other.mUIparamsInt );
	    // mUIparamsString          = std::move( other.mUIparamsString );
	    // mUIparamsDouble          = std::move( other.mUIparamsDouble );         
	    // mUIparamsDoubleUnit      = std::move( other.mUIparamsDoubleUnit );     
	    // mUIparamsVector3         = std::move( other.mUIparamsVector3 );        
	    // mUIparamsVector3Unit     = std::move( other.mUIparamsVector3Unit );    
	    // mUIparamsBool            = std::move( other.mUIparamsBool );	
	    DefineParameter<std::string>("class_name", static_cast<std::string>(class_name) );
	}
	
	// ParameterGene(Parents... parents)
	//     {
	// 	mUIparamsInt             = new UIparamsInt;            
	// 	mUIparamsString          = new UIparamsString;         
	// 	mUIparamsDouble          = new UIparamsDouble;         
	// 	mUIparamsDoubleUnit      = new UIparamsDoubleUnit;     
	// 	mUIparamsVector3         = new UIparamsVector3;        
	// 	mUIparamsVector3Unit     = new UIparamsVector3Unit;    
	// 	mUIparamsBool            = new UIparamsBool;
	// 	DefineParameter<std::string>("class_name", "ParameterGene");
	// 	// InitializeListOfParameters();
	//     }	

	virtual ~TParameterGene()
	{
	    delete mUIparamsInt;
	    delete mUIparamsString;
	    delete mUIparamsDouble;
	    delete mUIparamsDoubleUnit;
	    delete mUIparamsVector3;
	    delete mUIparamsVector3Unit;
	    delete mUIparamsBool;
	}

	TParameterGene<ClassIDtype>& operator=(const TParameterGene<ClassIDtype>& other)
	{
	    if ( this == &other ) return *this;
	    *(this->mUIparamsInt)             = *(other.mUIparamsInt);
	    *(this->mUIparamsString)          = *(other.mUIparamsString);
	    *(this->mUIparamsDouble)          = *(other.mUIparamsDouble);      
	    *(this->mUIparamsDoubleUnit)      = *(other.mUIparamsDoubleUnit);     
	    *(this->mUIparamsVector3)         = *(other.mUIparamsVector3);        
	    *(this->mUIparamsVector3Unit)     = *(other.mUIparamsVector3Unit);    
	    *(this->mUIparamsBool)            = *(other.mUIparamsBool);
	    return *this;
	}
	
	// virtual void InitializeListOfParameters()
	// {
	//     if ( is_already_init==false ) {
	// 	mUIparamsInt             = new UIparamsInt;            
	// 	mUIparamsString          = new UIparamsString;         
	// 	mUIparamsDouble          = new UIparamsDouble;         
	// 	mUIparamsDoubleUnit      = new UIparamsDoubleUnit;     
	// 	mUIparamsVector3         = new UIparamsVector3;        
	// 	mUIparamsVector3Unit     = new UIparamsVector3Unit;    
	// 	mUIparamsBool            = new UIparamsBool;
	// 	DefineParameter<std::string>("class_name", "ParameterGene");
	//     }
	//     is_already_init = true;
	// }

	// template<typename... Ts> ParameterGene<Parents...>* DefineParameter(const std::string& key, Ts... defvalue)
	template<typename... Ts> TParameterGene* DefineParameter(const std::string& key, Ts... defvalue)
        {
	    // if ( is_already_init==false ) InitializeListOfParameters();
	    
	    if ( this->get_params_list<Ts...>()==nullptr ) {
		std::cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : Type of parameters must be int, string, double(w/ & wo/ unit), vector(3 value of double, w/ & wo/ unit) or bool" << std::endl;
		return this;
	    }	    
	    if ( exist(key)==true ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : A parameter " << key << " is already defined. Ignored this method call." << std::endl;
		return this;
	    }
            this->get_params_list<Ts...>()->define(key, defvalue...);

	    m_dict_index[ key ] = (int)m_list_of_keys.size();
	    m_list_of_keys.emplace_back( key );	    
	    auto [ k, v ] = this->get_params_list<Ts...>()->parameter_info(key);
	    std::stringstream ss;
	    ss << v;
	    m_dict_of_params.emplace_back( ss.str() );
	    
            return this;
        }

	// template<typename... Ts> ParameterGene<Parents...>* SetParameter(const std::string& key, Ts... value)
        template<typename... Ts> TParameterGene* SetParameter(const std::string& key, Ts... value)
        {
	    // if ( is_already_init==false ) InitializeListOfParameters();
	    
	    if ( this->get_params_list<Ts...>()==nullptr ) {
		std::cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : Type of parameters must be int, string, double(w/ & wo/ unit), vector(3 value of double, w/ & wo/ unit) or bool" << std::endl;
		return this;
	    } 
	    if ( exist(key)==false ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : A parameter " << key << " is not found." << std::endl;
		return this;
	    }
	    if ( this->get_params_list<Ts...>()->exist(key)==false ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : Type Error of parameter " << key << ". Ignored this method call." << std::endl;
		return this;
	    }
	    if ( this->get_params_list<Ts...>()->is_fixed(key)==true ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : A parameter " << key << " is already fixed." << std::endl;
		return this;
	    }
            this->get_params_list<Ts...>()->set(key, value...);

	    auto [ k, v ] = this->get_params_list<Ts...>()->parameter_info(key);
	    std::stringstream ss;
	    ss << v;
	    //m_dict_of_params.emplace_back( ss.str() );
	    m_dict_of_params[ m_dict_index[ key ] ] = ss.str();
	    
            return this;
        }

        template<typename... Ts> std::tuple<Ts...> GetParameter(const std::string& key) const
        {
	    // if ( is_already_init==false ) {
	    // 	std::cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : Initialization of ParameterGene is not done." << std::endl;
	    // 	return std::tuple<Ts...>();
	    // }
	    
	    if ( this->get_params_list<Ts...>()==nullptr ) {
		std::cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : Type of parameters must be int, string, double(w/ & wo/ unit), vector(3 value of double, w/ & wo/ unit) or bool" << std::endl;
		return std::tuple<Ts...>();
	    } 
	    if ( exist(key)==false ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : A parameter " << key << " is not found." << std::endl;
		return std::tuple<Ts...>();
	    }
	    if ( this->get_params_list<Ts...>()->exist(key)==false ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : Type Error of parameter " << key << ". Ignored this method call." << std::endl;
		return std::tuple<Ts...>();
	    }
            return this->get_params_list<Ts...>()->get(key);
        }	

        template<typename... Ts> bool IsModifiedParameter(const std::string& key) const
        {
	    if ( this->get_params_list<Ts...>()==nullptr ) {
		std::cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : Type of parameters must be int, string, double(w/ & wo/ unit), vector(3 value of double, w/ & wo/ unit) or bool" << std::endl;
		return false;
	    } 
	    if ( exist(key)==false ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : A parameter " << key << " is not found." << std::endl;
		return false;
	    }
	    if ( this->get_params_list<Ts...>()->exist(key)==false ) {
		std::cerr << "*Warning* in " << GetParameter<std::string>("class_name") << " : Type Error of parameter " << key << ". Ignored this method call." << std::endl;
		return false;
	    }
	    return this->get_params_list<Ts...>()->is_modified(key);
	}
	
	int ShowParameters() const
        {
	    // if ( is_already_init==false ) return -1;
	    
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

	// std::string ClassName() const
	// {
	//     return std::get<0>( GetParameter<std::string>("class_name") );
	// }
	
	// template<class T> friend pybind11::class_<T>& define_common_method(pybind11::class_<T>& pyclass)
	// {
	//     pyclass.def(pybind11::init<>())
	// 	.def("SetParameter",   &T::template SetParameter<int>)
	// 	.def("SetParameter",   &T::template SetParameter<double>)
	// 	.def("SetParameter",   &T::template SetParameter<std::string>)
	// 	.def("SetParameter",   &T::template SetParameter<double,unit>)
	// 	.def("SetParameter",   &T::template SetParameter<double,double,double>)
	// 	.def("SetParameter",   &T::template SetParameter<double,double,double,unit>)
	// 	.def("SwitchFlag",     &T::template SetParameter<bool>)
	// 	.def("ShowParameters", &T::ShowParameters);
	//     return pyclass;
	// }
	
	
    private:

	// bool is_already_init;
	
	std::vector<std::string> m_list_of_keys;
	std::vector<std::string> m_dict_of_params;
	std::map<std::string, int> m_dict_index;
	
	// std::unique_ptr<UIparamsInt>            mUIparamsInt;           
	// std::unique_ptr<UIparamsString>         mUIparamsString;        
	// std::unique_ptr<UIparamsDouble>         mUIparamsDouble;        
	// std::unique_ptr<UIparamsDoubleUnit>     mUIparamsDoubleUnit;    
	// std::unique_ptr<UIparamsVector3>        mUIparamsVector3;       
	// std::unique_ptr<UIparamsVector3Unit>    mUIparamsVector3Unit;   
	// std::unique_ptr<UIparamsBool>           mUIparamsBool;

	UIparamsInt*            mUIparamsInt;           
	UIparamsString*         mUIparamsString;        
	UIparamsDouble*         mUIparamsDouble;        
	UIparamsDoubleUnit*     mUIparamsDoubleUnit;    
	UIparamsVector3*        mUIparamsVector3;       
	UIparamsVector3Unit*    mUIparamsVector3Unit;   
	UIparamsBool*           mUIparamsBool;
	
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

	template<typename... Ts> VUIparametersList<Ts...>* get_params_list() const { return nullptr; }
	//{ throw std::invalid_argument("TParameterGene::get_params_list is a function template"); }
	template<> UIparamsInt*            get_params_list() const { return mUIparamsInt; }
	template<> UIparamsString*         get_params_list() const { return mUIparamsString; }
	template<> UIparamsDouble*         get_params_list() const { return mUIparamsDouble; }
	template<> UIparamsDoubleUnit*     get_params_list() const { return mUIparamsDoubleUnit; }
	template<> UIparamsVector3*        get_params_list() const { return mUIparamsVector3; }
	template<> UIparamsVector3Unit*    get_params_list() const { return mUIparamsVector3Unit; }
	template<> UIparamsBool*           get_params_list() const { return mUIparamsBool; }
	
    };
    
    using ParameterGene = TParameterGene<std::string>;
    
}

#endif
