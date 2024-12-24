#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

std::vector<std::string> split(std::string input, char delim) {
    std::string word;
    std::vector<std::string> words;
    std::stringstream ss(input);
    while(std::getline(ss, word, delim))
        words.push_back(word);
    return words;
}

pair<map<string, bool>, vector<vector<string>>> parseFile(string fileName) {
    ifstream file(fileName);
    string line;
    map<string, bool> wires;
    vector<vector<string>> connections;
    while(getline(file, line)) {
        if (line == "")
            break;
        wires[line.substr(0, 3)] = line[5] == '1';
    }
    while(getline(file, line)) {
        vector<string> connection = split(line, ' ');
        connections.push_back({connection[0], connection[1], connection[2], connection[4]});
    }
    file.close();
    return {wires, connections};
}

bool eval(bool a, string op, bool b) {
    if (op == "AND")
        return a && b;
    if (op == "XOR")
        return a ^ b;
    else // op == "OR"
        return a || b;
}

map<string, bool> run(map<string, bool> wires, vector<vector<string>> connections) {
    queue<string> todo;
    map<string, map<string, vector<vector<string>>>> connectionMap;
    for (auto connection : connections) {
        connectionMap[connection[0]][connection[2]].push_back(connection);
        connectionMap[connection[2]][connection[0]].push_back(connection);
    }
    for (auto [wireA, _] : wires) {
        for (auto [wireB, _] : wires) {
            if (wireA != wireB) {
                for (auto connection : connectionMap[wireA][wireB]) {
                    wires[connection[3]] = eval(wires[connection[0]], connection[1], wires[connection[2]]);
                    todo.push(connection[3]);
                }
            }
        }
    }
    while (!todo.empty()) {
        string wire = todo.front();
        todo.pop();
        for (auto [connected, wireConnections] : connectionMap[wire]) {
            for (auto connection : wireConnections) {
                if (wires.contains(connected)) {
                    wires[connection[3]] = eval(wires[connection[0]], connection[1], wires[connection[2]]);
                    todo.push(connection[3]);
                }
            }
        }
    }
    return wires;
}

uint64_t getZValue(map<string, bool> wires) {
    uint64_t zValue = 0;
    for (auto [wire, value] : wires) {
        if (wire[0] == 'z') {
            uint64_t wireNumber = stoi(wire.substr(1));
            zValue += uint64_t(value) << wireNumber;
        }
    }
    return zValue;
}

string zeroPrepend(string s,int len){
	while ((int)s.size()<len){
		s="0"+s;
	}
	return s;
}

vector<vector<string>> getConnectionsByExecutionOrder(map<string, bool> wires, vector<vector<string>> connections) {
	// x registers should come before y registers when sorting
	for (auto &connection : connections)
		if (connection[0][0]=='y' && connection[2][0]=='x')
            swap(connection[0], connection[2]);
	sort(connections.begin(),connections.end());
	vector<vector<string>> sorted;
	while (!connections.empty()){
		for (int i=0; i<connections.size(); i++) {
			auto temp=connections[i];
			if (wires.contains(temp[0]) && wires.contains(temp[2])) {
				wires[temp[3]] = eval(wires[temp[0]],temp[1],wires[temp[2]]);
				sorted.push_back(temp);
				connections.erase(connections.begin() + i);
				break;
			}
		}
	}
    return sorted;
}

void checkGate(set<string>& gatesToSwap, vector<string> connection, string a, string b) {
    if (!(connection[0] == a || connection[2] == a))
        gatesToSwap.insert(a);
    if (!(connection[0] == b || connection[2] == b))
        gatesToSwap.insert(b);
}

set<string> getGatesToSwap(map<string, bool> wires, vector<vector<string>> connections) {
	connections = getConnectionsByExecutionOrder(wires, connections);
	set<string> gatesToSwap;
	// Find the first carry bit
	string pre = (connections[0][1] == "AND" ? connections[0][3] : connections[1][3]);
	for (int i=0; i<45; i++){
		if (i==0) {
			connections.erase(connections.begin());
			connections.erase(connections.begin());
			continue;
		}
		vector<vector<string>> connectionGroup;
		for (int j=0;j<5;j++) {
			connectionGroup.push_back(connections[0]);
			connections.erase(connections.begin());
		}

		// Sort the 5 connections in each group
		sort(connectionGroup.begin(), connectionGroup.end(), [](auto &a,auto &b){
			if (a[0][0]=='x' && a[1]=="AND") return true;
			if (b[0][0]=='x' && b[1]=="AND") return false;
			if (a[0][0]=='x' && a[1]=="XOR") return true;
			if (b[0][0]=='x' && b[1]=="XOR") return false;
			if (a[1]=="AND") return true;
			if (b[1]=="AND") return false;
			if (a[1]=="XOR") return true;
			if (b[1]=="XOR") return false;
			if (a[1]=="OR") return true;
			if (b[1]=="OR") return false;
			return false;
		});

		// Check if the 5 pairs of parameters are valid
        checkGate(gatesToSwap, connectionGroup[2], connectionGroup[1][3], pre);
        checkGate(gatesToSwap, connectionGroup[3], connectionGroup[1][3], pre);
        if (connectionGroup[3][3] != "z" + zeroPrepend(to_string(i), 2))
            gatesToSwap.insert(connectionGroup[3][3]);
        checkGate(gatesToSwap, connectionGroup[4], connectionGroup[0][3], connectionGroup[2][3]);
        pre = connectionGroup[4][3];
	}
    return gatesToSwap;
}

string formatGates(set<string> gates) {
    string result = "";
	for (auto it=gates.begin(); it!=gates.end(); it++){
		if (it!=gates.begin())
            result+=",";
		result += *it;
	}
    return result;
}

int main() {
    auto [wires, connections] = parseFile("input.txt");
    auto result = run(wires, connections);
    cout << "Part 1: " << getZValue(result) << endl;
    cout << "Part 2: " << formatGates(getGatesToSwap(wires, connections)) << endl;
    return 0;
}
