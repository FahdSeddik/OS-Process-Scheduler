import imageio
from chart import create_gantt_chart
import matplotlib.pyplot as plt
import subprocess
import threading
from PIL import Image
import io

def parse_line(line):
    line = line.strip().split()
    name = line[4]
    arrival_time = int(line[7])
    event_time = int(line[2])
    event_type = line[5]
    return name, arrival_time, event_time, event_type

def parse_logs(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        processes = []
        arrivals = {}
        start_time = 0
        end_time = 0
        for line in lines:
            if len(line.strip()) == 0 or line[0] == '#': continue
            name, arrival_time, event_time, event_type = parse_line(line)
            if event_type in ['started', 'resumed']:
                start_time = event_time
                arrivals.setdefault(arrival_time, set()).add(name)
            elif event_type in ['finished', 'stopped']:
                end_time = event_time
                processes.append({'name': name, 'start': start_time, 'end': end_time})
        return processes, arrivals
    
def update_time_steps(time_steps, processes, arrivals):
    for process in processes:
        for j in range(process['start'], process['end']):
            time_steps[j]['name'] = process['name']

    for arrival, names in arrivals.items():
        time_steps[arrival]['arrived'].extend(names)

def get_time_steps(logs_path):
    processes, arrivals = parse_logs(logs_path)
    time_steps = [{'name':'', 'arrived':[]} for _ in range(processes[-1]['end'])]
    update_time_steps(time_steps, processes, arrivals)
    return time_steps


def save_plot_as_image(i, processes, ax):
    create_gantt_chart(processes, i, ax)
    buf = io.BytesIO()
    plt.savefig(buf, format='png')
    buf.seek(0)
    img = Image.open(buf)
    return img

def save_gif(processes, ax):
    images = [save_plot_as_image(i, processes, ax) for i in range(len(processes)+1)]
    imageio.mimsave('gantt.gif', images)

def handle_algorithm_change(values, window):
    is_rr = values['algorithm'] == 'RR'
    window['quatum_time_label'].update(visible=is_rr)
    window['quantum_time'].update(visible=is_rr)
    if not is_rr:
        window['quantum_time'].update('')

def handle_submit(values, window, logs_path, gui_queue):
    algorithm = values['algorithm']
    quantum_time = values['quantum_time']
    input_file = '/mnt/' + values['input_file'][0].lower() + values['input_file'][2:]
    window['status'].update('Running', visible=True)
    
    def run_subprocess(gui_queue):
        command = ['wsl', "'./build/process_generator.out'", input_file, algorithm, quantum_time]
        subprocess.run(command, shell=True, text=True, stderr=subprocess.STDOUT)
        window['status'].update('Finished')
        processes = get_time_steps(logs_path)
        gui_queue.put((processes, len(processes)))
    
    thread = threading.Thread(target=run_subprocess, args=(gui_queue,))
    thread.start()

def handle_plot_change(processes, current_idx, fig, ax, figure_canvas_agg, window):
    create_gantt_chart(processes, current_idx, ax)
    figure_canvas_agg.draw()
    figure_canvas_agg.get_tk_widget().pack()
