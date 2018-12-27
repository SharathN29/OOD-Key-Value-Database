#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// Source: Jim Fawcett, Object Oriented Design, Spring 2018        //
// Author: Sharath Nagendra, Syracuse University                   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, as well as
*   providex editing, querying, and persisting
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type.
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.2 : 7 Feb 2018
* - added a Payload class that contains the path to a file in the repository
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <regex>
#include "../DateTime/DateTime.h"

namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// DbElement class
	// - provides the value part of a NoSql key-value database

	template<typename T>
	class DbElement
	{
	public:
		using Key = std::string;
		using Children = std::vector<Key>;
		using Parent = std::vector<Key>;

		// methods to get and set DbElement fields

		std::string& name() { return name_; }
		std::string name() const { return name_; }
		void name(const std::string& name) { name_ = name; }

		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		void descrip(const std::string& name) { descrip_ = name; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

		Children& children() { return children_; }
		Children children() const { return children_; }
		void children(const Children& children) { children_ = children; }

		Parent& parent() { return parent_; }
		Parent parent() const { return parent_; }
		void parent(const Parent& parent) { parent_ = parent; }

		T& payLoad() { return payLoad_; }
		T payLoad() const { return payLoad_; }
		void payLoad(const T& payLoad) { payLoad_ = payLoad; }

	private:
		std::string name_;
		std::string descrip_;
		DateTime dateTime_;
		Children children_;
		Parent parent_;
		T payLoad_;
	};

	/////////////////////////////////////////////////////////////////////
	// DbCore class
	// - provides core NoSql db operations
	// - also provides editing, querying, and persistance operations

	template <typename T>
	class DbCore
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		using Children = Keys;
		using Parent = Keys;
		using DbStore = std::unordered_map<Key, DbElement<T>>;
		using iterator = typename DbStore::iterator;

		// methods to access database elements

		Keys keys();
		bool contains(const Key& key);
		size_t size();
		void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
		DbElement<T>& operator[](const Key& key);
		DbElement<T> operator[](const Key& key) const;
		typename iterator begin() { return dbStore_.begin(); }
		typename iterator end() { return dbStore_.end(); }
		void deleteKey(DbCore<T>& db, std::string key);
		//void keyMatch(DbCore<T>& db,std::string matchExp);
		void editKey(DbCore<T> &db, std::string key, DbElement<T> &dbelem);
		void keyadd(DbCore<T> &db, std::string key, DbElement<T> &dbelem);

		// methods to get and set the private database hash-map storage

		DbStore& dbStore() { return dbStore_; }
		DbStore dbStore() const { return dbStore_; }
		void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

	private:
		DbStore dbStore_;
		bool doThrow_ = false;
	};

	//////////////////////////////////////////////////////////////////////////
	// Payload class
	// - contains a string where each string is the name of a category 
	//   associated with the Payload file
	// For the Repository
	// - the key will be a file name, so the combination of key and 
	//   payload will provide details about a file stored in the repository

	class Payload
	{

	public:
		using FilePath = std::string;
		using Category = std::vector<std::string>;
		Payload() {}
		Payload(FilePath file) { this->filePath_ = file; }
		Payload(Category cate) { this->category_ = cate; }
		Payload(FilePath file, Category cate)
		{
			this->category_ = cate;
			this->filePath_ = file;
		}
		FilePath& fpath() { return filePath_; }
		Category& category() { return category_; }
		void fpath(const FilePath& file) { filePath_ = file; }
		void category(const Category& c) { category_ = c; }
		void category(std::string& c) { category_.push_back(c); }
		bool comparePayload(const Payload&);

	private:
		FilePath filePath_;
		Category category_;
	};

	bool Payload::comparePayload(const Payload& p1)
	{
		if ((this->filePath_.compare(p1.filePath_) != 0) &&
			(this->category_.size() == p1.category_.size()) &&
			(std::equal(p1.category_.begin(), p1.category_.end(), this->category_.begin(), this->category_.end())))
			return true;
		else
			return false;
	}

	/////////////////////////////////////////////////////////////////////
	// DbCore<T> methods

	//----< Does db contain this key? >----------------------------------

	template<typename T>
	bool DbCore<T>::contains(const Key& key)
	{
		iterator iter = dbStore_.find(key);
		return iter != dbStore_.end();
	}
	//----< returns current key set for db >-----------------------------

	template<typename T>
	typename DbCore<T>::Keys DbCore<T>::keys()
	{
		DbCore<T>::Keys dbKeys;
		DbStore& dbs = dbStore();
		size_t size = dbs.size();
		dbKeys.reserve(size);
		for (auto item : dbs)
		{
			dbKeys.push_back(item.first);
		}
		return dbKeys;
	}
	//----< return number of db elements >-------------------------------

	template<typename T>
	size_t DbCore<T>::size()
	{
		return dbStore_.size();
	}
	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes non-const db objects
	*  - In order to create a key-value pair in the database like this:
	*      db[newKey] = newDbElement
	*    we need to index with the new key and assign a default element.
	*    That will be replaced by newDbElement when we return from operator[]
	*  - However, if we want to index without creating new elements, we need
	*    to throw an exception if the key does not exist in the database.
	*  - The behavior we get is determined by doThrow_.  If false we create
	*    a new element, if true, we throw. Creating new elements is the default
	*    behavior.
	*/
	template<typename T>
	DbElement<T>& DbCore<T>::operator[](const Key& key)
	{
		if (!contains(key))
		{
			if (doThrow_)
				throw(std::exception("Key doesn't exist in db"));
			else
				return (dbStore_[key] = DbElement<T>());
		}
		return dbStore_[key];
	}
	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes const db objects
	*/
	template<typename T>
	DbElement<T> DbCore<T>::operator[](const Key& key) const
	{
		if (!contains(key))
		{
			throw(std::exception("Key doesn't exist in db"));
		}
		return dbStore_(key);
	}

	/////////////////////////////////////////////////////////////////////
	// display functions

	//----< display database key set >-----------------------------------

	template<typename T>
	void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
	{
		out << "\n  ";
		for (auto key : db.keys())
		{
			out << key << " ";
		}
	}
	//----< show record header items >-----------------------------------

	inline void showHeader(std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(26) << std::left << "DateTime";
		out << std::setw(10) << std::left << "Name";
		out << std::setw(25) << std::left << "Description";
		out << std::setw(25) << std::left << "Payload";
		out << "\n  ";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(10) << std::left << "--------";
		out << std::setw(25) << std::left << "-----------------------";
		out << std::setw(25) << std::left << "-----------------------";
	}
	//----< display DbElements >-----------------------------------------

	template<typename T>
	void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(10) << std::left << el.name();
		out << std::setw(25) << std::left << el.descrip();
		out << std::setw(25) << std::left;
		Payload pel = el.payLoad();
		Payload::Category category = pel.category();
		for (auto category : category)
			out << category << " ";
		out << std::setw(25) << std::left << pel.fpath();
		typename DbElement<T>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n    Child Keys are: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
	}
	//----< display all records in database >----------------------------

	template<typename T>
	void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
	{
		showHeader(out);
		typename DbCore<T>::DbStore dbs = db.dbStore();
		for (auto item : dbs)
		{
			showElem(item.second, out);
		}
	}

	// Clears the fields of a particular key
	template<typename T>
	void clearKey(DbCore<T>& db, const std::string key)
	{
		DbElement<Payload> tempElem;
		tempElem = db[key];
		tempElem.name(" ");
		tempElem.descrip(" ");
		tempElem.children().clear();
		db[key] = tempElem;

	}

	// Delete the contents of a particular key
	template<typename T>
	void DbCore<T> ::deleteKey(DbCore<T>& db, std::string deletekey)
	{
		DbElement<T> dbe = db[deletekey];
		dbe.children().clear();
		db.dbStore_.erase(deletekey);
	}

	// Edits the value fileds of a particular key

	template<typename T>
	void DbCore<T> ::editKey(DbCore<T> &db, std::string key, DbElement<T> &dbelem)
	{
		db[key] = dbelem;
	}

	// adds the key for a particular record in the database
	template<typename T>
	void DbCore<T> ::keyadd(DbCore<T> &db, std::string key, DbElement<T> &dbelem)
	{
		db[key] = dbelem;
	}

}