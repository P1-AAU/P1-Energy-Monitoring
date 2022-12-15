from heapq import merge
from re import S
import sys
import matplotlib
import pandas as pd
import matplotlib.pyplot as plt
import json
import datetime
from glom import glom

file_path = 'dishWasher.csv'
df = pd.read_csv(file_path)

with open('spotPrices.json','r') as f:
    data = json.loads(f.read())

df_tid = len(df) / 3600
print(df_tid)

time_ar = []
spot_ar = []
for record in data['records']:
    dt_str = record['HourDK']
    spot_str = record['SpotPriceDKK']
    dt = pd.to_datetime(dt_str)
    time = dt.time()
    print(time)
    print(spot_str)
    time_ar.append(time)
    spot_ar.append(spot_str)
df2 = pd.DataFrame({'Time': time_ar, 'SpotPriceDKK': spot_ar})
df2.plot(kind ='bar', x='Time', y='SpotPriceDKK')
plt.show()