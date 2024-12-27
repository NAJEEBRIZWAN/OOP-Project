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
    string output_line;
    string stock;

public:
    

    void updatestock() {
        int id;
        cout << "Enter item ID for Item for Which you want to update the  stock: ";
        cin >> id;

        ifstream file("Menu.csv");
        if (file.is_open()) {
            bool found = false;
           
            while (getline(file, line)) {
                row.clear();
                stringstream ss(line);
                while (getline(ss, cell, ',')) {
                    row.push_back(cell);
                }
                data.push_back(row);
            }

            
            for(int i=0; i<=4; ++i){
            output_line=data[id][i];
            if(i==4){
            	cout<<"("<<output_line<<" Avilibility)";
			}
			else{
				 cout<<output_line<<" ";
			}
           
			}
			cout<<endl;
			cout<<"Update the stock... : ";
			cin>>stock;
			data[id][4]=stock;
			
            

           
        } else {
            cout << "File failed to open" << endl;
        }
         ofstream updatefile("Menu.csv");
    if (updatefile.is_open()) {    
        for (const auto& roww : data) {
            for (auto i = 0; i < roww.size(); ++i) {
                updatefile << roww[i];
                if (i < roww.size() - 1) {
                    updatefile << ",";
                }
            }
            updatefile << endl;
        }
        updatefile.close();
    } else {
        cout << "Failed to open file for writing" << endl;
        
    }   cout << "Data updated successfully!" << endl;
    }
};

int main() {
    Menu m;
    m.updatestock();
    return 0;
}
