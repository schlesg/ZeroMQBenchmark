import subprocess
import os

# print(os.getcwd())
# os.chdir("")
commandList = []
commandList.append("./ZMQEchoer --SubEP tcp://192.168.1.105:4242 --PubEP tcp://192.168.1.102:4241")
commandList.append("./ZMQEchoer --SubEP tcp://192.168.1.105:4243 --PubEP tcp://192.168.1.102:4241")
commandList.append("./ZMQEchoer --SubEP tcp://192.168.1.105:4244 --PubEP tcp://192.168.1.102:4241")
commandList.append("./ZMQEchoer --SubEP tcp://192.168.1.105:4245 --PubEP tcp://192.168.1.102:4241")
commandList.append("./ZMQEchoer --SubEP tcp://192.168.1.105:4246 --PubEP tcp://192.168.1.102:4241")

# commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1,ipc:///tmp/pingTopic2,ipc:///tmp/pingTopic3,ipc:///tmp/pingTopic4,ipc:///tmp/pingTopic5 --SubEP ipc:///tmp/pongTopic --msgLength 10000")


# commandList.append("./initiator --SubTopic C5 --PubTopic MS1 --msgLength 5000 --roundtripCount 1000") #C5

for command in commandList:
    subprocess.Popen(command.split())


input("Press Enter to shutdown...\n")
os.system("pkill -f ZMQ")
