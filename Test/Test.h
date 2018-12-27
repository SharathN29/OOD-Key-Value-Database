///////////////////////////////////////////////////////////////////////
// Test.h - Contains test cases that checkes the functionality       //
// ver 1.0                                                           //
// Author: Sharath Nagendra, Object Oriented Design, Spring 2018     //
///////////////////////////////////////////////////////////////////////

#pragma once
#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../DateTime/DateTime.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Persistence/Persistance.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <regex>
#include <fstream>

using namespace NoSqlDb;

//----< Reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument

class DbProvider
{
public:
	DbCore<Payload>& db() { return db_; }
private:
	static DbCore<Payload> db_;
};

DbCore<Payload> DbProvider::db_;

////////////////////////////////////////////////////////////////
//Testing the requirements

//----< Demonstrating Requirement #1 >--------------------------

bool testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would  not compile unless C++11
}

//----< Demonstrating Requirement #2 >--------------------------

bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< Demonstrating Requirement #3a >--------------------------

bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - Creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbCore<Payload> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<Payload> demoElem;
	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	//demoElem.payLoad("The good news is ...");

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	//if (demoElem.payLoad() != "The good news is ...")
	//return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}

//----< Demonstrating Requirement #3b >--------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - Creating DbCore");

	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem = db["Fawcett"];

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	//demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Artemis");
	//demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;

	//demoElem.payLoad("You demonstrated Requirement #4");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;

	//demoElem.payLoad("You demonstrated Requirement #5");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	//demoElem.payLoad("You demonstrated Requirement #6");
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;

	//demoElem.payLoad("You demonstrated Requirement #7");
	demoElem.name("Sharath");
	db["Nagendra"] = demoElem;

	if (db.size() != 7)
		return false;

	std::cout << "\n  After adding elements with keys: ";
	DbCore<Payload>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  Make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");
	db["Fawcett"].children().push_back("Nagendra");

	showHeader();
	showElem(db["Fawcett"]);

	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");
	db["Salman"].children().push_back("Sharath");

	// display the results

	putLine();
	std::cout << "\n  Showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  Database keys are: ";
	showKeys(db);
	dbp.db() = db;
	putLine();
	return true;
}

//----< Demonstrating Requirement #4a >--------------------------

bool testR4a()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	bool retval = false;
	demoElem.name("Sharath");
	demoElem.descrip("Student of CSE687");
	demoElem.dateTime(DateTime().now());
	//demoElem.payLoad("You should try ...");
	Utilities::title("Demonstrating Requirement #4a - Adding new key to the database");
	std::cout << "\n A new key Nagendra is being added and its corresponding data is added to the database.\n";
	if (demoElem.name() != "Sharath")
	{
		retval = false;
	}
	if (demoElem.descrip() != "Student of CSE687")
	{
		retval = false;
	}

	//display the results

	showHeader();
	showElem(demoElem);
	db.keyadd(db, "Nagendra", demoElem);
	dbp.db() = db;
	putLine();
	showDb(db);
	putLine();
	std::cout << "\n  Database keys are :";
	showKeys(db);
	putLine();
	return true;
}

//----< Demonstrating Requirement #4b >--------------------------

bool testR4b()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	Utilities::title("Demonstrating Requirement #4b - Deletion of key from the database");
	std::cout << "\n Data with key - Arora and its values are being deleted. \n";
	demoElem = db["Arora"];

	//display the results

	showHeader();
	showElem(db["Arora"]);
	db.deleteKey(db, "Arora");
	dbp.db() = db;
	putLine();
	showDb(db);
	putLine();
	std::cout << "\n  Database keys are :";
	showKeys(db);
	putLine();
	return true;
}

//----< Demonstrating Requirement #5 >--------------------------

bool testR5()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	Utilities::title("Demonstrating Requirement #5 - Editing the value field in the database");
	putLine();
	std::cout << "Description of the key - Sun is being changed to Not a student anymore\n";

	//display the results

	std::cout << "\n  Before editing the description field\n";
	showDb(db);
	DbElement<Payload> tempelem = db["Sun"];
	tempelem.descrip("Not a student anymore");
	db.editKey(db, "Sun", tempelem);
	dbp.db() = db;
	putLine();
	std::cout << "\n  After editing the description field\n";
	showDb(db);
	putLine();
	return true;
}

