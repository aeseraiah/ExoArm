from sklearn_rvm.em_rvm import EMRVC
import pandas as pd
# from micromlgen import port_rvm

# data = pd.read_csv()

# classifiers
flexion = 0
extension = 1

feature1 = [[0,0,300,300, 0,0],[0,0,300,400, 0,0],[0,0,500,500, 0,0],[0,0,500,600, 0,0]]
y1 = [0, 0, 1, 1]
# trainingData = 
# testingData = 

model = EMRVC(kernel='rbf', gamma="auto")
model.fit(feature1, y1)

X = [[0,0,350,320, 0,0]]
prediction = model.predict(X)
print(prediction)

