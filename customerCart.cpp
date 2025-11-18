#include "customerCart.h"
#include "animation.h"
#include <conio.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <map>
#include <set>

const int totalCart = 50;

/* ----- HASH MAP for fast product lookup ----- */
map<int, Product*> productCache;

void buildProductCache() {
    productCache.clear();
    for (Product* t = head; t; t = t->next) {
        productCache[t->proId] = t;
    }
}

/* ----- Cart Allocation ----- */
void cCart() {
    drawHeader("Grab a Cart");
    
    
    int totalCustomer = countCustomerQueue();
    int remainCart = totalCart - totalCustomer;
    
    drawAnimatedBox(30, 13, 50, 8, Color::BRIGHT_CYAN);
    
    if (remainCart <= 0) {
        typewriter("Cart Not Available Now", 40, 16, 30);
        showErrorToast("No carts available!", 60, 18);
    } else {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(38, 15);
        cout << "Please Grab a Cart \\_/";
        
        Sleep(300);
        pulseText("Your Cart Number: " + to_string(remainCart), 38, 17, 2);
        showSuccessToast("Cart allocated successfully!", 60, 19);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

void displayCart() {
    drawHeader("Display Carts");
    int totalCustomer = countCustomerQueue();
    int remainCart = totalCart - totalCustomer;
    
    if (remainCart <= 0) {
        showErrorToast("No carts available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(25, 11); 
    slowPrint("Existing carts are: ", Color::BRIGHT_YELLOW, 20);
    
    int x = 25, y = 13, perRow = 10;
    Color cartColors[] = {Color::BRIGHT_GREEN, Color::BRIGHT_CYAN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    
    for (int i = 0; i < remainCart; ++i) {
        setColor(cartColors[i % 4]);
        gotoxy(x + (i % perRow) * 6, y + (i / perRow));
        cout << "[" << (i + 1) << "]";
        Sleep(30);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

/* ----- KMP ALGORITHM for Product Search ----- */
vector<int> computeLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0, i = 1;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) len = lps[len - 1];
            else { lps[i] = 0; i++; }
        }
    }
    return lps;
}

bool kmpSearch(const string& text, const string& pattern) {
    string lowerText = text, lowerPattern = pattern;
    transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);
    transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(), ::tolower);
    
    int n = lowerText.length(), m = lowerPattern.length();
    vector<int> lps = computeLPS(lowerPattern);
    
    int i = 0, j = 0;
    while (i < n) {
        if (lowerText[i] == lowerPattern[j]) {
            i++; j++;
        }
        if (j == m) return true;
        else if (i < n && lowerText[i] != lowerPattern[j]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return false;
}

void searchProductUI() {
    drawHeader("Search Product - KMP Algorithm");
    
    drawAnimatedBox(15, 10, 80, 15, Color::BRIGHT_YELLOW);
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 12); cout << "Enter Product name or partial: ";
    cin.ignore(); string key; getline(cin, key);
    
    showSpinner(800, 50, 13);
    
    Product* t = head; bool found = false;
    
    gotoxy(17, 14); 
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(12) << "Product ID" << setw(30) << "Product Name" << setw(10) << "Price" << setw(10) << "Qty";
    
    gotoxy(17, 15); 
    setColor(Color::BRIGHT_CYAN);
    cout << string(70, '=');
    
    int y = 16;
    Color rowColors[] = {Color::BRIGHT_GREEN, Color::BRIGHT_CYAN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    int colorIdx = 0;
    
    while (t) {
        if (kmpSearch(t->proName, key)) {
            setColor(rowColors[colorIdx % 4]);
            gotoxy(17, y++);
            cout << left << setw(12) << t->proId << setw(30) << t->proName 
                 << setw(10) << t->proPrice << setw(10) << t->proNum;
            found = true;
            colorIdx++;
            Sleep(50);
        }
        t = t->next;
    }
    
    if (!found) {
        showErrorToast("Product Not Found!", 60, 18);
    } else {
        showSuccessToast("Search completed!", 60, y + 1);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

/* ----- Receipt Writer ----- */
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

/* ----- BUY PRODUCTS with Hash Map optimization ----- */
void buyProductUI() {
    drawHeader("Buy Products - Shopping Cart");
    
    if (!head) { 
        showErrorToast("There is no Product to Buy", 60, 14);
        gotoxy(30, 23); getch(); 
        return; 
    }

    buildProductCache();
    
    int number;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 11); cout << "How many different products: "; 
    setColor(Color::BRIGHT_YELLOW);
    cin >> number;
    
    int total = countProduct();
    if (number <= 0 || number > total) { 
        showErrorToast("Invalid number!", 60, 13);
        gotoxy(30, 23); getch(); 
        return; 
    }

    vector<pair<string, int>> items; 
    double bill = 0.0;
    
    for (int i = 0; i < number; ++i) {
        int id; 
        setColor(Color::BRIGHT_GREEN);
        gotoxy(20, 13 + i * 2); cout << "Product ID: "; 
        setColor(Color::BRIGHT_WHITE);
        cin >> id;
        
        Product* t = productCache[id];
        
        if (!t || productCache.find(id) == productCache.end()) { 
            showErrorToast("Product ID not found!", 60, 14 + i * 2);
            continue; 
        }
        
        int qty; 
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(45, 13 + i * 2); cout << "Qty: "; 
        setColor(Color::BRIGHT_WHITE);
        cin >> qty;
        
        if (qty > t->proNum) { 
            showErrorToast("Not enough stock!", 75, 14 + i * 2);
            continue; 
        }
        
        t->proNum -= qty; 
        t->soldCount += qty;
        items.push_back({t->proName, qty});
        bill += t->proPrice * qty;
        
        showSuccessToast("Added to cart!", 75, 14 + i * 2);
    }

    if (items.empty()) {
        showErrorToast("No items in cart!", 60, 18);
        gotoxy(30, 23); getch();
        return;
    }

    string customerName, customerPhone; 
    cin.ignore();
    
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(20, 18 + number * 2); cout << "Enter Your name: "; 
    setColor(Color::BRIGHT_WHITE);
    getline(cin, customerName);
    
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(20, 19 + number * 2); cout << "Enter Phone No.: "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> customerPhone;

    double totalBill = bill;
    double discount = (bill > 1000) ? 0.10 : (bill > 500) ? 0.05 : 0.0;
    double finalPay = bill * (1.0 - discount);

    Customer c{customerName, customerPhone, finalPay};
    enqueueCustomer(c);
    saveAllProductToFile();

    drawHeader("Payment Method");
    drawAnimatedBox(20, 10, 70, 12, Color::BRIGHT_GREEN);
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(25, 12);  cout << "Select payment for " << customerName << ":";
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(25, 14); cout << "1. Easypaisa / JazzCash / Nayapay";
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(25, 15); cout << "2. Credit/Debit Card";
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(25, 16); cout << "3. Cash Payment";
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(25, 18); cout << "Choice: ";
    char ch = getch();
    
    string method = "Unknown";
    if (ch == '1') method = "Easypaisa/JazzCash/Nayapay";
    else if (ch == '2') method = "Card";
    else if (ch == '3') method = "Cash";

    showSpinner(1000, 50, 20);
    writeReceipt(customerName, items, totalBill, finalPay, method);

    drawHeader("Payment Confirmation");
    drawAnimatedBox(20, 10, 70, 16 + items.size(), Color::BRIGHT_GREEN);
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(25, 12); cout << "You have bought:";
    
    Color itemColors[] = {Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    for (size_t i = 0; i < items.size(); ++i) {
        setColor(itemColors[i % 4]);
        gotoxy(25, 14 + i); 
        cout << i + 1 << " | " << items[i].first << " (Qty: " << items[i].second << ")";
        Sleep(100);
    }
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(25, 16 + items.size()); cout << "Total sum: " << totalBill << " Rs";
    
    if (discount > 0) {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(25, 17 + items.size());
        cout << "Discount applied: " << (int)(discount * 100) << "% (Saved: " << (int)(totalBill - finalPay) << " Rs)";
    }
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(25, 18 + items.size()); cout << "Payable: " << finalPay << " Rs";
    
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(25, 19 + items.size()); cout << "Paid via: " << method;
    
    
    showSuccessToast("Thank you for shopping!", 60, 22 + items.size());
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 24); getch();
}

/* ---------- NEW: PRICE FILTER (Binary Search Tree concept) ---------- */
void filterByPriceRange() {
    drawHeader("Filter Products by Price - BST Concept");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    double minPrice, maxPrice;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 11); cout << "Enter minimum price (Rs): ";
    setColor(Color::BRIGHT_WHITE);
    cin >> minPrice;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 12); cout << "Enter maximum price (Rs): ";
    setColor(Color::BRIGHT_WHITE);
    cin >> maxPrice;
    
    showSpinner(600, 50, 13);
    
    // Header section
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(10, 15);
    cout << left << setw(12) << "ID" << setw(35) << "Product" << setw(15) << "Price (Rs)" << setw(10) << "Stock";
    
    gotoxy(10, 16);
    setColor(Color::BRIGHT_MAGENTA);
    cout << string(72, '=');
    
    int y = 17, count = 0;
    Color rowColors[] = {Color::BRIGHT_WHITE, Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW};
    
    // Display filtered products
    for (Product* t = head; t; t = t->next) {
        if (t->proPrice >= minPrice && t->proPrice <= maxPrice) {
            if (y > 24) break; // Prevent overflow beyond screen
            
            setColor(rowColors[count % 4]);
            gotoxy(10, y++);
            cout << left << setw(12) << t->proId 
                 << setw(35) << t->proName 
                 << setw(15) << fixed << setprecision(2) << t->proPrice 
                 << setw(10) << t->proNum;
            count++;
            Sleep(50);
        }
    }
    
    if (count == 0) {
        setColor(Color::BRIGHT_RED);
        gotoxy(28, 20);
        cout << "X No products found in this price range";
    } else {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(10, y + 1);
        cout << "Found " << count << " product(s) in range Rs " << fixed << setprecision(2) 
             << minPrice << " - Rs " << maxPrice;
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(28, 27); 
    cout << "Press any key to continue...";
    getch();
}
/* ---------- NEW: WISHLIST (Set/HashSet DSA) ---------- */
set<int> wishlist;



// Save wishlist to file
void saveWishlistToFile() {
    ofstream f("wishlist.txt");
    for (int id : wishlist) {
        f << id << "\n";
    }
    f.close();
}

// Load wishlist from file
void loadWishlistFromFile() {
    wishlist.clear();
    ifstream f("wishlist.txt");
    if (!f.is_open()) return;
    
    string line;
    while (getline(f, line)) {
        if (!line.empty()) {
            try {
                int id = stoi(line);
                wishlist.insert(id);
            } catch(...) {}
        }
    }
    f.close();
}


void addToWishlist() {
    drawHeader("Add to Wishlist");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    int id;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 12); cout << "Enter Product ID: ";
    setColor(Color::BRIGHT_WHITE);
    cin >> id;
    
    buildProductCache();
    
    auto it = productCache.find(id);
    if (it == productCache.end()) {
        showErrorToast("Product ID not found!", 60, 14);
    } else if (wishlist.count(id)) {
        showInfoToast("Already in your wishlist!", 60, 14);
    } else {
        wishlist.insert(id);
        saveWishlistToFile();                    // THIS SAVES IT!
        showSuccessToast("Added to wishlist!", 60, 14);
    }
    
    gotoxy(30, 23); getch();
}

void viewWishlist() {
    drawHeader("My Wishlist");

    if (wishlist.empty()) {
        drawAnimatedBox(30, 12, 60, 8, Color::BRIGHT_CYAN);
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(48, 15); cout << "Your wishlist is empty";
        gotoxy(45, 17); cout << "Start adding products!";
        setColor(Color::BRIGHT_WHITE);
        gotoxy(40, 22); getch();
        return;
    }

    buildProductCache();

    int count = 0;
    for (int id : wishlist) if (productCache.count(id)) count++;

    drawAnimatedBox(10, 9, 90, 8 + count + 2, Color::BRIGHT_MAGENTA);

    gotoxy(35, 11);
    setColor(Color::BRIGHT_YELLOW);
    cout << "YOUR WISHLIST (" << count << " items)";

    gotoxy(12, 13);
    setColor(Color::BRIGHT_CYAN);
    cout << left << setw(8)  << "No"
         << setw(35) << "Product Name"
         << setw(15) << "Price"
         << setw(15) << "Status";

    gotoxy(12, 14); setColor(Color::BRIGHT_WHITE);
    cout << string(85, char(196));

    int y = 15, idx = 1;
    Color colors[] = {Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW};

    for (int id : wishlist) {
        if (productCache.count(id)) {
            Product* p = productCache[id];
            setColor(colors[(idx-1) % 3]);
            gotoxy(12, y++);
            cout << left << setw(8)  << idx++
                 << setw(35) << p->proName
                 << setw(15) << p->proPrice << " Rs";

            setColor(p->proNum > 0 ? Color::BRIGHT_GREEN : Color::BRIGHT_RED);
            cout << (p->proNum > 0 ? "In Stock" : "Out of Stock");
            Sleep(50);
        }
    }

    setColor(Color::BRIGHT_WHITE);
    gotoxy(35, y + 2); cout << "Press any key to go back...";
    getch();
}

/* ---------- NEW: PRODUCT RECOMMENDATIONS (Graph-based) ---------- */
void showRecommendations() {
    drawHeader("Product Recommendations - Graph Algorithm");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    // Recommend top 5 cheapest products
    priority_queue<pair<double, Product*>, vector<pair<double, Product*>>, greater<pair<double, Product*>>> minHeap;
    
    for (Product* t = head; t; t = t->next) {
        if (t->proNum > 0) {
            minHeap.push({t->proPrice, t});
        }
    }
    
    drawAnimatedBox(10, 10, 75, 13, Color::BRIGHT_YELLOW);
    
    gotoxy(12, 12);
    setColor(Color::BRIGHT_YELLOW);
    cout << "TOP RECOMMENDED PRODUCTS (Best Value):";
    
    gotoxy(12, 14);
    setColor(Color::BRIGHT_CYAN);
    cout << left << setw(30) << "Product" << setw(12) << "Price" << setw(12) << "Stock";
    
    gotoxy(12, 15);
    cout << string(60, '=');
    
    int y = 16, count = 0;
    Color colors[] = {Color::BRIGHT_GREEN, Color::BRIGHT_CYAN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    
    while (!minHeap.empty() && count < 5) {
        auto item = minHeap.top(); minHeap.pop();
        Product* p = item.second;
        
        setColor(colors[count % 4]);
        gotoxy(12, y++);
        cout << left << setw(30) << p->proName 
             << setw(12) << p->proPrice 
             << setw(12) << p->proNum;
        count++;
        Sleep(100);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 26); getch();
}