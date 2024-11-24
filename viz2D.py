import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_csv(file_path, min_height=0.2):
    """
    Generate a 2D plot of x vs f(x, t) for different time values (t) from a CSV file,
    ensuring the initial condition (t=0) is prominently displayed.
    """
    # Load the CSV file
    data = pd.read_csv(file_path)

    # Strip any leading or trailing spaces from column names
    data.columns = data.columns.str.strip()

    # Normalize time values for transparency scaling
    t_values = data['t'].unique()
    min_t, max_t = t_values.min(), t_values.max()

    # Set up the figure
    plt.figure(figsize=(8, 4))  # Width=8 inches, Height=4 inches

    # Plot the initial condition (t = min_t) prominently
    subset_t0 = data[data['t'] == min_t]
    plt.plot(subset_t0['x'], subset_t0['f'], color='red', linewidth=2, label=f'T0: t={min_t:.2f} (initial)')

    # Plot the rest with lighter colors and variable opacity
    for t in t_values[1:]:
        subset = data[data['t'] == t]
        alpha = (t - min_t) / (max_t - min_t)  # Normalize to range [0, 1]
        plt.plot(subset['x'], subset['f'], color='blue', alpha=alpha, linestyle='--', label=f't={t:.2f}')

    # Customize the plot
    plt.xlabel('x')
    plt.ylabel('f(x,t)')
    plt.title(f'2D Plot of x vs f(x,t) for {os.path.basename(file_path)}')
    plt.legend(title="Time t", loc='upper left', bbox_to_anchor=(1, 1))
    plt.grid(True)

    # Enforce a minimum height scale for the y-axis
    f_min, f_max = data['f'].min(), data['f'].max()
    if (f_max - f_min) < min_height:
        mid = (f_max + f_min) / 2
        f_min, f_max = mid - min_height / 2, mid + min_height / 2
    plt.ylim(f_min, f_max)

    # Adjust layout and save the plot
    plt.tight_layout()
    output_file = os.path.splitext(file_path)[0] + "_2D_plot.png"
    plt.savefig(output_file, dpi=300)  # Save at 300 DPI for high quality
    print(f"Plot saved: {output_file}")

    # Clear the figure for the next plot
    plt.clf()

def main():
    """
    Process all CSV files in the 'Results' folder and generate a 2D plot for each,
    ensuring the initial condition (t=0) is prominently displayed.
    """
    folder = 'Results'
    if not os.path.exists(folder):
        print(f"The folder '{folder}' does not exist.")
        return

    csv_files = [os.path.join(folder, f) for f in os.listdir(folder) if f.endswith('.csv')]

    if not csv_files:
        print(f"No CSV files found in the folder '{folder}'.")
        return

    for file in csv_files:
        print(f"Processing file: {file}")
        plot_csv(file)

if __name__ == "__main__":
    main()
