#include "customerCart.h"
#include "animation.h"
#include <conio.h>
#include <fstream>
#include <ctime>
#include <iomanip>

const int totalCart = 50;

/* ----- Cart Allocation ----- */
void cCart() {
    drawHeader("Grab a Cart");
    int totalCustomer = countCustomerQueue();
    int remainCart = totalCart - totalCustomer;
    gotoxy(35, 15); cout << "Please Grab a Cart \\_/";
    if (remainCart <= 0) gotoxy(35, 17), cout << "Cart Not Available Now";
    else gotoxy(35, 17), cout << "Your Cart Number is : " << remainCart;
    gotoxy(30, 23); getch();
}

void displayCart() {
    drawHeader("Display Carts");
    int totalCustomer = countCustomerQueue();
    int remainCart = totalCart - totalCustomer;
    if (remainCart <= 0) gotoxy(35, 14), cout << "No carts available";
    else {
        gotoxy(25, 11); cout << "Existing carts are : ";
        int x = 25, y = 13, perRow = 10;
        for (int i = 0; i < remainCart; ++i) {
            gotoxy(x + (i % perRow) * 6, y + (i / perRow));
            cout << "[" << (i + 1) << "]";
        }
    }
    gotoxy(30, 23); getch();
}

/* ----- Product Search ----- */
void searchProductUI() {
    drawHeader("Search Product");
    gotoxy(20, 11); cout << "Enter Product name or partial: ";
    cin.ignore(); string key; getline(cin, key);
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    Product* t = head; bool found = false;
    gotoxy(12, 13); cout << left << setw(12) << "Product ID" << setw(30) << "Product Name" << setw(10) << "Price" << setw(10) << "Qty" << endl;
    gotoxy(12, 14); cout << string(70, '=') << endl;
    int y = 15;
    while (t) {
        string n = t->proName; string ln = n; transform(ln.begin(), ln.end(), ln.begin(), ::tolower);
        if (ln.find(key) != string::npos) {
            gotoxy(12, y++); cout << left << setw(12) << t->proId << setw(30) << t->proName << setw(10) << t->proPrice << setw(10) << t->proNum << endl;
            found = true;
        }
        t = t->next;
    }
    if (!found) gotoxy(30, 16), cout << "Product Not Found.";
    gotoxy(30, 23); getch();
}

/* ----- Receipt Writer (now includes payment method) ----- */
static void writeReceipt(const string &custName, const vector<pair<string, int>> &items,
                         double total, double finalPay, const string &paymentMethod) {
    auto t = time(nullptr); auto tm = *localtime(&t);
    char buf[64]; strftime(buf, sizeof(buf), "receipt_%Y%m%d_%H%M%S.txt", &tm);
    ofstream f(buf);
    if (!f.is_open()) return;
    f << "=== Shopping Mall Receipt ===\n";
    f << "Customer: " << custName << "\n";
    f << "Date: " << asctime(&tm);
    f << left << setw(5) << "No" << setw(30) << "Product" << setw(10) << "Qty" << setw(12) << "Price\n";
    f << string(60, '-') << "\n";
    int i = 1;
    for (auto &it : items) f << left << setw(5) << i++ << setw(30) << it.first << setw(10) << it.second << "\n";
    f << string(60, '-') << "\n";
    f << "Total: " << total << "\n";
    f << "Payable: " << finalPay << "\n";
    f << "Paid via: " << paymentMethod << "\n\nThank you!\n";
    f.close();
}

/* ----- BUY PRODUCTS (payment method added) ----- */
void buyProductUI() {
    drawHeader("Buy Products");
    if (!head) { gotoxy(30, 14); cout << "There is no Product to Buy"; gotoxy(30, 23); getch(); return; }

    int number;
    gotoxy(20, 11); cout << "How many different products you want to buy: "; cin >> number;
    int total = countProduct();
    if (number <= 0 || number > total) { gotoxy(25, 13); cout << "Invalid number"; gotoxy(30, 23); getch(); return; }

    vector<pair<string, int>> items; double bill = 0.0;
    for (int i = 0; i < number; ++i) {
        int id; gotoxy(20, 13 + i * 2); cout << "Enter product ID: "; cin >> id;
        Product* t = head; while (t && t->proId != id) t = t->next;
        if (!t) { gotoxy(20, 14 + i * 2); cout << "Product ID not found. Skipping."; continue; }
        int qty; gotoxy(45, 13 + i * 2); cout << "Qty: "; cin >> qty;
        if (qty > t->proNum) { gotoxy(20, 14 + i * 2); cout << "Requested qty not available. Skipping."; continue; }
        t->proNum -= qty; t->soldCount += qty;
        items.push_back({t->proName, qty});
        bill += t->proPrice * qty;
    }

    // ----- Customer info -----
    string customerName, customerPhone; cin.ignore();
    gotoxy(20, 18 + number * 2); cout << "Enter Your name: "; getline(cin, customerName);
    gotoxy(20, 19 + number * 2); cout << "Enter Phone No.: "; cin >> customerPhone;

    // ----- Billing -----
    double totalBill = bill;
    double discount = (bill > 1000) ? 0.10 : 0.0;
    double finalPay = bill * (1.0 - discount);

    // ----- Add to queue + CSV (immediately) -----
    Customer c{customerName, customerPhone, finalPay};
    enqueueCustomer(c);                 // writes to c.csv right now
    saveAllProductToFile();             // update product stock

    // ----- Payment method -----
    drawHeader("Payment Method");
    gotoxy(25, 11);  cout << "Select payment method for " << customerName << ":";
    gotoxy(25, 13); cout << "1. Easypaisa / JazzCash / Nayapay";
    gotoxy(25, 14); cout << "2. Credit/Debit Card";
    gotoxy(25, 15); cout << "3. Cash Payment";
    gotoxy(25, 17); cout << "Choice: ";
    char ch = getch();
    string method = "Unknown";
    if (ch == '1') method = "Easypaisa/JazzCash/Nayapay";
    else if (ch == '2') method = "Card";
    else if (ch == '3') method = "Cash";

    // ----- Receipt -----
    writeReceipt(customerName, items, totalBill, finalPay, method);

    // ----- Confirmation screen -----
    drawHeader("Payment Confirmation");
    gotoxy(25, 11); cout << "You have bought:";
    for (size_t i = 0; i < items.size(); ++i) {
        gotoxy(25, 13 + i); cout << i + 1 << " | " << items[i].first << " (" << items[i].second << ")";
    }
    gotoxy(25, 15 + items.size()); cout << "Total sum: " << totalBill;
    if (discount > 0) gotoxy(25, 16 + items.size()), cout << "Discount applied: " << (int)(discount * 100) << "%";
    gotoxy(25, 17 + items.size()); cout << "Payable: " << finalPay;
    gotoxy(25, 18 + items.size()); cout << "Paid via: " << method;
    gotoxy(30, 22 + items.size()); cout << "Thank you for shopping. Receipt saved.";
    gotoxy(30, 23); getch();
}