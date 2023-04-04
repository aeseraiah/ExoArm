# CLASSIFIER USING MATCH FILTERING

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
import math
from scipy.signal import find_peaks


#
#  SPECIFY NUMBER OF TRAINING DATA FILES #################################
#


window_size = 100
alpha = 0.1

excel_file = "../data/filenames-indexes_lpfilter.xlsx" # FOR TRAINING
TRAINING_DATA_COUNT = 1

test_file = '../data/lpdata/CoolTerm Capture 2023-03-28 10-41-00.txt' # FOR MODELING

filenames = []


# GET EACH FILENAME AND CORRESPONDING INDEXES
flexion_startindexes = []
sustain_startindexes = []
extension_startindexes = []
rest_startindexes = []
for sheet in range(0, TRAINING_DATA_COUNT):
    curr_sheet = pd.read_excel(excel_file, sheet)
    filenames.append(curr_sheet['FILENAMES'][0])
    flexion_startindexes.append(((curr_sheet['FLEXION'].dropna()).astype(int)).tolist())
    sustain_startindexes.append(((curr_sheet['SUSTAIN'].dropna()).astype(int)).tolist())
    extension_startindexes.append(((curr_sheet['EXTENSION'].dropna()).astype(int)).tolist())
    rest_startindexes.append(((curr_sheet['REST'].dropna()).astype(int)).tolist())

# READ IN DATA FROM EACH FILE
data = []
for i,filename in enumerate(filenames):
    data.append(pd.read_csv(filename, delimiter='\t', usecols=[1])) # COL 1 is NON TIME COL
# 
#           DATA PREPROCESSING #################################
# 

# TURN DATA INTO NUMPY ARRAY AND SMOOTH WITH EMA
np_data = []
expected = 0
actual = 0
for curr_array in data:
    d = curr_array.iloc[:,0].to_numpy()
    emg_data_ema = []
    for pt in d:
        actual = alpha * pt + (1 - alpha) * actual
        emg_data_ema += [actual]
    np_data.append(emg_data_ema)


# ENSURE DATA SEPARATED
# for npd in np_data:
#     plt.plot(npd)
#     plt.show()
# plt.title("ALL TRAINING SIGNALS")
# plt.show()

# 
# SHOW THE SEPARATION BETWEEN FEATURES #################################
# IS NOT SHOWING THE EMA FILTERED WAVES
#

# for index_index in range(0, TRAINING_DATA_COUNT):
#     for i in extension_startindexes[index_index]:
#         plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'green')
#     for i in flexion_startindexes[index_index]:
#         plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'red')
#     for i in sustain_startindexes[index_index]:
#         plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'purple')
#     for i in rest_startindexes[index_index]:
#         plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'orange')
#     plt.title("FLEXION - RED , EXTENSION - GREEN, SUSTAIN - PURPLE, REST - ORANGE")
#     plt.show()


# GENERATE FEATURE ARRAYS
FLEXION_DATA = []
EXTENSION_DATA = []
SUSTAIN_DATA = []
REST_DATA = []
for index_index in range(0, TRAINING_DATA_COUNT):
    for i in flexion_startindexes[index_index]:
        FLEXION_DATA.append(np_data[index_index][ i : i + window_size])

    for i in extension_startindexes[index_index]:
        EXTENSION_DATA.append(np_data[index_index][ i : i + window_size])

    for i in sustain_startindexes[index_index]:
        SUSTAIN_DATA.append(np_data[index_index][ i : i + window_size])

    for i in rest_startindexes[index_index]:
        REST_DATA.append(np_data[index_index][ i : i + window_size])

# classifiers
flexion = 0
extension = 1
sustain = 2
rest = 3


# GENERATE FLEXION FILTER

FLEX_FILTER = np.arange(0,window_size)
A_FLEX_FILTER = np.arange(0,window_size)

for data in FLEXION_DATA:
    for i, el in enumerate(data):
        FLEX_FILTER[i] += el
    peaks, _ = find_peaks(data, height= 330)
    print(peaks)
    # plt.plot(peaks[0], data[peaks[0]], 'o')
    if peaks:
        peak = peaks[0]
        # plt.plot(np.arange(0,len(data)) - peak, data, color = 'r')
        new_wave = []
        if peak > 50:
            gap = peak - 50
            new_wave = data[gap:]
            np.append(new_wave , (np.zeros(gap).tolist()))
            print(new_wave)
            # plt.plot(new_wave)
            # plt.show()

        elif peak <= 50:
            gap = 50 - peak
            new_wave = np.zeros(gap)
            np.append(new_wave,data[:100-gap])
            plt.plot(new_wave)
            plt.show()



# plt.show()
for i, data in enumerate(FLEX_FILTER):
    FLEX_FILTER[i] = data/len(FLEXION_DATA)
# plt.plot(FLEX_FILTER, color = 'b')
# plt.show()


# GENERATE EXTENSION FILTER

