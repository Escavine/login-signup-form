/* C++ Login + Registration form  */

#include <iostream>
#include <fstream> /* importing file library */
#include <string>

using namespace std;


bool isLoggedIn(string choice) {
    
    string username, password;
    string un, pw;
    
    cout << "Enter your username: " << endl;
    cin >> username;
    
    cout << endl; /* added space */
    cout << "Enter your password: " << endl;
    cin >> password;
    
    
    ifstream read(username + ".txt");
    
    if (read.is_open())
    {
        getline(read, un);
        getline(read, pw);
        
        if (un == username && pw == password) {
            cout << "User has successfully logged in! " << endl;
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


bool signUp(string choice)
{
    
    string username, password;
    string un, pw;
    
    cout << "Enter a username: " << endl;
    cin >> username;
    
    cout << endl; /* added space */
    cout << "Enter a password: " << endl;
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


int main() {
    choiceFunction();
    return 0;
}