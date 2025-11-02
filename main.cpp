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

/* ---------- ASCII ART TITLE (CENTERED) ---------- */
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
}

/* ---------- Credits (CENTERED) ---------- */
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

/* ---- Exit (CENTERED) ---- */
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

/* ---- CASHIER MODE (CENTERED) ---- */
void CashierMode() {
    char choice;
    while (1) {
        drawHeader("CASHIER PORTAL");
        
        drawAnimatedBox(40, 13, 48, 12, Color::BRIGHT_GREEN);
        
        setColor(Color::BRIGHT_CYAN);
        gotoxy(52, 15); cout << "1. Customer List";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(52, 16); cout << "2. Customer Check Out";
        setColor(Color::BRIGHT_GREEN);
        gotoxy(52, 17); cout << "3. View Total Revenue";
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(52, 19); cout << "0. Back to Main Menu";
        
        setColor(Color::BRIGHT_WHITE);
        gotoxy(52, 22); cout << "Enter Key: ";
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
        else if (choice=='0') return;
        else if (choice==27) exitConfirmation();
    }
}

/* ---- CASHIER LOGIN (CENTERED) ---- */
void cashierLogin() {
    drawHeader("CASHIER LOGIN");
    
    drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_GREEN);
    
    string username, password;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 17); cout << "Username : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> username;
    
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

/* ---- CUSTOMER MODE (CENTERED) ---- */
void CustomerMode() {
    char choice;
    while (1) {
        drawHeader("CUSTOMER PORTAL");
        
        drawAnimatedBox(40, 13, 48, 14, Color::BRIGHT_CYAN);
        
        setColor(Color::BRIGHT_CYAN);
        gotoxy(50, 15); cout << "1. Display All Products";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(50, 16); cout << "2. Search Products";
        setColor(Color::BRIGHT_GREEN);
        gotoxy(50, 17); cout << "3. Buy Products";
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(50, 18); cout << "4. Display Cart";
        setColor(Color::BRIGHT_RED);
        gotoxy(50, 20); cout << "0. Back to Main Menu";
        
        setColor(Color::BRIGHT_WHITE);
        gotoxy(52, 24); cout << "Enter Key: ";
        choice = getch();
        
        if (choice=='1') { displayProduct(); gotoxy(50,26); getch(); }
        else if (choice=='2') searchProductUI();
        else if (choice=='3') { cCart(); buyProductUI(); }
        else if (choice=='4') displayCart();
        else if (choice=='0') return;
        else if (choice==27) exitConfirmation();
    }
}

/* ---- CUSTOMER LOGIN (CENTERED) ---- */
void customerLogin() {
    drawHeader("CUSTOMER LOGIN");
    
    drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_CYAN);
    
    string username, password;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 17); cout << "Username : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> username;
    
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

/* ---- CUSTOMER SIGNUP (CENTERED) ---- */
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

/* ---- LOGIN OR SIGNUP (CENTERED) ---- */
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

/* ---- LOAD USERS FROM FILE ---- */
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

/* ---- MANAGER MODE (CENTERED) ---- */
void ManagerMode() {
    char choice;
    while (1) {
        drawHeader("MANAGER PORTAL");
        
        drawAnimatedBox(38, 12, 54, 16, Color::BRIGHT_MAGENTA);
        
        setColor(Color::BRIGHT_GREEN);
        gotoxy(50, 14); cout << "1. Add New Product";
        setColor(Color::BRIGHT_CYAN);
        gotoxy(50, 15); cout << "2. Display All Products";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(50, 16); cout << "3. Modify Product";
        setColor(Color::BRIGHT_RED);
        gotoxy(50, 17); cout << "4. Delete Product";
        setColor(Color::BRIGHT_BLUE);
        gotoxy(50, 18); cout << "5. Customer List";
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(50, 19); cout << "6. Sort by Name";
        setColor(Color::BRIGHT_CYAN);
        gotoxy(50, 20); cout << "7. Sort by Price";
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(50, 21); cout << "8. Top Sellers";
        setColor(Color::BRIGHT_WHITE);
        gotoxy(50, 23); cout << "0. Back to Main Menu";
        
        gotoxy(52, 26); cout << "Enter Key: ";
        choice = getch();
        
        if (choice=='1') addProduct();
        else if (choice=='2') { displayProduct(); gotoxy(50,26); getch(); }
        else if (choice=='3') modifyProduct();
        else if (choice=='4') deleteProduct();
        else if (choice=='5') customerListDisplay();
        else if (choice=='6') { sortProductsByName(); gotoxy(45,25); setColor(Color::BRIGHT_GREEN); cout << "Sorted by name!"; gotoxy(50,26); getch(); }
        else if (choice=='7') { sortProductsByPrice(); gotoxy(45,25); setColor(Color::BRIGHT_GREEN); cout << "Sorted by price!"; gotoxy(50,26); getch(); }
        else if (choice=='8') displayTopSellers(5);
        else if (choice=='0') return;
        else if (choice==27) exitConfirmation();
    }
}

/* ---- MANAGER LOGIN (CENTERED) ---- */
void managerLogin() {
    drawHeader("MANAGER LOGIN");
    
    drawAnimatedBox(40, 14, 50, 10, Color::BRIGHT_MAGENTA);
    
    string username, password;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(48, 17); cout << "Username : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> username;
    
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

/* ---- MAIN (WITH BETTER COLORS) ---- */
int main() {
    // ========== CHOOSE YOUR COLOR SCHEME ==========
    
    // 🌊 OPTION 1: Ocean Theme - Dark Blue bg + Bright Cyan text (Currently Active)
    system("color 1B");
    
    // 🌲 OPTION 2: Forest Theme - Dark Green bg + Bright Yellow text
    // system("color 2E");
    
    // 🌙 OPTION 3: Night Theme - Black bg + Bright Cyan text  
    // system("color 0B");
    
    // 💎 OPTION 4: Matrix Theme - Black bg + Bright Green text
    // system("color 0A");
    
    // 🔥 OPTION 5: Fire Theme - Dark Red bg + Bright Yellow text
    // system("color 4E");
    
    // 👑 OPTION 6: Royal Theme - Purple bg + Bright White text
    // system("color 5F");
    
    // ☁️ OPTION 7: Sky Theme - Blue bg + Bright White text
    // system("color 1F");
    
    // 🌺 OPTION 8: Sunset Theme - Dark Magenta bg + Bright Yellow text
    // system("color 5E");
    
    // 🍃 OPTION 9: Fresh Theme - Green bg + Bright White text
    // system("color 2F");
    
    // Maximize window for better display
    maximizeConsole();
    
    loadUsersFromFile();
    loadProductFromFile();
    loadCustomerFromFile();

    printTitle();
    Sleep(2000);
    showCredits();

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
            setColor(Color::BRIGHT_RED);
            gotoxy(52, 28); cout << "Invalid! Try again...";
            Sleep(800);
        }
    }
}