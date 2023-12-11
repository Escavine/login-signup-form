// C++ Login + Registration form  

#include <iostream>
#include <fstream> // importing file library 
#include <string>  // database for users
#include <sqlite3.h>


using namespace std;



bool isLoggedIn(int choice) {
    string username, password;
    string storedUsername = "John"; //subjected to change as this is not dynamic and will need to link to sql
    string storedPassword = "Doe";
    bool loggedInConfo = false;

    while (!loggedInConfo) {
        cout << "Enter your username:\n";
        cin >> username;

        cout << "Enter your password:\n ";
        cin >> password;

        cout << "Logging in...\n";

        if (storedUsername == username && storedPassword == password) {
            cout << "Login Success! " << endl;
            loggedInConfo = true;
        } else 
        {
            cout << "Details are incorrect, please input details again. " << endl;
        }
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
        const char* sql = "INSERT INTO users (username, password), VALUES (?, ?)"
        ;rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cerr << "Error preparing statement\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        else 
        {
            cout << "Sucessfully prepared statement\n" << endl;
        }

        // bind the information to the following columns username and password on the user database, the password will need to be hashed soon for security purposes...
        rc = sqlite3_bind_text(stmt, 1, un.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK) {
            cerr << "Error binding username\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        else 
        {
            cout << "Sucessfully binded username\n" << endl;
        }


        rc = sqlite3_bind_text(stmt, 2, pw.c_str(), -1, SQLITE_STATIC);


        if (rc != SQLITE_OK)
        {
            cerr << "Error binding password\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        else 
        {
            cout << "Sucessfully binded password\n" << endl;
        }

        int result = sqlite3_step(stmt); // once compiled, then the change is appended to the database


        if (result == SQLITE_DONE)
        {
            cout << "Sign up has been successfully initiated! " << endl; // in the instance that it is a success, then it will be outputted, or otherwise it has failed to append.
        }
        else 
        {
            cerr << "Sign up has failed to insert to the database " << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
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
    
    cout << "Reminder System Main Menu\n";
    cout << "------- Main Menu -------\n";
    
    
    cout << "1. Sign up\n";
    cout << "--------------";
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