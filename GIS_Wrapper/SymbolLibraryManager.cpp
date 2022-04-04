#include "SymbolLibraryManager.h"


SymbolLibraryManager::SymbolLibraryManager(void)
{
}


SymbolLibraryManager::~SymbolLibraryManager(void)
{
}

//int SymbolLibraryManager::InsertSymbol( int key, Symbol* symbl )
//{
//	symbolMap.insert(key, symbl);
//	return true;
//}
//
//int SymbolLibraryManager::RemoveSymbol( int key )
//{
//	return symbolMap.remove(key);
//}
//
//Symbol* SymbolLibraryManager::FindSymbol( int key )
//{
//	return symbolMap[key];
//}

int SymbolLibraryManager::LoadSymbolLibrary( QString& libname )
{
	QRegExp rx("/*.");
	
	QString name = libname.section(rx, 0);
	QLibrary* lib = new QLibrary(name);

	if(lib->load())
	{
		symbolLibMap.insert(name, lib);
	}
	else
	{
		if(lib)
			delete lib;

		symbolLibMap.insert(name, 0);
	}
	return true;
}

int SymbolLibraryManager::LoadSymbolLibrary( QStringList& libnamelist )
{
	for(QStringList::Iterator iter = libnamelist.begin(); iter != libnamelist.end(); iter++)
	{
		LoadSymbolLibrary((*iter));
	}
	return true;
}

int SymbolLibraryManager::LoadSymbol()
{
	for(SYMBOL_LIB_MAP::Iterator b = symbolLibMap.begin(); b != symbolLibMap.end(); b++)
	{
		(*b);
	}
	return true;
}

P_SYMBOL_LIB_MAP SymbolLibraryManager::SymbolLibraryMap()
{
	return &symbolLibMap;
}

QLibrary* SymbolLibraryManager::FindSymbolLibrary( QString& libname )
{
	return symbolLibMap[libname];
}

int SymbolLibraryManager::InsertSymbolLibrary( QString& libname, QLibrary* libpointer)
{
	symbolLibMap.insert(libname, libpointer);
	return true;
}

void SymbolLibraryManager::RemoveSymbolLibrary( QString& libname )
{
	symbolLibMap.remove(libname);
}

P_SYMBOL_FACTORY_MAP* SymbolLibraryManager::GetSymbolFactory(QString& libname)
{
	return 0;
}
