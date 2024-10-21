//Jayam Sutariya
//CS 457
//PA3

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

inline bool exists_test(const string& name);

int main() {
   
    string word;
    vector<string> words;

    string curr_dir;
    bool used = false, locked = false;

    vector<string> newSeat;
    vector<string> newStatus;
    string newpath;

    do {

        string line;
        getline(cin, line, ';');	

        //Extract the words
        istringstream input(line);
	
        while (input >> word) {
            words.push_back(word);
        }
        
        if ( words[0] == "CREATE" && words[1] == "DATABASE" ) {
            try {
                string db_name(words[2]);
              	if (exists_test(db_name)) {
			        throw (db_name);
                }
                else {
                    string str = "mkdir " + db_name;
		            const char *command = str.c_str();
		            system(command);
		            cout << "Database " << db_name << " created." << endl;
                }   
            }
            catch (string dbName) {
                cout << "!Failed to create database " << dbName << " because it already exists." << endl;
            }
        }
        
	    if ( words[0] == "DROP" && words[1] == "DATABASE" ) {
            try {
                string db_name(words[2]);
              	if (!exists_test(db_name)) {
			        throw (db_name);
		        }
                else {
                    string str = "rm -r  " + db_name;
		            const char *command = str.c_str();
		            system(command);
		            cout << "Database " << db_name << " deleted." << endl;
                } 
            }
            catch (string dbName) {
                cout << "!Failed to delete database " << dbName << " because it does not exist." << endl;
            }
        }

        if ( words[0] == "USE") {
            string db_name(words[1]);
            try {
                if (!exists_test(db_name)) {
                    throw(db_name);
                }
                else {
                    used = true;
                    curr_dir = words[1];
                    cout << "Using database " << curr_dir << "." << endl;
                }
            }
            catch (string dbName) {
                cout << "!Failed to use database " << dbName << " because it does not exist." << endl;
            }
        }

        if (words[0] == "create" && words[1] == "table") {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name(words[2]);
                        string path = curr_dir + "/" + tbl_name;
                        if (exists_test(path)) {
                            throw (tbl_name);
                        }
                        else {
                            string str = "touch " + path;
                            const char *command = str.c_str();
                            system(command);

                            ofstream outFile;
                            outFile.open(path, ios::app);

                            words[3].erase(0, 1);
                            words[words.size()-1].pop_back();
                            for (int i = 3; i < words.size(); i++) {
                                replace(words[i].begin(), words[i].end(), ',', '|');
                                outFile << words[i] << " ";
                            }
                            outFile << endl;
                            outFile.close();

                            cout << "Table " << tbl_name << " created." << endl;
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to create table " << tblName << " because it already exists." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to create table because no directory is currently in use." << endl;
            }
        }

        if ( words[0] == "DROP" && words[1] == "TABLE" ) {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name(words[2]);
                        string path = curr_dir + "/" + tbl_name;
                        if (!exists_test(path)) {
                            throw (tbl_name);
                        }
                        else {
                            string str = "rm -r " + path;
                            const char *command = str.c_str();
                            system(command);

                            cout << "Table " << tbl_name << " deleted." << endl;
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to delete table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to delete table because no directory is currently in use." << endl;
            }
        }

	    if (words[0] == "insert") {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name(words[2]);
                        string path = curr_dir + "/" + tbl_name;
                        if (!exists_test(path)) {
                            throw (tbl_name);
                        }
                        else {
                            ofstream outFile;
                            outFile.open(path, ios::app);

                            words[4].erase(0, 1);
                            words[words.size()-1].pop_back();
                            for (int i = 4; i < words.size(); i++) {
                                replace(words[i].begin(), words[i].end(), ',', '|');
                                replace(words[i].begin(), words[i].end(), '\'', ' ');
                                outFile << words[i];
                            }
                            outFile << endl;
                            outFile.close();

                            cout << "1 new record inserted." << endl;
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to modify table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to modify table because no directory is currently in use." << endl;
            }
	    }

        if (words[0] == "select" && words.size() == 4) {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name(words[3]);
                        string path = curr_dir + "/" + tbl_name;
                        
                        if (!exists_test(path)) {
                            throw (tbl_name);
                        } else {
                            string status, seat;
                            vector<string> statusArr;
                            vector<string> seatArr;

                            ifstream inFile(path);

                            string head;
                            if (inFile.is_open()) {
                                //ignore first line
                                getline(inFile, head, '\n');


                                while (!inFile.eof()) //while the end of file is NOT reached
                                {
                                    getline(inFile, seat, '|');
                                    seatArr.push_back(seat);
                                    getline(inFile, status, '\n');
                                    statusArr.push_back(status);
                                }
                                inFile.close(); //closing the file
                            }

                            cout << head << endl;
                            for (int i = 0; i < seatArr.size()-1; i++) {
                                    cout << seatArr[i] << "|" << statusArr[i] << endl;
                            }
                            
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to query table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to query table because no directory is currently in use." << endl;
            }
        }

        if (words[0] == "update") {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name(words[1]);
                        string path = curr_dir + "/" + tbl_name;

                        if (!exists_test(path)) {
                            throw (tbl_name);
                        }
                        else {
                            try {
                                if (locked == true) {
                                    throw(tbl_name);
                                }
                                else {
                                    for (int i = 5; i < words.size(); i++) {
                                        replace(words[i].begin(), words[i].end(), '\'', ' ');
                                    }

                                    string status, seat;
                                    vector<string> statusArr;
                                    vector<string> seatArr;

                                    ifstream inFile(path);

                                    if (inFile.is_open()) {
                                        //ignore first line
                                        string line;
                                        getline(inFile, line);

                                        while (!inFile.eof()) //while the end of file is NOT reached
                                        {
                                            getline(inFile, seat, '|');
                                            seatArr.push_back(seat);
                                            getline(inFile, status, '\n');
                                            statusArr.push_back(status);
                                        }
                                        inFile.close(); //closing the file
                                    }

                                    string setWhat = words[3];
                                    string whereWhat = words[7];
                                    string setTo = words[5];
                                    string whereFrom = words[9];

                                    int counter = 0;
                                    
                                    if (setWhat == "status") {
                                        for (int i = 0; i < seatArr.size()-1; i++) {
                                            if (seatArr[i] == whereFrom) {
                                                statusArr[i] = setTo;
                                                counter++;
                                            }
                                        }
                                    }

                                    copy(seatArr.begin(), seatArr.end(), back_inserter(newSeat));
                                    copy(statusArr.begin(), statusArr.end(), back_inserter(newStatus));
                                    newpath = path;

                                    cout << counter << " record modified." << endl;
                                }
                            }
                            catch(string tblName) {
                                cout << "Error: Table " << tblName << " is locked!" << endl;
                            }      
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to modify table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to modify table because no directory is currently in use." << endl;
            }
        }

        if (words[0] == "delete") {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name(words[2]);
                        string path = curr_dir + "/" + tbl_name;
                        if (!exists_test(path)) {
                            throw (tbl_name);
                        }
                        else {
                            for (int i = 5; i < words.size(); i++) {
                                replace(words[i].begin(), words[i].end(), '\'', ' ');
                            }

                            string pid, pname, price;
                            vector<string>pidArr;
                            vector<string>nameArr;
                            vector<float>priceArr;

                            ifstream inFile(path);

                            if (inFile.is_open()) {
                                //ignore first line
                                string line;
                                getline(inFile, line);

                                while (!inFile.eof()) //while the end of file is NOT reached
                                {
                                    getline(inFile, pid, '|');
                                    pidArr.push_back(pid);
                                    getline(inFile, pname, '|');
                                    nameArr.push_back(pname);
                                    getline(inFile, price, '\n');
                                    priceArr.push_back(stof(price));
                                }
                                inFile.close(); //closing the file
                            }

                            string checkWhat = words[4];
                            string operand = words[5];
                            string whereValue = words[6];

                            int counter = 0;
                            
                            if (checkWhat == "name") {
                                for (int i = 0; i < pidArr.size()-1; i++) {
                                    if (operand == "=") {
                                        if (nameArr[i] == whereValue) {
                                            pidArr.erase(pidArr.begin() + i);
                                            nameArr.erase(nameArr.begin() + i);
                                            priceArr.erase(priceArr.begin() + i);
                                            counter++;
                                        }
                                    }
                                }
                            }
                            
                            if (checkWhat == "price") {
                                for (int i = 0; i < pidArr.size()-1; i++) {
                                    if (operand == ">") {
                                        if (priceArr[i] > stof(whereValue)) {
                                            pidArr.erase(pidArr.begin() + i);
                                            nameArr.erase(nameArr.begin() + i);
                                            priceArr.erase(priceArr.begin() + i);
                                            counter++;
                                        }
                                    }
                                    
                                }
                            }

                            ofstream outFile(path);
                            outFile << "pid int| name varchar(20)| price float" << endl;
                            for (int i = 0; i < pidArr.size()-1; i++) {
                                outFile << pidArr[i] << "|" << nameArr[i] << "| " << priceArr[i] << endl;
                            }

                            cout << counter << " record deleted." << endl;
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to modify table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to modify table because no directory is currently in use." << endl;
            }
        }

        if (words[0] == "select" && words[1] != "*") {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name(words[4]);
                        string path = curr_dir + "/" + tbl_name;
                        if (!exists_test(path)) {
                            throw (tbl_name);
                        }
                        else {
                            for (int i = 1; i < words.size(); i++) {
                                replace(words[i].begin(), words[i].end(), ',', ' ');
                            }

                            string pid, pname, price;
                            vector<string>pidArr;
                            vector<string>nameArr;
                            vector<float>priceArr;

                            ifstream inFile(path);

                            if (inFile.is_open()) {
                                //ignore first line
                                string ignore, line;
                                getline(inFile, ignore, '|');
                                getline(inFile, line);
                                cout << line << endl;

                                while (!inFile.eof()) //while the end of file is NOT reached
                                {
                                    getline(inFile, pid, '|');
                                    pidArr.push_back(pid);
                                    getline(inFile, pname, '|');
                                    nameArr.push_back(pname);
                                    getline(inFile, price, '\n');
                                    priceArr.push_back(stof(price));
                                }
                                inFile.close(); //closing the file
                            }

                            string checkWhat = words[words.size()-3];
                            string operand = words[words.size()-2];
                            string whereValue = words[words.size()-1];

                            if (checkWhat == "pid") {
                                for (int i = 0; i < pidArr.size()-1; i++) {
                                    if (operand == "!=") {
                                        if (pidArr[i] != whereValue) {
                                            cout << nameArr[i] << "| " << priceArr[i] << endl;
                                        }
                                    }
                                    
                                }
                            }
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to query table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to query table because no directory is currently in use." << endl;
            }
        }

        if (words[0] == "select" && words.size() == 13) {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name1(words[3]);
                        string path1 = curr_dir + "/" + tbl_name1;
                        
                        string tbl_name2(words[7]);
                        string path2 = curr_dir + "/" + tbl_name2;
                        
                        if (!exists_test(path1)) {
                            throw (tbl_name1);
                        } else if (!exists_test(path2)) {
                            throw (tbl_name2);
                        }
                        else {
                            string id, name, eid, pid;
                            vector<string>idArr;
                            vector<string>nameArr;
                            vector<string>eidArr;
                            vector<string>pidArr;

                            ifstream inFile1(path1);
                            ifstream inFile2(path2);

                            string head1, head2;
                            if (inFile1.is_open()) {
                                //ignore first line
                                getline(inFile1, head1, '\n');


                                while (!inFile1.eof()) //while the end of file is NOT reached
                                {
                                    getline(inFile1, id, '|');
                                    idArr.push_back(id);
                                    getline(inFile1, name, '\n');
                                    nameArr.push_back(name);
                                }
                                inFile1.close(); //closing the file
                            }

                            if (inFile2.is_open()) {
                                //ignore first line
                                getline(inFile2, head2, '\n');

                                while (!inFile2.eof()) //while the end of file is NOT reached
                                {
                                    getline(inFile2, eid, '|');
                                    eidArr.push_back(eid);
                                    getline(inFile2, pid, '\n');
                                    pidArr.push_back(pid);
                                }
                                inFile2.close(); //closing the file
                            }

                            cout << head1 << "| " << head2 << endl;
                            for (int i = 0; i < idArr.size()-1; i++) {
                                for (int j = 0; j < eidArr.size()-1; j++) {
                                    if (idArr[i] == eidArr[j]) {
                                        cout << idArr[i] << "|" << nameArr[i] << "|" << eidArr[j] << "|" << pidArr[j] << endl;
                                    }
                                }
                            }
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to query table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to query table because no directory is currently in use." << endl;
            }
        }

        if (words[0] == "select" && words.size() == 14) {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    try {
                        string tbl_name1(words[3]);
                        string path1 = curr_dir + "/" + tbl_name1;
                        
                        string tbl_name2(words[8]);
                        string path2 = curr_dir + "/" + tbl_name2;
                        
                        if (!exists_test(path1)) {
                            throw (tbl_name1);
                        } else if (!exists_test(path2)) {
                            throw (tbl_name2);
                        }
                        else {
                            string id, name, eid, pid;
                            vector<string>idArr;
                            vector<string>nameArr;
                            vector<string>eidArr;
                            vector<string>pidArr;

                            ifstream inFile1(path1);
                            ifstream inFile2(path2);

                            string head1, head2;
                            if (inFile1.is_open()) {
                                //ignore first line
                                getline(inFile1, head1, '\n');


                                while (!inFile1.eof()) //while the end of file is NOT reached
                                {
                                    getline(inFile1, id, '|');
                                    idArr.push_back(id);
                                    getline(inFile1, name, '\n');
                                    nameArr.push_back(name);
                                }
                                inFile1.close(); //closing the file
                            }

                            if (inFile2.is_open()) {
                                //ignore first line
                                getline(inFile2, head2, '\n');

                                while (!inFile2.eof()) //while the end of file is NOT reached
                                {
                                    getline(inFile2, eid, '|');
                                    eidArr.push_back(eid);
                                    getline(inFile2, pid, '\n');
                                    pidArr.push_back(pid);
                                }
                                inFile2.close(); //closing the file
                            }

                            int count = 0;
                            cout << head1 << "| " << head2 << endl;
                            for (int i = 0; i < idArr.size()-1; i++) {
                                for (int j = 0; j < eidArr.size()-1; j++) {
                                    if (idArr[i] == eidArr[j]) {
                                        cout << idArr[i] << "|" << nameArr[i] << "|" << eidArr[j] << "|" << pidArr[j] << endl;
                                        count++;
                                    }
                                }
                                if (count == 0) {
                                    cout << idArr[i] << "|" << nameArr[i] << "||" << endl;
                                }
                                count = 0;
                            }
                        }
                    }
                    catch (string tblName) {
                        cout << "!Failed to query table " << tblName << " because it does not exist." << endl;
                    }
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to query table because no directory is currently in use." << endl;
            }
        }

        if (words[0] == "begin") {
            try {
                if (used == false) {
                    throw(used);
                }
                else {
                    string lock_file = curr_dir + "/lock";
                    if (exists_test(lock_file)) {
                        locked = true;    
                    } else {
                        string str = "touch " + lock_file;
                        const char *command = str.c_str();
                        system(command);
                    }
                    cout << "Transaction starts." << endl;
                }
            }
            catch (bool usedVar) {
                cout << "!Failed to create table because no directory is currently in use." << endl;
            }
        }

        if (words[0] == "commit") {
            try {
                if (locked == true) {
                    locked = false;
                    throw(locked);
                }
                else {
                    string lock_file = curr_dir + "/lock";
                    string str = "rm -r " + lock_file;
                    const char *command = str.c_str();
                    system(command);

                    ofstream outFile(newpath);
                    outFile << "seat int| status int" << endl;
                    for (int i = 0; i < newSeat.size()-1; i++) {
                        outFile << newSeat[i] << "|" << newStatus[i] << endl;
                    }

                    cout << "Transaction committed." << endl;
                }
            }
            catch (bool lockedVar) {
                cout << "Transaction abort." << endl;
            }
        }

        words.clear();

    } while( (words[0] != ".exit") && (words[0] != ".EXIT") );

    cout << "All done." << endl;

    return 0;
}

inline bool exists_test(const string& name) {
    ifstream f(name.c_str());
    return f.good();
}