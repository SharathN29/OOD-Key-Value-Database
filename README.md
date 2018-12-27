# OOD-Key-Value-Database

## Purpose
There is currently a lot of technical interest in "Big Data". Extreme examples are: data collection and analyses from the Large Hadron Collider, the Sloan Sky Survey, analyses of Biological Genomes, measuring weather patterns, collecting data for global climate models, and analyzing client interactions in social networks.
Conventional SQL databases are not well suited for these kinds of applications. While they have worked very well for many business applications and record keeping, they get overwhelmed by massive streams of data. Developers are turning to "noSQL" databases like MongoDB, couchDB, Redis, and Big Table to handle massive data collection and analyses.
In this project we will explore how a non SQL database can be constructed and used. In the second project we will use this database to store package information in a code repository.

## What has been implemented 

### Implemented a generic key/value in-memory database where each value consists of
#### Metadata:
* A name string
* A text description of the item
* A DateTime instance recording the date and time the value was written to the database.
* A finite number (possibly zero) of child relationships with other values. Each element of the child relationships collection is the key of another item in the database. Any item holding a key in this collection is the parent of the element accessed by the key.

* An instance of the generic type1. This might be a string, a container of a set of values of the same type, or some other collection of data captured in some, perhaps custom, data structure. We will refer to this instance as the database element's payload.
* Shall support addition and deletion of key/value pairs.
* Shall support editing of values including the addition and/or deletion of relationships, editing text metadata, and replacing an existing value's instance with a new instance. Editing of keys is forbidden.
* If you delete child relationships, e.g., delete keys from an element's child collection, you are forbidden to delete the elements accessed from those keys from the database.
#### Shall support queries for:
* The value of a specified key.
* The children of a specified key.
* The set of all keys matching a specified regular-expression pattern.
* All keys that contain a specified string in their metadata section, where the specification is based on a regular-expression pattern.
* All keys that contain values written within a specified time-date interval. If only one end of the interval is provided shall take the present as the other end of the interval.
 
* Shall support queries on the set of keys returned by a previous query3, e.g., an "and"ing of multiple queries. Shall also support queries on the union of results of one or more previous queries, e.g., an "or"ing of multiple queries.
* Shall, on command, persist a colllection of database contents, defined by a collection of keys, to an XML file4,5. It is intended that this process be reversible, e.g., the database can be restored or augmented6 from an existing XML file as well as write its contents out to one or more XML files.
* Shall implement a Payload type that contains a string, which for Project #2, will be the path to a file in the Repository. The Payload type shall also contain a std::vector<std::string>, where each string is the name of a category associated with the Payload file. For the Repository, the key will be a file name, so the combination of key and payload will provide details about a file stored in the Repository.
* Shall provide, in your implementation, at least the following packages: Executive, DBCore, Query, Test.
