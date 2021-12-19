import requests
buffer = []
max_len = 100


def append(rgb, depth, ts):
    if(len(buffer) <= max_len):
        print('''===========>apend image buffer in rgb size:%d,depth size:%d,ts:%d===========>''' % (
            len(rgb), len(depth), ts))
        buffer.append((rgb, depth, ts))
    # else:
    #     print('''===========>exceed the buffer max length,skip===========>''')

def popHead():
    if len(buffer)==0:
        return "","",0
    else:
        return buffer.pop(0)

if __name__ == '__main__':
    ans = requests.get('http://127.0.0.1:5000/recent/image')
    print('''response of answser size:%s'''%(ans.text))
