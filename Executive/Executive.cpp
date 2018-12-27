///////////////////////////////////////////////////////////////////////
// Test_Project1 - Contains function calls for the test cases        //
// ver 1.0                                                           //
// Author: Sharath Nagendra, Object Oriented Design, Spring 2018     //
///////////////////////////////////////////////////////////////////////

#include "../Test/Test.h"
#ifdef TESTSUITE
using namespace Utilities;
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	// Define test structures with test function and message.

	TestExecutive ex;
	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4a{ testR4a, "Addition of a new Key to the Database" };
	TestExecutive::TestStr ts4b{ testR4b, "Deletion of a new Key from the Database" };
	TestExecutive::TestStr ts5{ testR5, "Editing the key in the database" };
	TestExecutive::TestStr ts6a{ testR6a, "Retrieving the value of a given key" };
	TestExecutive::TestStr ts6b{ testR6b, "Retrieving the children of a given key" };
	TestExecutive::TestStr ts6c{ testR6c, "Retrieving the set of all keys based on the specified regular expression" };
	TestExecutive::TestStr ts6d{ testR6d, "Retrieving the set of all keys based on the specified regular expression in a metadata" };
	TestExecutive::TestStr ts7{ testR7, "And-ing and Or-ing of queries" };
	TestExecutive::TestStr ts8{ testR8, "Persisting a collection of database" };
	TestExecutive::TestStr ts9{ testR9, "Payload Demonstration" };
	TestExecutive::TestStr ts10{ testR10, "Implementation of the solution in the packages" };
	TestExecutive::TestStr ts11{ testR11, "Describes the projects package structure and dependency relationship" };
	TestExecutive::TestStr ts12{ testR12, "Demostrates that all the requirements should be met" };
	TestExecutive::TestStr ts13{ testR13, "PDF file of the package diagram" };

	// Register test structures with TestExecutive instance, ex

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4a);
	ex.registerTest(ts4b);
	ex.registerTest(ts5);
	ex.registerTest(ts6a);
	ex.registerTest(ts6b);
	ex.registerTest(ts6c);
	ex.registerTest(ts6d);
	ex.registerTest(ts7);
	ex.registerTest(ts8);
	ex.registerTest(ts9);
	ex.registerTest(ts10);
	ex.registerTest(ts11);
	ex.registerTest(ts12);
	ex.registerTest(ts13);
	// Run the tests

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  All tests passed";
	else
		std::cout << "\n  At least one test failed";

	putLine(2);
	std::cin.get();
	std::getchar();
	//std::getchar();
	return 0;
}
#endif // TESTSUITE
