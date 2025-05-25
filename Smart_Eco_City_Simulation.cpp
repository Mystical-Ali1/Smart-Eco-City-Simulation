#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <iomanip> 
using namespace std;


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

const int MAX_LOGS = 10;
const int MAX_CITIZENS = 5;
const int MAX_BUILDINGS = 5;
const int MAX_TRANSPORTS = 5;
const int MAX_POWER = 5;

void showTotalEmissions(int totalCOX, double totalSOX, double totalNOX) {
    cout << fixed << setprecision(2);
    cout << CYAN << "\n--- Total Emissions ---\n" << RESET;
    cout << RED << "COX: " << totalCOX << " g\n" << RESET;
    cout << RED << "SOX: " << totalSOX << " g\n" << RESET;
    cout << RED << "NOX: " << totalNOX << " g\n" << RESET;
}

class CityObject {
protected:
    string name;
public:
    CityObject(string n) : name(n) {}
    virtual void displayInfo() const = 0;
    virtual void operate() = 0;
    string getName() const { return name; }
    virtual ~CityObject() {}
};

class Building : public CityObject {
protected:
    int energyConsumption;
public:
    Building(string n, int energy) : CityObject(n), energyConsumption(energy) {}
    virtual void operate() {
        cout << name << " is operating as a generic building.\n";
    }
};

class Residential : public Building {
public:
    Residential(string n, int energy) : Building(n, energy) {}
    void operate() override {
        cout << GREEN << name << " is housing citizens.\n" ;
        cout << name << " (Residential) uses " << energyConsumption << " kWh.\n";
    }
    void displayInfo() const override {
        cout <<YELLOW << "Residential: " << name << ", Energy: " << energyConsumption << " kWh\n"<< RESET;
    }
};

class Commercial : public Building {
public:
    Commercial(string n, int energy) : Building(n, energy) {}
    void operate() override {
        cout << YELLOW << name << " is generating revenue using " << energyConsumption << " kWh.\n" << RESET;
    }
    void displayInfo() const override {
        cout << YELLOW << "Commercial: " << name << ", Energy: " << energyConsumption << " kWh\n"<< RESET;
    }
};

class Transport : public CityObject {
protected:
    int COXemissionRate;
    double SOXemissionRate;
    double NoxemissionRate;
public:
    Transport(string n, int em1 = 160, double em2 = 0.5, double em3 = 1.0)
        : CityObject(n), COXemissionRate(em1), SOXemissionRate(em2), NoxemissionRate(em3) {}

    virtual void move() = 0;

    virtual void displayInfo() const override {
        cout << "Transport: " << name << "\n";
    }

    void calculateEmissions(int kmDriven, int& totalCOX, double& totalSOX, double& totalNOX) {
        int cox = COXemissionRate * kmDriven;
        double sox = SOXemissionRate * kmDriven;
        double nox = NoxemissionRate * kmDriven;

        totalCOX += cox;
        totalSOX += sox;
        totalNOX += nox;

        cout << fixed << setprecision(2);
        cout << RED << "COX emission produced: " << cox << " g/km\n" ;
        cout << RED << "SOX emission produced: " << sox << " g/km\n" ;
        cout << RED << "NOX emission produced: " << nox << " g/km\n" << RESET;
    }
};

class ElectricBus : public Transport {
public:
    ElectricBus(string n) : Transport(n, 0, 0.0, 0.0) {}
    void move() override {
        cout << GREEN << name << " is gliding silently across town with zero emissions.\n" << RESET;
    }
    void operate() override {
        cout << BLUE<< name << " is running smoothly.\n"<<RESET;
    }
    void displayInfo() const override {
        cout << BLUE<< "Electric Bus: " << name << "\n"<<RESET;
    }
};

class DieselBus : public Transport {
public:
    DieselBus(string n, int em1 = 160, double em2 = 0.5, double em3 = 1.0) : Transport(n, em1, em2, em3) {}
    void move() override {
        cout << RED << name << " is belching fumes while moving.\n" << RESET;
    }
    void operate() override {
        cout <<RED << name << " is running on diesel, emitting pollutants.\n" <<RESET;
    }
    void displayInfo() const override {
        cout <<RED << "Diesel Bus: " << name << "\n" <<RESET;
    }
};

class Car : public Transport {
public:
    Car(string n) : Transport(n, 140, 0.7, 1.3) {}
    void move() override {
        cout << YELLOW << name << " is driving through the city.\n" << RESET;
    }
    void operate() override {
        cout << CYAN << name << " is driving on roads, emitting pollutants.\n"<<RESET;
    }
    void displayInfo() const override {
        cout << CYAN << "Car: " << name << "\n"<<RESET;
    }
};

class Citizen : public CityObject {
    int happiness;
public:
    Citizen(string n) : CityObject(n), happiness(50) {}
    void displayInfo() const override {
        cout << BLUE << "Citizen: " << name << ", Happiness: " << happiness << "\n";
    }
    void participate(const string& activity) {
        cout << CYAN << name << " is doing " << activity << ".\n" << RESET;
        happiness += 5;
    }
    void operate() override {
        cout << name << " is living and contributing to the city.\n"<<RESET;
    }
};

