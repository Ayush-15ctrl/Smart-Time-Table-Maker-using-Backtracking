#include <iostream>
using namespace std;

struct Lecture {
    string course_name, instructor, section_name;
};

Lecture course_list[20];
int lecture_count;

int NUM_DAYS = 6, NUM_PERIODS = 9, NUM_ROOMS = 20;

void fetchInputs() {
    cout << "\nInput total number of classes to schedule: ";
    cin >> lecture_count;

    cin.ignore();

    for(int idx = 0; idx < lecture_count; idx++) {
        cout << "\nClass details " << idx+1 << ":\n";

        cout << "Course Name: ";
        getline(cin, course_list[idx].course_name);
        
        cout << "Instructor Name: ";
        getline(cin, course_list[idx].instructor); 
        
        cout << "Target Section: ";
        getline(cin, course_list[idx].section_name);
    }
}