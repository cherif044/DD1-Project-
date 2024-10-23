#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>;

using namespace std;

struct ioput{
    string name;
    int state;
    ioput(string s, int i){
        name =s;
        state =i;
    }
    ioput(){
        name = "";
        state = -1;
    }
};

struct Gate {
    string type="";
    ioput output;
    vector<ioput> inputs;
};


struct Event {
    long long timestamp; // In picostates
    ioput input;

    // Comparator to order events by timestamp (smallest name)
    bool operator<(const Event& other) const {
        return timestamp > other.timestamp;  // Min-heap: smaller timestamp has higher priority
    }
};


struct Circuit {
    vector<ioput> inputs;
    vector<ioput> outputs;
    vector<ioput> wires;
    vector<Gate> gates;
    priority_queue<Event> event_queue;


    void FillEventQueue(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;  // Skip empty lines

            istringstream iss(line);
            char hash;
            long long timestamp;
            string input;
            char equals;
            int logic_value;

            // Parse the line in the format: #<timestamp> <input>=<value>;
            iss >> hash >> timestamp >> input >> equals >> logic_value;
            if (input.back() == ';') input.pop_back();  // Remove the trailing semicolon

            // Create a new event
            Event new_event{timestamp, ioput(input,logic_value)};

            // Add the event to the priority queue
            event_queue.push(new_event);
        }

        file.close();
    }

    // Method to process events in the queue (example)
    void UpdateInput() {
        if(!event_queue.empty()) cout<<"there are no events.";
        else if(event_queue.top().timestamp == 0)
        while (event_queue.top().timestamp == 0) {
            Event current_event = event_queue.top();
            ioput current_input = current_event.input;
            for(auto input: inputs){
                if(input.name == current_input.name) input.state = current_input.state;
            }
            
            event_queue.pop();

            // Process the event (for now, just print it)
            cout << "Timestamp: " << current_event.timestamp
                 << ", Input: " << current_event.input.name
                 << ", New Value: " << current_event.input.state << endl;
        }
        else{
            Event current_event = event_queue.top();
            ioput current_input = current_event.input;
            for(auto input: inputs){
                if(input.name == current_input.name) input.state = current_input.state;
            }
            
            event_queue.pop();

            // Process the event (for now, just print it)
            cout << "Timestamp: " << current_event.timestamp
                 << ", Input: " << current_event.input.name
                 << ", New Value: " << current_event.input.state << endl;
        }
        }

    void ReadGate(){

    }

    void UpdateOutputs(){

    }
    void parseVerilogFile(const string& filename) {
    
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string word;

        iss >> word;
        if (word == "input") {
            // Parse inputs
            string input;
            while (iss >> input) {
                if (input.back() == ';') input.pop_back();  // Remove trailing semicolon
                inputs.push_back(ioput(input,0));
            }
        } else if (word == "output") {
            // Parse outputs
            string output;
            while (iss >> output) {
                if (output.back() == ';') output.pop_back();  // Remove trailing semicolon
                outputs.push_back(ioput(output,0));
            }
        } else if (word == "wire") {
            // Parse wires
            string wire;
            while (iss >> wire) {
                if (wire.back() == ';') wire.pop_back();  // Remove trailing semicolon
                wires.push_back(ioput(wire,0));
            }
        } else if (word == "and" || word == "or" || word == "not" || word == "xor" ||
                   word == "nand" || word == "nor" || word == "xnor" || word == "buf") {
            // Parse gates
            Gate gate;
            gate.type = word;
            string output, input;
            iss >> output;
            if (output.back() == ',') output.pop_back();  // Remove trailing comma
            gate.output.name = output;
            while (iss >> input) {
                if (input.back() == ',' || input.back() == ';') input.pop_back();  // Remove trailing comma or semicolon
                gate.inputs.push_back(ioput(input,0));
            }
            gates.push_back(gate);
        }
    }
    }
};

// void ReadStim(const string& filename){

//   }




int main() {
    Circuit circuit;
    circuit.parseVerilogFile("circuit.v");

    // Print parsed data
    cout << "Inputs: ";
    for (const auto& input : circuit.inputs) cout << input.name << " ";
    cout << "\nOutputs: ";
    for (const auto& output : circuit.outputs) cout << output.name << " ";
    cout << "\nWires: ";
    for (const auto& wire : circuit.wires) cout << wire.name << " ";
    cout << "\nGates:\n";
    for (const auto& gate : circuit.gates) {
        cout << gate.type << " " << gate.output.name << " (";
        for (const auto& input : gate.inputs) cout << input.name << " ";
        cout << ")\n";
    }

    return 0;
}
