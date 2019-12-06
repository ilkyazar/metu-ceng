import json, socket, pickle

class Client(object):
    clientSocket = None

    def __del__(self):
        self.close()

    def connect(self, host, port):
        self.clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.clientSocket.connect((host, port))
        return self

    def send(self, data):
        if not self.clientSocket:
            raise Exception('Connect before sending')

        try:
            # Encode JSON to a string --> json.dumps()
            json_string = pickle.dumps(data)
        except (TypeError, ValueError) as e:
            raise Exception('Only send JSON-serializable data')

        self.clientSocket.send(json_string)

        return self

    def recv(self):
        if not self.clientSocket:
            raise Exception('Connect before receiving')

        reply = self.clientSocket.recv(1024)
        data = pickle.loads(reply)
        return data

    def close(self):
        if self.clientSocket:
            self.clientSocket.close()
            self.clientSocket = None


