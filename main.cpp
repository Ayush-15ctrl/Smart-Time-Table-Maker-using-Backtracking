#include <iostream>
using namespace std;

void fetchInputs();
bool performScheduling(int curr);
void displayTimetable();

bool performLogin(string &user_role, string &user_name);
void performSignup();
void showAdminMenu();

void exportTimetable(string user_name);
void importTimetable(string user_name);
void showSavedTimetables(string user_name);
void removeTimetable(string user_name);

void runGenerator(string user_name) {
    fetchInputs();

    if(performScheduling(0)) {
        cout << "\nGeneration Successful!\n\n";
        displayTimetable();
        exportTimetable(user_name);
    } else {
        cout << "\nFailed to generate a valid timetable.\n";
    }
}

int main() {
    int user_choice;
    string user_role, user_name;

    while(true) {
        cout << "\n--- TIMETABLE SCHEDULER ---\n";
        cout << "1. Sign In\n";
        cout << "2. Register\n";
        cout << "3. Quit\n";
        cout << "Select option: ";
        cin >> user_choice;

        if(user_choice == 1) {
            if(performLogin(user_role, user_name)) {

                int sub_opt;
                do {
                    cout << "\n--- DASHBOARD ---\n";
                    cout << "1. Create Timetable\n";
                    cout << "2. View Saved Timetable\n";
                    cout << "3. List All Timetables\n";
                    cout << "4. Delete a Timetable\n";

                    if(user_role == "admin")
                        cout << "5. Administration Panel\n";

                    cout << "6. Log Out\n";
                    cin >> sub_opt;

                    if(sub_opt == 1) runGenerator(user_name);
                    else if(sub_opt == 2) importTimetable(user_name);
                    else if(sub_opt == 3) showSavedTimetables(user_name);
                    else if(sub_opt == 4) removeTimetable(user_name);
                    else if(sub_opt == 5 && user_role == "admin") showAdminMenu();

                } while(sub_opt != 6);
            }
        }
        else if(user_choice == 2) {
            performSignup();
        }
        else break;
    }

    return 0;
}