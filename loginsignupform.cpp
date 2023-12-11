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
        cout << "Enter your username: " << endl;
        cin >> username;

        cout << "Enter your password: " << endl;
        cin >> password;

        cout << "Logging in... " << endl;

        if (username == storedUsername && password == storedPassword) {
            cout << "Login Success! " << endl;
            loggedInConfo = true;
        } else {
            cout << "Details are incorrect, please input details again. " << endl;
        }
    }

    // This part of the code will proceed to read from the database and compare the input to the stored values in the given database,
    // If they are matched, then the user will then see their dashboard

    return loggedInConfo;
}

bool signUp(int choice) 
{
    string username, password;
    bool signUpSuccess = false;
    

    while (!signUpSuccess) 
    {
        cout << "Enter a username: " << endl;
        cin >> username;
    
        cout << endl;
        cout << "Enter a password: " << endl; 
        cin >> password;
    
        cout << "Signing up... " << endl;
        signUpSuccess = true;
    }
    
    return signUpSuccess; // confirms output
    // this part of the code will append the username and password given here into the database, other measures will be made to ensure that the passwords are secure (i.e. encryption techniques)    
}



void choiceFunction()
{
    int choice;
    
    cout << endl; 
    cout << "Main menu" << endl;
    
    
    cout << endl;
    cout << "1. Sign up " << endl;
    cout << "2. Login " << endl;      //cout << "Would you like to sign up/login? " << endl;
    cin >> choice;
    
    
    if (choice == 2)
    {
        cout << "You will be redirected to the login form... " << endl;
        bool loginResult = isLoggedIn(choice);
    }
    else if (choice == 1)
    {
        cout << "You will be directed to the sign-up form... " << endl;
        signUp(choice);
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
        cerr << "Database has not been connected... " << endl;
        sqlite3_close(db);
        return 1; // indicating error
    }
    else 
    {
        cout << "Database connection has been intialized! " << endl;
    }

    cout << "Test 1 successful" << endl; // if this part works, then this section works 
    choiceFunction(); // once database connection is established, proceed to go to the sign-in/login page

    sqlite3_close(db);
    return 0;
}