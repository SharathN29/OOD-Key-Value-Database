#pragma once
/////////////////////////////////////////////////////////////////////
// Persistance.h - Contains various XML functionalities            //
// ver 1.0                                                         //
// Author: Sharath Nagendra, Syracuse University                   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* XmlDocument.h, XmlDocument.cpp
* XmlElement.h, XmlElement.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Feb 2018
* - first release
*/

#include "../DbCore/DbCore.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "../../Project1/XmlDocument/XmlDocument.h"
#include "../../Project1/XmlElement/XmlElement.h"


using namespace NoSqlDb;
using namespace XmlProcessing;

/////////////////////////////////////////////////////////////////////////
// Persistence class is used to:
// - load the string into the xml
// - convert into an object
// - derive the string from the xml

template <typename T>
class Persistence
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	using xml = std::string;
	DbCore<T> loadXml(std::string filePath);
	DbCore<T> convertIntoObj(const Key xmlString);
	DbCore<T> fromXml(const xml xmlString);
	void shardXml(DbCore<T>& db, Keys);
};

/////////////////////////////////////////////////////////////
// Takes in query result keys
// Gets elements of all the keys and stores in temp db
// Invokes create xml for the db

template <typename T>
void Persistence<T>::shardXml(DbCore<T>& db, Keys k)
{
	DbCore<T> newDb;
	for (std::vector<std::string>::iterator itkeys = k.begin();
		itkeys != k.end(); ++itkeys)
	{
		newDb[*itkeys] = db[*itkeys];
	}

	std::string x;
	Persistence<std::string> q;
	x = toXml(newDb);
	newDb = q.loadXml("testShard.xml");
	std::cin.get();

}

////////////////////////////////////////////////////////////////////////
// Takes the xmlString and fetches all the records having data elements
// like name, description, dateTime, payLoad and children 

template<typename T>
DbCore<T> Persistence<T> ::fromXml(const xml xmlString)
{
	std::cout << "\n\n Printing XML to be converted into DB Store\n\n" << xmlString << std::endl;

	XmlDocument newXDoc(xmlString, XmlDocument::str);
	DbCore<std::string> db;

	using Sptr = std::shared_ptr<AbstractXmlElement>;
	std::vector<Sptr> records = newXDoc.descendents("record").select();
	for (auto pRecord : records)
	{
		Key key;
		DbElement<std::string> elem;
		std::vector<std::string> childList;
		std::vector<Sptr> pChildren = pRecord->children();
		for (auto pChild : pChildren)
		{

			if (pChild->tag() == "key")
			{
				key = pChild->children()[0]->value();
			}
			else
			{
				// value represents a MockDbElem with children name and description

				std::vector<Sptr> pValueChildren = pChild->children();

				for (auto pValueChild : pValueChildren)
				{
					if (pValueChild->tag() == "name")
					{
						// get value of TextElement child
						elem.name(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "description")
					{
						// get value of TextElement child
						elem.descrip(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "dateTime")
					{
						// get value of TextElement child
						elem.dateTime(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "payLoad")
					{
						// get value of TextElement child
						elem.payLoad(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "children") {
						std::vector<Sptr> pValueKeyChildren = pValueChild->children();
						for (auto pValueChild : pValueKeyChildren) {
							if (pValueChild->tag() == "key") {
								childList.push_back(pValueChild->children()[0]->value());
							}
						}
					}
				}
			}
		}
		elem.children(childList);
		db[key] = elem;
	}
	std::cout << "\n\nShowing DB extracted from XML\n\n";
	showDb(db);

	std::cout << "\n\n";
	return db;
}

////////////////////////////////////////////////////////////////////////
// Take the xmlString and test whether its possible to open or not

template<typename T>
DbCore<T> Persistence<T> ::convertIntoObj(const Key xmlString)
{
	std::ifstream in(xmlString);
	if (!in.good())
		throw(std::exception(("can't open source file " + xmlString).c_str()));
	std::ostringstream out;
	out << in.rdbuf();
	std::string forOpening = std::move(out.str());
	DbCore<T> db = fromXml(forOpening);
	return db;
}

//////////////////////////////////////////////////////////////////////
// Take the filePath and convert into an object

template<typename T>
DbCore<T> Persistence<T> ::loadXml(std::string filePath)
{
	DbCore<T> db = convertIntoObj(filePath);
	return db;
}

/////////////////////////////////////////////////////////////////////
// Parse the string as an XML

template<typename T>
std::string toXml(DbCore<T> db)
{

	using Sptr = std::shared_ptr<AbstractXmlElement>;
	using Key = std::string;

	Sptr pDb = makeTaggedElement("NoSQLDB");
	Sptr pDocElem = makeDocElement(pDb);
	XmlDocument xDoc(pDocElem);

	for (auto item : db)
	{
		Sptr pRecord = makeTaggedElement("record");
		pDb->addChild(pRecord);
		Sptr pKey = makeTaggedElement("key", item.first);
		pRecord->addChild(pKey);

		Sptr pValue = makeTaggedElement("value");
		pRecord->addChild(pValue);
		Sptr pName = makeTaggedElement("name", item.second.name());
		pValue->addChild(pName);
		Sptr pDateTime = makeTaggedElement("dateTime", item.second.dateTime());
		pValue->addChild(pDateTime);
		Sptr pPayLoad = makeTaggedElement("payLoad", item.second.payLoad());
		pValue->addChild(pPayLoad);
		Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
		pValue->addChild(pDescrip);
		Sptr pChildren = makeTaggedElement("children");
		pValue->addChild(pChildren);
		for (auto child : item.second.children()) {
			Sptr pchild = makeTaggedElement("key", child);
			pChildren->addChild(pchild);
		}
	}
	std::string Xml = xDoc.toString();
	std::ofstream out("test.xml");
	out << Xml;
	out.close();
	return Xml;
}

