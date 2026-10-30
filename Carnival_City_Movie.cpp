// ============================================================
// Carnival City Movie Ticket Reservation System
// ------------------------------------------------------------
// Reads client/reservation records from an input file, performs
// several file-processing and reporting tasks, and writes a
// sorted report to an output file. Implemented using classes
// and arrays, and driven by a do-while / switch menu.
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <cassert>
#include <iomanip>
using namespace std;

const int MAX_CLIENTS = 100; // maximum number of records the array can hold

// ------------------------------------------------------------
// Helper function: trim
// Purpose: Removes leading and trailing whitespace from a
//          string. Used when splitting comma-separated fields,
//          since the input file has ", " (comma + space) as
//          the separator.
// ------------------------------------------------------------
string trim(const string& s) {
    const auto WHITESPACE = " \t\n\r\f\v";
    size_t start = s.find_first_not_of(WHITESPACE);  // first char that is NOT space/tab/return/newline
    size_t end = s.find_last_not_of(WHITESPACE);     // last char that is NOT space/tab/return/newline
    
    // indicates that no non - whitespace character was found.
    // returns an empty string in that case.
    if (start == string::npos) 
        return "";           // string is all whitespace
    //extracts the portion between the first and last non - whitespace characters, inclusive
    return s.substr(start, end - start + 1);        // cut from start to end 
}

// ============================================================
// Class: Client
// Purpose: Represents a single client's movie ticket reservation
//          (surname, name, movie title, movie type, seat number,
//          price, and reference number).
// ============================================================
class Client {
private:
    string surname;
    string firstName;
    string movieTitle;
    string movieType;
    string seatNumber;
    double price;
    string refNumber;

public:
    // Default constructor: initialises price to 0
    //Client() : price(0.0) {}
    Client()
	{
		surname = "";
		firstName = "";
		movieTitle = "";
		movieType = "";
		seatNumber = "";
		price = 0.0;
		refNumber = "";
    }

    // ---- Setters: used to populate a Client object ----
    void setSurname(const string& s) 
    { 
        this->surname = s; 
    }
    void setFirstName(const string& s) 
    { 
        this->firstName = s; 
    }
    void setMovieTitle(const string& s) 
    { 
        this->movieTitle = s; 
    }
    void setMovieType(const string& s) 
    { 
        this->movieType = s; 
    }
    void setSeatNumber(const string& s) 
    { 
        this->seatNumber = s; 
    }
    void setPrice(double p) 
    { 
        this->price = p; 
    }
    void setRefNumber(const string& s) 
    { 
        this->refNumber = s; 
    }

    // ---- Getters: used to read a Client object's data ----
    string getSurname() const
    { 
        return surname; 
    }
    string getFirstName() const 
    { 
        return firstName; 
    }
    string getMovieTitle() const 
    {
        return movieTitle; 
    }
    string getMovieType() const 
    { 
        return movieType; 
    }
    string getSeatNumber() const 
    { 
        return seatNumber; 
    }
    double getPrice() const 
    { 
        return price; 
    }
    string getRefNumber() const 
    { 
        return refNumber; 
    }
};

// ============================================================
// Class: ReservationSystem
// Purpose: Encapsulates the array of Client records and all
//          the file-processing / reporting operations required
//          by the system.
// ============================================================
class ReservationSystem {
private:
    Client clients[MAX_CLIENTS]; // 1D array of Client objects
    int count;                   // number of records actually loaded
    string inputFileName;
    string outputFileName;

public:
    // Constructor: no records loaded yet
    //ReservationSystem() : count(0) {}
    ReservationSystem()
    {
        count = 0;
        inputFileName = "";
        outputFileName = ""; 
    }
    // --------------------------------------------------------
    // Function: setFileNames
    // Purpose: Prompts the user for the input and output file
    //          names and stores them for later use.
    // --------------------------------------------------------
    void setFileNames() 
    {
        cout << "Please enter input file name (e.g. clients_info.dat): ";
        cin >> inputFileName;
        cout << "Please enter output file name (e.g. movie_report.txt): ";
        cin >> outputFileName;
    }

    // --------------------------------------------------------
    // Function: validateFile
    // Purpose: Checks whether a given file can be opened.
    //          Returns true if valid/openable, false otherwise.
    // --------------------------------------------------------
    bool validateFile(const string& fileName) 
    {
        ifstream file(fileName.c_str());
        if (!file.is_open()) 
        {
            cout << "Error: could not open file \"" << fileName << "\".\n";
            return false;
        }
        file.close();
        return true;
    }

