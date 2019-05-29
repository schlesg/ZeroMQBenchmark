import subprocess
import os

# print(os.getcwd())
# os.chdir("build/")
commandList = []
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/pingTopic1 --PubEP ipc:///tmp/pongTopic")
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/pingTopic2 --PubEP ipc:///tmp/pongTopic")
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/pingTopic3 --PubEP ipc:///tmp/pongTopic")
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/pingTopic4 --PubEP ipc:///tmp/pongTopic")
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/pingTopic5 --PubEP ipc:///tmp/pongTopic")

commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1,ipc:///tmp/pingTopic2,ipc:///tmp/pingTopic3,ipc:///tmp/pingTopic4,ipc:///tmp/pingTopic5 --SubEP ipc:///tmp/pongTopic --msgLength 10000")


# commandList.append("./initiator --SubTopic C5 --PubTopic MS1 --msgLength 5000 --roundtripCount 1000") #C5

for command in commandList:
    subprocess.Popen(command.split(), cwd="build/")


input("Press Enter to shutdown...\n")
os.system("pkill -f ZMQ")
