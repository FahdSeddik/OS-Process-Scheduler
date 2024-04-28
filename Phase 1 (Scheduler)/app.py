import PySimpleGUI as sg
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from utils import get_time_steps, save_gif, handle_algorithm_change, handle_submit, handle_plot_change


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

logs_path = './logs/scheduler.log'

current_idx = 0
processes = []

def main_loop(window):
    
    while True:
        event, values = window.read(timeout=100)
        if event == sg.WIN_CLOSED:
            plt.close()
            break
        
        if event == 'algorithm':
            handle_algorithm_change(values, window)
        
        if event == 'Submit':
            processes, current_idx = handle_submit(values, window)
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