class PowerSource : public CityObject {
public:
    PowerSource(string n) : CityObject(n) {}
};

class SolarPlant : public PowerSource {
public:
    SolarPlant(string n) : PowerSource(n) {}
    void operate() override {
        cout << GREEN << name << " is generating clean solar energy.\n"<<RESET ;
    }
    void displayInfo() const override {
        cout << GREEN<< "Solar Plant: " << name << "\n"<< RESET;
    }
};

class CoalPlant : public PowerSource {
public:
    CoalPlant(string n) : PowerSource(n) {}
    void operate() override {
        cout << RED << name << " is generating dirty power with heavy CO2 output.\n"<< RESET;
    }
    void displayInfo() const override {
        cout << RED<< "Coal Plant: " << name << "\n"<< RESET;
    }
};

class Activity {
public:
    virtual void execute(Citizen& c) = 0;
    virtual ~Activity() {}
};

class TreePlanting : public Activity {
public:
    void execute(Citizen& c) override {
        c.participate("Tree Planting");
    }
};

class Swimming : public Activity {
public:
    void execute(Citizen& c) override {
        c.participate("Swimming");
    }
};

class Gym : public Activity {
public:
    void execute(Citizen& c) override {
        c.participate("Gym");
    }
};

class Walking : public Activity {
public:
    void execute(Citizen& c) override {
        c.participate("Walking");
    }
};

template <typename T>
class Log {
    T entries[MAX_LOGS];
    int count = 0;
public:
    void add(const T& e) {
        if (count < MAX_LOGS) entries[count++] = e;
        else cout << "Log full.\n";
    }
    void show() const {
        ifstream inFile("city_log.txt");
        if (!inFile) {
            cout << "No log file found.\n";
            return;
        }

        cout << "\n-- City Log --\n";
        string line;
        while (getline(inFile, line)) {
            cout << line << "\n";
        }

        inFile.close();
    }

    void saveToFile(const string& filename) const {
        ofstream outFile(filename , ios::app);
        if (!outFile) {
            cerr << "Error opening file.\n";
            return;
        }
        for (int i = 0; i < count; ++i)
            outFile << entries[i] << "\n";
        outFile.close();
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "No previous log found.\n";
            return;
        }
        string line;
        while (getline(inFile, line) && count < MAX_LOGS) {
            entries[count++] = line;
        }
        inFile.close();
    }
};

class UpgradeException : public exception {
public:
    const char* what() const noexcept override {
        return "Budget exceeded! You're trying to build Disneyland with a lemonade stand budget!";
    }
};

void upgrade(Building* b, int budget) {
    if (budget < 500)
        throw UpgradeException();
    cout << b->getName() << " upgraded successfully!\n";
}

