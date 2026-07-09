# Pet-Management-System

The Pet Management System is a desktop automation application developed in the C programming language using the Raylib and Raygui libraries. It is designed to digitalize and streamline the registration, tracking, and database management processes for veterinary clinics or animal shelters through a modern, dynamic Graphical User Interface (GUI).
<img width="1097" height="780" alt="image" src="https://github.com/user-attachments/assets/2ec43151-01a2-4093-bd93-ecb4f8ed3686" />


Moving beyond traditional terminal-based (command-line) applications, this project delivers a fully interactive, responsive, and user-friendly experience. Under the hood, it rigorously implements core computer science concepts such as dynamic memory management, file I/O persistence, and modular software design.

🛠 Technical Features & Engineering Principles
1. Modular Architecture (Separation of Concerns)
To comply with clean code standards, the codebase is structurally divided into three distinct layers:

pet.h: The header file defining the core data model (struct Pet) and the function prototypes managing the business logic.

pet.c: The backend layer containing the implementation of database operations, sorting/shifting algorithms, and calendar validation logic.

main.c: The frontend layer solely responsible for the graphical user interface rendering loop, responsive layouts, and user event handling.

This modularity maximizes code maintainability and strictly prevents compilation errors like "multiple definition".

2. Dynamic Memory Management
Instead of relying on fixed-size arrays that risk buffer overflows or waste memory, the system employs a flexible runtime memory strategy:

Memory is dynamically allocated and resized in real-time using malloc and realloc as records are added or deleted.

Upon exiting the application, free() is explicitly called on the allocated data blocks to return all resources to the operating system, ensuring a zero memory leak execution.

3. Persistent Data Management (File I/O)
The system ensures data persistence across sessions. When launched, it scans the disk for the PetData.txt file, automatically parses the records, and populates the dynamic array in RAM. When safely closed, the updated database is formatted and written back to the disk file seamlessly.

4. Robust Calendar & Date Validation Algorithm
The vaccination tracking module features a rigorous, multi-layered validation mechanism for incoming date strings (DD/MM/YYYY):

sscanf is utilized to securely parse the string layout into separate numerical variables (day, month, year) in a single step.

Upper day boundaries are strictly checked according to the specific month (e.g., preventing 31st of April).

A Leap Year calculation algorithm is fully integrated; it checks if a year is divisible by 4 and not 100, or divisible by 400, dynamically adjusting February's limit to 29 days when applicable. Invalid dates immediately trigger explicit UI error messages.

5. Dynamic and Responsive UI Design
Responsive Layouts: Enabled by the FLAG_WINDOW_RESIZABLE flag, the window handles scaling gracefully. No matter how the user resizes the window, the placement formulas dynamically calculate boundaries using GetScreenWidth() and GetScreenHeight(), keeping forms, text, and tables perfectly centered.

UI Locking Stability: Standard standalone toggles in raygui often cause state-collision bugs in single-choice inputs (Radio Buttons). This was resolved by implementing GuiToggleGroup, which locks button behaviors on a hardware-like logical abstraction layer, making it impossible to select conflicting options simultaneously.

🎯 Functional Modules
Add New Pet: Registers new pets into the system with automated, unique, and sequential ID assignment (Auto-increment tracking).

Sort Pet List: Sorts all registered records alphabetically (A to Z) based on the pet's name using an optimized Bubble Sort implementation.

Display Pet List: Renders the active database in RAM inside a clean, grid-aligned data table with overflow protection to handle large datasets.

Delete Pet By ID: Searches for a specific unique ID. If found, it removes the entry, shifts all subsequent array elements to the left to maintain sequence, and downscales the memory block via realloc.

Vaccinate Pet By ID: Targets a pet by its unique ID, verifies the newly provided calendar date against the validation algorithm, and automatically sets the status field to "vaccinated".
<img width="1093" height="783" alt="image" src="https://github.com/user-attachments/assets/e2a61e09-cd8b-43d5-b1b9-4de67a941c9a" />
<img width="1095" height="778" alt="image" src="https://github.com/user-attachments/assets/3c05f847-d614-4bb5-b519-3c6cf5b12033" />
<img width="1094" height="780" alt="image" src="https://github.com/user-attachments/assets/6af43758-e1c6-414e-90ae-d33bb032576b" />


