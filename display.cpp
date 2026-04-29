#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

extern int lecture_count;

struct Lecture {
    string course_name, instructor, section_name;
};

extern Lecture course_list[20];

struct Assignment {
    int day_idx, time_slot, room_no;
};

extern Assignment allocations[20];

void displayTimetable() {

    string day_labels[] = { "Mon","Tue","Wed","Thu","Fri","Sat" };

    string unique_classes[20];
    int cls_count = 0;
    for (int idx = 0; idx < lecture_count; idx++) {
        bool is_present = false;
        for (int c = 0; c < cls_count; c++) {
            if (unique_classes[c] == course_list[idx].section_name) {
                is_present = true;
                break;
            }
        }
        if (!is_present) {
            unique_classes[cls_count++] = course_list[idx].section_name;
        }
    }

    cout << "\n==================== MASTER SCHEDULE ====================\n";

    for (int c = 0; c < cls_count; c++) {
        string target_sec = unique_classes[c];
        cout << "\n--- GROUP: " << target_sec << " ---\n\n";

        string grid_data[6][9];
        for(int r = 0; r < 6; r++) {
            for(int c_idx = 0; c_idx < 9; c_idx++) {
                grid_data[r][c_idx] = "---";
            }
        }

        for(int idx = 0; idx < lecture_count; idx++) {
            if (course_list[idx].section_name == target_sec) {
                int d_val = allocations[idx].day_idx;
                int t_val = allocations[idx].time_slot;
                grid_data[d_val][t_val] = course_list[idx].course_name + "(" + course_list[idx].instructor + ",R" + to_string(allocations[idx].room_no + 1) + ")";
            }
        }

        cout << left << setw(10) << "Day" << setw(10) << "Time";
        for(int p_idx = 1; p_idx <= 9; p_idx++) {
            if(p_idx == 5) {
                cout << setw(10) << "LUNCH";
            }
            cout << setw(12) << ("Slot " + to_string(p_idx));
        }

        cout << "\n--------------------------------------------------------------------------\n";
        for(int r = 0; r < 6; r++) {

            cout << left << setw(10) << day_labels[r] << setw(10) << "8AM-6PM";
            for(int c_idx = 0; c_idx < 9; c_idx++) {

                if(c_idx == 4) {
                    cout << setw(10) << "---";
                }

                cout << setw(12) << grid_data[r][c_idx];
            }
            cout << endl;
        }

        cout << "--------------------------------------------------------------------------\n";
    }
    cout << "\n Slot Details:\n";
    cout << "Slot 1: 8-9 | Slot 2: 9-10 | Slot 3: 10-11 | Slot 4: 11-12\n";
    cout << "LUNCH: 12-1\n";
    cout << "Slot 5: 1-2 | Slot 6: 2-3 | Slot 7: 3-4 | Slot 8: 4-5 | Slot 9: 5-6\n";
}

void exportTimetable(string user_name) {

    string f_name = "timetable_" + user_name + ".txt";
    ofstream out_file(f_name);

    if(!out_file) {
        cout << "Failed to export!\n";
        return;
    }

    string day_labels[] = { "Mon","Tue","Wed","Thu","Fri","Sat" };

    string unique_classes[20];
    int cls_count = 0;
    for (int idx = 0; idx < lecture_count; idx++) {
        bool is_present = false;
        for (int c = 0; c < cls_count; c++) {
            if (unique_classes[c] == course_list[idx].section_name) {
                is_present = true;
                break;
            }
        }
        if (!is_present) {
            unique_classes[cls_count++] = course_list[idx].section_name;
        }
    }

    for (int c = 0; c < cls_count; c++) {
        string target_sec = unique_classes[c];
        out_file << "==================== GROUP: " << target_sec << " ====================\n\n";

        string grid_data[6][9];
        for(int r = 0; r < 6; r++) {
            for(int c_idx = 0; c_idx < 9; c_idx++) {
                grid_data[r][c_idx] = "---";
            }
        }

        for(int idx = 0; idx < lecture_count; idx++) {
            if (course_list[idx].section_name == target_sec) {
                int d_val = allocations[idx].day_idx;
                int t_val = allocations[idx].time_slot;
                grid_data[d_val][t_val] = course_list[idx].course_name + "(" + course_list[idx].instructor + ",R" + to_string(allocations[idx].room_no + 1) + ")";
            }
        }

        for(int r = 0; r < 6; r++) {
            out_file << day_labels[r] << ":\n";

            for(int c_idx = 0; c_idx < 9; c_idx++) {
                out_file << "Slot " << c_idx+1 << " -> " << grid_data[r][c_idx] << "\n";
            }

            out_file << "-----------------------------\n";
        }
        out_file << "\n";
    }

    out_file.close();
    cout << "Exported as " << f_name << endl;
}

void importTimetable(string user_name) {
    string f_name;
    cout << "Input file name: ";
    cin >> f_name;

    ifstream in_file(f_name);

    if(!in_file) {
        cout << "Missing file!\n";
        return;
    }

    string ln_data;
    cout << "\n--- RETRIEVED DATA ---\n\n";

    while(getline(in_file, ln_data)) {
        cout << ln_data << endl;
    }

    in_file.close();
}

void showSavedTimetables(string user_name) {
    cout << "\nLook for files matching:\n";
    cout << "timetable_" << user_name << ".txt\n";
}

void removeTimetable(string user_name) {
    string f_name;
    cout << "Input file name to erase: ";
    cin >> f_name;

    if(remove(f_name.c_str()) == 0) cout << "File erased\n";
    else cout << "Missing file\n";
}