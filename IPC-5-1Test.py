import subprocess
import os

# print(os.getcwd())
# os.chdir("build/")
commandList = []
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/pingTopic1 --PubEP ipc:///tmp/pongTopic1")

msgLen = '10000'
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --SubEP ipc:///tmp/pongTopic1 --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --pubName Dummy --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --pubName Dummy --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --pubName Dummy --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --pubName Dummy --msgLength " + msgLen)

# commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --SubEP ipc:///tmp/pongTopic3 --msgLength 50")
# commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --SubEP ipc:///tmp/pongTopic4 --msgLength 50")
# commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --SubEP ipc:///tmp/pongTopic5 --msgLength 50")


# commandList.append("./initiator --SubTopic C5 --PubTopic MS1 --msgLength 5000 --roundtripCount 1000") #C5

for command in commandList:
    subprocess.Popen(command.split(), cwd="build/")


input("Press Enter to shutdown...\n")
os.system("pkill -f ZMQ")
