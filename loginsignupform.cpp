// C++ Login + Registration form  

#include <iostream>
#include <fstream> // importing file library 
#include <string>  // database for users
#include <sqlite3.h>


using namespace std;



bool isLoggedIn(int choice) {
    string un, pw;
    int rc; // return code 
    bool loggedInConfo = false;
    sqlite3_stmt *stmt; // initialise the statement function
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
        ;rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); // initialize sql usage

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
            cout << "Sign up has failed\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1; // indicate an issue
        }

    loggedInConfo = true;
}



    // This part of the code will proceed to read from the database and compare the input to the stored values in the given database,
    // If they are matched, then the user will then see their dashboard

    return loggedInConfo;
}

bool signUp(int choice) 
{
    string un, pw;
    bool signUpSuccess = false;
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

        // prepare to insert a new user to the database
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

        // bind the information to the following columns username and password on the user database, the password will need to be hashed soon for security purposes...
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

        int result = sqlite3_step(stmt); // once compiled, then the change is appended to the database


        if (result == SQLITE_DONE)
        {
            cout << "Sign up has been successfully initiated! " << endl; // in the instance that it is a success, then it will be outputted, or otherwise it has failed to append.
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
    
    return signUpSuccess; // confirms output
    // this part of the code will append the username and password given here into the database, other measures will be made to ensure that the passwords are secure (i.e. encryption techniques)    
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
        choiceFunction(); // keep recusing till right details are inputted  
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