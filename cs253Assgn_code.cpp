/**************************************************************************
*
*-------------------- LIBRARY MANAGEMENT SYSTEM --------------------------
*
*    This program implements a Library Management System in C++ using OOP
*    concepts (abstraction, inheritance, polymorphism, encapsulation).
*    It manages books and users (Students, Faculty, and Librarians) and
*    supports operations such as borrowing, returning, reserving books,
*    fine management, and administrative functions for the librarian.
*
*    Students: Can borrow up to 3 books for 15 days max. Overdue books
*              incur a fine of 10 rupees per extra day. The overdue fine is
*              shown in account details until paid; when paying the fine,
*              the overdue count resets to zero.
*
*    Faculty:  Can borrow up to 5 books for 30 days max. They incur no fine,
*              but if any borrowed book is overdue by more than 60 days, further
*              borrowing is blocked.
*
*    Librarians: Can manage the library (add, remove, update books and users)
*                but cannot borrow or reserve books.
*
*    Note: There are three default librarians with the following credentials (username/password):
*          librarian1 / admin1, librarian2 / admin2, and librarian3 / admin3.
*
*    Each Book record (representing one copy) displays:
*              Book ID, Title, Publisher, Year, ISBN, and a computed Status.
*              Computed Status (determined at view time) is:
*              "Available" if not borrowed;
*              "Borrowed" if borrowed by the current user;
*              "Reserved" if reserved by someone else and
*              (shows "Reserved (For You)" if reserved by the current user).
*
*    Default data consists of 10 titles with 5 copies each (50 records total).
*    Each copy is assigned a unique Book ID.
*
*    Data is persisted immediately to files: books.txt, users.txt, and
*    transactions.txt.
*
*    Compile with: g++ -std=c++11 cs253Assgn.cpp -o cs253Assgn
*    Run with:     ./cs253Assgn
*
**************************************************************************/

// ========== Standard Library Includes ==========

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cctype>

using namespace std;

// ========== Utility Functions ==========

// Function: trim()
// Removes leading and trailing whitespace from a string.
string trim(const string & s)
{
    auto start = s.begin();
    while (start != s.end() && isspace(*start))
    {
        start++;
    }

    auto end = s.end();
    if (start != s.end())
    {
        do
        {
            end--;
        }
        while (distance(start, end) > 0 && isspace(*end));
    }
    return string(start, end + 1);
}


// Function: getTimeString()
// Converts a time_t value to a human-readable string using ctime().
// Also removes the trailing newline character.
string getTimeString(time_t t)
{
    char * dt = ctime(&t);
    string s(dt);
    if (!s.empty() && s.back() == '\n')
    {
        s.pop_back();
    }
    return s;
}


// ========== Enumeration and Conversion Functions ==========

// Enumeration: BookStatus
enum class BookStatus
{
    Available,
    Borrowed,
    Reserved
};


// Function: statusToString()
// Converts a BookStatus enum value to its string representation.
string statusToString(BookStatus status)
{
    switch (status)
    {
        case BookStatus::Available:
        {
            return "Available";
        }
        case BookStatus::Borrowed:
        {
            return "Borrowed";
        }
        case BookStatus::Reserved:
        {
            return "Reserved";
        }
    }
    return "Unknown";
}


// Function: stringToStatus()
// Converts a string to a BookStatus enum value.
BookStatus stringToStatus(const string & str)
{
    if (str == "Available")
    {
        return BookStatus::Available;
    }
    if (str == "Borrowed")
    {
        return BookStatus::Borrowed;
    }
    if (str == "Reserved")
    {
        return BookStatus::Reserved;
    }
    return BookStatus::Available;
}


// ========== Class Definitions ==========

// Class: Book
// Represents one copy of a book in the library.
// (Only computed status is shown when viewing details.)
class Book
{
private:
    int id;
    string title;
    string author;      // Not displayed.
    string publisher;
    int year;
    string ISBN;
    BookStatus status;
    int borrowedBy;     // 0 if not borrowed.
    int reservedBy;     // 0 if not reserved.
    
public:
    // Default constructor.
    Book()
    : id(0)
    , title("")
    , author("")
    , publisher("")
    , year(0)
    , ISBN("")
    , status(BookStatus::Available)
    , borrowedBy(0)
    , reservedBy(0)
    {
    }
    
    
    // Parameterized constructor.
    Book(int id, const string & title, const string & author, const string & publisher, int year, const string & ISBN, BookStatus status = BookStatus::Available)
    : id(id)
    , title(title)
    , author(author)
    , publisher(publisher)
    , year(year)
    , ISBN(ISBN)
    , status(status)
    , borrowedBy(0)
    , reservedBy(0)
    {
    }
    
    
    // Getter methods.
    int getId() const
    {
        return id;
    }
    
    
    string getTitle() const
    {
        return title;
    }
    
    
    string getPublisher() const
    {
        return publisher;
    }
    
    
    int getYear() const
    {
        return year;
    }
    
    
    string getISBN() const
    {
        return ISBN;
    }
    
    
    BookStatus getStatus() const
    {
        return status;
    }
    
    
    int getBorrowedBy() const
    {
        return borrowedBy;
    }
    
    
    int getReservedBy() const
    {
        return reservedBy;
    }
    
    
    // Update methods.
    void updateTitle(const string & newTitle)
    {
        title = newTitle;
    }
    
    
    void updateAuthor(const string & newAuthor)
    {
        author = newAuthor;
    }
    
    
    void updatePublisher(const string & newPublisher)
    {
        publisher = newPublisher;
    }
    
    
    void updateYear(int newYear)
    {
        year = newYear;
    }
    
    
    void updateISBN(const string & newISBN)
    {
        ISBN = newISBN;
    }
    
    
    void updateStatus(BookStatus newStatus)
    {
        status = newStatus;
    }
    
    
    void updateBorrowedBy(int userId)
    {
        borrowedBy = userId;
    }
    
    
    void updateReservedBy(int userId)
    {
        reservedBy = userId;
    }
    
    
    // Prints book details (without stored status).
    void printDetails() const
    {
        cout << "---------------------------------------" << endl;
        cout << "Book ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "Year: " << year << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "---------------------------------------" << endl;
    }
    
    
    // Serializes book data.
    string serialize() const
    {
        ostringstream oss;
        oss << id << ";" << title << ";" << author << ";" << publisher << ";" 
            << year << ";" << ISBN << ";" << statusToString(status) << ";" 
            << borrowedBy << ";" << reservedBy;
        return oss.str();
    }
    
    
    // Deserializes book data.
    void deserialize(const string & data)
    {
        try
        {
            istringstream iss(data);
            string token;
            getline(iss, token, ';');
            id = stoi(token);
            getline(iss, title, ';');
            getline(iss, author, ';');
            getline(iss, publisher, ';');
            getline(iss, token, ';');
            year = stoi(token);
            getline(iss, ISBN, ';');
            getline(iss, token, ';');
            status = stringToStatus(token);
            getline(iss, token, ';');
            borrowedBy = stoi(token);
            getline(iss, token, ';');
            reservedBy = stoi(token);
        }
        catch (exception & e)
        {
            id = 0;
            title = "";
            author = "";
            publisher = "";
            year = 0;
            ISBN = "";
            status = BookStatus::Available;
            borrowedBy = 0;
            reservedBy = 0;
        }
    }
};


