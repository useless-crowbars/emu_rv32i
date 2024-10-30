import os

mem_start = 0x10000000
mem_size = 4 * 1024 * 1024

sections = {
    'text':   (0x10000000, 'text.bin'),
    'data':   (0x10040000, 'data.bin'),
    'bss':    (0x10050000, None),
    'rodata': (0x10100000, 'rodata.bin'),
}

memory_file = bytearray(mem_size)

for section, (start_addr, file_name) in sections.items():
    start_index = start_addr - mem_start
    
    if file_name is not None:
        with open(file_name, 'rb') as f:
            data = f.read()
            memory_file[start_index:start_index + len(data)] = data
    else:
        end_index = start_index + (1 * 1024 * 1024)
        memory_file[start_index:end_index] = bytes(1 * 1024 * 1024)

with open("memory_representation.bin", "wb") as f:
    f.write(memory_file)

print("4MB memory representation file created.")
