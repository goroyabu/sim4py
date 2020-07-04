
#ifndef SingletonGene_hpp
#define SingletonGene_hpp

namespace sim4py
{
    template<class T>
    class SingletonGene
    {
	
    public:

	static T* Instance()
	{
	    
	}
	

    private:

	static T* instance;
	
    }
