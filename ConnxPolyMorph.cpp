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

		

		OverLoad OperatorPlus(OverLoad const& obj)
		{
			OverLoad tmp;
			tmp.number = this->number + obj.number;
			return tmp;
		}


		// In my story about Operator overload, the intersting thing was that we had an operator overload on a "++" on a parent class that people
		// The object that might be called employeeObj that was of time Employee was actually part of one or many different sets. Whenever you did
		// a ++, it would increment
		OverLoad operator + (OverLoad const& obj)
		{
			OverLoad tmp;
			tmp.number = this->number + obj.number;
			return tmp;
		}


		void print()
		{
			cout << "Overload value is  " << number << endl;
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
	//printf("A is %d\n", a);
	return a + b;
}

int main()
{
	int retStatus;

	srand(time(0));

	// Will be opening my database. Don't really care, as from our perspective it is all the same
	printf("We will be demonstrating Inclusion Polymorphism (Runtime\n");
	printf("We will be creating a Database to one of three types, and using a base pointer\n");
	printf("To call into the various methods (OpenDatabase and CloseDatabase\n\n");

	for (int i = 0; i < 5; i++)
	{
		// We will be getting a database. Who know's where it is coming from
		DatabaseConnect* pConnxDb = DatabaseFactory::RandomDatabaseConnnect();
		char* parms = (char*)"dbParms;user=dbuser, password=dbpassword";
		retStatus = pConnxDb->OpenDatabase((char*) parms); // Example of Coercion

		// will be closing my Database. Not sure what it will be, but we don't care
		retStatus = pConnxDb->CloseDatabase();

		// Log it, show the Error Status and print a couple of blank lines
		pConnxDb->Log();
		printf("about to log with file\n");
		pConnxDb->Log("log.txt");
		pConnxDb->ErrorStatus();
		printf("\n\n");

		// Cleanup
		delete pConnxDb;
	}


	// Operator Overload
	OverLoad a1(10), a2(5);
	OverLoad a3 = a1 + a2;

	printf("Operater Overload Example. We will be overloading the + operator\n");
	a3.print();
	// This line is added to take away the mystery that Operator overload
	// could have easily been a function
	printf("We are calling a function instead of overloading the + operator. Results should be same\n");
	OverLoad a4 = a1.OperatorPlus(a2);
	a4.print();

	printf("\n\n");
	printf("Template Example. We have three different types\n\n");
	cout << "Example 1 should be 7 as it greatest of (3,7): " << myMax<int>(3, 7) << endl;  // Call myMax for int
	cout << "Example 2 should be 7.42 as it is greatest of (3.5,7.42) " << myMax<double>(3.5, 7.42) << endl; // call myMax for double
	cout << "Example 3 should be 'g' as it is greatest of ('g','e') " << myMax<char>('g', 'e') << endl;   // call myMax for char

		// Passing double arguments, as expected

	printf("\n\n");
	printf("Method Overloading - Ad hoc Polymorphism\n");

	printf("We are calling a special add function that takes doubles.\n");
	printf("We are passing 2.4 and 8.5 and expecting to see 10.9\n");
	cout << "Sum = " << add(2.4, 8.5) << endl;

	printf("We are calling a special add function that takes doubles.\n");
	printf("We are passing 16 and 18 and expecting to see 34\n");
	cout << "Sum = " << add(16, 18) << endl;


	// Passing int arguments, when double is expected
	// This will lead to Coercion Polymorphism
	printf("\n\nCoercion Polymorphism\n");
	printf("We are passing 2.1 and 5.6 and are expecting to see a 7\n");
	cout << "We return an Integer: Answer = " << addreturnint(2.1, 5.6) << endl;


}

