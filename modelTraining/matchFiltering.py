# CLASSIFIER USING MATCH FILTERING

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
import math
from scipy.signal import find_peaks
from scipy.signal import convolve

def align_signal(data):
    # may have to align for flex and for extend
    # convolve and take the best of the two

    ## ALIGN FOR FLEXION
    max_height = PEAK_HEIGHT_FLX
    peaks, _ = find_peaks(data, height= max_height) # find maximum peak
    if len(peaks)>0:
        peak = peaks[0]
        # print(f'flx peak: {peak}')
        # plt.plot(np.arange(0,len(data)) - peak, data, color = 'r')
        if peak > MID:
            gap = peak - MID
            end_buffer = [DC_COMPONENT for i in range(gap)]
            flex_new_wave = data[gap:]
            flex_new_wave = np.append(flex_new_wave , end_buffer)

        elif peak <= MID:
            gap = MID - peak
            end_buffer = [DC_COMPONENT for i in range(gap)]
            flex_new_wave = end_buffer
            flex_new_wave = np.append(flex_new_wave , data[:100-gap])
    else:
        flex_new_wave = data # IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE

    ## ALIGN FOR EXTEND
    max_height = PEAK_HEIGHT_EXT
    peaks, _ = find_peaks([-val for val in data], height= -max_height) # negate data to find minimum
    if len(peaks)>0:
        peak = peaks[0]
        # print(f'ext peak: {peak}')
        # plt.plot(np.arange(0,len(data)) - peak, data, color = 'r')
        if peak > MID:
            gap = peak - MID
            end_buffer = [DC_COMPONENT for i in range(gap)]
            ext_new_wave = data[gap:]
            ext_new_wave = np.append(ext_new_wave , end_buffer)

        elif peak <= MID:
            gap = MID - peak
            end_buffer = [DC_COMPONENT for i in range(gap)]
            ext_new_wave = end_buffer
            ext_new_wave = np.append(ext_new_wave , data[:100-gap])
    else:
        ext_new_wave = data # IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE

    return flex_new_wave, ext_new_wave

def generate_data_lists(excel_file, TRAINING_DATA_COUNT):
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
    for npd in np_data:
        plt.plot(npd)
        # plt.show()
    plt.title("ALL TRAINING SIGNALS")
    plt.xlabel("Time(ms)")
    plt.ylabel("EMG Amplitude (mV)")
    plt.show()

    # 
    # SHOW THE SEPARATION BETWEEN FEATURES #################################
    # IS NOT SHOWING THE EMA FILTERED WAVES
    #

    for index_index in range(0, TRAINING_DATA_COUNT):
        for i in extension_startindexes[index_index]:
            plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'green')
        for i in flexion_startindexes[index_index]:
            plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'red')
        for i in sustain_startindexes[index_index]:
            plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'purple')
        for i in rest_startindexes[index_index]:
            plt.plot(data[index_index].iloc[:,0][ int(i) : int(i) + window_size], color = 'orange')
        plt.title("FLEXION - RED , EXTENSION - GREEN, SUSTAIN - PURPLE, REST - ORANGE")
        plt.show()


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


    return FLEXION_DATA, EXTENSION_DATA, REST_DATA, SUSTAIN_DATA

def my_convolve(A, B):
    CONVOLUTION = [0 for x in range(100)]
    for i in range(100):
        CONVOLUTION[i] = 0
        for j in range(100):
            CONVOLUTION[i] += (A[i-j] * B[j])
        
    return CONVOLUTION

