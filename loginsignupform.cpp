// C++ Login + Registration form  

// pre-requisite libraries
#include <iostream>
#include <fstream> 
#include <string>  
#include <sqlite3.h> // header library for SQL command utilisation
#include <stdlib.h> // standard library 


using namespace std;


// void hashingAlgorithm() {} // for further encrypting the password


void loginSession(bool loggedInConfo, int UID) { 
    sqlite3* db;
    int rc;
    sqlite3_stmt* stmt;
    int choice;
    int numOfReminders;

    sqlite3_open("userdata.db", &db); 

    cout << "Welcome to my reminders\n";

    const char* query = "SELECT * FROM reminderPrompt";

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        cout << "List has failed to load\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db); // in the instance that user has failed to load, then the database will close
        exit(0);
    }

    rc = sqlite3_bind_int(stmt, 1, UID);

    if (rc != SQLITE_OK)
    {
        cout << "UID has not been binded \n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char* reminder = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        cout << "Current reminder: " << reminder << endl;
    }


    cout << "Options \n" << endl;
    cout << endl;
    cout << "1. Add reminders\n";
    cout << "2. Log out \n";
    cin >> choice;

    if (choice == 1)
    {
        int i;
        cout << "How many reminders do you want?";
        cin >> numOfReminders;

        for (i = 0; numOfReminders; i++)
        {
            string reminderInput;
            cout << "Reminder no. " << i; // the value i will keep incrementing to indicate the number of reminder
            cin >> reminderInput;
            
            sqlite3_open("userdata.db", &db);

            const char* addReminder = "SELECT reminder FROM reminderPrompt;";

            sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

            rc = sqlite3_bind_text(stmt, 1, reminderInput.c_str(), -1, SQLITE_STATIC);

            if (rc != SQLITE_OK)
            {
                cerr << "Reminder binding has failed!\n";
            }
            else 
            {
                cout << "Reminder binding successful\n"; // testing measures
            }

            rc = sqlite3_step(stmt);

            if (rc != SQLITE_OK)
            {
                cerr << "Reminder has failed to append to the database :( \n"; // testing measures
            }
            else
            {
                cout << "Reminder has successfully appended to the database\n";
            }

        }

    }
    else if (choice == 2)
    {
        cout << "Log out initiated, see you soon!\n" << endl;
        exit(0);
    }

    else 
    {
        cout << "Invalid input, try again";
        loginSession(loggedInConfo, UID); // recurse till correct info is inputted
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

} 


bool isLoggedIn(int choice, int retryAttempts) 
{
    string un, pw;
    int rc; // return code 
    bool loggedInConfo = false; // local variables
    sqlite3_stmt *stmt; 
    sqlite3* db;
    int result;

    rc = sqlite3_open("userdata.db", &db);

    if (rc != SQLITE_OK)
    {
        cerr << "Database failed to initialize \n";
        sqlite3_close(db);
        return false;
    }
    else 
    {
        cout << "Database has successfully loaded!\n"; // testing measures will be removed once completed
    }

    const char* sql = "SELECT userID, username, password FROM users WHERE username = ? AND password = ?";
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
        cout << "Statement successfully initialized\n";
    }
 
    cout << "Enter your username:\n";
    cin >> un;

    cout << "Enter your password:\n ";
    cin >> pw;

    cout << "Logging in...\n";

    rc = sqlite3_bind_text(stmt, 1, un.c_str(), -1, SQLITE_STATIC);

    if (rc != SQLITE_OK)
    {
        cerr << "Error binding username\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    else 
    {
        cout << "Username processing complete... \n";
    }

    rc = sqlite3_bind_text(stmt, 2, pw.c_str(), -1, SQLITE_STATIC);

    if (rc != SQLITE_OK)
    {
        cerr << "Password has not been initialized\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    else
    {
        cout << "Password processing complete... \n";
    }

    do 
    { 
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
        {
            int UID = sqlite3_column_int(stmt, 0); // this will figure out userID of the person
            cout << "User ID: " << UID << endl;
            cout << "User authenticated.\n" << endl;
            loggedInConfo = true;
            loginSession(loggedInConfo, UID);
            break;
        }
        else 
        {
            cerr << "Details are incorrect, please try again\n" << endl;

            if (retryAttempts > 0)
            {
                return isLoggedIn(choice, retryAttempts - 1); // recurse to allow the user to input their details again until they are correct.
            }
            else
            {
                cerr << "Too many attempts made, terminating... ";
                break;
            }
        }

    } while (!loggedInConfo);

    return loggedInConfo;
} 

bool signUp(int choice, int retryAttempts) 
{
    string un, pw;
    bool signUpSuccess = false; // local variables
    sqlite3_stmt* stmt;
    int rc;
    sqlite3* db;

    rc = sqlite3_open("userdata.db", &db);

    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?)";
    ;rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); 

    if (rc != SQLITE_OK)
    {
        cerr << "Error preparing statement\n" << endl;
        return false;
    }
    else 
    {
        cout << "Sucessfully prepared statement\n" << endl;
    }

    while (!signUpSuccess) 
    {
        cout << "Enter a new username:\n ";
        cin >> un;
    
        cout << "Enter a new password:\n "; 
        cin >> pw;
    
        cout << "Signing up... \n";

        rc = sqlite3_bind_text(stmt, 1, un.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK) {
            cerr << "Error binding username\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            break;
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
            break;
        }
        else 
        {
            cout << "Sucessfully binded password\n" << endl;
        }

        int result = sqlite3_step(stmt); // insert the new user to the database once compilation is completed


        if (result != SQLITE_DONE) 
        {
            cerr << "Sign up has failed to insert to the database " << endl;
        
            if (retryAttempts > 0) // mitigating infinite recursion to prevent stack overflow errors
            {
                signUp(choice, retryAttempts - 1);
            }
            else
            {
                cerr << "Too many attempts, program terminating...";
                exit(0);
            }
        }
        else 
        {
            cout << "Sign up has been successfully initiated! " << endl; // in the instance that it is a success, then it will be outputted, or otherwise it has failed to append
            signUpSuccess = true;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    }
    return signUpSuccess; // confirms output, other measures will be made to ensure that the passwords are secure (i.e. encryption techniques such as a hash salt)    
}



void choiceFunction()
{
    int choice;
    int retryAttempts = 3; // user can only input details 3 times before termination to prevent infinite recursion
    
    cout << "Reminder System Main Menu\n" << endl;
    cout << endl;
    cout << "------- Main Menu -------\n";
    
    
    cout << "1. Sign up\n";
    cout << "2. Login\n";    
    cin >> choice;
    
    
    if (choice == 1)
    {
        cout << "You will be redirected to the sign-up form... " << endl;
        bool signUpProcess = signUp(choice, retryAttempts);
    }
    else if (choice == 2)
    {
        cout << "You will be redirected to the login form... " << endl;
        bool loginResult = isLoggedIn(choice, retryAttempts);
    }
    else
    {
        cerr << "Input not recognized, please use a sensible input" << endl;
        choiceFunction(); // keep recusing till right input is made 
    }
}


int main()
{
    sqlite3* db;
    int rc; // return code

    rc = sqlite3_open("userdata.db", &db);

    if (rc != SQLITE_OK) 
    {
        cerr << "Users database hasn't been initialized\n";
    }
    else 
    {
        cout << "Users database has been intialized\n"; // test
    }

    choiceFunction(); // once database connection is established, proceed to go to the sign-in/login page

    sqlite3_close(db);
    return 0;
}
















