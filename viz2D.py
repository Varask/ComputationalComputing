import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np
from matplotlib.cm import ScalarMappable
from matplotlib.colors import Normalize

def plot_csv(file_path, output_folder, min_height=0.2, margin_factor=0.2):
    """
    Generate a 2D plot of x vs f(x, t) for different time values (t) from a CSV file,
    ensuring the initial condition (t=0) is prominently displayed and a gradient is added to other curves.
    """
    # Load the CSV file
    data = pd.read_csv(file_path)

    # Strip any leading or trailing spaces from column names
    data.columns = data.columns.str.strip()

    # Normalize time values for colormap scaling
    t_values = data['t'].unique()
    min_t, max_t = t_values.min(), t_values.max()
    norm = Normalize(vmin=min_t, vmax=max_t)
    cmap = plt.get_cmap('plasma')  # Use a high-contrast colormap

    # Set up the figure
    fig, ax = plt.subplots(figsize=(8, 4))  # Create figure and axes

    # Plot the initial condition (t = min_t) prominently
    subset_t0 = data[data['t'] == min_t]
    ax.plot(subset_t0['x'], subset_t0['f'], color='red', linewidth=3)  # Thicker line for visibility

    # Plot the rest with polychrome gradient and thicker lines
    for t in t_values[1:]:
        subset = data[data['t'] == t]
        color = cmap(norm(t))  # Get color from the colormap
        ax.plot(subset['x'], subset['f'], color=color, linestyle='--', linewidth=1.5)  # Thicker dashed lines

    # Add a colorbar to indicate the mapping of time values to colors
    sm = ScalarMappable(cmap=cmap, norm=norm)
    sm.set_array([])
    cbar = fig.colorbar(sm, ax=ax, label='Time t')  # Associate colorbar with the axes

    # Customize the plot
    ax.set_xlabel('x')
    ax.set_ylabel('f(x,t)')
    ax.set_title(f'2D Plot of x vs f(x,t) for {os.path.basename(file_path)}')
    ax.grid(True)

    # Adjust y-axis limits with a margin
    f_min, f_max = data['f'].min(), data['f'].max()
    margin = (f_max - f_min) * margin_factor
    ax.set_ylim(f_min - margin, f_max + margin)

    # Enforce a minimum height scale for the y-axis if specified
    if (f_max - f_min) < min_height:
        mid = (f_max + f_min) / 2
        f_min, f_max = mid - min_height / 2, mid + min_height / 2
        ax.set_ylim(f_min, f_max)

    # Ensure the 'Images' folder exists
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    # Save the plot in the 'Images' folder
    output_file = os.path.join(output_folder, os.path.basename(file_path).replace('.csv', '_2D_plot.png'))
    fig.tight_layout()
    fig.savefig(output_file)  # Save the plot
    print(f"Plot saved: {output_file}")

    # Clear the figure for the next plot
    plt.close(fig)

def main():
    """
    Process all CSV files in the 'Results' folder and generate a 2D plot for each,
    ensuring the initial condition (t=0) is prominently displayed.
    """
    # Input and output folder paths
    input_folder = 'Results'
    output_folder = os.path.join(input_folder, 'Images')

    if not os.path.exists(input_folder):
        print(f"The folder '{input_folder}' does not exist.")
        return

    csv_files = [os.path.join(input_folder, f) for f in os.listdir(input_folder) if f.endswith('.csv')]

    if not csv_files:
        print(f"No CSV files found in the folder '{input_folder}'.")
        return

    for file in csv_files:
        print(f"Processing file: {file}")
        plot_csv(file, output_folder)

if __name__ == "__main__":
    main()
