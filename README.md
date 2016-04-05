# projetEPP
Rush-Hour solver
# Compiling
If you don't want to save path, and just know if there is a solution comment [here](https://github.com/norips/Rush-Hour-Solver/blob/master/includes/solv.h#L16)

Go to the build folder with `cd build` and do `cmake ..` and `make`. 

# Use
Lib are placed to build/lib.
To get there, you must typing in the terminal `cd build/lib`.

Binaries are placed to build/bin.
To get there, you must typing in the terminal `cd build/bin`.

# Play
To start the solver, you have to go to the bin folder and do `./solver NbOfLoop GameType PathToLevel ` first and second parameters are required.
GameType are : 
* 0 = Ane Rouge
* 1 = Rush Hour
# Code Coverage
Go to the build folder with `cd build` and do `cmake -DCoverage=on ..` and `make && make RushHourSolver_coverage` and follow the instruction.

