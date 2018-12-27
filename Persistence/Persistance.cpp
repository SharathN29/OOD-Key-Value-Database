/////////////////////////////////////////////////////////////////////
// Persistance.cpp - Contains the required data for the XML file   //
// ver 1.0                                                         //
// Author: Sharath Nagendra, Syracuse University                   //
/////////////////////////////////////////////////////////////////////

#include "Persistance.h"
#include "../DbCore/DbCore.h"

#ifdef TEST_PERSISTANCE

int main() {
	std::cout << "\nBeginning the Persistance test\n";
	Persistence<std::string> p;
	DbCore<std::string> db;
	DbElement<std::string> dbe;
	dbe.name("Sharath");
	dbe.dateTime(DateTime().now());
	dbe.descrip("This has been a stressful day");
	dbe.payLoad("Students of OOD");
	std::vector<std::string> c;
	c.push_back("c1");
	c.push_back("c2");
	dbe.children(c);
	db["Nagendra"] = dbe;
	showDb(db);
	std::string res = toXml(db);
	std::cout << "\nThe output is:\n" << res << "\n";
	db = p.loadXml("test.xml");
	std::cout << "\nDisplaying contents from \"test.xml\" file\n\n";
	showDb(db);
	std::cin.get();
	return 0;
}

#endif // TEST_PERSISTANCE