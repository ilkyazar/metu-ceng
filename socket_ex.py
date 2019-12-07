# A thread per connection stream service and clients
import time
import random
import threading 
import socket

def echoservice(sock):
    ''' echo uppercase string back in a loop'''
    req = sock.recv(1000)
    while req and req != '':
        # remove trailing newline and blanks
        req = req.rstrip()
        sock.send(req.decode().upper().encode())
        req = sock.recv(1000)
    print(sock.getpeername(), ' closing')

def client(n, port):
    # send n random request
    # the connection is kept alive until client closes it.
    mess = ['hello', 'bye', 'why', 'yes', 'no', 'maybe', 'are you sure', 'why not?']
    c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    c.connect(('127.0.0.1', port))
    for i in range(n):
        #time.sleep(random.random()*3)
        c.send(random.choice(mess).encode())
        reply = c.recv(1024)
        print(c.getsockname(), reply)
    #c.close()
        
def server(port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('',port))
    s.listen(1)    # 1 is queue size for "not yet accept()'ed connections"
    try:
        #while True:
        for i in range(5):    # just limit # of accepts for Thread to exit
            ns, peer = s.accept()
            print(peer, "connected")
            # create a thread with new socket
            t = threading.Thread(target = echoservice, args=(ns,))
            t.start()
            # now main thread ready to accept next connection
    finally:
        s.close()
        
    

server = threading.Thread(target=server, args=(20445,))
server.start()

