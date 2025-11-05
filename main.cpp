#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include "animation.h"
#include "manager.h"
#include "customerCart.h"
#include "customerQueue.h"
using namespace std;

/* ---- Login linked-list ---- */
struct Login {
    string lname, lphone, lusername, lpassword;
    Login *linker;
    Login(string a="", string b="", string c="", string d="") 
        : lname(a), lphone(b), lusername(c), lpassword(d), linker(nullptr) {}
};

static Login *first = nullptr;

/* ---- Forward declarations ---- */
void loginOrSign();
void saveNewUserToFile(Login *temp);
void loadUsersFromFile();
void exitConfirmation();
void showCredits();
void CashierMode();
void CustomerMode();
void ManagerMode();
void cashierLogin();
void managerLogin();


/* ---- Forward declarations ---- */
void loginOrSign();
void saveNewUserToFile(Login *temp);
void loadUsersFromFile();
void exitConfirmation();
void showCredits();
void CashierMode();
void CustomerMode();
void ManagerMode();
void cashierLogin();
void managerLogin();

/* Missing Function Declarations Added */
void showSalesAnalytics();
void showPeakHourAnalysis();
void searchCustomerInQueue();
void filterByPriceRange();
void addToWishlist();
void viewWishlist();
void showRecommendations();
void reverseProductList();
void showProductStatistics();
void showLowStockAlerts();

/**
 * @brief Renders the application's ASCII-art title, applies color styling, and pauses briefly.
 *
 * Clears the console, draws a multi-line ASCII title and subtitle using positioned output with multiple colors, then waits 5 seconds before returning.
 */
void printTitle() {
    system("cls");
    setColor(Color::BRIGHT_CYAN);
    gotoxy(30, 6);  cout << " _____ _   _  ___  ____  ____  ___ _   _  ____";
    gotoxy(30, 7);  cout << "/ ____| | | |/ _ \\|  _ \\|  _ \\|_ _| \\ | |/ ___|";
    gotoxy(30, 8);  cout << "\\___ \\| |_| | | | | |_) | |_) || ||  \\| | |  _";
    gotoxy(30, 9);  cout << " ___) |  _  | |_| |  __/|  __/ | || |\\  | |_| |";
    gotoxy(30, 10); cout << "|____/|_| |_|\\___/|_|   |_|   |___|_| \\_|\\____|";
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(25, 13); cout << "   __  __    _    _     _        ______   _______ _____ __  __";
    gotoxy(25, 14); cout << "  |  \\/  |  / \\  | |   | |      / ___\\ \\ / / ____|_   _|  \\/  |";
    gotoxy(25, 15); cout << "  | |\\/| | / _ \\ | |   | |      \\___ \\\\ V /\\___ \\ | | | |\\/| |";
    gotoxy(25, 16); cout << "  | |  | |/ ___ \\| |___| |___    ___) |  |  ___) || | | |  | |";
    gotoxy(25, 17); cout << "  |_|  |_/_/   \\_\\_____|_____| |____/ \\_/ |____/ |_| |_|  |_|";
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(32, 20); cout << "D A T A   S T R U C T U R E   P R O J E C T   2 0 2 5";
    setColor(Color::BRIGHT_WHITE);
    
    // ADD THIS: 5 second delay
    Sleep(5000);
}

/**
 * @brief Displays the application's development team credits in a styled, centered screen and waits for user acknowledgement.
 *
 * Shows the "DEVELOPMENT TEAM" header, a decorative box, the list of creators with their identifiers, and a prompt. The function blocks until the user presses any key.
 */
void showCredits() {
    drawHeader("DEVELOPMENT TEAM");
    
    drawAnimatedBox(35, 12, 60, 14, Color::BRIGHT_MAGENTA);
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(57, 14); cout << "CREATED BY:";
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(45, 17); cout << "Faizan Ahmed   (CT-194)";
    gotoxy(45, 18); cout << "Bilal Shams    (CT-196)";
    gotoxy(45, 19); cout << "Abbas Kazmi    (CT-187)";
    gotoxy(45, 20); cout << "Muhammad Ezaan (CT-199)";
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(45, 23); cout << "Press any key to continue...";
    getch(); 
}

