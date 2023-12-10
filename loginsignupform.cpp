// C++ Login + Registration form  

#include <iostream>
#include <fstream> // importing file library 
#include <string>
#include <sqlite3.h> // database for users

using namespace std;



bool isLoggedIn(std::string choice) {
    std::string username, password;
    std::string storedUsername = "John";
    std::string storedPassword = "Doe";
    bool loggedInConfo = false;

    while (!loggedInConfo) {
        std::cout << "Enter your username: " << std::endl;
        std::cin >> username;

        std::cout << "Enter your password: " << std::endl;
        std::cin >> password;

        std::cout << "Logging in... " << std::endl;

        if (username == storedUsername && password == storedPassword) {
            std::cout << "Login Success! " << std::endl;
            loggedInConfo = true;
        } else {
            std::cout << "Details are incorrect, please input details again. " << std::endl;
        }
    }

    // This part of the code will proceed to read from the database and compare the input to the stored values in the given database,
    // If they are matched, then the user will then see their dashboard

    return loggedInConfo;
}

bool signUp(string choice) 
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
    
    return signUpSuccess;
    // this part of the code will append the username and password given here into the database, other measures will be made to ensure that the passwords are secure (i.e. encryption techniques)    
}



void choiceFunction()
{
    string choice;
    
    cout << endl; 
    cout << "Main menu" << endl;
    
    
    cout << endl;
    cout << "Would you like to sign up/login? " << endl;
    cin >> choice;
    
    
    if (choice == "login" || choice == "Login")
    {
        cout << "You will be redirected to the login form... " << endl;
        bool loginResult = isLoggedIn(choice);
    }
    else if (choice == "signup" || choice == "sign-up")
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

    choiceFunction(); // once database connection is established, proceed to go to the sign-in/login page

    sqlite3_close(db);
    return 0;
}