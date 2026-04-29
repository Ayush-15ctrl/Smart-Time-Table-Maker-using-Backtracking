#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;

bool performLogin(string &user_role, string &user_name) {
    string u_input, p_input;
    string f_user, f_pass, f_role;

    cout << "\n--- SIGN IN ---\n";
    cout << "User ID: ";
    cin >> u_input;
    cout << "Passcode: ";
    cin >> p_input;

    ifstream fs("users.txt");

    if(!fs) {
        cout << "Could not open user database!\n";
        return false;
    }

    while(fs >> f_user >> f_pass >> f_role) {
        if(u_input == f_user && p_input == f_pass) {
            user_role = f_role;
            user_name = u_input;
            cout << "Signed in successfully!\n";
            return true;
        }
    }

    cout << "Incorrect credentials!\n";
    return false;
}

void performSignup() {
    string new_usr, new_pwd;
    string curr_u, curr_p, curr_r;

    cout << "\n--- REGISTER ---\n";
    cout << "Desired User ID: ";
    cin >> new_usr;

    ifstream fs("users.txt");
    while(fs >> curr_u >> curr_p >> curr_r) {
        if(new_usr == curr_u) {
            cout << "User ID already taken!\n";
            return;
        }
    }
    fs.close();

    cout << "Desired Passcode: ";
    cin >> new_pwd;

    ofstream os("users.txt", ios::app);
    os << new_usr << " " << new_pwd << " user" << endl;

    cout << "Registration complete. Please sign in.\n";
}

void displayAllUsers() {
    ifstream fs("users.txt");
    string usr, pwd, rol;

    cout << "\n--- USER LIST ---\n";

    if(!fs) {
        cout << "No records found!\n";
        return;
    }

    while(fs >> usr >> pwd >> rol) {
        cout << "ID: " << usr << " | Access: " << rol << endl;
    }
}

void removeUserRecord() {
    string tgt;
    cout << "Input ID to remove: ";
    cin >> tgt;

    ifstream in_fs("users.txt");
    ofstream out_fs("temp.txt");

    string usr, pwd, rol;
    bool is_found = false;

    while(in_fs >> usr >> pwd >> rol) {
        if(usr != tgt) {
            out_fs << usr << " " << pwd << " " << rol << endl;
        } else {
            is_found = true;
        }
    }

    in_fs.close();
    out_fs.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if(is_found)
        cout << "Record removed.\n";
    else
        cout << "Record does not exist.\n";
}

void browseAllFiles() {
    cout << "\n--- SAVED FILES ---\n";
    cout << "Look for files matching the pattern:\n";
    cout << "timetable_<id>_<num>.txt\n";
}

void eraseAnyFile() {
    string target_fname;
    cout << "File to erase: ";
    cin >> target_fname;

    if(remove(target_fname.c_str()) == 0) cout << "File erased.\n";
    else cout << "File missing.\n";
}

void showAdminMenu() {
    int admin_opt;

    do {
        cout << "\n--- ADMINISTRATION ---\n";
        cout << "1. List Users\n";
        cout << "2. Remove User\n";
        cout << "3. List Saved Files\n";
        cout << "4. Remove File\n";
        cout << "5. Return\n";
        cout << "Select option: ";
        cin >> admin_opt;

        if(admin_opt == 1) displayAllUsers();
        else if(admin_opt == 2) removeUserRecord();
        else if(admin_opt == 3) browseAllFiles();
        else if(admin_opt == 4) eraseAnyFile();

    } while(admin_opt != 5);
}