/**
 * @brief Displays an exit confirmation prompt and handles the user's choice.
 *
 * Presents a confirmation dialog that waits for a single-key response. If the user
 * confirms ('Y' or 'y'), a farewell message is shown and the process terminates.
 * If the user cancels ('N' or 'n'), the function returns to the caller without
 * exiting the program.
 */
void exitConfirmation() {
    drawHeader("EXIT CONFIRMATION");
    
    drawAnimatedBox(40, 14, 50, 8, Color::BRIGHT_RED);
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(48, 17); cout << "Are you sure you want to exit?";
    setColor(Color::BRIGHT_CYAN);
    gotoxy(55, 19); cout << "[Y] Yes  [N] No";
    
    while (1) {
        char c = getch();
        if (c=='Y' || c=='y') {
            system("cls");
            setColor(Color::BRIGHT_GREEN);
            gotoxy(50, 15); cout << "Thank you for visiting!";
            Sleep(800);
            exit(0);
        }
        if (c=='N' || c=='n') return;
    }
}

/**
 * @brief Presents the Cashier portal menu and dispatches cashier-related actions.
 *
 * Displays the cashier interface and handles user selection for cashier features:
 * Customer List, Customer Check Out, View Total Revenue, Sales Analytics, Peak Hour Analysis,
 * Search Customer, returning to the main menu, and exit confirmation.
 */
void CashierMode() {
    char choice;
    while (1) {
        drawHeader("CASHIER PORTAL");
        
        drawAnimatedBox(38, 11, 54, 18, Color::BRIGHT_GREEN);
        
        setColor(Color::BRIGHT_CYAN);
        gotoxy(50, 13); cout << "1. Customer List";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(50, 14); cout << "2. Customer Check Out";
        setColor(Color::BRIGHT_GREEN);
        gotoxy(50, 15); cout << "3. View Total Revenue";
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(50, 16); cout << "4. Sales Analytics";
        setColor(Color::BRIGHT_CYAN);
        gotoxy(50, 17); cout << "5. Peak Hour Analysis";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(50, 18); cout << "6. Search Customer";
        
        setColor(Color::BRIGHT_RED);
        gotoxy(50, 21); cout << "0. Back to Main Menu";
        
        setColor(Color::BRIGHT_WHITE);
        gotoxy(52, 25); cout << "Enter Key: ";
        choice = getch();

        
        if (choice=='1') customerListDisplay();
        else if (choice=='2') dequeueCustomer();
        else if (choice=='3') {
            double rev = 0.0;
            int salesCount = 0;
            ifstream f("sales_history.csv");
            if (!f.is_open()) {
                drawHeader("TODAY'S REVENUE");
                setColor(Color::BRIGHT_RED);
                gotoxy(45, 14); cout << "No sales history file found.";
                setColor(Color::BRIGHT_WHITE);
                gotoxy(40, 15); cout << "Complete a customer checkout first.";
                gotoxy(50, 25); getch();
                continue;
            }
            string line; bool header = true;
            while (getline(f,line)) {
                if (header) { header = false; continue; }
                if (line.empty()) continue;
                istringstream ss(line);
                string name, phone, bill, method;
                getline(ss,name,','); getline(ss,phone,','); getline(ss,bill,','); getline(ss,method);
                try { 
                    rev += stod(bill); 
                    salesCount++;
                } catch(...) {}
            }
            f.close();

            drawHeader("TODAY'S REVENUE REPORT");
            
            drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_YELLOW);
            
            setColor(Color::BRIGHT_CYAN);
            gotoxy(52, 17); cout << "Total Sales: " << salesCount;
            
            gotoxy(50, 19);
            if (rev > 0) {
                setColor(Color::BRIGHT_GREEN);
                cout << "Total Revenue: ";
                setColor(Color::BRIGHT_YELLOW);
                cout << (int)rev << " Rs";
            } else {
                setColor(Color::BRIGHT_RED);
                cout << "No completed checkouts yet";
            }
            
            setColor(Color::BRIGHT_WHITE);
            gotoxy(50, 26); getch();
        }
        else if (choice=='4') showSalesAnalytics();
        else if (choice=='5') showPeakHourAnalysis();
        else if (choice=='6') searchCustomerInQueue();
        else if (choice=='0') {  return; }
        else if (choice==27) exitConfirmation();
    }
}

