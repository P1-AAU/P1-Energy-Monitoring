from heapq import merge
from re import S
import sys
import matplotlib
import pandas as pd
import matplotlib.pyplot as plt
import json
from datetime import timedelta
import numpy

with open('../spotPrices.json','r') as f:
    data = json.loads(f.read())

file_path = '../dishWasher.txt'
df = pd.read_csv(file_path)
rawdishdf = pd.read_csv("../dishWasherTime.txt", header=None)
rawdishdf = rawdishdf.values.ravel()
i = 0
df_ar = df.values.ravel()
timewatt_ar = []
for x in df.iterrows():
    tid = pd.to_datetime(i + rawdishdf[0], unit='s', errors='coerce')
    timewatt_ar.append(tid.time())
    i=i+1

time_ar = []
spot_ar = []
for record in data['records']:
    dt_str = record['HourDK']
    spot_str = record['SpotPriceDKK']
    dt = pd.to_datetime(dt_str)
    time = dt.time()
    time_ar.append(time)
    spot_ar.append(spot_str / 1000)

df1 = pd.DataFrame({'Time2': timewatt_ar, 'Wattage': df_ar})
df2 = pd.DataFrame({'Time': time_ar, 'SpotPriceDKK': spot_ar})

fig, ax = plt.subplots(nrows=1, ncols=1)

ax2 = ax.twinx()

df1.plot(x='Time2', y='Wattage', label='DataFrame 1', color='blue', ax=ax, kind='line')
df2.plot(x='Time', y='SpotPriceDKK', label='DataFrame 2', color='red', ax=ax2, kind='line')

ax2.set_ylim(0, 5)

plt.show()