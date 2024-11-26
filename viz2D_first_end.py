import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_csv(file_path, output_folder, min_height=0.2, margin_factor=0.2):
    """
    Generate a 2D plot of x vs f(x, t) for only the first (t=min_t) and last (t=max_t) time values.
    """
    # Load the CSV file
    data = pd.read_csv(file_path)

    # Strip any leading or trailing spaces from column names
    data.columns = data.columns.str.strip()

    # Extract the first and last time values
    t_values = data['t'].unique()
    min_t, max_t = t_values.min(), t_values.max()

    # Set up the figure
    plt.figure(figsize=(8, 4))  # Width=8 inches, Height=4 inches

    # Plot the first time point (t = min_t)
    subset_t0 = data[data['t'] == min_t]
    plt.plot(subset_t0['x'], subset_t0['f'], color='red', linewidth=2, label=f'T0: t={min_t:.2f} (initial)')

    # Plot the last time point (t = max_t)
    subset_t_last = data[data['t'] == max_t]
    plt.plot(subset_t_last['x'], subset_t_last['f'], color='blue', linewidth=2, linestyle='--', label=f'TN: t={max_t:.2f} (final)')

    # Customize the plot
    plt.xlabel('x')
    plt.ylabel('f(x,t)')
    plt.title(f'2D Plot of x vs f(x,t) for {os.path.basename(file_path)}')
    plt.legend(title="Time t", loc='upper left', bbox_to_anchor=(1, 1))
    plt.grid(True)

    # Adjust y-axis limits with a margin
    f_min, f_max = data['f'].min(), data['f'].max()
    margin = (f_max - f_min) * margin_factor
    plt.ylim(f_min - margin, f_max + margin)

    # Enforce a minimum height scale for the y-axis if specified
    if (f_max - f_min) < min_height:
        mid = (f_max + f_min) / 2
        f_min, f_max = mid - min_height / 2, mid + min_height / 2
        plt.ylim(f_min, f_max)

    # Ensure the 'Images' folder exists
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    # Save the plot in the 'Images' folder
    output_file = os.path.join(output_folder, os.path.basename(file_path).replace('.csv', '_2D_plot.png'))
    plt.tight_layout()
    plt.savefig(output_file)  # Save the plot
    print(f"Plot saved: {output_file}")

    # Clear the figure for the next plot
    plt.clf()

def main():
    """
    Process all CSV files in the 'Results' folder and generate a 2D plot for each,
    showing only the first and last time points.
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
