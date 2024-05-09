from matplotlib.ticker import MultipleLocator, AutoLocator

# Define a list of colors
colors =  [ "#2caffe", "#544fc5", "#00e272", "#fe6a35", "#6b8abc", "#d568fb", "#2ee0ca", "#fa4b42", "#feb56a", "#91e8e1" ]


def process_data(processes):
    names = set([process['name'] for process in processes if process['name'] != ''])
    arrivals = [process['arrived'] for process in processes]
    for arrived in arrivals:
        names.update(arrived) if arrived else None
    names = list(names)
    # remove empty string
    
    names.sort()
    y_pos = {name: i for i, name in enumerate(names)}
    process_colors = {name: colors[i % len(colors)] for i, name in enumerate(names)}
    return names, arrivals, y_pos, process_colors

names = None
arrivals = None
y_pos = None
process_colors = None

def create_gantt_chart(processes, current_idx, ax):
    global names, arrivals, y_pos, process_colors
    if len(processes) == 0:
        return
    
    # Clear the axes
    ax.clear()

    # Process has the following structure:
    # [
    #   { 'name': 'A', # Currently running process
    #     'arrived': ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'], # List of processes that arrived at this time
    #   },
    #   ...
    # ]

    if names is None:
        names, arrivals, y_pos, process_colors = process_data(processes)

    # Set the y-axis labels
    ax.set_yticks([y + 0.25 for y in y_pos.values()])  # Add 0.25 to each y position to center the ticks
    ax.set_yticklabels(y_pos.keys())
    ax.set_ylim(-0.5, len(names))

    # Set the x-axis label
    ax.xaxis.set_major_locator(AutoLocator())
    ax.xaxis.set_minor_locator(MultipleLocator(1))

    # set the x-axis size to len(processes) + 1
    ax.set_xlim(0, len(processes))

    ax.set_xlabel('Time')
    ax.set_ylabel('Processes')
    ax.grid(True, which='both')
    ax.set_axisbelow(True)

    # Loop through the processes
    for i, process in enumerate(processes[:current_idx]):
        # Plot a bar for the process with a color from the list
        duration = 1
        name = process['name']
        arrivals = process['arrived']
        if name:
            ax.broken_barh([(i, duration)], (y_pos[name], 0.5), facecolors=process_colors[name])
        
        if len(arrivals) > 0:
            for process in arrivals:
                ax.plot(i, y_pos[process]+0.25, 'ro')
    
    # plot vertical line at x = current_idx
    ax.axvline(x=current_idx, color='black') 


