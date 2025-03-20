# Voting System in C

This is a simple command-line voting system implemented in C. It allows users to register voters and candidates, cast votes, and view election results.

## Features
- Register voters with name, voter ID, and birthdate
- Register candidates with name
- Validate voter age (must be 18 or older)
- Allow voters to cast their vote only once
- Store votes and display election results
- Sort results based on votes received

## Installation & Compilation
1. Clone the repository or download the source code:
   ```sh
   git clone https://github.com/yourusername/voting-system.git
   cd voting-system
   ```
2. Compile the program using GCC:
   ```sh
   gcc -o voting_system "c project.c"
   ```
3. Run the executable:
   ```sh
   ./voting_system
   ```

## Usage
1. Enter the number of voters and candidates.
2. Register each voter by providing their name, voter ID, and birthdate.
3. Register each candidate by providing their name.
4. Start the voting process where each voter can cast their vote.
5. View the election results after all votes are cast.

## Example Output
```
How many voters? 3
How many candidates? 2

Voter 1 Details:
Full Name: John Doe
Voter ID: 101
Date of Birth (DD MM YYYY): 15 6 2000

Candidate 1 Name: Alice
Candidate 2 Name: Bob

--- Voting Starts ---
Enter Voter ID: 101
Candidates:
1. Alice
2. Bob
Vote for candidate number: 1
Vote recorded!

--- Election Results ---
Alice got 1 votes.
Bob got 0 votes.
```

## License
This project is open-source and available under the MIT License.

## Author
Faujul Kabir - (https://github.com/Faujull)