int main() {
    Building* buildings[MAX_BUILDINGS];
    Transport* transports[MAX_TRANSPORTS];
    Citizen* citizens[MAX_CITIZENS];
    PowerSource* powers[MAX_POWER];
    int bCount = 0, tCount = 0, cCount = 0, pCount = 0;

    Log<string> log;
    bool running = true;

    int totalCOX = 0;
    double totalSOX = 0.0, totalNOX = 0.0;

    while (running) {
        cout << BLUE << "\n--- Smart Eco City ---\n" << RESET;
        cout <<GREEN<< "1. Add Building\n2. Add Bus\n3. Add Citizen\n4. Add Power Source\n";
        cout << "5. Show All Objects\n6. Upgrade Building\n7. Add Car and Drive\n";
        cout << "8. Show All Emissions\n9. Save To Log\n10.Show Log\n0. Exit\nChoice: "<<RESET;

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            if (bCount < MAX_BUILDINGS) {
                string name; int energy; int type;
                cout <<CYAN<< "Building name: "; getline(cin, name);
                cout << "Energy use: "; cin >> energy;
                cout << "Type (1. Residential, 2. Commercial): "<<RESET; cin >> type;
                cin.ignore();
                if (type == 1)
                    buildings[bCount] = new Residential(name, energy);
                else
                    buildings[bCount] = new Commercial(name, energy);
                buildings[bCount]->displayInfo();
                buildings[bCount]->operate();
                log.add("Added Building: " + name);
                bCount++;
            } else cout << YELLOW << "Building limit reached.\n" << RESET;
            break;

        case 2:
            if (tCount < MAX_TRANSPORTS) {
                string name;
                int type;
                cout <<YELLOW << "Enter Bus Name: "; getline(cin, name);
                cout << "Select Bus Type:\n1. Electric Bus\n2. Diesel Bus\nChoice: ";
                cin >> type;
                cin.ignore();
                int distance = 0;

                if (type == 1) {
                    transports[tCount] = new ElectricBus(name);
                } else {
                    cout << "Enter distance travelled in KM: "<<RESET;
                    cin >> distance;
                    cin.ignore();
                    transports[tCount] = new DieselBus(name);
                }
                
                transports[tCount]->move();
                transports[tCount]->displayInfo();
                transports[tCount]->operate();
                
                if (type == 2)
                    transports[tCount]->calculateEmissions(distance, totalCOX, totalSOX, totalNOX);

                log.add("Added Bus: " + name);
                tCount++;
            } else cout << YELLOW << "Bus limit reached.\n" << RESET;
            break;

        case 3:
            if (cCount < MAX_CITIZENS) {
                string name;
                char ans;
                cout << BLUE << "Citizen name: "; getline(cin, name);
                citizens[cCount] = new Citizen(name);
                citizens[cCount]->displayInfo();
                citizens[cCount]->operate();
                log.add("Citizen added: " + name);
                cCount++;
				cout<<RESET;
                
                do {
                cout << CYAN << "Do you want to perform an activity? (Y/N): " << RESET;
                cin >> ans;

                if (ans == 'Y' || ans == 'y') {
                int activityChoice;
                cout << GREEN << "Which activity would you like to do?\n";
                cout << "1. Swimming\n2. Gym\n3. Walking\n4. Tree Planting\nChoice: " << RESET;
                cin >> activityChoice;

        switch (activityChoice) {
            case 1:
                citizens[cCount - 1]->participate("Swimming");
                break;
            case 2:
                citizens[cCount - 1]->participate("Gym");
                break;
            case 3:
                citizens[cCount - 1]->participate("Walking");
                break;
            case 4:
                citizens[cCount-1]->participate("Tree Planting");
                break;
            default:
                cout << RED << "Invalid activity choice.\n" << RESET;
        }
    } 
            else if (ans != 'N' && ans != 'n') {
                cout << RED << "Invalid input. Please enter Y or N.\n" << RESET;
    }
            }
             while (ans != 'N' && ans != 'n');
            }
             else cout << YELLOW << "Citizen limit reached.\n" << RESET;
            break;
        
            


        case 4:
            if (pCount < MAX_POWER) {
                string name;
                int type;
                cout << CYAN << "Power Source Name: "; getline(cin, name);
                cout << "Type (1. Solar, 2. Coal): "<< RESET; cin >> type;
                cin.ignore();
                if (type == 1)
                    powers[pCount] = new SolarPlant(name);
                else
                    powers[pCount] = new CoalPlant(name);
                powers[pCount]->displayInfo();
                powers[pCount]->operate();
                log.add("Power Source added: " + name);
                pCount++;
            } else cout << YELLOW << "Power source limit reached.\n" << RESET;
            break;

        case 5:
            cout << BLUE << "\n--- Buildings ---\n" << RESET;
            for (int i = 0; i < bCount; ++i) {
                buildings[i]->displayInfo();
                buildings[i]->operate();
            }
            cout << BLUE << "\n--- Buses ---\n" << RESET;
            for (int i = 0; i < tCount; ++i) {
                transports[i]->displayInfo();
                transports[i]->move();
                transports[i]->operate();
            }
            cout << BLUE << "\n--- Citizens ---\n" << RESET;
            for (int i = 0; i < cCount; ++i) {
                citizens[i]->displayInfo();
                citizens[i]->operate();
            }
            cout << BLUE << "\n--- Power Sources ---\n" << RESET;
            for (int i = 0; i < pCount; ++i) {
                powers[i]->displayInfo();
                powers[i]->operate();
            }
            break;


        case 6:
            if (bCount > 0) {
                int id, budget;
                cout <<YELLOW<< "Choose building (0 to " << bCount - 1 << "): "; cin >> id;
                cout << "Enter budget: " <<RESET ; cin >> budget;
                try {
                    upgrade(buildings[id], budget);
                    log.add("Upgraded: " + buildings[id]->getName());
                } catch (const exception& e) {
                    cout << RED << e.what() << "\n" << RESET;
                }
            } else cout << RED << "No buildings.\n" << RESET;
            break;

                case 7:
        {
            if (tCount < MAX_TRANSPORTS) {
                string name;
                cout << CYAN << "Enter Car Name: "; 
                cin.ignore(); 
                getline(cin, name);
                transports[tCount] = new Car(name);
                cout << "Enter distance travelled in KM: " << RESET;
                int distance;
                cin >> distance;
                transports[tCount]->move();
                transports[tCount]->displayInfo();
                transports[tCount]->operate();
                transports[tCount]->calculateEmissions(distance, totalCOX, totalSOX, totalNOX);
                log.add("Added Car: " + name);
                tCount++;
            } else cout << YELLOW << "Transport limit reached.\n" << RESET;
            break;
        }

        case 8:
            showTotalEmissions(totalCOX, totalSOX, totalNOX);
            break;

        case 9:
            log.saveToFile("city_log.txt");
            cout << GREEN << "Log saved to file.\n" << RESET;
            break;

        case 10:
            log.show();
            break;

        case 0:
            running = false;
            break;

        default:
            cout << RED << "Invalid choice.\n" << RESET;
        }
    }


    for (int i = 0; i < bCount; ++i) delete buildings[i];
    for (int i = 0; i < tCount; ++i) delete transports[i];
    for (int i = 0; i < cCount; ++i) delete citizens[i];
    for (int i = 0; i < pCount; ++i) delete powers[i];

    cout << GREEN << "Exiting Smart Eco City Simulation. Goodbye!\n" << RESET;
    return 0;
}
