// Link to youtube video to explain code 
// https://www.youtube.com/watch?v=WmGWJ9LnEs4

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class WirelessSignal {
public:  // Make all data members public so we can access them directly
    double power;           // Signal power in watts
    double frequency;       // Frequency in MHz
    double distance;        // Distance in meters
    string deviceType;      // "WiFi", "Bluetooth", "Radio", etc.
    
    // Default constructor
    WirelessSignal() {
        power = 0.0;
        frequency = 0.0;
        distance = 0.0;
        deviceType = "Unknown";
    }
    
    // Parameterized constructor
    WirelessSignal(double p, double f, double d, string type) {
        power = p;
        frequency = f;
        distance = d;
        deviceType = type;
    }
    
    // Simple calculation: signal strength decreases with distance
    double calculateSignalStrength() {
        if (distance == 0) return power;
        return power / (distance * distance);  // Simple inverse square law
    }
    
    // Display signal information
    void displaySignal() {
        cout << "Device: " << deviceType << endl;
        cout << "Power: " << power << " watts" << endl;
        cout << "Frequency: " << frequency << " MHz" << endl;
        cout << "Distance: " << distance << " meters" << endl;
        cout << "Signal Strength: " << calculateSignalStrength() << endl;
        cout << "------------------------" << endl;
    }
    
    // Save signal to file
    void saveToFile(ofstream& file) {
        file << deviceType << " " << power << " " << frequency << " " << distance << endl;
    }
    
    // Load signal from file
    void loadFromFile(ifstream& file) {
        file >> deviceType >> power >> frequency >> distance;
    }
};

class SignalDatabase {
public:  // Make all data members public for direct access
    WirelessSignal* signals;    // Dynamic array of signals
    int currentSize;            // Current number of signals
    int maxSize;               // Maximum capacity
    
    // Constructor
    SignalDatabase(int size) {
        maxSize = size;
        currentSize = 0;
        signals = new WirelessSignal[maxSize];  // Dynamic memory allocation
    }
    
    // Destructor
    ~SignalDatabase() {
        delete[] signals;  // Clean up dynamic memory
    }
    
    // Add a signal to the database
    bool addSignal(WirelessSignal newSignal) {
        if (currentSize < maxSize) {
            signals[currentSize] = newSignal;
            currentSize++;
            return true;
        }
        return false;  // Database is full
    }
    
    // Sort signals by frequency (needed for binary search)
    void sortByFrequency() {
        for (int i = 0; i < currentSize - 1; i++) {
            for (int j = 0; j < currentSize - i - 1; j++) {
                if (signals[j].frequency > signals[j + 1].frequency) {
                    // Swap signals
                    WirelessSignal temp = signals[j];
                    signals[j] = signals[j + 1];
                    signals[j + 1] = temp;
                }
            }
        }
    }
    
    // Binary search for signal by frequency
    int findSignalByFrequency(double targetFreq) {
        int left = 0;
        int right = currentSize - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (signals[mid].frequency == targetFreq) {
                return mid;  // Found the signal
            }
            else if (signals[mid].frequency < targetFreq) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        
        return -1;  // Signal not found
    }
    
    // Display all signals
    void displayAllSignals() {
        cout << "=== Signal Database ===" << endl;
        cout << "Total signals: " << currentSize << endl << endl;
        
        for (int i = 0; i < currentSize; i++) {
            cout << "Signal " << (i + 1) << ":" << endl;
            signals[i].displaySignal();
        }
    }
    
    // Save all signals to file
    void saveToFile(string filename) {
        ofstream file(filename);
        
        if (file.is_open()) {
            file << currentSize << endl;  // Save the count first
            
            for (int i = 0; i < currentSize; i++) {
                signals[i].saveToFile(file);
            }
            
            file.close();
            cout << "Database saved to " << filename << endl;
        }
        else {
            cout << "Error: Could not open file for writing!" << endl;
        }
    }
    
    // Load signals from file
    void loadFromFile(string filename) {
        ifstream file(filename);
        
        if (file.is_open()) {
            int count;
            file >> count;  // Read the count first
            
            currentSize = 0;  // Reset current size
            
            for (int i = 0; i < count && i < maxSize; i++) {
                WirelessSignal tempSignal;
                tempSignal.loadFromFile(file);
                addSignal(tempSignal);
            }
            
            file.close();
            cout << "Database loaded from " << filename << endl;
        }
        else {
            cout << "Error: Could not open file for reading!" << endl;
        }
    }
    
    // Find signals within a power range
    void findSignalsInPowerRange(double minPower, double maxPower) {
        cout << "Signals with power between " << minPower << " and " << maxPower << " watts:" << endl;
        bool found = false;
        
        for (int i = 0; i < currentSize; i++) {
            double power = signals[i].power;
            if (power >= minPower && power <= maxPower) {
                signals[i].displaySignal();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No signals found in that power range." << endl;
        }
    }
};

// Main program demonstrating all concepts
int main() {
    // Create a signal database with dynamic memory
    SignalDatabase* database = new SignalDatabase(10);
    
    // Add some sample signals - directly accessing public data members
    WirelessSignal wifi(0.1, 2400.0, 10.0, "WiFi");
    WirelessSignal bluetooth(0.001, 2450.0, 5.0, "Bluetooth");
    WirelessSignal radio(50.0, 101.5, 1000.0, "Radio");
    
    database->addSignal(wifi);
    database->addSignal(bluetooth);
    database->addSignal(radio);
    
    // You can also modify signals directly after adding them
    database->signals[0].power = 0.2;  // Change WiFi power
    database->signals[1].distance = 3.0; // Change Bluetooth distance
    
    // Display all signals
    database->displayAllSignals();
    
    // Demonstrate file I/O
    database->saveToFile("signals.txt");
    
    // Create another database and load from file
    SignalDatabase* loadedDatabase = new SignalDatabase(10);
    loadedDatabase->loadFromFile("signals.txt");
    
    // Sort signals by frequency (required for binary search)
    loadedDatabase->sortByFrequency();
    
    // Demonstrate binary search
    cout << "\n=== Binary Search Demo ===" << endl;
    int index = loadedDatabase->findSignalByFrequency(2400.0);
    if (index != -1) {
        cout << "Found WiFi signal at index " << index << endl;
        cout << "Device type: " << loadedDatabase->signals[index].deviceType << endl;
    }
    else {
        cout << "Signal not found" << endl;
    }
    
    // Find signals in power range
    cout << "\n=== Power Range Search ===" << endl;
    loadedDatabase->findSignalsInPowerRange(0.0, 1.0);
    
    // Demonstrate direct access to data members
    cout << "\n=== Direct Data Access ===" << endl;
    cout << "First signal frequency: " << loadedDatabase->signals[0].frequency << " MHz" << endl;
    cout << "Database current size: " << loadedDatabase->currentSize << endl;
    cout << "Database max size: " << loadedDatabase->maxSize << endl;
    
    // Clean up dynamic memory
    delete database;
    delete loadedDatabase;
    
    return 0;
}