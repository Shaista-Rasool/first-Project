#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>
#include <windows.h>
using namespace std;

struct Student {
    string name,id,grade,address,dob,contact;
};

vector<Student> students;

void loadFromFile();
void saveToFile();
void addStudent();
void searchRecord();
void updateRecord();
void deleteRecord();
void showRecords();
void sortRecords();
void displayMenu();
int getValidatedChoice();
void clearScreen();
void displayError(const string&message);
void displaySuccess(const string&message);
bool validateContact(const string&contact);
bool validateDOB(const string&dob);
bool isUniqueID(const string&id);
void confirmExit();
void setColor(int color);

void loadFromFile() {
    ifstream file("records.txt");
    if(!file) {
        displayError("Could not open records file. Starting fresh.");
        return;
    }
    Student student;
    while(file>>student.id>>student.name>>student.grade>>student.address>>student.dob>>student.contact) {
        students.push_back(student);
    }
    file.close();
}

void saveToFile() {
    ofstream file("records.txt");
    if(!file) {
        displayError("Could not open records file for saving.");
        return;
    }
    for(const auto&student:students) {
        file<<student.id<<" "<<student.name<<" "<<student.grade<<" "
            <<student.address<<" "<<student.dob<<" "<<student.contact<<endl;
    }
    file.close();
}

void addStudent() {
    int recordCount;
    cout<<"\nHow many students do you want to add? ";
    cin>>recordCount;

    for(int i=0;i<recordCount;i++) {
        Student student;
        cout<<"\nEnter details for student "<<i+1<<":\n";
        cout<<"Enter ID (unique): ";
        do {
            cin>>student.id;
            if(!isUniqueID(student.id)) displayError("ID already exists! Enter a unique ID.");
        } while(!isUniqueID(student.id));//loop will terminate when id will be unique

        cout<<"Enter Name: ";
        cin>>ws;
        getline(cin,student.name);

        cout<<"Enter Class/Grade: ";
        cin>>student.grade;

         cout<<"Enter Address: ";
        cin>>ws;//ignore leading whitespace character
        getline(cin,student.address);

        cout<<"Enter Date of Birth (DD/MM/YYYY): ";
        do {
            cin>>student.dob;
            if(!validateDOB(student.dob)) displayError("Invalid DOB format! Use DD/MM/YYYY.");
        } while(!validateDOB(student.dob));//loop terminate when condition true

        cout<<"Enter Parent/Guardian Contact: ";
        do {
            cin>>student.contact;
            if(!validateContact(student.contact)) displayError("Invalid contact number! Use digits only (10-12 digits).");
        } while(!validateContact(student.contact));

        students.push_back(student);
        displaySuccess("Student added successfully!");
    }
    saveToFile();
}

bool validateContact(const string&contact) {
    return all_of(contact.begin(),contact.end(),::isdigit)&&contact.length()>=10&&contact.length()<=12;
}

bool validateDOB(const string&dob) {
    if(dob.length()!=10||dob[2]!='/'||dob[5]!='/') return false;
    string day=dob.substr(0,2),month=dob.substr(3,2),year=dob.substr(6,4);
    return all_of(day.begin(),day.end(),::isdigit)&&all_of(month.begin(),month.end(),::isdigit)&&
           all_of(year.begin(),year.end(),::isdigit);
}

bool isUniqueID(const string&id) {
    for(const auto&student:students) {
        if(student.id==id) return false;
    }
    return true;
}

void searchRecord() {
    string searchID;
    cout<<"\nEnter the ID of the student to search: ";
    cin>>searchID;

    for(const auto&student:students) {
        if(student.id==searchID) {
            cout<<"\nStudent Details:\n";
            cout<<"ID: "<<student.id<<"\nName: "<<student.name<<"\nClass: "<<student.grade
                <<"\nAddress: "<<student.address<<"\nDOB: "<<student.dob
                <<"\nContact: "<<student.contact<<endl;
            return;
        }
    }
    displayError("No student found with the given ID.");
}

