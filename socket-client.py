from socket_ex import client
import threading

clientThread = threading.Thread(target = client, args=(5, 20445))
clientThread.start()