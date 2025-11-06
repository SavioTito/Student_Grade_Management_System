#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// Base Student Class
class Student
{
protected:
    string studentId;
    string name;
    int age;
    string course;

public:
    Student(string id, string studentName, int studentAge, string studentCourse)
        : studentId(id), name(studentName), age(studentAge), course(studentCourse) {}

    virtual void displayInfo()
    {
        cout << "ID: " << studentId << " | Name: " << name
             << " | Age: " << age << " | Course: " << course << endl;
    }

    virtual double calculateGrade() = 0; // Pure virtual function
    virtual string getGradeStatus() = 0;

    // Getters
    string getStudentId() { return studentId; }
    string getName() { return name; }
    string getCourse() { return course; }

    virtual ~Student() {} // Virtual destructor
};

// Undergraduate Student Class
class UndergraduateStudent : public Student
{
private:
    vector<double> assignments;
    vector<double> exams;
    double projectGrade;

public:
    UndergraduateStudent(string id, string name, int age, string course)
        : Student(id, name, age, course), projectGrade(0) {}

    void addAssignmentGrade(double grade)
    {
        assignments.push_back(grade);
    }

    void addExamGrade(double grade)
    {
        exams.push_back(grade);
    }

    void setProjectGrade(double grade)
    {
        projectGrade = grade;
    }

    double calculateGrade() override
    {
        if (assignments.empty() && exams.empty())
            return 0;

        double assignmentAvg = 0;
        for (double grade : assignments)
            assignmentAvg += grade;
        if (!assignments.empty())
            assignmentAvg /= assignments.size();

        double examAvg = 0;
        for (double grade : exams)
            examAvg += grade;
        if (!exams.empty())
            examAvg /= exams.size();

        // Weighted average: 40% assignments, 40% exams, 20% project
        return (assignmentAvg * 0.4) + (examAvg * 0.4) + (projectGrade * 0.2);
    }

    string getGradeStatus() override
    {
        double finalGrade = calculateGrade();
        if (finalGrade >= 70)
            return "First Class";
        else if (finalGrade >= 60)
            return "Second Class Upper";
        else if (finalGrade >= 50)
            return "Second Class Lower";
        else if (finalGrade >= 40)
            return "Pass";
        else
            return "Fail";
    }

    void displayInfo() override
    {
        Student::displayInfo();
        cout << "   Type: Undergraduate | Final Grade: " << calculateGrade()
             << " | Status: " << getGradeStatus() << endl;
    }
};

// Graduate Student Class
class GraduateStudent : public Student
{
private:
    vector<double> researchPapers;
    double thesisGrade;
    double comprehensiveExam;

public:
    GraduateStudent(string id, string name, int age, string course)
        : Student(id, name, age, course), thesisGrade(0), comprehensiveExam(0) {}

    void addResearchPaper(double grade)
    {
        researchPapers.push_back(grade);
    }

    void setThesisGrade(double grade)
    {
        thesisGrade = grade;
    }

    void setComprehensiveExam(double grade)
    {
        comprehensiveExam = grade;
    }

    double calculateGrade() override
    {
        if (researchPapers.empty())
            return 0;

        double researchAvg = 0;
        for (double grade : researchPapers)
            researchAvg += grade;
        researchAvg /= researchPapers.size();

        // Weighted average: 50% research, 30% thesis, 20% comprehensive exam
        return (researchAvg * 0.5) + (thesisGrade * 0.3) + (comprehensiveExam * 0.2);
    }

    string getGradeStatus() override
    {
        double finalGrade = calculateGrade();
        if (finalGrade >= 80)
            return "Distinction";
        else if (finalGrade >= 70)
            return "Merit";
        else if (finalGrade >= 60)
            return "Pass";
        else
            return "Fail";
    }

    void displayInfo() override
    {
        Student::displayInfo();
        cout << "   Type: Graduate | Final Grade: " << calculateGrade()
             << " | Status: " << getGradeStatus() << endl;
    }
};

// Grade Management System Class
class GradeManagementSystem
{
private:
    vector<Student *> students;

public:
    ~GradeManagementSystem()
    {
        for (auto student : students)
            delete student;
    }

    void addStudent()
    {
        string id, name, course;
        int age, type;

        cout << "\n=== Add New Student ===" << endl;
        cout << "1. Undergraduate Student" << endl;
        cout << "2. Graduate Student" << endl;
        cout << "Choose student type: ";
        cin >> type;

        cin.ignore();
        cout << "Enter Student ID: ";
        getline(cin, id);
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cin.ignore();
        cout << "Enter Course: ";
        getline(cin, course);

        Student *newStudent = nullptr;

        if (type == 1)
        {
            newStudent = new UndergraduateStudent(id, name, age, course);
        }
        else if (type == 2)
        {
            newStudent = new GraduateStudent(id, name, age, course);
        }
        else
        {
            cout << "Invalid student type!" << endl;
            return;
        }

        students.push_back(newStudent);
        cout << "✅ Student added successfully!" << endl;
    }

    void addGrades()
    {
        if (students.empty())
        {
            cout << "🚫 No students found!" << endl;
            return;
        }

        displayAllStudents();
        string studentId;

        cout << "Enter Student ID to add grades: ";
        cin.ignore();
        getline(cin, studentId);

        Student *student = findStudent(studentId);
        if (!student)
        {
            cout << "🚫 Student not found!" << endl;
            return;
        }

        // Dynamic cast to check student type
        if (UndergraduateStudent *ug = dynamic_cast<UndergraduateStudent *>(student))
        {
            addUndergraduateGrades(ug);
        }
        else if (GraduateStudent *grad = dynamic_cast<GraduateStudent *>(student))
        {
            addGraduateGrades(grad);
        }
    }

