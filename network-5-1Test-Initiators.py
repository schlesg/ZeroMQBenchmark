import subprocess
import os

# print(os.getcwd())
# os.chdir("build/")
commandList = []

msgLen = '10000'
commandList.append("./ZMQInitiator --PubEP tcp://192.168.1.105:4242 --SubEP tcp://192.168.1.102:4241 --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP tcp://192.168.1.105:4242 --pubName Dummy --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP tcp://192.168.1.105:4242 --pubName Dummy --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP tcp://192.168.1.105:4242 --pubName Dummy --msgLength " + msgLen)
commandList.append("./ZMQInitiator --PubEP tcp://192.168.1.105:4242 --pubName Dummy --msgLength " + msgLen)


for command in commandList:
    subprocess.Popen(command.split(), cwd="build/")


input("Press Enter to shutdown...\n")
os.system("pkill -f ZMQ")
