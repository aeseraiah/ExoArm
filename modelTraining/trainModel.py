from sklearn_rvm.em_rvm import EMRVC
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from micromlgen import port
from sklearn.svm import SVC
import warnings
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import classification_report, confusion_matrix 

#
#  SPECIFY NUMBER OF TRAINING DATA FILES #################################
#

TRAINING_DATA_COUNT = 3
window_size = 90

excel_file = "../data/filenames-indexes.xlsx"
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

# TURN DATA INTO NUMPY ARRAY
np_data = []
for curr_array in data:
    np_data.append(curr_array.iloc[:,0].to_numpy())

# ENSURE DATA SEPARATED
for npd in np_data:
    plt.plot(npd)
plt.title("ALL TRAINING SIGNALS")
plt.show()

# 
# SHOW THE SEPARATION BETWEEN FEATURES #################################
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

# classifiers
flexion = 0
extension = 1
sustain = 2
rest = 3

# SPLIT DATA INTO TRAINING/TESTING
STOP_TRAIN = (6*len(FLEXION_DATA))//10
# print(STOP_TRAIN)
features = FLEXION_DATA[0:STOP_TRAIN] + EXTENSION_DATA[0:STOP_TRAIN] + SUSTAIN_DATA[0:STOP_TRAIN] + REST_DATA[0:STOP_TRAIN]

# FILL LABEL ARRAY
LABEL = []
for i in range(0, STOP_TRAIN):
    LABEL.append(flexion)

for i in range(0, STOP_TRAIN):
    LABEL.append(extension)

for i in range(0, STOP_TRAIN):
    LABEL.append(sustain)

for i in range(0, STOP_TRAIN):
    LABEL.append(rest)

#
#
# INIT MODEL AND FIT FEATURES/LABELS #################################
#
#

# model = SVC(kernel='rbf', gamma=100, decision_function_shape='ovr')
# model = SVC(kernel='sigmoid', gamma=.0000001)
# model.fit(features, LABEL)

# FILL PREDICITON DATA ARRAY AND ANSWER ARRAY
X = FLEXION_DATA[STOP_TRAIN:] + EXTENSION_DATA[STOP_TRAIN:] + SUSTAIN_DATA[STOP_TRAIN:] + REST_DATA[STOP_TRAIN:] 

answ = []
for i in range(0, len(FLEXION_DATA[STOP_TRAIN:])):
    answ.append(flexion)

for i in range(0, len(FLEXION_DATA[STOP_TRAIN:])):
    answ.append(extension)

for i in range(0, len(FLEXION_DATA[STOP_TRAIN:])):
    answ.append(sustain)

for i in range(0, len(FLEXION_DATA[STOP_TRAIN:])):
    answ.append(rest)

#
#
# # MAKE PREDICTION #################################
# prediction = model.predict(X)

# # CHECK PREDICTION
# if prediction.tolist() != answ:
#     print("\nFAIL!!  ")
#     print("\nPREDICTION: ",prediction)
#     print("SHOULD BE:  ",np.array(answ))
# else:
#     print("PREDICTION:",prediction)

# print("SCORE:     ", model.score(X, answ))


# c_code = port(model)
# # print(c_code)

warnings.filterwarnings("ignore")
# kernels = ["linear", "rbf", "poly"]
# gammas = [0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0.00000001, 0.000000001]
# cs = [0.01, 0.1, 1, 10, 100, 1000]
# degrees = [0, 1, 2, 3, 4, 5, 6]

# iterations = len(kernels) * len(gammas) * len(cs) * len(degrees)
# i = 0
# for degree in degrees:
#     for c in cs:
#         for gamma in gammas:
#             for kernel in kernels:
#                 svc = SVC(kernel=kernel, gamma=gamma, C=c, degree=degree, verbose=False, max_iter=10000000).fit(features, LABEL)
#                 SCORE = svc.score(X, answ)
#                 if(SCORE>.4584):
#                     print("SCORE:",SCORE,kernel,gamma,c,degree)
#                 i+=1
#                 if(i%50==0):
#                     print("ITERATION:",i,"/",iterations)

# print("DONE")
# 'rbf', 'poly', 'sigmoid', 


param_grid = {'degree': [0, 1, 2, 3, 4], 'C': [0.0001,0.00001,0.0000001,0.1,1], 'gamma': [0.01,0.1,0.01,0.001, 0.000001],'kernel': ['linear', 'rbf', 'poly'], 'max_iter': [10000000]}
grid = GridSearchCV(SVC(),param_grid,refit=True)
grid.fit(features,LABEL)
print(grid.best_estimator_)
print("\nscore\n",grid.best_estimator_.score(X,answ))
grid_predictions = grid.predict(X)
print(confusion_matrix(answ,grid_predictions))
print(classification_report(answ,grid_predictions))