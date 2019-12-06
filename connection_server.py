#from connection import server
from servertest import server
import threading

server = threading.Thread(target=server)
server.start()
