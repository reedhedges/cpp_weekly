import cppyy
from array import array
import time

# This is like life-cppyy.py, except the creation of the initial Automata instance, 
# the call to add_glider(), and the game run loop is all in one C++ function 
# run_glider_demo_game(), rather than separate calls from Python.

start = time.perf_counter()

cppyy.include("life-cppyy.hpp")

setup_time_elapsed_ms = (time.perf_counter() - start) * 1000.0
start_simulation = time.perf_counter()

obj = cppyy.gbl.run_glider_demo_game();

simulation_time_elapsed_ms = (time.perf_counter() - start_simulation) * 1000.0
start_output = time.perf_counter()

for y in range(obj.height):
    for x in range(obj.width):
        if obj.get(cppyy.gbl.Automata.Point(x, y)):
            print("X", end="")
        else:
            print(".", end="")
    print()

output_time_elapsed_ms = (time.perf_counter() - start_output) * 1000.0
total_time_elapsed_ms = (time.perf_counter() - start) * 1000.0

print('')
print(f'setup      {setup_time_elapsed_ms:>10.2f} ms')
print(f'simulation {simulation_time_elapsed_ms:>10.2f} ms')
print(f'output     {output_time_elapsed_ms:>10.2f} ms')
print(f'total      {total_time_elapsed_ms:>10.2f} ms')

