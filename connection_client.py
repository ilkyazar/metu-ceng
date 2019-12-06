#from connection import client
from clienttest import client
import threading

client = threading.Thread(target = client)
client.start()