// Class: BorrowRecord
// Stores a borrow record.
struct BorrowRecord
{
    int bookId;
    time_t borrowTimestamp;
    int borrowDays;
};

    
// Class: Account
// Manages borrow records and fines.
class Account
{
private:
    vector<BorrowRecord> borrowRecords;
    double fineDue;
    
public:
    // Default constructor.
    Account()
    : fineDue(0.0)
    {
    }
    
    
    // Returns borrow records.
    vector<BorrowRecord> getBorrowRecords() const
    {
        return borrowRecords;
    }
    
    
    // Returns current fine.
    double getFine() const
    {
        return fineDue;
    }
    
    
    // Adds a new borrow record.
    void addBorrowedBook(int bookId, int borrowDays)
    {
        BorrowRecord record;
        record.bookId = bookId;
        record.borrowTimestamp = time(0);
        record.borrowDays = borrowDays;
        borrowRecords.push_back(record);
    }
    
    
    // Removes a borrow record.
    void removeBorrowedBook(int bookId)
    {
        auto it = remove_if(borrowRecords.begin(), borrowRecords.end(),
            [bookId](const BorrowRecord & r)
            {
                return r.bookId == bookId;
            }
        );
        if (it != borrowRecords.end())
        {
            borrowRecords.erase(it, borrowRecords.end());
        }
    }
    
    
    // Adds a fine.
    void addFine(double fine)
    {
        fineDue += fine;
    }
    
    
    // Resets the fine amount.
    void resetFine()
    {
        fineDue = 0;
    }
    
    
    // Resets borrow timestamps to current time.
    void resetBorrowTimestamps()
    {
        time_t now = time(0);
        for (auto & record : borrowRecords)
        {
            record.borrowTimestamp = now;
        }
    }
    
    
    // Prints account details.
    void printAccountDetails() const
    {
        cout << "Borrowed Books:" << endl;
        time_t now = time(0);
        bool nonOverdueFound = false;
        for (const auto & record : borrowRecords)
        {
            int daysElapsed = static_cast<int>(difftime(now, record.borrowTimestamp) / 86400);
            if (daysElapsed <= record.borrowDays)
            {
                cout << "Book ID: " << record.bookId
                     << ", Borrow Date: " << getTimeString(record.borrowTimestamp)
                     << ", Intended Borrow Days: " << record.borrowDays
                     << ", Days Elapsed: " << daysElapsed << endl;
                nonOverdueFound = true;
            }
        }
        if (!nonOverdueFound)
        {
            cout << "No currently borrowed (non-overdue) books." << endl;
        }
        cout << "\nOverdue Books:" << endl;
        bool overdueFound = false;
        for (const auto & record : borrowRecords)
        {
            int daysElapsed = static_cast<int>(difftime(now, record.borrowTimestamp) / 86400);
            if (daysElapsed > record.borrowDays)
            {
                cout << "Book ID: " << record.bookId
                     << ", Borrow Date: " << getTimeString(record.borrowTimestamp)
                     << ", Intended Borrow Days: " << record.borrowDays
                     << ", Days Elapsed: " << daysElapsed << endl;
                overdueFound = true;
            }
        }
        if (!overdueFound)
        {
            cout << "No overdue books." << endl;
        }
        cout << "Fine Due: " << fineDue << " rupees" << endl;
    }
    
    
    // Serializes account data.
    string serialize() const
    {
        ostringstream oss;
        oss << fineDue;
        for (const auto & record : borrowRecords)
        {
            oss << ";" << record.bookId << "," << record.borrowTimestamp << "," << record.borrowDays;
        }
        return oss.str();
    }
    
    
    // Deserializes account data.
    void deserialize(const string & data)
    {
        borrowRecords.clear();
        try
        {
            istringstream iss(data);
            string token;
            getline(iss, token, ';');
            fineDue = stod(token);
            while (getline(iss, token, ';'))
            {
                istringstream recordStream(token);
                BorrowRecord record;
                string part;
                getline(recordStream, part, ',');
                record.bookId = stoi(part);
                getline(recordStream, part, ',');
                record.borrowTimestamp = static_cast<time_t>(stoll(part));
                getline(recordStream, part, ',');
                record.borrowDays = stoi(part);
                borrowRecords.push_back(record);
            }
        }
        catch (exception & e)
        {
            borrowRecords.clear();
            fineDue = 0;
        }
    }
};


// ========== Forward Declarations ==========

class Library; // Forward declaration of Library for use in User display.

// ========== Class: User ==========
class User
{
protected:
    int userId;
    string username;
    string password;
    Account account;
    
public:
    // Default constructor.
    User()
    : userId(0)
    , username("")
    , password("")
    {
    }
    
    
    // Parameterized constructor.
    User(int id, const string & uname, const string & pwd)
    : userId(id)
    , username(uname)
    , password(pwd)
    {
    }
    
    
    virtual ~User()
    {
    }
    
    
    int getUserId() const
    {
        return userId;
    }
    
    
    string getUsername() const
    {
        return username;
    }
    
    
    bool checkPassword(const string & pwd) const
    {
        return password == pwd;
    }
    
    
    Account & getAccount()
    {
        return account;
    }
    
    
    // Declaration: Display user details, account info, and reserved books.
    virtual void display(const Library & lib) const;
    
    
    // Pure virtual functions for borrowing and returning books.
    virtual void borrowBook(Library & lib) = 0;
    virtual void returnBook(Library & lib) = 0;
    
    
    // Serializes user data.
    virtual string serialize() const
    {
        ostringstream oss;
        oss << userId << ";" << username << ";" << password << ";" << account.serialize();
        return oss.str();
    }
    
    
    // Deserializes user data.
    virtual void deserialize(const string & data)
    {
        istringstream iss(data);
        string token;
        getline(iss, token, ';');
        userId = stoi(token);
        getline(iss, username, ';');
        username = trim(username);
        getline(iss, password, ';');
        password = trim(password);
        string accData;
        getline(iss, accData);
        account.deserialize(accData);
    }
    
    
    // Setters.
    void setUsername(const string & uname)
    {
        username = uname;
    }
    
    
    void setPassword(const string & pwd)
    {
        password = pwd;
    }
};


