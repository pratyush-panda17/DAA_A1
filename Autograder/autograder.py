# import subprocess
# import os,glob

# student_folders = {
#     "student1": "student1_folder",
#     "student2": "student2_folder"
# }

# # List of test cases (input-output pairs)
# test_cases = [
#     ("5\n", "25\n"),  # Example: Square of a number
#     ("10\n", "100\n")
# ]

# def compile_code(student_folder):
#     """Compiles all C/C++ files in the folder and returns the executable path if successful."""
#     c_files = glob.glob(os.path.join(student_folder, "*.c"))
#     cpp_files = glob.glob(os.path.join(student_folder, "*.cpp"))

#     if not c_files and not cpp_files:
#         print(f"No source files found in {student_folder}")
#         return None

#     executable = os.path.join(student_folder, "program")  # Output binary name

#     # Determine compilation command
#     if c_files:
#         compile_cmd = ["gcc", "-o", executable] + c_files
#     else:
#         compile_cmd = ["g++", "-o", executable] + cpp_files

#     result = subprocess.run(compile_cmd, capture_output=True, text=True)
#     if result.returncode != 0:
#         print(f"Compilation failed for {student_folder}:\n{result.stderr}")
#         return None
#     return executable



def run_test_case(executable, input_data, expected_output):
    """Runs a compiled program with a test case and checks its output."""
    try:
        result = subprocess.run(
            [executable], input=input_data, text=True, capture_output=True, timeout=2
        )
        return result.stdout.strip() == expected_output.strip(), result.stdout.strip()
    except Exception as e:
        return False, str(e)

def grade_submissions(student_folders, test_cases):
    """Grades student submissions based on test cases."""
    scores = {}
    for student, folder in student_folders.items():
        print(f"Grading {student}'s submission ({folder})...")
        executable = compile_code(folder)
        if not executable:
            scores[student] = 0  # Compilation failed, score is 0
            continue
        
        score = 0
        for input_data, expected_output in test_cases:
            is_correct, output = run_test_case(executable, input_data, expected_output)
            if is_correct:
                score += 1
            else:
                print(f"Test failed for {student}: Expected {expected_output.strip()}, Got {output.strip()}")

        scores[student] = score
        os.remove(executable)  # Clean up the compiled file after grading

    return scores

grades = grade_submissions(student_folders, test_cases)
print("Final Scores:", grades)