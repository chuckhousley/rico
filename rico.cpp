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
#include <algorithm>
using namespace std;

struct attributes{
    string name;
    string type;
};

struct rule{
    vector <string> data;
    int instances;
};

struct covering{
    vector<unsigned int> attributes;
    vector< vector<int> > partitions;
};

bool comparePartitions(vector< vector<int> > p1, vector< vector<int> > p2);
vector< vector<unsigned int> > generateCombinations(vector<attributes> attr, vector<unsigned int> decAttr, unsigned int maxAttr);
void Parser( vector<attributes> &attr, vector< vector<string> > &data, string &filename);
void listAttributes( vector < attributes > attr);
void Report( string filename, int maxAttr, int minCoverage, bool unnecessary_dropped, vector <attributes> attr, vector <unsigned int> decAttr);
void rico(vector<covering> cover, vector< vector<string> > data, vector<unsigned int> decAttr, vector<attributes> attr, int minCoverage);

int main() {
    string filename;
	string num_input;
	string maxAttr_input;
	string minCoverage_input;
    unsigned int maxAttr = 0, minCoverage = 1;

    unsigned int num = 0;
	
    vector<attributes> attr;
    vector< vector<string> > data;
    vector<covering> cover;
    vector< vector<unsigned int> > combinations;
    cout << "----Welcome to Rule Induced from COverings----" << endl;
	cout << endl;
	// Prompts the User for a ARRF file 
	// and parses the content of that file 
	// into the attr and data variables 
	Parser( attr, data, filename);
   
    // Lists Attributes
	// with there indexes so a user can 
	// make his/her selections
    listAttributes( attr);  
    cout << "\nPlease enter how many decision attributes you would like: ";
    cin >> num_input;
    num = atoi(num_input.c_str());
     // Validates user input 
    while (num == 0 || num >= attr.size()) {
        cout << "Invalid input, please enter how many decision attributes you would like: ";
        cin >> num_input;
        num = atoi(num_input.c_str());
    }
    
    vector<unsigned int> decAttr(num,-1);
    cout << "Please enter the numbers of all decision attributes (separate by enter key): " << endl;
    // takes and validates user input 
    for(unsigned int i = 0; i < num; i++)
    {
        bool duplicate = true;
        while (duplicate) {
            duplicate = false;
            string dec_input;
            cin >> dec_input;
            while (dec_input.size() > 1 && atoi(dec_input.substr(0, 1).c_str()) == 0) {
                cout << "Invalid input, try again: ";
                cin >> dec_input;
            }
            int dec_input_i = atoi(dec_input.c_str());
            while (dec_input_i < 0 || dec_input_i > attr.size()-1) {
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
    }


    cout << "Please enter the maxium # of attributes to be considered as a covering: ";
    cin >> maxAttr_input;
    maxAttr = atoi(maxAttr_input.c_str());
    //validates user input
    while (maxAttr == 0 || maxAttr > attr.size() - decAttr.size()) {
        cout << "Invalid input, try again: ";
        cin >> maxAttr_input;
        maxAttr = atoi(maxAttr_input.c_str());
    }
    
    cout << "Please enter the minimum coverage required for a rule: ";
    cin >> minCoverage_input;
    minCoverage = atoi(minCoverage_input.c_str());
    //validates user input
    while (minCoverage == 0 || minCoverage > data.size()) {
        cout << "Invalid input, please try again: ";
        cin >> minCoverage_input;
        minCoverage = atoi(minCoverage_input.c_str());
    }
    
    
    // Generates a set of all possible combinations to attempt to
	// make coverings
    combinations = generateCombinations(attr, decAttr, maxAttr);
    /*
	// Uncomment out this to show all possible combinations
    cout << "\nCombinations to consider:" << endl;
    for (int i = 0; i < combinations.size(); i++) {
        for (int j = 0; j < combinations[i].size(); j++) {
            cout << combinations[i][j];
        }
        cout << "\n";
    }
    */
    //create decision base covering (not actually a covering, but what other coverings are based on)
    covering decision;
    decision.attributes = decAttr;
  	// Generates all coverings for the specified decision attribute  
    vector<int> row_nums;
    for (int i = 0; i < data.size(); i++) {
        row_nums.push_back(i);
    }
    vector<string> row;
    
    while(true) {
        vector<int> partition;
        int current_row = -1;
        for (int i = 0; i < row_nums.size(); i++) {
            if (row_nums[i] != -1) {
                current_row = i;
                row = data[i];
                break;
            }
        }
        if (current_row == -1) {break;}
        partition.push_back(current_row);
        row_nums[current_row] = -1;
        
        for (int i = 1; i < row_nums.size(); i++) {
            bool same = true;
            for (int k = 0; k < decision.attributes.size(); k++) {
                if(row[decision.attributes[k]] != data[i][decision.attributes[k]] || row_nums[i] == -1) {
                    same = false;
                }
            }
            if(same) {
                partition.push_back(i);
                row_nums[i] = -1;
            }
        }
        decision.partitions.push_back(partition);
    }
    /*cout << "decision covering: [";
    for (int i = 0; i < decision.partitions.size(); i++) {
        cout << "[";
        for (int j = 0; j < decision.partitions[i].size(); j++) {
            cout << decision.partitions[i][j];
        }
        cout << "] ";
    }
    cout << "]" << endl;*/ //prints the covering of decision attributes
    
    
    
    for (int i = 0; i < combinations.size(); i++) {
        covering new_cover;
        new_cover.attributes = combinations[i];

        vector<int> row_nums;
        for (int j = 0; j < data.size(); j++) {
            row_nums.push_back(j);
        }
        vector<string> row;
        bool its_a_subset = true;
        
        while(true) {
            vector<int> partition;
            int current_row = -1;
            for (int j = 0; j < row_nums.size(); j++) {
                if (row_nums[j] != -1) {
                    current_row = j;
                    row = data[j];
                    break;
                }
            }
            if (current_row == -1) {break;}
            partition.push_back(current_row);
            row_nums[current_row] = -1;
            
            for (int j = 1; j < row_nums.size(); j++) {
                bool same = true;
                for (int k = 0; k < new_cover.attributes.size(); k++) {
                    if(row[new_cover.attributes[k]] != data[j][new_cover.attributes[k]] || row_nums[j] == -1) {
                        same = false;
                    }
                }
                if(same) {
                    partition.push_back(j);
                    row_nums[j] = -1;
                }
            }
            
            int num_to_find = partition.size();
            bool partition_added = false;
            for (int j = 0; j < decision.partitions.size(); j++) {
                int num_found = 0;
                for (int k = 0; k < num_to_find; k++) {
                    if (count(decision.partitions[j].begin(), decision.partitions[j].end(), partition[k])) {
                        num_found++;
                    }
                }
                if (num_found == num_to_find) {
                    new_cover.partitions.push_back(partition);
                    partition_added = true;
                    break;
                }
            }
            if (!partition_added) {
                its_a_subset = false;
                break;
            }
        }
        
        if (its_a_subset) {
            bool its_minimal = true;
            for (int j = 0; j < cover.size(); j++) {
                int num_to_find = cover[j].attributes.size();
                int num_found = 0;
                for(int k = 0; k < num_to_find; k++) {
                    if (count(new_cover.attributes.begin(), new_cover.attributes.end(), cover[j].attributes[k])) {
                        num_found++;
                    }
                }
                vector< vector<int> > p1 = new_cover.partitions;
                vector< vector<int> > p2 = cover[j].partitions;
                if (num_to_find == num_found && comparePartitions(p1, p2)) {
                    its_minimal = false;
                }
            }
            
            if (its_minimal) {
                cover.push_back(new_cover);
            }
        }
    }
    cout << "Number of coverings found: " << cover.size() << endl;
    for (int i = 0; i < cover.size(); i++) {
		cout << i << ". " ;
        for (int j = 0; j < cover[i].attributes.size(); j++) {
            cout << cover[i].attributes[j];
        }
        cout << endl;
    }
    // Calls the rice algorithm 
	// which generates rules
    rico(cover, data, decAttr, attr, minCoverage);


    string unnecessary_input;
    cout << "Should unnecessary conditions be dropped? (y/n): n\n";
    /*cin >> unnecessary_input;
    //Validates User input
    while (unnecessary_input != "y" && unnecessary_input != "n") {
        cout << "Please enter 'y' or 'n': ";
        cin >> unnecessary_input;
    }*/
    unnecessary_input = "n"; //hahaha we don't care
    
    bool unnecessary_dropped = ((unnecessary_input == "y")? true: false);
        
  //Prints the Report
	Report( filename, maxAttr, minCoverage, unnecessary_dropped, attr, decAttr);
    return 0;
}

// Function that generates all possible combinations of attributes 
vector< vector<unsigned int> > generateCombinations(vector<attributes> attr, vector<unsigned int> decAttr, unsigned int maxAttr) {
    vector< vector<unsigned int> > combinations;
    for (unsigned int i = 0; i < attr.size(); i++) { //finds non-decision attrs
        vector<unsigned int> new_combination;
        bool decision = false;
        for (int j = 0; j < decAttr.size(); j++){
            if (i == decAttr[j]) { // if current attr is decision attr, don't add it
                decision = true;
            }
        }
        if (!decision) {
            new_combination.push_back(i);
            combinations.push_back(new_combination);
        }
    }
    
    unsigned int itr1 = 0;
    unsigned int itr2 = 1;
    int old_size = combinations.size();
    int new_size = combinations.size();
    for (int i = 2; i <= maxAttr; i++) {
        while (itr1 < new_size - 1) {
            while (itr2 < old_size) {
                vector<unsigned int> new_combination;
                new_combination = combinations[itr1];
                new_combination.push_back(combinations[itr2][0]);
                combinations.push_back(new_combination);
                itr2++;
            }
            itr1++;
            itr2 = combinations[itr1].back()+1;
        }
        itr1 = new_size;
        new_size = combinations.size();
        itr2 = combinations[new_size-1].size();
    }
    
    return combinations;
}


// Function compares two sets of rows to see if they are same
bool comparePartitions(vector< vector<int> > p1, vector< vector<int> > p2) {
    if(p1.size() != p2.size()) {
        return false;
    }
    
    for (int i = 0; i < p1.size(); i++) {
        if (p1[i].size() != p2[i].size()) {
            return false;
        }
        
        for (int m = 0; m < p1[i].size(); m++) {
            if (p1[i][m] != p2[i][m]) {
                return false;
            }
        }
    }
    return true;
}

// Function that takes a user specified AFFR file and then parses it and
// stores the parsed data in the data and attr variables 
void Parser( vector<attributes> &attr, vector< vector<string> > &data, string &filename)
{
    string line;
    string relation;

	// Gets the name of the input file
    cout<<"Enter a filename: ";
    cin>>filename;
    ifstream in;
    in.open(filename.c_str());
    
    while(!in.is_open()) {
        cout << "Failed to open file" << endl;
		cout <<"Enter a filemane: ";
		cin >> filename;
        in.open(filename.c_str());
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


// Prints a list of each name in the vector of attributes attr along with
// its index
void listAttributes( vector < attributes > attr)
{
	cout << "\nList of Attributes:" << endl;
    for( unsigned int i = 0; i < attr.size(); i++)
    {
        cout << i << ". " << attr[i].name << endl;
    }
}

// Prints out the final report
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

// Function RICOS enough said
void rico(vector<covering> cover, vector< vector<string> > data, vector<unsigned int> decAttr, vector<attributes> attr, int minCoverage)
{ 
  vector < vector<rule> > rules;
  for (int i = 0; i < decAttr.size(); i++) {
      vector<string> value;
      vector<int> occurences;
      for (int j = 0; j < data.size(); j++) {
        bool added = false;
        for (int k = 0; k < value.size(); k++) {
            if (data[j][decAttr[i]] == value[k]) {
                occurences[k]++;
                added = true; 
                break;
            }
        }
        if (!added) {
            value.push_back(data[j][decAttr[i]]);
            occurences.push_back(1);
        }
      }
      cout << "Distribution of values for " << attr[decAttr[i]].name << ":" << endl;
      for (int j = 0; j < value.size(); j++) {
        cout << "Value: " << value[j] << " Occurrences: " << occurences[j] <<endl;
      }
  }
  for (unsigned int input = 0; input < cover.size(); input++) 
  {
    covering current_covering = cover[input];
    vector<rule> current_rule_set;
    vector<unsigned int> current_attributes = current_covering.attributes;
    for (int i = 0; i < current_covering.partitions.size(); i++) { //gets each partition
        rule new_rule;
        vector<int> current_partition = current_covering.partitions[i];
        new_rule.instances = current_partition.size();
            for (int k = 0; k < current_attributes.size(); k++) {//
                new_rule.data.push_back(data[current_partition[0]][k]);
            }
            for (int k = 0; k < decAttr.size(); k++) {
                new_rule.data.push_back(data[current_partition[0]][decAttr[k]]);
            }
            
            current_rule_set.push_back(new_rule);
      }
      rules.push_back(current_rule_set);
      
  }
  for( unsigned int input = 0; input < rules.size();input++)
  {
    int too_few = 0;
    for (int i = 0; i < rules[input].size(); i++) {
        if(rules[input][i].instances < minCoverage) {too_few++;}
    }
    if (too_few == rules[input].size()) {
        cout << "All rules for covering " << input << " do not meet the minimum requirements for a rule, skipping" << endl;
        continue;
    }
    cout << "\nRules for cover: " << input << endl;
    cout << "[" << endl;
    for ( unsigned int i = 0; i < rules[input].size(); i++)
    {
        if (rules[input][i].instances < minCoverage) {break;}
        cout << "[[";
		
        for( unsigned int j = 0; j < rules[input][i].data.size(); j++)
        {
            cout << rules[input][i].data[j];
			if( j != rules[input][i].data.size()-1)
				cout << ", ";
        }
        cout << "]";
        cout << rules[input][i].instances << "]";
		if(i != rules[input].size()-1)
			cout << "," << endl;
    }
	cout << "]" << endl;
  }
}