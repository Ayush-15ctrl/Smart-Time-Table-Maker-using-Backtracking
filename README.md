# Smart Timetable System (STS)

A comprehensive, full-stack timetable scheduling application built with a C++ core backtracking algorithm, a Node.js REST API backend, and a modern web dashboard. 

This project aims to automate and streamline the process of scheduling lectures, teachers, and rooms by resolving resource conflicts automatically using constraint satisfaction and backtracking logic.

## 🚀 Features

- **Automated Scheduling**: Employs an intelligent C++ and JS-based recursive backtracking algorithm to schedule lectures without clashes.
- **Resource Constraints**: Automatically manages overlapping constraints including:
  - No single teacher can teach two sections at the same time.
  - No single room can be booked for two sections at the same time.
  - Distinguishes between "Lab" classes (requires smaller rooms) and standard lectures (requires larger halls).
- **Role-Based Access**: Secure login/signup system with distinct roles for `admin` and regular `user` profiles.
- **SysAdmin Dashboard**: Admins can manage the user database, monitor all generated timetables, and delete resources.
- **Persistent Storage**: Uses local text files (`users.txt` and `timetable_*.txt`) for lightweight, seamless database management.
- **Modern User Interface**: A clean, responsive, and interactive frontend built with HTML, CSS, and Vanilla JavaScript.

## 💻 Tech Stack

- **Core Algorithm / Command Line Interface**: C++
- **Backend Server**: Node.js, Express.js
- **Frontend**: HTML5, CSS3, Vanilla JavaScript

## 🛠️ Prerequisites

To run this project locally, ensure you have the following installed:
1. **Node.js** (v14 or higher) - [Download Here](https://nodejs.org/)
2. **C++ Compiler** (e.g., GCC/g++ or MSVC) - To compile the core C++ backend files if you wish to run the CLI version.

## ⚙️ Installation & Running

### 1. Start the Node.js Server
The backend API server handles frontend requests, user authentication, and file management.
Open your terminal in the project directory and run:

```bash
# Install required dependencies (Express, CORS)
npm install express cors

# Start the server
node server.js
```
*The server will start running on `http://localhost:3000`.*

### 2. Access the Web Application
Once the server is running, you can access the frontend application:
- Simply open the `auth.html` file in your favorite web browser.
- Register for a new account or log in to generate timetables.

### 3. Run the C++ CLI (Optional)
If you want to use the pure command-line interface instead of the web dashboard:
```bash
# Compile the C++ files
g++ main.cpp auth.cpp display.cpp input.cpp scheduler.cpp -o timetable

# Run the executable
./timetable
```

## 📁 Project Structure

- `server.js`: The Express.js backend API routing and JavaScript implementation of the scheduler.
- `auth.html`: Frontend portal for user Login and Registration.
- `dashboard.html`: The central hub for authenticated users.
- `generate.html`: The interface where users input constraints and trigger timetable generation.
- `timetable-viewer.html`: The UI for loading, viewing, and managing saved timetables.
- `admin.html`: The dashboard for administrators to view and delete user accounts and timetables.
- `*.cpp`: The C++ files powering the algorithmic core and the alternative CLI interface.

## 🤝 Contribution
Feel free to fork this project, submit pull requests, or report issues!