// ========== Class: Student ==========
class Student : public User
{
private:
    int maxBooks;    // 3 books maximum.
    int maxDays;     // 15 days maximum.
    double fineRate; // 10 rupees per overdue day.
    
public:
    // Default constructor.
    Student()
    : User()
    , maxBooks(3)
    , maxDays(15)
    , fineRate(10.0)
    {
    }
    
    
    // Parameterized constructor.
    Student(int id, const string & uname, const string & pwd)
    : User(id, uname, pwd)
    , maxBooks(3)
    , maxDays(15)
    , fineRate(10.0)
    {
    }
    
    
    virtual void borrowBook(Library & lib) override;
    virtual void returnBook(Library & lib) override;
    
    // Function to reserve a book.
    void reserveBook(Library & lib);
    
    
    // Declaration: Display student details along with computed fine and reserved books.
    virtual void display(const Library & lib) const override;
};


// ========== Class: Faculty ==========
class Faculty : public User
{
private:
    int maxBooks;    // 5 books maximum.
    int maxDays;     // 30 days maximum.
    
public:
    // Default constructor.
    Faculty()
    : User()
    , maxBooks(5)
    , maxDays(30)
    {
    }
    
    
    // Parameterized constructor.
    Faculty(int id, const string & uname, const string & pwd)
    : User(id, uname, pwd)
    , maxBooks(5)
    , maxDays(30)
    {
    }
    
    
    virtual void borrowBook(Library & lib) override;
    virtual void returnBook(Library & lib) override;
    void reserveBook(Library & lib);
    
    // Declaration: Display faculty details.
    virtual void display(const Library & lib) const override;
};


// ========== Class: Librarian ==========
class Librarian : public User
{
public:
    // Default constructor.
    Librarian()
    : User()
    {
    }
    
    
    // Parameterized constructor.
    Librarian(int id, const string & uname, const string & pwd)
    : User(id, uname, pwd)
    {
    }
    
    
    virtual void borrowBook(Library & lib) override
    {
        cout << "Librarian cannot borrow books." << endl;
    }
    
    
    virtual void returnBook(Library & lib) override
    {
        cout << "Librarian does not return books." << endl;
    }
    
    
    // Declaration: Display librarian details.
    virtual void display(const Library & lib) const override;
    
    
    // Administrative functions.
    void addBook(Library & lib);
    void removeBook(Library & lib);
    void updateBook(Library & lib);
    void addUser(Library & lib);
    void removeUser(Library & lib);
    void updateUser(Library & lib);
};


// ========== Forward Declarations for Portal Menus ==========
void userPortalMenu(User * user, Library & lib);
void librarianPortalMenu(Librarian * libUser, Library & lib);