    // --------------------------------------------------------
    // Function: readClientsFromFile
    // Purpose: Reads the input file CHARACTER BY CHARACTER,
    //          rebuilding each line, then splits that line on
    //          commas to populate a Client object. This fulfils
    //          the requirement to read the file character by
    //          character rather than using getline() directly.
    // --------------------------------------------------------
    void readClientsFromFile() 
    {
        if (!validateFile(inputFileName)) 
            return;

        ifstream file(inputFileName.c_str());
        char ch;
        string line = "";
        count = 0;

        // Read the file one character at a time
        while (file.get(ch)) {
            if (ch == '\n') {
                // A full line has been built - process it
                if (!line.empty() && count < MAX_CLIENTS) {
                    parseLine(line);
                    count++;
                }
                line = ""; // reset for the next line
            }
            else if (ch != '\r') {
                line += ch; // append character to current line
            }
        }

        // Handle the last line if the file does not end with '\n'
        if (!line.empty() && count < MAX_CLIENTS) {
            parseLine(line);
            count++;
        }

        file.close();
        cout << count << " client record(s) loaded successfully.\n";
    }

    // --------------------------------------------------------
    // Function: parseLine
    // Purpose: Splits one comma-separated line of text into its
    //          7 fields and stores them in a Client object,
    //          which is then added to the clients array.
    // --------------------------------------------------------
    void parseLine(const string& line) {
        stringstream ss(line);
        string field;
        string fields[7];
        int idx = 0;

        while (getline(ss, field, ',') && idx < 7) {
            fields[idx] = trim(field);
            idx++;
        }
        if (idx < 7) return; // malformed line - skip it

        Client c;
        c.setSurname(fields[0]);
        c.setFirstName(fields[1]);
        c.setMovieTitle(fields[2]);
        c.setMovieType(fields[3]);
        c.setSeatNumber(fields[4]);
        c.setPrice(atof(fields[5].c_str()));
        c.setRefNumber(fields[6]);

        clients[count] = c;
    }

    // --------------------------------------------------------
    // Function: countLines
    // Purpose: Counts and returns the number of lines in the
    //          input file.
    // --------------------------------------------------------
    int countLines() {
        if (!validateFile(inputFileName)) return 0;

        ifstream file(inputFileName.c_str());
        char ch;
        int lines = 0;
        bool lastCharWasNewline = true; // tracks whether file ends without '\n'

        while (file.get(ch)) {
            if (ch == '\n') {
                lines++;
                lastCharWasNewline = true;
            }
            else {
                lastCharWasNewline = false;
            }
        }
        // count the final line if the file doesn't end with a newline
        if (!lastCharWasNewline) lines++;

        file.close();
        return lines;
    }

    // --------------------------------------------------------
    // Function: countWords
    // Purpose: Counts and returns the number of words in the
    //          input file. A "word" is any text bordered by
    //          whitespace (space, comma, tab, newline) or the
    //          start/end of the file.
    // --------------------------------------------------------
    int countWords() {
        if (!validateFile(inputFileName)) return 0;

        ifstream file(inputFileName.c_str());
        char ch;
        int words = 0;
        bool insideWord = false;

        while (file.get(ch)) {
            // Treat space, comma, tab, \r and \n as word separators
            bool isSeparator = (ch == ' ' || ch == ',' || ch == '\t' ||
                ch == '\r' || ch == '\n');
            if (!isSeparator) {
                if (!insideWord) {
                    words++;       // start of a new word
                    insideWord = true;
                }
            }
            else {
                insideWord = false; // we are now between words
            }
        }

        file.close();
        return words;
    }

    // --------------------------------------------------------
    // Function: promptForCharacter
    // Purpose: Asks the user which character they want to
    //          search for and returns it. Kept separate from
    //          the counting function as required.
    // --------------------------------------------------------
    char promptForCharacter() {
        char ch;
        cout << "Enter the character to search for: ";
        cin >> ch;
        return ch;
    }

    // --------------------------------------------------------
    // Function: countCharacterOccurrences
    // Purpose: Opens the input file, counts how many times the
    //          given character occurs (case-insensitive), closes
    //          the file, and returns the count.
    // --------------------------------------------------------
    int countCharacterOccurrences(char target) {
        if (!validateFile(inputFileName)) return 0;

        ifstream file(inputFileName.c_str());
        char ch;
        int occurrences = 0;
        char targetLower = tolower(target);

        while (file.get(ch)) {
            if (tolower(ch) == targetLower) {
                occurrences++;
            }
        }

        file.close(); // file is closed before returning, as required
        return occurrences;
    }

