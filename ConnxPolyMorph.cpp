// ConnxPolyMorph.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This Program will simulate how Polymorphism might work in a former company
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


template <typename T>
T myMax(T x, T y)
{
	return (x > y) ? x : y;
}

class OverLoad {
	int number;
	public:
		OverLoad(int num=0)
		{
			number = num;
		}

		OverLoad operator + (OverLoad const& obj)
		{
			OverLoad tmp;
			tmp.number = this->number + obj.number;
			return tmp;
		}

		void print()
		{
			cout << "Object val is  " << number;
		}

};



class DatabaseConnect
{
public:

	virtual int ErrorStatus() = 0;

	virtual void Log()
	{
		printf("Base has a log implementation as Log()  was not implemented in Child\n");
	}

	virtual void Log(const char* logFile)
	{
		printf("Base has a log implementation as Log(char *logFile)  was not implemented in Child yet. \n");
		printf("It demonstrates Compile Time overload of methods\n");
	}

	virtual int OpenDatabase(char* parms)=0;
	virtual int CloseDatabase()=0;




};

class OracleDatabaseConnect :
	public DatabaseConnect
{
public:
	int errorVal = 0;

	OracleDatabaseConnect() {};

	int OpenDatabase(char* parms) 
	{
		cout << "Open Oracle Database:" << parms << endl;
		return 0;
	};

	int CloseDatabase() 
	{
		cout << "Close Oracle Database" << endl;
		return 0;
	};

	int ErrorStatus()
	{
		cout << "Error status for Oracle:" << errorVal << endl;
		return 0;
	}

	void Log()
	{
		cout << "We have overridden the log implementation for Oracle" << endl;
	}

};


class SQLServerDatabaseConnect :
	public DatabaseConnect
{

	int errorVal = 0;
public:
	SQLServerDatabaseConnect() {};

	int OpenDatabase(char* parms)
	{
		cout << "Open SQLServer Database:" << parms << endl;
		return 0;
	};

	int CloseDatabase()
	{
		cout << "Close SQLServer Database" << endl;
		return 0;
	};

	virtual int ErrorStatus()
	{
		cout << "show errorStatus: " << errorVal << endl;
		return errorVal;
	}

//	virtual void log()
//	{
//		cout << "logger implementation for SQLServer" << endl;
//	}

};


class MYSQLDatabaseConnect :
	public DatabaseConnect
{
	int errorVal = 0;
public:
	MYSQLDatabaseConnect() {} 

	int OpenDatabase(char* parms)
	{

		cout << "Open MYSQL Database:" << parms << endl;
		return 0;
	};

	int CloseDatabase()
	{
		cout << "Close MYSQL Database" << endl;
		return 0;
	};

	virtual int ErrorStatus()
	{
		cout << "show MYSQL ErrorStatus" << errorVal << endl;
		return errorVal;
	}

	virtual void Log()
	{
		cout << "We overridden the Log()  in MYSQL" << endl;
	}

};

/*
	DatabaseFactory will have access to all the various header files for Implementations of OracleDatabaseConnect
	Currently there are three implementations.
		Oracle
		MySQL
		SQL Server
	It uses a random number between 0 and 2
*/
class DatabaseFactory
{
public:

	static DatabaseConnect* RandomDatabaseConnnect()
	{
		// Use some type of random number generator
		enum DatabaseTypes {ORACLE, SQLSERVER, MYSQL};

		DatabaseTypes randomNum; // Default
		
		const int DATABASE_COUNT = 3;

		DatabaseConnect* pDbObj=NULL;

		// Random number generator simulates user picking a particular database
		randomNum = (DatabaseTypes) (rand() % DATABASE_COUNT); 

		// Which one dependent on random variable
		switch (randomNum)
		{
			case ORACLE:
				pDbObj = new OracleDatabaseConnect();
				break;
			case SQLSERVER:
				pDbObj = new SQLServerDatabaseConnect();
				break;
			case MYSQL:
				pDbObj = new MYSQLDatabaseConnect();
				break;
		}

		return pDbObj;
	}
};


double add(double a, double b)
{
	return a + b;
}

int addreturnint(int a, int b)
{
	printf("A is %d\n", a);
	return a + b;
}

int main()
{
	int retStatus;

	srand(time(0));

	// Will be opening my database. Don't really care, as from our perspective it is all the same
	for (int i = 0; i < 10; i++)
	{
		// We will be getting a database. Who know's where it is coming from
		DatabaseConnect* pConnxDb = DatabaseFactory::RandomDatabaseConnnect();
		char* parms = (char*)"dbParms;user=dbuser, password=dbpassword";
		retStatus = pConnxDb->OpenDatabase((char*) parms); // Example of Coercion

		// will be closing my Database. Not sure what it will be, but we don't care
		retStatus = pConnxDb->CloseDatabase();

		// Log it, show the Error Status and print a couple of blank lines
		pConnxDb->Log();
		pConnxDb->Log("log.txt");
		pConnxDb->ErrorStatus();
		printf("\n\n");

		// Cleanup
		delete pConnxDb;
	}


	// Operator Overload
	OverLoad a1(10), a2(5);
	OverLoad a3 = a1 + a2;
	a3.print();


	cout << myMax<int>(3, 7) << endl;  // Call myMax for int
	cout << myMax<double>(3.0, 7.0) << endl; // call myMax for double
	cout << myMax<char>('g', 'e') << endl;   // call myMax for char

		// Passing double arguments, as expected
	cout << "Sum = " << add(2.4, 8.5) << endl;

	// Passing int arguments, when double is expected
	// This will lead to Argument Coercion
	cout << "Sum = " << add(16, 18) << endl;

	cout << "Sum = " << addreturnint(2.5, 5.54334) << endl;


}