/**
 * @brief Presents the cashier login UI, authenticates credentials, and enters the cashier portal.
 *
 * Displays a styled login box, reads a username (line) and a masked password from console input,
 * and validates them against the cashier credentials. On successful authentication it shows a
 * success message, runs the loading sequence, and transfers control to the cashier mode. On
 * failed authentication it shows an error and re-prompts for login.
 */
void cashierLogin() {
    drawHeader("CASHIER LOGIN");
    
    drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_GREEN);
    
    string username, password;
    
    // FIX: Clear input buffer first
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 17); cout << "Username : "; 
    setColor(Color::BRIGHT_WHITE);
    getline(cin, username);  // Changed from cin >> to getline
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 19); cout << "Password : ";
    setColor(Color::BRIGHT_WHITE);
    
    char ch; password="";
    while (1) {
        ch = getch();
        if (ch==13) break;
        if (ch==8) { if (!password.empty()) { password.pop_back(); cout << "\b \b"; } }
        else { password.push_back(ch); cout << '*'; }
    }
    
    if (username=="admin" && password=="123") {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(52, 21); cout << "Login Successful!";
        Sleep(500);
        loadingBar();
        CashierMode();
    } else {
        setColor(Color::BRIGHT_RED);
        gotoxy(50, 21); cout << "Wrong credentials!";
        Sleep(1000);
        cashierLogin();
    }
}



/**
 * @brief Presents the customer portal menu and handles customer interactions.
 *
 * Displays the Customer Portal UI and processes user input to perform product browsing,
 * searching, purchasing, cart viewing, price-range filtering, wishlist management, and
 * product recommendations. The function runs until the user returns to the main menu
 * or chooses to exit the application.
 */
void CustomerMode() {
    char choice;
    while (1) {
        drawHeader("CUSTOMER PORTAL");
        
        drawAnimatedBox(38, 11, 54, 19, Color::BRIGHT_CYAN);
        
        setColor(Color::BRIGHT_CYAN);
        gotoxy(48, 13); cout << "1. Display All Products";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(48, 14); cout << "2. Search Products";
        setColor(Color::BRIGHT_GREEN);
        gotoxy(48, 15); cout << "3. Buy Products";
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(48, 16); cout << "4. Display Cart";
        setColor(Color::BRIGHT_BLUE);
        gotoxy(48, 17); cout << "5. Filter by Price Range";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(48, 18); cout << "6. Wishlist";
        setColor(Color::BRIGHT_GREEN);
        gotoxy(48, 19); cout << "7. Recommendations";
        
        setColor(Color::BRIGHT_RED);
        gotoxy(48, 22); cout << "0. Back to Main Menu";
        
        setColor(Color::BRIGHT_WHITE);
        gotoxy(50, 26); cout << "Enter Key: ";
        choice = getch();
        
        
        if (choice=='1') { displayProduct(); gotoxy(50,26); getch(); }
        else if (choice=='2') searchProductUI();
        else if (choice=='3') { cCart(); buyProductUI(); }
        else if (choice=='4') displayCart();
        else if (choice=='5') filterByPriceRange();
        else if (choice=='6') {
            drawHeader("WISHLIST OPTIONS");
            drawAnimatedBox(45, 15, 40, 8, Color::BRIGHT_MAGENTA);
            setColor(Color::BRIGHT_CYAN);
            gotoxy(52, 17); cout << "1. Add to Wishlist";
            setColor(Color::BRIGHT_YELLOW);
            gotoxy(52, 18); cout << "2. View Wishlist";
            setColor(Color::BRIGHT_WHITE);
            gotoxy(52, 20); cout << "Choice: ";
            char wChoice = getch();
            if (wChoice == '1') addToWishlist();
            else if (wChoice == '2') viewWishlist();
        }
        else if (choice=='7') showRecommendations();
        else if (choice=='0') {  return; }
        else if (choice==27) exitConfirmation();
    }
}

