import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os

def plot_3d_csv(file_path, output_folder):
    """
    Reads a CSV file and generates a 3D visualization, saving the plot in the specified output folder.
    """
    # Load the data
    df = pd.read_csv(file_path)
    
    # Remove spaces from column names
    df.columns = df.columns.str.strip()
    
    # Extract columns
    x = df['x']
    t = df['t']
    f = df['f']
    
    # Create a color palette based on the f values
    norm = plt.Normalize(vmin=f.min(), vmax=f.max())  # Normalize based on f range
    colors = plt.cm.viridis(norm(f))  # Apply a color map
    
    # Create a 3D figure
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    # Plot the points with the color gradient
    sc = ax.scatter(x, t, f, c=colors, marker='o', label='Data Points')
    
    # Add a color bar to show the range of f values
    cb = plt.colorbar(plt.cm.ScalarMappable(norm=norm, cmap='viridis'), ax=ax)
    cb.set_label('f Values')
    
    # Configure labels and title
    ax.set_xlabel('x')
    ax.set_ylabel('t')
    ax.set_zlabel('f')
    ax.set_title(f'3D Visualization - {os.path.basename(file_path)}')
    
    # Save the plot in the specified output folder
    output_file = os.path.join(output_folder, os.path.basename(file_path).replace('.csv', '_3D_plot.png'))
    plt.savefig(output_file)
    print(f"Plot saved: {output_file}")
    
    # Close the figure to free memory
    plt.close(fig)

def main():
    """
    Main function to process all CSV files in the 'Results' folder and save plots in the 'Images' folder.
    """
    # Input and output folder paths
    input_folder = 'Results'
    output_folder = 'Images'

    # Ensure the output folder exists
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    if not os.path.exists(input_folder):
        print(f"The folder '{input_folder}' does not exist.")
        return
    
    # Get all CSV files in the input folder
    csv_files = [os.path.join(input_folder, f) for f in os.listdir(input_folder) if f.endswith('.csv')]
    
    if not csv_files:
        print(f"No CSV files found in the folder '{input_folder}'.")
        return
    
    # Process each CSV file
    for file in csv_files:
        print(f"Processing file: {file}")
        plot_3d_csv(file, output_folder)

if __name__ == "__main__":
    main()
