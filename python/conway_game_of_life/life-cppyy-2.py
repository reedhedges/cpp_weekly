import cppyy
from array import array
import time


# This is like life-cppyy.py, but the game loop is in a C++ function rather than here in Python.

start = time.perf_counter()

cppyy.include("life-cppyy.hpp")


born = cppyy.gbl.std.vector[bool](
    (False, False, False, True, False, False, False, False, False)
)
survives = cppyy.gbl.std.vector[bool](
    (False, False, True, True, False, False, False, False, False)
)

obj = cppyy.gbl.Automata(40, 20, born, survives)

obj.add_glider(cppyy.gbl.Automata.Point(0, 18))

setup_time_elapsed_ms = (time.perf_counter() - start) * 1000.0
start_simulation = time.perf_counter()

obj = cppyy.gbl.run_demo_game(obj);

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

