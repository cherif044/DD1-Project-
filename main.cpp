#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

//-------------------------------------------------------------------- input/output struct type 
struct ioput {
    string name;
    int state;
    ioput(string s, int i) {
        name = s;
        state = i;
    }
    ioput() {
        name = "";
        state = -1;
    }
};

struct Gate {
    string type = "";
    ioput output;
    vector<ioput> inputs;
};

//-------------------------------------------------------------------- Event struct type 
struct Event {
    long long timestamp; // In picostates
    ioput input;

    // Comparator to order events by timestamp (min-heap)
    bool operator>(const Event& other) const {
        return timestamp > other.timestamp; // Min-heap: smaller timestamp has higher priority
    }

    // Constructor
    Event(ioput n, long long timestamp) : input(n), timestamp(timestamp) {}
};

//------------------------------------------------------------------ Circuit generated from verilog 
struct Circuit {
    vector<ioput> inputs;
    vector<ioput> outputs;
    vector<ioput> wires;
    vector<Gate> gates;
    priority_queue<Event, vector<Event>, greater<Event>> event_queue; // Use min-heap
    long long last_event_timestamp; // Store the last event timestamp

//------------------------------------------ Queue of inputs to update (events) --
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
            Event new_event(ioput(input, logic_value), timestamp);

            // Add the event to the priority queue
            event_queue.push(new_event);
        }

        file.close();
    }

    ///------------------------------------------  Method to update the inputs according to the events ---------------------------------------------------------------
    void UpdateInput() {
        if (event_queue.empty()) {
            cout << "There are no events." << endl;
            return;
        }

        // Get the top event
        Event current_event = event_queue.top();

        // Check if the event has timestamp 0
        while (!event_queue.empty() && current_event.timestamp == 0) {
            event_queue.pop(); // Remove the processed event
            
            // Update corresponding inputs
            ioput current_input = current_event.input;
            for (auto& input : inputs) {
                if (input.name == current_input.name) {
                    input.state = current_input.state;
                }
            }
            cout << "Timestamp: " << current_event.timestamp
                 << ", Input: " << current_event.input.name
                 << ", New Value: " << current_event.input.state << endl;

            if (!event_queue.empty()) {
                current_event = event_queue.top(); // Get the next event
            }
        }

        // Process the next event if it's not zero
        if (!event_queue.empty()) {
            current_event = event_queue.top(); // Get the next event
            last_event_timestamp = current_event.timestamp; // Store the last event timestamp
           // event_queue.pop(); // Remove the processed event

            ioput current_input = current_event.input;
            for (auto& input : inputs) { // Look for the corresponding input
                if (input.name == (current_input.name + ",")) {
                    input.state = current_input.state; // Input updated
                    cout << "Timestamp: " << current_event.timestamp
                         << ", Input: " << current_event.input.name
                         << ", New Value: " << current_event.input.state << endl;
                    WriteSimulationResults("simulation.sim", input, current_event.timestamp);
                }
            }
        }
    }

    ///------------------------------------------  Method to output outputs
    void UpdateOutputs() {
        for (auto& gate : gates) {
            int result;
            // Determine the operation based on the gate type
            if (gate.type == "and") {
                result = 1;  // AND gate starts with true (1)
                for (const auto& input : gate.inputs) {
                    result &= input.state;
                }
            } else if (gate.type == "or") {
                result = 0;  // OR gate starts with false (0)
                for (const auto& input : gate.inputs) {
                    result |= input.state;
                }
            } else if (gate.type == "not") {
                // NOT gate has only one input
                result = !gate.inputs[0].state;
            } else if (gate.type == "xor") {
                result = gate.inputs[0].state ^ gate.inputs[1].state;
            } else if (gate.type == "nand") {
                result = 1;  // NAND starts as true (1)
                for (const auto& input : gate.inputs) {
                    result &= input.state;
                }
                result = !result;  // NAND inverts the result
            } else if (gate.type == "nor") {
                result = 0;  // NOR starts as false (0)
                for (const auto& input : gate.inputs) {
                    result |= input.state;
                }
                result = !result;  // NOR inverts the result
            } else if (gate.type == "xnor") {
                result = !(gate.inputs[0].state ^ gate.inputs[1].state);
            } else if (gate.type == "buf") {
                // BUF (buffer) just passes the input value to output
                result = gate.inputs[0].state;
            } else {
                cerr << "Unknown gate type: " << gate.type << endl;
                return;
            }

            // Update the output state if it has changed
            if (gate.output.state != result) {
                gate.output.state = result;

                // Print the gate operation result
                cout << "Gate: " << gate.type << ", Output: " << gate.output.name << ", New State: " << result << endl;
                WriteSimulationResults("simulation.sim", gate.output, last_event_timestamp + 50); // Use last_event_timestamp
            }
        }
    }

    ///------------------------------------------  Method to read verilog file 
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
                    inputs.push_back(ioput(input, 0));
                }
            } else if (word == "output") {
                // Parse outputs
                string output;
                while (iss >> output) {
                    if (output.back() == ';') output.pop_back();  // Remove trailing semicolon
                    outputs.push_back(ioput(output, 0));
                }
            } else if (word == "wire") {
                // Parse wires
                string wire;
                while (iss >> wire) {
                    if (wire.back() == ';') wire.pop_back();  // Remove trailing semicolon
                    wires.push_back(ioput(wire, 0));
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
                    gate.inputs.push_back(ioput(input, 0));
                }
                gates.push_back(gate);
            }
        }
    }

    ///------------------------------------------  Method to update input, then output(s), then pop event
    void ProcessEvent() {
        while (!event_queue.empty()) {
            UpdateInput();
           // UpdateOutputs();
            event_queue.pop();
        }
    }

    ///------------------------------------------ method to write simulation result
    void WriteSimulationResults(const string& filename, ioput updated_ioput, long long ts) {
        ofstream simFile(filename, ios::app);
        if (!simFile.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        simFile << ts << ", " << updated_ioput.name << " " << updated_ioput.state << "\n";
        simFile.close();
    }
};

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

    circuit.FillEventQueue("stimulicircuit1.stim");
   circuit.ProcessEvent(); 

    return 0;
}
