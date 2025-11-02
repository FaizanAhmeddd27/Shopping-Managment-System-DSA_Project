#include "manager.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <conio.h>
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

void addProduct() {
    drawHeader("Add New Product");
    Product *n = new Product;
    gotoxy(20, 11); cout << "Enter Product Info . . .";
    gotoxy(20, 13); cout << "Product Name     : "; cin.ignore(); getline(cin, n->proName);
    gotoxy(20, 14); cout << "Product ID       : "; cin >> n->proId;
    gotoxy(20, 15); cout << "Product Price    : "; cin >> n->proPrice;
    gotoxy(20, 16); cout << "Product Quantity : "; cin >> n->proNum;
    n->soldCount = 0; n->next = nullptr;
    // append to memory list
    if (!head) head = n;
    else { Product *t=head; while (t->next) t=t->next; t->next = n; }
    // append to file
    saveNewProductToFile(n);
    gotoxy(25, 19); cout << "Product \"" << n->proName << "\" added successfully.";
    gotoxy(30, 23); getch();
}

void displayProduct() {
    drawHeader("All Products");
    Product *t = head;
    if (!head) { gotoxy(34, 12); cout << "No Product is Found"; getch(); return; }
    gotoxy(12, 11);
    cout << left << setw(12) << "Product ID" << setw(30) << "Product Name" << setw(12) << "Price" << setw(10) << "Quantity" << setw(10) << "Sold" << endl;
    gotoxy(12, 12); cout << string(80, '=') << endl;
    int y = 13;
    while (t) {
        gotoxy(12, y++); cout << left << setw(12) << t->proId << setw(30) << t->proName << setw(12) << t->proPrice << setw(10) << t->proNum << setw(10) << t->soldCount << endl;
        t = t->next;
    }
    gotoxy(12, y + 1); cout << "Total Products : " << countProduct();
}

void modifyProduct() {
    drawHeader("Modify Product");
    if (!head) { gotoxy(34, 12); cout << "Store is Empty"; getch(); return; }
    int id; gotoxy(20, 11); cout << "Enter Product's ID : "; cin >> id;
    Product *t = head;
    while (t) {
        if (t->proId == id) {
            gotoxy(12, 13); cout << "Found:";
            gotoxy(12, 15); cout << left << setw(12) << t->proId << setw(30) << t->proName << setw(12) << t->proPrice << setw(10) << t->proNum << endl;
            gotoxy(25, 17); cout << "Enter new info:";
            gotoxy(30, 18); cout << "New Product ID: "; cin >> t->proId;
            gotoxy(30, 19); cout << "New Product Name: "; cin.ignore(); getline(cin, t->proName);
            gotoxy(30, 20); cout << "New Price: "; cin >> t->proPrice;
            gotoxy(30, 21); cout << "New Quantity: "; cin >> t->proNum;
            saveAllProductToFile();
            gotoxy(30, 23); cout << "Product modified."; gotoxy(30, 25); getch();
            return;
        }
        t = t->next;
    }
    gotoxy(30, 14); cout << "No Product Found"; gotoxy(30, 23); getch();
}

void deleteProduct() {
    drawHeader("Delete Product");
    if (!head) { gotoxy(34, 12); cout << "Store is Empty"; getch(); return; }
    int id; gotoxy(20, 11); cout << "Enter Product's ID : "; cin >> id;
    Product *t = head, *prev = nullptr;
    while (t) {
        if (t->proId == id) {
            if (!prev) head = t->next;
            else prev->next = t->next;
            gotoxy(25, 18); cout << "Product \"" << t->proName << "\" deleted successfully.";
            delete t;
            saveAllProductToFile();
            gotoxy(30, 23); getch();
            return;
        }
        prev = t; t = t->next;
    }
    gotoxy(30, 14); cout << "Product ID not found"; gotoxy(30, 23); getch();
}

void saveNewProductToFile(Product *temp) {
    // ensure file exists
    ofstream file("p.csv", ios::app);
    if (!file.is_open()) return;
    // if file was empty we might need a header, but appending is ok for now
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
        // create default file with header
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

// --- merge-sort helpers ---
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
    head = mergeSortProducts(head, false);
    saveAllProductToFile();
}

void sortProductsByPrice() {
    head = mergeSortProducts(head, true);
    saveAllProductToFile();
}

void displayTopSellers(int topN) {
    drawHeader("Top Sellers");
    vector<Product*> arr;
    for (Product* t = head; t; t = t->next) arr.push_back(t);
    sort(arr.begin(), arr.end(), [](Product* a, Product* b) { return a->soldCount > b->soldCount; });
    gotoxy(12, 11); cout << left << setw(6) << "Rank" << setw(30) << "Product" << setw(10) << "Sold" << endl;
    gotoxy(12, 12); cout << string(60, '=') << endl;
    int y = 13;
    for (int i = 0; i < min((int)arr.size(), topN); ++i) {
        gotoxy(12, y++); cout << left << setw(6) << (i + 1) << setw(30) << arr[i]->proName << setw(10) << arr[i]->soldCount << endl;
    }
    gotoxy(12, y + 1); getch();
}