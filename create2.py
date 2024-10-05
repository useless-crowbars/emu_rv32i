import os

mem_size = 4 * 1024 * 1024

memory_file = bytearray(mem_size)
for i in range(mem_size):
    if i % 2:
        memory_file[i] = 0xff
    else:
        memory_file[i] = 0xff

with open("memory0xffff.bin", "wb") as f:
    f.write(memory_file)

print("4MB memory representation file created.")
