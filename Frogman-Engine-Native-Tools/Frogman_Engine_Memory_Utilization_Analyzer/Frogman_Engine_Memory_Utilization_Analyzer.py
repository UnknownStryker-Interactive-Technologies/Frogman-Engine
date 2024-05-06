import matplotlib.pyplot as plt
import pandas as pd
import os




def initialize():
    os.system('cls' if os.name == 'nt' else 'clear')
    print("- Welcome to Frogman Engine Memory Utilization Analyzer - version: 2023.05.31 release build\n\n")




def change_directory(path_p):
    try:
        os.chdir(path_p)
        print(f"Your Current working directory has changed to: {os.getcwd()}")

    except  FileNotFoundError:
        print("Error: Could not find the directory.")




def create_folder(foldername_p):
    try:
        os.mkdir(foldername_p)
        print(f"Created a new folder named '{foldername_p}'.")

    except  FileExistsError:
        print(f"Error: {foldername_p}' is an already existing folder.")




def find_csv(filename_p):
    l_is_file_found = False

    if  filename_p.endswith('.csv'):
        l_is_file_found = True
        l_csv_file = pd.read_csv(filename_p)
        l_frames = l_csv_file.columns[0]
        l_labels = l_csv_file.columns[1:]

        l_idx = 0
        while   l_idx < len(l_labels):
            plt.plot( l_csv_file[ l_frames ], l_csv_file[ l_labels[l_idx] ] )
            l_idx += 1
            
        plt.legend(l_labels)
        plt.xlabel('Elapsed Frames')
        plt.ylabel('Frogman Engine Memory Utilization In Bytes')
        plt.title(f"Frogman Engine Memory Utilization Graph: {filename_p}")

        plt.show()

    elif    not l_is_file_found:
        print(f"Could not find the CSV file '{filename_p}'. You need to add .csv file extension to the file name.")




def export_csv(filename_p):
    l_is_file_found = False

    if  filename_p.endswith('.csv'):
        l_is_file_found = True
        l_csv_file = pd.read_csv(filename_p)
        l_frames = l_csv_file.columns[0]
        l_labels = l_csv_file.columns[1:]

        l_idx = 0
        while   l_idx < len(l_labels):
            plt.plot( l_csv_file[ l_frames ], l_csv_file[ l_labels[l_idx] ] )
            l_idx += 1
            
        plt.legend(l_labels)
        plt.xlabel('Elapsed Frames')
        plt.ylabel('Frogman Engine Memory Utilization In Bytes')
        plt.title(f"Frogman Engine Memory Utilization Graph: {filename_p}")

        plt.savefig(f'{filename_p}.png', dpi=1200)
        print(f"The Graph is exported as a '{filename_p}.png' file")

    elif    not l_is_file_found:
        print(f"Could not find the CSV file '{filename_p}'. You need to add .csv file extension to the file name.")




def main():

    initialize()

    while True:
        l_command = input("Enter a command: ").split()

        if not l_command:
            continue

        elif l_command[0] == 'pwd' and len(l_command) == 1:
            print(os.getcwd())

        elif l_command[0] == 'cd' and len(l_command) == 2:
            change_directory(l_command[1])

        elif l_command[0] == 'mkdir' and len(l_command) == 2:
            create_folder(l_command[1])

        elif l_command[0] == 'find' and len(l_command) == 2:
            find_csv(l_command[1])

        elif l_command[0] == 'export' and len(l_command) == 2:
            export_csv(l_command[1])

        elif l_command[0] == 'clear' and len(l_command) == 1:
            os.system('cls' if os.name == 'nt' else 'clear')

        elif l_command[0] == 'help' and len(l_command) == 1:
            print("pwd - it prints the current working directory.\ncd - it changes the current working directory.\nmkdir - it creates a new directory.\nfind - it searches a CSV file and draws a graph on the screen.\nexport - it searches a CSV file and exports a graph as a .png file.\nclear - it clears the screen\nhelp - it lists all commands\nexit - it terminates the application process.")

        elif l_command[0] == 'exit' and len(l_command) == 1:
            print("Terminating the application process.")
            break

        else:
            print("Warning: invalid command.")




main()