/**
 * @brief Prompts for customer credentials, validates them against stored users, and enters the customer portal on success.
 *
 * Prompts the user for a username and a masked password, compares the credentials against the in-memory user list,
 * and on a successful match displays confirmation and transfers control to CustomerMode. On failure it displays an
 * error message and returns to the login/signup menu.
 */
void customerLogin() {
    drawHeader("CUSTOMER LOGIN");
    
    drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_CYAN);
    
    string username, password;
    
    // FIX: Clear input buffer first
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 17); cout << "Username : "; 
    setColor(Color::BRIGHT_WHITE);
    getline(cin, username);  // Changed from cin >> to getline
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 19); cout << "Password : ";
    setColor(Color::BRIGHT_WHITE);
    
    char ch; password="";
    while (1) {
        ch = getch();
        if (ch==13) break;
        if (ch==8) { if (!password.empty()) { password.pop_back(); cout << "\b \b"; } }
        else { password.push_back(ch); cout << '*'; }
    }
    
    Login *temp = first;
    while (temp) {
        if (username == temp->lusername && password == temp->lpassword) {
            setColor(Color::BRIGHT_GREEN);
            gotoxy(52, 21); cout << "Login Successful!";
            Sleep(500);
            loadingBar();
            CustomerMode(); 
            return;
        }
        temp = temp->linker;
    }
    
    setColor(Color::BRIGHT_RED);
    gotoxy(48, 21); cout << "Wrong Username or Password!";
    Sleep(1000);
    loginOrSign();
}



/**
 * @brief Prompts for and registers a new customer account, then proceeds to the customer login flow.
 *
 * Collects full name, phone, username, and password from the user, persists the new account to storage,
 * appends the account to the in-memory customer linked list, displays a success message, and then
 * transfers control to the customer login flow.
 *
 * Side effects:
 * - Calls saveNewUserToFile(...) to persist the new account.
 * - Modifies the global customer list head (`first`) and its links.
 * - Calls customerLogin().
 */
void customerSignup() {
    drawHeader("SIGN UP");
    
    drawAnimatedBox(38, 12, 54, 14, Color::BRIGHT_MAGENTA);
    
    Login *newUser = new Login;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(45, 15); cout << "Full Name : "; 
    setColor(Color::BRIGHT_WHITE);
    cin.ignore(); getline(cin, newUser->lname);
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(45, 16); cout << "Phone     : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> newUser->lphone;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(45, 17); cout << "Username  : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> newUser->lusername;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(45, 18); cout << "Password  : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> newUser->lpassword;
    
    saveNewUserToFile(newUser);
    newUser->linker = nullptr;
    if (!first) first = newUser;
    else { Login *t = first; while (t->linker) t = t->linker; t->linker = newUser; }
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(50, 21); cout << "Sign Up Successful!";
    Sleep(1000);
    customerLogin();
}

/**
 * @brief Presents a customer-facing menu to choose between login, sign up, returning to main menu, or exiting.
 *
 * Displays a persistent menu until the caller is returned; user input is handled via single-key presses:
 * - Enter: invoke customerLogin()
 * - '+' (plus) or ASCII 43: invoke customerSignup()
 * - '0': return to the caller (back to main menu)
 * - Esc (ASCII 27): invoke exitConfirmation()
 */
void loginOrSign() {
    char choice;
    while (1) {
        drawHeader("CUSTOMER - LOGIN OR SIGN UP");
        
        drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_CYAN);
        
        setColor(Color::BRIGHT_GREEN);
        gotoxy(52, 17); cout << "[Enter] Login";
        
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(52, 19); cout << "[  +  ] Sign Up";
        
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(50, 21); cout << "[  0  ] Back to Main";
        
        setColor(Color::BRIGHT_WHITE);
        choice = getch();
        
        if (choice==13) customerLogin();
        else if (choice==43 || choice=='+') customerSignup();
        else if (choice=='0') return;
        else if (choice==27) exitConfirmation();
    }
}

