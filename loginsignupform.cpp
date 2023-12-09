/* C++ Login + Registration form  */

#include <iostream>
#include <fstream> /* importing file library */
#include <string>

using namespace std;


bool isLoggedIn(string choice)
{
    string username, password;
    string un, pw;
    
    cout << "Enter your username: " << endl;
    cin >> username;
    cout << "Enter your password: " << endl;
    cin >> password;
    
    ifstream read(username + ".txt");
    getline(read, un);
    getline(read, pw);
    
    if (username == un && password == pw)
    {
        cout << "User has successfully logged in! " << endl;
        return true;
    }
    else 
    {
        cout << "Details are incorrect, please input details again! " << endl;
        isLoggedIn(choice); /* recurse till correct details are inputted */
    }
    
}


bool signUp(string choice)
{
    string username, password;
    string un, pw;
    
    cout << "Enter a username: " << endl;
    cin >> username;
    
    cout << "Enter a password: " << endl;
    cin >> password;
    
    cout << "Signing up... " << endl;
    
    ifstream read(username, ".txt");
    getline(read, un);
    getline(read, pw);
    
    if (username == un && pw == password)
    {
        cout << "User has successfully signed up! " << endl;
        return true;
    }
    else 
    {
        cout << "Details are incorrect, please input details again! " << endl;
        signUp(choice); /* recurse till user details are viable for signing up */
    }
}


void choiceFunction()
{
    string choice;
    cout << "Would you like to sign up/login? " << endl;
    cin >> choice;
    
    
    if (choice == "login")
    {
        cout << "You will be redirected to the login form... " << endl;
        isLoggedIn(choice);
    }
    else if (choice == "signup" || choice == "sign-up")
    {
        cout << "You will be taken to the sign-up form... " << endl;
        signUp(choice);
    }
    else
    {
        cout << "Input not recognized, please use a sensible input" << endl;
        choiceFunction() /* keep recusing till right details are inputted  */
    }
}


int main() {
    choiceFunction();
    return 0;
}