def match_filter_prediction(d):
    # THIS IS CONVERTED TO ARDUINO CODE/CPP

    flx_aligned, ext_aligned = align_signal(d) # ALIGN AS YOU CONVOLVE
    out_flx_aligned = my_convolve(flx_aligned, A_FLEX_FILTER) # CONVOLVE FLX ALIGNED W/ FLX FILTER & EXT ALIGNED W/ EXT FILTER
    out_ext_aligned = my_convolve(flx_aligned, A_EXT_FILTER)
    # out_rst_aligned = my_convolve(flx_aligned, A_RST_FILTER) # CONVOLVE RST FILTER WITH EITHER ALIGNMENT
    # out_ext_aligned = my_convolve(ext_aligned, A_EXT_FILTER)
    # out_rst_aligned = my_convolve(ext_aligned, A_RST_FILTER) # CONVOLVE RST FILTER WITH EITHER ALIGNMENT

    # print(f"FLX CONV {out_flx_aligned}")
    # print(f"EXT CONV {out_ext_aligned}")
    # print(f"RST CONV {out_rst_aligned}")
    plt.plot(out_flx_aligned, 'r')
    plt.plot(out_ext_aligned, 'b')
    # plt.plot(out_rst_aligned, 'y')

    MAX_FLX = np.max(out_flx_aligned)
    MAX_EXT = np.max(out_ext_aligned)
    # MAX_RST = np.max(out_rst_aligned)


    PRED = np.max([MAX_FLX, MAX_EXT])

    print(f'\n{MAX_FLX:.2e}, {MAX_EXT:.2e}')
    # AVG_DIFF = np.average(np.abs(np.diff([MAX_FLX, MAX_EXT, MAX_RST])))


    # DIFF_EXT_RST = np.diff([MAX_RST, MAX_EXT])
    # DIFF_FLX_RST = np.diff([MAX_RST, MAX_FLX])

    # AVG_DIFF = np.average([DIFF_EXT_RST, DIFF_FLX_RST])
    # # print(f'AVG DIFFERENCE: {AVG_DIFF}')
    # # print(DIFF_FLX_RST)
    # # print(DIFF_EXT_RST)
    # # if (DIFF_FLX_RST < AVG_DIFF) and (DIFF_EXT_RST < AVG_DIFF):
    # if AVG_DIFF < POOR_PREDICTION_THRESHOLD:
    #     PRED = MAX_RST

    flexion = 0
    extension = 1
    sustain = 2
    rest = 3

    if PRED == MAX_FLX:
        print(f'PREDICT FLX {np.max(out_flx_aligned):.2e}')
        return flexion
    elif PRED == MAX_EXT:
        print(f'PREDICT EXT {np.max(out_flx_aligned):.2e}')
        return extension
    # elif PRED == MAX_RST: # REST CASE
    #     print(f'PREDICT RST {np.max(out_rst_aligned):.2e}')
    #     return rest
    else:
        return -1

#
#  SPECIFY NUMBER OF TRAINING DATA FILES #################################
#




# plot filter in EEPROM
EEPROM_flex = [23,23,23,23,23,23,23,23,23,23,23,23,23,22,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,22,22,21,22,21,20,22,22,22,22,22,22,22,22,23,23,25,30,36,33,32,30,27,25,23,20,16,14,14,16,20,20,20,19,19,18,15,14,14,15,17,15,16,17,18,19,20,20,21,19,19,20,19,20,21,21,22,23,24,23,23,22,23,24,23,23,23,24]
EEPROM_ext = [23,23,23,23,23,23,23,23,23,23,22,23,23,23,23,23,22,23,23,23,22,23,23,23,22,23,22,23,23,23,24,24,25,26,25,26,27,27,26,28,27,26,26,24,23,22,20,18,14,13,10,12,13,14,17,16,16,18,19,20,20,20,21,22,22,22,22,22,22,22,22,22,22,22,23,23,23,22,22,23,22,22,22,23,23,23,23,23,23,22,22,22,23,23,23,23,23,23,23,23]
# plt.plot(EEPROM_flex)
# plt.plot(EEPROM_ext)
# plt.legend(["EEPROM FLEXION", "EEPROM EXTENSION"])
# plt.show()


# CONSTANTS
window_size = 100
alpha = 0.3
DC_COMPONENT = 60 # unfiltered: 300 , used for edge cases
# DC_COMPONENT = 0 # filtered: 0

MID = window_size//2 # AT WHAT POINT TO ALIGN PEAKS, 50 is halfway into window

PEAK_HEIGHT_FLX = 64
# PEAK_HEIGHT_FLX = 335
PEAK_HEIGHT_EXT = 55 # WILL BE NEGATED
# PEAK_HEIGHT_EXT = 295 # WILL BE NEGATED

# POOR_PREDICTION_THRESHOLD = 850000


# excel_file = "../data/filenames-indexes_lpfilter.xlsx" # FOR TRAINING
# excel_file = "../data/filenames-indexes.xlsx" # FOR TRAINING
excel_file = "../data/filenames-indexes_hplp.xlsx" # FOR TRAINING
TRAINING_DATA_COUNT = 1 # WILL DO THE FIRST ONE IN THE EXCEL SPREADSHEET
# test_file_realtime = '../data/CoolTerm Capture 2023-02-13 18-31-26.txt' # FOR MODELING REALLY GOOD ACCURACY
test_file_realtime = '../data/lp_hp_Data/CoolTerm Capture 2023-04-19 14-57-55.txt'


filenames = []

FLEXION_DATA, EXTENSION_DATA, REST_DATA, SUSTAIN_DATA = generate_data_lists(excel_file, TRAINING_DATA_COUNT)
REST_DATA += SUSTAIN_DATA # COMBINE THE TWO, SAME CONCEPT


# classifier key
flexion = 0
extension = 1
sustain = 2
rest = 3


