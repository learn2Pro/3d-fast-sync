
buffer = []

def append(rgb,depth,ts):
    buffer.append((rgb,depth,ts))

def popHead():
    return buffer.pop(0)