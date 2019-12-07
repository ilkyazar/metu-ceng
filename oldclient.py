import json, socket, pickle
import threading

class Client(threading.Thread):
    clientSocket = None

    def __init__(self, host, port):
        threading.Thread.__init__(self)

        self.clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.connect(host, port)

        #print ("New connection added: ", clientAddress)

    def __del__(self):
        self.close()

    def connect(self, host, port):
        self.clientSocket.connect((host, port))
        return self

    def send(self, data):
        print("Sending", data, " to client socket", self.clientSocket)

        if not self.clientSocket:
            raise Exception('Connect before sending')

        try:
            # Encode JSON to a string --> json.dumps()
            json_string = pickle.dumps(data)
        except (TypeError, ValueError) as e:
            raise Exception('Pickle exception')

        self.clientSocket.send(json_string)
        print("Sent", data)

        return self

    def recv(self):
        if not self.clientSocket:
            raise Exception('Connect before receiving')

        print("Receving")

        reply = self.clientSocket.recv(1024)
        data = pickle.loads(reply)
        return data

    def close(self):
        if self.clientSocket:
            self.clientSocket.close()
            self.clientSocket = None

    def run(self):
        print ("Running")
        
        while True:
            board_data = self.recv()

            print("Recevied: ", board_data)

            username_requ = self.recv()

            print("Receive: ", username_requ)

            self.send(input())

