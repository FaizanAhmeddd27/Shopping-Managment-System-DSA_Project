#include "customerQueue.h"
#include "animation.h" // Ensure animation.h is included for UI functions
#include <fstream>
#include <iomanip>
#include <sstream>
#include <conio.h>
#include <queue> // Changed to standard queue
#include <map>
#include <algorithm> // For transform

using namespace std; // Using standard namespace for common features

static queue<Customer> q;

int countCustomerQueue() { return (int)q.size(); }

void enqueueCustomer(const Customer &c) {
    q.push(c);
    ofstream f("c.csv", ios::app);
    if (!f.is_open()) {
        ofstream nf("c.csv"); nf << "Name,Phone,Bill\n"; nf.close();
        f.open("c.csv", ios::app);
    }
    f << c.cname << "," << c.cphone << "," << fixed << setprecision(2) << c.cbill << "\n"; // Added setprecision
    f.close();
}

/* ---- CASHIER CHECK-OUT with Enhanced UI (Speed/Clarity Improvements) ---- */
void dequeueCustomer() {
    drawHeader("Cash Counter - Customer Checkout");

    if (q.empty()) {
        showErrorToast("No Customer in Queue", 60, 12);
        gotoxy(30, 23); getch();
        return;
    }

    Customer top = q.front(); q.pop();

    drawAnimatedBox(25, 11, 70, 16, Color::BRIGHT_GREEN);

    setColor(Color::BRIGHT_CYAN);
    gotoxy(30, 13);  cout << "Customer Details:";

    setColor(Color::BRIGHT_YELLOW);
    gotoxy(30, 15);  cout << "Name    : " << top.cname;
    gotoxy(30, 16);  cout << "Phone   : " << top.cphone;

    setColor(Color::BRIGHT_GREEN);
    gotoxy(30, 17);  cout << "Bill    : " << fixed << setprecision(2) << top.cbill << " Rs";

    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(30, 19); cout << "Select payment method (Press 1, 2, or 3):";

    setColor(Color::BRIGHT_GREEN);
    gotoxy(32, 21); cout << "1. Easypaisa / JazzCash / Nayapay";
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(32, 22); cout << "2. Credit/Debit Card";
    setColor(Color::BRIGHT_CYAN);
    gotoxy(32, 23); cout << "3. Cash Payment";

    setColor(Color::BRIGHT_WHITE);
    gotoxy(32, 25); cout << "Choice: ";
    char ch = getch();

    string method = "Unknown";
    if (ch == '1') method = "Easypaisa/JazzCash/Nayapay";
    else if (ch == '2') method = "Card";
    else if (ch == '3') method = "Cash";

    showSpinner(500, 50, 26); // Reduced spinner time

    ofstream salesFile("sales_history.csv", ios::app);
    if (!salesFile.is_open()) {
        ofstream nf("sales_history.csv");
        nf << "Name,Phone,Bill,PaymentMethod\n";
        nf.close();
        salesFile.open("sales_history.csv", ios::app);
    }
    salesFile << top.cname << "," << top.cphone << "," << fixed << setprecision(2) << top.cbill << "," << method << "\n";
    salesFile.close();

    saveAllCustomerToFile();

    system("cls"); // Clear screen for confirmation
    drawHeader("Payment Successful");
    drawAnimatedBox(30, 13, 60, 10, Color::BRIGHT_GREEN);

    setColor(Color::BRIGHT_GREEN);
    gotoxy(40, 16); cout << "Payment successful via " << method;
    gotoxy(40, 18); cout << "Amount paid: " << fixed << setprecision(2) << top.cbill << " Rs";

    setColor(Color::BRIGHT_CYAN);
    gotoxy(40, 20); cout << "Thank you, " << top.cname << "!";

    showSuccessToast("Transaction completed!", 60, 22);

    setColor(Color::BRIGHT_WHITE);
    gotoxy(40, 26); cout << "Press any key to continue...";
    getch();
}

