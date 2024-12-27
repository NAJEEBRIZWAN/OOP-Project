#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Menu {
protected:
    string line;
    vector<string> row;
    vector<vector<string>> data;
    string cell;
    int menuItem = 20;
    int id;

public:
   
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear"); 
        #endif
    }

    bool isDigit(const string& str) {
        for (char c : str) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    void addMenuItem() {
    clearScreen();
    ofstream menuFile("Menu.csv", ios::app);
    if (!menuFile.is_open()) {
        cout << "Error opening menu file.\n";
        return;
    }

    string name, category;
    float price;
    int availability;

    vector<string> validCategories = {"Vegetarian", "Vegan", "Gluten-Free", "Specialty", "Drinks"};

    cout << "Enter menu item name: ";
    cin.ignore();
    getline(cin, name);

    bool validCategory = false;
    while (!validCategory) {
        cout << "Enter category (Vegetarian/Vegan/Gluten-Free/Specialty/Drinks): ";
        getline(cin, category);

        for (const string& valid : validCategories) {
            if (category == valid) {
                validCategory = true;
                break;
            }
        }

        if (!validCategory) {
            cout << "Invalid category. Please choose from the available options.\n";
        }
    }

    cout << "Enter price: ";
    cin >> price;

    cout << "Enter availability: ";
    cin >> availability;

    menuItem++;  
    menuFile << menuItem << "," <<category << "," <<  name << "," <<"$"<< price << "," << availability << "\n";
    menuFile.close();

    cout << "Menu item added successfully.\n";
}


    void updatestock() {
        clearScreen(); 
        cout << "Enter item ID for the item you want to update the stock: ";
        cin >> id;

        ifstream file("Menu.csv");
        if (!file.is_open()) {
            cout << "Error opening menu file.\n";
            return;
        }

        while (getline(file, line)) {
            row.clear();
            stringstream ss(line);
            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        file.close();

        for (size_t i = 0; i < data.size(); ++i) {
            if (stoi(data[i][0]) == id) {
                cout << "Current Stock: " << data[i][4] << endl;
                cout << "Update the stock: ";
                string stock;
                while (true) {
                    cin >> stock;
                    if (isDigit(stock)) {
                        data[i][4] = stock;
                        break;
                    } else {
                        cout << "Invalid input. Please enter a valid number: ";
                    }
                }
                break;
            }
        }

        ofstream updateFile("Menu.csv");
        if (updateFile.is_open()) {
            for (const auto& row : data) {
                for (size_t i = 0; i < row.size(); ++i) {
                    updateFile << row[i];
                    if (i < row.size() - 1) {
                        updateFile << ",";
                    }
                }
                updateFile << endl;
            }
            updateFile.close();
        } else {
            cout << "Failed to open file for writing.\n";
        }

        cout << "Data updated successfully.\n";
    }

    void displayMenu() {
        clearScreen();
        ifstream menuFile("Menu.csv");
        if (!menuFile.is_open()) {
            cout << "Error opening menu file.\n";
            return;
        }

        cout << "Menu Items:\n";
        while (getline(menuFile, line)) {
            cout << line << endl;
        }
        menuFile.close();
    }

    void removeMenuItem() {
        clearScreen(); 
        cout << "Enter item ID for the item you want to remove: ";
        cin >> id;

        ifstream menuFile("Menu.csv");
        ofstream tempFile("TempMenu.csv");
        if (!menuFile.is_open() || !tempFile.is_open()) {
            cout << "Error opening files.\n";
            return;
        }

        bool found = false;
        while (getline(menuFile, line)) {
            stringstream ss(line);
            string itemId;

            if (!getline(ss, itemId, ',')) {
                tempFile << line << endl;  
                continue;
            }

            try {
                int currentId = stoi(itemId);
                if (currentId == id) {
                    found = true;
                    continue;  
                }
            } catch (const invalid_argument&) {
                tempFile << line << endl;
                continue;  
            } catch (const out_of_range&) {
                tempFile << line << endl;
                continue;  
            }

            tempFile << line << endl;  
        }

        menuFile.close();
        tempFile.close();

        if (found) {
            remove("Menu.csv");
            rename("TempMenu.csv", "Menu.csv");
            cout << "Menu item removed successfully.\n";
        } else {
            remove("TempMenu.csv");
            cout << "Item not found.\n";
        }
    }
};

int main() {
    Menu menu;
    int choice;
    do {
       
        cout << "\nMenu Management System\n";
        cout << "1. Add Menu Item\n";
        cout << "2. Update Stock\n";
        cout << "3. Display Menu\n";
        cout << "4. Remove Menu Item\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                menu.addMenuItem();
                break;
            case 2:
                menu.updatestock();
                break;
            case 3:
                menu.displayMenu();
                break;
            case 4:
                menu.removeMenuItem();
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
