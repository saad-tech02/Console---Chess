 Chess Game:

A console-based two-player Chess Game implemented in C++ using Object-Oriented Programming concepts.

About The Project:

This project implements a complete chess game that can be played by two players on the same computer. The game follows standard chess rules including piece-specific movements, check detection, checkmate detection, and stalemate detection.


 Complete 8×8 chessboard display in console
All six chess pieces with correct movement rules
Turn-based gameplay (White moves first)
Check detection and warning system
Checkmate and stalemate detection
 Input validation and error handling
  Illegal move prevention
  Main menu with Play and Exit options
  Clean console interface

 OOP Concepts Demonstrated:

 **Encapsulation** | Private/protected data members with public getters/setters |
| **Inheritance** | Base `Piece` class inherited by Pawn, Rook, Knight, Bishop, Queen, King |
| **Polymorphism** | Virtual function `canMove()` overridden by each piece class |
| **Abstraction** | Abstract base class `Piece` with pure virtual functions |
| **Composition** | `Board` class contains 2D array of `Piece*` objects |
| **Constructors/Destructors** | Proper initialization and memory cleanup |


### Prerequisites

- C++ Compiler (GCC/G++ recommended)
- Windows/Linux/Mac terminal

### Installation

1. Clone the repository
```bash
git clone https://github.com/your-username/chess-game-oop.git