/* ---- SAVE NEW USER TO FILE ---- */
void saveNewUserToFile(Login *temp) {
    ofstream file("l.csv", ios::app);
    if (!file.is_open()) {
        ofstream nf("l.csv");
        nf << "Name,Phone,Username,Password\n";
        nf.close();
        file.open("l.csv", ios::app);
    }
    file << temp->lname << "," << temp->lphone << "," << temp->lusername << "," << temp->lpassword << "\n";
    file.close();
}

/**
 * @brief Loads user accounts from the persistent CSV file into the in-memory linked list.
 *
 * Reads "l.csv" (expected header: "Name,Phone,Username,Password"), clears any existing in-memory
 * user list, and populates the global `first` pointer with Login nodes created from each CSV row.
 * If the file does not exist, it is created with the required header and no users are loaded.
 *
 * Empty lines in the file are ignored.
 */
void loadUsersFromFile() {
    while (first) {
        Login *d = first;
        first = first->linker;
        delete d;
    }
    ifstream file("l.csv");
    if (!file.is_open()) {
        ofstream nf("l.csv");
        nf << "Name,Phone,Username,Password\n";
        nf.close();
        return;
    }
    string line; bool firstLine=true;
    while (getline(file,line)) {
        if (firstLine) { firstLine=false; continue; }
        if (line.empty()) continue;
        istringstream ss(line);
        string sname, sphone, susername, spassword;
        getline(ss,sname,','); getline(ss,sphone,','); getline(ss,susername,','); getline(ss,spassword);
        Login *u = new Login(sname,sphone,susername,spassword);
        if (!first) first = u;
        else { Login *t=first; while (t->linker) t=t->linker; t->linker = u; }
    }
    file.close();
}

/**
 * @brief Presents the Manager Portal UI and handles manager actions until exit.
 *
 * @details Displays an interactive menu that lets a manager add, view, modify,
 * delete and search products; view customer lists; sort or reverse the product
 * list; view top sellers and product statistics; filter low-stock items; and
 * navigate back to the main menu or exit the application. The function runs an
 * input-driven loop and dispatches the selected manager operations until the
 * user returns to the main menu or confirms exit.
 */
void ManagerMode() {
    char choice;
    while (1) {
        drawHeader("MANAGER PORTAL");
        
        drawAnimatedBox(36, 10, 58, 24, Color::BRIGHT_MAGENTA);
        
        setColor(Color::BRIGHT_GREEN);
        gotoxy(48, 12); cout << "1.  Add New Product";
        setColor(Color::BRIGHT_CYAN);
        gotoxy(48, 13); cout << "2.  Display All Products";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(48, 14); cout << "3.  Modify Product";
        setColor(Color::BRIGHT_RED);
        gotoxy(48, 15); cout << "4.  Delete Product";
        setColor(Color::BRIGHT_BLUE);
        gotoxy(48, 16); cout << "5.  Customer List";
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(48, 17); cout << "6.  Sort by Name";
        setColor(Color::BRIGHT_CYAN);
        gotoxy(48, 18); cout << "7.  Sort by Price";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(48, 19); cout << "8.  Top Sellers";
        setColor(Color::BRIGHT_GREEN);
        gotoxy(48, 20); cout << "9.  Search Product";
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(48, 21); cout << "10. Reverse Product List";
        setColor(Color::BRIGHT_CYAN);
        gotoxy(48, 22); cout << "11. Product Statistics";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(48, 23); cout << "12. Low Stock Alerts";
        
        setColor(Color::BRIGHT_WHITE);
        gotoxy(48, 26); cout << "0.  Back to Main Menu";
        
        gotoxy(50, 30); cout << "Enter Key: ";
        choice = getch();
        
        
        if (choice=='1') addProduct();
        else if (choice=='2') { displayProduct(); gotoxy(50,26); getch(); }
        else if (choice=='3') modifyProduct();
        else if (choice=='4') deleteProduct();
        else if (choice=='5') customerListDisplay();
        else if (choice=='6') { sortProductsByName(); showSuccessToast("Sorted by name!", 60, 25); gotoxy(50,26); getch(); }
        else if (choice=='7') { sortProductsByPrice(); showSuccessToast("Sorted by price!", 60, 25); gotoxy(50,26); getch(); }
        else if (choice=='8') displayTopSellers(5);
        else if (choice=='9') searchProductUI();
        else if (choice=='a' || choice=='A') reverseProductList();
        else if (choice=='b' || choice=='B') showProductStatistics();
        else if (choice=='c' || choice=='C') showLowStockAlerts();
        else if (choice=='0') { return; }
        else if (choice==27) exitConfirmation();
        
        // Extended keys for 10-12
        if (choice=='1') {
            char next = getch();
            if (next=='0') reverseProductList();
            else if (next=='1') showProductStatistics();
            else if (next=='2') showLowStockAlerts();
        }
    }
}