//----< Demonstrating Requirement #6a >--------------------------

bool testR6a()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	Utilities::title("Demonstrating Requirement #6a - Retrieving the different values of a given key");
	putLine();
	std::cout << "\nAll the values of the key which starts with an S is being displayed.\n";
	QueryExecuter<std::string> q1;
	std::string tempst = "((.*)(S)(.*))";
	DateTime cur = DateTime().now();
	q1.select("key").from(db).where((tempst), db, cur, cur);
	PrintKeys(q1.FinalKeys);
	putLine();
	return true;
}

//----< Demonstrating Requirement #6b >--------------------------

bool testR6b()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	Utilities::title("Demonstrating Requirement #6b - Retrieving the different childrens of a given key");
	putLine();
	std::cout << "\n All the children whose key starts with Faw is being displayed\n";
	QueryExecuter<std::string> q1;
	std::string tempst = "((Faw)(.*))";
	DateTime cur = DateTime().now();
	q1.select("keychildren").from(db).where((tempst), db, cur, cur);
	PrintKeys(q1.FinalKeys);
	putLine();
	return true;
}

//----< Demonstrating Requirement #6c >--------------------------

bool testR6c()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	Utilities::title("Demonstrating Requirement #6c - Retrieving the set of all keys matching a specified regular expression");
	std::cout << "\nDisplaying all the key which has word TA in their discription is being displayed\n";
	QueryExecuter<std::string> q1;
	std::string tempst = "((.*)(TA)(.*))";
	DateTime cur = DateTime().now();
	q1.select("description").from(db).where((tempst), db, cur, cur);
	putLine();
	std::cout << "The different keys matching a specific regular expression are:\n";
	putLine();
	for (auto key : q1.FinalKeys)
	{
		std::cout << key << "\n";
	}
	putLine();
	return true;
}

//----< Demonstrating Requirement #6d >--------------------------

bool testR6d()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	Utilities::title("Demonstrating Requirement #6d - Retrieving the set of all keys matching a specified regular expression in a metadata");
	std::cout << "Displaying all the keys whose name field starts with an A";
	QueryExecuter<std::string> q1;
	std::string tempst = "((A)(.*))";
	DateTime cur = DateTime().now();
	q1.select("name").from(db).where((tempst), db, cur, cur);
	putLine();
	std::cout << "The keys matching a specific regular expression in the metadata are:\n";
	putLine();
	for (auto key : q1.FinalKeys)
	{
		std::cout << key << "\n";
	}
	putLine();
	return true;
}


//----< Demonstrating Requirement #7 >--------------------------

bool testR7()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbElement<Payload> demoElem;
	Utilities::title("Demonstrating Requirement #7 - Performing and-ing and or-ing of two queries");
	std::cout << "Query 1 retrives all the keys whose description has CSE in it\n";
	std::cout << "Query 2 retrives all the keys whose name starts with Am\n ";
	QueryExecuter<std::string> q1, q2;
	std::string tempst = "((.*)(CSE)(.*))";
	std::string tempst1 = "((Am)(.*))";
	DateTime cur = DateTime().now();
	q1.select("description").from(db).where((tempst), db, cur, cur);
	q2.select("name").from(db).where((tempst1), db, cur, cur);
	std::cout << "From query 1\n";
	for (auto key : q1.FinalKeys)
	{
		std::cout << key << "\n";
	}
	putLine();
	std::cout << "  From query 2\n";
	for (auto key : q2.FinalKeys)
	{
		std::cout << key << "\n";
	}

	std::cout << "\n ORing between the keys from query 1 and query 2\n";
	putLine();
	QueryAndOr(q1.FinalKeys, q2.FinalKeys, "OR");
	//PrintKeys(q1.FinalKeys);

	std::cout << "\n ANDing\n between the keys from query 1 and query 2";
	putLine();
	QueryAndOr(q1.FinalKeys, q2.FinalKeys, "AND");
	//PrintKeys(q1.FinalKeys);
	putLine();
	std::cout << " Before editing\n";
	showDb(db);
	DbElement<Payload> tempelem = db["Arora"];
	tempelem.descrip("Student");
	db.editKey(db, "Arora", tempelem);
	putLine();
	std::cout << " \n After editing\n";
	showDb(db);
	std::cout << "\n";
	std::cout << "  \n Testing for children key search\n";
	q2.select("children").from(db).where((tempst1), db, cur, cur);
	PrintKeys(q2.FinalKeys);
	putLine();
	return true;
}

