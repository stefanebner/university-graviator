//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Abstract Object Factory
//	to easily create new objects, is a template so in the
//	end we got 2 factories, one for players and the other
//	for gravitational objects
//
//*********************************************************

#ifndef SERVER_OBJECT_FACTORY_H
#define SERVER_OBJECT_FACTORY_H

#include <map>
#include <string>

#define VT(t,c) tCreatorMap::value_type((t), (c))

using std::string;


template<class T>
class ServerObjectFactory
{
public:
	typedef T* (*tCreator) ();
	typedef std::map<std::string, tCreator> tCreatorMap;

	bool Register(const char * pType, tCreator aCreator);
	T* create(const char* pType);
	static ServerObjectFactory<T>& getInstance();

protected:
	static ServerObjectFactory* mFactoryInstance;
	ServerObjectFactory(const ServerObjectFactory&) {}
	ServerObjectFactory() {}
	~ServerObjectFactory() {}

	tCreatorMap mCreators;
};

template<class T>
ServerObjectFactory<T>* ServerObjectFactory<T>::mFactoryInstance = NULL;

template<class T>
ServerObjectFactory<T>& ServerObjectFactory<T>::getInstance()
{
	if ( !mFactoryInstance )
		mFactoryInstance = new ServerObjectFactory();
	return *mFactoryInstance;
}

template<class T>
bool ServerObjectFactory<T>::Register(const char* pType, tCreator aCreator)
{
	string str = string (pType);
	return mCreators.insert(VT(str, aCreator)).second;
}

template<class T> 
T* ServerObjectFactory<T>::create(const char* pType)
{
	tCreatorMap::iterator i = mCreators.find(string(pType));

	if(i!=mCreators.end())
	{
		tCreator aCreator = (*i).second;
		return aCreator();
	}

	return 0;
}



#endif