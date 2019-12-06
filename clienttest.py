from client import Client

def client():
    host = 'localhost'
    port = 8000

    client = Client()
    client.connect(host, port)

    hello_message = client.recv()
    print("hello msg from server", hello_message)

    client.send("{'some_list': [123, 456]}")
    response = client.recv()
    print('Response is: ', response)


    client.close()
