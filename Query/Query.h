#pragma once
/////////////////////////////////////////////////////////////////////////////////
// Query.h - Implements queries to perform various operations like "and"ing    //
// and "or"ing																   //
// ver 1.0                                                                     // 
// Author: Sharath Nagendra, Syracuse University, Spring 2018 				   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains class QueryExecutor where:
* - we fetch DbElement as per query requirements
* - perform "add"ing and "or"ing of multiple queries
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Query.h, Query.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 6 Feb 2018
* - first release
*/
#include "../DbCore/DbCore.h"
#include "../DateTime/DateTime.h"
#include <vector>

using namespace NoSqlDb;

///////////////////////////////////////////////////////////////////////
// Make sure the comparing date is within the given date time range

bool compdate(DateTime startdate, DateTime enddate, DateTime compdate)
{
	bool retval = false;
	if ((enddate < compdate) & (startdate < compdate))
	{
		retval = true;
	}
	return retval;
}

template <typename T>

/////////////////////////////////////////////////////////////////////////////////
// QueryExecutor class 
//It has the queries to get the values of DbElement like name, 
//date time, payload, child, description

class QueryExecuter {
public:

	using Key = std::string;
	using Keys = std::vector<Key>;
	using Children = std::vector<Key>;
	Keys KeylistStart, FinalKeys;
	DbElement<Payload> searchElem;
	int FieldType = 0;
	QueryExecuter& from(DbCore<Payload>& db) { KeylistStart = db.keys(); return *this; }
	QueryExecuter& select(std::string Field) { FieldType = SelectField(Field); return *this; }
	QueryExecuter& where(std::string searchString, DbCore<Payload> dbSearch, DateTime startDate, DateTime endDate = DateTime().now())
	{
		KeylistStart = dbSearch.keys();
		std::regex conditionMatch(searchString);
		bool matched = false;
		Key key;


		switch (FieldType) {
		case 1:
			for (std::vector<std::string>::iterator itkeys = KeylistStart.begin(); itkeys != KeylistStart.end(); ++itkeys)
			{

				DbElement<Payload> searchElem = dbSearch[*itkeys];
				//DbElement<std::string> searchElem = db[*itkeys];
				matched = MyRegex(searchElem.name(), conditionMatch);
				if (matched == true)
				{
					//KeylistResult.push_back(*itkeys);
					FinalKeys.push_back(*itkeys);
				}
			}
			break;
		case 2:
			for (std::vector<std::string>::iterator itkeys = KeylistStart.begin(); itkeys != KeylistStart.end(); ++itkeys)
			{

				DbElement<Payload> searchElem = dbSearch[*itkeys];
				matched = MyRegex(searchElem.descrip(), conditionMatch);
				if (matched == true)
				{

					FinalKeys.push_back(*itkeys);
				}
			}
			break;
		case 3:
			for (std::vector<std::string>::iterator itkeys = KeylistStart.begin(); itkeys != KeylistStart.end(); ++itkeys)
			{

				DbElement<Payload> searchElem = dbSearch[*itkeys];
				//DateTime temp();
				if (compdate(startDate, endDate, searchElem.dateTime()))
				{
					FinalKeys.push_back(*itkeys);
				}
			}
		case 4:
			for (std::vector<std::string>::iterator itkeys = KeylistStart.begin(); itkeys != KeylistStart.end(); ++itkeys)
			{
				DbElement<Payload> searchElem = dbSearch[*itkeys];
				Keys children = searchElem.children();
				std::vector<std::string>::iterator itr;
				if (children.size() > 0)
				{
					for (itr = children.begin(); itr < children.end(); ++itr)
					{
						matched = MyRegex(*itr, conditionMatch);
						if (matched == true)
						{
							FinalKeys.push_back(*itkeys);
						}
					}
				}
			}
		case 5:
			for (std::vector<std::string>::iterator itkeys = KeylistStart.begin(); itkeys != KeylistStart.end(); ++itkeys)
			{
				searchElem = dbSearch[*itkeys];
				std::vector<std::string>::iterator itr;
				Payload pel = searchElem.payLoad();
				Payload::Category category = pel.category();
				for (itr = pel.category().begin(); itr < pel.category().end(); ++itr)
				{
					matched = MyRegex(*itr, conditionMatch);
					if (matched == true)
					{
						FinalKeys.push_back(*itkeys);
					}
				}
			}
			break;
		case 6:
			for (std::vector<std::string>::iterator itkeys = KeylistStart.begin(); itkeys != KeylistStart.end(); ++itkeys)
			{
				matched = MyRegex(*itkeys, conditionMatch);
				if (matched == true)
				{
					searchElem = dbSearch[*itkeys];
					showHeader();
					showElem(searchElem);
				}
			}
			break;
		case 7:
			for (std::vector<std::string>::iterator itkeys = KeylistStart.begin(); itkeys != KeylistStart.end(); ++itkeys)
			{
				matched = MyRegex(*itkeys, conditionMatch);
				if (matched == true)
				{
					searchElem = dbSearch[*itkeys];
					typename DbElement<T>::Children children = searchElem.children();
					if (children.size() > 0)
					{
						std::cout << "\n child keys: ";
						for (auto key : children)
						{
							std::cout << " " << key;
						}
					}
				}
			}
			break;
		case 8:
			showKeys(dbSearch);
			break;
		default: std::cout << "\nInvalid field";
		}

		std::cout << "\n  ";
		return *this;
	}
};

