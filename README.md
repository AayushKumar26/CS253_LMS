# Library Management System

## Objective
Design and implement a Library Management System (LMS) in C++ using Object-Oriented Programming (OOP) concepts. This system demonstrates abstraction, inheritance, polymorphism, and encapsulation to manage library operations such as borrowing books, returning them, calculating fines, and managing both users and the book collection.

## Introduction
Modern libraries need efficient, automated systems to manage their resources and user interactions. This project provides a console-based LMS where different types of users—students, faculty, and librarians—can interact with the system. Each role has specific privileges and restrictions, ensuring smooth library operations.

## Key Features

### User Roles and Permissions

#### Students
- **Borrowing:**
  - Can borrow up to **3 books** simultaneously.
  - Maximum borrowing duration: **15 days**.
  - Overdue books incur a fine of **10 rupees per day**.
- **Returning:**
  - On return, the system calculates the overdue period (if any) and adds the corresponding fine.
- **Account Details:**
  - Displays currently borrowed books.
  - Separately lists overdue books.
  - Now also displays **Reserved Books** (i.e. books that have been reserved while still on loan).
  - Shows both:
    - **Computed Overdue Fine** (active borrows, calculated in real time).
    - **Fine Due** (finalized fine for returned overdue books).
- **Fine Clearance:**
  - Students can clear outstanding fines via a dedicated menu option. This resets the fine amount and updates borrowing records.

#### Faculty
- **Borrowing:**
  - Can borrow up to **5 books** simultaneously.
  - Maximum borrowing duration: **30 days**.
  - No fines are imposed for overdue books.
  - However, if any borrowed book is overdue by **more than 60 days**, the faculty member is blocked from borrowing additional books.
- **Returning:**
  - The process is similar to that for students: books are marked as available and borrow records are updated.
  - A warning is issued if a book is overdue, particularly if it exceeds 60 days.
- **Account Details:**
  - Displays the list of currently borrowed books.
  - Shows overdue information (without fines).
  - Also displays any reserved books.
- **Reserve Book:**
  - Faculty can reserve a book that is currently borrowed by another user.

#### Librarians
- **Administrative Functions:**
  - **Book Management:**
    - Add new books.
    - Remove or update existing books.
    - View the complete list of books with their computed statuses.
  - **User Management:**
    - Register new users (students and faculty).
    - Update or remove existing user accounts.
  - **Transaction Log:**
    - Access a detailed log of all system transactions (borrowing, returning, fines, and administrative actions).
- **Restrictions:**
  - Librarians cannot borrow or reserve books.
- **Default Accounts:**
  - There are **three default librarian accounts** with the following credentials:
    - **librarian1** / **admin1**
    - **librarian2** / **admin2**
    - **librarian3** / **admin3**

### Book Management
- **Book Records:**
  - Each book has a unique ID, title, author, publisher, year, ISBN, and a computed status.
  - **Status:**
    - **Available:** The book is not currently borrowed.
    - **Borrowed:** The book is currently checked out.
    - **Reserved:** The book is borrowed by another user.
    - In user views, the computed status is shown as:
      - "Borrowed" if the logged‐in user has borrowed that copy.
      - "Reserved (For You)" if the logged‐in user reserved that copy.
      - "Reserved" if another user reserved that copy.
- **Viewing Books:**
  - Users can view a full list of books with their details and computed status.

### Account Management and Fine Calculation
- **Account Tracking:**
  - Each user has an account that maintains:
    - A record of currently borrowed books (each record stores the book ID, borrow timestamp, and intended borrow days).
    - A dynamic calculation of fines for overdue books.
- **Fine Calculation (Students):**
  - **Fine Due (Finalized):**  
    For returned overdue books, the fine is calculated as:
    ```
    Fine = (Days Overdue) × 10 rupees/day
    ```
  - **Computed Overdue Fine (Active Borrows):**  
    A real-time calculation for books still on loan and overdue. This value is displayed under “Computed Overdue Fine.”
- **Overdue Books Display:**
  - Borrowed books are categorized into:
    - **Borrowed Books:** Books still within the allowed period.
    - **Overdue Books:** Books that have exceeded the allowed borrowing period.
  - Additionally, **Reserved Books** are now shown.

### Transaction Logging
- **Logging:**
  - Every operation (borrowing, returning, reserving, updating, etc.) is recorded in a transaction log with a timestamp.
- **Persistence:**
  - The transaction log is stored in `transactions.txt` to track all events across sessions.

### Data Persistence and File I/O
- **Files Used:**
  - `books.txt` – Stores all book records.
  - `users.txt` – Stores user data (including account details such as borrow records and fines).
  - `transactions.txt` – Contains a log of all transactions.
- **Data Loading and Saving:**
  - On startup, data is loaded from these files. If they are missing or empty, the system initializes with default data.
  - All changes are immediately saved to ensure persistence between sessions.