void updateRecord() {
    string updateID;
    cout<<"\nEnter the ID of the student to update: ";
    cin>>updateID;

    for(auto&student:students) {
        if(student.id==updateID) {
            cout<<"\nCurrent details of the student:\n";
            cout<<"ID: "<<student.id<<"\nName: "<<student.name<<"\nClass: "<<student.grade
                <<"\nAddress: "<<student.address<<"\nDOB: "<<student.dob
                <<"\nContact: "<<student.contact<<endl;

            cout<<"\nEnter new details (leave blank to keep current value):\n";

            cout<<"Name ("<<student.name<<"): ";
            cin>>ws;
            string input;
            getline(cin,input);
            if(!input.empty()) student.name=input;

            cout<<"Class/Grade ("<<student.grade<<"): ";
            getline(cin,input);
            if(!input.empty()) student.grade=input;

            cout<<"Address ("<<student.address<<"): ";
            getline(cin,input);
            if(!input.empty()) student.address=input;

            cout<<"Date of Birth ("<<student.dob<<"): ";
            do {
                getline(cin,input);
                if(input.empty()||validateDOB(input)) break;
                displayError("Invalid DOB format! Use DD/MM/YYYY.");
            } while(true);
            if(!input.empty()) student.dob=input;

            cout<<"Contact ("<<student.contact<<"): ";
            do {
                getline(cin,input);
                if(input.empty()||validateContact(input)) break;
                displayError("Invalid contact number! Use digits only (10-12 digits).");
            } while(true);
            if(!input.empty()) student.contact=input;

            displaySuccess("Student record updated successfully!");
            saveToFile();
            return;
        }
    }
    displayError("No student found with the given ID.");

}
void deleteRecord() {
    string deleteID;
    cout<<"\nEnter the ID of the student to delete: ";
    cin>>deleteID;

    auto it=remove_if(students.begin(),students.end(),[&deleteID](const Student&student) {
        return student.id==deleteID;
    });

    if(it!=students.end()) {
        students.erase(it,students.end());
        displaySuccess("Student record deleted successfully!");
        saveToFile();
    } else {
        displayError("No student found with the given ID.");
    }
}

void showRecords() {
    if(students.empty()) {
        displayError("No records found.");
        return;
    }
    cout<<"\nAll Student Records:\n";
    for(const auto&student:students) {
        cout<<"ID: "<<student.id<<", Name: "<<student.name<<", Class: "<<student.grade
            <<", Address: "<<student.address<<", DOB: "<<student.dob
            <<", Contact: "<<student.contact<<endl;
    }
}

void sortRecords() {
    sort(students.begin(),students.end(),[](const Student&a,const Student&b) {
        return a.name<b.name;
    });
    displaySuccess("Records sorted by name.");
    saveToFile();
}

void setColor(int color) {
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,color);
}

void displayMenu() {
    setColor(15| BACKGROUND_RED);
    cout<<"\nStudent Management System\n";
    setColor(10);
    cout<<"1. Add Student\n";
    setColor(14);
    cout<<"2. Search Student\n";
    setColor(9);
    cout<<"3. Update Student\n";
    setColor(12);
    cout<<"4. Delete Student\n";
    setColor(11);
    cout<<"5. Show All Records\n";
    setColor(13);
    cout<<"6. Sort Records\n";
    setColor(14);
    cout<<"7. Exit\n";
    setColor(15);
}

void clearScreen() {
    system("cls");
}

void displayError(const string&message) {
    setColor(12);
    cerr<<"Error: "<<message<<endl;
    setColor(15);
}

void displaySuccess(const string&message) {
    setColor(10);
    cout<<message<<endl;
    setColor(5);
}

void confirmExit() {
    char choice;
    cout<<"Are you sure you want to exit? (Y/N): ";
    cin>>choice;
    if(tolower(choice)=='y') {
        exit(0);
    }
}

int main() {
    loadFromFile();
    while(true) {
        clearScreen();
        displayMenu();
        int choice=getValidatedChoice();

        switch(choice) {
            case 1: addStudent(); break;
            case 2: searchRecord(); break;
            case 3: updateRecord(); break;
            case 4: deleteRecord(); break;
            case 5: showRecords(); break;
            case 6: sortRecords(); break;
            case 7: confirmExit(); break;
            default: displayError("Invalid choice! Please try again.");
        }

        cout<<"\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin.get();
    }
    return 0;
}

int getValidatedChoice() {
    int choice;
    while(true) {
        cout<<"\nEnter your choice: ";
        cin>>choice;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            displayError("Invalid input! Please enter a number between 1 and 7.");
        } else if(choice<1||choice>7) {
            displayError("Invalid choice! Please enter a number between 1 and 7.");
        } else {
            return choice;
        }
    }
}