/* ---- Customer List with Enhanced Display (Speed Improvement) ---- */
void customerListDisplay() {
    drawHeader("Customer Queue - FIFO Order");

    if (q.empty()) {
        showErrorToast("No Customer in Queue", 60, 12);
        gotoxy(30, 23); getch();
        return;
    }

    drawAnimatedBox(10, 10, 85, 5 + q.size(), Color::BRIGHT_CYAN);

    gotoxy(12, 11);
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(5) << "SL";
    setColor(Color::BRIGHT_CYAN);
    cout << setw(30) << "Customer Name";
    setColor(Color::BRIGHT_GREEN);
    cout << setw(20) << "Phone Number";
    setColor(Color::BRIGHT_MAGENTA);
    cout << setw(15) << "Bill (Rs)"; // Removed endl

    gotoxy(12, 12);
    setColor(Color::BRIGHT_WHITE);
    cout << string(75, '='); // Removed endl

    queue<Customer> temp = q;
    int i = 1, y = 13;
    Color rowColors[] = {Color::BRIGHT_WHITE, Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW};

    while (!temp.empty()) {
        Customer c = temp.front(); temp.pop();
        setColor(rowColors[i % 4]);
        gotoxy(12, y++);
        cout << left << setw(5) << i++
             << setw(30) << c.cname
             << setw(20) << c.cphone
             << setw(15) << fixed << setprecision(2) << c.cbill; // Removed endl
        Sleep(20); // Reduced sleep for faster display
    }

    setColor(Color::BRIGHT_GREEN);
    gotoxy(12, y + 1);
    cout << "Total Customers Waiting: " << q.size();

    setColor(Color::BRIGHT_WHITE);
    gotoxy(12, y + 3); getch();
}

/* ---- CSV Persistency (Added setprecision to Bill) ---- */
void saveAllCustomerToFile() {
    ofstream f("c.csv");
    if (!f.is_open()) return;
    f << "Name,Phone,Bill\n";
    queue<Customer> temp = q;
    while (!temp.empty()) {
        Customer c = temp.front(); temp.pop();
        f << c.cname << "," << c.cphone << "," << fixed << setprecision(2) << c.cbill << "\n";
    }
    f.close();
}

void loadCustomerFromFile() {
    ifstream f("c.csv");
    if (!f.is_open()) {
        ofstream nf("c.csv"); nf << "Name,Phone,Bill\n"; nf.close();
        return;
    }
    // Clear existing queue before loading
    while (!q.empty()) q.pop();

    string line; bool header = false;
    while (getline(f, line)) {
        if (!header) { header = true; continue; }
        if (line.empty()) continue;
        istringstream ss(line);
        string name, phone, bill;
        if (getline(ss, name, ',') && getline(ss, phone, ',') && getline(ss, bill)) {
            try {
                Customer c{name, phone, stod(bill)};
                q.push(c);
            } catch (...) { continue; }
        }
    }
    f.close();
}

/* ---------- NEW: SALES ANALYTICS (Aggregation DSA) ---------- */

void showSalesAnalytics() {
    drawHeader("Sales Analytics - Data Aggregation");

    ifstream f("sales_history.csv");
    if (!f.is_open()) {
        showErrorToast("No sales history found", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }

    map<string, int> paymentMethodCount;
    map<string, double> paymentMethodRevenue;
    double totalRevenue = 0;
    int totalSales = 0;

    string line; bool header = true;
    while (getline(f, line)) {
        if (header) { header = false; continue; }
        if (line.empty()) continue;

        istringstream ss(line);
        string name, phone, bill, method;
        if (getline(ss, name, ',') && getline(ss, phone, ',') && getline(ss, bill, ',') && getline(ss, method)) {
            try {
                double amount = stod(bill);
                totalRevenue += amount;
                totalSales++;
                paymentMethodCount[method]++;
                paymentMethodRevenue[method] += amount;
            } catch (...) { continue; }
        }
    }
    f.close();

    // Calculate dynamic box height
    int boxHeight = 18 + paymentMethodCount.size();
    drawDynamicBox(15, 10, 80, boxHeight, Color::BRIGHT_CYAN);

    setColor(Color::BRIGHT_YELLOW);
    gotoxy(30, 12); cout << "SALES ANALYTICS DASHBOARD";

    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 14); cout << "Total Sales        : " << totalSales;
    gotoxy(20, 15); cout << "Total Revenue      : " << fixed << setprecision(2) << totalRevenue << " Rs";

    if (totalSales > 0) {
        gotoxy(20, 16); cout << "Average Bill       : " << fixed << setprecision(2) << (totalRevenue / totalSales) << " Rs";
    }

    setColor(Color::BRIGHT_GREEN);
    gotoxy(20, 18); cout << "Payment Method Breakdown:";

    int y = 19;
    Color colors[] = {Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA, Color::BRIGHT_CYAN};
    int idx = 0;

    for (auto& pair : paymentMethodCount) {
        setColor(colors[idx % 3]);
        gotoxy(25, y++);
        cout << left << setw(30) << pair.first
             << " : " << pair.second << " sales, "
             << fixed << setprecision(2) << paymentMethodRevenue[pair.first] << " Rs";
        idx++;
        Sleep(50);
    }

    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, y + 2); getch();
}

