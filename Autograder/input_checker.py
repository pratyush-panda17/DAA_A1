# Python script for checking if a given output is valid without looking at the input.

import hashlib
import os

def list_files_in_folder(folder):
    """Get the file paths of all files in a given folder."""
    return [os.path.join(folder, f) for f in os.listdir(folder) if os.path.isfile(os.path.join(folder, f))]

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


def are_files_identical(file1_path, file2_path):
    try:
        with open(file1_path, 'r') as file1, open(file2_path, 'r') as file2:
            # Use zip_longest to handle files of different lengths
            from itertools import zip_longest
            
            for line_num, (line1, line2) in enumerate(zip_longest(file1, file2), 1):
                # If one file has more lines than the other
                if line1 is None or line2 is None:
                    print(f"Files differ at line {line_num}: One file is longer than the other")
                    return False
                    
                # Compare each line
                if line1.strip() != line2.strip():
                    print(f"Files differ at line {line_num}")
                    print("Test line : " + line2)
                    print("Input Line: "+line1)
                    return False
                    
            return True
            
    except FileNotFoundError:
        print("Error: One or both files not found.")
        return False
    except Exception as e:
        print(f"Error: {e}")
        return False

scores = {1:2.5,2:2.5,3:5}

studentInputs = list_files_in_folder("./Autograder/Student_Inputs")
testCases = ['./Autograder/Outputs/output1.txt', './Autograder/Outputs/output2.txt', './Autograder/Outputs/output3.txt']
print(studentInputs)
print(testCases)


score = 0
for i in range(1,4):
    if are_files_identical(studentInputs[i-1],testCases[i-1]):
        print(f"Test Case {i} Passed")
        score += scores[i]
        continue
    print(f"Test Case {i} failed")
    
print(score)