# ---------GENERATE FLEXION FILTER

FLEX_FILTER = np.arange(0,window_size)
A_FLEX_FILTER = np.arange(0,window_size)

for y, data in enumerate(FLEXION_DATA):
    # SAVE UNALIGNED FILTER TO RUNNING AVG
    for i, el in enumerate(data):
        FLEX_FILTER[i] += el

    # CREATE HIGH PEAK ALIGNED FILTER, SAVE FILTER TO RUNNING AVG
    flex_new_wave, _ = align_signal(data)
    plt.plot(flex_new_wave)
    for i, el in enumerate(flex_new_wave):
        A_FLEX_FILTER[i] += el

plt.title("ALL FLEX SIGNALS")
plt.show() 

# ---------GENERATE EXTENSION FILTER

EXT_FILTER = np.arange(0,window_size)
A_EXT_FILTER = np.arange(0,window_size)

for y, data in enumerate(EXTENSION_DATA):
    # SAVE UNALIGNED FILTER TO RUNNING AVG
    for i, el in enumerate(data):
        EXT_FILTER[i] += el

    # CREATE HIGH PEAK ALIGNED FILTER, SAVE FILTER TO RUNNING AVG
    ext_new_wave, _ = align_signal(data) # align signal like flexion for extension
    # _, ext_new_wave = align_signal(data)
    plt.plot(ext_new_wave)
    for i, el in enumerate(ext_new_wave):
        A_EXT_FILTER[i] += el
plt.title("ALL EXT SIGNALS")
plt.ylim((0,100))
plt.show() 

# ---------GENERATE REST FILTER

RST_FILTER = np.arange(0,window_size)
A_RST_FILTER = np.arange(0,window_size)

for y, data in enumerate(REST_DATA):
    # SAVE UNALIGNED FILTER TO RUNNING AVG
    for i, el in enumerate(data):
        RST_FILTER[i] += el

    # CREATE HIGH PEAK ALIGNED FILTER, SAVE FILTER TO RUNNING AVG
    rst_new_wave, _ = align_signal(data) # IF NO PEAKS, SHOULD MOST LIKELY BE REST, ORIGINAL SIGNAL, EITHER OUTPUT VALID
    plt.plot(rst_new_wave) # USE FOR PRUNING DATA
    # plt.title(y+1)
    # plt.ylim((250,350))
    # plt.show()
    for i, el in enumerate(rst_new_wave):
        A_RST_FILTER[i] += el

plt.title("ALL RST SIGNALS")
plt.ylim((0,100))
plt.show() 

#
# ---------AVERAGE FILTERS
#
for i, data in enumerate(FLEX_FILTER):
    FLEX_FILTER[i] = data/len(FLEXION_DATA)
    A_FLEX_FILTER[i] = A_FLEX_FILTER[i]/len(FLEXION_DATA)

for i, data in enumerate(EXT_FILTER):
    EXT_FILTER[i] = data/len(EXTENSION_DATA)
    A_EXT_FILTER[i] = A_EXT_FILTER[i]/len(EXTENSION_DATA)

for i, data in enumerate(RST_FILTER):
    RST_FILTER[i] = data/len(REST_DATA)
    A_RST_FILTER[i] = A_RST_FILTER[i]/len(REST_DATA)

# A_RST_FILTER = [0 for i in range(0,len(A_RST_FILTER))]

#
# ---------PLOT FILTERS FOR FLEXION, EXTENSION, REST
#

CHANGE_DC_OFFSET = DC_COMPONENT - 24

li = [(i - CHANGE_DC_OFFSET) for i in FLEX_FILTER]
print(li)
# plt.plot(FLEX_FILTER, color = 'b')
plt.plot(A_FLEX_FILTER, color = 'r')
plt.legend(["ALIGNED"])
# plt.legend(["UNALIGNED", "ALIGNED"])
plt.title("FLEXION FILTERS")
plt.ylim((0,100))
plt.show()

li = [(i - CHANGE_DC_OFFSET) for i in EXT_FILTER]
print(li)
# plt.plot(EXT_FILTER, color = 'b')
plt.plot(A_EXT_FILTER, color = 'r')
plt.legend(["ALIGNED"])
# plt.legend(["UNALIGNED", "ALIGNED"])
plt.title("EXTENSION FILTERS")
plt.ylim((0,100))
plt.show()

li = [(i - CHANGE_DC_OFFSET) for i in RST_FILTER]
print(li)
# plt.plot(RST_FILTER, color = 'b')
plt.plot(A_RST_FILTER, color = 'r')
plt.legend(["ALIGNED"])
# plt.legend(["UNALIGNED", "ALIGNED"])
plt.title("REST FILTERS")
plt.ylim((0,100))
plt.show()