/* ---------- NEW: PEAK HOUR ANALYSIS (Time-based Queue) ---------- */
void showPeakHourAnalysis() {
    drawHeader("Peak Hour Analysis - Time Series");

    int queueSize = countCustomerQueue();
    double totalWaitingBill = 0;

    queue<Customer> temp = q;
    while (!temp.empty()) {
        totalWaitingBill += temp.front().cbill;
        temp.pop();
    }

    // Dynamic box size
    drawDynamicBox(20, 11, 75, 16, Color::BRIGHT_YELLOW);

    setColor(Color::BRIGHT_CYAN);
    gotoxy(30, 13); cout << "Current Queue Status:";

    setColor(Color::BRIGHT_GREEN);
    gotoxy(30, 15); cout << "Customers Waiting  : " << queueSize;
    gotoxy(30, 16); cout << "Total Pending      : " << fixed << setprecision(2) << totalWaitingBill << " Rs";

    string status;
    Color statusColor;

    if (queueSize == 0) {
        status = "IDLE - No customers";
        statusColor = Color::BRIGHT_CYAN;
    } else if (queueSize <= 3) {
        status = "NORMAL - Light traffic";
        statusColor = Color::BRIGHT_GREEN;
    } else if (queueSize <= 7) {
        status = "BUSY - Moderate traffic";
        statusColor = Color::BRIGHT_YELLOW;
    } else {
        status = "PEAK HOUR - Heavy traffic!";
        statusColor = Color::BRIGHT_RED;
    }

    setColor(statusColor);
    gotoxy(30, 18); cout << "Status: " << status;

    if (queueSize > 5) {
        setColor(Color::BRIGHT_RED);
        gotoxy(30, 20); cout << "RECOMMENDATION: Open additional counter!";
    }

    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 24); getch();
}



/* ---------- NEW: CUSTOMER SEARCH (Linear Search with Details) ---------- */
void searchCustomerInQueue() {
    drawHeader("Search Customer - Linear Search Algorithm");

    if (q.empty()) {
        showErrorToast("No customers in queue", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }

    string searchName;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 12); cout << "Enter customer name to search: ";
    setColor(Color::BRIGHT_WHITE);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, searchName);

    showSpinner(300, 50, 13); // Reduced spinner time

    queue<Customer> temp = q;
    bool found = false;
    int position = 0, currentPos = 1;

    // Clear screen before showing results
    system("cls");
    drawHeader("Search Customer - Results");
    
    drawAnimatedBox(15, 10, 75, 15, Color::BRIGHT_GREEN); // Larger, fixed box for details

    while (!temp.empty()) {
        Customer c = temp.front(); temp.pop();

        string lowerName = c.cname, lowerSearch = searchName;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

        if (lowerName.find(lowerSearch) != string::npos) {
            position = currentPos; // Record the position of the first match
            found = true;

            // Display the details only for the first match for simplicity
            setColor(Color::BRIGHT_YELLOW);
            gotoxy(20, 12); cout << "CUSTOMER FOUND!";

            setColor(Color::BRIGHT_CYAN);
            gotoxy(20, 14); cout << "Position in Queue : " << position;
            gotoxy(20, 15); cout << "Name              : " << c.cname;
            gotoxy(20, 16); cout << "Phone             : " << c.cphone;

            setColor(Color::BRIGHT_GREEN);
            gotoxy(20, 17); cout << "Bill Amount       : " << fixed << setprecision(2) << c.cbill << " Rs";

            int estimatedWaitTime = position * 3;
            setColor(Color::BRIGHT_MAGENTA);
            gotoxy(20, 19); cout << "Estimated Wait    : ~" << estimatedWaitTime << " minutes";
            
            showSuccessToast("Found: " + c.cname, 60, 21);
            break; // Stop after finding the first match
        }
        currentPos++;
    }

    if (!found) {
        showErrorToast("Customer not found in queue", 60, 12);
    }

    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 24); getch();
}