# EXT_FILTER = np.arange(0,window_size)
# for data in EXTENSION_DATA:
#     for i, el in enumerate(data):
#         EXT_FILTER[i] += el
#     plt.plot(data, color = 'r')
# for i, data in enumerate(EXT_FILTER):
#     EXT_FILTER[i] = data/len(EXTENSION_DATA)
# plt.plot(EXT_FILTER, color = 'b')
# plt.show()

pred = []
# for d in FLEXION_DATA:
    # pred += [np.convolve(d,FLEX_FILTER, mode = 'valid')]
    # plt.plot(np.convolve(d, EXT_FILTER))
    # plt.plot(np.convolve(d, FLEX_FILTER))

# plt.plot(pred)
# plt.title("CONVOLUTION OF FLEXION FILTERS \nWITH KNOWN EXTENSION DATA")
# plt.show()

#
#   MATCH FILTERING TO CLASSIFY A FILE/MODELING
#

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

test_data = pd.read_csv(test_file, delimiter='\t')
emg_data_raw = test_data[test_data.columns[1]]
alpha = 0.1
expected = 0
actual = 0

# 
# USE EMA TO SMOOTH SIGNAL BEFORE PREDICTION
#

emg_data = []
for pt in emg_data_raw:
    actual = alpha * pt + (1 - alpha) * actual
    emg_data += [actual]

# plt.plot(emg_data_raw)
# plt.plot(emg_data)
# plt.legend("EMG DATA", "EMG DATA WITH EMA")
# plt.show()

# 
# 
# DYNAMIC WINDOWING 
# 
# predictions made when drop is detected

prev_i = 0 # for comparison
gap = 1 # how much of a decrease between two idx's
idx = 0 # index for iteration
backtrack = 10 # how much to go back once window starts
count_features = 0


while((idx+window_size)<len(emg_data)):
    if (gap + emg_data[idx]) < prev_i:
        # print(f'WINDOW START: {idx}')
        window = emg_data[idx:idx+window_size]

        # MAKE PREDICTION
        fc = np.convolve(window, FLEX_FILTER)
        ec = np.convolve(window, EXT_FILTER)
        # plt.plot(fc)
        # print(f"AVERAGE CONV FOR FLEX {np.average(fc)}")
        # print(f"AVERAGE CONV FOR EXT {np.average(ec)}")
        # plt.show()

        # SAVE PREVIOUS DATA PNT FOR FINDING GAP
        prev_i = emg_data[idx]
        idx = idx+window_size
        count_features += 1 # feature counted
    
    else:
        prev_i = emg_data[idx]
        idx+=1
# plt.show()
print(f'NUMBER OF FEATURES COUNTED {count_features}')

curr_pos = []

prev_classification = rest
fromhere = start
tohere = end
atstep = step 




# #
# #
# #   ANIMATION WORKFLOW
# #

# traj += [-np.pi for i in range(0,10)] # DISTINGUISHABLE STARTING PT
# for prog,elem in enumerate(prediction):
#     # if (prog % 10 == 0): # PROGRESS
#         # print(f'{prog}/{len(prediction)}')
#     classification = elem[0]
#     if (classification == flexion) and (prev_classification != flexion):
#         # curr_pos = (add_step * np.pi/180) # cos/sin needs radians
#         fromhere = start
#         tohere = end
#         atstep = step
#         prev_classification = classification
#         print(f"{prog*100} FLEXION")
#     elif (classification == extension) and (prev_classification != extension):
#         # curr_pos = -(add_step * np.pi/180)
#         fromhere = end
#         tohere = start
#         atstep = -step
#         prev_classification = classification
#         print(f"{prog*100} EXTENSION")
#     else: 
#         curr_pos = [tohere for i in range(0,5)]
#         traj += curr_pos
#         print(f"{prog*100} STAYING")
#         continue

#     curr_pos = [i for i in np.arange(fromhere,tohere,atstep)]
#     traj += curr_pos
    

# # SET UP THE ANIMATION OBJECTS
# fig = plt.figure(figsize=(3, 3),dpi=80)
# ax = plt.axes(xlim=(-1, 2), ylim=(-1, 2)) 
# line, = ax.plot([], [], lw=3, color='black') 
# circle = plt.Circle((0, 1), 0.1, color='r') 
# ax.add_patch(circle)
# plt.grid('on')

# def animate(i):
#     theta = traj[i*1]

#     endx = L*np.sin(theta)
#     endy = L*np.cos(theta)

#     line.set_xdata(np.array([0,endx]))
#     line.set_ydata(np.array([0,endy]))
#     ax.set_title(f'Torque: {(.3*np.cos(theta)*50):.2f} N*m')

#     circle.center = (endx, endy)

#     return line,

# # SAVE ANIMATION
# anim = FuncAnimation(fig, animate, frames= len(traj), interval=70, blit=True) 
# anim.save('moveArm.gif', writer='imagemagick')
# # plt.show()

