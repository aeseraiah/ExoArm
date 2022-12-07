from sklearn_rvm.em_rvm import EMRVC
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from micromlgen import port
from sklearn.svm import SVC

data = pd.read_csv("../data/12-1-data.txt", delimiter='\t')
data1 = pd.read_csv("../data/CoolTerm Capture 2022-12-06 20-02-36.txt")

# plt.plot(data)
# plt.show()

# COLUMN NAME: 2
flexion_startindexes = [391, 773, 1140, 1515, 1881, 2252, 2638, 3000, 3412, 3757, 4133]
sustain_startindexes = [470, 860, 1220, 1575, 1975,2375,2730,3103, 3490, 3839, 4210]
extension_startindexes = [551, 917, 1293, 1675, 2051, 2437, 2824, 3200, 3582, 3937, 4319]
window_size = 90
rest_startindexes = [i + window_size for i in extension_startindexes]


# COLUMN NAME: 34
# flexion_startindexes = [400, 535, 643, 751, 1159, 1328, 1426]
# extension_startindexes = [457, 562, 693,  791, 1193, 1406, 1460]
# window_size = 40

# TURN DATA INTO NUMPY ARRAY
np_data = data['2'].to_numpy()

# plt.plot(np_data, color = 'blue')
# plt.plot(data['2'], color = 'blue')

# for i in extension_startindexes:
#     plt.plot(data['2'][ i : i + window_size], color = 'green')
# for i in flexion_startindexes:
#     plt.plot(data['2'][ i : i + window_size], color = 'red')
# for i in sustain_startindexes:
#     plt.plot(data['2'][ i : i + window_size], color = 'purple')
# for i in rest_startindexes:
#     plt.plot(data['2'][ i : i + window_size], color = 'orange')
# plt.title("FLEXION - RED , EXTENSION - GREEN, SUSTAIN - PURPLE, REST - ORANGE")
# plt.show()


# GENERATE FLEXION ARRAY AND EXTENSION ARRAY
FLEXION_DATA = []
for i in flexion_startindexes:
    FLEXION_DATA.append(np_data[ i : i + window_size])

EXTENSION_DATA = []
for i in extension_startindexes:
    EXTENSION_DATA.append(np_data[ i : i + window_size])

SUSTAIN_DATA = []
for i in sustain_startindexes:
    SUSTAIN_DATA.append(np_data[ i : i + window_size])

REST_DATA = []
for i in rest_startindexes:
    REST_DATA.append(np_data[ i : i + window_size])

# classifiers
flexion = 0
extension = 1
sustain = 2
rest = 3

STOP_TRAIN = 6
features = FLEXION_DATA[0:STOP_TRAIN] + EXTENSION_DATA[0:STOP_TRAIN] + SUSTAIN_DATA[0:STOP_TRAIN] + REST_DATA[0:STOP_TRAIN]

# y1 = [flexion, flexion, flexion, flexion, extension, extension, extension, extension]
y1 = []
for i in range(0, STOP_TRAIN):
    y1.append(flexion)

for i in range(0, STOP_TRAIN):
    y1.append(extension)

for i in range(0, STOP_TRAIN):
    y1.append(sustain)

for i in range(0, STOP_TRAIN):
    y1.append(rest)

model = SVC(kernel='rbf', gamma=.0000000001)
model.fit(features, y1)

# X = [[0,0,350,320, 0,0]]
X = FLEXION_DATA[STOP_TRAIN:] + EXTENSION_DATA[STOP_TRAIN:] + SUSTAIN_DATA[STOP_TRAIN:] + REST_DATA[STOP_TRAIN:] 
prediction = model.predict(X)
answ = [flexion, flexion, flexion, flexion, flexion, extension, extension, extension, extension, extension, sustain, sustain, sustain, sustain, sustain, rest, rest, rest, rest, rest]
if prediction.tolist() != answ:
    print("\nFAIL!!  ")
    print("\nPREDICTION: ",prediction)
    print("SHOULD BE:   [0 0 0 0 0 1 1 1 1 1 2 2 2 2 2 3 3 3 3 3]")
else:
    print("PREDICTION:",prediction)

# print("\nPREDICTION: ", prediction)

print("SCORE: ", model.score(X, answ))


c_code = port(model)
# print(c_code)




# TEST ON UNTRAINED DATAs
pred_array = np.array([])
# print(data1)
np_data1 = data1['87'].to_numpy()


pred_array = []
for i in range(0,len(np_data1)-window_size,window_size):
    # print(i)
    pred_array.append(np.array(np_data1[ i : i + window_size]))
# 
prediction = model.predict(pred_array)

print(prediction)

plt.plot(prediction)
plt.show()
