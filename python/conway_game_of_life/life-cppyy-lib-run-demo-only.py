import cppyy
from array import array
import time

# This is like life-cppyy.py, except we load a library (which may have been optimized differently) and only call the run_glider_demo_game()  function.

start = time.perf_counter()

cppyy.include("lifelib/rundemos.hpp")
cppyy.load_library("lifelib/liblifelib")

setup_time_elapsed_ms = (time.perf_counter() - start) * 1000.0
start_simulation = time.perf_counter()

n = cppyy.gbl.run_glider_demo_game();

simulation_time_elapsed_ms = (time.perf_counter() - start_simulation) * 1000.0

print(f'{n} cells alive')

total_time_elapsed_ms = (time.perf_counter() - start) * 1000.0

print('')
print(f'setup      {setup_time_elapsed_ms:>10.2f} ms')
print(f'simulation {simulation_time_elapsed_ms:>10.2f} ms')
print(f'total      {total_time_elapsed_ms:>10.2f} ms')