//----< requirement #8 >------------------------------------------
bool testR8()
{
	Utilities::title("Demonstrating Requirement #8 - Persisting and Restoring the database to and from XML file");
	DbCore<std::string> db;
	Persistence<std::string> p;

	//Converts XML to DbCore object
	db = p.convertIntoObj("../Persistence/test.xml");

	//Converts DbCore to XML
	std::string res = toXml(db);
	std::cout << "\n\n Result after converting to XML: " << res << "\n\n";

	return true;
}

//----< Demonstrating Requirement #9 >--------------------------

bool testR9()
{
	DbCore<Payload> db;
	DbElement<Payload> demoElem;
	QueryExecuter<Payload> q;
	DateTime cur;
	Utilities::title("Demonstrating Requirement #9 - Demonstrating the payload");
	demoElem.name("Example");
	demoElem.descrip("Type");
	db["Value"] = demoElem;
	Payload p;
	std::string tem1 = "example1";
	std::string tem2 = "example2";
	std::string tem3 = "filepath";
	std::vector<std::string> ex;
	ex.push_back(tem1);
	ex.push_back(tem2);
	p.category(ex);
	p.fpath(tem3);
	demoElem.payLoad(p);
	showElem(demoElem);
	q.select("payload").from(db).where(("(ex)(.*)"), db, cur, cur);
	showDb(db);
	putLine();
	return true;
}

//----< Demonstrating Requirement #10 >--------------------------

bool testR10()
{
	Utilities::title("Demonstrating Requirement #10 - Implementation of the solution in packages: DBCore, Query, Test, Persistence, Executive");
	std::cout << "\nThe structure of the package is shownp.\n";
	std::cout << "DBCore: Implementing the NoSqlDb, it also provides different functionality for the database. \n";
	std::cout << "Query: Implements the querying of the database.\n";
	std::cout << "Persistence: Contains the required functionality to persist the database to an XML file.\n";
	std::cout << "Test: Has all the functions to execute the different test cases.\n";
	std::cout << "Executive: This has the starting point of execution of the test cases which are defined in Test package.\n";
	std::cout << "\nAlternatively, running \"dir\" command under solution folder displays the packages.\n\n";
	return true;
}

//----< requirement #11 >------------------------------------------
bool testR11() {
	Utilities::title("Demonstrating Requirement #11 - Package structure and dependence of the project being described by the XML");
	std::string filePath = "..//OOProject.xml";
	std::ifstream in(filePath);
	if (!in.good())
		throw(std::exception(("can't open source file " + filePath).c_str()));
	std::ostringstream out;
	out << in.rdbuf();
	std::string forOpening = std::move(out.str());
	std::cout << "\nThe XML with package structure is:\n" << forOpening << "\n\n";
	return true;
}

//----< requirement #12 >------------------------------------------
bool testR12() {
	Utilities::title("Demonstrating Requirement #12 - The requirements #2 to #9 are met");
	std::cout << "\nIf the statement \"all tests passed\" is printed in the end, requirement #12 is met.\n";
	return true;
}

//----< requirement #13 >------------------------------------------
bool testR13() {
	Utilities::title("Demonstrating Requirement #13 - Package diagram PDF");
	std::string path = "..\\PackageDiagram.pdf";
	std::cout << "\nThe PDF is present in path: " << path << " i.e. in the same path as the solution file.\n\n";
	return true;
}