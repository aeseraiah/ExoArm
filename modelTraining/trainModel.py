from sklearn_rvm.em_rvm import EMRVC
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from micromlgen import port
from sklearn.svm import SVC

data = pd.read_csv("../data/12-1-data.txt", delimiter='\t')

# plt.plot(data)
# plt.show()

# COLUMN NAME: 2
flexion_startindexes = [391, 773, 1140, 1515, 1881, 2252, 2638, 3000, 3412, 3757, 4133]
extension_startindexes = [551, 917, 1293, 1675, 2051, 2437, 2824, 3200, 3582, 3937, 4319]
window_size = 100

# COLUMN NAME: 34
# flexion_startindexes = [400, 535, 643, 751, 1159, 1328, 1426]
# extension_startindexes = [457, 562, 693,  791, 1193, 1406, 1460]
# window_size = 40

# TURN DATA INTO NUMPY ARRAY
np_data = data['2'].to_numpy()

# plt.plot(np_data, color = 'blue')
# for i in extension_startindexes:
#     plt.plot(np_data[ i : i + window_size], color = 'green')
# for i in flexion_startindexes:
#     plt.plot(np_data[ i : i + window_size], color = 'red')
# plt.title("FLEXION - RED , EXTENSION - GREEN")
# plt.show()


# GENERATE FLEXION ARRAY AND EXTENSION ARRAY
FLEXION_DATA = []
for i in flexion_startindexes:
    FLEXION_DATA.append(np_data[ i : i + window_size])

EXTENSION_DATA = []
for i in extension_startindexes:
    EXTENSION_DATA.append(np_data[ i : i + window_size])

# classifiers
flexion = 0
extension = 1

STOP_TRAIN = 6
features = FLEXION_DATA[0:STOP_TRAIN] + EXTENSION_DATA[0:STOP_TRAIN]

# y1 = [flexion, flexion, flexion, flexion, extension, extension, extension, extension]
y1 = []
for i in range(0, STOP_TRAIN):
    y1.append(flexion)

for i in range(0, STOP_TRAIN):
    y1.append(extension)

model = SVC(kernel='rbf', gamma=.0000000001)
model.fit(features, y1)

# X = [[0,0,350,320, 0,0]]
X = FLEXION_DATA[STOP_TRAIN:] + EXTENSION_DATA[STOP_TRAIN:]
prediction = model.predict(X)
answ = [flexion, flexion, flexion, flexion, flexion, extension, extension, extension, extension, extension]
if prediction.tolist() != answ:
    print("\nFAIL!!  ")
    print("\nPREDICTION: ",prediction)
    print("SHOULD BE:   [0 0 0 0 0 1 1 1 1 1]")
else:
    print("PREDICTION:",prediction)

# print("\nPREDICTION: ", prediction)

print("SCORE: ", model.score(X, answ))


c_code = port(model)
print(c_code)