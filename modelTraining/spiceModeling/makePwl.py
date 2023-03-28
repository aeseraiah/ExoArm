import pandas as pd

#data_file = "~/Desktop/engrCapst/ExoArm/data/lpdata/CoolTerm Capture 2023-03-28 10-41-00.txt"
data_file = "/Users/nikol/Desktop/engrCapst/ExoArm/data/CoolTerm Capture 2023-03-22 19-36-43.txt"
data = pd.read_csv(data_file, delimiter='\t', header=None)

emgdata = data[1]

pwldata = {}
with open('dataPWL.txt', 'w') as f:
	for i, x in enumerate(emgdata):
		f.write(f'{i*30}m\t{x}m\n') # every point is 30ms
print(f'{i*30} ms')
