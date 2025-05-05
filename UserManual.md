# Library Management System User Manual

## User Roles and Functionalities 

### Role Selection
- **On Startup**:  
  - Student
  - Faculty
  - Librarian
  - Exit

- **Input**: Select a role by typing the corresponding number (e.g., `1` for Student).

---

## Login/Registration 

### Existing Users
1. Enter **username** and **password**.  
2. System validates credentials and grants access.  

### New Users (Students/Faculty Only)
1. Select **registration option**.  
2. Provide:  
 - Unique `username` (no spaces).  
 - `password` and confirm it.  
3. Log in with new credentials after registration.  

---

## Student Profile 

### View Book List
- Displays all books with **computed statuses**:  
- `Available`, `Borrowed`, `Reserved`, or `Reserved (For You)`.

### Borrow Book
1. Enter **Book ID** from the list.  
2. Specify borrowing days (max **15 days**).  
3. System records the transaction and updates book status.  

### Return Book
1. Enter **Book ID**.  
2. System:  
 - Calculates overdue fines (**₹10/day**).  
 - Updates book status to `Available` or assigns to reserving user.  

### View Account Details
Displays:  
- **Borrowed Books**: Within allowed period.  
- **Overdue Books**: Exceeded borrowing period.  
- **Reserved Books**: Reserved while borrowed by others.  
- **Computed Overdue Fine**: Real-time calculation for active borrows.  
- **Fine Due**: Finalized fines for returned books.  

### Pay Fine
- Clears outstanding fines and resets borrow timestamps.  

---

## Faculty Profile 

### View Book List
- Displays books with **computed statuses**.  

### Borrow Book
1. Enter **Book ID**.  
2. Specify borrowing days (max **30 days**).  
 - **Note**: Overdue >60 days blocks further borrowing.  

### Return Book
1. Enter **Book ID**.  
2. System issues warnings for overdue books.  

### View Account Details
- Displays borrow records, overdue notifications, and reserved books.  

### Reserve Book
- Enter **Book ID** of a borrowed book (not already reserved).  

---

## Librarian Profile 

### Manage Books
- **Add Book**:  
Input `title`, `author`, `publisher`, `year`, `ISBN`.  
System assigns unique **Book ID** and sets status to `Available`.  
- **Remove/Update Book**: Use **Book ID** to modify records.  

### Manage Users
- Add/update/remove **students** and **faculty** accounts.  

### View All Records
- Displays:  
- All books with **computed statuses**.  
- All registered users.  

### View Transaction Log
- Detailed log of actions with timestamps (e.g., `[Thu Mar 16 14:22:45 2023] Student alice borrowed "Clean Code"`).  

 **Note**: Librarians **cannot** borrow or reserve books.  
 

---

## Data Persistence and File I/O 

### Files Used
| File               | Purpose                                                                 |
|--------------------|-------------------------------------------------------------------------|
| `books.txt`        | Stores book records (ID, title, publisher, year, ISBN, computed status). |
| `users.txt`        | Stores user data (type, username, password, borrow records, fines).     |
| `transactions.txt` | Logs all transactions (borrow/return/reserve actions, admin changes).    |

### Data Handling
- **Auto-Load**: Data loads on startup.  
- **Default Data**: Initializes if files are missing or empty.  
- **Auto-Save**: Changes persist immediately.  

---

## Error Handling and User Guidance 

### Input Validation
- Ensures valid **numeric inputs** (e.g., Book IDs, days).  
- Displays clear error messages for invalid inputs.  

### User Prompts
- Guides users through every step with concise console messages.  

### Robustness
- Handles file I/O errors gracefully (e.g., loads default data if corrupted).  

---

## Conclusion 
This **role-based system** leverages **C++ OOP** for efficient library management:  
-  **User-friendly** console interface.  
-  Dynamic fine calculation and reserved book tracking.  
-  Immediate data persistence via file I/O.  
-  Comprehensive transaction logging.  

Ideal for **educational use** or small-to-medium libraries!  