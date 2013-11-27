/*
  Rico
  A Program by Chuck and Pearson
  Presented by Computer Science 301
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

struct attributes{
    string name;
    string type;
};

struct coverings{
   string P;
   vector< vector<int> > rows;
};

int main() {
    string filename;
    string line;
    string relation;
    unsigned int maxAttr = 0, minCoverage = 1;
        
    vector<attributes> attr;
    vector< vector<string> > data;
    vector<coverings> cover;
        
    // Gets the name of the input file
    cout<<"Enter a filename: ";
    //cin>>filename;
    filename = "table3_10_fg.arff"; //TESTING PURPOSES ONLY
    
    ifstream in;
    in.open(filename.c_str());
    
    if(!in.is_open()) {
        cout << "Failed to open file" << endl;
        exit(2);
    }
    
        //Parses ARFF file
    while (getline(in, line)) {
        //Reading in the attributes
        if (line.substr(0,2) == "@a") {
            attributes new_attr;
            size_t itr1 = line.find(' ');
            size_t itr2 = line.find(' ', itr1+1);
            new_attr.name = line.substr(itr1+1, itr2-itr1-1);
            new_attr.type = line.substr(itr2+1); //TODO: clean up type input
            attr.push_back(new_attr);
        }
        //reading in the data
        //assumes that data does not contain commas
        //commas are used for the delimiter only
        if (line.substr(0, 1) != "@" && line.find(",") != string::npos) {
            vector<string> row;
            string entry = "";
            for (int i = 0; i < line.size(); i++) {
                if (line[i] != ',') {
                    entry += line[i];
                }
                else {
                    row.push_back(entry);
                    entry = "";
                }
            }
            row.push_back(entry);
            data.push_back(row);
        }
    }
    in.close();
    
        cout << "\nList of Attributes:" << endl;
        for( unsigned int i = 0; i < attr.size(); i++)
        {
         cout << i << ". " << attr[i].name << endl;
        }
    
        unsigned int num = 0;
        cout << "\nPlease enter how many decision attributes you would like: ";
    string num_input;
        cin >> num_input;
    num = atoi(num_input.c_str());
    
    while (num == 0 || num > attr.size()) {
        cout << "Invalid input, please enter how many decision attributes you would like: ";
        cin >> num_input;
        num = atoi(num_input.c_str());
    }
        num = 1; //TESTING PURPOSES ONLY
    
    
        vector<unsigned int> decAttr(num,-1);
        cout << "Please enter the numbers of all decision attributes: " << endl;
        for(unsigned int i = 0; i < num; i++)
        {
        /*cout << "Attribute " << i << ": ";
        bool duplicate = true;
		while (duplicate) {
		duplicate = false;
		string dec_input;
		cin >> dec_input;
		int dec_input_i = atoi(dec_input.c_str());
		while (dec_input_i < 1 || dec_input_i > attr.size()-1) {
		  cout << "Invalid input, try again: ";
		  cin >> dec_input;
			dec_input_i = atoi(dec_input.c_str());
		}
		for (int j = 0; j < i; j++) {
			if (dec_input_i == decAttr[j]) {
				duplicate = true;
			}
		}
		if (!duplicate) {
			decAttr[i] = dec_input_i;
			break;
		}
		else {
			cout << Duplicate number detected, please choose another: ";
		}
		}*/
        }
    
        decAttr[0] = 5; //TESTING PURPOSES
    //decAttr[1] = 4;
        cout << "Please enter the maxium # of attributes to be considered as a covering: ";
        cin >> maxAttr;
    
        // Generates Coverings single attr
        for( unsigned int i = 0; i < attr.size(); i++)
        {
         coverings newCover;
         newCover.P = attr[i].name;
         for( unsigned int j = 0; j < data[i].size(); j++)
         {
         vector<int> rows;
        for( unsigned int k = 0; k < data[i].size(); k++)
        {                
         if( (data[i][j] == data[i][k]) && j != k)
                 {
                 rows.push_back(j);
            //cout << data[i][j] << "=" << data[i][k] <<endl;                        
                 }
                }
                newCover.rows.push_back(rows);
         }
         cover.push_back(newCover);
        }
    
        // coverings to size maxAttr
        
        //rico
        
        //After Running rico
        cout << "Please enter the minimum coverage required for a rule: ";
        cin >> minCoverage;
    
    string unnecessary_input;
    cout << "Should unnecessary conditions be dropped? (y/n): ";
    cin >> unnecessary_input;
    
    while (unnecessary_input != "y" && unnecessary_input != "n") {
        cout << "Please enter 'y' or 'n': ";
        cin >> unnecessary_input;
    }
    
    bool unnecessary_dropped = ((unnecessary_input == "y")? true: false);
        
        //Reporting
    cout << "\n----------OUTPUT----------\n";
    cout << "Name of input file: " << filename << endl;
    cout << "\nMaximum number of attributes to be considered for a covering: " << maxAttr;
    cout << "\nMinimum coverage required for a rule to be reported: " << minCoverage;
    cout << "\nUnnecessary conditions have been " << ((unnecessary_dropped)? "dropped.\n": "left in.\n") << endl;
    
    cout << "Decision Attribute" << ((decAttr.size() > 1)? "s: [": ": [");
    for (int i = 0; i < decAttr.size(); i++) {
        cout << attr[decAttr[i]].name;
        cout << ((i < decAttr.size()-1)? ", ":"]\n");
    }
    
    
    cout << endl; //one more empty line for the hell of it, and we're done
    return 0;
}