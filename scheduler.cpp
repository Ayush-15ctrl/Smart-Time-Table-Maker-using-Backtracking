#include <iostream>
using namespace std;

extern int lecture_count;
extern int NUM_DAYS, NUM_PERIODS, NUM_ROOMS;
int room_sizes[20] = {120, 110, 100, 90, 80, 70, 60, 55, 120, 110, 50, 45, 40, 35, 30, 45, 50, 40, 30, 50};

struct Lecture {
    string course_name, instructor, section_name;
};

extern Lecture course_list[20];

struct Assignment {
    int day_idx, time_slot, room_no;
};

Assignment allocations[20];

bool checkLab(string c_name){
    return c_name.find("Lab") != string::npos;
}

bool canSchedule(int curr, int d, int t, int r) {
    for(int i = 0; i < curr; i++) {
        if(allocations[i].day_idx == d && allocations[i].time_slot == t && allocations[i].room_no == r) {
            return false;
        }
        
        if(allocations[i].day_idx == d && allocations[i].time_slot == t) {
            if(course_list[i].instructor == course_list[curr].instructor) return false;
            if(course_list[i].section_name == course_list[curr].section_name) return false;
        }
    }

    if(checkLab(course_list[curr].course_name)) {
        if(room_sizes[r] > 50) return false;
    } else {
        if(room_sizes[r] <= 50) return false;
    }
    
    return true;
}

bool performScheduling(int curr) {
    if(curr == lecture_count) return true;

    for(int d = 0; d < NUM_DAYS; d++) {
        for(int t = 0; t < NUM_PERIODS; t++) {
            for(int r = 0; r < NUM_ROOMS; r++) {

                if(canSchedule(curr, d, t, r)) {
                    allocations[curr] = {d, t, r};

                    if(performScheduling(curr + 1))
                        return true;

                    allocations[curr] = {-1, -1, -1};
                }
            }
        }
    }
    return false;
}