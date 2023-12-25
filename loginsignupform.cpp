// C++ Reminders Project

// pre-requisite libraries
#include <iostream>
#include <fstream> 
#include <string>  
#include <sqlite3.h> // header library for SQL command utilisation
#include <stdlib.h> // standard library 
#include <sstream>
#include <limits>
#include <chrono>
#include <thread>


using namespace std;


// void hashingAlgorithm() {} // for further encrypting the password


string bindText(sqlite3_stmt* stmt, const string& reminderInput)
{
    int rc;

    do 
    {
        rc = sqlite3_bind_text(stmt, 1, reminderInput.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK) 
        {
            cerr << "Input bind fail!" << "Error code: " << sqlite3_errcode(sqlite3_db_handle(stmt)) << "\n" 
                 << "Error message: " << sqlite3_errmsg(sqlite3_db_handle(stmt)) << "\n" << endl; // ChatGPT changes
        }

    } while (rc == SQLITE_BUSY);

    return "Text has been binded successfully!";
}

int bindInt(sqlite3_stmt* stmt, int UID)
{
    int rc;

    do
    {
        rc = sqlite3_bind_int(stmt, 2, UID);

        if (rc != SQLITE_OK)
        {
            cerr << "userID has failed to bind to the reminders table! " << "Error code: " << sqlite3_errcode(sqlite3_db_handle(stmt)) << "\n" 
                 << "Error message: " << sqlite3_errmsg(sqlite3_db_handle(stmt)) << "\n" << endl; // ChatGPT changes
        }

    } while (rc == SQLITE_BUSY);

    return rc;
}


bool addRemindersToUserTable(int UID)
{
    string reminderInput;
    int numOfReminders;
    int rc; // return code for testing measures
    sqlite3* db;
    sqlite3_stmt* stmt;

    cout << "How many reminders do you wish to add? \n";
    cin >> numOfReminders;

    // Add this line to consume the newline character left by cin >>
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "Affirming userID before adding reminder: " << "(ID is " << UID << " )" << endl; // testing measures (IDS ARE ZERO BASED)

    rc = sqlite3_open("userdata.db", &db);

    if (rc != SQLITE_OK)
    {
        cerr << "Error opening database\n";
        return false;
    }

    // Begin a transaction (ChatGPT prompt)
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    if (rc != SQLITE_OK)
    {
        cerr << "Failed to begin transaction\n";
        sqlite3_close(db);
        return false;
    }

    string addReminder = "INSERT INTO userReminders_" + to_string(UID) + " (individualReminder, userID) VALUES (?, ?)";
    const char* charQuery = addReminder.c_str();

    // cout << "TESTING MEASURE FOR SQL STATEMENT: " << charQuery << endl; 

    rc = sqlite3_prepare_v2(db, charQuery, -1, &stmt, nullptr);  // ready the SQL statement
    
    if (rc != SQLITE_OK)
    {
        cerr << "SQL statement initialization has failed! " << "Error code: " << sqlite3_errcode(db) << "\n" << "Error message: " << sqlite3_errmsg(db) << "\n" << endl;
        sqlite3_close(db);
        return false;
    }

    for (int i = 0; i < numOfReminders; i++) 
    {
        cout << "Write the details of the given reminder (No. " << i + 1 << "): ";
        cin >> reminderInput;

        string resultText = bindText(stmt, reminderInput);
        cout << resultText << endl;

        int resultInt = bindInt(stmt, UID);

        int result = sqlite3_step(stmt);

        if (resultInt != SQLITE_OK)
        {
            cerr << "Error binding UID to the reminders table!" << endl;
            sqlite3_reset(stmt);
            continue; // Continue to the next iteration without finalizing the statement
        }

        if (result != SQLITE_DONE) 
        {
            cerr << "Reminder has failed to append to the database: " << "Error code: " << sqlite3_errcode(db) << "\n" 
                 << "Error message: " << sqlite3_errmsg(db) << "\n" << endl;

            // Introduce a delay to simulate a longer transaction time
            this_thread::sleep_for(chrono::seconds(1));

            // Reset the statement for the next iteration
            rc = sqlite3_reset(stmt);
            continue; // Continue to the next iteration without finalizing the statement
        } 
        else 
        {
            cout << "Reminder has successfully appended to the database!" << endl;
        }
    }

    // Commit the transaction (ChatGPT prompt)
    rc = sqlite3_exec(db, "COMMIT", 0, 0, 0);
    if (rc != SQLITE_OK)
    {
        cerr << "Failed to commit transaction\n";
    }

    rc = sqlite3_finalize(stmt); // ensuring statement properly finalizes
    sqlite3_close(db);
    return true;
}


