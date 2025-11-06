#include "manager.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <cmath>
#include <stack>
#include "animation.h"

Product *head = nullptr;

int countProduct() {
    Product *t = head; int cnt=0;
    while (t) { ++cnt; t = t->next; }
    return cnt;
}

void deleteList() {
    Product *t = head;
    while (t) { Product *d = t; t = t->next; delete d; }
    head = nullptr;
}

/* ---------- BINARY SEARCH (Req: sorted by ID) ---------- */
Product* binarySearchById(int targetId) {
    vector<Product*> arr;
    for (Product* t = head; t; t = t->next) arr.push_back(t);
    
    sort(arr.begin(), arr.end(), [](Product* a, Product* b) { 
        return a->proId < b->proId; 
    });
    
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid]->proId == targetId) return arr[mid];
        if (arr[mid]->proId < targetId) left = mid + 1;
        else right = mid - 1;
    }
    return nullptr;
}

void addProduct() {
    drawHeader("Add New Product");
    
    drawAnimatedBox(15, 10, 75, 12, Color::BRIGHT_GREEN);
    
    Product *n = new Product;
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(20, 12); cout << "Enter Product Info...";
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 14); cout << "Product Name     : "; 
    setColor(Color::BRIGHT_WHITE);
    cin.ignore(); getline(cin, n->proName);
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(20, 15); cout << "Product ID       : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> n->proId;
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(20, 16); cout << "Product Price    : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> n->proPrice;
    
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(20, 17); cout << "Product Quantity : "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> n->proNum;
    
    n->soldCount = 0; n->next = nullptr;
    
    showSpinner(800, 45, 19);
    
    if (!head) head = n;
    else { Product *t=head; while (t->next) t=t->next; t->next = n; }
    
    saveNewProductToFile(n);
    
    showSuccessToast("Product '" + n->proName + "' added!", 60, 20);
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

void displayProduct() {
    drawHeader("All Products");
    
    Product *t = head;
    if (!head) { 
        showErrorToast("No Product is Found", 60, 12);
        getch(); 
        return; 
    }
    
    gotoxy(12, 11);
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(12) << "Product ID";
    setColor(Color::BRIGHT_CYAN);
    cout << setw(30) << "Product Name";
    setColor(Color::BRIGHT_GREEN);
    cout << setw(12) << "Price";
    setColor(Color::BRIGHT_MAGENTA);
    cout << setw(10) << "Quantity";
    setColor(Color::BRIGHT_RED);
    cout << setw(10) << "Sold" << endl;
    
    gotoxy(12, 12); 
    setColor(Color::BRIGHT_CYAN);
    cout << string(80, '=') << endl;
    
    int y = 13;
    Color rowColors[] = {Color::BRIGHT_WHITE, Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW};
    int idx = 0;
    
    while (t) {
        setColor(rowColors[idx % 4]);
        gotoxy(12, y++); 
        cout << left << setw(12) << t->proId 
             << setw(30) << t->proName 
             << setw(12) << t->proPrice 
             << setw(10) << t->proNum 
             << setw(10) << t->soldCount << endl;
        Sleep(30);
        t = t->next;
        idx++;
    }
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(12, y + 1); 
    cout << "Total Products: " << countProduct();
    
    setColor(Color::BRIGHT_WHITE);
}

