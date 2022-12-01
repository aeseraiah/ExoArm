from sklearn_rvm.em_rvm import EMRVC
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
# from micromlgen import port_rvm

data = pd.read_csv("../data/tmp.txt", delimiter='\t')

flexion_startindexes = [400, 535, 643, 751, 1159, 1328, 1426]
extension_startindexes = [457, 562, 693,  791, 1193, 1406, 1460]
window_size = 40

plt.plot(data['34'], color = 'blue')
for i in extension_startindexes:
    plt.plot(data['34'][ i : i + window_size], color = 'green')
for i in flexion_startindexes:
    plt.plot(data['34'][ i : i + window_size], color = 'red')
plt.title("FLEXION - RED , EXTENSION - GREEN")
plt.show()

# TURN DATA INTO NUMPY ARRAY
np_data = data['34'].to_numpy()

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

features = FLEXION_DATA[0:4] + EXTENSION_DATA[0:4]

y1 = [flexion, flexion, flexion, flexion, extension, extension, extension, extension]

model = EMRVC(kernel='rbf', gamma="scale")
model.fit(features, y1)

# X = [[0,0,350,320, 0,0]]
X = FLEXION_DATA[4:] + EXTENSION_DATA[4:]
prediction = model.predict(X)
answ = [flexion, flexion, flexion, extension, extension, extension]
if prediction.tolist() != answ:
    print("\nFAIL!!  ")
    print("\nPREDICTION: ",prediction)
    print("SHOULD BE:   [0 0 0 1 1 1]")
else:
    print("PREDICTION:",prediction)

print("SCORE: ", model.score(X, answ))