    void addUndergraduateGrades(UndergraduateStudent *student)
    {
        int choice;
        double grade;

        do
        {
            cout << "\n=== Add Undergraduate Grades ===" << endl;
            cout << "1. Add Assignment Grade" << endl;
            cout << "2. Add Exam Grade" << endl;
            cout << "3. Set Project Grade" << endl;
            cout << "4. Done" << endl;
            cout << "Choose option: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                cout << "Enter assignment grade: ";
                cin >> grade;
                student->addAssignmentGrade(grade);
                cout << "✅ Assignment grade added!" << endl;
                break;
            case 2:
                cout << "Enter exam grade: ";
                cin >> grade;
                student->addExamGrade(grade);
                cout << "✅ Exam grade added!" << endl;
                break;
            case 3:
                cout << "Enter project grade: ";
                cin >> grade;
                student->setProjectGrade(grade);
                cout << "✅ Project grade set!" << endl;
                break;
            }
        } while (choice != 4);
    }

    void addGraduateGrades(GraduateStudent *student)
    {
        int choice;
        double grade;

        do
        {
            cout << "\n=== Add Graduate Grades ===" << endl;
            cout << "1. Add Research Paper Grade" << endl;
            cout << "2. Set Thesis Grade" << endl;
            cout << "3. Set Comprehensive Exam Grade" << endl;
            cout << "4. Done" << endl;
            cout << "Choose option: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                cout << "Enter research paper grade: ";
                cin >> grade;
                student->addResearchPaper(grade);
                cout << "✅ Research paper grade added!" << endl;
                break;
            case 2:
                cout << "Enter thesis grade: ";
                cin >> grade;
                student->setThesisGrade(grade);
                cout << "✅ Thesis grade set!" << endl;
                break;
            case 3:
                cout << "Enter comprehensive exam grade: ";
                cin >> grade;
                student->setComprehensiveExam(grade);
                cout << "✅ Comprehensive exam grade set!" << endl;
                break;
            }
        } while (choice != 4);
    }

    void displayAllStudents()
    {
        if (students.empty())
        {
            cout << "🚫 No students found!" << endl;
            return;
        }

        cout << "\n=== All Students ===" << endl;
        for (auto student : students)
        {
            student->displayInfo();
            cout << "------------------------" << endl;
        }
    }

    void displayStudentDetails()
    {
        if (students.empty())
        {
            cout << "🚫 No students found!" << endl;
            return;
        }

        string studentId;
        cout << "Enter Student ID: ";
        cin.ignore();
        getline(cin, studentId);

        Student *student = findStudent(studentId);
        if (student)
        {
            cout << "\n=== Student Details ===" << endl;
            student->displayInfo();
        }
        else
        {
            cout << "🚫 Student not found!" << endl;
        }
    }

    void showClassStatistics()
    {
        if (students.empty())
        {
            cout << "🚫 No students found!" << endl;
            return;
        }

        double totalGrade = 0;
        double highestGrade = 0;
        double lowestGrade = 100;
        int passCount = 0;

        for (auto student : students)
        {
            double grade = student->calculateGrade();
            totalGrade += grade;
            if (grade > highestGrade)
                highestGrade = grade;
            if (grade < lowestGrade)
                lowestGrade = grade;
            if (grade >= 40)
                passCount++; // Assuming 40 is passing
        }

        cout << "\n=== Class Statistics ===" << endl;
        cout << "Total Students: " << students.size() << endl;
        cout << "Average Grade: " << (totalGrade / students.size()) << endl;
        cout << "Highest Grade: " << highestGrade << endl;
        cout << "Lowest Grade: " << lowestGrade << endl;
        cout << "Passing Students: " << passCount << endl;
        cout << "Failure Rate: " << ((students.size() - passCount) * 100.0 / students.size()) << "%" << endl;
    }

private:
    Student *findStudent(string studentId)
    {
        for (auto student : students)
        {
            if (student->getStudentId() == studentId)
            {
                return student;
            }
        }
        return nullptr;
    }

public:
    void showMenu()
    {
        cout << "\n"
             << string(50, '=') << endl;
        cout << "    🎓 STUDENT GRADE MANAGEMENT SYSTEM" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Add New Student" << endl;
        cout << "2. Add Grades" << endl;
        cout << "3. Display All Students" << endl;
        cout << "4. Display Student Details" << endl;
        cout << "5. Show Class Statistics" << endl;
        cout << "6. Exit" << endl;
        cout << string(50, '=') << endl;
        cout << "Choose option (1-6): ";
    }
};

// Main function
int main()
{
    GradeManagementSystem system;
    int choice;

    cout << "🎓 Student Grade Management System Started!" << endl;
    cout << "Perfect for OOP Presentation - Single File Implementation" << endl;

    do
    {
        system.showMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            system.addStudent();
            break;
        case 2:
            system.addGrades();
            break;
        case 3:
            system.displayAllStudents();
            break;
        case 4:
            system.displayStudentDetails();
            break;
        case 5:
            system.showClassStatistics();
            break;
        case 6:
            cout << "Thank you for using Student Grade Management System!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 6);

    return 0;
}