#
# ---------TEST PREDICTIONS WITH SELECTED DATA, NOT REAL-TIME SIMULATION
#

pred = []
PASS = 0 # KEEP TRACK OF PASS/FAIL FOR ACCURATION
FAIL = 0 # KEEP TRACK OF PASS/FAIL FOR ACCURATION
COUNT = len(EXTENSION_DATA) + len(FLEXION_DATA) + len(REST_DATA)


# print('\n\nEXTENSION DATA')
# for d in EXTENSION_DATA:

#     PRED = match_filter_prediction(d)

#     if PRED == extension:
#         print(f"PASS: PREDICT EXT")
#         PASS += 1
#     elif PRED == flexion:
#         print(f'FAIL: PREDICTION FLX')
#         FAIL += 1
#     elif PRED == rest:
#         print(f'FAIL: PREDICTION RST')
#         FAIL += 1

# # plt.title("FLX (red) , EXT (blue) & RST (yellow) \n on EXTENSION DATA ")
# # plt.show()

# print('\n\nREST DATA')
# for d in REST_DATA:

#     PRED = match_filter_prediction(d)


#     if PRED == extension:
#         print(f"FAIL: PREDICT EXT")
#         FAIL += 1
#     elif PRED == flexion:
#         print(f'FAIL: PREDICTION FLX')
#         FAIL += 1
#     elif PRED == rest:
#         print(f'PASS: PREDICTION RST')
#         PASS += 1

# # plt.title("FLX (red) , EXT (blue) & RST (yellow) \n on REST DATA ")
# # plt.show()

# print('\n\nFLEXION DATA')
# for d in FLEXION_DATA:

#     PRED = match_filter_prediction(d)

#     if PRED == extension:
#         print(f"FAIL: PREDICT EXT")
#         FAIL += 1
#     elif PRED == flexion:
#         print(f'PASS: PREDICTION FLX')
#         PASS += 1
#     elif PRED == rest:
#         print(f'FAIL: PREDICTION RST')
#         FAIL += 1

# # plt.title("FLX (red), EXT (blue) & RST (yellow) \n on FLEXION DATA ")
# # plt.show()

# print(f'PASS/FAIL: {PASS}/{FAIL}')
# print(f'ACCURACY: {100*(PASS/COUNT):.2f}%')



#
# ---------MATCH FILTERING TO CLASSIFY A FILE/MODELING LIKE REAL-TIME
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

test_data = pd.read_csv(test_file_realtime, delimiter='\t')
emg_data_raw = test_data[test_data.columns[1]]
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
# plt.legend(["EMG DATA", "EMG DATA WITH EMA"])
# plt.show()

# 
# 
# DYNAMIC WINDOWING 
# 
# predictions made when drop is detected

prev_i = 0 # for comparison
gap = 8 # how much of a decrease between two idx's
idx = 15 # index for iteration
backtrack = 0 # how much to go back once window starts
count_features = 0


while((idx+window_size)<len(emg_data)):
    if ((gap + emg_data[idx]) < prev_i) or (-gap +emg_data[idx] > prev_i):
        # print(f'WINDOW START: {idx}')
        window = emg_data[idx:idx+window_size]
        plt.plot(window)
        aligned_window , _ = align_signal(window)
        plt.plot(aligned_window)
        # plt.plot(FLEX_FILTER)
        # plt.plot(EXT_FILTER)
        plt.title(f"MOVEMENT DETECTED")
        # plt.title(f"WINDOW START {idx}")
        plt.ylim(0,100)
        # plt.legend(["UNALIGNED WINDOW", "CURRENT WINDOW", "FLEX FILTER", "EXTEND FILTER"])
        plt.legend(["UNALIGNED WINDOW", "ALIGNED WINDOW"])
        plt.show()
        
        PRED = match_filter_prediction(window)
            
        if PRED == flexion:
            print(f'XXXX: PREDICT FLX')
            title = "PREDICT FLEXION"
        elif PRED == extension:
            print(f'XXXX: PREDICT EXT')
            title = "PREDICT EXTENSION"
        elif PRED == rest: # REST CASE
            print(f'XXXX: PREDICT RST')
            title = "PREDICT REST"

        plt.legend(["FLEX", "EXTEND", "REST"])
        plt.title(title)
        plt.show()

        # SAVE PREVIOUS DATA PNT FOR FINDING GAP
        prev_i = emg_data[idx]
        idx = idx+window_size
        count_features += 1 # feature counted
        # break #### ONLY RUN ONCE, REMOVE


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
# #---------ANIMATION WORKFLOW
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