void modifyProduct() {
    drawHeader("Modify Product");
    
    if (!head) { 
        showErrorToast("Store is Empty", 60, 12);
        getch(); 
        return; 
    }
    
    int id; 
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 11); cout << "Enter Product's ID: "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> id;
    
    showSpinner(600, 45, 12);
    
    Product *t = binarySearchById(id);
    
    if (t) {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(12, 13); cout << "Found:";
        
        drawAnimatedBox(10, 14, 80, 10, Color::BRIGHT_YELLOW);
        
        setColor(Color::BRIGHT_CYAN);
        gotoxy(12, 16); 
        cout << left << setw(12) << t->proId << setw(30) << t->proName 
             << setw(12) << t->proPrice << setw(10) << t->proNum << endl;
        
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(25, 18); cout << "Enter new info:";
        
        setColor(Color::BRIGHT_GREEN);
        gotoxy(30, 19); cout << "New Product ID: "; 
        setColor(Color::BRIGHT_WHITE);
        cin >> t->proId;
        
        setColor(Color::BRIGHT_CYAN);
        gotoxy(30, 20); cout << "New Product Name: "; 
        setColor(Color::BRIGHT_WHITE);
        cin.ignore(); getline(cin, t->proName);
        
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(30, 21); cout << "New Price: "; 
        setColor(Color::BRIGHT_WHITE);
        cin >> t->proPrice;
        
        setColor(Color::BRIGHT_MAGENTA);
        gotoxy(30, 22); cout << "New Quantity: "; 
        setColor(Color::BRIGHT_WHITE);
        cin >> t->proNum;
        
        saveAllProductToFile();
        showSuccessToast("Product modified successfully!", 60, 24);
        
        setColor(Color::BRIGHT_WHITE);
        gotoxy(30, 25); getch();
    } else {
        showErrorToast("No Product Found", 60, 14);
        setColor(Color::BRIGHT_WHITE);
        gotoxy(30, 23); getch();
    }
}