    // --------------------------------------------------------
    // Function: characterSearchLoop
    // Purpose: Repeatedly prompts the user for a character to
    //          search for, reports the occurrence count, and
    //          asks (yes/no) whether to search again. An assert
    //          terminates the program if the user enters
    //          anything other than 'y' or 'n'.
    // --------------------------------------------------------
    void characterSearchLoop() {
        char answer;
        do {
            char target = promptForCharacter();
            int result = countCharacterOccurrences(target);
            cout << "The character '" << target << "' occurs "
                << result << " time(s) in the file (case-insensitive).\n";

            cout << "Search for another character? (y/n): ";
            cin >> answer;
            answer = tolower(answer);

            // Terminate the program if the answer is not 'y' or 'n'
            assert(answer == 'y' || answer == 'n');

        } while (answer == 'y');
    }

    // --------------------------------------------------------
    // Function: replaceFirstTwoDigits
    // Purpose: Replaces the first two characters of a reference
    //          number with a new two-character string supplied
    //          by the caller, and returns the updated reference
    //          number.
    // --------------------------------------------------------
    string replaceFirstTwoDigits(const string& refNumber, const string& newDigits) {
        string updated = refNumber;
        if (updated.length() >= 2 && newDigits.length() >= 2) {
            updated[0] = newDigits[0];
            updated[1] = newDigits[1];
        }
        return updated;
    }

    // --------------------------------------------------------
    // Function: replaceReferenceMenu
    // Purpose: Lets the user pick a client record (by index) and
    //          replace the first two digits of that client's
    //          reference number, then updates the array.
    // --------------------------------------------------------
    void replaceReferenceMenu() {
        if (count == 0) {
            cout << "No records loaded yet.\n";
            return;
        }
        displayAllRecords();
        int index;
        cout << "Enter the record number to update (1 to " << count << "): ";
        cin >> index;
        if (index < 1 || index > count) {
            cout << "Invalid record number.\n";
            return;
        }
        string newDigits;
        cout << "Enter the two new digits: ";
        cin >> newDigits;

        string oldRef = clients[index - 1].getRefNumber();
        string newRef = replaceFirstTwoDigits(oldRef, newDigits);
        clients[index - 1].setRefNumber(newRef);

        cout << "Reference number updated: " << oldRef << " -> " << newRef << endl;
    }

    // --------------------------------------------------------
    // Function: displayAllRecords
    // Purpose: Prints a numbered list of all loaded records so
    //          the user can select one (used by the reference
    //          number replacement menu).
    // --------------------------------------------------------
    void displayAllRecords() {
        for (int i = 0; i < count; i++) {
            cout << (i + 1) << ". " << clients[i].getSurname() << ", "
                << clients[i].getFirstName() << " - "
                << clients[i].getRefNumber() << endl;
        }
    }

    // --------------------------------------------------------
    // Function: countMovieType
    // Purpose: Counts and returns the number of reservations
    //          that match a given movie type (e.g. "Action").
    // --------------------------------------------------------
    int countMovieType(const string& type) {
        int total = 0;
        for (int i = 0; i < count; i++) {
            if (equalsIgnoreCase(clients[i].getMovieType(), type)) {
                total++;
            }
        }
        return total;
    }

    // --------------------------------------------------------
    // Function: totalAmountForType
    // Purpose: Calculates and returns the total amount (rand
    //          value) paid for a given movie type.
    // --------------------------------------------------------
    double totalAmountForType(const string& type) {
        double total = 0.0;
        for (int i = 0; i < count; i++) {
            if (equalsIgnoreCase(clients[i].getMovieType(), type)) {
                total += clients[i].getPrice();
            }
        }
        return total;
    }

    // --------------------------------------------------------
    // Function: totalAmountAll
    // Purpose: Calculates and returns the total amount of all
    //          movie reservations combined.
    // --------------------------------------------------------
    double totalAmountAll() {
        double total = 0.0;
        for (int i = 0; i < count; i++) {
            total += clients[i].getPrice();
        }
        return total;
    }

