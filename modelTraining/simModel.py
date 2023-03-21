import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.animation import FuncAnimation
import numpy as np
import pickle
import math

AVG_en = 1 # 0 for non avg model, 1 for avg model

#   GOALS: 
#       Implement Kalman Filtering into Real Time Actuation
#       Implement a modeling simulation to view what is to be predicted of the arm's movement

test_file = '../data/CoolTerm Capture 2023-02-07 10-29-08.txt'

L = 1 # LENGTH OF ARM IN CM


# SPECIFY START/END ANGLES
buffer = np.pi/2
start = 0 + buffer
end = -np.pi/2 + buffer
step = -.5

# 
# 
# SET THE TRAJECTORY FOR THE ARM
#
#

traj = []

# sample trajectories

# traj = np.arange(buffer + start, buffer + end, step)
# traj = [i for i in np.arange(start,end,step)] + [i for i in np.arange(end,start/2,-step)] + [i for i in np.arange(start/2,end,step)] 

test_data = pd.read_csv(test_file, delimiter='\t')
emg_data_raw = test_data[test_data.columns[1]]
alpha = 0.1
expected = 0
actual = 0

# USE EMA TO SMOOTH SIGNAL BEFORE PREDICTION
emg_data = []
for pt in emg_data_raw:
    actual = alpha * pt + (1 - alpha) * actual
    emg_data += [actual]

plt.plot(emg_data_raw)
plt.plot(emg_data)
plt.legend("EMG DATA", "EMG DATA WITH EMA")
plt.show()

# import model
if(AVG_en == 0):
    pred_model = pickle.load(open('trained_model.sav', 'rb'))
    WINDOW = 100
else:
    pred_model_AVG = pickle.load(open('trained_modelAVG.sav', 'rb'))
    WINDOW = 40
prediction = []

# DYNAMIC WINDOWING 
# predictions made when drop is detected
prev_i = 0 # for comparison
gap = 1 # how much of a decrease between two idx's
idx = 0 # index for iteration
backtrack = 10 # how much to go back once window starts
count_features = 0

if(AVG_en == 0):
    while((idx+WINDOW)<len(emg_data)):
        if (gap + emg_data[idx]) < prev_i:
            print(f'WINDOW START: {idx}')
            # plt.plot(emg_data[idx-10:idx+WINDOW-10])
            # plt.show()
            prediction.append((pred_model.predict(np.array(emg_data[idx-10:idx+WINDOW-10]).reshape(1,-1))).tolist())
            prev_i = emg_data[idx]
            idx = idx+WINDOW
            count_features += 1 # feature counted
        
        else:
            prev_i = emg_data[idx]
            idx+=1
elif(AVG_en == 1):
    while((idx+WINDOW)<len(emg_data)):
        if (gap + emg_data[idx]) < prev_i:
            print(f'WINDOW START: {idx}')
            # plt.plot(emg_data[idx-10:idx+WINDOW-10])
            # plt.show()
            prediction.append((pred_model_AVG.predict(np.array(np.average(emg_data[idx-10:idx+WINDOW-10])).reshape(1,-1))).tolist())
            prev_i = emg_data[idx]
            idx = idx+WINDOW
            count_features += 1 # feature counted
        
        else:
            prev_i = emg_data[idx]
            idx+=1

print(f'NUMBER OF FEATURES COUNTED {count_features}')


flexion = 0
extension = 1
sustain = 2
rest = 3
curr_pos = []
# add_step = 100
# traj += [curr_pos]
prev_classification = rest
fromhere = start
tohere = end
atstep = step 

traj += [-np.pi for i in range(0,10)] # DISTINGUISHABLE STARTING PT
for prog,elem in enumerate(prediction):
    # if (prog % 10 == 0): # PROGRESS
        # print(f'{prog}/{len(prediction)}')
    classification = elem[0]
    if (classification == flexion) and (prev_classification != flexion):
        # curr_pos = (add_step * np.pi/180) # cos/sin needs radians
        fromhere = start
        tohere = end
        atstep = step
        prev_classification = classification
        print(f"{prog*100} FLEXION")
    elif (classification == extension) and (prev_classification != extension):
        # curr_pos = -(add_step * np.pi/180)
        fromhere = end
        tohere = start
        atstep = -step
        prev_classification = classification
        print(f"{prog*100} EXTENSION")
    else: 
        curr_pos = [tohere for i in range(0,15)]
        traj += curr_pos
        print(f"{prog*100} STAYING")
        continue

    curr_pos = [i for i in np.arange(fromhere,tohere,atstep)]
    traj += curr_pos
    

# SET UP THE ANIMATION OBJECTS
fig = plt.figure(figsize=(3, 3),dpi=80)
ax = plt.axes(xlim=(-1, 2), ylim=(-1, 2)) 
line, = ax.plot([], [], lw=3, color='black') 
circle = plt.Circle((0, 1), 0.1, color='r') 
ax.add_patch(circle)
plt.grid('on')

def animate(i):
    theta = traj[i*1]

    endx = L*np.sin(theta)
    endy = L*np.cos(theta)

    line.set_xdata(np.array([0,endx]))
    line.set_ydata(np.array([0,endy]))
    ax.set_title(f'Torque: {(.3*np.cos(theta)*50):.2f} N*m')

    circle.center = (endx, endy)

    return line,

# SAVE ANIMATION
anim = FuncAnimation(fig, animate, frames= len(traj), interval=100, blit=True) 
anim.save('moveArm.gif', writer='imagemagick')
# plt.show()
