import pandas as pd
import matplotlib.pyplot as plt
import os

def parse_title(file_name):
    """
    Parse the CSV file name to construct a descriptive title.
    
    Example file name: 'E_FTBS_SET2_exp_100_10.csv'
    Parsed title: 'Explicit FTBS (Schemes), SET n°2, Mode: Explicit, n=100, T=10'
    """
    # Remove the file extension
    base_name = os.path.splitext(file_name)[0]
    parts = base_name.split('_')

    # Map schemes to descriptions
    scheme_map = {
        'E': 'Explicit FTBS',
        'I': 'Implicit FTBS',
        'LW': 'Lax Wendroff',
        'Richtmyer': 'Richtmyer'
    }

    # Extract components
    try:
        scheme = scheme_map[parts[0]]  # Map scheme
        if ("SET" in parts[1]):
            set_number = int(parts[1][-1])
            n = int(parts[3])
            T = int(parts[4])
        elif ("SET" in parts[2]):
            set_number = int(parts[2][-1])
            n = int(parts[4])
            T = int(parts[5])

        set_number = f"SET n°{set_number}"  # Set number
        

    except IndexError:
        # Handle unexpected file name structure
        return "Invalid file name structure"

    # Construct the title
    return f"{scheme}, {set_number}, n={n}, T={T}"


def plot_csv(file_path, output_folder, min_height=0.2, margin_factor=0.2):
    """
    Generate a 2D plot of x vs f(x, t) for the first (t=min_t), last (t=max_t),
    and midpoint (t closest to max_t/2) time values.
    """
    # Load the CSV file
    data = pd.read_csv(file_path)

    # Strip any leading or trailing spaces from column names
    data.columns = data.columns.str.strip()

    # Extract the unique time values
    t_values = data['t'].unique()
    min_t, max_t = t_values.min(), t_values.max()
    mid_t = max_t / 2

    # Find the time value closest to t = max_t / 2
    closest_mid_t = t_values[(abs(t_values - mid_t)).argmin()]

    # Parse the title from the file name
    title = parse_title(os.path.basename(file_path))

    # Set up the figure with a 1.1:1 width-to-height ratio
    height = 6  # Define a standard height
    width = height * 1.1
    plt.figure(figsize=(width, height))

    # Plot the first time point (t = min_t)
    subset_t0 = data[data['t'] == min_t]
    plt.plot(subset_t0['x'], subset_t0['f'], color='#B74F6F', linewidth=2)

    # Plot the midpoint time (t closest to max_t / 2)
    subset_t_mid = data[data['t'] == closest_mid_t]
    plt.plot(subset_t_mid['x'], subset_t_mid['f'], color='#FB8B24', linewidth=2, linestyle='-.')

    # Plot the last time point (t = max_t)
    subset_t_last = data[data['t'] == max_t]
    plt.plot(subset_t_last['x'], subset_t_last['f'], color='#3185FC', linewidth=2, linestyle='--')

    # Customize the plot
    plt.xlabel('x')
    plt.ylabel('f(x,t)')
    plt.title(title)
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
    showing the first, last, and midpoint time points.
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
