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
	
    cout<<"Enter a filename: ";
    //cin>>filename;
    filename = "table3_10_fg.arff";
    
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
            new_attr.type = line.substr(itr2+1);
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
	cout << "List of Attributes:" << endl;
	for( unsigned int i = 0; i < attr.size(); i++)
	{
	  cout << i << ". " << attr[i].name << endl;
	}
	unsigned int num = 0;
	cout << "Please Enter How Many Decision Attributes You Would Like" << endl;
	// Add a check 
	//cin >> num;
	num = 1;
	vector<unsigned int> decAttr(num,-1);
	cout << "Please Enter the Numbers of All Decision Attributes" << endl;
	for(unsigned int i = 0; i < num; i++)
	{
	  //cin >> decAttr[i];
	}
	decAttr[0] = 5;
	cout << "Please Enter the Maxium # of attributes to cosider a covering" << endl; 
	cin >> maxAttr; 
	// Generates Coverings single attr
	for( unsigned int i = 0; i < attr.size(); i++)
	{
	  coverings newCover;
	  newCover.P = (attr[i].name);
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
	cout << "Please Enter the Minimum Coverage required for a rule" << endl;
	cin >> minCoverage;
	
	//Reporting 
	
	
	
    in.close();
    return 0;
}
