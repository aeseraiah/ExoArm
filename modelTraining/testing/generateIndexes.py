import pandas as pd

def gen_index(MARKER_FILE, SHEET_NUMBER, data_file, window):

    markers = pd.read_csv(MARKER_FILE, header=None)

    excel_file = "../../data/filenames-indexes_hplp.xlsx"


if __name__ == '__main__':
    MARKER_FILE = '../markerData/hp_lp_data/14_57_50_04_19_23.csv'  # CHANGE EACH TIME, SHOULD NOT WORK O/W
    SHEET_NUMBER = 'Sheet2'                  # CHANGE EACH TIME, SHOULD NOT WORK O/W
    data_file = '../../data/lp_hp_Data/CoolTerm Capture 2023-04-19 14-57-55.txt'           # CHANGE EACH TIME, SHOULD NOT WORK O/W
    gen_index(MARKER_FILE,SHEET_NUMBER,data_file,window = 100)