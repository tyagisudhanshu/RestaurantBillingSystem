#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

// Item details structure
struct MenuItem {
    string name;
    double price;
};

// Order structure
struct Order {
    string itemName;
    int quantity;
    double totalCost;
};

// Display available items
void showMenu(const vector<MenuItem>& items) {
    cout << "\n====== MENU ======\n";
    cout << left << setw(5) << "No." << setw(20) << "Item" << "Price ($)\n";
    cout << "-----------------------------\n";
    for (size_t i = 0; i < items.size(); ++i) {
        cout << left << setw(5) << i + 1 << setw(20) << items[i].name << fixed << setprecision(2) << items[i].price << "\n";
    }
}

// Get current time as string
string getCurrentTime() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// Create a bill for the customer
void createBill(const vector<MenuItem>& items) {
    vector<Order> currentOrder;
    int itemIndex, qty;
    double total = 0.0;

    while (true) {
        showMenu(items);
        cout << "\nSelect item number (0 to finish): ";
        cin >> itemIndex;

        if (itemIndex == 0) break;

        if (itemIndex < 1 || itemIndex > items.size()) {
            cout << "Invalid item. Try again.\n";
            continue;
        }

        cout << "Quantity: ";
        cin >> qty;

        Order o;
        o.itemName = items[itemIndex - 1].name;
        o.quantity = qty;
        o.totalCost = qty * items[itemIndex - 1].price;

        currentOrder.push_back(o);
        total += o.totalCost;
    }

    // Display bill
    cout << "\n====== BILL ======\n";
    cout << left << setw(20) << "Item" << setw(10) << "Qty" << "Total ($)\n";
    cout << "------------------------------\n";
    for (const auto& order : currentOrder) {
        cout << left << setw(20) << order.itemName << setw(10) << order.quantity << fixed << setprecision(2) << order.totalCost << "\n";
    }
    cout << "------------------------------\n";
    cout << "Total Payable: $" << total << "\n";

    // Save to file
    ofstream outFile("bills.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "------ Bill (" << getCurrentTime() << ") ------\n";
        for (const auto& order : currentOrder) {
            outFile << order.itemName << " x" << order.quantity << " = $" << fixed << setprecision(2) << order.totalCost << "\n";
        }
        outFile << "Total: $" << fixed << setprecision(2) << total << "\n\n";
        outFile.close();
        cout << "\nBill saved successfully.\n";
    } else {
        cout << "Failed to save bill.\n";
    }
}

// Show previously saved bills
void readOldBills() {
    ifstream inFile("bills.txt");
    if (inFile.is_open()) {
        cout << "\n==== Previous Bills ====\n";
        string line;
        while (getline(inFile, line)) {
            cout << line << "\n";
        }
        inFile.close();
    } else {
        cout << "No bill history available.\n";
    }
}

int main() {
    vector<MenuItem> menu = {
        {"Burger", 5.99},
        {"Pizza", 8.99},
        {"Pasta", 7.49},
        {"Fries", 3.49},
        {"Soda", 1.99}
    };

    int option;
    do {
        cout << "\n==== Restaurant Billing System ====\n";
        cout << "1. Show Menu\n";
        cout << "2. Create New Bill\n";
        cout << "3. View Past Bills\n";
        cout << "4. Exit\n";
        cout << "Your choice: ";
        cin >> option;

        switch (option) {
            case 1:
                showMenu(menu);
                break;
            case 2:
                createBill(menu);
                break;
            case 3:
                readOldBills();
                break;
            case 4:
                cout << "Thank you for using the system!\n";
                break;
            default:
                cout << "Invalid input. Try again.\n";
        }
    } while (option != 4);

    return 0;
}
