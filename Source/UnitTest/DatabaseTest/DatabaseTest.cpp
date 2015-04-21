#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <stdio.h>
/// Compile OTL 4.0/ODBC
#ifdef WIN32
#define OTL_ODBC
#else
#define OTL_ODBC_UNIX
#endif//WIN32
/// The following #define is required with MyODBC 5.1 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
/// Compile OTL with Unicode 
#define OTL_UNICODE  
/// include the OTL 4.0 header file
#undef close
#include <Otl/otlv4.h>

#include "Database/Database.h"

/// connection string
char* pcConStr = "Driver={MySQL ODBC 5.1 Driver}; Server=localhost; Database=test; Uid=root; Pwd=123; CharSet=UTF8";
/// connect object
otl_connect db; 

/// insert rows into table
void otl_insert()
{ 
	otl_stream o(1, // buffer size should be == 1 always on INSERT. 
		"insert into test_tab values(:f1<int>,:f2<char[5]>)", 
		// SQL statement, char[5] means 5 2-byte 
		// Unicode charatcters including a null
		// terminator
		db // connect object
		);

	unsigned short tmp[32]; // Null terminated Unicode character array.

	for(int i = 1; i <= 100; ++i)
	{
		o << i;
		tmp[0] = 1111; // Unicode character (decimal code of 1111)
		tmp[1] = 2222; // Unicode character (decimal code of 2222)
		tmp[2] = 3333; // Unicode chracater (decimal code of 3333)
		tmp[3] = 4444; // Unicode chracater (decimal code of 4444)
		tmp[4] = 0; // Unicode null terminator 
		o << (unsigned char*)tmp; 
		// overloaded operator<<(const unsigned char*) in the case of Unicode
		// OTL accepts a pointer to a Unicode character array.
		// operator<<(const unsigned short*) wasn't overloaded
		// in order to avoid ambiguity in C++ type casting.
	}
}

void otl_select()
{ 
	otl_stream i(50, // buffer size
		"select * from test_tab "
		"where f1>=:f11<int> "
		"  and f1<=:f12<int>*2",
		// SELECT statement
		db // connect object
		); 
	// create select stream

	int f1;
	unsigned short f2[32];

	i << 8 << 8; // assigning :f11 = 8, f12 = 8
	// SELECT automatically executes when all input variables are
	// assigned. First portion of output rows is fetched to the buffer

	while(!i.eof())
	{ // while not end-of-data
		i >> f1;
		i >> (unsigned char*)f2;
		// overloaded operator>>(unsigned char*) in the case of Unicode
		// OTL accepts a pointer to a Unicode chracter array.
		// operator>>(unsigned short*) wasn't overloaded 
		// in order to avoid ambiguity in C++ type casting.
		std::cout << "f1=" << f1 << ", f2=";
		for(int j = 0; f2[j] != 0; ++j)
			std::cout << " " << f2[j];
		std::cout << std::endl;
	}

	i << 4 << 4; // assigning :f11 = 4, :f12 = 4
	// SELECT automatically executes when all input variables are
	// assigned. First portion of output rows is fetched to the buffer

	while(!i.eof())
	{ // while not end-of-data
		i >> f1 >> (unsigned char*)f2;
		std::cout << "f1=" << f1 << ", f2=";
		for(int j = 0; f2[j] != 0; ++j)
			std::cout << " " << f2[j];
		std::cout << std::endl;
	}

}

BOOST_AUTO_TEST_SUITE(DatabaseTestSuite)

BOOST_AUTO_TEST_CASE(OtlTry)
{
	otl_connect::otl_initialize(); // initialize the database API environment
	try
	{
		db.rlogon(pcConStr); // connect to the database

		otl_cursor::direct_exec
			(
			db,
			"drop table test_tab",
			otl_exception::disabled // disable OTL exceptions
			); // drop table

		otl_cursor::direct_exec
			(
			db,
			"create table test_tab(f1 int, f2 varchar(11))"
			);  // create table

		otl_insert(); // insert records into table
		otl_select(); // select records from table

	}
	catch(otl_exception& p)
	{ // intercept OTL exceptions
		std::cerr << p.msg << std::endl; // print out error message
		std::cerr << p.stm_text << std::endl; // print out SQL that caused the error
		std::cerr << p.var_info << std::endl; // print out the variable that caused the error
	}

	db.logoff(); // disconnect from the database
}

BOOST_AUTO_TEST_CASE(DatabaseConnectTest)
{
	Database::OtlConnectionMgr kService;
	BOOST_CHECK_MESSAGE(kService.Initialize(), 
		"database service initialize failed.");
	std::string strCon = pcConStr;
	Database::OtlConnection* pkDataConnection = kService.Connect(strCon);
	BOOST_CHECK_MESSAGE(pkDataConnection != 0, 
		"database connection error.");
	BOOST_CHECK_MESSAGE(kService.CloseConnection(pkDataConnection), 
		"database close connection failed.");
	BOOST_CHECK_MESSAGE(kService.Shutdown(),
		"database service shutdown failed.");
}

BOOST_AUTO_TEST_CASE(DatabaseDirectExecuteTest)
{
    Database::OtlConnectionMgr kService;
	BOOST_CHECK_MESSAGE(kService.Initialize(), 
		"database service initialize failed.");
	std::string strCon = pcConStr;
	Database::OtlConnection* pkDataConnection = kService.Connect(strCon);
	BOOST_CHECK_MESSAGE(pkDataConnection != 0, 
		"database connection error.");

	if (pkDataConnection)
	{
		//std::string strSQL0 = "create table test_tab(f1 int, f2 varchar(11))";
		//Database::DirectExecuteTask* pkTask0 = new Database::DirectExecuteTask;
		//pkTask0->setExecuteSQL(strSQL0);
		//pkDataConnection->addTask(pkTask0);

		std::string strSQL1 = "drop table test_tab";
		Database::OtlDirectExecuteTask* pkTask1 = new Database::OtlDirectExecuteTask;
		pkTask1->setExecuteSQL(strSQL1);
		pkDataConnection->addTask(pkTask1);
	}

	kService.Running();

	BOOST_CHECK_MESSAGE(kService.CloseConnection(pkDataConnection), 
		"database close connection failed.");
	BOOST_CHECK_MESSAGE(kService.Shutdown(),
		"database service shutdown failed.");
}

BOOST_AUTO_TEST_SUITE_END()
