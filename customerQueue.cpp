#include "customerQueue.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <conio.h>

static queue<Customer> q;

int countCustomerQueue() { return (int)q.size(); }

void enqueueCustomer(const Customer &c) {
    q.push(c);
    // Add to queue file (pending customers)
    ofstream f("c.csv", ios::app);
    if (!f.is_open()) {
        ofstream nf("c.csv"); nf << "Name,Phone,Bill\n"; nf.close();
        f.open("c.csv", ios::app);
    }
    f << c.cname << "," << c.cphone << "," << c.cbill << "\n";
    f.close();
}

/* ---- CASHIER CHECK-OUT ---- */
void dequeueCustomer() {
    drawHeader("Cash Counter");
    if (q.empty()) { gotoxy(35, 12); cout << "No Customer in Queue"; gotoxy(30, 23); getch(); return; }

    Customer top = q.front(); q.pop();

    gotoxy(20, 11);  cout << "Customer: " << top.cname;
    gotoxy(20, 12);  cout << "Phone   : " << top.cphone;
    gotoxy(20, 13);  cout << "Bill    : " << fixed << setprecision(2) << top.cbill << " Rs";

    gotoxy(20, 15); cout << "Select payment method:";
    gotoxy(22, 17); cout << "1. Easypaisa / JazzCash / Nayapay";
    gotoxy(22, 18); cout << "2. Credit/Debit Card";
    gotoxy(22, 19); cout << "3. Cash Payment";
    gotoxy(22, 21); cout << "Choice: ";
    char ch = getch();

    string method = "Unknown";
    if (ch == '1') method = "Easypaisa/JazzCash/Nayapay";
    else if (ch == '2') method = "Card";
    else if (ch == '3') method = "Cash";

    gotoxy(20, 23); cout << "Payment successful via " << method;
    gotoxy(20, 24); cout << "Amount paid: " << top.cbill << " Rs";

    // *** SAVE TO SALES HISTORY (never gets deleted) ***
    ofstream salesFile("sales_history.csv", ios::app);
    if (!salesFile.is_open()) {
        ofstream nf("sales_history.csv");
        nf << "Name,Phone,Bill,PaymentMethod\n";
        nf.close();
        salesFile.open("sales_history.csv", ios::app);
    }
    salesFile << top.cname << "," << top.cphone << "," << top.cbill << "," << method << "\n";
    salesFile.close();

    saveAllCustomerToFile();        // rewrite queue file (removes checked-out customer)
    gotoxy(30, 26); cout << "Press any key to continue...";
    getch();
}

/* ---- Customer List ---- */
void customerListDisplay() {
    drawHeader("Customer Queue");
    if (q.empty()) { gotoxy(35, 12); cout << "No Customer in Queue"; gotoxy(30, 23); getch(); return; }
    gotoxy(12, 11);
    cout << left << setw(5) << "SL" << setw(25) << "Customer Name" << setw(18) << "Phone Number" << setw(10) << "Bill" << endl;
    gotoxy(12, 12); cout << string(70, '=') << endl;
    queue<Customer> temp = q;
    int i = 1, y = 13;
    while (!temp.empty()) {
        Customer c = temp.front(); temp.pop();
        gotoxy(12, y++);
        cout << left << setw(5) << i++ << setw(25) << c.cname << setw(18) << c.cphone << setw(10) << c.cbill << endl;
    }
    gotoxy(12, y + 1); getch();
}

/* ---- CSV Persistency ---- */
void saveAllCustomerToFile() {
    ofstream f("c.csv");
    if (!f.is_open()) return;
    f << "Name,Phone,Bill\n";
    queue<Customer> temp = q;
    while (!temp.empty()) {
        Customer c = temp.front(); temp.pop();
        f << c.cname << "," << c.cphone << "," << c.cbill << "\n";
    }
    f.close();
}

void loadCustomerFromFile() {
    ifstream f("c.csv");
    if (!f.is_open()) {
        ofstream nf("c.csv"); nf << "Name,Phone,Bill\n"; nf.close();
        return;
    }
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