bool loadingUserReminders(int UID) 
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open("userdata.db", &db);

    if (rc != SQLITE_OK)
    {
        cerr << "Error opening database\n" << "Error message: " << sqlite3_errmsg(db) << "\n" << "Error code: " << sqlite3_errcode(db) << endl;
        return false;
    }

    string query = "SELECT individualReminder, uniqueReminderID FROM userReminders_" + to_string(UID);
    const char* charQuery = query.c_str();

    // cout << charQuery << endl; 
    // cout << "TESTING MEASURE TO CONFIRM USER ID: " << UID << endl; 

    rc = sqlite3_prepare_v2(db, charQuery, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        cerr << "Error preparing statement\n" << "Error message: " << sqlite3_errmsg(db) << "\n" << "Error code: " << sqlite3_errcode(db) << endl;
        return false;

    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char* reminder = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        cout << "Current reminder: " << reminder << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}


bool loginSession(bool loggedInConfo, int UID, string personalName, string personalSurname) // once user is authenticated, this function will run
{ 
    sqlite3* db;
    int rc;
    sqlite3_stmt* stmt;
    int choice;

    sqlite3_open("userdata.db", &db); // open the database

    cout << "My Reminders App\n";


    int result = loadingUserReminders(UID);


    cout << "Options \n";
    cout << endl;
    cout << "1. Add reminders\n";
    cout << "2. Log out \n";
    cout << "3. Remove reminders (NOT WORKING YET)\n";
    cin >> choice;

    if (choice == 1)
    {
        addRemindersToUserTable(UID); // function call will send the given argument UID to allow for user to add reminders to their table
    }

    else if (choice == 2)
    {
        cout << "Log out initiated, see you soon!\n" << endl;
        int i;
        int seconds = 5; // countdown

        for (i = seconds; i >=0; i--)
        {
            cout << "Countdown intiated: " << i << "seconds" << endl;
        }

        exit(0);
    }


    else 
    {
        cerr << "Invalid input, try again";
        loginSession(loggedInConfo, UID, personalName, personalSurname); // recurse till correct info is inputted
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}




bool isLoggedIn(int choice, int retryAttempts) {
    string un, pw;
    int rc; // return code 
    bool loggedInConfo = false; // local variables
    sqlite3_stmt *stmt; 
    sqlite3* db;

    rc = sqlite3_open("userdata.db", &db);

    const char* sql = "SELECT * FROM users WHERE username = ? AND password = ?";
    ;rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); 

    if (rc != SQLITE_OK)
    {
        cerr << "Error preparing statement\n";
        sqlite3_close(db);
        return false;
    }
 

    while (!loggedInConfo) 
    {
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
            break;
        }

        rc = sqlite3_bind_text(stmt, 2, pw.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK)
        {
            cerr << "Password has not been initalized\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            break;
        }


        int result = sqlite3_step(stmt);


        if (result == SQLITE_ROW)
        {
            int UID = sqlite3_column_int(stmt, 0); // this will figure out userID of person
            string personalName = (const char*)sqlite3_column_text(stmt, 3); // will find the name of the individual
            string personalSurname = (const char*)sqlite3_column_text(stmt, 4); // will find the surname of the individual
            cout << "User ID: " << UID << endl;
            cout << "User authenticated.\n" << endl;
            cout << "Welcome " << personalName << " " << personalSurname << endl; // testing measure 
            loggedInConfo = true;
            loginSession(loggedInConfo, UID, personalName, personalSurname);
        }
        else 
        {
            cerr << "Details are incorrect, please try again\n" << endl;

            if (retryAttempts > 0)
            {
                isLoggedIn(choice, retryAttempts - 1); // recurse to allow user to input their details again till they are correct.
            }
            else
            {
                cerr << "Too many attempts made, terminating... ";
                exit(0);
            }
        }

    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return loggedInConfo;
}



string generateRemindersTableName(int UID)
{
    return "userReminders_" + to_string(UID); // will concatenate the table name with the userID to prevent data integrity issues
}   


void reminderTableGeneration(int UID, string remindersTableName) // table will be generated for a new user
{
    sqlite3* db;
    int rc;

    sqlite3_open("userdata.db", &db);

    string createRemindersTable = R"(
        CREATE TABLE )" + remindersTableName + R"(
        (
            uniqueReminderID INTEGER PRIMARY KEY NOT NULL,
            individualReminder TEXT,
            userID INTEGER,
            FOREIGN KEY (userID) REFERENCES users(userID)
        )
    )";

    const char* query = createRemindersTable.c_str(); // convert to string
    
    // cout << "Query: " << createRemindersTable.c_str() << endl; 

    rc = sqlite3_exec(db, query, nullptr, nullptr, nullptr);


    if (rc != SQLITE_OK)
    {
        cerr << "Issue generating table\n" << sqlite3_errmsg(db);
    }
    else
    {
        cout << "Successfully generated table!\n";
    }
    sqlite3_close(db);

}