/**
 * @brief Presents a manager login prompt, validates credentials, and transfers control to the manager console.
 *
 * Displays a framed login UI, reads the manager username and a masked password, and compares them to the hard-coded
 * credentials. On successful authentication it shows a success message, runs the loading sequence, and enters
 * ManagerMode. On authentication failure it shows an error message and re-displays the login prompt.
 */
void managerLogin() {
    drawHeader("MANAGER LOGIN");
    
    drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_MAGENTA);
    
    string username, password;
    
    // FIX: Clear input buffer first
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 17); cout << "Username : "; 
    setColor(Color::BRIGHT_WHITE);
    getline(cin, username);  // Changed from cin >> to getline
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 19); cout << "Password : ";
    setColor(Color::BRIGHT_WHITE);
    
    char ch; password="";
    while (1) {
        ch = getch();
        if (ch==13) break;
        if (ch==8) { if (!password.empty()) { password.pop_back(); cout << "\b \b"; } }
        else { password.push_back(ch); cout << '*'; }
    }
    
    if (username=="admin" && password=="123") {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(52, 21); cout << "Login Successful!";
        Sleep(500);
        loadingBar();
        ManagerMode();
    } else {
        setColor(Color::BRIGHT_RED);
        gotoxy(50, 21); cout << "Wrong credentials!";
        Sleep(1000);
        managerLogin();
    }
}






/**
 * @brief Initializes the application, loads persisted data, configures the console UI, and runs the main interactive menu loop.
 *
 * The function sets console colors and visibility, loads users, products, and customers from their files, displays the title and credits screens, and then enters a persistent menu loop that dispatches to the Manager, Customer, and Cashier portals or triggers the exit confirmation when ESC is pressed.
 *
 * @return int Exit code (0 on normal termination).
 */
int main() {
    system("color 17");
    
    maximizeConsole();
    hideCursor();
    
    loadUsersFromFile();
    loadProductFromFile();
    loadCustomerFromFile();

    printTitle();
    
    showCredits();
    
    showCursor();

    char choice;
    while (1) {
        drawHeader("MAIN MENU");
        
        drawAnimatedBox(42, 14, 46, 12, Color::BRIGHT_CYAN);
        
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(54, 17); cout << "1. Manager Portal";
        setColor(Color::BRIGHT_CYAN);
        gotoxy(54, 19); cout << "2. Customer Portal";
        setColor(Color::BRIGHT_GREEN);
        gotoxy(54, 21); cout << "3. Cashier Portal";
        
        setColor(Color::BRIGHT_RED);
        gotoxy(52, 24); cout << "Press ESC to Exit";
        
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(54, 26); cout << "Enter choice: ";
        
        choice = getch();
        
        if (choice=='1') managerLogin();
        else if (choice=='2') loginOrSign();
        else if (choice=='3') cashierLogin();
        else if (choice==27) exitConfirmation();
        else {
            showErrorToast("Invalid choice! Try again...", 60, 28);
            
        }
    }
}