import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.animation import FuncAnimation
import numpy as np
import pickle
import math

#   GOALS: 
#       Implement Kalman Filtering into Real Time Actuation
#       Implement a modeling simulation to view what is to be predicted of the arm's movement

test_file = '../data/CoolTerm Capture 2023-02-07 10-29-08.txt'

L = 2 # LENGTH OF ARM


buffer = np.pi/2
start = 0 + buffer
end = -np.pi/2 + buffer
step = -.1

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
emg_data = test_data[test_data.columns[1]]

# import model
pred_model = pickle.load(open('trained_model.sav', 'rb'))
prediction = []

# FOR NOT AVG MODEL
emg_data_tohundr = len(emg_data) - len(emg_data) % +100

WINDOW = 100 # NOT AVG
# for i in range(0,emg_data_tohundr,WINDOW):
#     prediction.append((pred_model.predict(np.array(emg_data[i:i+WINDOW]).reshape(1,-1))).tolist())

# DYNAMIC WINDOWING
prev_i = 0 # for comparison
gap = 5 # how much of a decrease between two idx's
idx = 0 # index for iteration
backtrack = 10 # how much to go back once window starts
count_features = 0
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

print(f'NUMBER OF FEATURES COUNTED {count_features}')
# # FOR AVG MODEL
# WINDOW = 35  # AVG
# for i in range(0,emg_data_tohundr,WINDOW):
#     avg_mag = np.average(emg_data[i:i+WINDOW])
#     prediction.append((pred_model.predict(np.array(avg_mag).reshape(-1,1))).tolist())

# print(prediction)


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
fig = plt.figure(figsize=(4, 4),dpi=80)
ax = plt.axes(xlim=(-1, 3), ylim=(-1, 3)) 
line, = ax.plot([], [], lw=3, color='black') 
circle = plt.Circle((0, 1), 0.05, color='r') 
ax.add_patch(circle)
plt.grid('on')
plt.title("Move dat arm")

def animate(i):
    theta = traj[i*1]

    endx = L*np.sin(theta)
    endy = L*np.cos(theta)

    line.set_xdata(np.array([0,endx]))
    line.set_ydata(np.array([0,endy]))

    circle.center = (endx, endy)

    return line,

# SAVE ANIMATION
# anim = FuncAnimation(fig, animate, frames= len(traj), interval=200, blit=True) 
# anim.save('moveArm.gif', writer='imagemagick')
# plt.show()