void deleteProduct() {
    drawHeader("Delete Product");
    
    if (!head) { 
        showErrorToast("Store is Empty", 60, 12);
        getch(); 
        return; 
    }
    
    int id; 
    setColor(Color::BRIGHT_RED);
    gotoxy(20, 11); cout << "Enter Product's ID to DELETE: "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> id;
    
    showSpinner(600, 55, 12);
    
    Product *t = head, *prev = nullptr;
    while (t) {
        if (t->proId == id) {
            if (!prev) head = t->next;
            else prev->next = t->next;
            
            showSuccessToast("Product '" + t->proName + "' deleted!", 60, 18);
            
            delete t;
            saveAllProductToFile();
            
            setColor(Color::BRIGHT_WHITE);
            gotoxy(30, 23); getch();
            return;
        }
        prev = t; t = t->next;
    }
    
    showErrorToast("Product ID not found", 60, 14);
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

void saveNewProductToFile(Product *temp) {
    ofstream file("p.csv", ios::app);
    if (!file.is_open()) return;
    file << temp->proId << "," << temp->proName << "," << temp->proPrice << "," << temp->proNum << "," << temp->soldCount << "\n";
    file.close();
}

void saveAllProductToFile() {
    ofstream file("p.csv");
    if (!file.is_open()) return;
    file << "ID,Name,Price,Quantity,Sold\n";
    Product *t = head;
    while (t) {
        file << t->proId << "," << t->proName << "," << t->proPrice << "," << t->proNum << "," << t->soldCount << "\n";
        t = t->next;
    }
    file.close();
}

void loadProductFromFile() {
    deleteList();
    ifstream file("p.csv");
    if (!file.is_open()) {
        ofstream nf("p.csv"); nf << "ID,Name,Price,Quantity,Sold\n"; nf.close();
        return;
    }
    string line; bool first = true;
    while (getline(file, line)) {
        if (first) { first = false; continue; }
        if (line.empty()) continue;
        istringstream ss(line);
        string sid, sname, sprice, snum, ssold;
        getline(ss, sid, ','); getline(ss, sname, ','); getline(ss, sprice, ','); getline(ss, snum, ','); getline(ss, ssold);
        try {
            int iid = stoi(sid);
            double dprice = stod(sprice);
            int inum = stoi(snum);
            int sold = ssold.empty() ? 0 : stoi(ssold);
            Product *n = new Product(iid, sname, dprice, inum);
            n->soldCount = sold;
            if (!head) head = n; else { Product *t = head; while (t->next) t = t->next; t->next = n; }
        } catch (...) { continue; }
    }
    file.close();
}

/* ---------- MERGE SORT ALGORITHM ---------- */
Product* midNode(Product* h) {
    if (!h) return nullptr;
    Product* slow = h; Product* fast = h->next;
    while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
    return slow;
}

Product* mergeSorted(Product* a, Product* b, bool sortByPrice) {
    if (!a) return b;
    if (!b) return a;
    Product* result = nullptr;
    bool takeA;
    if (sortByPrice) takeA = a->proPrice <= b->proPrice;
    else {
        string A = a->proName, B = b->proName;
        transform(A.begin(), A.end(), A.begin(), ::tolower);
        transform(B.begin(), B.end(), B.begin(), ::tolower);
        takeA = A <= B;
    }
    if (takeA) { result = a; result->next = mergeSorted(a->next, b, sortByPrice); }
    else { result = b; result->next = mergeSorted(a, b->next, sortByPrice); }
    return result;
}

Product* mergeSortProducts(Product* node, bool sortByPrice) {
    if (!node || !node->next) return node;
    Product* mid = midNode(node);
    Product* second = mid->next;
    mid->next = nullptr;
    Product* left = mergeSortProducts(node, sortByPrice);
    Product* right = mergeSortProducts(second, sortByPrice);
    return mergeSorted(left, right, sortByPrice);
}

void sortProductsByName() {
    showSpinner(800, 60, 15);
    head = mergeSortProducts(head, false);
    saveAllProductToFile();
}

void sortProductsByPrice() {
    showSpinner(800, 60, 15);
    head = mergeSortProducts(head, true);
    saveAllProductToFile();
}

/* ---------- HEAP SORT for Top Sellers with DYNAMIC BOX ---------- */
void displayTopSellers(int topN) {
    drawHeader("Top Sellers - Using Heap Sort");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    priority_queue<pair<int, Product*>> maxHeap;
    
    for (Product* t = head; t; t = t->next) {
        maxHeap.push({t->soldCount, t});
    }
    
    int actualCount = min(topN, (int)maxHeap.size());
    
    // DYNAMIC BOX HEIGHT
    int boxHeight = 12 + actualCount;
    drawAnimatedBox(10, 10, 70, boxHeight, Color::BRIGHT_YELLOW);
    
    gotoxy(12, 12); 
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(6) << "Rank";
    setColor(Color::BRIGHT_CYAN);
    cout << setw(30) << "Product";
    setColor(Color::BRIGHT_GREEN);
    cout << setw(10) << "Sold";
    setColor(Color::BRIGHT_MAGENTA);
    cout << setw(12) << "Revenue" << endl;
    
    gotoxy(12, 13); 
    setColor(Color::BRIGHT_MAGENTA);
    cout << string(65, '=') << endl;
    
    int y = 14;
    Color rankColors[] = {Color::BRIGHT_YELLOW, Color::BRIGHT_GREEN, Color::BRIGHT_CYAN, Color::BRIGHT_MAGENTA, Color::BRIGHT_WHITE};
    
    for (int i = 0; i < actualCount; ++i) {
        auto top = maxHeap.top(); maxHeap.pop();
        Product* p = top.second;
        
        setColor(rankColors[i % 5]);
        gotoxy(12, y++); 
        cout << left << setw(6) << (i + 1) 
             << setw(30) << p->proName 
             << setw(10) << p->soldCount
             << setw(12) << (int)(p->soldCount * p->proPrice) << " Rs" << endl;
        Sleep(100);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(12, y + 1); getch();
}

/* ---------- REVERSE LIST (Stack-based DSA) ---------- */
void reverseProductList() {
    drawHeader("Reverse Product List - Using Stack");
    
    if (!head) {
        showErrorToast("List is empty", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    showSpinner(1000, 60, 13);
    
    stack<Product*> stk;
    Product* temp = head;
    
    while (temp) {
        stk.push(temp);
        temp = temp->next;
    }
    
    head = stk.top(); stk.pop();
    temp = head;
    
    while (!stk.empty()) {
        temp->next = stk.top();
        stk.pop();
        temp = temp->next;
    }
    temp->next = nullptr;
    
    saveAllProductToFile();
    showSuccessToast("List reversed successfully!", 60, 15);
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

/* ---------- PRODUCT STATISTICS (Graph Theory) ---------- */
void showProductStatistics() {
    drawHeader("Product Statistics - Advanced Analytics");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    drawAnimatedBox(15, 10, 80, 18, Color::BRIGHT_MAGENTA);
    
    int totalProducts = 0, totalStock = 0, totalSold = 0;
    double totalValue = 0, avgPrice = 0, maxPrice = 0, minPrice = 999999;
    
    for (Product* t = head; t; t = t->next) {
        totalProducts++;
        totalStock += t->proNum;
        totalSold += t->soldCount;
        totalValue += t->proPrice * t->proNum;
        avgPrice += t->proPrice;
        if (t->proPrice > maxPrice) maxPrice = t->proPrice;
        if (t->proPrice < minPrice) minPrice = t->proPrice;
    }
    
    avgPrice /= totalProducts;
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(20, 12); cout << "INVENTORY STATISTICS:";
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(25, 14); cout << "Total Products      : " << totalProducts;
    gotoxy(25, 15); cout << "Total Stock Items   : " << totalStock;
    gotoxy(25, 16); cout << "Total Items Sold    : " << totalSold;
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(25, 18); cout << "Inventory Value     : " << (int)totalValue << " Rs";
    gotoxy(25, 19); cout << "Average Price       : " << (int)avgPrice << " Rs";
    gotoxy(25, 20); cout << "Highest Price       : " << (int)maxPrice << " Rs";
    gotoxy(25, 21); cout << "Lowest Price        : " << (int)minPrice << " Rs";
    
    setColor(Color::BRIGHT_MAGENTA);
    double turnoverRate = totalProducts > 0 ? (double)totalSold / (totalSold + totalStock) * 100 : 0;
    gotoxy(25, 23); cout << "Stock Turnover Rate : " << fixed << setprecision(2) << turnoverRate << "%";
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 26); getch();
}

/* ---------- LOW STOCK ALERT with DYNAMIC BOX ---------- */
void showLowStockAlerts() {
    drawHeader("Low Stock Alerts - Priority Queue");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    priority_queue<pair<int, Product*>, vector<pair<int, Product*>>, greater<pair<int, Product*>>> minHeap;
    
    int lowStockCount = 0;
    for (Product* t = head; t; t = t->next) {
        minHeap.push({t->proNum, t});
        if (t->proNum <= 10) lowStockCount++;
    }
    
    if (lowStockCount == 0) {
        drawAnimatedBox(10, 14, 75, 8, Color::BRIGHT_GREEN);
        setColor(Color::BRIGHT_GREEN);
        gotoxy(25, 18);
        cout << "All products have sufficient stock!";
        setColor(Color::BRIGHT_WHITE);
        gotoxy(30, 26); getch();
        return;
    }
    
    // DYNAMIC BOX HEIGHT
    int displayCount = min(lowStockCount, 10);
    int boxHeight = 15 + displayCount;
    drawAnimatedBox(10, 10, 75, boxHeight, Color::BRIGHT_RED);
    
    gotoxy(12, 12);
    setColor(Color::BRIGHT_RED);
    cout << "LOW STOCK WARNING!";
    
    gotoxy(12, 14);
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(30) << "Product" << setw(15) << "Stock" << setw(20) << "Status";
    
    gotoxy(12, 15);
    setColor(Color::BRIGHT_CYAN);
    cout << string(70, '=');
    
    int y = 16, count = 0;
    while (!minHeap.empty() && count < 10) {
        auto item = minHeap.top(); minHeap.pop();
        Product* p = item.second;
        
        if (p->proNum <= 10) {
            setColor(p->proNum == 0 ? Color::BRIGHT_RED : p->proNum <= 5 ? Color::RED : Color::BRIGHT_YELLOW);
            gotoxy(12, y++);
            cout << left << setw(30) << p->proName 
                 << setw(15) << p->proNum
                 << setw(20) << (p->proNum == 0 ? "OUT OF STOCK" : "RESTOCK SOON");
            count++;
            Sleep(80);
        }
    }
    
    if (lowStockCount > 10) {
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(12, y);
        cout << "... and " << (lowStockCount - 10) << " more low stock items";
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, y + 3); getch();
}