# Python script for checking if a given output is valid without looking at the input.

def isDominated(layer1,layer2):
    for point1 in layer1:
        counter = 0
        for point2 in layer2:
            if point2[0]>=point1[0] and point2[1]>=point1[1]:
                counter+=1
                break
        if counter==0:
            return False
        
    return True
    

def checker(layers,n):
    for i in range(1,n):
        if not isDominated(layers[i],layers[i-1]):
            return False
    return True

def read_blocks(filename):
    with open(filename, 'r') as file:
        blocks = []
        current_block = []

        for line in file:
            line = line.strip()
            if not line:  # Empty line indicates a new block
                if current_block:  # Only add non-empty blocks
                    blocks.append(current_block)
                    current_block = []
            else:
                x, y = map(int, line.split())  # Convert to integers
                current_block.append((x, y))

        if current_block:  # Append last block if file doesn't end with a newline
            blocks.append(current_block)

    return blocks

def count_blocks(filename):
    with open(filename, 'r') as file:
        block_count = 0
        inside_block = False  # Flag to track if we're inside a block

        for line in file:
            line = line.strip()
            if line:  # If line is not empty, we are inside a block
                if not inside_block:
                    block_count += 1  # Start of a new block
                    inside_block = True
            else:
                inside_block = False  # Empty line indicates end of block

    return block_count

filename = "Outputs/output3.txt"  

blocks = read_blocks(filename)
n = count_blocks(filename)

# for i, block in enumerate(blocks):
#     print(f"Block {i+1}: {block}")

print(checker(blocks,n))
