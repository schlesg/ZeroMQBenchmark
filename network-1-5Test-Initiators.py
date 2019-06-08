import subprocess
import os

# print(os.getcwd())
# os.chdir("build/")
commandList = []
commandList.append("./ZMQInitiator --PubEP tcp://192.168.1.105:4242,tcp://192.168.1.105:4243,tcp://192.168.1.105:4244,tcp://192.168.1.105:4245,tcp://192.168.1.105:4246 --SubEP tcp://192.168.1.102:4241 --msgLength 10000")


for command in commandList:
    subprocess.Popen(command.split(), cwd="build/")


input("Press Enter to shutdown...\n")
os.system("pkill -f ZMQ")