// ========== Class: Library ==========
class Library
{
private:
    vector<Book> books;         // Collection of books.
    vector<User*> users;        // Collection of users.
    vector<string> transactionLog;
    const string booksFile = "books.txt";
    const string usersFile = "users.txt";
    const string logFile = "transactions.txt";
    
    
    // Loads default book data.
    void loadDefaultBooks()
    {
        books.clear();
        
        vector<tuple<string, string, string, int, string>> defaultTitles = {
            make_tuple("The C++ Programming Language", "Bjarne Stroustrup", "Addison-Wesley", 2013, "9780321563842"),
            make_tuple("Effective C++", "Scott Meyers", "O'Reilly", 2005, "9780321334879"),
            make_tuple("Clean Code", "Robert C. Martin", "Prentice Hall", 2008, "9780132350884"),
            make_tuple("Design Patterns", "Erich Gamma et al.", "Addison-Wesley", 1994, "9780201633610"),
            make_tuple("Modern Operating Systems", "Andrew Tanenbaum", "Pearson", 2014, "9780133591620"),
            make_tuple("Introduction to Algorithms", "Cormen et al.", "MIT Press", 2009, "9780262033848"),
            make_tuple("Artificial Intelligence: A Modern Approach", "Stuart Russell", "Pearson", 2009, "9780136042594"),
            make_tuple("The Pragmatic Programmer", "Andrew Hunt", "Addison-Wesley", 1999, "9780201616224"),
            make_tuple("Code Complete", "Steve McConnell", "Microsoft Press", 2004, "9780735619678"),
            make_tuple("Refactoring", "Martin Fowler", "Addison-Wesley", 1999, "9780201485677")
        };
        
        int newId = 1;
        for (auto & tpl : defaultTitles)
        {
            for (int i = 0; i < 5; i++)
            {
                Book book(newId, get<0>(tpl), get<1>(tpl), get<2>(tpl), get<3>(tpl), get<4>(tpl), BookStatus::Available);
                books.push_back(book);
                newId++;
            }
        }
    }
    
    
public:
    // Constructor: Loads books, users, and transaction log.
    Library()
    {
        loadBooks();
        loadUsers();
        loadTransactionLog();
    }
    
    
    // Destructor: Saves data and cleans up.
    ~Library()
    {
        saveBooks();
        saveUsers();
        saveTransactionLog();
        for (auto user : users)
        {
            delete user;
        }
    }
    
    
    // Loads books from file or defaults.
    void loadBooks()
    {
        books.clear();
        ifstream fin(booksFile);
        if (!fin || fin.peek() == ifstream::traits_type::eof())
        {
            cout << "Books file not found or empty. Loading default books." << endl;
            loadDefaultBooks();
            return;
        }
        string line;
        vector<Book> tempBooks;
        while (getline(fin, line))
        {
            if (line.empty())
            {
                continue;
            }
            Book book;
            book.deserialize(line);
            tempBooks.push_back(book);
        }
        fin.close();
        if (tempBooks.empty() || tempBooks[0].getTitle().empty())
        {
            cout << "Books file data invalid. Loading default books." << endl;
            loadDefaultBooks();
        }
        else
        {
            books = tempBooks;
        }
    }
    
    
    // Saves books to file.
    void saveBooks()
    {
        ofstream fout(booksFile);
        for (auto & book : books)
        {
            fout << book.serialize() << "\n";
        }
        fout.close();
    }
    
    
    // Returns all reserved books for a given user.
    vector<Book> getReservedBooksByUser(int userId) const
    {
        vector<Book> reservedBooks;
        for (const auto & book : books)
        {
            if (book.getReservedBy() == userId)
            {
                reservedBooks.push_back(book);
            }
        }
        return reservedBooks;
    }
    
    
    // Prints book list with computed status.
    void printBooksForUser(int currentUserId) const
    {
        cout << "\n********** Books List **********\n";
        for (const auto & book : books)
        {
            cout << "---------------------------------------" << endl;
            cout << "Book ID: " << book.getId() << endl;
            cout << "Title: " << book.getTitle() << endl;
            cout << "Publisher: " << book.getPublisher() << endl;
            cout << "Year: " << book.getYear() << endl;
            cout << "ISBN: " << book.getISBN() << endl;
            cout << "Status: ";
            if (book.getBorrowedBy() == 0)
            {
                cout << "Available";
            }
            else if (book.getBorrowedBy() == currentUserId)
            {
                cout << "Borrowed";
            }
            else if (book.getReservedBy() != 0)
            {
                if (book.getReservedBy() == currentUserId)
                {
                    cout << "Reserved (For You)";
                }
                else
                {
                    cout << "Reserved";
                }
            }
            else
            {
                cout << "Borrowed";
            }
            cout << "\n---------------------------------------\n";
        }
    }
    
    
    // Prints books currently borrowed by a user.
    void printBorrowedBooksByUser(User * user) const
    {
        cout << "\n********** Your Borrowed Books **********\n";
        vector<BorrowRecord> records = user->getAccount().getBorrowRecords();
        bool found = false;
        for (const auto & record : records)
        {
            for (const auto & book : books)
            {
                if (book.getId() == record.bookId)
                {
                    book.printDetails();
                    cout << "Borrow Date: " << getTimeString(record.borrowTimestamp)
                         << ", Intended Borrow Days: " << record.borrowDays << endl;
                    found = true;
                }
            }
        }
        if (!found)
        {
            cout << "You have not borrowed any books." << endl;
        }
    }
    
    
    // Loads users from file or default data.
    void loadUsers()
    {
        for (auto user : users)
        {
            delete user;
        }
        users.clear();
        ifstream fin(usersFile);
        if (!fin || fin.peek() == ifstream::traits_type::eof())
        {
            cout << "Users file not found or empty. Loading default users." << endl;
            users.push_back(new Student(1, "alice", "pass1"));
            users.push_back(new Student(2, "bob", "pass2"));
            users.push_back(new Student(3, "charlie", "pass3"));
            users.push_back(new Student(4, "diana", "pass4"));
            users.push_back(new Student(5, "eric", "pass5"));
            users.push_back(new Faculty(6, "profX", "pass6"));
            users.push_back(new Faculty(7, "drY", "pass7"));
            users.push_back(new Faculty(8, "mrZ", "pass8"));
            // Modified default librarians: three librarians with specified credentials.
            users.push_back(new Librarian(9, "librarian1", "admin1"));
            users.push_back(new Librarian(10, "librarian2", "admin2"));
            users.push_back(new Librarian(11, "librarian3", "admin3"));
            return;
        }
        string line;
        while (getline(fin, line))
        {
            if (line.empty())
            {
                continue;
            }
            istringstream iss(line);
            string type;
            getline(iss, type, ';');
            User * user = nullptr;
            if (type == "Student")
            {
                user = new Student();
            }
            else if (type == "Faculty")
            {
                user = new Faculty();
            }
            else if (type == "Librarian")
            {
                user = new Librarian();
            }
            if (user)
            {
                string userData;
                getline(iss, userData);
                user->deserialize(userData);
                users.push_back(user);
            }
        }
        fin.close();
    }
    
    
    void saveUsers()
    {
        ofstream fout(usersFile);
        for (auto user : users)
        {
            string type;
            if (dynamic_cast<Student*>(user))
            {
                type = "Student";
            }
            else if (dynamic_cast<Faculty*>(user))
            {
                type = "Faculty";
            }
            else if (dynamic_cast<Librarian*>(user))
            {
                type = "Librarian";
            }
            fout << type << ";" << user->serialize() << "\n";
        }
        fout.close();
    }
    
    
    // Loads transaction log.
    void loadTransactionLog()
    {
        transactionLog.clear();
        ifstream fin(logFile);
        if (!fin)
        {
            cout << "Transaction log file not found. Starting new log." << endl;
            return;
        }
        string line;
        while (getline(fin, line))
        {
            if (!line.empty())
            {
                transactionLog.push_back(line);
            }
        }
        fin.close();
    }
    
    
    void saveTransactionLog()
    {
        ofstream fout(logFile);
        for (auto & entry : transactionLog)
        {
            fout << entry << "\n";
        }
        fout.close();
    }
    
    
    void logTransaction(const string & entry)
    {
        time_t now = time(0);
        string dt = getTimeString(now);
        ostringstream oss;
        oss << "[" << dt << "] " << entry;
        transactionLog.push_back(oss.str());
        saveTransactionLog();
    }
    
    
    void viewTransactionLog()
    {
        cout << "--------- Transaction Log ---------" << endl;
        for (auto & entry : transactionLog)
        {
            cout << entry << endl;
        }
        cout << "-------------------------------------" << endl;
    }
    
    
    void addBookToLibrary(const Book & book)
    {
        books.push_back(book);
        logTransaction("Book added: " + book.getTitle());
        saveBooks();
    }
    
    
    void removeBookFromLibrary(int bookId)
    {
        auto it = remove_if(books.begin(), books.end(),
            [bookId](const Book & b)
            {
                return b.getId() == bookId;
            }
        );
        if (it != books.end())
        {
            logTransaction("Book removed (ID): " + to_string(bookId));
            books.erase(it, books.end());
            saveBooks();
        }
        else
        {
            cout << "Book with ID " << bookId << " not found." << endl;
        }
    }
    
    
    Book* findBookByTitle(const string & title)
    {
        for (auto & book : books)
        {
            if (book.getTitle() == title)
            {
                return &book;
            }
        }
        return nullptr;
    }
    
    
    Book* findBookById(int id)
    {
        for (auto & book : books)
        {
            if (book.getId() == id)
            {
                return &book;
            }
        }
        return nullptr;
    }
    
    
    int generateBookId()
    {
        int maxId = 0;
        for (auto & book : books)
        {
            maxId = max(maxId, book.getId());
        }
        return maxId + 1;
    }
    
    
    void addUserToLibrary(User * user)
    {
        users.push_back(user);
        logTransaction("User added: " + user->getUsername());
        saveUsers();
    }
    
    
    void removeUserFromLibrary(int userId)
    {
        auto it = remove_if(users.begin(), users.end(),
            [userId](User * u)
            {
                return u->getUserId() == userId;
            }
        );
        if (it != users.end())
        {
            for (auto itr = it; itr != users.end(); ++itr)
            {
                logTransaction("User removed: " + (*itr)->getUsername());
                delete *itr;
            }
            users.erase(it, users.end());
            saveUsers();
        }
        else
        {
            cout << "User with ID " << userId << " not found." << endl;
        }
    }
    
    
    void updateUserInLibrary(int userId, const string & newUsername, const string & newPassword)
    {
        for (auto user : users)
        {
            if (user->getUserId() == userId)
            {
                if (!newUsername.empty())
                {
                    user->setUsername(newUsername);
                }
                if (!newPassword.empty())
                {
                    user->setPassword(newPassword);
                }
                logTransaction("User updated: " + user->getUsername());
                saveUsers();
                return;
            }
        }
        cout << "User with ID " << userId << " not found." << endl;
    }
    
    
    User* authenticateUser(const string & uname, const string & pwd)
    {
        string tUname = trim(uname);
        string tPwd = trim(pwd);
        for (auto user : users)
        {
            if (user->getUsername() == tUname && user->checkPassword(tPwd))
            {
                return user;
            }
        }
        return nullptr;
    }
    
    
    void printAllBooks() const
    {
        cout << "\n********** Library Books **********\n";
        for (const auto & book : books)
        {
            book.printDetails();
            cout << "Status: " << statusToString(book.getStatus()) << endl;
        }
    }
    
    
    void printAllUsers() const
    {
        cout << "\n********** Library Users **********\n";
        for (auto user : users)
        {
            user->display(*this);
        }
    }
    
    
    int generateUserId()
    {
        int maxId = 0;
        for (auto user : users)
        {
            maxId = max(maxId, user->getUserId());
        }
        return maxId + 1;
    }
    
    
    User* findUserById(int id)
    {
        for (auto user : users)
        {
            if (user->getUserId() == id)
            {
                return user;
            }
        }
        return nullptr;
    }
};


