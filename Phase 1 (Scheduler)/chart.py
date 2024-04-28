# Define a list of colors
colors =  [ "#2caffe", "#544fc5", "#00e272", "#fe6a35", "#6b8abc", "#d568fb", "#2ee0ca", "#fa4b42", "#feb56a", "#91e8e1" ]

def create_gantt_chart(processes, current_idx, ax):
    global colors
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

    # get all unique process names in processes and arrivals
    names = set([process['name'] for process in processes])
    arrivals = [process['arrived'] for process in processes]
    # add all arrived processes to names
    for arrived in arrivals:
        names.update(arrived)
    names = list(names)
    names.sort()

    # Define the y position for each process
    y_pos = {name: i for i, name in enumerate(names)}

    # Create a dictionary to map each process to a color
    process_colors = {name: colors[i % len(colors)] for i, name in enumerate(names)}

    # Set the y-axis labels
    ax.set_yticks([y + 0.25 for y in y_pos.values()])  # Add 0.25 to each y position to center the ticks
    ax.set_yticklabels(y_pos.keys())
    ax.set_ylim(-0.5, len(names))

    # Set the x-axis label
    ax.set_xticks(range(0, len(processes) + 1, 1))
    # set the x-axis size to len(processes) + 1
    ax.set_xlim(0, len(processes))


    ax.set_xlabel('Time')
    ax.set_ylabel('Processes')
    ax.grid(True)
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


