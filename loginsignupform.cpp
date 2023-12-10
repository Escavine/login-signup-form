/* C++ Login + Registration form  */

#include <iostream>
#include <fstream> /* importing file library */
#include <string>
#include <sqlite3.h> // database for users

using namespace std;



bool isLoggedIn(const string& choice) {
    string username, password;
    string storedUsername, storedPassword;  // Define storedPassword here

    cout << "Enter your username: ";
    cin >> username;

    cout << "Enter your password: ";
    cin >> password;

    ifstream read(username + ".txt");

    if (read.is_open()) {
        getline(read, storedUsername);
        getline(read, storedPassword);

        if (storedUsername == username && storedPassword == password) {
            cout << "User has successfully logged in!" << endl;

            cout << endl;

            cout << "Welcome " << username << endl;

            cout << endl;

            cout << username << "'s " << "Dashboard: " << endl;

        } else {
            cout << "Details are incorrect. Please input details again!" << endl;
            return false;
        }
    } else {
        cout << "User not found. Please input details again!" << endl;
        isLoggedIn(choice);
    }

}

bool signUp(string choice)
{
    
    string username, password;
    string un, pw;
    
    cout << "Enter a username: " << endl;
    cin >> username;
    
    cout << endl; /* added space */
    cout << "Enter a password: " << endl; /* make a process to encrypt user/passwords */
    cin >> password;
    
    cout << "Signing up... " << endl;
    
    ifstream read(username + ".txt");
    
    if (read.is_open())
    {
        getline(read, un);
        getline(read, pw);
        
        if (un == username && pw == password) 
        {
            cout << "User has successfully signed up! " << endl;
            return true;
        }
    else 
    {
        cout << "Details are incorrect, please input details again! " << endl;
        isLoggedIn(choice);
        return false;
    }
   } 
}



void choiceFunction()
{
    string choice;
    
    cout << endl; /* added space for aesthetics */
    cout << "Main menu" << endl;
    
    
    cout << endl;
    cout << "Would you like to sign up/login? " << endl;
    cin >> choice;
    
    
    if (choice == "login" || choice == "Login")
    {
        cout << "You will be redirected to the login form... " << endl;
        isLoggedIn(choice);
    }
    else if (choice == "signup" || choice == "sign-up")
    {
        cout << "You will be directed to the sign-up form... " << endl;
        signUp(choice);
    }
    else
    {
        cout << "Input not recognized, please use a sensible input" << endl;
        choiceFunction(); /* keep recusing till right details are inputted  */
    }
}


int main()
{
    sqlite3* db;
    choiceFunction();
    return 0;
}