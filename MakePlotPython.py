from heapq import merge
from re import S
import sys
import matplotlib
import pandas as pd
import matplotlib.pyplot as plt
import json
from datetime import timedelta
import numpy

with open('spotPrices.json','r') as f:
    data = json.loads(f.read())

file_path = 'dishWasher.txt'
df = pd.read_csv(file_path)
print(df.values.ravel())
i = 0
df_ar = df.values.ravel()
timewatt_ar = []
for x in df.iterrows():
    tid = pd.to_datetime(i, unit='s', errors='coerce')
    timewatt_ar.append(tid.time())
    i=i+1

print(timewatt_ar)
print(df_ar)

time_ar = []
spot_ar = []
for record in data['records']:
    dt_str = record['HourDK']
    spot_str = record['SpotPriceDKK']
    dt = pd.to_datetime(dt_str)
    time = dt.time()
    time_ar.append(time)
    spot_ar.append(spot_str)
df = pd.DataFrame({'Time': timewatt_ar, 'Wattage': df_ar})
df.plot()
plt.show()
plt.savefig('dishWasher.png')

df2 = pd.DataFrame({'Time': time_ar, 'SpotPriceDKK': spot_ar})
df2.plot(kind ='bar', x='Time', y='SpotPriceDKK')
plt.tight_layout()
plt.show()
plt.savefig('spotPrices.png')
