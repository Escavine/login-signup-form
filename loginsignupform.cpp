// C++ Login + Registration form  

// pre-requisite libraries
#include <iostream>
#include <fstream> 
#include <string>  
#include <sqlite3.h> // header library for SQL command utilisation


using namespace std;



bool isLoggedIn(int choice) {
    string un, pw;
    int rc; // return code 
    bool loggedInConfo = false; // local variables
    sqlite3_stmt *stmt; 
    sqlite3* db;

    while (!loggedInConfo) {
        cout << "Enter your username:\n";
        cin >> un;

        cout << "Enter your password:\n ";
        cin >> pw;

        cout << "Logging in...\n";

        rc = sqlite3_open("users.db", &db);

        if (rc != SQLITE_OK)
        {
            cerr << "Database connection hasn't been initialized";
            sqlite3_close(db);
            return false;
        }
        else 
        {
            cout << "Database connection has been initialized";
            sqlite3_finalize(stmt);
            return true;
        }

        const char* sql = "SELECT * FROM users WHERE username = ? AND password = ?";
        ;rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); 

        if (rc != SQLITE_OK)
        {
            cerr << "Error preparing statement\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }
        else 
        {
            cout << "Statement initialization completed\n";
            sqlite3_finalize(stmt);
            return true;
        }

        rc = sqlite3_bind_text(stmt, 1, un.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK)
        {
            cerr << "Username hasn't been initialized\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;

        }
        else
        {
            cout << "Username has been initialized\n";
            sqlite3_finalize(stmt);
            return true;

        }


        rc = sqlite3_bind_text(stmt, 1, pw.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK)
        {
            cerr << "Password has not been initalized\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }
        else
        {
            cout << "Password has been intialized\n";
            sqlite3_finalize(stmt);
            return true;
        }

        int result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
        {
            cout << "User authenticated.\n" << endl;
            sqlite3_finalize(stmt);
            return 0;
        }
        else 
        {
            cout << "Details are incorrect, please try again\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            isLoggedIn(choice); // recurse to allow user to input their details again till they are correct.
            return 1; // indicate an issue
        }

        loggedInConfo = true;
    }

    return loggedInConfo;
}

bool signUp(int choice) 
{
    string un, pw;
    bool signUpSuccess = false; // local variables
    sqlite3_stmt* stmt;
    int rc;
    sqlite3* db;
    
    while (!signUpSuccess) 
    {
        cout << "Enter a new username:\n ";
        cin >> un;
    
        cout << "Enter a new password:\n "; 
        cin >> pw;
    
        cout << "Signing up... \n";

        rc = sqlite3_open("users.db", &db);

        if (rc != SQLITE_OK) {
            cout << "Database has not been initialized ";
            sqlite3_close(db);
            return 1;
        }
        else 
        {
            cout << "Database has been initialized ";
            sqlite3_finalize(stmt);
            return 0;
        }

        const char* sql = "INSERT INTO users (un, pw), VALUES (?, ?)"
        ;rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); 

        if (rc != SQLITE_OK)
        {
            cerr << "Error preparing statement\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
        else 
        {
            cout << "Sucessfully prepared statement\n" << endl;
            sqlite3_finalize(stmt);
            return 0;
        }

        rc = sqlite3_bind_text(stmt, 1, un.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK) {
            cerr << "Error binding username\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
        else 
        {
            cout << "Sucessfully binded username\n" << endl;
            sqlite3_finalize(stmt);
            return 0;
        }


        rc = sqlite3_bind_text(stmt, 2, pw.c_str(), -1, SQLITE_STATIC); 


        if (rc != SQLITE_OK)
        { 
            cerr << "Error binding password\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
        else 
        {
            cout << "Sucessfully binded password\n" << endl;
            sqlite3_finalize(stmt);
            return 0;
        }

        int result = sqlite3_step(stmt); // insert the new user to the database once compilation is completed


        if (result == SQLITE_DONE)
        {
            cout << "Sign up has been successfully initiated! " << endl; // in the instance that it is a success, then it will be outputted, or otherwise it has failed to append
            sqlite3_finalize(stmt);
            return 0;
        }
        else 
        {
            cerr << "Sign up has failed to insert to the database " << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
        signUpSuccess = true;


    }
    
    return signUpSuccess; // confirms output, other measures will be made to ensure that the passwords are secure (i.e. encryption techniques such as a hash salt)    
}


// void hashingAlgorithm() {} // for further encrypting the password


void choiceFunction()
{
    int choice;
    
    cout << "Reminder System Main Menu\n" << endl;
    cout << endl;
    cout << "------- Main Menu -------\n";
    
    
    cout << "1. Sign up\n";
    cout << "2. Login\n";    
    cin >> choice;
    
    
    if (choice == 1)
    {
        cout << "You will be redirected to the sign-up form... " << endl;
        signUp(choice);
    }
    else if (choice == 2)
    {
        cout << "You will be redirected to the login form... " << endl;
        bool loginResult = isLoggedIn(choice);
    }
    else
    {
        cout << "Input not recognized, please use a sensible input" << endl;
        choiceFunction(); // keep recusing till right input is made 
    }
}


int main()
{
    sqlite3* db;
    int rc; // return code

    rc = sqlite3_open("users.db", &db);

    if (rc != SQLITE_OK)
    {
        cerr << "Database has not been connected...\n" << endl;
        sqlite3_close(db);
        return 1; // indicating error
    }
    else 
    {
        cout << "Database connection has been intialized!\n" << endl;
    }

    choiceFunction(); // once database connection is established, proceed to go to the sign-in/login page

    sqlite3_close(db);
    return 0;
}