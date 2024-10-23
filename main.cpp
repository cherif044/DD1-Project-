#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

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

struct Event {
    long long ts; // In picoseconds
    ioput input;

    // Comparator to order events by timestamsp (smallest name)
    bool operator<(const Event& other) const {
        return ts > other.ts;  // min-heap
    }
};

struct Circuit {
    vector<ioput> inputs;
    vector<ioput> outputs;
    vector<ioput> wires;
    vector<Gate> gates;
    priority_queue<Event> event_queue;
    vector<Event> simulation_results;  // To store the simulation results

    void FillEventQueue(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;  //z

            istringstream iss(line);
            char hash;
            long long ts;
            string input;
            char equals;
            int logic_value;

            // Parse #<pr> <input>=<value>
            iss >> hash >> ts >> input >> equals >> logic_value;
            if (input.back() == ';') input.pop_back();  // Remove the  semicolon

            // Create a new event
            Event new_event{ ts, ioput(input, logic_value) };

            // Add the event to the priority queue
            event_queue.push(new_event);
        }

        file.close();
    }

    // Method to process events in the queue
    void UpdateInput() {
        while (!event_queue.empty()) {
            Event current_event = event_queue.top();
            ioput current_input = current_event.input;

            // Update the input state
            for (auto& input : inputs) {
                if (input.name == current_input.name) {
                    input.state = current_input.state;
                }
            }

            // Add to simulation results
            simulation_results.push_back(current_event);

            // Process the event (for now, just print it)
            cout << "TS: " << current_event.ts
                << ", Input: " << current_event.input.name
                << ", New Value: " << current_event.input.state << endl;

            event_queue.pop();
        }
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
                    inputs.push_back(ioput(input, 0));
                }
            }
            else if (word == "output") {
                // Parse outputs
                string output;
                while (iss >> output) {
                    if (output.back() == ';') output.pop_back();  // Remove trailing semicolon
                    outputs.push_back(ioput(output, 0));
                }
            }
            else if (word == "wire") {
                // Parse wires
                string wire;
                while (iss >> wire) {
                    if (wire.back() == ';') wire.pop_back();  // Remove trailing semicolon
                    wires.push_back(ioput(wire, 0));
                }
            }
            else if (word == "and" || word == "or" || word == "not" || word == "xor" ||
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

    // external file
    void WriteSimulationResults(const string& filename) {
        ofstream simFile(filename);
        if (!simFile.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        for (const auto& event : simulation_results) {
            simFile << event.ts << ", " << event.input.name << ", " << event.input.state << "\n";
        }

        simFile.close();
    }
};

int main() {
    Circuit circuit;
    circuit.parseVerilogFile("circuit.v");
    circuit.FillEventQueue("stimulicircuit1.stim");
    circuit.UpdateInput();

    // SAve in simulation.sim file
    circuit.WriteSimulationResults("simulation.sim");

    return 0;
}   // There are a few bugs in the output, which will be solved soon