bool signUp(int choice, int retryAttempts) 
{
    string un, pw, personalName, personalSurname;
    bool signUpSuccess = false; 
    sqlite3_stmt* stmt;
    int rc;
    sqlite3* db;

    rc = sqlite3_open("userdata.db", &db);

    const char* sql = "INSERT INTO users (username, password, individualName, individualSurname) VALUES (?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); 

    if (rc != SQLITE_OK)
    {
        cerr << "Error preparing statement\n" << endl;
        return false;
    }

    while (!signUpSuccess) 
    {
        cout << "Enter a new username:\n ";
        cin >> un;
    
        cout << "Enter a new password:\n "; // FUTURE REFERENCE: ENSURE THAT PASSWORD ALLOWS FOR 6/8 CHARACTERS, 1 CAPITAL LETTER AND A SPECIAL CHARACTER
        cin >> pw;

        cout << "What is your name?\n";
        cin >> personalName;

        cout << "What is your surname?\n";
        cin >> personalSurname;
    
        cout << "Signing up... \n";

        rc = sqlite3_bind_text(stmt, 1, un.c_str(), -1, SQLITE_STATIC); // to ensure unique usernames, make a condition to ensure it isn't present in any of the users rows

        if (rc != SQLITE_OK) {
            cerr << "Error binding username\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            break;
        }


        rc = sqlite3_bind_text(stmt, 2, pw.c_str(), -1, SQLITE_STATIC); 


        if (rc != SQLITE_OK)
        { 
            cerr << "Error binding password\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            break;
        }


        rc = sqlite3_bind_text(stmt, 3, personalName.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK)
        { 
            cerr << "Error binding name\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            break;
        }


        rc = sqlite3_bind_text(stmt, 4, personalSurname.c_str(), -1, SQLITE_STATIC);

        if (rc != SQLITE_OK)
        { 
            cerr << "Error binding surname\n" << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            break;
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

            // code below will generate the 'reminders' table for each individual user after they've been inserted into the 'users' table
            int UID = sqlite3_last_insert_rowid(db);
            string remindersTableName = generateRemindersTableName(UID); // function call that will ensure that all titles of the reminders tables are unique
            reminderTableGeneration(UID, remindersTableName); // function call that will generate a unique reminders table for the user
            signUpSuccess = true;
        }

        bool relogin = false;
        do {
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
                relogin = true;
            }
            else if (choice == 2)
            {
                cout << "You will be redirected to the login form... " << endl;
                bool loginResult = isLoggedIn(choice, retryAttempts);
                relogin = true;
            }
            else
            {
                cerr << "Input not recognized, please use a sensible input" << endl;
            }
    
          } while (relogin = false);

    }
            
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return signUpSuccess; // confirms output, other measures will be made to ensure that the passwords are secure (i.e. encryption techniques such as a hash salt)    
}



void clearInputBuffer()
{
    cin.clear(); // clears error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // disards characters until a new line is created
}


void choiceFunction(int retryAttempts) {
    int choice;
    bool acceptableInput = false;

    cout << "Reminder System Main Menu\n" << endl;
    cout << "------- Main Menu -------\n";

    cout << "1. Sign up\n";
    cout << "2. Login\n";
    cin >> choice;

    if (cin.fail()) 
    {
        cerr << "Invalid input, please try again\n" << endl;
        clearInputBuffer();
        choiceFunction(retryAttempts);
        return;
    }

    if (choice == 1) 
    {
        cout << "You will be redirected to the sign-up form... " << endl;
        acceptableInput = true;
        bool signUpProcess = signUp(choice, retryAttempts);
    } 
    else if (choice == 2) 
    {
        cout << "You will be redirected to the login form... " << endl;
        acceptableInput = true;
        bool loginResult = isLoggedIn(choice, retryAttempts);
    } 
    else 
    {
        if (choice > 2 || choice < 0)
        {
            cerr << "Invalid input, please try again\n" << endl;
            clearInputBuffer();
            choiceFunction(retryAttempts - 1);
            return;
        } 
        else if (retryAttempts == 0)
        {
            cerr << "Too many incorrect attempts, program will now terminate... " << endl;
            exit(0);
        }
    }
}
   


int main()
{
    sqlite3* db;
    int rc; // return code
    int retryAttempts = 3; // prevent input trash

    rc = sqlite3_open("userdata.db", &db);

    if (rc != SQLITE_OK) 
    {
        cerr << "Users database hasn't been initialized\n";
    }

    choiceFunction(retryAttempts); // once database connection is established, proceed to go to the sign-in/login page

    sqlite3_close(db);
    return 0;
}
















