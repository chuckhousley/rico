#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct attributes{
    string name;
    string type;
};

int main() {
	//one last last test
    string filename;
    string line;
    string relation;
    vector<attributes> attr;
    ifstream in;
    vector< vector<string> > data;
    cout<<"Enter a filename: "<<endl;
    cin>>filename;
    in.open(filename.c_str());
    if(in.is_open()) {
        while (getline(in, line)) {
            if (line.substr(0,2) == "@a") {
                attributes new_attr;
                cout << line << endl;
                size_t itr1 = line.find(' ');
                size_t itr2 = line.find(' ', itr1+1);
                new_attr.name = line.substr(itr1+1, itr2-itr1-1);
                new_attr.type = line.substr(itr2+1);
                attr.push_back(new_attr);
            }
            if (line.substr(0, 1) != "@" && line.substr(0, 1) != "\n") {
                vector<string> row;
                while(line.substr(0,1) != "/n") {
                    size_t itr1 = line.find(',');
                    cout << line.subs
                    /*row.push_back(line.substr(0,itr1));
                    line.erase(0, itr1);*/
                }
                for (int i = 0; i < row.size(); i++)
                    cout<< row[i] << ',';
                cout << endl;
                data.push_back(row);
            }

        }
    }
    in.close();
}
