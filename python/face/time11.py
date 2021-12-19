import time

import datetime
dt_ms = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
print(dt_ms)
#print(time.strftime())
print(time.localtime(time.time()))

