#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <unordered_set> 
using namespace std;

class Cafeteria {
private:
    string menuFileName = "Menu.csv";
    string salesReportFileName = "sales_report.csv";
    string customerFileName = "customers.csv";
    int count;
    vector<string> row;
    vector<vector<string>> data;
    string line;
    bool booll = true;
    vector<string> customer;
    bool usercheck;
    vector<vector<string>> cus_data;
    string line2;

    unordered_set<string> customerIDs; 

public:
    Cafeteria() {
        CSvToVector();
        loadCustomerIDs(); 
    }

    
    void CSvToVector() {
        ifstream File(menuFileName);
        if (File.is_open()) {
            while (getline(File, line)) {
                row.clear();
                stringstream ss(line);
                while (getline(ss, line2)) {
                    row.push_back(line2);
                }
                data.push_back(row);
            }
        }
        File.close();
    }

   
    void loadCustomerIDs() {
        ifstream id(customerFileName);
        if (!id.is_open()) {
            cout << "Error opening customer file.\n";
            return;
        }

        customerIDs.clear(); 
        string line;
        while (getline(id, line)) {
            stringstream s(line);
            string customerId;
            getline(s, customerId, ','); 
            customerIDs.insert(customerId); 
        }
        id.close();
    }

    
    void addMenuItem() {
        ofstream menuFile(menuFileName, ios::app);
        if (!menuFile.is_open()) {
            cout << "Error opening menu file.\n";
            return;
        }

        string name, category;
        float price;
        int availability;

        checkId();

        cout << "Enter menu item name: ";
        cin.ignore();
        getline(cin, name);

        string validCategories[] = {"Vegetarian", "Vegan", "Gluten-Free", "Specialty", "Drinks"};
        bool validCategory = false;

        while (!validCategory) {
            cout << "Enter category (Vegetarian/Vegan/Gluten-Free/Specialty/Drinks): ";
            getline(cin, category);

            for (const string &validCat : validCategories) {
                if (category == validCat) {
                    validCategory = true;
                    break;
                }
            }

            if (!validCategory) {
                cout << "Invalid category. Please enter a valid category.\n";
            }
        }

        cout << "Enter price: ";
        cin >> price;

        cout << "Enter availability: ";
        cin >> availability;

        menuFile << count << "," << name << "," << category << "," << price << "," << availability << "\n";
        menuFile.close();

        cout << "Menu item added successfully.\n";
        booll = true;
        data.clear();
        CSvToVector();
    }

    // Remove a menu item
    void removeMenuItem() {
        string name;
        cout << "Enter the name of the item to remove: ";
        cin.ignore();
        getline(cin, name);

        ifstream menuFile(menuFileName);
        ofstream tempFile("temp_menu.csv");

        if (!menuFile.is_open() || !tempFile.is_open()) {
            cout << "Error opening files.\n";
            return;
        }

        string line;
        bool found = false;
        while (getline(menuFile, line)) {
            stringstream ss(line);
            string id, menuName, category, price, availability;
            getline(ss, id, ',');
            getline(ss, menuName, ',');

            if (menuName == name) {
                found = true;
                continue;
            }
            tempFile << line << "\n";
        }

        menuFile.close();
        tempFile.close();

        if (found) {
            remove(menuFileName.c_str());
            rename("temp_menu.csv", menuFileName.c_str());
            cout << "Menu item removed successfully.\n";
        } else {
            remove("temp_menu.csv");
            cout << "Item not found.\n";
        }
    }

    
    void displayMenu() {
        ifstream menuFile(menuFileName);
        if (!menuFile.is_open()) {
            cout << "Error opening menu file.\n";
            return;
        }

        string line;
        cout << "Menu Items:\n";

        string categories[] = {"Vegetarian", "Vegan", "Gluten-Free", "Specialty", "Drinks"};

        for (const string &category : categories) {
            cout << "\n" << category << ":\n";
            menuFile.clear();
            menuFile.seekg(0, ios::beg);

            while (getline(menuFile, line)) {
                stringstream ss(line);
                string id, name, itemCategory, price, availability;
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, itemCategory, ',');
                getline(ss, price, ',');
                getline(ss, availability, ',');

                if (itemCategory == category) {
                    cout << id << ". " << name << " - $" << price << " (Available: " << availability << ")\n";
                }
            }
        }