// ========== Out-of-line Definitions for Display Functions ==========

void User::display(const Library & lib) const
{
    cout << "=====================================" << endl;
    cout << "User ID: " << userId << endl;
    cout << "Username: " << username << endl;
    account.printAccountDetails();
    vector<Book> reservedBooks = lib.getReservedBooksByUser(userId);
    cout << "\nReserved Books:" << endl;
    if (reservedBooks.empty())
    {
        cout << "No reserved books." << endl;
    }
    else
    {
        for (const auto & book : reservedBooks)
        {
            cout << "Book ID: " << book.getId() << ", Title: " << book.getTitle() << endl;
        }
    }
    cout << "=====================================" << endl;
}

void Student::display(const Library & lib) const
{
    cout << "----- Student Portal -----" << endl;
    time_t now = time(0);
    double computedFine = 0;
    for (const auto & record : account.getBorrowRecords())
    {
        int daysElapsed = static_cast<int>(difftime(now, record.borrowTimestamp) / 86400);
        if (daysElapsed > 15)
        {
            computedFine += (daysElapsed - 15) * fineRate;
        }
    }
    cout << "User ID: " << userId << "\nUsername: " << username << endl;
    account.printAccountDetails();
    cout << "Computed Overdue Fine (for active borrows): " << computedFine << " rupees" << endl;
    vector<Book> reservedBooks = lib.getReservedBooksByUser(userId);
    cout << "\nReserved Books:" << endl;
    if (reservedBooks.empty())
    {
        cout << "No reserved books." << endl;
    }
    else
    {
        for (const auto & book : reservedBooks)
        {
            cout << "Book ID: " << book.getId() << ", Title: " << book.getTitle() << endl;
        }
    }
    cout << "-------------------------------------" << endl;
}

void Faculty::display(const Library & lib) const
{
    cout << "----- Faculty Portal -----" << endl;
    User::display(lib);
}

void Librarian::display(const Library & lib) const
{
    cout << "----- Librarian Portal -----" << endl;
    User::display(lib);
}


// ========== Implementation of Student Member Functions ==========

void Student::borrowBook(Library & lib)
{
    if (account.getFine() > 0)
    {
        cout << "Outstanding fine of " << account.getFine() << " rupees. Please pay fine before borrowing." << endl;
        return;
    }
    if (account.getBorrowRecords().size() >= (size_t)maxBooks)
    {
        cout << "Borrowing limit reached (" << maxBooks << " books maximum). No further books can be borrowed." << endl;
        return;
    }
    cout << "Enter the Book ID to borrow: ";
    int bookId;
    cin >> bookId;
    Book* book = lib.findBookById(bookId);
    if (!book)
    {
        cout << "Book not found." << endl;
        return;
    }
    if (book->getBorrowedBy() != 0)
    {
        cout << "Book is not available." << endl;
        return;
    }
    cout << "Enter number of days to borrow (maximum " << maxDays << "): ";
    int days;
    cin >> days;
    if (days > maxDays)
    {
        cout << "Borrowing period exceeds the maximum allowed for students." << endl;
        return;
    }
    book->updateStatus(BookStatus::Borrowed);
    book->updateBorrowedBy(this->getUserId());
    account.addBorrowedBook(book->getId(), days);
    cout << "Book \"" << book->getTitle() << "\" successfully borrowed for " << days << " days." << endl;
    lib.logTransaction("Student " + getUsername() + " borrowed book \"" + book->getTitle() + "\" for " + to_string(days) + " days.");
    lib.saveBooks();
}

