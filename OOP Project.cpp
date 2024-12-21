#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    // File name
    const string fileName = "Menu.csv";

    // Step 1: Create and Write to the CSV file
    ofstream outFile(fileName); // Open file in write mode
    if (outFile.is_open()) {
        // Write data to the file
        outFile << "ID,Name,Age,Score\n";
        outFile << "1,Alice,24,88.5\n";
        outFile << "2,Bob,30,92.0\n";
        outFile << "3,Charlie,22,76.8\n";
        outFile.close(); // Close the file
        cout << "File created and data written successfully.\n";
    } else {
        cerr << "Error: Unable to create the file." << endl;
        return 1; // Exit program if file creation fails
    }

    // Step 2: Open and Read the CSV file
    ifstream csvFile(fileName); // Open file in read mode
    if (csvFile.is_open()) {
        string line;
        cout << "\nReading the CSV file:\n";
        cout << "----------------------\n";

        // Read file line by line
        while (getline(csvFile, line)) {
            // Use stringstream to split the line by commas
            stringstream ss(line);
            string value;
            vector<string> row;

            // Extract each value separated by a comma
            while (getline(ss, value, ',')) {
                row.push_back(value);
            }

            // Output the row to the terminal
            for (size_t i = 0; i < row.size(); ++i) {
                cout << row[i];
                if (i < row.size() - 1) {
                    cout << " | "; // Add a separator for better readability
                }
            }
            cout << endl; // New line for the next row
        }

        csvFile.close(); // Close the file
    } else {
        cerr << "Error: Unable to open the file." << endl;
    }

    return 0;
}