    // --------------------------------------------------------
    // Function: equalsIgnoreCase
    // Purpose: Compares two strings for equality, ignoring case.
    //          Used so movie-type comparisons like "action" and
    //          "Action" are treated the same.
    // --------------------------------------------------------
    bool equalsIgnoreCase(const string& a, const string& b) {
        if (a.length() != b.length()) return false;
        for (size_t i = 0; i < a.length(); i++) {
            if (tolower(a[i]) != tolower(b[i])) return false;
        }
        return true;
    }

    // --------------------------------------------------------
    // Function: showMovieTypeStats
    // Purpose: Displays the count of reservations and the total
    //          amount for each of the 3 movie types (Action,
    //          Horror, Animation), and the grand total.
    // --------------------------------------------------------
    void showMovieTypeStats() {
        string types[3] = { "Action", "Horror", "Animation" };
        cout << fixed << setprecision(2);
        for (int i = 0; i < 3; i++) {
            cout << types[i] << ": " << countMovieType(types[i])
                << " reservation(s), total R" << totalAmountForType(types[i]) << endl;
        }
        cout << "Grand total (all movie types): R" << totalAmountAll() << endl;
    }

    // --------------------------------------------------------
    // Function: generateReport
    // Purpose: Sorts the client records in DESCENDING order by
    //          surname, then writes each client's name, surname,
    //          movie type, reference number, and the total
    //          amount paid for that movie type to the output
    //          file (movie_report.txt).
    // --------------------------------------------------------
    void generateReport() {
        if (count == 0) {
            cout << "No records loaded - nothing to report.\n";
            return;
        }

        // Work on a copy so the original array order is preserved
        Client sorted[MAX_CLIENTS];
        for (int i = 0; i < count; i++) sorted[i] = clients[i];

        // Simple bubble sort - descending order by surname
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - 1 - i; j++) {
                if (sorted[j].getSurname() < sorted[j + 1].getSurname()) {
                    Client temp = sorted[j];
                    sorted[j] = sorted[j + 1];
                    sorted[j + 1] = temp;
                }
            }
        }

        ofstream outFile(outputFileName.c_str());
        if (!outFile.is_open()) {
            cout << "Error: could not create output file.\n";
            return;
        }

        outFile << left << setw(15) << "Name" << setw(15) << "Surname"
            << setw(12) << "MovieType" << setw(15) << "RefNumber"
            << "TotalForType\n";
        outFile << "--------------------------------------------------------------\n";

        outFile << fixed << setprecision(2);
        for (int i = 0; i < count; i++) {
            double typeTotal = totalAmountForType(sorted[i].getMovieType());
            outFile << left << setw(15) << sorted[i].getFirstName()
                << setw(15) << sorted[i].getSurname()
                << setw(12) << sorted[i].getMovieType()
                << setw(15) << sorted[i].getRefNumber()
                << "R" << typeTotal << "\n";
        }

        outFile.close();
        cout << "Report written to \"" << outputFileName << "\" successfully.\n";
    }
};

// ============================================================
// Function: displayMenu
// Purpose: Prints the main menu options to the console.
// ============================================================
void displayMenu() {
    cout << "\n========= Carnival City Reservation System =========\n";
    cout << "1. Load and validate client data from input file\n";
    cout << "2. Count number of lines in the input file\n";
    cout << "3. Count number of words in the input file\n";
    cout << "4. Count occurrences of a character (search loop)\n";
    cout << "5. Replace first two digits of a reference number\n";
    cout << "6. Count number of reservations for a movie type\n";
    cout << "7. Show totals per movie type and grand total\n";
    cout << "8. Generate movie_report.txt (sorted by surname, descending)\n";
    cout << "0. Exit\n";
    cout << "======================================================\n";
    cout << "Enter your choice: ";
}

// ============================================================
// Function: main
// Purpose: Entry point. Sets up the file names, then drives the
//          program using a do-while loop and a switch statement
//          to call the appropriate ReservationSystem function.
// ============================================================
int main() {
    ReservationSystem system;
    system.setFileNames();

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            system.readClientsFromFile();
            break;
        case 2:
            cout << "Number of lines in file: " << system.countLines() << endl;
            break;
        case 3:
            cout << "Number of words in file: " << system.countWords() << endl;
            break;
        case 4:
            system.characterSearchLoop();
            break;
        case 5:
            system.replaceReferenceMenu();
            break;
        case 6: {
            string type;
            cout << "Enter movie type (Action / Horror / Animation): ";
            cin >> type;
            cout << type << " reservation count: " << system.countMovieType(type) << endl;
            break;
        }
        case 7:
            system.showMovieTypeStats();
            break;
        case 8:
            system.generateReport();
            break;
        case 0:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
