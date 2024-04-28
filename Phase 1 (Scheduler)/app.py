import PySimpleGUI as sg
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from utils import save_gif, handle_algorithm_change, handle_submit, handle_plot_change
import queue
import os

# the layout of the window is as follows
layout = [
    [sg.Text('Scheduler', font=('Helvetica', 20))],
    [sg.Text('Algorithm:'), sg.Combo(['HPF', 'SRTN', 'RR'], key='algorithm', enable_events=True)],
    [sg.Text('Quantum Time:', key='quatum_time_label', visible=False), sg.InputText(key='quantum_time', visible=False)],
    [sg.Text('Input File (Processes):'), sg.InputText(key='input_file'), sg.FileBrowse()],
    [sg.Button('Submit')],
    [sg.Text('Status: Running', key='status', visible=False)],
    [sg.Canvas(key='canvas')],
    [sg.Push(), sg.Button('Previous'), sg.Button('Next'), sg.Push()],
    [sg.Push(), sg.Button('Save GIF'), sg.Push()]
]


# create the window
window = sg.Window('Scheduler', layout)

# Create a figure
fig, ax = plt.subplots()

figure_canvas_agg = None
os.chdir(os.path.dirname(__file__) + '/code')
logs_path = './logs/scheduler.log'

current_idx = None
processes = None

def main_loop(window):
    global current_idx, processes, figure_canvas_agg, fig, ax, logs_path
    gui_queue = queue.Queue()
    while True:
        event, values = window.read(timeout=100)
        if event == sg.WIN_CLOSED:
            plt.close()
            break
        
        if event == 'algorithm':
            handle_algorithm_change(values, window)
        
        if event == 'Submit':
            # check if the input file exists
            if not os.path.exists(values['input_file']):
                sg.popup('Please select an input file')
                continue
            handle_submit(values, window, logs_path, gui_queue)
            window['Submit'].update(disabled=True)
            
        try:
            processes, current_idx = gui_queue.get_nowait()
            window['Next'].update(disabled=False)
            window['Previous'].update(disabled=False)
            window['Save GIF'].update(disabled=False)
            current_idx = 1
        except queue.Empty:
            pass
        
        if processes is None:
            # disable the buttons if processes is None
            window['Next'].update(disabled=True)
            window['Previous'].update(disabled=True)
            window['Save GIF'].update(disabled=True)
            continue
        
        if not figure_canvas_agg:
            figure_canvas_agg = FigureCanvasTkAgg(fig,window['canvas'].TKCanvas)
        handle_plot_change(processes, current_idx, fig, ax, figure_canvas_agg, window)

        if event == 'Next' and current_idx < len(processes):
            current_idx += 1
            handle_plot_change(processes, current_idx, fig, ax, figure_canvas_agg, window)

        if event == 'Previous' and current_idx > 1:
            current_idx -= 1
            handle_plot_change(processes, current_idx, fig, ax, figure_canvas_agg, window)

        if event == 'Save GIF':
            save_gif(processes, ax)

main_loop(window)