void Student::reserveBook(Library & lib)
{
    cout << "Enter the Book ID to reserve: ";
    int bookId;
    cin >> bookId;
    Book* book = lib.findBookById(bookId);
    if (!book)
    {
        cout << "Book not found." << endl;
        return;
    }
    if (book->getStatus() != BookStatus::Borrowed)
    {
        cout << "You can only reserve a book that is currently borrowed." << endl;
        return;
    }
    if (book->getReservedBy() != 0)
    {
        cout << "Book is already reserved by another user." << endl;
        return;
    }
    if (book->getBorrowedBy() == this->getUserId())
    {
        cout << "You have already borrowed this book; reservation not allowed." << endl;
        return;
    }
    book->updateReservedBy(this->getUserId());
    book->updateStatus(BookStatus::Reserved);
    cout << "Book \"" << book->getTitle() << "\" reserved successfully. It will be automatically borrowed for you upon return." << endl;
    lib.logTransaction("Student " + getUsername() + " reserved book \"" + book->getTitle() + "\".");
    lib.saveBooks();
}

void Student::returnBook(Library & lib)
{
    lib.printBorrowedBooksByUser(this);
    cout << "Enter the Book ID to return: ";
    int bookId;
    cin >> bookId;
    Book* book = lib.findBookById(bookId);
    if (!book)
    {
        cout << "Book not found." << endl;
        return;
    }
    vector<BorrowRecord> records = account.getBorrowRecords();
    bool found = false;
    int intendedDays = -1;
    time_t borrowTime;
    for (const auto & record : records)
    {
        if (record.bookId == book->getId())
        {
            intendedDays = record.borrowDays;
            borrowTime = record.borrowTimestamp;
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "You did not borrow this book." << endl;
        return;
    }
    time_t now = time(0);
    int elapsedDays = static_cast<int>(difftime(now, borrowTime) / 86400);
    cout << "Book was kept for " << elapsedDays << " days." << endl;
    int allowedDays = 15;
    if (elapsedDays > allowedDays)
    {
        int overdue = elapsedDays - allowedDays;
        double fine = overdue * 10;
        account.addFine(fine);
        cout << "Book is overdue by " << overdue << " days. Fine of " << fine << " rupees imposed." << endl;
    }
    if (book->getReservedBy() != 0)
    {
        User* reservingUser = lib.findUserById(book->getReservedBy());
        if (reservingUser != nullptr)
        {
            int defaultDays = 15;
            if (dynamic_cast<Faculty*>(reservingUser))
            {
                defaultDays = 30;
            }
            book->updateStatus(BookStatus::Borrowed);
            book->updateBorrowedBy(reservingUser->getUserId());
            book->updateReservedBy(0);
            reservingUser->getAccount().addBorrowedBook(book->getId(), defaultDays);
            lib.logTransaction("Book \"" + book->getTitle() + "\" automatically borrowed by reserving user " + reservingUser->getUsername() + " for " + to_string(defaultDays) + " days upon return.");
            cout << "Book reserved for you has been automatically borrowed upon return." << endl;
        }
        else
        {
            book->updateStatus(BookStatus::Available);
            book->updateBorrowedBy(0);
        }
    }
    else
    {
        book->updateStatus(BookStatus::Available);
        book->updateBorrowedBy(0);
    }
    account.removeBorrowedBook(book->getId());
    cout << "Book returned successfully." << endl;
    lib.logTransaction("Student " + getUsername() + " returned book \"" + book->getTitle() + "\"; kept for " + to_string(elapsedDays) + " days (allowed: " + to_string(allowedDays) + ").");
    lib.saveBooks();
}


// ========== Implementation of Faculty Member Functions ==========

void Faculty::borrowBook(Library & lib)
{
    time_t now = time(0);
    for (const auto & record : getAccount().getBorrowRecords())
    {
        double diff = difftime(now, record.borrowTimestamp);
        if (diff > (record.borrowDays + 60) * 86400)
        {
            cout << "You have a book overdue by more than 60 days. You cannot borrow new books until you return it." << endl;
            return;
        }
    }
    if (account.getBorrowRecords().size() >= (size_t)maxBooks)
    {
        cout << "Borrowing limit reached (" << maxBooks << " books maximum)." << endl;
        return;
    }
    cout << "Enter the Book ID to borrow: ";
    int bookId;
    cin >> bookId;
    Book* book = lib.findBookById(bookId);
    if (!book)
    {
        cout << "Book not found." << endl;
        return;
    }
    if (book->getBorrowedBy() != 0)
    {
        cout << "Book is not available." << endl;
        return;
    }
    cout << "Enter number of days to borrow (maximum " << maxDays << "): ";
    int days;
    cin >> days;
    if (days > maxDays)
    {
        cout << "Borrowing period exceeds the maximum allowed for faculty." << endl;
        return;
    }
    book->updateStatus(BookStatus::Borrowed);
    book->updateBorrowedBy(this->getUserId());
    account.addBorrowedBook(book->getId(), days);
    cout << "Book \"" << book->getTitle() << "\" successfully borrowed for " << days << " days." << endl;
    lib.logTransaction("Faculty " + getUsername() + " borrowed book \"" + book->getTitle() + "\" for " + to_string(days) + " days.");
    lib.saveBooks();
}

void Faculty::reserveBook(Library & lib)
{
    cout << "Enter the Book ID to reserve: ";
    int bookId;
    cin >> bookId;
    Book* book = lib.findBookById(bookId);
    if (!book)
    {
        cout << "Book not found." << endl;
        return;
    }
    if (book->getStatus() != BookStatus::Borrowed)
    {
        cout << "You can only reserve a book that is currently borrowed." << endl;
        return;
    }
    if (book->getReservedBy() != 0)
    {
        cout << "Book is already reserved by another user." << endl;
        return;
    }
    if (book->getBorrowedBy() == this->getUserId())
    {
        cout << "You have already borrowed this book; reservation not allowed." << endl;
        return;
    }
    book->updateReservedBy(this->getUserId());
    book->updateStatus(BookStatus::Reserved);
    cout << "Book \"" << book->getTitle() << "\" reserved successfully. It will be automatically borrowed for you upon return." << endl;
    lib.logTransaction("Faculty " + getUsername() + " reserved book \"" + book->getTitle() + "\".");
    lib.saveBooks();
}

void Faculty::returnBook(Library & lib)
{
    lib.printBorrowedBooksByUser(this);
    cout << "Enter the Book ID to return: ";
    int bookId;
    cin >> bookId;
    Book* book = lib.findBookById(bookId);
    if (!book)
    {
        cout << "Book not found." << endl;
        return;
    }
    vector<BorrowRecord> records = account.getBorrowRecords();
    bool found = false;
    int intendedDays = -1;
    time_t borrowTime;
    for (const auto & record : records)
    {
        if (record.bookId == book->getId())
        {
            intendedDays = record.borrowDays;
            borrowTime = record.borrowTimestamp;
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "You did not borrow this book." << endl;
        return;
    }
    time_t now = time(0);
    int elapsedDays = static_cast<int>(difftime(now, borrowTime) / 86400);
    cout << "Book was kept for " << elapsedDays << " days." << endl;
    if (elapsedDays > intendedDays)
    {
        int overdue = elapsedDays - intendedDays;
        cout << "Book is overdue by " << overdue << " days. (No fine imposed for faculty)" << endl;
        if (overdue > 60)
        {
            cout << "Warning: You have an overdue book for more than 60 days." << endl;
        }
    }
    if (book->getReservedBy() != 0)
    {
        User* reservingUser = lib.findUserById(book->getReservedBy());
        if (reservingUser != nullptr)
        {
            int defaultDays = 30;
            book->updateStatus(BookStatus::Borrowed);
            book->updateBorrowedBy(reservingUser->getUserId());
            book->updateReservedBy(0);
            reservingUser->getAccount().addBorrowedBook(book->getId(), defaultDays);
            lib.logTransaction("Book \"" + book->getTitle() + "\" automatically borrowed by reserving user " + reservingUser->getUsername() + " for " + to_string(defaultDays) + " days upon return.");
            cout << "Book reserved for you has been automatically borrowed upon return." << endl;
        }
        else
        {
            book->updateStatus(BookStatus::Available);
            book->updateBorrowedBy(0);
        }
    }
    else
    {
        book->updateStatus(BookStatus::Available);
        book->updateBorrowedBy(0);
    }
    account.removeBorrowedBook(book->getId());
    cout << "Book returned successfully." << endl;
    lib.logTransaction("Faculty " + getUsername() + " returned book \"" + book->getTitle() + "\"; kept for " + to_string(elapsedDays) + " days (intended: " + to_string(intendedDays) + ").");
    lib.saveBooks();
}


// ========== Implementation of Librarian Administrative Functions ==========

void Librarian::addBook(Library & lib)
{
    int newId = lib.generateBookId();
    cin.ignore();
    cout << "Enter title: ";
    string title;
    getline(cin, title);
    cout << "Enter author: ";
    string author;
    getline(cin, author);
    cout << "Enter publisher: ";
    string publisher;
    getline(cin, publisher);
    cout << "Enter publication year: ";
    int year;
    cin >> year;
    cin.ignore();
    cout << "Enter ISBN: ";
    string isbn;
    getline(cin, isbn);
    Book newBook(newId, title, author, publisher, year, isbn, BookStatus::Available);
    lib.addBookToLibrary(newBook);
    cout << "Book added successfully." << endl;
}

void Librarian::removeBook(Library & lib)
{
    cout << "Enter Book ID to remove: ";
    int id;
    cin >> id;
    lib.removeBookFromLibrary(id);
}

void Librarian::updateBook(Library & lib)
{
    cout << "Enter Book ID to update: ";
    int id;
    cin >> id;
    Book* book = lib.findBookById(id);
    if (!book)
    {
        cout << "Book not found." << endl;
        return;
    }
    cin.ignore();
    cout << "Updating book details. Press ENTER to skip a field." << endl;
    cout << "Current Title: " << book->getTitle() << ". New Title: ";
    string input;
    getline(cin, input);
    if (!input.empty())
    {
        book->updateTitle(input);
    }
    cout << "Current Publisher: " << book->getPublisher() << ". New Publisher: ";
    getline(cin, input);
    if (!input.empty())
    {
        book->updatePublisher(input);
    }
    cout << "Current Year: " << book->getYear() << ". New Year: ";
    getline(cin, input);
    if (!input.empty())
    {
        book->updateYear(stoi(input));
    }
    cout << "Current ISBN: " << book->getISBN() << ". New ISBN: ";
    getline(cin, input);
    if (!input.empty())
    {
        book->updateISBN(input);
    }
    cout << "Book updated successfully." << endl;
    lib.logTransaction("Librarian updated book (ID): " + to_string(id));
    lib.saveBooks();
}

void Librarian::addUser(Library & lib)
{
    int newId = lib.generateUserId();
    cout << "Enter user type (1 for Student, 2 for Faculty): ";
    int type;
    cin >> type;
    cin.ignore();
    cout << "Enter username: ";
    string uname;
    cin >> uname;
    cout << "Enter password: ";
    string pwd;
    cin >> pwd;
    User * newUser = nullptr;
    if (type == 1)
    {
        newUser = new Student(newId, uname, pwd);
    }
    else if (type == 2)
    {
        newUser = new Faculty(newId, uname, pwd);
    }
    else
    {
        cout << "Invalid user type." << endl;
        return;
    }
    lib.addUserToLibrary(newUser);
    cout << "User added successfully." << endl;
}

void Librarian::removeUser(Library & lib)
{
    cout << "Enter User ID to remove: ";
    int id;
    if (!(cin >> id))
    {
        cout << "Invalid input for User ID. Please enter an integer." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    if (id == this->getUserId())
    {
        cout << "You cannot remove your own account." << endl;
        return;
    }
    lib.removeUserFromLibrary(id);
}

void Librarian::updateUser(Library & lib)
{
    cout << "Enter User ID to update: ";
    int id;
    if (!(cin >> id))
    {
        cout << "Invalid input for User ID. Please enter an integer." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore();
    cout << "Enter new username (or press ENTER to leave unchanged): ";
    string newUsername;
    getline(cin, newUsername);
    cout << "Enter new password (or press ENTER to leave unchanged): ";
    string newPassword;
    getline(cin, newPassword);
    lib.updateUserInLibrary(id, newUsername, newPassword);
    cout << "User updated successfully." << endl;
}


// ========== Portal Menus ==========

void userPortalMenu(User * user, Library & lib)
{
    int choice;
    do
    {
        cout << "\n----- User Portal -----" << endl;
        cout << "1. View Book List" << endl;
        cout << "2. Borrow Book" << endl;
        cout << "3. Reserve Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. View Account Details" << endl;
        cout << "6. Pay Fine" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice)
        {
            case 1:
            {
                lib.printBooksForUser(user->getUserId());
                break;
            }
            case 2:
            {
                user->borrowBook(lib);
                break;
            }
            case 3:
            {
                if (dynamic_cast<Student*>(user))
                {
                    static_cast<Student*>(user)->reserveBook(lib);
                }
                else if (dynamic_cast<Faculty*>(user))
                {
                    static_cast<Faculty*>(user)->reserveBook(lib);
                }
                break;
            }
            case 4:
            {
                user->returnBook(lib);
                break;
            }
            case 5:
            {
                user->display(lib);
                break;
            }
            case 6:
            {
                double fine = user->getAccount().getFine();
                if (fine > 0)
                {
                    cout << "Paying fine of " << fine << " rupees." << endl;
                    user->getAccount().resetFine();
                    if (dynamic_cast<Student*>(user))
                    {
                        user->getAccount().resetBorrowTimestamps();
                    }
                    cout << "Fine cleared." << endl;
                }
                else
                {
                    cout << "No fine due." << endl;
                }
                break;
            }
            case 7:
            {
                cout << "Logging out..." << endl;
                break;
            }
            default:
            {
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        }
        
    } while (choice != 7);
}


void librarianPortalMenu(Librarian * libUser, Library & lib)
{
    int choice;
    do
    {
        cout << "\n----- Librarian Portal -----" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. Update Book" << endl;
        cout << "4. Add User" << endl;
        cout << "5. Remove User" << endl;
        cout << "6. Update User" << endl;
        cout << "7. View All Books" << endl;
        cout << "8. View All Users" << endl;
        cout << "9. View Transaction Log" << endl;
        cout << "10. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice)
        {
            case 1:
            {
                libUser->addBook(lib);
                break;
            }
            case 2:
            {
                libUser->removeBook(lib);
                break;
            }
            case 3:
            {
                libUser->updateBook(lib);
                break;
            }
            case 4:
            {
                libUser->addUser(lib);
                break;
            }
            case 5:
            {
                libUser->removeUser(lib);
                break;
            }
            case 6:
            {
                libUser->updateUser(lib);
                break;
            }
            case 7:
            {
                lib.printAllBooks();
                break;
            }
            case 8:
            {
                lib.printAllUsers();
                break;
            }
            case 9:
            {
                lib.viewTransactionLog();
                break;
            }
            case 10:
            {
                cout << "Logging out..." << endl;
                break;
            }
            default:
            {
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        }
        
    } while (choice != 10);
}


// ========== Role Selection and Login/Registration ==========

void displayRoleSelectionMenu()
{
    cout << "\n========================================" << endl;
    cout << "         Library Management System      " << endl;
    cout << "========================================" << endl;
    cout << "Select your role:" << endl;
    cout << "1. Student" << endl;
    cout << "2. Faculty" << endl;
    cout << "3. Librarian" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}


void registrationProcess(Library & lib, int role)
{
    cin.ignore();
    cout << "\n=== Registration ===" << endl;
    cout << "Enter desired username (no spaces): ";
    string uname;
    cin >> uname;
    cout << "Enter password: ";
    string pwd;
    cin >> pwd;
    cout << "Confirm password: ";
    string cpwd;
    cin >> cpwd;
    if (pwd != cpwd)
    {
        cout << "Passwords do not match. Registration failed." << endl;
        return;
    }
    int newId = lib.generateUserId();
    User * newUser = nullptr;
    if (role == 1)
    {
        newUser = new Student(newId, uname, pwd);
    }
    else if (role == 2)
    {
        newUser = new Faculty(newId, uname, pwd);
    }
    lib.addUserToLibrary(newUser);
    cout << "Registration successful. Please log in with your new credentials." << endl;
}


void loginProcess(Library & lib, int role)
{
    cin.ignore();
    cout << "\n=== Login ===" << endl;
    cout << "Enter username: ";
    string uname;
    cin >> uname;
    cout << "Enter password: ";
    string pwd;
    cin >> pwd;
    User * user = lib.authenticateUser(uname, pwd);
    if (user != nullptr)
    {
        if (role == 1 && dynamic_cast<Student*>(user))
        {
            cout << "Login successful. Welcome, " << user->getUsername() << "!" << endl;
            userPortalMenu(user, lib);
        }
        else if (role == 2 && dynamic_cast<Faculty*>(user))
        {
            cout << "Login successful. Welcome, " << user->getUsername() << "!" << endl;
            userPortalMenu(user, lib);
        }
        else if (role == 3 && dynamic_cast<Librarian*>(user))
        {
            cout << "Login successful. Welcome, Librarian!" << endl;
            librarianPortalMenu(dynamic_cast<Librarian*>(user), lib);
        }
        else
        {
            cout << "Your account does not match the selected role." << endl;
        }
    }
    else
    {
        cout << "Invalid credentials." << endl;
    }
}


// ========== Main Function ==========

int main()
{
    Library lib;
    int roleChoice;
    while (true)
    {
        displayRoleSelectionMenu();
        cin >> roleChoice;
        if (roleChoice == 4)
        {
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        }
        if (roleChoice < 1 || roleChoice > 3)
        {
            cout << "Invalid role selection. Please try again." << endl;
            continue;
        }
        if (roleChoice == 1 || roleChoice == 2)
        {
            cout << "Are you already registered? (YES/yes or NO/no): ";
            string answer;
            cin >> answer;
            for (auto & c : answer)
            {
                c = toupper(c);
            }
            if (answer == "YES")
            {
                loginProcess(lib, roleChoice);
            }
            else if (answer == "NO")
            {
                registrationProcess(lib, roleChoice);
            }
            else
            {
                cout << "Invalid answer. Please enter YES or NO." << endl;
            }
        }
        else if (roleChoice == 3)
        {
            loginProcess(lib, roleChoice);
        }
    }
    return 0;
}


/* ============================================================================
   End of Library Management System Code
============================================================================ */
