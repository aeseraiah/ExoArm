from sklearn_rvm.em_rvm import EMRVC
import pandas as pd
from micromlgen import port_rvm

data = pd.read_csv()

# trainingData = 
# testingData = 

model = EMRVC(kernel='rbf')
model.fit(X, y)
prediction = model.predict(X)