////////////////////////////////////////////////////////////////
// Wrapper is used for regex,it  also compares the input 
//and search string with the help of Regex expression


bool MyRegex(std::string inp, std::regex searchExp)
{
	bool match = false;

	if (std::regex_match(inp, searchExp) == true)
	{
		match = true;
	}
	return match;
}

///////////////////////////////////////////////////////////////////////
// Selects the DbElement like name, description, date time, children

int SelectField(std::string Field)
{
 int value = 0;
 if (Field == "name") {
  value = 1;
 }
 else if (Field == "description") {
  value = 2;
 }
 else if (Field == "datetime") {
  value = 3;
 }
 else if (Field == "children") {
  value = 4;
 }
 else if (Field == "payload") {
  value = 5;
 }
 else if (Field == "key") {
  value = 6;
 }
 else if (Field == "keychildren") {
  value = 7;
 }
 else if (Field == "") {
  value = 8;
 }
 else {
  std::cout << "Invalid field\n";
 }
 return value;
}

////////////////////////////////////////////////////////////////////
// Display all the keys that have been selected from the database

void PrintKeys(DbCore<std::string> ::Keys FinalKeys)
{
	for (auto key : FinalKeys)
	{
		std::cout << key << "\n";
	}

}

/////////////////////////////////////////////////////////////////////////////////
// QueryAndOr checks which condition is selected
// - if AND is selected it displays the common keys 
// - if OR is selected it displays all the keys 


void QueryAndOr(DbCore<std::string>::Keys FirstQuery, DbCore<std::string>::Keys SecondQuery,
	std::string condition)
{
	DbCore<Payload>::Keys FinalKeys;
	size_t i = 0;
	if ("AND" == condition) {
		if (FirstQuery.size() == 0 || SecondQuery.size() == 0)
		{
			std::cout << "Nothing to display one of the final result set of query is empty\n";
			exit(0);
		}
		std::sort(FirstQuery.begin(), FirstQuery.end());
		std::sort(SecondQuery.begin(), SecondQuery.end());
		std::set_intersection(FirstQuery.begin(), FirstQuery.end(),
			SecondQuery.begin(), SecondQuery.end(),
			std::back_inserter(FinalKeys));
		PrintKeys(FinalKeys);
	}
	else if ("OR" == condition) {
		while (i < FirstQuery.size())
		{
			FinalKeys.push_back(FirstQuery.at(i));
			i++;
		}
		i = 0;
		while (i < SecondQuery.size())
		{
			FinalKeys.push_back(SecondQuery.at(i));
			i++;
		}
		std::sort(FinalKeys.begin(), FinalKeys.end());
		FinalKeys.erase(std::unique(FinalKeys.begin(), FinalKeys.end()), FinalKeys.end());
		PrintKeys(FinalKeys);
	}
	else
	{
		std::cout << "\nInvalid operation";
	}
}