        menuFile.close();
    }

    
    bool isDigitsOnly(string& str) {
        for (char c : str) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

   
    void processOrder() {
        string customerId;
        cout << "Enter customer ID: ";
        cin >> customerId;

        
        if (customerIDs.find(customerId) == customerIDs.end()) {
            cout << "Invalid or unregistered customer ID. Please register the customer first.\n";
            return;
        }

        cout << "Customer found. Proceeding with the order...\n";

        string itemName;
        int quantity;

        displayMenu(); 
        cout << "Enter menu item name to order: ";
        cin.ignore();
        getline(cin, itemName);

        cout << "Enter quantity: ";
        cin >> quantity;

        
        ifstream menuFile(menuFileName);
        ofstream tempFile("temp_menu.csv");
        if (!menuFile.is_open() || !tempFile.is_open()) {
            cout << "Error opening menu files.\n";
            return;
        }

        bool itemFound = false;
        float price = 0;

        while (getline(menuFile, line)) {
            stringstream ss(line);
            string id, name, category, priceStr, availabilityStr;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, category, ',');
            getline(ss, priceStr, ',');
            getline(ss, availabilityStr, ',');

            if (name == itemName) {
                itemFound = true;
                price = stof(priceStr);
                int availability = stoi(availabilityStr);
                if (availability >= quantity) {
                    availability -= quantity;
                    tempFile << id << "," << name << "," << category << "," << priceStr << "," << availability << "\n";
                } else {
                    cout << "Insufficient stock for the item.\n";
                    tempFile << line << "\n";
                }
            } else {
                tempFile << line << "\n";
            }
        }

        if (!itemFound) {
            cout << "Item not found on the menu.\n";
        }

        menuFile.close();
        tempFile.close();

        remove(menuFileName.c_str());
        rename("temp_menu.csv", menuFileName.c_str());

       
        float totalPrice = price * quantity;
        ofstream salesReportFile(salesReportFileName, ios::app);
        if (!salesReportFile.is_open()) {
            cout << "Error opening sales report file.\n";
            return;
        }

        time_t now = time(0);
        char *dt = ctime(&now);

        salesReportFile << "Customer ID: " << customerId << ", Item: " << itemName
                         << ", Quantity: " << quantity << ", Total: $" << totalPrice
                         << ", Date: " << dt;

        salesReportFile.close();

        cout << "Order processed. Total: $" << totalPrice << "\n";
        
    }

   
    void registerCustomer() {
        ofstream customerFile(customerFileName, ios::app);
        if (!customerFile.is_open()) {
            cout << "Error opening customer file.\n";
            return;
        }

        string id;
        string name, contact, dietary;

        while (true) {
            cout << "Enter customer ID: ";
            cin >> id;
            if (!isDigitsOnly(id)) {
                cout<<"Its not digit"<<endl;
                
               
            }
			else if (customerIDs.find(id) != customerIDs.end()) {
            cout << "use different ID It Is Already Present .\n";
            
        } 
		else {
                cout << "ID Accepted" << endl;
                break;
            }
        }

        cin.ignore();
        cout << "Enter customer name: ";
        getline(cin, name);

        cout << "Enter customer contact: ";
        getline(cin, contact);

        cout << "Enter dietary preferences (Vegetarian/Vegan/Gluten-Free): ";
        getline(cin, dietary);

        customerFile << id << "," << name << "," << contact << "," << dietary << ","<<"0"<<"\n";
        customerFile.close();

       
        customerIDs.insert(id);

        cout << "Customer registered successfully.\n";
    }

   
    void displayCustomers() {
        ifstream customerFile(customerFileName);
        if (!customerFile.is_open()) {
            cout << "Error opening customer file.\n";
            return;
        }

        string line;
        cout << "Customer List:\n";
        while (getline(customerFile, line)) {
            cout << line << "\n";
        }
        customerFile.close();
    }

    
    void checkId() {
        for (int i = 1; booll; i++) {
            cout << "Enter Menu ID: ";
            cin >> count;
            if (count == data.size()) {
                cout << "Id is Correct" << endl;
                booll = false;
                break;
            } else {
                cout << "ID is already present or invalid. Current ID that you can use is " << data.size() << endl;
            }
        }
    }
};

int main() {
    Cafeteria caf;
    int choice;

    do {
        cout << "\nCafeteria Management System\n";
        cout << "1. Add Menu Item\n";
        cout << "2. Remove Menu Item\n";
        cout << "3. Display Menu\n";
        cout << "4. Process Order\n";
        cout << "5. Register New Customer\n";
        cout << "6. Display All Customers\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                caf.addMenuItem();
                break;
            case 2:
                caf.removeMenuItem();
                break;
            case 3:
                caf.displayMenu();
                break;
            case 4:
                caf.processOrder();
                break;
            case 5:
                caf.registerCustomer();
                break;
            case 6:
                caf.displayCustomers();
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