### Error Handling and User Guidance
- **Input Validation:**
  - The system validates all inputs (e.g., numeric values for days, valid book IDs) and displays appropriate error messages.
- **Clear Prompts:**
  - Users are guided step-by-step with clear console messages.
- **Robustness:**
  - File I/O errors are handled gracefully and data integrity is maintained.

## Assumptions and Constraints

### Assumptions
- **Unique Identification:**
  - Users are uniquely identified by their username, and each book is assigned a unique ID automatically.
- **Accurate Input:**
  - Users are expected to provide correct information regarding borrowing durations and returns.
- **Fine Payment:**
  - Fines apply only to students (10 rupees per overdue day); faculty do not incur fines.

### Constraints
- **Borrowing Limits:**
  - **Students:** Maximum 3 books for 15 days.
  - **Faculty:** Maximum 5 books for 30 days.
- **Fine Calculation:**
  - For students, fines are calculated at 10 rupees per day overdue.
- **Data Storage:**
  - Data is stored in plain text files, suitable for small-to-medium libraries.
- **User Interface:**
  - The system is console-based and designed for single-user operation.

## File Structure
- **cs253Assgn.cpp:**  
  Contains the full source code for the Library Management System.
- **books.txt:**  
  Stores book details (Book ID, title, publisher, year, ISBN, and computed status).
- **users.txt:**  
  Stores user details (user type, username, password, and account details including borrow records and fines).
- **transactions.txt:**  
  Logs all system transactions (e.g., borrowing, returning, fine updates, administrative actions).

## Pre-Existing Data

### Users
- **Students:**
  - `alice` (ID 1, password: pass1)
  - `bob` (ID 2, password: pass2)
  - `charlie` (ID 3, password: pass3)
  - `diana` (ID 4, password: pass4)
  - `eric` (ID 5, password: pass5)
- **Faculty:**
  - `profX` (ID 6, password: pass6)
  - `drY` (ID 7, password: pass7)
  - `mrZ` (ID 8, password: pass8)
- **Librarians:**
  - `librarian1` (ID 9, password: admin1)
  - `librarian2` (ID 10, password: admin2)
  - `librarian3` (ID 11, password: admin3)

### Books
A default set of 10 titles with 5 copies each (totaling 50 records) is loaded if no file data is found.

### Transactions
All operations (borrowing, returning, updating, etc.) are logged with timestamps in `transactions.txt`.

## How to Use

### Compilation
Compile the program using a C++ compiler:
```bash
g++ -std=c++11 cs253Assgn.cpp -o cs253Assgn
```
### Running the Program
Run the compiled binary:

```bash
./cs253Assgn
```
## Navigation

### Role Selection:
- On startup, select your role from the main menu:
  - Student
  - Faculty
  - Librarian
  - Exit

### Login/Registration:
- **Existing Users:**
  - Log in with your username and password.
- **New Users:**
  - Register by providing a unique username and password.
  - (Note: Registration is available for students and faculty only.)

### Student Profile:
- **View Book List:**
  - Displays all available books with their computed statuses.
- **Borrow Book:**
  - Enter the Book ID and borrowing duration (up to 15 days).
- **Return Book:**
  - Return a borrowed book; the system calculates any overdue fine.
- **View Account Details:**
  - Lists currently borrowed books, overdue books, and reserved books, along with the computed overdue fine.
- **Pay Fine:**
  - Clear outstanding fines and update your borrowing records.

### Faculty Profile:
- **View Book List:**
  - Displays all books with computed statuses.
- **Borrow Book:**
  - Enter the Book ID and borrowing duration (up to 30 days).
  - *Note: No fines are imposed; however, borrowing is blocked if any book is overdue by more than 60 days.*
- **Return Book:**
  - Return books; a warning is issued if a book is overdue.
- **View Account Details:**
  - Displays borrowed books, overdue warnings, and reserved books.
- **Reserve Book:**
  - Reserve a book that is currently borrowed by another user.

### Librarian Profile:
- **Manage Books:**
  - Add, remove, or update book records.
- **Manage Users:**
  - Add, update, or remove user accounts.
- **View All Records:**
  - View the complete list of books (with computed statuses) and users.
- **View Transaction Log:**
  - Access the full log of system transactions.
- **Note:**
  - Librarians cannot borrow or reserve books.

## Conclusion

This Library Management System is a robust, role-based solution that leverages C++ and OOP principles to manage library operations efficiently. It provides:

- A user-friendly console interface,
- Detailed functionalities for students and faculty (including dynamic fine calculation and reserved books tracking),
- Comprehensive administrative tools for librarians (with three default librarian accounts),
- Immediate data persistence through file I/O,
- And thorough transaction logging for audit purposes.

This system is ideal for educational purposes and for small to medium-scale libraries. Enjoy managing your library efficiently!

Happy Managing!!
