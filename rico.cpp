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

struct covering{
     vector<unsigned int> attributes;
    vector< vector<int> > partitions;
};

struct rule{
vector <string> data; 
int instances;
};

void Parser( vector<attributes> &attr, vector< vector<string> > &data);
void listAttributes( vector < attributes > attr);
void Report( string filename, int maxAttr, int minCoverage, bool unnecessary_dropped, vector <attributes> attr, vector <unsigned int> decAttr);
void rico( vector <covering> cover, vector < vector < string> > data, vector<unsigned int> decAttr);

int main() {
    string filename = "I need to fix this";
	string num_input;
	string maxAttr_input;
    unsigned int maxAttr = 0, minCoverage = 1;
    unsigned int num = 0;
	
    vector<attributes> attr;
    vector< vector<string> > data;
    vector<covering> cover(1);
	vector <int> c0;
	c0.push_back(0);
	c0.push_back(2);
    cover[0].partitions.push_back(c0);
	vector <int> c1;
	c0.push_back(1);
	c0.push_back(3);
    cover[0].partitions.push_back(c1);
	vector <int> c2;
	c0.push_back(4);
	c0.push_back(5);
    cover[0].partitions.push_back(c2);
	vector <int> c3;
	c0.push_back(6);
	c0.push_back(7);
    cover[0].partitions.push_back(c3);
	
	cover[0].attributes.push_back(0);
	cover[0].attributes.push_back(2);
	
	
	// Does the File IO things 
	// and parsers the Input
	Parser( attr, data);
   
    // Lists Attributes
    listAttributes( attr);   
    
    cout << "\nPlease enter how many decision attributes you would like: ";
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
    /*for(unsigned int i = 0; i < num; i++)
    {
        cout << "Attribute " << i << ": ";
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
                cout << "Duplicate number detected, please choose another: ";
            }
        }
    }*/

    decAttr[0] = 5; //TESTING PURPOSES
    //decAttr[1] = 4;
    
    cout << "Please enter the maxium # of attributes to be considered as a covering: ";
    cin >> maxAttr_input;
    maxAttr = atoi(maxAttr_input.c_str());
    while (maxAttr == 0 || maxAttr > attr.size() - decAttr.size()) {
        cout << "Invalid input, try again: ";
        cin >> maxAttr_input;
        maxAttr = atoi(maxAttr_input.c_str());
    }

    // Generates Coverings single attr
    
    // coverings to size maxAttr
    
    //rico
    rico( cover, data, decAttr);
	
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
        
    //Prints the Report
	Report( filename, maxAttr, minCoverage, unnecessary_dropped, attr, decAttr);
    return 0;
}


// Funtion that does File IO and Pasering stuff
void Parser( vector<attributes> &attr, vector< vector<string> > &data)
{
    string filename;
    string line;
    string relation;

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
}


// Prints a list of each name in the vector of attributes attr
void listAttributes( vector < attributes > attr)
{
	cout << "\nList of Attributes:" << endl;
    for( unsigned int i = 0; i < attr.size(); i++)
    {
        cout << i << ". " << attr[i].name << endl;
    }
}

// Prints out the final results 
void Report( string filename, int maxAttr, int minCoverage, bool unnecessary_dropped, vector <attributes> attr, vector<unsigned int> decAttr)
{
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
}

void rico( vector <covering> cover, vector < vector < string> > data, vector<unsigned int> decAttr)
{
  unsigned int input = 0;
  cout << "Please Choose a Covering:" << endl;
  // List of Coverings 
  // listCoverings();
  cin >> input;
  // Check to make sure input is valid
  
  vector <rule> rules(cover[input].partitions.size());
  for(unsigned int i = 0; i < rules.size(); i++)
  { 
    rules[i].instances = 1;
	for( unsigned int j = 0; j < cover[input].partitions[i].size(); j++)
    { 
	  rules[i].data.push_back(data[cover[input].partitions[i][j]][cover[input].attributes[j]]);
	}
	// Adds Value of Decision Attribute to the set
	for( unsigned int j = 0; j < decAttr.size(); j++)
	{
	  //for( unsigned int k = 0; k < data[k].size(); j++)
		rules[i].data.push_back(data[i][decAttr[j]]);
	}
  }
  cout << "[" << endl;
  cout << "[";
  for ( unsigned int i = 0; i < rules.size(); i++)
  {
    cout << "[";
    for( unsigned int j = 0; j < rules[i].data.size(); j++)
	{
	  cout << rules[i].data[j] << ", ";
	}
	cout << "]";
	cout << rules[i].instances << "]" << endl;
  }
  cout << "]" << endl;
  /*
  for ( unsigned int i = 0; i < rules.size(); i++)
  {
    for( unsigned int j = i+1; j < rules.size(); j++)
	{
	  if ( rule[i] == rule[j])
	  {
	     rule.erase(j);
		 rule[i].instances++;
	  }
	}
  }
  }*/
}