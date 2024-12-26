#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> 
using namespace std;

class Menu {
public:
    void updatestock() {
        int searchId;
        cout << "Enter item ID to check: ";
        cin >> searchId;

        ifstream file("Menu.csv");
        if (!file.is_open()) {
            cerr << "Error: Could not open the CSV file!" << endl;
            return;
        }

        string line;
        bool found = false;
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string idStr, category, item, priceStr, availabilityStr;

            if (!getline(ss, idStr, ',') ||
                !getline(ss, category, ',') ||
                !getline(ss, item, ',') ||
                !getline(ss, priceStr, ',') ||
                !getline(ss, availabilityStr, ',')) {
                continue;
            }

            priceStr.erase(remove(priceStr.begin(), priceStr.end(), ' '), priceStr.end());

            int id = 0;
            int availability = 0;
            bool validData = true;

            try {
                id = stoi(idStr);
            } catch (...) {
                validData = false;
            }

            if (validData) {
                try {
                    availability = stoi(availabilityStr);
                } catch (...) {
                    validData = false;
                }
            }

            if (validData) {
                if (id == searchId) {
                    found = true;
                    if (!priceStr.empty() && priceStr[0] == '$') {
                        priceStr.erase(0, 1);
                    }

                    cout << "Item Found:" << endl;
                    cout << "ID: " << id << endl;
                    cout << "Category: " << category << endl;
                    cout << "Item: " << item << endl;
                    cout << "Price: $" << priceStr << endl;
                    cout << "Availability: " << availability << " units" << endl;
                    cout<<"Enter the new stock "<<endl;
                    
                    break;
                }
            } else {
                cerr << "Error: Invalid data in CSV file. Skipping line: " << line << endl;
            }
        }

        if (!found) {
            cout << "No item found with ID: " << searchId << endl;
        }

        file.close();
    }
};

int main() {
    Menu menu;
    menu.updatestock();
    return 0;
}
